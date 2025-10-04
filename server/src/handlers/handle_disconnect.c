/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_disconnect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 11:01:01 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/04 11:02:27 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	remove_connection_for_fd(struct s_server *srv, int fd)
{
	size_t				idx;
	struct s_cdll_node	*node;

	node = srv->connections->head;
	idx = 0;
	while (idx < srv->connections->count)
	{
		if (node->data && ((struct s_connection *)node->data)->fd == fd)
		{
			if (srv->connections->count > 1)
			{
				if (node == srv->connections->head)
					srv->connections->head = node->next;
				if (node == srv->connections->tail)
					srv->connections->tail = node->prev;
				if (node->prev)
					node->prev->next = node->next;
				if (node->next)
					node->next->prev = node->prev;
			}
			else
			{
				srv->connections->head = 0;
				srv->connections->tail = 0;
			}
			node->next = 0;
			node->prev = 0;
			srv->connections->count -= 1;
			terminate_child(node->data);
			free(node);
			return (1);
		}
		node = node->next;
		idx++;
	}
	return (0);
}

int	handle_disconnect(struct s_server *srv, int fd)
{
	return (remove_connection_for_fd(srv, fd));
}
