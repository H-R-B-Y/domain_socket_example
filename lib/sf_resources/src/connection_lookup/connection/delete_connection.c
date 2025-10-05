/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_connection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 15:03:43 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/05 15:03:50 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_internal.h"

void	delete_connection(struct s_connection *conn)
{
	if (!conn)
		return ;
	if (conn->cleanup_user_data && conn->user_data)
		conn->cleanup_user_data(conn->user_data);
	if (conn->fd >= 0)
		close(conn->fd);
	free(conn);
}