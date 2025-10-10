/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection_lookup_init.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 15:04:35 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 19:07:38 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"


static unsigned int _hash_connection_id(const void *key)
{
	return (*(t_connection_id *)key % 1024);
};

static int _compare_connection_id(const void *a, const void *b)
{
	t_connection_id	id_a;
	t_connection_id	id_b;

	if (!a || !b)
		return (0);
	id_a = *(t_connection_id *)a;
	id_b = *(t_connection_id *)b;
	if (id_a == id_b)
		return (1);
	return (0);
}

static unsigned int _hash_connection_fd(const void *key)
{
	return (*(int *)key % 1024);
}

static int _compare_connection_fd(const void *a, const void *b)
{
	int	fd_a;
	int	fd_b;

	if (!a || !b)
		return (0);
	fd_a = *(int *)a;
	fd_b = *(int *)b;
	if (fd_a == fd_b)
		return (0);
	return (1);
}

static int	_heap_compare_ids(const void *a, const void *b)
{
	t_connection_id	id_a;
	t_connection_id	id_b;

	if (!a || !b)
		return (0);
	id_a = *(t_connection_id *)a;
	id_b = *(t_connection_id *)b;
	if (id_a < id_b)
		return (-1);
	else if (id_a > id_b)
		return (1);
	return (0);
}

int	connection_lookup_init(struct s_connection_lookup *lookup)
{
	if (!lookup)
		return (0);
	lookup->id_to_connection = hm_create(_hash_connection_id, _compare_connection_id, 1024);
	if (!lookup->id_to_connection.pairs)
		return (0);
	lookup->fd_to_connection = hm_create(_hash_connection_fd, _compare_connection_fd, 1024);
	if (!lookup->fd_to_connection.pairs)
	{
		free(lookup->id_to_connection.pairs);
		return (0);
	}
	lookup->available_ids = init_heap(1024, sizeof(t_connection_id), (void *)_heap_compare_ids);
	if (!lookup->available_ids.heap)
	{
		free(lookup->id_to_connection.pairs);
		free(lookup->fd_to_connection.pairs);
		return (0);
	}
	lookup->next_id = 1;
	lookup->connections = cdll_init();
	return (1);
}
