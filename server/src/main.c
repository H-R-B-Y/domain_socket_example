/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 15:02:56 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/03 18:26:36 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "server.h"

t_vs32 signal_info = 0;

void	safe_terminate(int signo, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	signal_info = signo;
}

int		make_fd_nonblocking(int fd)
{
	int flags;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		return (0);
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) < 0)
		return (0);
	return (1);
}

void	terminate_child(void *chld)
{
	struct s_connection *c;

	c = chld;
	if (!chld || !c->fd)
		return ;
	if (c->epoll_parent)
		epoll_ctl(c->epoll_parent, EPOLL_CTL_DEL, c->fd, NULL);
	close(c->fd);
	free(chld);
}

void	terminate_room(void *room)
{
	(void)room;
}

void terminate_srv(struct s_server *srv, const char *msg, t_u8 ret_code)
{
	if (!srv)
		_exit(ret_code);
	dprintf(STDERR_FILENO, "Terminating server (%d): %s\n", ret_code, msg);
	if (srv->rooms)
		cdll_delete(&srv->rooms, terminate_room);
	if (srv->connections)
		cdll_delete(&srv->connections, terminate_child);
	if (srv->epoll_fd)
		close(srv->epoll_fd);
	if (srv->server_fd)
	{
		close(srv->server_fd);
		unlink(SOCKET_PATH);
	}
	exit(ret_code);
}


int	socket_exists(void)
{
	struct stat	data;

	switch(stat(SOCKET_PATH, &data))
	{
		case (0):
			return (1);
		default:
			return (0);
	}
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

void	create_new_connection(struct s_server *srv)
{
	struct s_connection	*child;
	struct s_cdll_node	*node;
	int					child_addr;
	struct epoll_event	ev = {0};
	
	child_addr = accept(srv->server_fd, NULL, NULL);
	if (child_addr < 0)
		terminate_srv(srv, "Failed to create connection", 1);
	child = ft_calloc(1, sizeof(struct s_connection));
	if (!child)
	{
		close(child_addr);
		terminate_srv(srv, "Failed to allocate connection", 1);
	}
	(*child) = (struct s_connection){.fd = child_addr, .epoll_parent = 0};
	node = cdll_init_node(child);
	if (!node)
	{
		terminate_child(child);
		terminate_srv(srv, "Failed to allocate connection node", 1);
	}
	if (!make_fd_nonblocking(child_addr))
	{
		terminate_child(child);
		terminate_srv(srv, "Failed to make child non blocking", 1);
	}
	// Add the new child to the servers epoll list
	ev.data.fd = child_addr;
	ev.events = EPOLLIN;
	if (epoll_ctl(srv->epoll_fd, EPOLL_CTL_ADD, child_addr, &ev) < 0)
	{
		terminate_child(child);
		terminate_srv(srv, "Failed to add connection to epoll list", 1);
	}
	child->epoll_parent = srv->epoll_fd;
	cdll_push_back(srv->connections, node);
	return ;
}

int	handle_msg(struct s_server *srv, int fd)
{
	(void)srv;
	/*
	Read from fd of size pre header chunk
	Read from fd for bytes in pre header chunk
	Read from fd size of content length
	*/
	char					empty;
	int						prechunk;
	struct s_header_chunk	header;
	int						status;

	status = read(fd, (void *)&prechunk, PRE_HEADER_CHUNK);
	if (status == 0)
		return (-1);
	else if (status < 0)
		return (-1);
	if (prechunk != 1)
		return (1);
	status = read(fd, (void *)&header, sizeof(header));
	if (status == 0)
		return (-1);
	else if (status < 0)
		return (-1);
	else if (status < (long int)sizeof(header))
		return (-1);
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
		printf("Content: %s\n", buffer);
		free(buffer);
	}
	return (0);
}

int	remove_connection_for_fd(struct s_server *srv, int fd)
{
	size_t				idx;
	struct s_cdll_node	*node;

	node = srv->connections->head;
	idx = 0;
	while (idx < srv->connections->count)
	{
		if (node->data && ((struct s_connection *)node->data)->fd == fd)
		{
			if (srv->connections->count > 1)
			{// need to pop this value from the cdll;
				if (node == srv->connections->head)
					srv->connections->head = node->next;
				if (node == srv->connections->tail)
					srv->connections->tail = node->prev;
				if (node->prev)
					node->prev->next = node->next;
				if (node->next)
					node->next->prev = node->prev;
			}
			else
			{
				srv->connections->head = 0;
				srv->connections->tail = 0;
			}
			node->next = 0;
			node->prev = 0;
			srv->connections->count -= 1;
			terminate_child(node->data);
			free(node);
			return (1);
		}
		node = node->next;
		idx++;
	}
	return (0);
}

void	main_loop(struct s_server *srv)
{
	struct epoll_event	events[64];
	int					count;
	int					idx;
	int					status;

	if (!srv)
		return ;
	while (1)
	{
		if (signal_info != 0)
			return ;
		idx = 0;
		count = epoll_wait(srv->epoll_fd, events, 64, -1); // Note: we should probably have a timeout
		if (count == 0)
			continue ;
		else if (count < 0)
		{
			dprintf(STDERR_FILENO, "EPOLL error: %s\n", strerror(errno));
			continue ;
		}
		while (idx < count)
		{
			if (events[idx].data.fd == srv->server_fd)
				create_new_connection(srv);
			else if (events[idx].data.fd == STDIN_FILENO)
			{
				char *buff = get_next_line(STDIN_FILENO);
				if (ft_strcmp(buff, "EXIT\n") == 0)
				{
					free(buff);
					return ;
				}
				else
				{
					printf("SERVER: %s", buff);
					free(buff);
				}
			}
			else 
			{
				status = handle_msg(srv, events[idx].data.fd);
				if (status < 0)
				{
					// client disconnected or was terminated by an error.
					// cleanup
					remove_connection_for_fd(srv, events[idx].data.fd);
				}
			}
			idx++;
		}
	}
}

void	setup_signal_handler(void)
{
	struct sigaction sa = {0};

	sa.sa_flags |= SA_SIGINFO;
	sa.sa_flags |= SA_RESTART;
	sa.sa_sigaction = &safe_terminate;
	sigaction(SIGINT, &sa, 0);
}


int main(void)
{
	struct s_server	srv = {0};
	// first we want to ensure that we are a singleton
	setup_signal_handler();
	if (socket_exists())
	{
		dprintf(STDERR_FILENO, "Socket file %s already exists\nTerminating\n", SOCKET_PATH);
		return (1);
	}
	if (!init_server(&srv))
	{
		dprintf(STDERR_FILENO, "Terminating\n");
		return (1);
	}
	main_loop(&srv);
	terminate_srv(&srv, "Ok!", 0);
	return (0);
}
