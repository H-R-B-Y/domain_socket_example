/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_connection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 10:59:40 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/04 10:59:48 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	create_new_connection(struct s_server *srv)
{
	struct s_connection	*child;
	struct s_cdll_node	*node;
	int					child_addr;
	struct epoll_event	ev = {0};
	
	child_addr = accept(srv->server_fd, NULL, NULL);
	if (child_addr < 0)
		terminate_srv(srv, "Failed to create connection", 1);
	child = ft_calloc(1, sizeof(struct s_connection));
	if (!child)
	{
		close(child_addr);
		terminate_srv(srv, "Failed to allocate connection", 1);
	}
	(*child) = (struct s_connection){.fd = child_addr, .epoll_parent = 0};
	node = cdll_init_node(child);
	if (!node)
	{
		terminate_child(child);
		terminate_srv(srv, "Failed to allocate connection node", 1);
	}
	if (!make_fd_nonblocking(child_addr))
	{
		terminate_child(child);
		terminate_srv(srv, "Failed to make child non blocking", 1);
	}
	// Add the new child to the servers epoll list
	ev.data.fd = child_addr;
	ev.events = EPOLLIN;
	if (epoll_ctl(srv->epoll_fd, EPOLL_CTL_ADD, child_addr, &ev) < 0)
	{
		terminate_child(child);
		terminate_srv(srv, "Failed to add connection to epoll list", 1);
	}
	child->epoll_parent = srv->epoll_fd;
	cdll_push_back(srv->connections, node);
	printf("New connection added to connections list %d\n", child_addr);
	return ;
}
