/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection_lookup_get.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 15:22:00 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/09 17:59:09 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"

struct s_connection			*get_connection_by_id(struct s_connection_lookup *lookup, t_connection_id id)
{
	if (!lookup || id == 0)
		return (0);
	return (hm_get_value(&lookup->id_to_connection, &id));
}

struct s_connection			*get_connection_by_fd(struct s_connection_lookup *lookup, int fd)
{
	if (!lookup || fd < 0)
		return (0);
	return (hm_get_value(&lookup->fd_to_connection, &fd));
}
