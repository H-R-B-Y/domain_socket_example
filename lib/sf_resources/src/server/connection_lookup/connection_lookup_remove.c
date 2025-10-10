/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection_lookup_remove.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 19:08:14 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/09 17:59:09 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"

static int	compare_connection_ptrs(const void *a, const void *b)
{
	return (a == b);
}

int							connection_remove_by_ref(struct s_connection_lookup *lookup, struct s_connection *conn)
{
	struct s_cdll_node	*node;

	if (!lookup || !conn)
		return (0);
	hm_remove_pair(&lookup->id_to_connection, &conn->id);
	hm_remove_pair(&lookup->fd_to_connection, &conn->fd);
	// Add the id back to the available ids heap
	heap_insert(&lookup->available_ids, &conn->id);
	// Remove from the connections list
	node = cdll_find(&lookup->connections, conn, (void *)&compare_connection_ptrs);
	cdll_remove_node(&lookup->connections, node, (t_freefn)&delete_connection);
	delete_connection(conn);
	return (1);
}


int							connection_remove_by_id(struct s_connection_lookup *lookup, t_connection_id id)
{
	struct s_connection	*conn;

	if (!lookup || id == 0)
		return (0);
	conn = hm_get_value(&lookup->id_to_connection, &id);
	if (!conn)
		return (0);
	return (connection_remove_by_ref(lookup, conn));
}

int							connection_remove_by_fd(struct s_connection_lookup *lookup, int fd)
{
	struct s_connection	*conn;

	if (!lookup || fd < 0)
		return (0);
	conn = hm_get_value(&lookup->fd_to_connection, &fd);
	if (!conn)
		return (0);
	return (connection_remove_by_ref(lookup, conn));
}
