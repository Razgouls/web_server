/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionServer.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:51:57 by elie              #+#    #+#             */
/*   Updated: 2021/11/25 15:07:34 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNEXIONSERVER_HPP
# define CONNEXIONSERVER_HPP

# include "Utils.hpp"
# include "Server.hpp"

# define RECV_SIZE 131072
# define SIZE_PFDS 20
# define MAX_CONN 2000

class ConnexionServer
{
	public:
		ConnexionServer();
		~ConnexionServer();

		void													run();
		int														manage_connexion(int &fd);
		int														s_accept(int j);
		void													init_pfds(void);
		void													init_listen_fd(void);
		int														init_poll(int *nfds);
		int														get_pos_socket(void);

		void													set_servers(std::vector<Server> &servers);

	private:
		std::map<int, std::string>								_requests;
		std::vector<struct sockaddr_in>							_vect_address;
		std::vector<int>										_vect_listen_fd;
		std::map<int, Server>									_map_fd_server;								
		struct pollfd											_pfds[SIZE_PFDS];
		int														_index;
		std::vector<Server>										_servers;
};

#endif