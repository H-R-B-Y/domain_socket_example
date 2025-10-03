/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:37:46 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/03 16:46:57 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

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
# include <sys/socket.h>
# include <sys/epoll.h>
# include <sys/stat.h>
# include <sys/un.h>

# define FT_INCLUDE_ALL
# include "libft.h"

# include "shared_resource.h"

# ifndef SOCKET_PATH
#  define SOCKET_PATH "/tmp/server_5790.sock"
# endif

struct s_connection
{
	int	fd;
	int	epoll_parent; // The fd associated with the reader of this fd
};

struct s_room
{

};

struct s_server
{
	int					server_fd;
	int					epoll_fd;
	struct sockaddr_un	addr;
	t_cdll				*connections;
	t_cdll				*rooms;
};

#endif
