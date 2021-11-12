/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:38:51 by elie              #+#    #+#             */
/*   Updated: 2021/11/05 18:26:26 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_HPP
# define LIB_HPP

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

# define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
# define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
# define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
# define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
# define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
# define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
# define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
# define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
# define WHITE       "\033[0m\033[0m"       /* White */

/*
** PARSER
*/
bool									syntax_bracket_open(std::ifstream &file_config, std::string line);
void									syntax_bracket(std::string &path);
std::pair<std::string, std::string>		get_infos_line(std::string &line);
bool									is_valid_infos_server(std::pair<std::string, std::string> &infos);
std::pair<bool, std::string>			get_path_location(std::string line);
bool									is_valid_infos_location(std::pair<std::string, std::string> &infos);
bool									is_valid_code(int code);
int		                                hex_to_dec(std::string hexVal);
std::string			                    transform_query_char(std::string str);

#endif