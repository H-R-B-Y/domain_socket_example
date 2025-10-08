/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partial_write_management.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:36:25 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/08 17:18:19 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shared_resource.h"

struct s_partial_write *pop_partial_write(struct s_partial_write **pw)
{
	struct s_partial_write *ret;

	if (!pw || !*pw)
		return (NULL);
	ret = *pw;
	*pw = (*pw)->next;
	ret->next = NULL;
	return (ret);
}

int						push_partial_write(struct s_partial_write **head,
							struct s_partial_write *new_pw)
{
	struct s_partial_write	*current;

	if (!head || !new_pw)
		return (-1);
	if (!*head)
	{
		*head = new_pw;
		return (0);
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_pw;
	return (0);
}

void	partial_write_clear_list(struct s_partial_write **head)
{
	if (!head || !*head)
		return ;
	if ((*head)->next)
		partial_write_clear_list(
			(struct s_partial_write **)(&((*head)->next))
		);
	partial_write_destroy(*head);
	(*head) = 0;
}
