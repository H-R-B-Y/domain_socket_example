/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 10:58:12 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/05 00:58:52 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	main_loop(struct s_server *srv)
{
	struct epoll_event	events[64];
	int					count;
	int					idx;
	int					status;

	if (!srv)
		return ;
	while (1)
	{
		if (g_signal_info != 0)
			return ;
		idx = 0;
		count = epoll_wait(srv->epoll_fd, events, 64, -1); // Note: we should probably have a timeout
		if (count == 0)
			continue ;
		else if (count < 0)
		{
			dprintf(STDERR_FILENO, "EPOLL error: %s\n", strerror(errno));
			continue ;
		}
		printf("Epoll events: %d\n", count);
		while (idx < count)
		{
			if (events[idx].data.fd == srv->server_fd)
				create_new_connection(srv);
			else if (events[idx].data.fd == STDIN_FILENO)
			{
				char *buff = get_next_line(STDIN_FILENO);
				if (ft_strcmp(buff, "EXIT\n") == 0)
				{
					free(buff);
					return ;
				}
				else
				{
					broadcast_message(srv, buff, ft_strlen(buff));
					printf("SERVER: %s", buff);
					free(buff);
				}
			}
			else 
			{
				status = handle_msg(srv, events[idx].data.fd);
				if (status < 0)
				{
					// client disconnected or was terminated by an error.
					// cleanup
					printf("Removing client from server: %d\n", events[idx].data.fd);
					remove_connection_for_fd(srv, events[idx].data.fd);
				}
			}
			idx++;
		}
	}
}
