/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate_client.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 13:52:14 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 14:00:12 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_client_int.h"

void	_terminate_client(struct s_client *client, const char *msg, int exit_code, int show_errno)
{
	if (!client)
		return ;
	if (show_errno)
		dprintf(STDERR_FILENO, "Client terminating with errno %s\n Reason %s\n", strerror(errno), msg);
	else
		dprintf(STDERR_FILENO, "Client terminating %s\n", msg);
	if (client->appdata && client->free_appdata)
		client->free_appdata(client->appdata);
	partial_read_reset(&client->read_state);
	partial_write_clear_list(&client->partial_write_head);
	if (client->epoll_fd)
	{
		epoll_ctl(client->epoll_fd, EPOLL_CTL_DEL, STDIN_FILENO, 0);
		if (client->server_fd)
			epoll_ctl(client->epoll_fd, EPOLL_CTL_DEL, client->server_fd, 0);
		close(client->epoll_fd);
	}
	if (client->server_fd)
		close(client->server_fd);
	exit(exit_code);
}
