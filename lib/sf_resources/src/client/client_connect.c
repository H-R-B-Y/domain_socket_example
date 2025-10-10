/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_connect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:35:34 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 18:30:48 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_client_int.h"

int	client_connect(struct s_client *client)
{
	if (!client)
		return (1);
	if (!client->server_fd)
	{
		client->server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (client->server_fd < 0)
			return (-1);
	}
	if (connect(client->server_fd, (struct sockaddr *)&client->addr, sizeof(client->addr)) < 0)
		return (-1);
	struct epoll_event	ev;
	ev.data.fd = client->server_fd;
	ev.events = EPOLLIN;
	if (epoll_ctl(client->epoll_fd, EPOLL_CTL_ADD, client->server_fd, &ev) < 0)
		return (-1);
	if (client->on_connect && !client->on_connect(client, client->appdata))
		return (1);
	return (0);
}
