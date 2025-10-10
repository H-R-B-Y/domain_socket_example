/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate_srv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:56:32 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 13:54:50 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"

void	_terminate_srv(struct s_server *srv,\
	const char *msg,
	int exit_code,
	int show_errno
)
{
	if (!srv)
		return ;
	if (show_errno)
		dprintf(STDERR_FILENO, "Server terminating with errno %s\nReason: %s\n", strerror(errno), msg);
	else
		dprintf(STDERR_FILENO, "Server terminating: %s\n", msg);
	// Not sure if this is a good idea because termiante is internal
	if (srv->appdata && srv->free_appdata)
		srv->free_appdata(srv->appdata);
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
	exit(exit_code);
}
