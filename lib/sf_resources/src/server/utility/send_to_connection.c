/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_to_connection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 19:20:59 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/11 15:17:32 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"

int	send_to_connection_ref(
	struct s_server *srv, 
	struct s_connection *conn,
	struct s_header_chunk *header,
	void *content
)
{
	struct s_partial_write	*pw;
	int						register_for_write;

	if (!srv || !conn)
		return (0);
	pw = partial_write_create();
	partial_write_init(pw);
	ft_memmove(&pw->header, header, sizeof(*header));
	pw->buffer = ft_memdup(content, header->content_length);
	if (!pw->buffer)
	{
		partial_write_destroy(pw);
		return (-1);
	}
	pw->prechunk = 1;
	register_for_write = 1 * (conn->write_queue_head == 0);
	push_partial_write(&conn->write_queue_head, pw);
	if (register_for_write)
	{
		struct epoll_event ev;
		ev.data.fd = conn->fd;
		ev.events = EPOLLIN | EPOLLOUT;
		epoll_ctl(srv->epoll_fd, EPOLL_CTL_MOD, conn->fd, &ev);
		// what do we do if this fails, right now just assume that it works.
	}
	return (0);
}

int	send_to_connection_id(
	struct s_server *srv,
	t_connection_id id,
	struct s_header_chunk *header,
	void *content
)
{
	struct s_connection	*conn;

	if (!srv || id == 0)
		return (0);
	conn = get_connection_by_id(&srv->connection_data, id);
	if (!conn)
		return (-1);
	return (send_to_connection_ref(srv, conn, header, content));
}
