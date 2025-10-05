/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_resource.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 11:01:30 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/05 14:46:44 by hbreeze          ###   ########.fr       */
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

# ifndef SOCKET_PATH
#  define SOCKET_PATH "/tmp/server_5790.sock"
# endif

enum e_message_types
{
	MTYPE_NONE,
	MTYPE_STR,
	MTYPE_TYPE_COUNT
};

struct s_header_chunk
{
	enum e_message_types	msg_type;
	size_t					content_length;
};

int		make_fd_nonblocking(int fd);
int		socket_exists(const char *socket_path);


enum e_read_state
{
	READ_STATE_PRECHUNK,
	READ_STATE_HEADER,
	READ_STATE_CONTENT
};

struct s_partial_read
{
	enum e_read_state	state;
	size_t				bytes_read;
	size_t				bytes_expected;
	int					prechunk;
	struct s_header_chunk	header;
	char				*buffer;
};

void	partial_read_init(struct s_partial_read *pr);
void	partial_read_reset(struct s_partial_read *pr);
int		partial_read_process(struct s_partial_read *pr, int fd);

int		send_message(int fd, enum e_message_types type, const char *content, size_t length);

#endif