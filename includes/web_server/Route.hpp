/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 14:06:11 by elie              #+#    #+#             */
/*   Updated: 2021/11/28 13:59:53 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
# define ROUTE_HPP

# include "Utils.hpp"

class Route
{
	public:
		Route();
		Route(std::string path, std::string index, std::list<std::string> list_methods, std::string auto_index);
		Route(const Route &r);
		~Route();

		Route						&operator=(const Route &r);

		std::string					&get_path(void);
		std::string					&get_index(void);
		std::list<std::string>		&get_list_methods(void);
		std::list<std::string>		&get_list_cgi_bin(void);
		std::list<std::string>		&get_list_cgi_extension(void);
		std::string					&get_auto_index(void);
		std::string					&get_path_uploads(void);
		std::string					get_cgi_bin(std::string extension);

		void						set_path(std::string &path);
		void						set_index(std::string &index);
		void						set_auto_index(std::string &auto_index);
		void						set_list_methods(std::list<std::string> methods);
		void						set_list_bin(std::list<std::string> bins);
		void						set_list_extension(std::list<std::string> extensions);
		void						set_path_uploads(std::string &path_uploads);
		void						clear();

	private:
		std::string					_path;
		std::string					_index;
		std::list<std::string>		_list_methods;
		std::list<std::string>		_list_cgi_bin;
		std::list<std::string>		_list_cgi_extension;
		std::string					_auto_index;
		std::string					_path_uploads;
};

std::ostream& 						operator<<(std::ostream &os, Route &f);

#endif