/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_tester.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 12:04:46 by elie              #+#    #+#             */
/*   Updated: 2021/12/03 16:21:37 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_TESTER_HPP
# define UTILS_TESTER_HPP

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

# define PORT 8005
# define LONG 0

std::string								get_file_content(const std::string &filename);
std::string                             get_dir_content(const std::string &path);

#endif