/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 10:01:43 by elie              #+#    #+#             */
/*   Updated: 2021/11/25 13:05:10 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Utils.hpp"

namespace UtilsParser
{
	bool									syntax_bracket_open(std::string &line)
	{
		if (line.find("{") != std::string::npos)
			return (true);
		return (false);
	}

	std::string								get_path_location(std::string &line)
	{
		while (std::isspace(*(line.begin())))
			line.erase(0, 1);
		line.erase(0, line.find("/"));
		if (line.find(' ') != std::string::npos)
			line = line.substr(0, line.find(' '));
		else
			line = line.substr(0, line.find('{'));
		if (*(line.begin()) != '/')
			return ("ERROR_PATH");
		return (line);
		
	}

	void									syntax_bracket(std::string &path)
	{
		std::string					line;
		std::ifstream				file_config(path.c_str());
		std::multimap<char, char>	map_bracket;
		int							i;

		while (getline(file_config, line))
		{
			i = -1;
			while (line[++i])
			{
				if (line[i] == '{')
					map_bracket.insert(std::pair<char, char>('{', '{'));
				else if (line[i] == '}')
					map_bracket.insert(std::pair<char, char>('}', '}'));
			}
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
		std::string opt_location[] = { "methods", "index", "autoindex", "root", "upload_dir", "cgi_extension", "cgi_bin" };
		size_t		i = 0;
		size_t		size = opt_location->size();

		while (i < size)
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

	std::pair<int, std::string>				create_pair_file_error(std::pair<std::string, std::string> &infos)
	{
		std::pair<int, std::string>	tmp;
		if (infos.second.find(",") != std::string::npos)
		{
			std::string code_error = infos.second.substr(0, infos.second.find(","));
			std::string html_error = infos.second.substr(code_error.length() + 1, infos.second.length() - code_error.length() - 1);
			tmp.first = atoi(code_error.c_str());
			tmp.second = html_error;
		}
		else
		{
			tmp.first = 0;
			tmp.second = ";";
		}
		return (tmp);
	}

	void									check_point_virgule(char c, std::string &str)
	{
		UtilsString::trim(c, str);
		if (*(str.rbegin()) != ';')
			throw std::string("Point virgule manquant.");
		str.erase(str.end() - 1);
	}
}

namespace UtilsString
{
	int										hex_to_dec(std::string &hexVal)
	{
		int len = hexVal.size();
		int base = 1;
		int dec_val = 0;
	
		for (int i = len - 1; i >= 0; i--) {
			if (hexVal[i] >= '0' && hexVal[i] <= '9') {
				dec_val += (int(hexVal[i]) - 48) * base;
				base = base * 16;
			}
			else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
				dec_val += (int(hexVal[i]) - 55) * base;
				base = base * 16;
			}
		}
		return (dec_val);
	}

	std::string								transform_query_char(std::string str)
	{
		int		i;
		std::string	new_str;
		std::string	tmp;

		i = 0;
		while (str[i])
		{
			if (str[i] == '+')
				new_str += ' ';
			else if (str[i] == '%')
			{
				tmp += str[i + 1];
				tmp += str[i + 2];
				new_str += (char)hex_to_dec(tmp);
				i += 2;
			}
			else
				new_str += str[i];
			i++;
		}
		return (new_str);
	}

	std::string								replace_me(std::string& str, const std::string& sub, const std::string& mod)
	{
		std::string tmp(str);
		tmp.replace(tmp.find(sub), mod.length(), mod);
		return (tmp);
	}

	int										last_line_chunked(std::string &req)
	{
		if (req.find("0\r\n\r\n") != std::string::npos)
			return (0);
		return (1);
	}

	bool									is_blank(std::string &line)
	{
		int			i = -1;

		while (line[++i])
		{
			if ((line[i] < 9 || line[i] > 13) && line[i] != 32)
				return (false);
		}
		return (true);
	}

	void									trim(char c, std::string &str)
	{
		while ((*(str.begin())) == c)
			str.erase(0, 1);
		while ((*(str.rbegin())) == c)
			str.erase(str.end() - 1, str.end());
	}
}

namespace UtilsFile
{
	std::string								get_file_content(const std::string &filename)
	{
		std::ifstream file(filename.c_str());
		if (file.is_open() && file.good())
		{
			std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
			return (content);
		}
		return (NULL);
	}

	bool									is_file(const std::string& filename)
	{
		struct stat buf;
		if (stat(filename.c_str(), &buf) != -1)
		{
			if (buf.st_mode & S_IFREG)
				return (true);
		}
		return (false);
	}

	bool									is_image(const std::string &path)
	{
		if (path.find(".gif") == std::string::npos || path.find(".jpeg") == std::string::npos || path.find(".jpg") == std::string::npos ||
			path.find(".png") == std::string::npos || path.find(".svg") == std::string::npos || path.find(".webp") == std::string::npos)
				return (true);
		return (false);
	}

	int										size_file(const std::string& filename)
	{
		struct stat buf;
		if (stat(filename.c_str(), &buf) != -1)
			return (buf.st_size);
		return (-1);
	}

	std::string								get_extension(const std::string &path)
	{
		std::string		tmp = path;
		int				last_dot = tmp.find_last_of(".");

		tmp.erase(0, last_dot);
		return (tmp);
	}

	bool									permission_write(const std::string& filename)
	{
		struct stat buf;
		if (stat(filename.c_str(), &buf) != -1)
		{
			if (buf.st_mode & S_IWUSR)
				return (true);
		}
		return (false);
	}

	bool									permission_read(const std::string& filename)
	{
		struct stat buf;
		if (stat(filename.c_str(), &buf) != -1)
		{
			if (buf.st_mode & S_IRUSR)
				return (true);
		}
		return (false);
	}

	bool									permission_exec(const std::string& filename)
	{
		struct stat buf;
		if (stat(filename.c_str(), &buf) != -1)
		{
			if (buf.st_mode & S_IXUSR)
				return (true);
		}
		return (false);
	}

	bool									fd_is_valid(int fd)
	{
		struct stat buf;
		if (fstat(fd, &buf) != -1)
		{
			if (buf.st_nlink == 0)
				return (true);
		}
		return (false);
	}
}

namespace UtilsDir
{
	void									can_open_dir(const std::string &path)
	{
		DIR		*dir = opendir(path.c_str());

		if (!dir)
			throw std::string("Le dossier " + path + " n'existe pas.");
		closedir(dir);
	}

	bool									is_dir(const std::string& filename)
	{
		struct stat buf;
		if (stat(filename.c_str(), &buf) != -1)
		{
			if (buf.st_mode & S_IFDIR)
				return (true);
		}
		return (false);
	}
}