/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_resource.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:22:40 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/03 16:31:44 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_RESOURCE_H
# define SHARED_RESOURCE_H

# include <stddef.h>

/*
Data used for data sent between the sockets
*/
# define PRE_HEADER_CHUNK sizeof(int)
# define HEADER_CHUNK sizeof(struct s_header_chunk)

struct s_header_chunk
{
	enum {
		MTYPE_NONE,
		MTYPE_STR,
		MTYPE_TYPE_COUNT
	}	msg_type;
	size_t	content_length;
};

#endif
