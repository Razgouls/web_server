/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 22:02:36 by elie              #+#    #+#             */
/*   Updated: 2021/11/28 11:31:21 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSERSERVER_HPP
# define PARSERSERVER_HPP

# include "Utils.hpp"
# include "ParserRoute.hpp"

class ParserServer
{
	public:
		ParserServer();
		~ParserServer();

		void								set_head(std::string &line);
		void								add_data_map(std::string &key, std::string &value);
		void								add_data_map_error(std::pair<int, std::string> p);
		void								add_data_v(ParserRoute &pa);
		void								add_cgi_bin(std::string &bin);
		void								add_cgi_extension(std::string &extension);
		std::ostream&						print(std::ostream &os);
		void								clear(void);

		std::string							&get_head(void);
		std::map<std::string, std::string>	&get_m_parser(void);
		std::map<int, std::string>			&get_map_error(void);
		std::vector<ParserRoute>			&get_parser_route(void);

		void								head_valid(void);
		void								port_valid(void);
		void								host_valid(void);
		void								name_valid(void);
		void								root_valid(void);
		void								limit_body_size_valid(void);
		void								error_page_valid(void);
		void								route_valid(void);
		void								map_parser_valid(void);
		void								check_key_m_parser(std::string mess, std::string key);
		void								server_valid(void);

	private:
		std::map<std::string, std::string>	_m_parser;
		std::map<int, std::string>			_m_error;
		std::list<std::string>				_l_cgi_bin;
		std::list<std::string>				_l_cgi_extension;
		std::vector<ParserRoute>			_v_route;
		std::string							_head;
};

std::ostream& 						operator<<(std::ostream &os, ParserServer &pa);

#endif