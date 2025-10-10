/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sock_client_framework.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 18:02:57 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 18:18:48 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCK_CLIENT_FRAMEWORK_H
# define SOCK_CLIENT_FRAMEWORK_H

# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 1
# endif
# ifndef _POSIX_SOURCE
#  define _POSIX_SOURCE
# endif
# ifndef _XOPEN_SOURCE
#  define _XOPEN_SOURCE 700
# endif

# include "shared_resource.h"

# define FT_INCLUDE_ALL
# include "libft.h"

struct s_client;

typedef int		(*t_on_client_connected)(
	struct s_client *client,
	void *appdata
);

typedef int		(*t_on_client_message_recv)(
	struct s_client *client,
	int prechunk,
	struct s_header_chunk *header,
	void *content,
	void *appdata
);

typedef void	(*t_on_client_disconnect)(
	struct s_client *client,
	void *appdata
);

typedef int (*t_on_client_stdin)(
	struct s_client *client,
	void *appdata
);

typedef int	(*t_loop_hook)(
	struct s_client *client,
	void *appdata
);

struct s_client
{
	int							server_fd;
	struct sockaddr_un			addr;
	int							epoll_fd;


	struct s_partial_read		read_state;
	struct s_partial_write		*partial_write_head;

	int							connected;

	void						*appdata;
	t_freefn					free_appdata;

	t_on_client_connected		on_connect;
	t_on_client_message_recv	on_msg;
	t_on_client_disconnect		on_disconnect;
	t_on_client_stdin			on_stdin;
	t_loop_hook					pre_poll_hook;
	t_loop_hook					post_poll_hook;
};

int		client_init(struct s_client *client, const char *sock_path);
int		client_run(struct s_client *client);
int		client_connect(struct s_client *client);
void	client_stop(struct s_client *client);
void	client_shutdown(struct s_client *client);

/*
We need some way to queue messages to send to the server
not sure if we should create a new message type of just queue the raw data
will do raw data for now but will have to make a generic message type 
in the shared resource folder later on.
*/
// int		client_send_message(struct s_client *client, int prechunk, struct s_header *header, void *content);

int			client_send_message(
	struct s_client *client,
	int prechunk,
	struct s_header_chunk *header,
	void *content
);

#endif
