/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_stop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 14:04:02 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 17:13:36 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_client_int.h"

void	client_stop(struct s_client *client)
{
	if (!client)
		return ;
	if (!client->connected)
	{
		dprintf(STDERR_FILENO, "Unable to stop client that is not connected");
		return ;
	}
	client->connected = 0;
	return ;
}
