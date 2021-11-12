/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 14:06:11 by elie              #+#    #+#             */
/*   Updated: 2021/11/06 11:31:59 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
# define ROUTE_HPP

# include "lib.hpp"

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
		std::string					&get_auto_index(void);

		void						set_path(std::string path);
		void						set_index(std::string index);
		void						set_auto_index(std::string auto_index);
		void						set_list_methods(std::string method);

	private:
		std::string					_path;
		std::string					_index;
		std::list<std::string>		_list_methods;
		std::string					_auto_index;
};

std::ostream& 						operator<<(std::ostream &os, Route &f);

#endif