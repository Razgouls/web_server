/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 21:17:17 by elie              #+#    #+#             */
/*   Updated: 2021/11/28 22:39:32 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILECONFIG_HPP
# define FILECONFIG_HPP

# include "Utils.hpp"

class FileConfig
{
	public:
		FileConfig();
		FileConfig(std::string first_line, int port, std::string host, std::string server_name, std::string root, int limit_client_body_size,
					std::list<std::string> map_error, std::string methods, std::string path_location,
					std::string autoindex, std::string index, std::string upload_dir, std::string cgi_bin,
					std::string cgi_extension);
		~FileConfig();
		void		create_file(void);
		void		gen_path(std::string &s, const int len);

	private:
		std::string					_first_line;
		int							_port;
		std::string					_host;
		std::string					_server_name;
		std::string					_root;
		int							_limit_client_body_size;
		std::list<std::string>		_map_error;
		//LOCATION
		std::string					_methods;
		std::string					_path_location;
		std::string					_autoindex;
		std::string					_index;
		std::string					_upload_dir;
		std::string					_cgi_bin;
		std::string					_cgi_extension;
};

#endif