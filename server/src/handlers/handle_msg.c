/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_msg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 11:00:28 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/05 00:55:21 by hbreeze          ###   ########.fr       */
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

struct s_connection *get_connection(struct s_server *srv, int fd)
{
	struct s_cdll_node	*node;
	size_t				idx;

	idx = 0;
	node = srv->connections->head;
	while (idx < srv->connections->count)
	{
		if (((struct s_connection *)node->data)->fd == fd)
			return (node->data);
		idx++;
		node = node->next;
	}
	return (0);
}

int	handle_msg(struct s_server *srv, int fd)
{
	// First thing we need to do is get the connection
	// from the cdll
	struct s_connection *conn;

	conn = get_connection(srv, fd);
	if (!conn)
	{
		dprintf(STDERR_FILENO, "If you see this, something bad has happend\n");
		// should not hit this case, but if we do we should probably remove the fd
		// that caused the error from the epoll list
		epoll_ctl(srv->epoll_fd, EPOLL_CTL_DEL, fd, 0);
		return (-1);
	}
	if (handle_message(srv, conn) <= 0)
	{
		dprintf(STDERR_FILENO, "Failed to recieve a message from fd %d\n", fd);
		// Should probably list out all the possible causes here
		// but for now we should just disconnect the client.
		handle_disconnect(srv, fd); // TODO: cleanup the disconnect, and make an overload that takes a connection
		return (-1);
	}
	if (conn->msg_complete != 1)
		return (0); // Just wait until more data is available on the socket
	/*
	Here we can either call the handler functions setup in the server
	srv->handlers[conn->header.msg_type](); or something like that
	*/
	if (conn->header.msg_type == MTYPE_STR)
	{
		broadcast_message(srv, conn->content_buffer, conn->header.content_length);
	}
	else
	{;}
	free(conn->content_buffer);
	conn->msg_complete = 0;
	return (1);
}
