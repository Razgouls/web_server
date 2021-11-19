/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 13:59:32 by elie              #+#    #+#             */
/*   Updated: 2021/11/19 10:10:28 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONF_HPP
# define SERVERCONF_HPP

# include "Utils.hpp"
# include "Route.hpp"

class	ServerConf
{
	public:
		ServerConf();
		ServerConf(int listen, std::string host, std::string server_name, std::string root, std::list<Route> list_route,
					std::map<int, std::string> map_error, int limit_body_size);
		ServerConf(const ServerConf &serverConf);
		~ServerConf();

		ServerConf							&operator=(const ServerConf &serverConf);

		void								set_listen(std::string &listen);
		void								set_host(std::string &host);
		void								set_server_name(std::string &server_name);
		void								set_root(std::string &root);
		void								set_list_routes(Route &r);
		void								set_map_error(std::pair<int, std::string> &error);
		void								set_limite_body_size(std::string &limit_body_size);

		int									&get_listen(void);
		std::string							&get_host(void);
		std::string							&get_server_name(void);
		std::string							&get_root(void);
		std::list<Route>					&get_list_routes(void);
		std::map<int, std::string>			&get_map_error(void);
		int									&get_limite_body_size(void);

	private:
		int									_listen;
		std::string							_host;
		std::string							_server_name;
		std::string							_root;
		std::list<Route>					_list_routes;
		std::map<int, std::string>			_map_error;
		int									_limit_body_size;
};

std::ostream& 								operator<<(std::ostream &os, ServerConf &f);

#endif