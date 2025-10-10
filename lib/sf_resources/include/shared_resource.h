/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_resource.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 11:01:30 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 16:29:30 by hbreeze          ###   ########.fr       */
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
# include <time.h>

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


enum e_partial_state
{
	PARTIAL_STATE_PRECHUNK,
	PARTIAL_STATE_HEADER,
	PARTIAL_STATE_CONTENT
};

struct s_partial_read
{
	enum e_partial_state	state;
	size_t					bytes_read;
	size_t					bytes_expected;
	int						prechunk;
	struct s_header_chunk	header;
	char					*buffer;
};

void	partial_read_init(struct s_partial_read *pr);
void	partial_read_reset(struct s_partial_read *pr);

/**
 * @brief Process a partial read, will read into the pr struct
 * 
 * @param pr The parital read struct to store the data
 * @param fd The file desc to read from
 * @return int:
 * 1 if the read data is ready
 * 0 if the read was partial
 * -1 if the read failed or an error occured
 * 
 */
int		partial_read_process(struct s_partial_read *pr, int fd);

struct s_partial_write
{
	enum e_partial_state	state;
	size_t					bytes_written;
	size_t					write_size;
	int						prechunk;
	struct s_header_chunk	header;
	char					*buffer;
	void					*next;
};

void					partial_write_init(struct s_partial_write *pw);
struct s_partial_write	*partial_write_create(void);
void					partial_write_reset(struct s_partial_write *pw);
int						partial_write_process(struct s_partial_write *pw, int fd);
void					partial_write_destroy(struct s_partial_write *pw);
void					free_partial_write(struct s_partial_write *pw);
void					partial_write_clear_list(struct s_partial_write **head);
int						push_partial_write(struct s_partial_write **head,
							struct s_partial_write *new_pw);
/**
 * @brief Pop an element from the front of the linked list, next element becomes the new head
 * 
 * Since partial writes are queued, we pop the first element returning it,
 * and the next element becomes the new head of the list.
 * 
 * @param pw Pointer to the head of the linked list
 * @return struct s_partial_write* previous head of the list
 */
struct s_partial_write *pop_partial_write(struct s_partial_write **pw);

int		send_message(int fd, enum e_message_types type, const char *content, size_t length);

#endif