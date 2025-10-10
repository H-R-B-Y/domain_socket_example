/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_global.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 19:34:07 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/09 18:01:33 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"

/*
Again, really really bad implementation, 
TODO: implement partial write for non-blocking sockets
*/
int send_global_message(
	struct s_server *srv,
	struct s_header_chunk *header,
	void *content,
	size_t size
)
{
	struct s_cdll_node	*node;
	size_t				idx;
	int					result;

	if (!srv || !header || (header->content_length > 0 && (!content || size == 0)))
		return (-1);
	if (srv->connection_data.connections.count == 0)
		return (0);
	node = srv->connection_data.connections.head;
	idx = 0;
	result = 0;
	while (idx < srv->connection_data.connections.count)
	{
		if (node->data)
		{
			if (server_send_to_connection(srv, ((struct s_connection *)node->data)->
id, header, content, size) == -1)
				result = -1;
		}
		node = node->next;
		idx++;
	}
	return (result);
}

