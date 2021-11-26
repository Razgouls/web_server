/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserRoute.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 22:53:38 by elie              #+#    #+#             */
/*   Updated: 2021/11/25 15:52:31 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserRoute.hpp"

ParserRoute::ParserRoute()
{

}

ParserRoute::~ParserRoute()
{
	
}

void					ParserRoute::set_path(std::string path)
{
	this->_path = path;
}

void					ParserRoute::add_data_map(std::string &key, std::string &value)
{
	_m_parser[key] = value;
}

void					ParserRoute::path_valid(std::string &path_server)
{
	std::string		    path;

	if (_path.empty())
		throw std::string("Le path de la route 'location " + _path + "' ne peut etre vide dans la route");
	if (*(_path.end() - 1) == '/' && _path != "/")
		throw std::string("Le path '" + _path + "' est invalide (veuillez supprimer le '/' a la fin du path de la route 'location " + _path + "')");
	path = path_server + _path;
	if (!UtilsDir::is_dir(path))
		throw std::string("Le path '" + path + "' de la route 'location " + _path + "' n'est pas un dossier");
}

void					ParserRoute::index_valid(std::string &path_server)
{
	std::string         &index = _m_parser["index"];
	std::string         path_index;

	UtilsParser::check_point_virgule(' ', index);
	if (index.empty())
		throw std::string("L'index ne peut etre vide dans la route 'location " + _path + "'");
	path_index = path_server + _path;
	if (*(path_index.end() - 1) != '/')
		path_index += "/";
	path_index += index;
	if (!UtilsFile::is_file(path_index))
		throw std::string("L'index '" + index + "' n'est pas un fichier valide dans la route 'location " + _path + "'");
}

void					ParserRoute::method_valid(void)
{
	std::string         methods = _m_parser["methods"];
	std::string         path_index;
	std::string         method;

	UtilsParser::check_point_virgule(' ', methods);
	if (methods.empty())
		throw std::string("Les methodes ne peut etre vide dans la route 'location " + _path + "'");
	while (!methods.empty())
	{
		method = methods.substr(0, methods.find(","));
		if (methods.find(",") == std::string::npos)
			methods.erase(0, methods.size());
		else
			methods.erase(0, methods.find(",") + 1);
		if (method != "GET" && method != "POST" && method != "PUT" && method != "DELETE")
			throw std::string("La methode '" + method + "' est invalide dans la route 'location " + _path + "'");
		_l_method.push_back(method);
	}
}

void                    ParserRoute::autoindex_valid(void)
{
	std::string         &autoindex = _m_parser["autoindex"];

	UtilsParser::check_point_virgule(' ', autoindex);
	if (autoindex.empty())
		throw std::string("L'autoindex ne peut etre vide dans la route 'location " + _path + "'");
	if (autoindex != "on" && autoindex != "off")
		throw std::string("L'autoindex '" + autoindex + "' est invalide dans la route 'location " + _path + "'");
}

void                    ParserRoute::upload_dir_valid(void)
{
	std::string		    &upload_dir = _m_parser["upload_dir"];

	UtilsParser::check_point_virgule(' ', upload_dir);
	if (upload_dir.empty())
		throw std::string("L'upload_dir ne peut etre vide dans la route 'location " + _path + "'");
	if (*(upload_dir.end() - 1) == '/' && upload_dir != "/")
		throw std::string("Le path '" + upload_dir + "' est invalide (veuillez supprimer le '/' a la fin du upload_dir) dans la route 'location " + _path + "'");
	if (!UtilsDir::is_dir(upload_dir))
		throw std::string("L'upload_dir '" + upload_dir + "' n'est pas un dossier dans la route 'location " + _path + "'");
}

void                    ParserRoute::cgi_extension_valid(void)
{
	std::string		    &cgi_extension = _m_parser["cgi_extension"];

	UtilsParser::check_point_virgule(' ', cgi_extension);
	if (cgi_extension.empty())
		throw std::string("L'extension du cgi ne peut etre vide dans la route 'location " + _path + "'");
	if (cgi_extension != ".php")
		throw std::string("L'extension '" + cgi_extension + "' est invalide (doit etre egale a '.php' dans la route 'location " + _path + "'");
}

void                    ParserRoute::cgi_bin_valid(void)
{
	std::string		    &cgi_bin = _m_parser["cgi_bin"];

	UtilsParser::check_point_virgule(' ', cgi_bin);
	if (cgi_bin.empty())
		throw std::string("Le cgi bin ne peut etre vide dans la route 'location " + _path + "'");
	if (cgi_bin != "/usr/bin/php-cgi")
		throw std::string("Le cgi bin '" + cgi_bin + "' est invalide dans la route 'location " + _path + "'");
}

bool				ParserRoute::check_key_m_parser(std::map<std::string, std::string>	&map_tmp, std::string key)
{
	std::map<std::string, std::string>::iterator	it;

	it = map_tmp.find(key);
	if (it != map_tmp.end())
	{
		map_tmp.erase(key);
		return (true);
	}
	return (false);
}

void				ParserRoute::route_valid(std::string &path_server)
{
	std::map<std::string, std::string>	map_tmp = _m_parser;

	try {
		path_valid(path_server);
		if (check_key_m_parser(map_tmp, "index"))
			index_valid(path_server);
		if (check_key_m_parser(map_tmp, "methods"))
			method_valid();
		if (check_key_m_parser(map_tmp, "autoindex"))
			autoindex_valid();
		if (check_key_m_parser(map_tmp, "upload_dir"))
			upload_dir_valid();
		if (check_key_m_parser(map_tmp, "cgi_extension"))
			cgi_extension_valid();
		if (check_key_m_parser(map_tmp, "cgi_bin"))
			cgi_bin_valid();
	}
	catch(const std::string &error) {
		throw;
	}
	if (!map_tmp.empty())
		throw std::string("La mot clé '" + (map_tmp.begin())->first + "' est invalide dans la route 'location " + _path + "'");
}

void                    ParserRoute::print(void)
{
	std::cout << "PATH : [" << _path << "]" << std::endl;
	std::map<std::string, std::string>::iterator		it_begin = _m_parser.begin();
	std::map<std::string, std::string>::iterator		it_end = _m_parser.end();
	std::cout << "M_PARSER : [" << std::endl;
	while (it_begin != it_end)
	{
		std::cout << it_begin->first << " -> " << it_begin->second << std::endl;
		it_begin++;
	}
	std::cout << "]" << std::endl << std::endl;
}

std::string							&ParserRoute::get_path(void)
{
	return (this->_path);
}

std::map<std::string, std::string>	&ParserRoute::get_m_parser(void)
{
	return (this->_m_parser);
}

std::list<std::string>			&ParserRoute::get_methods(void)
{
	return (this->_l_method);
}

std::ostream& 						operator<<(std::ostream &os, ParserRoute &pr)
{
	pr.print();
	return (os);
}