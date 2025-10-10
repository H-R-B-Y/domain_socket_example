/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_join.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:34:29 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/09 17:59:09 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"

int	_on_join_handler(struct s_server *srv, int fd)
{
	int					status;
	struct s_connection	*conn;
	if (!srv || fd < 0)
		return (1);
	/*
	Dont know what the best order for this would be
	Accept the connection
	create a new connection struct to store the new client in
	add the new connection to the connection lookup
	add the connection to this room
	add the connection to epoll
	call the onjoin handler
	*/
	status = accept(srv->server_fd, NULL, NULL);
	if (status < 0)
		return (-1);
	conn = new_connection(status, srv->epoll_fd);
	if (!conn)
		return (-1);
	connection_add(&srv->connection_data, conn);
	cdll_push_back(&srv->server_room, cdll_init_node(conn));
	struct epoll_event	ev;
	ev.events = EPOLLIN;
	ev.data.fd = conn->fd;
	status = epoll_ctl(srv->epoll_fd, EPOLL_CTL_ADD, conn->fd, &ev);
	if (status < 0)
		return (-1); // should we cleanup the connection here?
	if (srv->on_connect)
		srv->on_connect(srv, conn, srv->appdata);
	return (0);
}

