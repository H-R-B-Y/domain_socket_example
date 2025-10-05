/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_resource.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:22:40 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/05 10:53:59 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_RESOURCE_H
# define SHARED_RESOURCE_H

# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 1
# endif
# ifndef _POSIX_SOURCE
#  define _POSIX_SOURCE
# endif
# ifndef _XOPEN_SOURCE
#  define _XOPEN_SOURCE 700
# endif

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <signal.h>
# include <stddef.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <sys/stat.h>
# include <sys/un.h>

# define FT_INCLUDE_ALL
# include "libft.h"

# include "shared_resource.h"
/*
Data used for data sent between the sockets
*/
# define PRE_HEADER_CHUNK sizeof(int)
# define HEADER_CHUNK sizeof(struct s_header_chunk)

# ifndef SOCKET_PATH
#  define SOCKET_PATH "/tmp/server_5790.sock"
# endif

extern t_vs32 g_signal_info;

static inline void	signal_global_notifier(int signo, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	g_signal_info = signo;
}

static inline int	socket_exists(void)
{
	struct stat	data;

	switch(stat(SOCKET_PATH, &data))
	{
		case (0):
			return (1);
		default:
			return (0);
	}
}

static inline int		make_fd_nonblocking(int fd)
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

enum e_message_types {
	MTYPE_NONE,
	MTYPE_STR,
	MTYPE_TYPE_COUNT
};

struct s_header_chunk
{
	enum e_message_types	msg_type;
	size_t					content_length;
};

#endif
