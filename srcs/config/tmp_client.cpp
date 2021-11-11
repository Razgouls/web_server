/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp_client.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 16:08:07 by elie              #+#    #+#             */
/*   Updated: 2021/10/21 14:12:43 by elie             ###   ########.fr       */
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

int     main(void)
{
	int     socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	char	hello[] = "Hello";
	char	buffer[1024] = { 0 };
	struct  sockaddr_in server_addr;

	if (socket_fd < 0)
	{
		std::cout << "Erreur lors de la creation du socket client" << std::endl;
		return (-1);
	}
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);									//port du server
	inet_pton(AF_INET, "192.168.1.29", &server_addr.sin_addr);			//adresse IP du serveur

	if (connect(socket_fd, (struct sockaddr *)&server_addr, (socklen_t)sizeof(server_addr)) < 0)
	{
	    std::cout << "Erreur lors du connect" << std::endl;
	    return (-1);
	}
	send(socket_fd, &hello, strlen(hello), 0);
	std::cout << "Message envoyé au server !" << std::endl;
	recv(socket_fd, buffer, 1024, 0);
	std::cout << buffer << std::endl;
	close(socket_fd);
}