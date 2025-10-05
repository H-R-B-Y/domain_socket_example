/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stateful_reading.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 00:16:40 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/05 00:55:55 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static int handle_prechunk(struct s_server *srv, struct s_connection *conn, ssize_t *status)
{
	(void)srv;
	(*status) = read(conn->fd, 
		((char *)&conn->prechunk),
		PRE_HEADER_CHUNK - conn->bytes_read
	);
	if ((*status) <= 0)
	{
		return (0);
	}
	conn->bytes_read += (*status);
	if (conn->bytes_read >= PRE_HEADER_CHUNK)
	{
		conn->read_state = READ_STATE_HEADER;
		conn->bytes_read = 0;
	}
	return (1);
}

static int	handle_header(struct s_server *srv, struct s_connection *conn, ssize_t *status)
{
	(void)srv;
	(*status) = read(conn->fd,
		((char *)&conn->header),
		HEADER_CHUNK - conn->bytes_read
	);
	if ((*status) <= 0)
	{
		return (0);
	}
	conn->bytes_read += (*status);
	if (conn->bytes_read >= HEADER_CHUNK)
	{
		conn->read_state = READ_STATE_CONTENT;
		conn->bytes_expected = conn->header.content_length;
		conn->bytes_read = 0;
		/*
		Ideally we can just move something like this towards
		an arena buffer or free list, just to keep memory management to a minimum.
		*/
		conn->content_buffer = ft_calloc(conn->bytes_expected + 1, sizeof(char));
	}
	return (1);
}

static int	handle_message_content(struct s_server *srv, struct s_connection *conn, ssize_t *status)
{
	(void)srv;
	(*status) = read(conn->fd,
		((char *)conn->content_buffer),
		conn->bytes_expected - conn->bytes_read
	);
	if ((*status) <= 0)
	{
		return (0);
	}
	conn->bytes_read += (*status);
	if (conn->bytes_read >= conn->bytes_expected)
	{
		conn->read_state = READ_STATE_PRECHUNK;
		conn->bytes_read = 0;
		conn->bytes_expected = 0;
		conn->msg_complete = 1;
	}
	return (1);
}

int	handle_message(struct s_server *srv, struct s_connection *conn)
{
	ssize_t	status;

	switch (conn->read_state)
	{
		case READ_STATE_PRECHUNK:
			if (handle_prechunk(srv, conn, &status) < 1)
				return (0); // This is a failed read state
			break ;
		
		case READ_STATE_HEADER:
			if (handle_header(srv, conn, &status) < 1)
				return (0);
			break ;
		
		case READ_STATE_CONTENT:
			if (handle_message_content(srv, conn, &status) < 1)
				return (0);
			break ;
	}
	return (1);
}
