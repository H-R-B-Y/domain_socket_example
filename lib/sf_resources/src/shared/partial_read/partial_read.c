/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partial_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 11:12:24 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/08 16:40:07 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shared_resource.h"

void	partial_read_init(struct s_partial_read *pr)
{
	if (!pr)
		return ;
	pr->buffer = 0;
	pr->bytes_expected = sizeof(int);
	pr->bytes_read = 0;
	pr->state = PARTIAL_STATE_PRECHUNK;
	pr->prechunk = 0;
	pr->header = (struct s_header_chunk){0};
}

void	partial_read_reset(struct s_partial_read *pr)
{
	if (!pr)
		return ;
	if (pr->buffer)
		free(pr->buffer);
	partial_read_init(pr);
}
