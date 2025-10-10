/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disconnect_connection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 19:32:06 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/09 17:59:09 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"

int	disconnect_connection(struct s_server *srv, t_connection_id id)
{
	struct s_connection	*conn;

	if (!srv || id == 0)
		return (-1);
	conn = get_connection_by_id(&srv->connection_data, id);
	if (!conn)
		return (-1);
	if (srv->on_disconnect)
		srv->on_disconnect(srv, conn, srv->appdata);
	connection_remove_by_ref(&srv->connection_data, conn);
	return (0);
}
