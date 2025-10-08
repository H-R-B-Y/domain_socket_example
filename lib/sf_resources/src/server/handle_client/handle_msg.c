/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_msg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:27:42 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/08 14:58:58 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_internal.h"

int	_handle_msg(struct s_server *srv, struct s_connection *conn)
{
	int					status;
	struct s_message	msg;

	if (!srv || !conn)
		return (1);
	status = partial_read_process(&conn->read_state, conn->fd);
	switch (status)
	{
		case 0:
			return (0);
			break ;
		case 1:
			// we need some way to pop the message from the client into a message struct
			msg.sender = conn;
			msg.header = &conn->read_state.header;
			msg.content = conn->read_state.buffer;
			msg.recv_at = time(NULL);
			if (srv->on_message)
				srv->on_message(srv, &msg, srv->appdata);
			msg = (struct s_message){0};
			partial_read_reset(&conn->read_state);
			return (0);
			break ;
		case -1:
			return (-1);
			break ;
	}
}


