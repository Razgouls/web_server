/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 14:59:40 by elie              #+#    #+#             */
/*   Updated: 2021/11/23 17:33:53 by elie             ###   ########.fr       */
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
	this->_cgi_extension = r._cgi_extension;
	this->_cgi_bin = r._cgi_bin;
}

Route					&Route::operator=(const Route &r)
{
	this->_list_methods.clear();

	this->_path = r._path;
	this->_index = r._index;
	this->_auto_index = r._auto_index;
	this->_list_methods = r._list_methods;
	this->_path_uploads = r._path_uploads;
	this->_cgi_extension = r._cgi_extension;
	this->_cgi_bin = r._cgi_bin;

	return (*this);
}

Route::~Route()
{

}

void					Route::clear(void)
{
	_path.clear();
	_index.clear();
	_list_methods.clear();
	_auto_index.clear();
	_path_uploads.clear();
	_cgi_extension.clear();
	_cgi_bin.clear();
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

std::string				&Route::get_cgi_extension(void)
{
	return (this->_cgi_extension);
}

std::string				&Route::get_cgi_bin(void)
{
	return (this->_cgi_bin);
}

void					Route::set_path(std::string &path)
{
	this->_path = path;
}

void					Route::set_path_uploads(std::string &path_uploads)
{
	this->_path_uploads = path_uploads;
}

void					Route::set_index(std::string &index)
{
	this->_index = index;
}

void					Route::set_cgi_extension(std::string &cgi_extension)
{
	this->_cgi_extension = cgi_extension;
}

void					Route::set_cgi_bin(std::string &cgi_bin)
{
	this->_cgi_bin = cgi_bin;
}

void					Route::set_list_methods(std::string &method)
{
	this->_list_methods.push_back(method);
}

void					Route::set_auto_index(std::string &auto_index)
{
	this->_auto_index = auto_index;
}

std::ostream			&operator<<(std::ostream &os, Route &r)
{
	os << BOLDGREEN << "location :" << WHITE << std::endl;
	os << BOLDCYAN << "  ➜ path : " << WHITE << r.get_path() << std::endl;
	if (!r.get_index().empty())
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
	if (!r.get_cgi_extension().empty())
		os << BOLDCYAN << "  ➜ cgi extension : " << WHITE << r.get_cgi_extension() << std::endl;
	if (!r.get_cgi_bin().empty())
		os << BOLDCYAN << "  ➜ cgi bin : " << WHITE << r.get_cgi_bin() << std::endl;
	return (os);
}
