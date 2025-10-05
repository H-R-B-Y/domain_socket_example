/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:37:46 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/05 10:53:32 by hbreeze          ###   ########.fr       */
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

	enum e_read_state
	{
		READ_STATE_PRECHUNK,
		READ_STATE_HEADER,
		READ_STATE_CONTENT,
	}	read_state;
	size_t	bytes_read;
	size_t	bytes_expected;
	int		prechunk;
	struct s_header_chunk header;
	char	*content_buffer;
	int		msg_complete;
};

struct s_room
{

};

typedef int (*t_msg_handler)(void *srv, struct s_header_chunk *header, int client);


struct s_server
{
	int					server_fd;
	int					epoll_fd;
	struct sockaddr_un	addr;
	t_cdll				*connections;
	t_cdll				*rooms;
	t_msg_handler		handlers[MTYPE_TYPE_COUNT];
};


void	main_loop(struct s_server *srv);
void	terminate_child(void *chld);
void	terminate_room(void *room);
void terminate_srv(struct s_server *srv, const char *msg, t_u8 ret_code);
int init_server(struct s_server *srv);
void	setup_signal_handler(void);

void	create_new_connection(struct s_server *srv);
int	handle_msg(struct s_server *srv, int fd);
int	handle_disconnect(struct s_server *srv, int fd);
int	remove_connection_for_fd(struct s_server *srv, int fd);
int	handle_message(struct s_server *srv, struct s_connection *conn);

void	broadcast_message(struct s_server *srv, char *buffer, size_t size);

#endif
