/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 18:07:56 by elie              #+#    #+#             */
/*   Updated: 2021/11/23 18:11:31 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI()
{

}

CGI::~CGI()
{

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

void						CGI::add_var_env(const std::string &key, const std::string &value)
{
	_map_var_env[key] = value;
}