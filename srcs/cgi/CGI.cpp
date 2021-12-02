/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 18:07:56 by elie              #+#    #+#             */
/*   Updated: 2021/12/03 00:16:42 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI()
{

}

CGI::~CGI()
{

}

char						**CGI::convert_map(void)
{
	std::map<std::string, std::string>::iterator	it_begin = _map_var_env.begin();
	std::map<std::string, std::string>::iterator	it_end = _map_var_env.end();
	char											**env;
	int												i = 0;
	std::string										res;

	env = (char **)malloc(sizeof(char *) * (_map_var_env.size() + 1));
	while (it_begin != it_end)
	{
		res = it_begin->first + "=" + it_begin->second;
		env[i] = strdup(res.c_str());
		i++;
		it_begin++;
	}
	env[i] = 0;
	return (env);
}

void						CGI::execute(std::string &bin, std::string &path, std::string &body)
{
	pid_t					pid;
	int						status;

	FILE					*input = tmpfile();
	FILE					*output = fopen("output.txt", "w+");
	int						input_fd = fileno(input);
	int						output_fd = fileno(output);
	char					**argv;

	write(input_fd, body.c_str(), body.length());
	lseek(input_fd, 0, SEEK_SET);
	pid = fork();
	if (pid < 0)
		return ;
	else if (pid == 0)
	{
		argv = new char*[3];
		argv[0] = strdup(bin.c_str());
		argv[1] = strdup(path.c_str());
		argv[2] = 0;

		dup2(output_fd, STDOUT);
		dup2(input_fd, STDIN);
		if (execve(bin.c_str(), argv, convert_map()) == -1)
			std::cout << "ERROR execve" << std::endl;
		exit(0);
	}
	else
	{
		waitpid(pid, &status, 0);
		fclose(output);
		fclose(input);
	}
}

void						CGI::add_var_env(const std::string &key, const std::string &value)
{
	_map_var_env[key] = value;
}

std::string					&CGI::get_cgi_extension(void)
{
	return (this->_cgi_extension);
}

std::string					&CGI::get_cgi_bin(void)
{
	return (this->_cgi_bin);
}

std::map<std::string, std::string>	&CGI::get_map_var_env(void)
{
	return (this->_map_var_env);
}

void						CGI::set_cgi_bin(std::string &cgi_bin)
{
	this->_cgi_bin = cgi_bin;
}

void						CGI::set_cgi_extension(std::string &cgi_extension)
{
	this->_cgi_extension = cgi_extension;
}

void						CGI::set_map_var_env(std::map<std::string, std::string> &map)
{
	_map_var_env.clear();
	_map_var_env = map;
}
