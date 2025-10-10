/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sock_client_int.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 18:02:51 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/10 16:32:49 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCK_CLIENT_INT_H
# define SOCK_CLIENT_INT_H

# include "shared_resource.h"
# include "sock_client_framework.h"

void	_terminate_client(struct s_client *client, const char *msg, int exit_code, int show_errno);

int		_on_connect(struct s_client *client);

int		_handle_client_msg_recv(struct s_client *client);

int		_handle_client_msg_send(struct s_client *client);

int		_handle_stdin(struct s_client *client);

#endif
