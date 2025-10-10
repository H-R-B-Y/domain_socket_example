/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partial_write_process.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:25:49 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/08 16:44:19 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shared_resource.h"

int	handle_prechunk(int fd, struct s_partial_write *pw, ssize_t *status)
{
	(*status) = write(fd,
		((char *)&pw->prechunk) + pw->bytes_written,
		sizeof(int) - pw->bytes_written
	);
	if ((*status) <= 0)
	{
		return (-1);
	}
	pw->bytes_written += (*status);
	if (pw->bytes_written >= sizeof(int))
	{
		pw->state = PARTIAL_STATE_HEADER;
		pw->bytes_written = 0;
		return (1);
	}
	return (0);
}

int handle_header(int fd, struct s_partial_write *pw, ssize_t *status)
{
	(*status) = write(fd,
		((char *)&pw->header) + pw->bytes_written,
		sizeof(struct s_header_chunk) - pw->bytes_written
	);
	if ((*status) <= 0)
	{
		return (-1);
	}
	pw->bytes_written += (*status);
	if (pw->bytes_written >= sizeof(struct s_header_chunk))
	{
		pw->state = PARTIAL_STATE_CONTENT;
		pw->bytes_written = 0;
		pw->write_size = pw->header.content_length;
		return (1);
	}
	return (0);
}

int handle_content(int fd, struct s_partial_write *pw, ssize_t *status)
{
	(*status) = write(fd,
		pw->buffer + pw->bytes_written,
		pw->write_size - pw->bytes_written
	);
	if ((*status) <= 0)
	{
		return (-1);
	}
	pw->bytes_written += (*status);
	if (pw->bytes_written >= pw->write_size)
	{
		return (1);
	}
	return (0);
}

int	partial_write_process(struct s_partial_write *pw, int fd)
{
	ssize_t	status;
	int		code;

	if (!pw)
		return (-1);
	switch (pw->state)
	{
		case PARTIAL_STATE_PRECHUNK:
			code = handle_prechunk(fd, pw, &status);
			if (code <= 0)
				return (code);
		// fallthrough
		case PARTIAL_STATE_HEADER:
			code = handle_header(fd, pw, &status);
			if (code <= 0)
				return (code);
		// fallthrough
		case PARTIAL_STATE_CONTENT:
			return (handle_content(fd, pw, &status));
		default:
			return (-1);
	}
	return (-1);
}
