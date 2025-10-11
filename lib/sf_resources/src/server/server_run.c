/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_run.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 14:53:31 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/11 12:41:21 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"

/**
 * @brief Run the server main loop
 * This will block and run the server main loop, handling connections and messages.
 * It will return when the server is shutdown.
 * @param srv The server structure to run
 * @return void
 */
void	server_run(struct s_server *srv)
{
	struct epoll_event	ev[200];
	int					status;
	int					idx;

	srv->running = 1;
	while (srv->running)
	{
		status = epoll_wait(srv->epoll_fd, ev, 200, -1);
		if (status < 0)
		{
			printf("EPOLL: error code: %s\n", strerror(errno));
			if (errno != EAGAIN && errno != EWOULDBLOCK)
				server_stop(srv);
		}
		else if (status == 0)
			continue ;
		idx = 0;
		while (idx < status)
		{
			if (ev[idx].data.fd == srv->server_fd)
			{
				switch (_on_join_handler(srv, ev[idx].data.fd))
				{
					case 0:
						break ; // OK case 
					case -1:
						printf("CONNECTION: error code: %s\n", strerror(errno));
						// Should probably figure out what went wrong and do something about it.
						break ;
					case 1:
						printf("CONNECTION: Internal error occured\n");
						// Should probably do something safe here, might not need to terminate, just recover
						break ;
				}
			}
			else if (ev[idx].data.fd == STDIN_FILENO)
			{
				switch (_handle_terminal(srv))
				{
					case 0:
						break ;
					case -1:
						printf("TERMINAL: error code: %s\n", strerror(errno));
						break ;
					case 1:
						printf("TERMINAL: Internal error occured\n");
						break ;
				}
			}
			else
			{
				struct s_connection	*conn;
				conn = get_connection_by_fd(&srv->connection_data, ev[idx].data.fd);
				if (!conn)
				{
					printf("Recieved message from client that is not in connection lookup\n"
						"Saving data for next loop"
					);
					idx++;
					continue ;
				}
				/*
				TODO: define what to do when these dont return good codes
				*/
							// TODO: move all the error prints to stdout
				if (ev[idx].events & EPOLLHUP || ev[idx].events & EPOLLRDHUP)
				{
					switch (_handle_disconnect(srv, conn))
					{
						case 0:
							// OK
							break ;
						case -1:
							printf("DISCONNECT: error code while disconnecting %d: %s\n", ev[idx].data.fd, strerror(errno));
							break ;
						case 1:
							printf("DISCONNECT: Internal error when disconnecting %d\n", ev[idx].data.fd);
							break ;
						}
					idx++;
					continue;
				}
				if (ev[idx].events & EPOLLIN)
				{
					switch (_handle_msg(srv, conn))
					{
						case 0:
							// OK
							break ;
						case -1:
							printf("MESSAGE: message from %d caused error code: %s\n", conn->fd, strerror(errno));
							break ;
						case 1:
							printf("MESSAGE: message from %d caused an internal error\n", conn->fd);
							break ;
					}
				}
				if (ev[idx].events & EPOLLOUT)
				{
					switch(_handle_write(srv, conn))
					{
						case 0:
							// OK
							break ;
						case -1:
							printf("WRITE: message to %d caused error code: %s\n", conn->fd, strerror(errno));
							break ;
						case 1:
							printf("WRITE: message to %d caused an internal error\n", conn->fd);
							break ;
					}
				}
			}
			idx++;
		}
	}
}
