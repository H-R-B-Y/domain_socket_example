/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_shutdown.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 14:53:52 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/09 17:59:09 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"

/*
Note to self:
This will also need to cleanup the threads, so we need some way
to communicate with the threads to tell them to exit.
and we should ideally wait for them to exit before we free the server and
its contents.
*/

void	server_shutdown(struct s_server *srv)
{
	if (!srv)
		return ;
	// Cleanup connections in room
	cdll_destroy(&srv->server_room, NULL); // No need to free connection here
	// Close all connections
	connection_lookup_destroy(&srv->connection_data);
	if (srv->free_appdata && srv->appdata)
		srv->free_appdata(srv->appdata);
	// Close the server socket
	if (srv->server_fd >= 0)
		close(srv->server_fd);
	// Close the epoll instance
	if (srv->epoll_fd >= 0)
		close(srv->epoll_fd);
	// Remove the socket file
	unlink(srv->addr.sun_path);
	// Free any other resources here
	return ;
}

void	server_stop(struct s_server *srv)
{
	if (!srv)
		return ;
	if (!srv->running)
	{
		dprintf(STDERR_FILENO, "Server is not running\n");
		return ;
	}
	srv->running = 0;
	return ;
}
