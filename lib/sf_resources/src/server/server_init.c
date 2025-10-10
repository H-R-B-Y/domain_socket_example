/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 14:42:27 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/09 17:59:09 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"

static int	_server_init_socket(struct s_server *srv, const char *socket_path)
{
	if (socket_exists(socket_path))
		_terminate_srv(srv, "Socket file already exists", 1, 0);
	srv->server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (srv->server_fd < 0)
		_terminate_srv(srv, "Failed to create socket", 1, 1);
	if (!make_fd_nonblocking(srv->server_fd))
		_terminate_srv(srv, "Failed to make socket non blocking", 1, 1);
	srv->addr = (struct sockaddr_un){0};
	srv->addr.sun_family = AF_UNIX;
	ft_memmove(srv->addr.sun_path, socket_path, ft_strlen(socket_path));
	if (bind(srv->server_fd, (struct sockaddr *)&srv->addr, sizeof(struct sockaddr_un)) < 0)
		_terminate_srv(srv, "Failed to bind socket", 1, 1);
	if (listen(srv->server_fd, 128) < 0)
		_terminate_srv(srv, "Failed to listen on socket", 1, 1);
	return (1);
}

static int _server_init_epoll(struct s_server *srv)
{
	struct epoll_event	ev;

	srv->epoll_fd = epoll_create1(0);
	if (srv->epoll_fd < 0)
		_terminate_srv(srv, "Failed to create epoll instance", 1, 1);
	// Add the server socket to the epoll instance
	{
		ev = (struct epoll_event){0};
		ev.data.fd = srv->server_fd;
		ev.events = EPOLLIN;
		if (epoll_ctl(srv->epoll_fd, EPOLL_CTL_ADD, srv->server_fd, &ev) < 0)
			_terminate_srv(srv, "Failed to add server socket to epoll instance", 1, 1);
	}
	// Add stdin to the epoll instance
	{
		ev = (struct epoll_event){0};
		if (!make_fd_nonblocking(STDIN_FILENO))
			_terminate_srv(srv, "Failed to make stdin non blocking", 1, 1);
		ev.data.fd = STDIN_FILENO;
		ev.events = EPOLLIN;
		if (epoll_ctl(srv->epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) < 0)
			_terminate_srv(srv, "Failed to add stdin to epoll instance", 1, 1);
	}
	return (1);
}

	// In order we need to:
	/*
	1. Check the socket doesnt already exist
	2. Create the socket
	3. Bind the socket
	4. Listen on the socket
	5. Create the epoll instance
	6. Add the socket to the epoll instance
	7. Initialize the connection lookup
	8. Initialize any other data structures
	*/
int		server_init(struct s_server *srv, const char *socket_path)
{
	if (!srv || !socket_path)
		return (-1);
	(*srv) = (struct s_server){0};
	if (!_server_init_socket(srv, socket_path))
		_terminate_srv(srv, "Failed to initialize server socket", 1, 1);
	if (!_server_init_epoll(srv))
		_terminate_srv(srv, "Failed to initialize epoll instance", 1, 1);
	if (!connection_lookup_init(&srv->connection_data))
		_terminate_srv(srv, "Failed to create connection lookup", 1, 1);
	srv->server_room = cdll_init();
	return (0);
}
