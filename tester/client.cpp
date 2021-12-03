/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 11:36:08 by elie              #+#    #+#             */
/*   Updated: 2021/12/03 17:45:50 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"
#include "Post.hpp"
#include "Delete.hpp"
#include "Header.hpp"
#include "File_config.hpp"
#include "Requete_tester.hpp"

int		new_client(void)
{
	int					socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	std::string			requete;
	struct sockaddr_in	server_addr;

	if (socket_fd < 0)
	{
		std::cout << "Erreur lors de la creation du socket client" << std::endl;
		return (-1);
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);									//port du server
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);				//adresse IP du serveur

	if (connect(socket_fd, (struct sockaddr *)&server_addr, (socklen_t)sizeof(server_addr)) < 0)
	{
		std::cout << "Erreur lors du connect" << std::endl;
		return (-1);
	}
	return (socket_fd);
}

void	print_body_recv(char buffer[131000])
{
	std::string	str;
	int			i = -1;

	while (buffer[++i])
		str.append(1, buffer[i]);
	std::cout << BOLDRED << "Recv Headers :" << std::endl << "[" << WHITE << str.substr(0, str.find("\r\n\r\n")) << BOLDRED << "]" << WHITE << std::endl << std::endl;
	if (!str.empty() && str.find("\r\n\r\n") != std::string::npos && str.substr(str.find("\r\n\r\n") + 4).length() > 1000)
		std::cout << BOLDRED << "Recv Body :" << std::endl << "[" << WHITE << "Body Is Too Long" << BOLDRED << "]" << WHITE << std::endl << std::endl;
	else if (!str.empty())
		std::cout << BOLDRED << "Recv Body :" << std::endl << "[" << WHITE << str.substr(str.find("\r\n\r\n") + 4) << BOLDRED << "]" << WHITE << std::endl << std::endl;
}

void	manage_requete(Requete_tester &rt, std::string &requete)
{
	char			buffer[131000] = { 0 };
	int				new_fd = new_client();

	if (new_fd == -1)
		return ;
	rt.affiche();

	send(new_fd, requete.c_str(), requete.length(), 0);
	recv(new_fd, buffer, 131000, 0);
	close(new_fd);
	print_body_recv(buffer);
}

void	send_get(void)
{
	Get				get;
	std::string		requete;

	get.get_file_basic(requete);
	manage_requete(get, requete);

	get.get_file_basic_chunked(requete);
	manage_requete(get, requete);

	get.get_file_no_perm(requete);
	manage_requete(get, requete);

	get.get_file_no_found(requete);
	manage_requete(get, requete);

	get.get_dir_index_exist(requete);
	manage_requete(get, requete);

	get.get_dir_autoindex_on(requete);
	manage_requete(get, requete);

	get.get_dir_autoindex_off(requete);
	manage_requete(get, requete);

	get.get_dir_redirection(requete);
	manage_requete(get, requete);

	get.get_cgi_empty(requete);
	manage_requete(get, requete);

	get.get_cgi_hello(requete);
	manage_requete(get, requete);

	if (LONG)
	{
		get.get_cgi_long(requete);
		manage_requete(get, requete);
	}

	get.get_cgi_chunked(requete);
	manage_requete(get, requete);

	get.get_cgi_multi_chunked(requete);
	manage_requete(get, requete);
}

void	send_post(void)
{
	Post			post;
	std::string		requete;

	post.post_no_file(requete);
	manage_requete(post, requete);

	post.post_create_file_empty(requete);
	manage_requete(post, requete);

	post.post_create_file_data(requete);
	manage_requete(post, requete);

	post.post_update_file(requete);
	manage_requete(post, requete);

	post.post_update_file_no_data(requete);
	manage_requete(post, requete);
}

void	send(void)
{
	// send_get();
	send_post();
}

int		main(void)
{
	send();
	return (0);
}