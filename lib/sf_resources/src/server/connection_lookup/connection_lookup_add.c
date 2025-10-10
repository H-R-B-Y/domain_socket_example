/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection_lookup_add.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 15:15:31 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/09 17:59:09 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"

t_connection_id connection_add(struct s_connection_lookup *lookup, struct s_connection *conn)
{
	t_connection_id *id;

	if (!lookup || !conn)
		return (0);
	// Assign an ID to the connection
	if (lookup->available_ids.size > 0)
	{
		id = ((t_connection_id *)heap_pop(&lookup->available_ids));
		// then just use the next one
		if (!id)
			conn->id = lookup->next_id++;
		else
			conn->id = *id;
	}
	else
		conn->id = lookup->next_id++;
	// Add to both hashmaps
	hm_add_value(&lookup->id_to_connection, &conn->id, conn);
	hm_add_value(&lookup->fd_to_connection, &conn->fd, conn);
	// Add to the connections list
	cdll_push_back(&lookup->connections, cdll_init_node(conn));
	return (conn->id);
}
