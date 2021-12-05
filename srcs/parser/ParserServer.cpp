/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 22:54:00 by elie              #+#    #+#             */
/*   Updated: 2021/12/05 15:30:15 by elie             ###   ########.fr       */
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
		throw std::string(UtilsString::create_reponse_parser("L'entete est invalide :", _head, false));
}

void				ParserServer::port_valid(void)
{
	int				i = -1;
	std::string		&port = _m_parser["listen"];

	UtilsString::trim(" \t\r", port);
	UtilsParser::check_point_virgule(port);
	if (port.empty())
		throw std::string(UtilsString::create_reponse_parser("Le port ne peut etre vide :", port, false));
	while (port[++i])
		if (port[i] < '0' || port[i] > '9')
			throw std::string(UtilsString::create_reponse_parser("Le port donné est invalide :", port, false));
	int	num_port = atoi(port.c_str());
	if (num_port <= 0 || num_port > 65535)
		throw std::string(UtilsString::create_reponse_parser("Le port donné est en dehors des plages [1 ; 65535] :", port, false));
	if (PRINT_CONFIG)
		std::cout << UtilsString::create_reponse_parser("Port :", port, true);
}

void				ParserServer::host_valid(void)
{
	std::string		&host = _m_parser["host"];

	UtilsString::trim(" \t\r", host);
	UtilsParser::check_point_virgule(host);
	if (host.empty())
		throw std::string(UtilsString::create_reponse_parser("L'adresse IP ne peut etre vide :", host, false));
	if (host != "127.0.0.1")
		throw std::string(UtilsString::create_reponse_parser("L'adresse IP donnée est invalide :", host, false));
	if (PRINT_CONFIG)
		std::cout << UtilsString::create_reponse_parser("Adresse IP :", host, true);
}

void				ParserServer::name_valid(void)
{
	std::string		&server_name = _m_parser["server_name"];

	UtilsString::trim(" \t\r", server_name);
	UtilsParser::check_point_virgule(server_name);
	if (server_name.empty())
		throw std::string(UtilsString::create_reponse_parser("Le nom du serveur ne peut etre vide :", server_name, false));
	if (PRINT_CONFIG)
		std::cout << UtilsString::create_reponse_parser("Server name :", server_name, true);
}

void				ParserServer::root_valid(void)
{
	std::string		&root = _m_parser["root"];

	UtilsString::trim(" \t\r", root);
	UtilsParser::check_point_virgule(root);
	if (root.empty())
		throw std::string(UtilsString::create_reponse_parser("Le root du serveur ne peut etre vide :", root, false));
	if (*(root.end() - 1) == '/')
		throw std::string(UtilsString::create_reponse_parser("Le root donné est invalide (veuillez supprimer le '/' a la fin du path) :", root, false));
	if (!UtilsDir::is_dir(root))
		throw std::string(UtilsString::create_reponse_parser("Le path du root doit etre un dossier :", root, false));
	if (PRINT_CONFIG)
		std::cout << UtilsString::create_reponse_parser("Root server :", root, true);
}

void				ParserServer::limit_body_size_valid(void)
{
	std::string			&limit = _m_parser["limit_client_body_size"];
	int					i = -1;

	UtilsString::trim(" \t\r", limit);
	UtilsParser::check_point_virgule(limit);
	if (limit.empty())
		throw std::string(UtilsString::create_reponse_parser("Le limite client body size du serveur ne peut etre vide :", limit, false));
	while (limit[++i])
		if (limit[i] < '0' || limit[i] > '9')
			throw std::string(UtilsString::create_reponse_parser("La limite client body size donné est invalide :", limit, false));
	int	limit_client = atoi(limit.c_str());
	if (limit_client < 0 || limit_client > 2147483647)
		throw std::string(UtilsString::create_reponse_parser("La limite client body size donné est invalide :", limit, false));
	if (PRINT_CONFIG)
		std::cout << UtilsString::create_reponse_parser("Limite client body size :", limit, true);
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
			throw std::string(UtilsString::create_reponse_parser("Configuration des fichiers errors incomplete dans le server :", it_begin->second, false));
		if (!UtilsFile::is_file(it_begin->second))
			throw std::string(UtilsString::create_reponse_parser("Le fichier erreur donné doit etre valide :", it_begin->second, false));
		if (PRINT_CONFIG)
			std::cout << UtilsString::create_reponse_parser("Fichier page erreur :", it_begin->second, true);
		it_begin++;
	}
}

void				ParserServer::check_key_m_parser(std::string mess, std::string key)
{
	std::map<std::string, std::string>::iterator	it;
	std::string	tmp = "";

	it = _m_parser.find(key);
	if (it == _m_parser.end())
		throw std::string(UtilsString::create_reponse_parser(mess, tmp, false));
	else
		_m_parser.erase(key);
}

void				ParserServer::map_parser_valid(void)
{
	std::map<std::string, std::string>	map_tmp = _m_parser;

	try {
		check_key_m_parser("Listen manquant :", "listen");
		check_key_m_parser("Host manquant :", "host");
		check_key_m_parser("Server_name manquant :", "server_name");
		check_key_m_parser("Root manquant :", "root");
		check_key_m_parser("Limit_client_body_size manquant :", "limit_client_body_size");
		check_key_m_parser("Erreur dans la bracket de fin", "}");
	}
	catch(const std::string &error) {
		throw;
	}
	if (!_m_parser.empty())
		throw std::string(UtilsString::create_reponse_parser("Le mot clé est invalide", _m_parser.begin()->first, false));
	if (_m_error.empty())
		throw std::string(UtilsString::create_reponse_parser("Aucun fichier d'erreur n'a ete configuré dans le serveur :", map_tmp["server_name"] , false));
	_m_parser = map_tmp;
}

void				ParserServer::route_valid(void)
{
	std::vector<ParserRoute>::iterator		it_begin = _v_route.begin();
	std::vector<ParserRoute>::iterator		it_end = _v_route.end();
	while (it_begin != it_end)
	{
		if (PRINT_CONFIG)
			std::cout << std::endl << BOLDCYAN << "Location : " << WHITE << std::endl;
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
		if (PRINT_CONFIG)
		{
			std::cout << BOLDRED << "=================================================================" << std::endl;
			std::cout << BOLDRED << "====================== PARSER SERVER CONFIG =====================" << std::endl;
			std::cout << BOLDRED << "=================================================================" << WHITE << std::endl;
			std::cout << BOLDCYAN << "Server :" << WHITE << std::endl;
		}
		map_parser_valid();
		name_valid();
		head_valid();
		port_valid();
		host_valid();
		root_valid();
		limit_body_size_valid();
		error_page_valid();
		route_valid();
		if (PRINT_CONFIG)
			std::cout << std::endl << std::endl;
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
