/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sock_server_int.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 11:29:23 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/08 16:44:17 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCK_SERVER_INT_H
# define SOCK_SERVER_INT_H

# include "shared_resource.h"
# include "sock_server_framework.h"


void	_terminate_srv(struct s_server *srv, const char *msg, int exit_code, int show_errno);

/*
Move the connection lookup here
move some other stuff here because it should not be exposed to the user
*/


/*
Handler functions:

Each handler should return some code:
0 = OK!
-1 = check errno
1 = internal error
*/

/*
This will be called when a new connection is notified from epoll.
This will create a new connection struct, add it to the connections list
and then call the on_connect handler if it is set.
*/
int	_on_join_handler(struct s_server *srv, int fd);

/*
Called when a message is notified from epoll.
This will read the message from the socket, or partial read if needed, multiple 
epolls can be needed to read the full message.
When the full message is read, the on message handler will be called if it is set.
*/
int	_handle_msg(struct s_server *srv, struct s_connection *conn);

int	_handle_write(struct s_server *srv, struct s_connection *conn);

/*
Not sure how this one will work yet, because sometimes a disconnect is 
just a read, which would be handled through handle_msg.
So i guess handle msg will return some identifier for this,
then the server can call this function to handle the disconnect.
and call the on disconnect handler if it is set.
*/
int	_handle_disconnect(struct s_server *srv, struct s_connection *conn);


int	_handle_terminal(struct s_server *srv);

#endif
