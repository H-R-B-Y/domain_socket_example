/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_disconnect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:06:35 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/11 12:44:18 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "sock_server_int.h"

static int	cmp_conn(void *conn1, void *conn2)
{
	return (conn1 - conn2);
}

int	_handle_disconnect(struct s_server *srv, struct s_connection *conn)
{
	// What do we need to do when a client disconnects
	// flush any and all read / write data (done in connection destroy)
	// remove from epoll (done in connection destroy)
	// remove from room 
	// remove from lookup (done in connection destroy)
	// close fd (done in connection destroy)
	struct s_cdll_node	*room_node;
	if (srv->on_disconnect)	
		srv->on_disconnect(srv, conn, srv->appdata);
	room_node = cdll_find(&srv->server_room, conn, cmp_conn);
	if (room_node)
		cdll_remove_node(&srv->server_room, room_node, 0);
	connection_remove_by_ref(&srv->connection_data, conn);
	return (0);
}
