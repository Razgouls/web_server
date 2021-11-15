/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 14:22:40 by elie              #+#    #+#             */
/*   Updated: 2021/11/15 13:00:46 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"
#include "Route.hpp"
#include "lib.hpp"
#include "Server.hpp"

void					add_infos_server(ServerConf &serv_conf, std::pair<std::string, std::string> &infos)
{
	if (infos.first == "listen")
		serv_conf.set_listen(infos.second);
	else if (infos.first == "host")
		serv_conf.set_host(infos.second);
	else if (infos.first == "server_name")
		serv_conf.set_server_name(infos.second);
	else if (infos.first == "root")
		serv_conf.set_root(infos.second);
	else if (infos.first == "limit_client_body_size")
		serv_conf.set_limite_body_size(infos.second);
	else
	{
		std::string code_error = infos.second.substr(0, infos.second.find(","));
		std::string html_error = infos.second.substr(code_error.length() + 1, infos.second.length() - code_error.length() - 1);
		std::pair<int, std::string>	tmp;
		tmp.first = atoi(code_error.c_str());
		if (!is_valid_code(tmp.first))
			throw std::string("Code " + code_error + " non valide.");
		tmp.second = html_error;
		serv_conf.set_map_error(tmp);
	}
}

void					location_is_valide(Route &r)
{
	std::list<std::string> list_method = r.get_list_methods();
	std::list<std::string>::iterator	it_begin = list_method.begin();
	std::list<std::string>::iterator	it_end = list_method.end();

	while (it_begin != it_end)
	{
		if (*it_begin != "GET" && *it_begin != "POST" && *it_begin != "DELETE" && *it_begin != "PUT")
			throw std::string("Nom de methode " + *it_begin + " de la route " + r.get_path() + " non autorisée");
		it_begin++;
	}
	if (r.get_auto_index() != "on" && r.get_auto_index() != "off")
		throw std::string("Valeur de l'autoindex " + r.get_auto_index() + " de la route " + r.get_path() + " non autorisée");
	
}

void					locations_is_valide(std::list<Route> list_route)
{
	std::list<Route>::iterator	it_begin_routes = list_route.begin();
	std::list<Route>::iterator	it_end_routes = list_route.end();
	bool						path_default = false;

	while (it_begin_routes != it_end_routes)
	{
		if ((*it_begin_routes).get_path() == "/")
			path_default = true;
		try {
			location_is_valide(*it_begin_routes);
		}
		catch(std::string const &chaine) {
			throw ;
		}
		
		it_begin_routes++;
	}
	if (!path_default)
		throw std::string("Pas de location par default");
}

void					infos_server_conf_valide(ServerConf &server_conf)
{
	if (server_conf.get_listen() < 1 || server_conf.get_listen() > 65536)
		throw std::string("Port dans le server " + server_conf.get_server_name() + " invalide");
	if (server_conf.get_host() != "127.0.0.1")
		throw std::string("Addresse IP " + server_conf.get_host() + " invalide");
	if (server_conf.get_server_name().empty())
		throw std::string("Name du server " + server_conf.get_server_name() + " inconnu");
	if (server_conf.get_root().empty())
		throw std::string("Racine du server " + server_conf.get_server_name() + " inconnu");
	if (server_conf.get_map_error().empty())
		throw std::string("Pas de fichier d'erreur par default dans le server " + server_conf.get_server_name());
}

void					server_conf_valide(ServerConf &server_conf)
{
	try {
		locations_is_valide(server_conf.get_list_routes());
		infos_server_conf_valide(server_conf);
	}
	catch(std::string const &chaine) {
		throw ;
	}
	
}

void					add_location_server(Route &r, std::pair<std::string, std::string> &infos)
{
	std::string method;

	if (infos.first == "index")
		r.set_index(infos.second);
	else if (infos.first == "autoindex")
		r.set_auto_index(infos.second);
	else if (infos.first == "upload_dir")
	{
		try {
			is_dir(infos.second);
			r.set_path_uploads(infos.second);
		}
		catch(std::string const &chaine) {
			throw;
		}
	}
	else
	{
		if (infos.second.empty())
			throw std::string("Methodes dans location manquantes.");
		while (!infos.second.empty())
		{
			method = infos.second.substr(0, infos.second.find(","));
			if (infos.second.find(",") == std::string::npos)
				infos.second.erase(0, infos.second.size());
			else
				infos.second.erase(0, infos.second.find(",") + 1);
			r.set_list_methods(method);
		}
	}
}

Route						createRoute(std::ifstream &file_config, std::string &line)
{
	std::pair<std::string, std::string>		infos;
	std::pair<bool, std::string>			path_location;
	Route									r;

	path_location = get_path_location(line);
	if (!path_location.first)
		throw std::string("Path de la location invalid (minimum /).");
	if (!syntax_bracket_open(file_config, line))
		throw std::string("Erreur de bracket pour le bloc location.");											//ERROR
	while (getline(file_config, line) && line.find("}") == std::string::npos)
	{
		infos = get_infos_line(line);
		if (!is_valid_infos_location(infos))
			throw std::string("Location : Mot clé invalide ou \";\" manquant.");									//ERROR
		try {
			add_location_server(r, infos);
		}
		catch(std::string const &chaine) {
			throw ;
		}
	}
	r.set_path(path_location.second);
	return (r);
}

void					createServerConf(ServerConf &server_conf, std::ifstream &file_config, std::string &line)
{
	std::pair<std::string, std::string>		infos;
	int										ret;

	if (!syntax_bracket_open(file_config, line))
		throw std::string("Erreur de bracket pour le bloc server.");							//ERROR
	while (getline(file_config, line))
	{
		if (line.find("server") != std::string::npos && line.find("server_") == std::string::npos)
			break ;
		else if (line.find("location") != std::string::npos)
		{
			try {
				Route tmp = createRoute(file_config, line);
				server_conf.set_list_routes(tmp);
			}
			catch(std::string const &chaine) {
				throw;
			}
		}
		else if (line != "" && line.find("}") == std::string::npos)
		{
			infos = get_infos_line(line);
			ret = is_valid_infos_server(infos);
			if (ret == 1)
				throw std::string("Server " + server_conf.get_server_name() + ": [" + infos.second + "] --> \";\" manquant");					//ERROR
			if (ret == 2)
				throw std::string("Server " + server_conf.get_server_name() + ": " + infos.first + " invalide");					//ERROR
			try {
				add_infos_server(server_conf, infos);
			}
			catch(std::string const &chaine) {
				throw;
			}
		}
	}
}

std::vector<ServerConf>		createListServerConf(std::ifstream &file_config)
{
	std::string				line;
	std::vector<ServerConf>	list_server_conf;

	if (file_config.is_open())
	{
		getline(file_config, line);
		if (line.find("server") == std::string::npos)
			throw std::string("Erreur bloc server");
		while (line.find("server") != std::string::npos)
		{
			try {
				ServerConf server_conf;
				createServerConf(server_conf, file_config, line);
				server_conf_valide(server_conf);
				list_server_conf.push_back(server_conf);
			}
			catch(std::string const &chaine) {
				throw;
			}
			
		}
	}
	else
		throw std::string("Le fichier ne peut pas etre ouvert.");
	return (list_server_conf);
}

int	run(std::ifstream &file_config, std::string &path)
{
	std::vector<ServerConf>		list_servers;
	Server						server;

	try {
		syntax_bracket(path);
		list_servers = createListServerConf(file_config);
	}
	catch(std::string const &chaine) {
		std::cerr << chaine << std::endl;
		return (-1);
	}
	std::vector<ServerConf>::iterator		it_begin = list_servers.begin();
	std::vector<ServerConf>::iterator		it_end = list_servers.end();
	while (it_begin != it_end)
	{
		std::cout << *it_begin << std::endl;
		it_begin++;
	}
	server.set_list_server(list_servers);
	try {
		server.run();
	}
	catch(std::string const &chaine) {
		std::cerr << chaine << std::endl;
		return (-1);
	}
	return (0);
}


int     main(int argc, char **argv)
{
	(void)argc;
	std::string path = "Config/default.conf";
	std::ifstream file_config(path.c_str());
	std::string file;
	
	if (argv[1])
		file = argv[1];
	std::ifstream file_config_argv(file.c_str());
	if (file_config_argv.is_open())
	{
		if (run(file_config_argv, file) < 0)
			return (-1);
	}
	else
	{
		if (run(file_config, path) < 0)
			return (-1);
	}
	return (0);
}


/*
std::list<ServerConf>::iterator		it_begin = list_servers.begin();
std::list<ServerConf>::iterator		it_end = list_servers.end();
while (it_begin != it_end)
{
	std::cout << *it_begin << std::endl;
	it_begin++;
}
*/