/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 15:02:56 by hbreeze           #+#    #+#             */
/*   Updated: 2025/10/04 11:01:15 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "server.h"

int main(void)
{
	struct s_server	srv = {0};
	// first we want to ensure that we are a singleton
	setup_signal_handler();
	if (socket_exists())
	{
		dprintf(STDERR_FILENO, "Socket file %s already exists\nTerminating\n", SOCKET_PATH);
		return (1);
	}
	if (!init_server(&srv))
	{
		dprintf(STDERR_FILENO, "Terminating\n");
		return (1);
	}
	main_loop(&srv);
	terminate_srv(&srv, "Ok!", 0);
	return (0);
}
