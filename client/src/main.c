/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 18:08:56 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 18:21:20 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"


int	on_connect(struct s_client *client, void *appdata)
{
	(void)client;
	(void)appdata;

	dprintf(STDERR_FILENO, "Client connected\n");
	return (1);
}

int	on_msg_recv(
	struct s_client *client,
	int prechunk,
	struct s_header_chunk *header,
	void *content,
	void *appdata
)
{
	(void)client;
	(void)appdata;
	(void)prechunk;
	(void)header;

	if (content)
		dprintf(STDOUT_FILENO, "MSG: %s\n", (char *)content);
	else
		dprintf(STDOUT_FILENO, "MSG: (NULL)\n");
	return (1);
}

void	on_disconnect(struct s_client *client, void *appdata)
{
	(void)client;
	(void)appdata;
	dprintf(STDERR_FILENO, "Client disconnected\n");
}

int	on_stdin(struct s_client *client, void *appdata)
{
	struct s_header_chunk	header = {0};
	char					*content;
	
	(void)appdata;
	content = get_next_line(STDIN_FILENO);
	header.content_length = ft_strlen(content);
	header.msg_type = MTYPE_STR;
	client_send_message(client, 1, &header, content);
	return (1);
}


int main(void)
{
	struct s_client	client = {0};

	if (!client_init(&client, SOCKET_PATH))
		return (1);
	client.on_connect = on_connect;
	client.on_disconnect = on_disconnect;
	client.on_msg = on_msg_recv;
	client.on_stdin = on_stdin;
	client_run(&client);
	client_shutdown(&client);
}
