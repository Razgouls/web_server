/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 18:07:56 by elie              #+#    #+#             */
/*   Updated: 2021/11/26 13:52:58 by elie             ###   ########.fr       */
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

/*
else if (pid == 0)
	{
		dup2(output_fd, STDOUT_FILENO);
		dup2(input_fd, STDIN_FILENO);

		execve(route.get_cgi_bin().c_str(), av, this->convert_map());
	}
	else
	{
		waitpid(-1, NULL, 0);
		lseek(output_fd, 0, SEEK_SET);

		close(output_fd);
		close(input_fd);

		dup2(ex_fd[0], STDIN_FILENO);
		dup2(ex_fd[1], STDOUT_FILENO);
	}
*/

// void						CGI::execute(Route &route, std::string &path)
// {
// 	pid_t	pid;
// 	int		status;

// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		char **t = new char*[3];
// 		_output = fopen("output.txt", "w+");
// 		int fd = fileno(_output);
	
// 		t[0] = strdup(route.get_cgi_bin().c_str());
// 		t[1] = strdup(path.c_str());
// 		t[2] = 0;
// 		dup2(fd, STDOUT);
// 		if (execve(route.get_cgi_bin().c_str(), t, convert_map()) == -1)
// 			perror("ERROR EXECVE");
// 		exit(0);
// 	}
// 	else
// 		waitpid(pid, &status, 0);
// }

void						CGI::execute(Route &route, std::string &path, std::string &body)
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
		argv[0] = new char(route.get_cgi_bin().length());
		argv[1] = new char(path.length());
		argv[2] = 0;

		strcpy(argv[0], route.get_cgi_bin().c_str());
		strcpy(argv[1], path.c_str());
		dup2(output_fd, STDOUT);
		dup2(input_fd, STDIN);
		if (execve(route.get_cgi_bin().c_str(), argv, convert_map()) == -1)
			perror("ERROR EXECVE");
		exit(0);
	}
	else
	{
		waitpid(pid, &status, 0);
		close(input_fd);
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
