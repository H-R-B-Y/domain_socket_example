/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_to_connection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 19:20:59 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/05 19:22:42 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_internal.h"

int	server_send_to_connection(
	struct s_server *srv,
	t_connection_id id,
	struct s_header_chunk *header,
	void *content,
	size_t size
)
{
	struct s_connection	*conn;
	ssize_t				sent_bytes;
	size_t				total_bytes;
	size_t				to_send;
	char				*send_buffer;

	if (!srv || id == 0)
		return (0);
	conn = get_connection_by_id(&srv->connection_data, id);
	if (!conn)
		return (0);
	if (!header || (header->content_length > 0 && (!content || size == 0)))
		return (0);
	// Send the prechunk
	to_send = sizeof(int);
	sent_bytes = send(conn->fd, &to_send, sizeof(int), 0);
	if (sent_bytes != sizeof(int))
		return (-1);
	// Send the header
	to_send = sizeof(struct s_header_chunk);
	sent_bytes = send(conn->fd, header, to_send, 0);
	if (sent_bytes != (ssize_t)to_send)
		return (-1);
	// Send the content if there is any
	if (header->content_length > 0)
	{
		total_bytes = 0;
		send_buffer = (char *)content;
		while (total_bytes < header->content_length)
		{
			to_send = header->content_length - total_bytes;
			sent_bytes = send(conn->fd, send_buffer + total_bytes, to_send, 0);
			if (sent_bytes <= 0)
				return (-1);
			total_bytes += sent_bytes;
		}
	}
	return (1);
}
