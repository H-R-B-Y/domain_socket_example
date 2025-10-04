/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 10:55:16 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/04 11:31:24 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

t_vs32 g_signal_info = 0;

void	setup_signal_handler(void)
{
	struct sigaction sa = {0};

	sa.sa_flags |= SA_SIGINFO;
	sa.sa_flags |= SA_RESTART;
	sa.sa_sigaction = &signal_global_notifier;
	sigaction(SIGINT, &sa, 0);
}

# define FAILINITMSG dprintf(STDERR_FILENO, "Failed to init server: %s\n", strerror(errno))
int init_server(struct s_server *srv)
{
	if ((srv->server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		FAILINITMSG;
		terminate_srv(srv, "Failed to open domain socket", 1);
	}
	if (make_fd_nonblocking(srv->server_fd) == 0)
	{
		FAILINITMSG;
		terminate_srv(srv, "Could not make the server non blocking", 1);
	}
	srv->addr = (struct sockaddr_un){0};
	srv->addr.sun_family = AF_UNIX;
	ft_memmove(srv->addr.sun_path, SOCKET_PATH, sizeof(srv->addr.sun_path) - 1);
	if (bind(srv->server_fd, (struct sockaddr *)&srv->addr, sizeof(srv->addr)) < 0)
	{
		FAILINITMSG;
		terminate_srv(srv, "Failed to bind to socket path", 1);
	}
	// So we now know that the server has a valid domain socket
	if ((srv->connections = cdll_init()) == 0)
	{
		FAILINITMSG;
		terminate_srv(srv, "Failed to initialise connections linked list",1);
	}
	if ((srv->rooms = cdll_init()) == 0)
	{
		FAILINITMSG;
		terminate_srv(srv, "Failed to initialise rooms linked list", 1);
	}
	if (listen(srv->server_fd, 10) < 0)
	{
		FAILINITMSG;
		terminate_srv(srv, "Failed to open server for listening", 1);
	}
	if ((srv->epoll_fd = epoll_create1(0)) <0 )
	{
		FAILINITMSG;
		terminate_srv(srv, "Failed to create epoll fd", 1);
	}
	struct epoll_event ev = {0};
	ev.events = EPOLLIN;
	ev.data.fd = srv->server_fd;
	if (epoll_ctl(srv->epoll_fd, EPOLL_CTL_ADD, srv->server_fd, &ev) < 0)
	{
		FAILINITMSG;
		terminate_srv(srv, "Failed to add server fd to the epoll list", 1);
	}
	ev.events = EPOLLIN;
	ev.data.fd = STDIN_FILENO;
	if (epoll_ctl(srv->epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) < 0)
	{
		FAILINITMSG;
		terminate_srv(srv, "Failed to add server fd to the epoll list", 1);
	}
	return (1);
}
# undef FAILINITMSG
