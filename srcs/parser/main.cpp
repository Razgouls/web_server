/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 14:22:40 by elie              #+#    #+#             */
/*   Updated: 2021/11/29 13:37:03 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"
#include "Utils.hpp"
#include "Server.hpp"
#include "ParserServer.hpp"
#include "ParserRoute.hpp"
#include "ConnexionServer.hpp"
#include "FileConfig.hpp"

ParserRoute						create_parser_route(std::ifstream &file_config, std::string &line)
{
	ParserRoute								p_route;
	std::pair<std::string, std::string>		infos;

	if (!UtilsParser::syntax_bracket_open(line))
		throw std::string("Erreur de bracket dans un bloc location.");
	p_route.set_path(UtilsParser::get_path_location(line));
	while (getline(file_config, line) && line.find("}") == std::string::npos)
	{
		if (UtilsString::is_blank(line))
			continue ;
		infos = UtilsParser::get_infos_line(line);
		p_route.add_data_map(infos.first, infos.second);
	}
	return (p_route);
}

void							add_parser_server(ParserServer &p_server, std::list<ParserServer> &l_parser)
{
	if (!p_server.get_head().empty())
	{
		l_parser.push_back(p_server);
		p_server.clear();
	}
}

std::list<ParserServer>			create_list_parser_server(std::ifstream &file_config)
{
	ParserServer				_p_server;
	std::list<ParserServer>		l_parser;
	std::string					line;

	while (getline(file_config, line))
	{
		if (line.find("server") != std::string::npos && line.find("server_") == std::string::npos)			//permet de verifier si on est sur un nouveau bloc server et pas si on tombe sur le server_name
		{
			if (!UtilsParser::syntax_bracket_open(line))
				throw std::string("Erreur de bracket dans un bloc server.");
			add_parser_server(_p_server, l_parser);
			_p_server.set_head(line);
		}
		else if (line.find("location") != std::string::npos)
		{
			ParserRoute pr = create_parser_route(file_config, line);
			_p_server.add_data_v(pr);
		}
		else if (!UtilsString::is_blank(line))
		{
			std::pair<std::string, std::string> infos = UtilsParser::get_infos_line(line);
			if (infos.first == "error")
				_p_server.add_data_map_error(UtilsParser::create_pair_file_error(infos));
			else
				_p_server.add_data_map(infos.first, infos.second);
		}
	}
	add_parser_server(_p_server, l_parser);
	return (l_parser);
}

Route	init_route(ParserRoute &p)
{
	std::map<std::string, std::string>	&m_parser = p.get_m_parser();
	Route								r;

	r.set_path(p.get_path());
	r.set_index(m_parser["index"]);
	r.set_list_methods(p.get_methods());
	r.set_auto_index(m_parser["autoindex"]);
	r.set_path_uploads(m_parser["upload_dir"]);
	r.set_list_bin(p.get_cgi_bin());
	r.set_list_extension(p.get_cgi_extension());
	return (r);
}

Server	init_server(ParserServer &p)
{
	std::map<std::string, std::string>	&m_parser = p.get_m_parser();
	std::vector<ParserRoute>			&v_parser_route = p.get_parser_route();
	std::list<Route>					l_route;
	size_t								i = -1;
	Server								s;

	s.set_port(m_parser["listen"]);
	s.set_host(m_parser["host"]);
	s.set_server_name(m_parser["server_name"]);
	s.set_root(m_parser["root"]);
	s.set_map_error(p.get_map_error());
	s.set_limite_body_size(m_parser["limit_client_body_size"]);
	while (++i < v_parser_route.size())
		l_route.push_back(init_route(v_parser_route[i]));
	s.set_list_routes(l_route);
	s.init_page_error();
	s.init_mime();
	std::cout << s << std::endl;
	return (s);
}

std::vector<Server>	init_servers(std::list<ParserServer> &l_parser)
{
	std::list<ParserServer>::iterator		it_begin = l_parser.begin();
	std::list<ParserServer>::iterator		it_end = l_parser.end();
	std::vector<Server>						servers;

	while (it_begin != it_end)
	{
		try {
			it_begin->server_valid();
		}
		catch(const std::string &error) {
			throw ;
		}
		it_begin++;
	}
	it_begin = l_parser.begin();
	while (it_begin != it_end)
	{
		servers.push_back(init_server(*it_begin));
		it_begin++;
	}
	return (servers);
}

void	run(std::ifstream &file_config, std::string &path)
{
	std::list<ParserServer>		l_parser;
	std::vector<Server>			servers;
	ConnexionServer				connexion_server;

	try {
		UtilsParser::syntax_bracket(path);
		l_parser = create_list_parser_server(file_config);
		servers = init_servers(l_parser);
		connexion_server.set_servers(servers);
		connexion_server.run();
	}
	catch(std::string const &chaine) {
		throw;
	}
}

void	create_conf(void)
{
	std::list<std::string>	map_error;
	map_error.push_back("401,./www/errors/401.html");
	/*
	** HEADER
	*/
	{FileConfig file("server{", 8005, "127.0.0.1", "default_server1", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server", 8005, "127.0.0.1", "default_server2", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server\t\t{", 8005, "127.0.0.1", "default_server3", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server    {", 8005, "127.0.0.1", "default_server4", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("", 8005, "127.0.0.1", "default_server5", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	
	/*
	** PORT
	*/
	{FileConfig file("server {", -2147483648, "127.0.0.1", "default_server6", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", -1, "127.0.0.1", "default_server7", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 0, "127.0.0.1", "default_server8", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	
	/*
	** HOST
	*/
	{FileConfig file("server {", 8005, "", "default_server9", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "\t\t127.0.01\t\t", "default_server10", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "   127.0.0.1   ", "default_server11", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.2.0.1", "default_server12", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "0.0.0.0", "default_server13", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "8.8.8.8", "default_server13", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}

	/*
	** SERVER NAME
	*/
	{FileConfig file("server {", 8005, "127.0.0.1", "", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}

	/*
	** ROOT
	*/
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server14", "", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server15", "/elie", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server16", "./www/", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}

	/*
	** LIMIT CLIENT BODY SIZE
	*/
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server17", "./www", -2147483648, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server18", "./www", -1, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}

	/*
	** ERROR
	*/
	map_error.clear();
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server19", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	
	map_error.push_back(",./www/errors/401.html");
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server20", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	
	map_error.clear();
	map_error.push_back("./www/errors/401.html");
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server21", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	
	map_error.clear();
	map_error.push_back("401,");
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server22", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	
	map_error.clear();
	map_error.push_back("401");
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server23", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	
	map_error.clear();
	map_error.push_back("401,./www/elie/401.html");
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server24", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	map_error.clear();
	map_error.push_back("401,./www/errors/401.html");
	/*
	** METHODS LOCATION
	*/
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server25", "./www", 300000, map_error, "", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server26", "./www", 300000, map_error, "GET, POST, ELIE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server27", "./www", 300000, map_error, "GET, ", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}

	/*
	** PATH LOCATION
	*/
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server28", "./www", 300000, map_error, "GET, POST, DELETE", "/", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server29", "./www", 300000, map_error, "GET, POST, DELETE", "/{", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server30", "./www", 300000, map_error, "GET, POST, DELETE", "/www {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server31", "./www", 300000, map_error, "GET, POST, DELETE", "/errors/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server32", "./www", 300000, map_error, "GET, POST, DELETE", "/errors/401.html {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server33", "./www", 300000, map_error, "GET, POST, DELETE", "/errors {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}

	/*
	** AUTOINDEX LOCATION
	*/
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server34", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server35", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "elie", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server36", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "off", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}

	/*
	** INDEX LOCATION
	*/
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server37", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server38", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "elie.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server39", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "cgi", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server40", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "/cgi", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}

	/*
	** UPLOAD DIR LOCATION
	*/
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server41", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server42", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/elie", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server43", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads/", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .php");}

	/*
	** CGI BIN LOCATION
	*/
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server44", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server45", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh,", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server46", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server47", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", ", /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server48", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", ",/usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server49", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/php-cgi,/usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server50", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/she, /usr/bin/php-cgi", ".sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server51", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgie", ".sh, .php");}

	/*
	** CGI EXTENSION LOCATION
	*/
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server52", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", "");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server53", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh,      ");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server54", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh,");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server55", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ", .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server56", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".she, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server57", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, .phpe");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server58", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", "sh, .php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server59", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh, php");}
	{FileConfig file("server {", 8005, "127.0.0.1", "default_server60", "./www", 300000, map_error, "GET, POST, DELETE", "/ {", "on", "index.html", "www/uploads", "/usr/bin/sh, /usr/bin/php-cgi", ".sh,.php");}
}

int		main(int argc, char **argv)
{
	std::string		file("./config/default.conf");
	std::ifstream	ifs;

	if (argc == 2 && strcmp(argv[1], "--gen-conf-file") == 0)
	{
		srand (time(NULL));
		create_conf();
		return (0);		
	}
	if (argv[1])
		file = argv[1];
	ifs.open(file.c_str());
	if (ifs.is_open())
	{
		try {
			run(ifs, file);
		}
		catch(const std::string& e) {
			std::cerr << e << std::endl;
		}
		ifs.close();
	}
	return (0);
}
