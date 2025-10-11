/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_terminal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:35:31 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/11 13:24:50 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_server_int.h"

int	_handle_terminal(struct s_server *srv)
{
	char	*str;
	/*
	Not sure what to do with the terminal input here
	*/
	(void)srv;
	str = get_next_line(STDIN_FILENO);
	if (!str)
		server_stop(srv);
	else
	{
		struct s_header_chunk header;
		header.content_length = ft_strlen(str) + 1;
		header.msg_type = MTYPE_STR;
		send_to_room(srv, &srv->server_room, &header, str);
		free(str);
	}
	return (0);
}
