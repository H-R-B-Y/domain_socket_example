/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partial_write_free.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:23:58 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/06 14:24:44 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shared_resource.h"

void					partial_write_destroy(struct s_partial_write *pw)
{
	if (!pw)
		return ;
	if (pw->buffer)
		free(pw->buffer);
	ft_memset(pw, 0, sizeof(struct s_partial_write));
}

void					free_partial_write(struct s_partial_write *pw)
{
	if (!pw)
		return ;
	partial_write_destroy(pw);
	free(pw);
}

