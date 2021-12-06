/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 11:36:08 by elie              #+#    #+#             */
/*   Updated: 2021/12/06 01:25:15 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"
#include "Post.hpp"
#include "Delete.hpp"
#include "Header.hpp"
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
	{
		std::cout << "ERROR CREATION SOCKET" << std::endl;
		return ;
	}
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
	char			c;

	get.get_file_basic(requete);
	manage_requete(get, requete);
	std::cin >> c;

	get.get_file_basic_chunked(requete);
	manage_requete(get, requete);
	std::cin >> c;

	chmod("./tester/www/get/no_perm.html", 0);
	get.get_file_no_perm(requete);
	manage_requete(get, requete);
	chmod("./tester/www/get/no_perm.html", S_IRWXU);
	std::cin >> c;

	get.get_file_no_found(requete);
	manage_requete(get, requete);
	std::cin >> c;

	get.get_dir_index_exist(requete);
	manage_requete(get, requete);
	std::cin >> c;

	get.get_dir_autoindex_on(requete);
	manage_requete(get, requete);
	std::cin >> c;

	get.get_dir_autoindex_off(requete);
	manage_requete(get, requete);
	std::cin >> c;

	get.get_dir_redirection(requete);
	manage_requete(get, requete);
	std::cin >> c;

	get.get_cgi_empty(requete);
	manage_requete(get, requete);
	std::cin >> c;

	get.get_cgi_hello(requete);
	manage_requete(get, requete);
	std::cin >> c;

	get.get_cgi_data_sh(requete);
	manage_requete(get, requete);
	std::cin >> c;

	if (LONG)
	{
		get.get_cgi_long(requete);
		manage_requete(get, requete);
	std::cin >> c;
	}

	get.get_cgi_chunked(requete);
	manage_requete(get, requete);
	std::cin >> c;

	get.get_cgi_chunked_bad_length(requete);
	manage_requete(get, requete);
	std::cin >> c;

	get.get_cgi_multi_chunked(requete);
	manage_requete(get, requete);
	std::cin >> c;
}

void	send_post(void)
{
	Post			post;
	std::string		requete;
	char			c;

	post.post_no_file(requete);
	manage_requete(post, requete);
	std::cin >> c;

	post.post_create_file_empty(requete);
	manage_requete(post, requete);
	std::cin >> c;

	post.post_create_file_data(requete);
	manage_requete(post, requete);
	std::cin >> c;

	post.post_create_file_data_bad_length(requete);
	manage_requete(post, requete);
	std::cin >> c;

	post.post_update_file(requete);
	manage_requete(post, requete);
	std::cin >> c;

	post.post_update_file_no_length(requete);
	manage_requete(post, requete);
	std::cin >> c;

	post.post_update_file_no_length2(requete);
	manage_requete(post, requete);
	std::cin >> c;

	post.post_update_file_no_data(requete);
	manage_requete(post, requete);
	std::cin >> c;
	
	post.post_cgi_no_data(requete);
	manage_requete(post, requete);
	std::cin >> c;

	post.post_cgi_data(requete);
	manage_requete(post, requete);
	std::cin >> c;

	post.post_create_file_data_chunked(requete);
	manage_requete(post, requete);
	std::cin >> c;

	post.post_create_file_data_multi_chunked(requete);
	manage_requete(post, requete);
	std::cin >> c;

	post.post_cgi_data_chunked(requete);
	manage_requete(post, requete);
	std::cin >> c;

	post.post_cgi_data_multi_chunked(requete);
	manage_requete(post, requete);
	std::cin >> c;
}

void	send_delete(void)
{
	Delete			m_delete;
	std::string		requete;
	char			c;

	m_delete.delete_file_exist(requete);
	manage_requete(m_delete, requete);
	std::cin >> c;

	m_delete.delete_file_not_exist(requete);
	manage_requete(m_delete, requete);
	std::cin >> c;

	chmod("./tester/www/delete/noperm.html", 0);
	m_delete.delete_file_no_perm(requete);
	manage_requete(m_delete, requete);
	chmod("./tester/www/delete/noperm.html", S_IRWXU);
	std::cin >> c;

	m_delete.delete_dir(requete);
	manage_requete(m_delete, requete);
	std::cin >> c;
}

void	send_headers(void)
{
	Header			header;
	std::string		requete;
	char			c;

	header.header_bad_method(requete);
	manage_requete(header, requete);
	std::cin >> c;

	header.header_empty_method(requete);
	manage_requete(header, requete);
	std::cin >> c;

	header.header_bad_version(requete);
	manage_requete(header, requete);
	std::cin >> c;

	header.header_empty_version(requete);
	manage_requete(header, requete);
	std::cin >> c;

	header.header_empty_first_line(requete);
	manage_requete(header, requete);
	std::cin >> c;

	header.header_two_infos(requete);
	manage_requete(header, requete);
	std::cin >> c;

	header.header_four_infos(requete);
	manage_requete(header, requete);
	std::cin >> c;

	header.header_two_first_line(requete);
	manage_requete(header, requete);
	std::cin >> c;

	header.header_host_bad_ip(requete);
	manage_requete(header, requete);
	std::cin >> c;

	header.header_host_empty_ip(requete);
	manage_requete(header, requete);
	std::cin >> c;

	header.header_host_no_exist(requete);
	manage_requete(header, requete);
	std::cin >> c;

	header.header_empty_host(requete);
	manage_requete(header, requete);
	std::cin >> c;

	header.header_host_three_infos(requete);
	manage_requete(header, requete);
	std::cin >> c;

	header.header_two_host(requete);
	manage_requete(header, requete);
	std::cin >> c;
}

void	send(void)
{
	// send_get();
	send_post();
	// send_delete();
	// send_headers();
	remove("./tester/www/post/data_chunked.html");
	remove("./tester/www/post/data_multi_chunked.html");
	remove("./tester/www/post/data.html");
	remove("./tester/www/post/empty.html");
	
	std::string filename = "./tester/www/delete/file.txt";
	std::ofstream o(filename.c_str());
	o << "DELETE" << std::endl;
	o.close();
}

void	help()
{
	std::cout << "Utilisation : ./web_test [option]" << std::endl << "Options :" << std::endl;
	std::cout << "  -conf\t\t\tLancez le tester sur les fichiers de configuration." << std::endl;
	std::cout << "  -h\t\t\tAffichez ce message et quitter." << std::endl;
	std::cout << "  -webserv\t\tLancez le tester sur le webserver." << std::endl;
}

int		main(int argc, char **argv)
{
	(void)argc;
	char	c;
	
	if (argc != 2)
	{
		help();
		return (1);
	}
	if (strcmp(argv[1], "-h") == 0)		//help
	{
		help();
		return (0);
	}
	else if (strcmp(argv[1], "-conf") == 0)
		system("./tester/file_config/launch_config.sh");
	else if (strcmp(argv[1], "-webserv") == 0)
	{
		std::cout << "Veuillez lancer le webserver avec la commande /webserv tester/file_config/default_tester.conf" << std::endl;
		std::cout << "Appuyez sur 'c' une fois lancé" << std::endl;
		std::cin >> c;
		if (c == 'c')
			send();
	}
	else
		help();
	return (0);
}