/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_stdin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:40:53 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 16:56:57 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_client_int.h"

int	_handle_stdin(struct s_client *client)
{
	if (!client)
		return (1);
	if (client->on_stdin && !client->on_stdin(client, client->appdata))
		return (-1);
	return (0);
}
