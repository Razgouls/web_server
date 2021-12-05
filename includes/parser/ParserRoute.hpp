/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserRoute.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 22:07:22 by elie              #+#    #+#             */
/*   Updated: 2021/12/05 18:08:33 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSERROUTE_HPP
# define PARSERROUTE_HPP

# include "Utils.hpp"

extern int PRINT;
extern int PRINT_CONFIG;

class ParserRoute
{
	public:
		ParserRoute();
		~ParserRoute();

		void								set_path(std::string path);
		void								add_data_map(std::string &key, std::string &value);
		void								print(void);
	
		std::string							&get_path(void);
		std::map<std::string, std::string>	&get_m_parser(void);
		std::list<std::string>				&get_methods(void);
		std::list<std::string>				&get_cgi_bin(void);
		std::list<std::string>				&get_cgi_extension(void);

		void								path_valid(std::string &path_server);
		void								index_valid(std::string &path_server);
		void								method_valid(void);
		void								autoindex_valid(void);
		void								upload_dir_valid(void);
		void								cgi_extension_valid(void);
		void								cgi_bin_valid(void);
		void								route_valid(std::string &path_server);
		bool								check_key_m_parser(std::map<std::string, std::string>	&map_tmp, std::string key);

	private:
		std::map<std::string, std::string>	_m_parser;
		std::list<std::string>				_l_method;
		std::list<std::string>				_l_cgi_bin;
		std::list<std::string>				_l_cgi_extension;
		std::string							_path;
};

std::ostream& 						operator<<(std::ostream &os, ParserRoute &pr);

#endif