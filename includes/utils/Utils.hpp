/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 10:01:43 by elie              #+#    #+#             */
/*   Updated: 2021/11/26 14:54:03 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <fstream>
# include <map>
# include <list>
# include <vector>
# include <ctime>
# include <sstream>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/socket.h>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <poll.h>
# include <dirent.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

# define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
# define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
# define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
# define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
# define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
# define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
# define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
# define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
# define WHITE       "\033[0m\033[0m"       /* White */

# define M_FILE 0
# define DIRECTORY 1
# define ERROR 2
# define MESSAGE 3
# define UNKNOW 4

# define STDIN 0
# define STDOUT 1
# define STDERR 2

namespace UtilsParser
{
	bool									syntax_bracket_open(std::string &line);
	std::string								get_path_location(std::string &line);
	void									syntax_bracket(std::string &path);
	int										is_valid_infos_server(std::pair<std::string, std::string> &infos);
	bool									is_valid_infos_location(std::pair<std::string, std::string> &infos);
	std::pair<std::string, std::string>		get_infos_line(std::string &line);
	bool									is_valid_code(int code);
	std::pair<int, std::string>				create_pair_file_error(std::pair<std::string, std::string> &infos);
	void									check_point_virgule(char c, std::string &str);
}

namespace UtilsDir
{
	void									can_open_dir(const std::string &path);
	bool									is_dir(const std::string& filename);
}

namespace UtilsFile
{
	bool									is_file(const std::string& filename);
	bool									is_image(const std::string &path);
	int										size_file(const std::string& filename);
	std::string								get_extension(const std::string &path);
	std::string								get_file_content(const std::string &filename);
	bool									permission_write(const std::string& filename);
	bool									permission_read(const std::string& filename);
	bool									permission_exec(const std::string& filename);
	bool									fd_is_valid(int fd);
}

namespace UtilsString
{
	void									print_matrice(char **mat);
	int										hex_to_dec(std::string &hexVal);
	std::string								transform_query_char(std::string str);
	std::string								replace_me(std::string& str, const std::string& sub, const std::string& mod);
	int										last_line_chunked(std::string &req);
	bool									is_blank(std::string &line);
	void									trim(char c, std::string &str);
	void									split(const std::string &chaine, char delimiteur, std::vector<std::string> &elements);
	std::string								to_string(int val);
	int										find_char(char c, std::string &str);
}

#endif