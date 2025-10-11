/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_to_room.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 12:49:30 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/11 15:18:35 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"


// We just want to iterate through the list of connections and call send to client on each one
int		send_to_room(struct s_server *srv, t_cdll *room, struct s_header_chunk *header, void *content)
{
	size_t				idx;
	struct s_cdll_node	*node;

	if (!srv || !room || !header || !content)
		return (-1);
	idx = 0;
	node = room->head;
	while (idx < room->count)
	{
		send_to_connection_ref(
			srv,
			(struct s_connection *)node->data,
			header,
			content
		); // just assume right now that it worked
		// TODO: work out what to do on error
		// TODO: work out lifetime of message sent (right now the header and content are just copied so the ones passed to this function can be free'd but where is the question)
		node = node->next;
		idx++;
	}
	return (0);
}

