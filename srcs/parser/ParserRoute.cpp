/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserRoute.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 22:53:38 by elie              #+#    #+#             */
/*   Updated: 2021/12/05 15:34:24 by elie             ###   ########.fr       */
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
		throw std::string(UtilsString::create_reponse_parser("Le path ne peut etre vide :", _path, false));
	if (*(_path.end() - 1) == '/' && _path != "/")
		throw std::string(UtilsString::create_reponse_parser("Le path est invalide (veuillez supprimer le '/' a la fin du path) :", _path, false));
	path = path_server + _path;
	if (!UtilsDir::is_dir(path))
		throw std::string(UtilsString::create_reponse_parser("Le path doit etre un dossier :", _path, false));
	if (PRINT_CONFIG)
		std::cout << UtilsString::create_reponse_parser("Path location :", _path, true);
}

void					ParserRoute::index_valid(std::string &path_server)
{
	std::string         &index = _m_parser["index"];
	std::string         path_index;

	UtilsString::trim(" \t\r", index);
	UtilsParser::check_point_virgule(index);
	if (index.empty())
		throw std::string(UtilsString::create_reponse_parser("L'index ne peut etre vide :", index, false));
	path_index = path_server + _path;
	if (*(path_index.end() - 1) != '/')
		path_index += "/";
	path_index += index;
	if (!UtilsFile::is_file(path_index))
		throw std::string(UtilsString::create_reponse_parser("L'index donné doit etre un fichier valide :", index, false));
	if (PRINT_CONFIG)
		std::cout << UtilsString::create_reponse_parser("Index location :", index, true);
}

void					ParserRoute::method_valid(void)
{
	std::string							methods = _m_parser["methods"];
	std::string							path_index;
	std::string							method;
	std::list<std::string>::iterator	it_begin;
	std::list<std::string>::iterator	it_end;

	UtilsString::trim(" \t\r", method);
	UtilsParser::check_point_virgule(methods);
	if (methods.empty())
		throw std::string(UtilsString::create_reponse_parser("Les methodes ne peuvent etre vides :", method, false));
	UtilsString::split(methods, ',', _l_method);
	it_begin = _l_method.begin();
	it_end = _l_method.end();
	while (it_begin != it_end)
	{
		UtilsString::trim(" \r\t", *it_begin);
		method = *it_begin;
		if (method.empty())
			throw std::string(UtilsString::create_reponse_parser("Les methodes ne peuvent etre vides :", *it_begin, false));
		if ((method != "GET" && method != "POST" && method != "DELETE"))
			throw std::string(UtilsString::create_reponse_parser("La methode " + method + " est invalide :", method, false));
		it_begin++;
	}
	if (PRINT_CONFIG)
		std::cout << UtilsString::create_reponse_parser("Methodes :", methods, true);
}

void                    ParserRoute::autoindex_valid(void)
{
	std::string         &autoindex = _m_parser["autoindex"];

	UtilsString::trim(" \t\r", autoindex);
	UtilsParser::check_point_virgule(autoindex);
	if (autoindex.empty())
		throw std::string(UtilsString::create_reponse_parser("L'autoindex ne peut etre vide :", autoindex, false));
	if (autoindex != "on" && autoindex != "off")
		throw std::string(UtilsString::create_reponse_parser("L'autoindex donné est invalide :", autoindex, false));
	if (PRINT_CONFIG)
		std::cout << UtilsString::create_reponse_parser("Autoindex :", autoindex, true);
}

void                    ParserRoute::upload_dir_valid(void)
{
	std::string		    &upload_dir = _m_parser["upload_dir"];

	UtilsString::trim(" \t\r", upload_dir);
	UtilsParser::check_point_virgule(upload_dir);
	if (upload_dir.empty())
		throw std::string(UtilsString::create_reponse_parser("L'upload_dir ne peut etre vide :", upload_dir, false));
	if (*(upload_dir.end() - 1) == '/' && upload_dir != "/")
		throw std::string(UtilsString::create_reponse_parser("Le path donné est invalide (veuillez supprimer le '/' a la fin du upload_dir) :", upload_dir, false));
	if (!UtilsDir::is_dir(upload_dir))
		throw std::string(UtilsString::create_reponse_parser("L'upload dir donné doit etre un dossier :", upload_dir, false));
	if (PRINT_CONFIG)
		std::cout << UtilsString::create_reponse_parser("Path upload dir :", upload_dir, true);
}

void					ParserRoute::cgi_bin_valid(void)
{
	std::string							&cgi_bin = _m_parser["cgi_bin"];
	std::list<std::string>::iterator	it_begin;
	std::list<std::string>::iterator	it_end;

	UtilsString::trim(" \t\r", cgi_bin);
	UtilsParser::check_point_virgule(cgi_bin);
	if (cgi_bin.empty())
			throw std::string(UtilsString::create_reponse_parser("Le CGI bin ne peut etre vide :", cgi_bin, false));
	UtilsString::split(cgi_bin, ',', _l_cgi_bin);
	it_begin = _l_cgi_bin.begin();
	it_end = _l_cgi_bin.end();
	while (it_begin != it_end)
	{
		UtilsString::trim(" \r\t", *it_begin);
		if (it_begin->empty())
			throw std::string(UtilsString::create_reponse_parser("Le CGI bin ne peut etre vide :", cgi_bin, false));
		if (!UtilsFile::check_file_exists(*it_begin))
			throw std::string(UtilsString::create_reponse_parser("Le CGI bin donné est invalide :", cgi_bin, false));
		it_begin++;
	}
	if (PRINT_CONFIG)
		std::cout << UtilsString::create_reponse_parser("CGI bin :", cgi_bin, true);
}

void					ParserRoute::cgi_extension_valid(void)
{
	std::string							&cgi_extension = _m_parser["cgi_extension"];
	std::list<std::string>::iterator	it_begin;
	std::list<std::string>::iterator	it_end;
	std::stringstream					rep;

	UtilsString::trim(" \t\r", cgi_extension);
	UtilsParser::check_point_virgule(cgi_extension);
	if (cgi_extension.empty())
			throw std::string(UtilsString::create_reponse_parser("L'extension du cgi ne peut etre vide :", cgi_extension, false));
	UtilsString::split(cgi_extension, ',', _l_cgi_extension);
	it_begin = _l_cgi_extension.begin();
	it_end = _l_cgi_extension.end();
	while (it_begin != it_end)
	{
		UtilsString::trim(" \t\r", *it_begin);
		if (it_begin->empty())
			throw std::string(UtilsString::create_reponse_parser("L'extension du cgi ne peut etre vide :", *it_begin, false));
		if (*(it_begin->begin()) != '.')
			throw std::string(UtilsString::create_reponse_parser("L'extension du cgi doit commencer par un '.' :", *it_begin, false));
		it_begin++;
	}
	if (PRINT_CONFIG)
		std::cout << UtilsString::create_reponse_parser("CGI extension :", cgi_extension, true);
}

bool					ParserRoute::check_key_m_parser(std::map<std::string, std::string>	&map_tmp, std::string key)
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
		throw std::string(UtilsString::create_reponse_parser("Le mot clé est invalide", (map_tmp.begin())->first, false));
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

std::list<std::string>			&ParserRoute::get_cgi_bin(void)
{
	return (this->_l_cgi_bin);
}

std::list<std::string>			&ParserRoute::get_cgi_extension(void)
{
	return (this->_l_cgi_extension);
}

std::ostream& 						operator<<(std::ostream &os, ParserRoute &pr)
{
	pr.print();
	return (os);
}