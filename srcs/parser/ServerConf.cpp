/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 14:16:44 by elie              #+#    #+#             */
/*   Updated: 2021/11/19 10:12:36 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"

ServerConf::ServerConf()
{
	this->_listen = -1;
	this->_host = "";
	this->_server_name = "";
	this->_root = "";

	this->_map_error[401] = "./www/errors/401.html";
	this->_map_error[403] = "./www/errors/403.html";
	this->_map_error[404] = "./www/errors/404.html";
	this->_map_error[405] = "./www/errors/405.html";
	this->_map_error[409] = "./www/errors/409.html";
	this->_map_error[500] = "./www/errors/500.html";
	this->_map_error[505] = "./www/errors/504.html";
}

ServerConf::ServerConf(int listen, std::string host, std::string server_name, std::string root, std::list<Route> list_routes,
					std::map<int, std::string> map_error, int limit_body_size) : _listen(listen), _host(host), _server_name(server_name),
					_root(root), _list_routes(list_routes), _map_error(map_error), _limit_body_size(limit_body_size)
{						
}

ServerConf::ServerConf(const ServerConf &s)
{
	this->_host = s._host;
	this->_root = s._root;
	this->_limit_body_size = s._limit_body_size;
	this->_list_routes = s._list_routes;
	this->_listen = s._listen;
	this->_map_error = s._map_error;
	this->_server_name = s._server_name;
}

ServerConf	&ServerConf::operator=(const ServerConf &s)
{
	_list_routes.clear();
	_map_error.clear();

	_listen = s._listen;
	_host = s._host;
	_root = s._root;
	_list_routes = s._list_routes;
	_map_error = s._map_error;
	_limit_body_size = s._limit_body_size;
	return (*this);
}

ServerConf::~ServerConf()
{

}

void					ServerConf::set_listen(std::string &listen)
{
	int		tmp_port;

	tmp_port = atoi(listen.c_str());
	if (tmp_port < 0)
		tmp_port = 0;
	this->_listen = tmp_port;
}

void					ServerConf::set_host(std::string &host)
{
	this->_host = host;
}

void					ServerConf::set_server_name(std::string &server_name)
{
	this->_server_name = server_name;
}

void					ServerConf::set_root(std::string &root)
{
	this->_root = root;
}

void					ServerConf::set_list_routes(Route &r)
{
	this->_list_routes.push_back(r);
}

void					ServerConf::set_map_error(std::pair<int, std::string> &error)
{
	this->_map_error.insert(error);
}

void					ServerConf::set_limite_body_size(std::string &limit_body_size)
{
	int		tmp;

	tmp = atoi(limit_body_size.c_str());
	if (tmp < 0)
		tmp = 0;
	this->_limit_body_size = tmp;
}

int						&ServerConf::get_listen(void)
{
	return (this->_listen);
}

std::string				&ServerConf::get_host(void)
{
	return (this->_host);
}

std::string				&ServerConf::get_server_name(void)
{
	return (this->_server_name);
}

std::string				&ServerConf::get_root(void)
{
	return (this->_root);
}

std::list<Route>			&ServerConf::get_list_routes(void)
{
	return (this->_list_routes);
}

std::map<int, std::string>	&ServerConf::get_map_error(void)
{
	return (this->_map_error);
}

int								&ServerConf::get_limite_body_size(void)
{
	return (this->_limit_body_size);
}

std::ostream				&operator<<(std::ostream &os, ServerConf &s)
{
	os << BOLDRED << "=================================================================" << std::endl;
	os << BOLDRED << "====================== INFOS SERVER_CONFIG ======================" << std::endl;
	os << BOLDRED << "=================================================================" << WHITE << std::endl;
	os << BOLDGREEN << "server :" << WHITE << std::endl;
	os << BOLDCYAN << "  ➜ listen : " << WHITE << s.get_listen() << std::endl;
	os << BOLDCYAN << "  ➜ host : " << WHITE << s.get_host() << std::endl;
	os << BOLDCYAN << "  ➜ server_name : " << WHITE << s.get_server_name() << std::endl;
	os << BOLDCYAN << "  ➜ root : " << WHITE << s.get_root() << std::endl;
	os << BOLDCYAN << "  ➜ error : " << WHITE << std::endl;
	
	std::map<int, std::string> 				map_tmp = s.get_map_error();
	std::map<int, std::string>::iterator	it_begin = map_tmp.begin();
	std::map<int, std::string>::iterator	it_end = map_tmp.end();
	while (it_begin != it_end)
	{
		os << "    " << BOLDYELLOW << it_begin->first << WHITE << " " << it_begin->second << std::endl;
		it_begin++;
	}
	std::cout << std::endl;
	std::list<Route> tmp_routes = s.get_list_routes();
	std::list<Route>::iterator	it_begin_list = tmp_routes.begin();
	std::list<Route>::iterator	it_end_list = tmp_routes.end();
	while (it_begin_list != it_end_list)
	{
		std::cout << *it_begin_list << std::endl;
		it_begin_list++;
	}
	return (os);
}