/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_shutdown.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 13:52:09 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 14:03:59 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_client_int.h"

/*
Maybe good idea to bzero the client? 
*/
void	client_shutdown(struct s_client *client)
{
	if (!client)
		return ;
	if (client->appdata && client->free_appdata)
		client->free_appdata(client->appdata);
	partial_read_reset(&client->read_state);
	partial_write_clear_list(&client->partial_write_head);
	epoll_ctl(client->epoll_fd, EPOLL_CTL_DEL, STDIN_FILENO, 0);
	epoll_ctl(client->epoll_fd, EPOLL_CTL_DEL, client->server_fd, 0);
	close(client->epoll_fd);
	close(client->server_fd);
	// Yeah probably
	ft_bzero(client, sizeof(*client));
	return ;
}
