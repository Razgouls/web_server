/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 14:59:40 by elie              #+#    #+#             */
/*   Updated: 2021/11/15 12:48:38 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"

Route::Route()
{

}

Route::Route(std::string path, std::string index, std::list<std::string> list_methods, std::string auto_index) : _path(path),
				_index(index), _list_methods(list_methods), _auto_index(auto_index)
{

}

Route::Route(const Route &r)
{
	this->_path = r._path;
	this->_index = r._index;
	this->_list_methods = r._list_methods;
	this->_auto_index = r._auto_index;
	this->_path_uploads = r._path_uploads;
}

Route					&Route::operator=(const Route &r)
{
	this->_path = r._path;
	this->_index = r._index;
	this->_auto_index = r._auto_index;
	this->_list_methods = r._list_methods;
	this->_path_uploads = r._path_uploads;
	return (*this);
}

Route::~Route()
{

}

std::string				&Route::get_path(void)
{
	return (this->_path);
}

std::string				&Route::get_index(void)
{
	return (this->_index);
}

std::list<std::string>	&Route::get_list_methods(void)
{
	return (this->_list_methods);
}

std::string				&Route::get_auto_index(void)
{
	return (this->_auto_index);
}

std::string				&Route::get_path_uploads(void)
{
	return (this->_path_uploads);
}

void					Route::set_path(std::string path)
{
	this->_path = path;
}

void					Route::set_path_uploads(std::string path_uploads)
{
	this->_path_uploads = path_uploads;
}

void					Route::set_index(std::string index)
{
	this->_index = index;
}

void					Route::set_list_methods(std::string method)
{
	this->_list_methods.push_back(method);
}

void					Route::set_auto_index(std::string auto_index)
{
	this->_auto_index = auto_index;
}

std::ostream			&operator<<(std::ostream &os, Route &r)
{
	os << BOLDGREEN << "location :" << WHITE << std::endl;
	os << BOLDCYAN << "  ➜ path : " << WHITE << r.get_path() << std::endl;
	if (r.get_index().empty())
		os << BOLDCYAN << "  ➜ index : " << WHITE << "VIDE" << std::endl;
	else
		os << BOLDCYAN << "  ➜ index : " << WHITE << r.get_index() << std::endl;
	os << BOLDCYAN << "  ➜ methods : " << WHITE ;

	std::list<std::string> tmp_list = r.get_list_methods();
	std::list<std::string>::iterator it_begin = tmp_list.begin();
	std::list<std::string>::iterator it_end = tmp_list.end();
	while (it_begin != it_end)
	{
		os << *it_begin;
		it_begin++;
		if (it_begin != it_end)
			os << ", ";
	}
	os << std::endl;
	os << BOLDCYAN << "  ➜ autoindex : " << WHITE << r.get_auto_index() << std::endl;
	if (!r.get_path_uploads().empty())
		os << BOLDCYAN << "  ➜ path uploads : " << WHITE << r.get_path_uploads() << std::endl;
	return (os);
}
