/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:32:59 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/08 17:26:23 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

// typedef int		(*t_on_connect_fn)(struct s_server *srv, struct s_connection *conn, void *appdata);
// typedef int		(*t_on_message_fn)(struct s_server *srv, struct s_message *message, void *appdata);
// typedef void	(*t_on_disconnect_fn)(struct s_server *srv, struct s_connection *conn, void *appdata);

int		on_connect(struct s_server *srv, struct s_connection *conn, void *appdata)
{
	(void)srv;
	(void)appdata;
	// We have already created the app data so lets just print something
	printf("Client connected with fd %d and id %zu\n", conn->fd, conn->id);
	return (1);
}

int	on_message(struct s_server *srv, struct s_message *msg, void *appdata)
{
	(void)srv;
	(void)appdata;
	printf("Recieved message from client with fd %d and id %zu\n", msg->sender->fd, msg->sender->id);
	printf("Message content: %s", msg->content);
	return (1);
}

void	on_disconnect(struct s_server *srv, struct s_connection *conn, void *appdata)
{
	(void)srv;
	(void)appdata;
	printf("Client with fd %d and id %zu has disconnected\n", conn->fd, conn->id);
	return ;
}


int main(void)
{
	struct s_server	srv;

	printf("connecting to socket %s\n", SOCKET_PATH);
	if (server_init(&srv, SOCKET_PATH) < 0)
		return (0);
	srv.appdata = 0;
	srv.free_appdata = 0;
	srv.on_connect = on_connect;
	srv.on_message = on_message;
	srv.on_disconnect = on_disconnect;
	server_run(&srv);
	server_shutdown(&srv);
	return (0);
}
