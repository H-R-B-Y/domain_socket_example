/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_msg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 11:00:28 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/04 11:30:36 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"


void	broadcast_message(struct s_server *srv, char *buffer, size_t size)
{
	struct s_cdll_node *node;
	size_t		idx;
	struct s_header_chunk header= {0};
	
	idx = 0;
	node = srv->connections->head;
	header.content_length = size;
	header.msg_type = MTYPE_STR;
	while (idx < srv->connections->count)
	{
		int fd = ((struct s_connection *)(node->data))->fd;
		int v = 1;
		write(fd, &v, sizeof(int));
		write(fd, &header, sizeof(header));
		write(
			fd,
			buffer,
			size
		);
		node = node->next;
		idx++;
	}
}

int	handle_msg(struct s_server *srv, int fd)
{
	(void)srv;
	char					empty;
	int						prechunk;
	struct s_header_chunk	header;
	int						status;

	status = read(fd, (void *)&prechunk, PRE_HEADER_CHUNK);
	if (status == 0)
		return (-1);
	else if (status < 0)
		return (-1);
	printf("Prechunk is %d\n", prechunk);
	if (prechunk != 1)
		return (1);
	status = read(fd, (void *)&header, sizeof(header));
	if (status == 0)
		return (-1);
	else if (status < 0)
		return (-1);
	else if (status < (long int)sizeof(header))
		return (-1);
	printf("Reading message content\n");
	if (header.msg_type == MTYPE_NONE)
	{
		while (--header.content_length)
		{
			read(fd, &empty, sizeof(char));
		}
		return (0);
	}
	else if (header.msg_type == MTYPE_STR)
	{
		char *buffer;
		buffer = ft_calloc(header.content_length + 1, sizeof(char));
		status = read(fd, buffer, header.content_length);
		if (status == 0)
			return (-1);
		else if (status < 0)
			return (-1);
		else if (status < (ssize_t)header.content_length)
			return (-1);
		buffer[header.content_length] = '\0';
		printf("Content: %s\n", buffer);
		broadcast_message(srv, buffer, header.content_length);
		free(buffer);
	}
	return (0);
}
