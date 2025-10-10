/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_run.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 15:34:48 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 18:24:53 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_client_int.h"

static int	_handle_server(struct s_client *client, struct epoll_event *ev)
{
	int	status;

	if (!(ev->events & EPOLLIN || ev->events & EPOLLOUT))
		return (-1);
	if (ev->events & EPOLLIN)
	{
		status = _handle_client_msg_recv(client);
		switch (status)
		{
			case (-1):
			case (1):
				return (-1);
			default:
				break ;
		}
	}
	if (ev->events & EPOLLOUT)
	{
		status = _handle_client_msg_send(client);
		switch (status)
		{
			case (-1):
			case (1):
				return (-1);
			default:
				break ;
		}
	}
	return (0);
}

// TODO: define some clear protocol here
/*
IF there was some error caused when reading or writing form the server
we should do something about it where ???? in the handle_server?
like if there was a disconnect? do i call client stop here or client stop there
or do i call client stop in the run function.

also do i call the disconnect handler when the disconnect happens?
or do i call if after the event loop has been stopped?
or do i do something else entirely?

ok right now we will ignore the disconnect handler
and we will stop the loop as soon as possible.
*/
static int	epoll_loop(struct s_client *client, struct epoll_event *ev, size_t event_count)
{
	unsigned int	idx;
	int				status;

	idx = 0;
	while (client->connected && idx < event_count)
	{
		if (ev[idx].data.fd == client->server_fd)
		{
			status = _handle_server(client, &ev[idx]);
			if (status < 0)
			{
				client_stop(client);
				return (-1);
			}
		}
		else if (ev[idx].data.fd == STDIN_FILENO)
			_handle_stdin(client);
		else
			dprintf(STDERR_FILENO, "EVENT LOOP: encountered an event that is not known\n");
		idx++;
	}
	return (0);
}

/*
int returned should be:
0 OK
-1 unclean exit
*/
int	client_run(struct s_client *client)
{
	struct epoll_event	ev[200];
	int					status;

	if (!client)
		return (0);
	if (!client->connected)
	{
		if (client_connect(client) != 0)
			client_stop(client);
		_on_connect(client);
	}
	while (client->connected)
	{
		if (client->pre_poll_hook
			&& client->pre_poll_hook(client, client->appdata) < 0)
		{
			client_stop(client);
			continue;
		}
		status = epoll_wait(client->epoll_fd, ev, 200, -1); // TODO: figure out a good timout, or make configurable
		if (status < 0)
		{
			dprintf(STDERR_FILENO, "EPOLL: error code: %s\n", strerror(errno));
			if (errno != EAGAIN && errno != EWOULDBLOCK)
			{
				client_stop(client);
				continue;
			}
		}
		else if (status)
			epoll_loop(client, ev, status);
		if (client->connected && client->post_poll_hook
			&& client->post_poll_hook(client, client->appdata) < 0)
			client_stop(client);
	}
	if (client->on_disconnect)
		client->on_disconnect(client, client->appdata);
	return (0);
}
