/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 14:09:47 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 17:10:32 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_client_int.h"

int _handle_client_msg_send(struct s_client *client)
{
	int	status;

	if (!client)
		return (1);
	if (!client->partial_write_head)
	{
		struct epoll_event	ev;
		ev.data.fd = client->server_fd;
		ev.events = EPOLLIN;
		epoll_ctl(client->epoll_fd, EPOLL_CTL_MOD, client->server_fd, &ev);
		return (0);
	}
	status = partial_write_process(client->partial_write_head, client->server_fd);
	switch (status)
	{
		case (0):
		case (-1):
			return (status);
		case (1):
			partial_write_destroy(pop_partial_write(&client->partial_write_head));
			if (!client->partial_write_head)
			{
				struct epoll_event	ev;
				ev.data.fd = client->server_fd;
				ev.events = EPOLLIN;
				epoll_ctl(client->epoll_fd, EPOLL_CTL_MOD, client->server_fd, &ev);
			}
			return (0);
	}
	return (1);
}
