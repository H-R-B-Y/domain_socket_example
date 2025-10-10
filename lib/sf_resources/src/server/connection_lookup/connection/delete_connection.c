/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_connection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 15:03:43 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/09 17:59:09 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"

void	delete_connection(struct s_connection *conn)
{
	if (!conn)
		return ;
	partial_read_reset(&conn->read_state);
	if (conn->write_queue_head)
		partial_write_clear_list(&conn->write_queue_head);
	if (conn->cleanup_user_data && conn->user_data)
		conn->cleanup_user_data(conn->user_data);
	if (conn->epoll_parent)
		epoll_ctl(conn->epoll_parent, EPOLL_CTL_DEL, conn->fd, 0);
	if (conn->fd >= 0)
		close(conn->fd);
	free(conn);
}