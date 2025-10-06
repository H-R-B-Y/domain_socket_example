/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partial_read_process.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 11:17:23 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/06 14:29:22 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shared_resource.h"


static int handle_prechunk(int fd, struct s_partial_read *pr, ssize_t *status)
{
	(*status) = read(fd, 
		((char *)&pr->prechunk) + pr->bytes_read,
		sizeof(int) - pr->bytes_read
	);
	if ((*status) <= 0)
	{
		return (-1);
	}
	pr->bytes_read += (*status);
	if (pr->bytes_read >= sizeof(int))
	{
		pr->state = PARTIAL_STATE_HEADER;
		pr->bytes_read = 0;
		pr->bytes_expected = sizeof(struct s_header_chunk);
		return (1);
	}
	return (0);
}

static int	handle_header(int fd, struct s_partial_read *pr, ssize_t *status)
{
	(*status) = read(fd,
		((char *)&pr->header) + pr->bytes_read,
		sizeof(struct s_header_chunk) - pr->bytes_read
	);
	if ((*status) <= 0)
	{
		return (-1);
	}
	pr->bytes_read += (*status);
	if (pr->bytes_read >= sizeof(struct s_header_chunk))
	{
		pr->state = PARTIAL_STATE_CONTENT;
		pr->bytes_expected = pr->header.content_length;
		pr->bytes_read = 0;
		/*
		Ideally we can just move something like this towards
		an arena buffer or free list, just to keep memory management to a minimum.
		*/
		pr->buffer = ft_calloc(pr->bytes_expected + 1, sizeof(char));
		return (1);
	}
	return (0);
}

static int	handle_message_content(int fd, struct s_partial_read *pr, ssize_t *status)
{
	(*status) = read(fd,
		((char *)pr->buffer) + pr->bytes_read,
		pr->bytes_expected - pr->bytes_read
	);
	if ((*status) <= 0)
	{
		return (0);
	}
	pr->bytes_read += (*status);
	if (pr->bytes_read >= pr->bytes_expected)
	{
		pr->state = PARTIAL_STATE_PRECHUNK;
		pr->bytes_read = 0;
		pr->bytes_expected = 0;
		return (1);
	}
	return (0);
}

int		partial_read_process(struct s_partial_read *pr, int fd)
{
	ssize_t	status;
	int		code;

	switch (pr->state)
	{
		case PARTIAL_STATE_PRECHUNK:
			code = handle_prechunk(fd, pr, &status);
			if (code < 0)
				return (-1);
			if (code == 0)
				return (0);
			// fallthrough
		case PARTIAL_STATE_HEADER:
			code = handle_header(fd, pr, &status);
			if (code < 0)
				return (-1);
			if (code == 0)
				return (0);
			// fallthrough
		case PARTIAL_STATE_CONTENT:
			code = handle_message_content(fd, pr, &status);
			break ;
	}
	return (code);
}
