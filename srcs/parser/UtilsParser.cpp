/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilsParser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:38:18 by elie              #+#    #+#             */
/*   Updated: 2021/11/06 12:20:37 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.hpp"

bool									syntax_bracket_open(std::ifstream &file_config, std::string line)
{
	if (line.find("{") != std::string::npos)
		return (true);
	getline(file_config, line);
	while (line == "" && getline(file_config, line)) {}
	if (line.find("{") != std::string::npos)
		return (true);
	return (false);
}

std::pair<bool, std::string>			get_path_location(std::string line)
{
	while (std::isspace(*(line.begin())))
		line.erase(0, 1);
	line.erase(0, line.find("/"));
	line = line.substr(0, line.find(' '));
	if (*(line.begin()) != '/')
		return (std::pair<bool, std::string>(false, line));
	return (std::pair<bool, std::string>(true, line));
	
}

void									syntax_bracket(std::string &path)
{
	std::string					line;
	std::ifstream				file_config(path.c_str());
	std::multimap<char, char>	map_bracket;

	while (getline(file_config, line))
	{
		if (line.find("{") != std::string::npos)
			map_bracket.insert(std::pair<char, char>('{', '{'));
		else if (line.find("}") != std::string::npos)
			map_bracket.insert(std::pair<char, char>('}', '}'));
	}
	if (map_bracket.count('{') != map_bracket.count('}'))
		throw std::string("Erreur de bracket dans le fichier de configuration.");
}

int										is_valid_infos_server(std::pair<std::string, std::string> &infos)
{
	std::string opt_server[] = { "listen", "host", "server_name", "root", "error", "limit_client_body_size" };
	int			i = 0;

	while (i < 6)
	{
		if (opt_server[i] == infos.first)
		{
			if (infos.second[infos.second.size() - 1] == ';')
			{
				infos.second.erase(infos.second.size() - 1);
				return (0);
			}
			else
				return (1);
		}
		i++;
	}
	return (2);
}

bool									is_valid_infos_location(std::pair<std::string, std::string> &infos)
{
	std::string opt_location[] = { "methods", "index", "autoindex", "root" };
	int			i = 0;

	while (i < 4)
	{
		if (opt_location[i] == infos.first)
		{
			if (infos.second[infos.second.size() - 1] == ';')
			{
				infos.second.erase(infos.second.size() - 1);
				return (true);
			}
			else
				return (false);
		}
		i++;
	}
	return (false);
}

std::pair<std::string, std::string>		get_infos_line(std::string	&line)
{
	int									i = 0;
	int									j;
	std::pair<std::string, std::string>	infos;

	while (line[i] && std::isspace(line[i]))
		i++;
	j = i;
	while (line[j] && !std::isspace(line[j]))
		j++;
	infos.first = line.substr(i, j - i);
	i = j;
	while (line[i] && std::isspace(line[i]))
		i++;
	j = i;
	while (line[j] && !std::isspace(line[j]))
		j++;
	infos.second = line.substr(i, j - i);
	return (infos);
}

bool									is_valid_code(int code)
{
	if (code < 100)
		return (false);
	if (code > 103 && code < 200)
		return (false);
	if (code > 210 && code != 226 && code < 300)
		return (false);
	if (code > 308 && code != 310 && code < 400)
		return (false);
	if (code > 527)
		return (false);
	return (true);
}