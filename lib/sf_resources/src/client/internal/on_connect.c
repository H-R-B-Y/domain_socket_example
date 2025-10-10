/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_connect.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 14:05:34 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 17:11:00 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_client_int.h"

/*
A note on implementation here
on connect was added because i was just copying what i wrote for the server
without thinking much about it. 

but I think it might actually be a good idea to move the connect call
to the start of the first loop? 
this way we have our own internal on connect funtion that runs on success
and client code can hook into this to idk initialise some data on connect
*/

int	_on_connect(struct s_client *client)
{
	// this will run on successful connection
	client->connected = 1;
	if (client->on_connect)
		client->on_connect(client, client->appdata);
	return (0);
}
