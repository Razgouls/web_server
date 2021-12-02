/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 10:37:15 by elie              #+#    #+#             */
/*   Updated: 2021/12/02 22:18:40 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Utils.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Route.hpp"
# include "CGI.hpp"

# define INDEX 0
# define AUTOINDEX 1
# define NONE 2

class Server
{
	public:
		Server();
		~Server();
		Server													&operator=(const Server &s);
		Server(const Server &s);

		int														c_recv(std::string &request);
		void													init_mime(void);
		void													init_page_error(void);

		/*
		** CGI
		*/
		void													init_var_cgi(void);
		void													manage_cgi(void);

		/*
		** REQUEST
		*/
		int														manage_request(std::string &request);
		void													parse_request(Request &r);
		bool													check_method_location(void);
		void													get_path_location(void);
		int														manage_auto_index(void);
		std::string												get_upload_dir(void);
		void													multipart(std::string &tmp_path);
		void													get_content_multipart(std::string &element, std::string &tmp_path);
		void													create_file(std::string &tmp_path);
		int														update_file(std::string &tmp_path);

		/*
		** MANAGE METHODS
		*/
		void													get_resource(void);
		void													post_resource(void);
		void													delete_resource(void);
		void													put_resource(void);

		/*
		** RESPONSE
		*/
		void													manage_reponse(void);
		void													fill_reponse(void);
		std::list<std::pair<std::string, unsigned char> >		get_file_in_dir(DIR *dir);

		/*
		** GETTEURS
		*/
		int														&get_port(void);
		std::string												&get_host(void);
		std::string												&get_server_name(void);
		std::string												&get_root(void);
		std::list<Route>										&get_list_routes(void);
		std::map<int, std::string>								&get_map_error(void);
		int														&get_limite_body_size(void);
		Response												&get_reponse(void);

		/*
		** SETTEURS
		*/
		void													set_port(std::string &listen);
		void													set_host(std::string &host);
		void													set_server_name(std::string &server_name);
		void													set_root(std::string &root);
		void													set_list_routes(std::list<Route> list_route);
		void													set_map_error(std::map<int, std::string> &map_error);
		void													set_limite_body_size(std::string &limit_body_size);

	private:
		Request													_request;
		Response												_reponse;
		Route													_route;
		std::map<std::string, std::string>						_mime;
		CGI														_cgi;

		int														_port;
		std::string												_host;
		std::string												_server_name;
		std::string												_root;
		std::list<Route>										_list_routes;
		std::map<int, std::string>								_map_error;
		int														_limit_client_body_size;

};

std::ostream& 													operator<<(std::ostream &os, Server &f);

#endif