/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 18:19:57 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 13:41:23 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client.h"

t_vs32 g_signal_info = 0;

void	terminate_client(struct s_client *cl, const char *msg, t_u8 stat)
{
	if (!cl)
		exit(1);
	dprintf(STDERR_FILENO, "Client Terminating (%d): %s\n", stat, msg);
	if (cl->server_fd)
		close(cl->server_fd);
	if (cl->epoll_fd)
		close(cl->epoll_fd);
	exit((int)stat);
}

# define CLINITFAIL dprintf(STDERR_FILENO, "Failed to init client: %s\n", strerror(errno))
int	init_client(struct s_client *cl)
{
	if ((cl->server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		CLINITFAIL;
		terminate_client(cl, "Failed to init socket", 1);
	}
	struct sockaddr_un	addr;
	addr = (struct sockaddr_un){0};
	addr.sun_family = AF_UNIX;
	ft_memmove(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path));
	if (connect(cl->server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		CLINITFAIL;
		terminate_client(cl, "Failed to connect to socket", 1);
	}
	if ((cl->epoll_fd = epoll_create1(0)) < 0)
	{
		CLINITFAIL;
		terminate_client(cl, "Failed to init epoll", 1);
	}
	if (!make_fd_nonblocking(STDIN_FILENO))
	{
		CLINITFAIL;
		terminate_client(cl, "Failed to set stdin nonblocking", 1);
	}
	if (!make_fd_nonblocking(cl->server_fd))
	{
		CLINITFAIL;
		terminate_client(cl, "Failed to set server nonblocking", 1);
	}
	struct epoll_event	ev;
	ev.data.fd = cl->server_fd;
	ev.events = EPOLLIN;
	if (epoll_ctl(cl->epoll_fd, EPOLL_CTL_ADD, cl->server_fd, &ev) < 0)
	{
		CLINITFAIL;
		terminate_client(cl, "Failed to add server to epoll", 1);
	}
	ev.data.fd = STDIN_FILENO;
	if (epoll_ctl(cl->epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) < 0)
	{
		CLINITFAIL;
		terminate_client(cl, "Failed to add stdin to epoll", 1);
	}
	return (1);
}

void	main_loop(struct s_client *cl)
{
	struct epoll_event	events[64];
	int					idx;
	int					count;

	if (!cl)
		return ;
	while (1)
	{
		count = epoll_wait(cl->epoll_fd, events, 64, -1); // add timeout
		if (g_signal_info != 0)
			terminate_client(cl, "Signal recieved\n", 1);
		if (count < 0)
		{
			dprintf(STDERR_FILENO, "EPOLL: %s\n", strerror(errno));
			terminate_client(cl, "Epoll returned an error", 1);
		}
		if (count == 0)
		{
			continue ;
		}
		idx = 0;
		while (idx < count)
		{
			if (events[idx].data.fd == STDIN_FILENO)
			{
				/*
				Send this data to the server
				*/
				char					*content;
				struct s_header_chunk	header = {0};
				content = get_next_line(events[idx].data.fd);
				header.content_length = ft_strlen(content);
				header.msg_type = MTYPE_STR;
				int ver = 1;

				write(cl->server_fd, &ver, sizeof(int));
				write(cl->server_fd, &header, sizeof(header));
				write(cl->server_fd, content, header.content_length);
				free(content);
			}
			else if (events[idx].data.fd == cl->server_fd)
			{
				/*
				Read message from server
				*/
				int	status;
				char	none;
				int		pre;
				struct s_header_chunk	header;
				char *content;

				status = read(events[idx].data.fd, &pre, sizeof(int));
				if (status == 0)
				{
					idx++;
					continue;
				}
				if (status < 0)
				{
					dprintf(STDERR_FILENO, "Disconnected? : %s\n", strerror(errno));
					terminate_client(cl, "Disconnected from server?", 1);
				}
				status = read(events[idx].data.fd, &header, sizeof(header));
				if (status == 0)
				{
					idx++;
					continue;
				}
				if (status < 0)
				{
					dprintf(STDERR_FILENO, "Disconnected? : %s\n", strerror(errno));
					terminate_client(cl, "Disconnected from server?", 1);
				}
				if (header.msg_type == MTYPE_STR)
				{
					content = get_next_line(events[idx].data.fd);
					printf("Message from server: %s", content);
					free(content);
				}
				else
				{
					size_t	i;
					i = 0;
					while (i < header.content_length)
					{
						read(events[idx].data.fd, &none, 1);
						i++;
					}
				}
			}
			else
			{
				terminate_client(cl, "Unexptected error", 1);
			}
			idx++;
		}
	}
}

int	main(void)
{
	struct s_client cl = {0};
	struct sigaction handle = {0};

	handle.sa_flags |= SA_SIGINFO;
	handle.sa_flags |= SA_RESTART;
	handle.sa_sigaction = signal_global_notifier;
	sigaction(SIGPIPE, &handle, 0);
	if (!socket_exists())
	{
		dprintf(STDERR_FILENO, "Unable to find server socket!\n");
		return (0);
	}
	if (!init_client(&cl))
		terminate_client(&cl, "FATAL", 1);
	main_loop(&cl);
	terminate_client(&cl, "Ok", 0);
	return (0);
}
