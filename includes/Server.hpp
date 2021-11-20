/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 10:37:15 by elie              #+#    #+#             */
/*   Updated: 2021/11/20 11:00:23 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Utils.hpp"
# include "ServerConf.hpp"
# include "Request.hpp"
# include "Response.hpp"

# define RECV_SIZE 131072
# define SIZE_PFDS 20
# define MAX_CONN 2000

class Server
{
	public:
		Server();
		Server(const Server &s);
		~Server();

		Server													&operator=(const Server &s);

		std::vector<ServerConf>									&get_list_server(void);
		int														&get_server_fd(void);
		struct sockaddr_in										&get_address(void);

		void													set_list_server(std::vector<ServerConf> &list_server);
		void													set_server_fd(int server_fd);
		void													set_address(struct sockaddr_in &address);
		void													set_map_error(std::map<int, std::string> &_map_error);

		void													run(void);
		void													run2(void);
		int														s_recv(int &fd, int i, int *nfds);
		int														s_send(int i, int *nfds);
		// int													s_accept();
		int														s_accept(int j);
		void													init_pfds(void);
		void													init_listen_fd(void);
		void													parse_request(Request &r);
		int														init_poll(int *nfds);
		void													gestion_file_dir(void);
		void													get_req_route(void);
		bool													gestion_valid_method(void);
		std::pair<bool, bool>									gestion_auto_index(void);
		std::list<std::pair<std::string, unsigned char> >		get_file_in_dir(DIR *dir);
		void													delete_resource(void);
		void													post_resource(void);
		void													get_resource(void);
		void													put_resource(void);
		void													fill_current_rep(void);
		void													get_index(void);
		int														get_pos_socket(void);
		void													init_mime(void);
		void													clear(void);



	private:
		Request													_current_req;
		Response												_current_rep;
		Route													_current_route;
		std::map<int, std::string>								_current_map_error;
		std::vector<ServerConf>									_list_server;
		int														_server_fd;
		struct sockaddr_in										_address;
		struct sockaddr_in										_address2;
		std::vector<struct sockaddr_in>							_vect_address;
		std::vector<int>										_vect_listen_fd;
		struct pollfd											_pfds[SIZE_PFDS];
		int														_listen_fd;
		int														_listen_fd2;
		int														_index;
		std::map<std::string, std::string>						_mime;

		int								get_socket_fd(void);	
};

#endif