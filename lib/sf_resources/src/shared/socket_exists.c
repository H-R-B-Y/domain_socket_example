/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_exists.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 11:04:04 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/05 14:46:33 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shared_resource.h"

int	socket_exists(const char *socket_path)
{
	struct stat	data;

	if (!socket_path)
		socket_path = SOCKET_PATH;
	switch(stat(socket_path, &data))
	{
		case (0):
			return (1);
		default:
			return (0);
	}
}
