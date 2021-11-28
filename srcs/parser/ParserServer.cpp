/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 22:54:00 by elie              #+#    #+#             */
/*   Updated: 2021/11/28 12:47:27 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserServer.hpp"

ParserServer::ParserServer()
{

}

ParserServer::~ParserServer()
{

}

void				ParserServer::clear(void)
{
	_m_parser.clear();
	_m_error.clear();
	_v_route.clear();
}

void				ParserServer::set_head(std::string &head)
{
	this->_head = head;
}

void				ParserServer::add_data_map(std::string &key, std::string &value)
{
	_m_parser[key] = value;
}

void				ParserServer::add_data_map_error(std::pair<int, std::string> p)
{
	_m_error[p.first] = p.second;
}

void				ParserServer::add_cgi_bin(std::string &bin)
{
	_l_cgi_bin.push_back(bin);
}

void				ParserServer::add_cgi_extension(std::string &extension)
{
	_l_cgi_extension.push_back(extension);
}

void				ParserServer::add_data_v(ParserRoute &pa)
{
	std::vector<ParserRoute>::iterator		it_begin = _v_route.begin();
	std::vector<ParserRoute>::iterator		it_end = _v_route.end();
	while (it_begin != it_end)
	{
		if ((*it_begin).get_path() == pa.get_path())
			return ;
		it_begin++;
	}
	_v_route.push_back(pa);
}

std::string			&ParserServer::get_head(void)
{
	return (this->_head);
}

void				ParserServer::head_valid(void)
{
	UtilsString::trim(" \t\r", _head);
	if (_head != "server {")
		throw std::string("L'entete '" + _head + "' est invalide.");
}

void				ParserServer::port_valid(void)
{
	int				i = -1;
	std::string		&port = _m_parser["listen"];

	UtilsString::trim(" \t\r", port);
	UtilsParser::check_point_virgule(port);
	if (port.empty())
		throw std::string("Le port ne peut etre vide dans le server '" + _m_parser["server_name"] + "'");
	while (port[++i])
		if (port[i] < '0' || port[i] > '9')
			throw std::string("Le port '" + port + "' est invalide dans le server '" + _m_parser["server_name"] + "'");
	int	num_port = atoi(port.c_str());
	if (num_port < 0 || num_port > 65535)
		throw std::string("Le port '" + port + "' est en dehors des plages 0 et 65535 dans le server '" + _m_parser["server_name"] + "'");
}

void				ParserServer::host_valid(void)
{
	std::string		&host = _m_parser["host"];

	UtilsString::trim(" \t\r", host);
	UtilsParser::check_point_virgule(host);
	if (host.empty())
		throw std::string("L'adresse IP ne peut etre vide dans le server '" + _m_parser["server_name"] + "'");
	if (host != "127.0.0.1")
		throw std::string("L'adresse IP '" + host + "' est invalide dans le server '" + _m_parser["server_name"] + "'");
}

void				ParserServer::name_valid(void)
{
	std::string		&server_name = _m_parser["server_name"];

	UtilsString::trim(" \t\r", server_name);
	UtilsParser::check_point_virgule(server_name);
	if (server_name.empty())
		throw std::string("Le nom du serveur ne peut etre vide");

}

void				ParserServer::root_valid(void)
{
	std::string		&root = _m_parser["root"];

	UtilsString::trim(" \t\r", root);
	UtilsParser::check_point_virgule(root);
	if (root.empty())
		throw std::string("Le root du serveur ne peut etre vide dans le server '" + _m_parser["server_name"] + "'");
	if (*(root.end() - 1) == '/')
		throw std::string("Le root '" + root + "' est invalid (veuillez supprimer le '/' a la fin du path) dans le server '" + _m_parser["server_name"] + "'");
	if (!UtilsDir::is_dir(root))
		throw std::string("Le path '" + root + "' du root n'est pas un dossier dans le server '" + _m_parser["server_name"] + "'");
}

void				ParserServer::limit_body_size_valid(void)
{
	std::string			&limit = _m_parser["limit_client_body_size"];
	int					i = -1;

	UtilsString::trim(" \t\r", limit);
	UtilsParser::check_point_virgule(limit);
	if (limit.empty())
		throw std::string("Le limite client body size du serveur ne peut etre vide dans le server '" + _m_parser["server_name"] + "'");
	while (limit[++i])
		if (limit[i] < '0' || limit[i] > '9')
			throw std::string("La limite client body size '" + limit + "' est invalide dans le server '" + _m_parser["server_name"] + "'");
	int	limit_client = atoi(limit.c_str());
	if (limit_client < 0 || limit_client > 2147483647)
		throw std::string("La limite client body size '" + limit + "' est invalide dans le server '" + _m_parser["server_name"] + "'");
}

void				ParserServer::error_page_valid(void)
{
	std::map<int, std::string>::iterator		it_begin = _m_error.begin();
	std::map<int, std::string>::iterator		it_end = _m_error.end();
	
	while (it_begin != it_end)
	{
		UtilsString::trim(" \t\r", it_begin->second);
		UtilsParser::check_point_virgule(it_begin->second);
		if (it_begin->first <= 0 || it_begin->second.empty())
			throw std::string("Configuration des fichiers errors incomplete dans le server '" + _m_parser["server_name"] + "'");
		if (!UtilsFile::is_file(it_begin->second))
			throw std::string("Le fichier erreur '" + it_begin->second + "' n'est pas un fichier valide dans le server '" + _m_parser["server_name"] + "'");
		it_begin++;
	}
}

void				ParserServer::check_key_m_parser(std::string mess, std::string key)
{
	std::map<std::string, std::string>::iterator	it;

	it = _m_parser.find(key);
	if (it == _m_parser.end())
		throw std::string(mess);
	else
		_m_parser.erase(key);
}

void				ParserServer::map_parser_valid(void)
{
	std::map<std::string, std::string>	map_tmp = _m_parser;

	try {
		check_key_m_parser("Listen manquant.", "listen");
		check_key_m_parser("Host manquant.", "host");
		check_key_m_parser("Server_name manquant.", "server_name");
		check_key_m_parser("Root manquant.", "root");
		check_key_m_parser("Limit_client_body_size manquant.", "limit_client_body_size");
		check_key_m_parser("Bracket de fin manquante", "}");
	}
	catch(const std::string &error) {
		throw;
	}
	if (!_m_parser.empty())
		throw std::string("La mot clé '" + (_m_parser.begin())->first + "' est invalide dans le server '" + map_tmp["server_name"] + "'");
	if (_m_error.empty())
		throw std::string("Aucun fichier d'erreur n'a ete configuré dans le server '" + map_tmp["server_name"] + "'");
	_m_parser = map_tmp;
}

void				ParserServer::route_valid(void)
{
	std::vector<ParserRoute>::iterator		it_begin = _v_route.begin();
	std::vector<ParserRoute>::iterator		it_end = _v_route.end();
	while (it_begin != it_end)
	{
		try {
			(*it_begin).route_valid(_m_parser["root"]);
		}
		catch (std::string &error) {
			throw;
		}
		it_begin++;
	}
}

void				ParserServer::server_valid(void)
{
	try
	{
		map_parser_valid();
		name_valid();
		head_valid();
		port_valid();
		host_valid();
		root_valid();
		limit_body_size_valid();
		error_page_valid();
		route_valid();
	}
	catch(const std::string &error)
	{
		throw;
	}
	
}	

std::ostream&		ParserServer::print(std::ostream &os)
{
	std::cout << "HEAD : [" << _head << "]" << std::endl;
	std::map<std::string, std::string>::iterator		it_begin = _m_parser.begin();
	std::map<std::string, std::string>::iterator		it_end = _m_parser.end();
	std::cout << "M_PARSER : [" << std::endl;
	while (it_begin != it_end)
	{
		std::cout << it_begin->first << " -> " << it_begin->second << std::endl;
		it_begin++;
	}
	std::cout << "]" << std::endl << std::endl;
	std::cout << "M_ERROR : " << std::endl;
	std::map<int, std::string>::iterator		it_begin2 = _m_error.begin();
	std::map<int, std::string>::iterator		it_end2 = _m_error.end();
	while (it_begin2 != it_end2)
	{
		std::cout << it_begin2->first << " -> " << it_begin2->second << std::endl;
		it_begin2++;
	}
	std::cout << std::endl << "ROUTE : [" << _v_route.size() << "]" << std::endl;
	std::vector<ParserRoute>::iterator		it_begin3 = _v_route.begin();
	std::vector<ParserRoute>::iterator		it_end3 = _v_route.end();
	while (it_begin3 != it_end3)
	{
		std::cout << (*it_begin3) << std::endl;
		it_begin3++;
	}
	return (os);
}

std::map<std::string, std::string>	&ParserServer::get_m_parser(void)
{
	return (this->_m_parser);
}

std::map<int, std::string>			&ParserServer::get_map_error(void)
{
	return (this->_m_error);
}

std::vector<ParserRoute>			&ParserServer::get_parser_route(void)
{
	return (this->_v_route);
}

std::ostream& 						operator<<(std::ostream &os, ParserServer &pa)
{
	return (pa.print(os));
}
