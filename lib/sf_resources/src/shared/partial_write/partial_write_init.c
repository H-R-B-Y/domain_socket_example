/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partial_write_init.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:20:39 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/06 14:29:10 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shared_resource.h"

void					partial_write_init(struct s_partial_write *pw)
{
	if (!pw)
		return ;
	pw->next = NULL;
	pw->buffer = NULL;
	pw->write_size = sizeof(int);
	pw->bytes_written = 0;
	pw->state = PARTIAL_STATE_PRECHUNK;
}

struct s_partial_write	*partial_write_create(void)
{
	struct s_partial_write	*pw;

	pw = ft_calloc(1, sizeof(struct s_partial_write));
	if (!pw)
		return (NULL);
	partial_write_init(pw);
	return (pw);
}

void					partial_write_reset(struct s_partial_write *pw)
{
	if (!pw)
		return ;
	if (pw->buffer)
		free(pw->buffer);
	partial_write_init(pw);
}
