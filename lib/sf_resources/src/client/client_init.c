/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 13:37:42 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 18:31:54 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_client_int.h"

# define TERMINATE(reason, show_errno) _terminate_client(client, reason, 1, show_errno)

static int	init_socket(struct s_client *client, const char *sock_path)
{
	if (!socket_exists(sock_path))
		TERMINATE("Socket not found", 0);
	client->server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (client->server_fd < 0)
		TERMINATE("Unable to create server socket", 1);
	client->addr.sun_family = AF_UNIX;
	ft_memmove(client->addr.sun_path, sock_path, sizeof(client->addr.sun_path));
	if (!make_fd_nonblocking(client->server_fd))
		TERMINATE("Unable to make server non-blocking", 1);
	return (1);
}

static int	init_epoll(struct s_client *client)
{
	struct epoll_event	ev;

	client->epoll_fd = epoll_create1(0);
	if (client->epoll_fd < 0)
		TERMINATE("Unable to create epoll fd", 1);
	ev.events = EPOLLIN;
	// ev.data.fd = client->server_fd;
	// if (epoll_ctl(client->epoll_fd, EPOLL_CTL_ADD, client->server_fd, &ev) < 0)
	// 	TERMINATE("Unable to add server to epoll", 1);
	if (!make_fd_nonblocking(STDIN_FILENO))
		TERMINATE("Unable to make stdin non-blocking", 1);
	ev.data.fd = STDIN_FILENO;
	if (epoll_ctl(client->epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) < 0)
		TERMINATE("Unable to add stdin to epoll", 1);
	return (1);
}


int client_init(struct s_client *client, const char *sock_path)
{
	ft_bzero(client, sizeof(*client));
	if (!init_socket(client, sock_path))
		return (0);
	if (!init_epoll(client))
		return (0);
	partial_read_init(&client->read_state);
	return (1);
}
