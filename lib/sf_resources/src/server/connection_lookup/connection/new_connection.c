/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_connection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 15:02:26 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/09 17:59:09 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"

struct s_connection	*new_connection(int fd, int epoll_parent)
{
	struct s_connection	*conn;

	conn = ft_calloc(1, sizeof(struct s_connection));
	if (!conn)
		return (NULL);
	*conn = (struct s_connection){
		.id = 0,
		.fd = fd,
		.epoll_parent = epoll_parent,
		.read_state = (struct s_partial_read){0},
		.connected_at = time(NULL),
		.user_data = NULL,
		.cleanup_user_data = NULL
	};
	return (conn);
}
