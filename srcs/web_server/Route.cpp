/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 14:59:40 by elie              #+#    #+#             */
/*   Updated: 2021/11/28 14:11:54 by elie             ###   ########.fr       */
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
	this->_list_cgi_extension = r._list_cgi_extension;
	this->_list_cgi_bin = r._list_cgi_bin;
}

Route					&Route::operator=(const Route &r)
{
	clear();
	this->_path = r._path;
	this->_index = r._index;
	this->_auto_index = r._auto_index;
	this->_list_methods = r._list_methods;
	this->_path_uploads = r._path_uploads;
	this->_list_cgi_extension = r._list_cgi_extension;
	this->_list_cgi_bin = r._list_cgi_bin;

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
	_list_cgi_extension.clear();
	_list_cgi_bin.clear();
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

std::list<std::string>	&Route::get_list_cgi_bin(void)
{
	return (this->_list_cgi_bin);
}

std::list<std::string>	&Route::get_list_cgi_extension(void)
{
	return (this->_list_cgi_extension);
}

std::string				Route::get_cgi_bin(std::string extension)
{
	std::list<std::string>::iterator	it_begin = _list_cgi_bin.begin();
	std::list<std::string>::iterator	it_end = _list_cgi_bin.end();
	std::string							ret = extension.substr(1);
	std::string							ret_bin = "";

	if (ret == "php")
		ret = "php-cgi";
	while (it_begin != it_end)
	{
		ret_bin = (*it_begin).substr(it_begin->find_last_of("/") + 1);
		if (ret_bin == ret)
			return (*it_begin);
		it_begin++;
	}
	return ("");
}

std::string				&Route::get_auto_index(void)
{
	return (this->_auto_index);
}

std::string				&Route::get_path_uploads(void)
{
	return (this->_path_uploads);
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

void					Route::set_list_methods(std::list<std::string> methods)
{
	this->_list_methods = methods;
}

void					Route::set_list_bin(std::list<std::string> bins)
{
	this->_list_cgi_bin = bins;
}

void					Route::set_list_extension(std::list<std::string> extensions)
{
	this->_list_cgi_extension = extensions;
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

	if (!r.get_list_cgi_extension().empty())
	{
		os << BOLDCYAN << "  ➜ cgi extension : " << WHITE;
		tmp_list = r.get_list_cgi_extension();
		it_begin = tmp_list.begin();
		it_end = tmp_list.end();
		while (it_begin != it_end)
		{
			os << *it_begin;
			it_begin++;
			if (it_begin != it_end)
				os << ", ";
		}
		os << std::endl;
	}
	if (!r.get_list_cgi_bin().empty())
	{
		os << BOLDCYAN << "  ➜ cgi bin : " << WHITE;
		tmp_list = r.get_list_cgi_bin();
		it_begin = tmp_list.begin();
		it_end = tmp_list.end();
		while (it_begin != it_end)
		{
			os << *it_begin;
			it_begin++;
			if (it_begin != it_end)
				os << ", ";
		}
		os << std::endl;
	}
	return (os);
}
