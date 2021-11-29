/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 10:01:43 by elie              #+#    #+#             */
/*   Updated: 2021/11/29 15:22:47 by elie             ###   ########.fr       */
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
		bool						check_vide = true;

		while (getline(file_config, line))
		{
			check_vide = false;
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
		if (check_vide)
			throw std::string("Fichier vide.");
	}

	std::pair<std::string, std::string>		get_infos_line(std::string	&line)
	{
		int									i = 0;
		int									j;
		std::pair<std::string, std::string>	infos;

		UtilsString::trim(" \r\t", line);
		while (line[i] && !UtilsString::is_blank(line[i]))
			i++;
		infos.first = line.substr(0, i);
		while (line[i] && UtilsString::is_blank(line[i]))
			i++;
		j = i;
		while (line[i])
			i++;
		infos.second = line.substr(j, i);
		return (infos);
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

	std::pair<std::string, std::string>			create_pair_cgi(std::pair<std::string, std::string> &infos)
	{
		std::pair<std::string, std::string>	tmp;
		if (infos.second.find(",") != std::string::npos)
		{
			std::string code_error = infos.second.substr(0, infos.second.find(","));
			std::string html_error = infos.second.substr(code_error.length() + 1, infos.second.length() - code_error.length() - 1);
			tmp.first = code_error;
			tmp.second = html_error;
		}
		else
		{
			tmp.first = "";
			tmp.second = ";";
		}
		return (tmp);
	}

	void									check_point_virgule(std::string &str)
	{
		if (*(str.rbegin()) != ';')
		{
			std::string	tmp = str + " ";
			std::stringstream	tmp_ret;
			size_t		size;
			size_t		i = -1;

			tmp_ret << UtilsString::create_reponse_parser("Point virgule manquant :", tmp, false);
			size = tmp_ret.str().size();
			while (++i < size - 29)
				tmp_ret << " ";
			tmp_ret << BOLDGREEN << "^" << WHITE;
			tmp_ret << "\n";
			i = -1;
			while (++i < size - 29)
				tmp_ret << " ";
			tmp_ret << BOLDGREEN << ";" << WHITE;
			throw std::string(tmp_ret.str());
		}
		str.erase(str.end() - 1);
	}
}

namespace UtilsString
{
	std::string								create_reponse_parser(std::string mess, const std::string &str, bool ok)
	{
		std::stringstream	rep;
	
		if (!ok)
			rep << BOLDRED << "  [KO] " << BOLDYELLOW << mess << WHITE << " [" << str << "]" << std::endl;
		else
			rep << BOLDGREEN << "  [OK] " << BOLDYELLOW << mess << WHITE << " [" << str << "]" << std::endl;
		return (rep.str());
	}

	void									vector_to_listpair(std::vector<std::pair<int, std::string> > &list, std::vector<std::string> &vector)
	{
		std::vector<std::string>::iterator		it_begin = vector.begin();
		std::vector<std::string>::iterator		it_end = vector.end();
		std::pair<int, std::string>				pair;

		while ((it_begin + 1) != it_end)
		{
			std::string num = (*it_begin).c_str();
			pair.first = UtilsString::hex_to_dec(num);
			pair.second = *(it_begin + 1);
			list.push_back(pair);
			it_begin += 2;
		}
	}

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

	bool									is_blank(char c)
	{
		if ((c < 9 || c > 13) && c != 32)
			return (false);
		return (true);
	}

	void									trim(std::string delim, std::string &str)
	{
		while (delim.find((*(str.begin()))) != std::string::npos)
			str.erase(0, 1);
		while (delim.find((*(str.rbegin()))) != std::string::npos)
			str.erase(str.end() - 1, str.end());
	}

	void									split(const std::string &chaine, char delimiteur, std::vector<std::string> &elements)
	{
		std::stringstream	ss(chaine);
		std::string			sousChaine;

		while (getline(ss, sousChaine, delimiteur))
		{
			if (*(sousChaine.begin()) == '\n')
				sousChaine.erase(0, 1);
			if (*(sousChaine.rbegin()) == '\n')
				sousChaine.erase(sousChaine.end() - 1, sousChaine.end());
			elements.push_back(sousChaine);
		}
	}

	void									split(const std::string &chaine, char delimiteur, std::list<std::string> &elements)
	{
		std::stringstream	ss(chaine);
		std::string			sousChaine;

		while (getline(ss, sousChaine, delimiteur))
		{
			if (!sousChaine.empty() && *(sousChaine.begin()) == '\n')
				sousChaine.erase(0, 1);
			if (!sousChaine.empty() && *(sousChaine.rbegin()) == '\n')
				sousChaine.erase(sousChaine.end() - 1, sousChaine.end());
			if (!sousChaine.empty())
				elements.push_back(sousChaine);
		}
	}

	std::string								to_string(int val)
	{
		std::stringstream	tmp;

		tmp << val;
		return (tmp.str());
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

	std::string								get_extension(const std::string &path)
	{
		std::string		tmp = path;
		int				last_dot = tmp.find_last_of(".");

		tmp.erase(0, last_dot);
		return (tmp);
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

	bool									check_file_exists(std::string &filename)
	{
		struct stat buffer;
		int exist;
		
		exist = stat(filename.c_str(), &buffer);
		if(exist == 0)
			return (true);
		return (false);
	}
}

namespace UtilsDir
{
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

namespace UtilsIterator
{
	bool									find_list(std::list<std::string> &list, std::string elem)
	{
		std::list<std::string>::iterator	it_begin = list.begin();
		std::list<std::string>::iterator	it_end = list.end();

		while (it_begin != it_end)
		{
			if (*it_begin == elem)
				return (true);
			it_begin++;
		}
		return (false);
	}
}
