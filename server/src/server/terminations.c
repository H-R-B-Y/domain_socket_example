/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminations.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 10:56:10 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/04 10:56:27 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	terminate_child(void *chld)
{
	struct s_connection *c;

	c = chld;
	if (!chld || !c->fd)
		return ;
	if (c->epoll_parent)
		epoll_ctl(c->epoll_parent, EPOLL_CTL_DEL, c->fd, NULL);
	close(c->fd);
	free(chld);
}

void	terminate_room(void *room)
{
	(void)room;
}

void terminate_srv(struct s_server *srv, const char *msg, t_u8 ret_code)
{
	if (!srv)
		_exit(ret_code);
	dprintf(STDERR_FILENO, "Terminating server (%d): %s\n", ret_code, msg);
	if (srv->rooms)
		cdll_delete(&srv->rooms, terminate_room);
	if (srv->connections)
		cdll_delete(&srv->connections, terminate_child);
	if (srv->epoll_fd)
		close(srv->epoll_fd);
	if (srv->server_fd)
	{
		close(srv->server_fd);
		unlink(SOCKET_PATH);
	}
	exit(ret_code);
}
