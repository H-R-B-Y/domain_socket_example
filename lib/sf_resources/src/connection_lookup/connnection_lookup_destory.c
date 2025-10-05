/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connnection_lookup_destory.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 15:11:38 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/05 15:14:23 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_internal.h"

void	connection_lookup_destroy(struct s_connection_lookup *lookup)
{
	if (!lookup)
		return ;
	hm_destroy(&lookup->id_to_connection);
	hm_destroy(&lookup->fd_to_connection);
	destroy_heap(&lookup->available_ids);
	cdll_destroy(&lookup->connections, (t_freefn)delete_connection);
	ft_memset(lookup, 0, sizeof(struct s_connection_lookup));
}
