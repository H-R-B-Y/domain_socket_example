/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_send_message.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:24:50 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/11 12:36:40 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_client_int.h"

int	client_send_message(
	struct s_client *client,
	int prechunk,
	struct s_header_chunk *header,
	void *content
)
{
	struct s_partial_write	*pw;
	int						make_writable;

	if (!client || !header)
		return (1);
	pw = partial_write_create();
	if (!pw)
		return (-1);
	make_writable = 1 * (client->partial_write_head == 0);
	partial_write_init(pw);
	pw->prechunk = prechunk;
	pw->header = *header;
	pw->buffer = content;
	if (push_partial_write(&client->partial_write_head, pw) < 0)
	{
		partial_write_destroy(pw);
		return (1);
	}
	if (make_writable)
	{
		struct epoll_event ev;
		ev.data.fd = client->server_fd;
		ev.events = EPOLLIN|EPOLLOUT;
		epoll_ctl(client->epoll_fd, EPOLL_CTL_MOD, client->server_fd, &ev);
		// Should probably check if that went through ok or handle errors that are produced from this
	}
	return (0);
}
