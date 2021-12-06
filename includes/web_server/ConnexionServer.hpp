/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionServer.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:51:57 by elie              #+#    #+#             */
/*   Updated: 2021/12/05 22:03:53 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNEXIONSERVER_HPP
# define CONNEXIONSERVER_HPP

# include "Utils.hpp"
# include "Server.hpp"

# define RECV_SIZE 131072
# define SIZE_PFDS 1000
# define MAX_CONN 200000

class ConnexionServer
{
	public:
		ConnexionServer();
		~ConnexionServer();

		void													run(void);
		int														manage_connexion(int &fd);
		int														s_accept(int j);
		void													init_pfds(void);
		void													init_listen_fd(void);
		int														init_poll(int *nfds);
		int														get_pos_socket(void);
		void													compress_array(int *nfds);

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