/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 15:30:20 by elie              #+#    #+#             */
/*   Updated: 2021/10/21 16:54:35 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>

# define PORT 8081

int		main(void)
{
	int					server_fd = socket(AF_INET, SOCK_STREAM, 0);
	int					new_socket;
	struct sockaddr_in	address;
	int					len = sizeof(address);
	char				buffer[1024] = { 0 };
	char				hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 32\n\nHello et Bonjour tout le monde !" ;
	int					ret_read;

	if (server_fd < 0)
	{
		std::cout << "Erreur lors de la creation de la socket" << std::endl;
		return (-1);
	}
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = INADDR_ANY;
	
	char buffer_tmp2[1024];
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cout << "Erreur lors du bind" << std::endl;
		return (-1);
	}
	if (listen(server_fd, 3) < 0)
	{
		std::cout << "Erreur lors du listen" << std::endl;
		return (0);
	}
	std::cout << inet_ntop(AF_INET, &address.sin_addr, buffer_tmp2, sizeof(buffer_tmp2)) << std::endl;
	while (1)
	{
		std::cout << "=========== Waiting for the connection ===========" << std::endl;
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&len)) < 0)
		{
			std::cout << "Erreur lors du accept" << std::endl;
			return (0);
		}
		char buffer_tmp[1024];
		std::cout << inet_ntop(AF_INET, &address.sin_addr, buffer_tmp, sizeof(buffer_tmp)) << std::endl;
		std::cout << "BUG : " << new_socket << std::endl;
		ret_read = recv(new_socket, buffer, 1024, 0);
		std::cout << "[Voila ce que jai recu en requete : " << std::endl << buffer << "]" << std::endl;
		if (ret_read < 0)
		{
			std::cout << "Erreur lors du recv" << std::endl;
			return (ret_read);
		}
		std::cout << buffer << std::endl;
		send(new_socket, hello, 1024, 0);
		std::cout << "Message envoyé au client !" << std::endl << std::endl;
		close(new_socket);
	}
	return (0);
}