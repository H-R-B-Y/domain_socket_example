/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection_lookup_get.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 15:22:00 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/05 15:22:17 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sock_internal.h"

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
