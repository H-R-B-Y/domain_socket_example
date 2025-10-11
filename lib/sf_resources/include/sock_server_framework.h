/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sock_server_framework.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 11:04:39 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/08 14:18:27 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCK_SERVER_FRAMEWORK_H
# define SOCK_SERVER_FRAMEWORK_H

#include "shared_resource.h"

typedef t_u64	t_connection_id;

/*
Connection represents a single connection to the server
and is not neceserilly a unique user, depends on implementation.
*/
struct s_connection
{
	/// @brief Unique id for the connection
	t_connection_id			id;
	/// @brief FD for the connection
	int						fd;
	/// @brief Parent epoll list, required to remove fd from epoll
	int						epoll_parent;

	/// @brief Partial read data
	struct s_partial_read	read_state;
	/// @brief Partial write data queue, allocated linked list of partial writes
	struct s_partial_write	*write_queue_head;
	/// @brief When connection was made
	time_t					connected_at;
	/// @brief Data specific to implementation
	void					*user_data;
	/// @brief function to free the user data
	t_freefn				cleanup_user_data;
};

/*
Should be internal only
*/
void	delete_connection(struct s_connection *conn);
struct s_connection	*new_connection(int fd, int epoll_parent);
/*
We want some sort of way to identify connects from either there fd
or there connection id, needs to be reversable lookup.

we also need to keep track of available id's so that when connections
are terminated we can re-use old id's

Two hashmaps lol, and a min heap for reclaimation
*/
struct s_connection_lookup
{
	/// @brief ID to connection lookup
	t_hashmap			id_to_connection;
	/// @brief FD to connection lookup
	t_hashmap			fd_to_connection;
	/// @brief Next available id reclaimation
	t_heap				available_ids;
	/// @brief Next available id if no reclaimation
	t_connection_id		next_id;
	/// @brief All the connection (for cleanup)
	t_cdll				connections;
};

/*
Generic functions for the connection lookup go here..
*/
/**
 * @brief Create a new connection lookup structure
 * @return struct s_connection_lookup
 */
int	connection_lookup_init(struct s_connection_lookup *lookup);
/**
 * @brief Destroy a connection lookup structure
 * Should not be done until we are sure that all connection references have
 * been cleaned up in other places, this will disconnect all clients.
 * @param lookup The lookup to destroy
 * @return void
 */
void						connection_lookup_destroy(struct s_connection_lookup *lookup);

/**
 * @brief Add a new connection to the lookup
 * The connection should be allocated by the server at join time,
 * then passed to this function to be assigned a new id and added to the lookup.
 * The connection will be owned by the lookup after this call, and should not be
 * freed by the caller.
 * @param lookup The lookup to add the connection to
 * @param conn The connection to add, should be allocated by the caller
 * @return t_connection_id The id assigned to the connection, or 0 on error
 */
t_connection_id				connection_add(struct s_connection_lookup *lookup, struct s_connection *conn);

/**
 * @brief Get a connection by id
 * @param lookup The lookup to search
 * @param id The id of the connection to find
 * @return struct s_connection* The connection, or NULL if not found
 */
struct s_connection			*get_connection_by_id(struct s_connection_lookup *lookup, t_connection_id id);
/**
 * @brief Get a connection by fd
 * @param lookup The lookup to search
 * @param fd The fd of the connection to find
 * @return struct s_connection* The connection, or NULL if not found
 */
struct s_connection			*get_connection_by_fd(struct s_connection_lookup *lookup, int fd);

/**
 * @brief Remove a connection from the lookup by id, fd or reference
 * This will remove the connection completely from the lookup, this should really only be done
 * when the connection is terminated and should only really be called by the server, as we need to ensure
 * the connection does not have references in other places.
 * This will free the connection and remove it from the lookup.
 * This will only free the user data if the free function is set.
 * @param lookup The lookup to remove the connection from
 * @param id The id of the connection to remove
 * @param fd The fd of the connection to remove
 * @param conn The connection to remove
 * @return int 1 on success, 0 on failure (not found)
 */
int							connection_remove_by_id(struct s_connection_lookup *lookup, t_connection_id id);
int							connection_remove_by_fd(struct s_connection_lookup *lookup, int fd);
int							connection_remove_by_ref(struct s_connection_lookup *lookup, struct s_connection *conn);

/*
Abstract message base struct:
This will be constructed by the server and sent to the installed handlers.
I think we also need to expand this to inlcude the prechunk, because that will 
eventually tell us which version of the header to use?!?!?!?!
*/
struct s_message
{
	struct s_connection		*sender;
	struct s_header_chunk	*header;
	char					*content;
	time_t					recv_at;
};

/*
The server generic
*/
// Forward decleration=
struct s_server;

/*
Ran on initial connection to the server.
Return 1 if everything is ok
Return 0 if there was an error (server will cleanup)
*/
typedef int		(*t_on_connect_fn)(struct s_server *srv, struct s_connection *conn, void *appdata);

/*
Ran on when a full message has been read from the server
Return 1 if everything is ok
Return 0 if there was an error (server will cleanup)
*/
typedef int		(*t_on_message_fn)(struct s_server *srv, struct s_message *message, void *appdata);

/*
Ran when client has been disconnnected from the server (for any reason)
*/
typedef void	(*t_on_disconnect_fn)(struct s_server *srv, struct s_connection *conn, void *appdata);

/*
How do we handle disconnects from rooms (threads)
need to think about this:
could have a pipe for each fork (not super ideal as we will end up with sooo many fds).
could use MPMC ring buffer for an event queue, main thread just picks up the disconnect events
and handles them in the main thread, threads just unhook the connection from their own even loop.
*/
struct s_server
{
	int							server_fd;
	int							epoll_fd;
	struct sockaddr_un			addr;
	struct s_connection_lookup	connection_data;

	/// @brief connections that are in the main server room, not in any other room.
	/// Will recieve messages broadcast to the server and global messages only.
	t_cdll						server_room;

	void						*appdata;
	t_freefn					free_appdata;
	
	/*
	Then we should setup some generic handlers here
	*/
	t_on_connect_fn				on_connect;
	t_on_disconnect_fn			on_disconnect;
	t_on_message_fn				on_message;

	/*
	Should also have some kind of event queue, that can be pushed too
	from threads or something
	*/
	t_mpmc_ringbuff				event_queue; // implement later

	t_vu32						running;
};

/*
Server api
*/
/**
 * @brief Initialize the server structure
 * This will create the socket, bind it and set it to listen.
 * It will also initialize the connection lookup structure and all other
 * internal data structures.
 * @param srv The server structure to initialize
 * @param socket_path The path to the socket to create
 * @return int 0 on success, -1 on failure
 */
int		server_init(struct s_server *srv, const char *socket_path);

/**
 * @brief Run the server main loop
 * This will block and run the server main loop, handling connections and messages.
 * It will return when the server is shutdown.
 * @param srv The server structure to run
 * @return void
 */
void	server_run(struct s_server *srv);

/**
 * @brief Hard shutdown the server
 * This will cleanup everything, at this point we should expect all threads 
 * to be terminated and all connections closed.
 * All resources will be freed and the socket file will be removed.
 * @param srv The server structure to shutdown
 * @return void
 */
void	server_shutdown(struct s_server *srv);

/**
 * @brief Stop the server main loop
 * This will cause the server main loop to exit and return from server_run.
 * This will not cleanup any resources, that is done in server_shutdown.
 * @param srv The server structure to stop
 * @return void
 */
void	server_stop(struct s_server *srv);

/*
Connection management
*/

/**
 * @brief Disconnect a connection by id
 * This will remove the connection from the server and cleanup all resources.
 * It will also call the on_disconnect handler if it is set.
 * The connection does not need to be in the server room, but must be a registered connection.
 * Must ensure that any threads or rooms that have references to this connection
 * remove them before calling this function.
 * @param srv The server structure
 * @param id The id of the connection to disconnect
 * @return int 0 on success, -1 on failure (not found)
 */
int	disconnect_connection(struct s_server *srv, t_connection_id id);


/*
Helper functions
*/
/**
 * @brief Send a message to all connections
 * This will send the message to all connections in the connections list.
 * This is a blocking call, it will not return until all messages are sent.
 * If there is an error sending to any connection, it will continue sending to the rest.
 * TODO: Implement partial writes and non-blocking sends
 * @param srv The server structure
 * @param header The header chunk to send
 * @param content The content to send, can be NULL if no content
 * @param size The size of the content, can be 0 if no content
 * @return int 0 on success, -1 on failure
 */
int		send_global_message(struct s_server *srv, struct s_header_chunk *header, void *content, size_t size);

int	send_to_connection_ref(
	struct s_server *srv, 
	struct s_connection *conn,
	struct s_header_chunk *header,
	void *content
);

int	send_to_connection_id(
	struct s_server *srv,
	t_connection_id id,
	struct s_header_chunk *header,
	void *content
);

/**
 * @brief Send a message to a room of connections
 * 
 * @param srv The server instance
 * @param room The room to send the message too (cdll of connections)
 * @param header The message header (must have content length populated)
 * @param content The content of the message
 * @return int 0 on success, -1 on failure
 */
int		send_to_room(struct s_server *srv, t_cdll *room, struct s_header_chunk *header, void *content);
/*
ToDo:
- Implement the code
- figure out partial writes
- update cdll code to have find functions and iterate functions etc.
- document lifetimes
- limits for reading

*/
#endif
