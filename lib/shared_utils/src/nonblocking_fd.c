/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nonblocking_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 11:03:24 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/05 11:03:50 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shared_resource.h"

int		make_fd_nonblocking(int fd)
{
	int flags;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		return (0);
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) < 0)
		return (0);
	return (1);
}
