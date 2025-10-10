/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_msg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 14:08:47 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 15:31:23 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_client_int.h"

/*
Do partial read, need to return 
0 = OK
-1 = check errno
1 = internal error

The fd is going to be the server fd
*/
int	_handle_client_msg_recv(struct s_client *client)
{
	int	status;

	if (!client)
		return (1);
	status = partial_read_process(&client->read_state, client->server_fd);
	switch (status)
	{
		case 0:
		case -1:
			return (status);
		case 1:
			/*
			Do something with the message
			*/
			if (client->on_msg)
			{
				if (!client->on_msg(client, client->read_state.prechunk,
						&client->read_state.header, client->read_state.buffer, client->appdata))
					return (1);
			}
			partial_read_reset(&client->read_state);
			return (0);
	}
	// Internal issue if no case was triggered
	return (1);
}
