/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:48:28 by elie              #+#    #+#             */
/*   Updated: 2021/11/28 13:53:15 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "Utils.hpp"
# include "Route.hpp"

class CGI
{
	public:
		CGI();
		~CGI();
		std::string								&get_cgi_extension(void);
		std::string								&get_cgi_bin(void);
		std::map<std::string, std::string>		&get_map_var_env(void);

		void									set_cgi_extension(std::string &cgi_extension);
		void									set_cgi_bin(std::string &cgi_bin);
		void									set_map_var_env(std::map<std::string, std::string> &map_var_env);

		void									add_var_env(const std::string &key, const std::string &value);
		char									**convert_map(void);
		void									execute(std::string &bin, std::string &path, std::string &body);

	private:
		CGI(const CGI &cgi);
		CGI										&operator=(const CGI &cgi);

		std::string								_cgi_bin;
		std::string								_cgi_extension;
		std::map<std::string, std::string>		_map_var_env;
};

#endif