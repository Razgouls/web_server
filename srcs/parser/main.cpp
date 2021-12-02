/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 14:22:40 by elie              #+#    #+#             */
/*   Updated: 2021/12/02 17:24:35 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"
#include "Utils.hpp"
#include "Server.hpp"
#include "ParserServer.hpp"
#include "ParserRoute.hpp"
#include "ConnexionServer.hpp"

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
		if (line.find("server") != std::string::npos && line.find("server_") == std::string::npos)
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

int		main(int argc, char **argv)
{
	std::string		file("./config/default.conf");
	std::ifstream	ifs;

	(void)argc;
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
