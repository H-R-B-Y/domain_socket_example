/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:31:37 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/08 16:43:13 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_internal.h"

int	_handle_write(struct s_server *srv, struct s_connection *conn)
{
	int						status;

	if (!srv || !conn)
		return (1);
	/*
	Check if there is data queued
	if not remove the write check from the epoll event.
	*/
	if (!conn->write_queue_head)
	{
		// should really make this a function
		struct epoll_event	ev;
		ev.data.fd = conn->fd;
		ev.events = EPOLLIN;
		if (epoll_ctl(conn->epoll_parent, EPOLL_CTL_MOD, conn->fd, &ev) < 0)
			return (-1);
	}
	status = partial_write_process(conn->write_queue_head, conn->fd);
	switch (status)
	{
		case 0:
			// Still more of the message to write, wait until next epoll
			return (0);
			break ;
		case 1:
			// message has been written completely
			partial_write_destroy(pop_partial_write(&conn->write_queue_head));
			struct epoll_event ev;
			ev.data.fd = conn->fd;
			ev.events = EPOLLIN;
			epoll_ctl(srv->epoll_fd, EPOLL_CTL_MOD, conn->fd, &ev);
			return (0);
			break ;
		case -1:
			return (-1);
			break ;
	}
	return (1);
}
