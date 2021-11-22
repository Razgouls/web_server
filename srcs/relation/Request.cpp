/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 15:23:07 by elie              #+#    #+#             */
/*   Updated: 2021/11/22 11:03:05 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{
	_url_request = "";
	_request = "";
	_map_request.clear();
	_method = "";
	_path = "";
	_path_query = "";
	_host = "";
	_body = "";
	_content_type = "";
	_content_length = "";
	_query_string.clear();
}

Request::Request(const Request &r)
{
	this->_request = r._request;
	this->_method = r._method;
	this->_path = r._path;
	this->_path_query = r._path_query;
	this->_host = r._host;
	this->_query_string = r._query_string;
}

Request::Request(std::string url_request, std::string request, std::string method, std::string path, std::string path_query, std::string host, std::string body, std::list<std::pair<std::string, std::string> > query_string) :
					_url_request(url_request), _request(request), _method(method), _path(path), _path_query(path_query), _host(host), _body(body), _query_string(query_string)
{
	
}

Request::~Request()
{
	
}

Request				&Request::operator=(Request &r)
{
	this->_map_request.clear();
	this->_request = r._request;
	this->_method = r._method;
	this->_path = r._path;
	this->_path_query = r._path_query;
	this->_host = r._host;
	this->_query_string = r._query_string;

	return (*this);
}

void				Request::get_infos_space(std::string &str, std::string &new_str, size_t &dep, size_t &last, char delim)
{
	new_str = str.substr(dep, last - dep);
	dep = last + 1;
	last = str.find(delim, dep);
}

void				Request::make_query_post_put()
{
	size_t			dep = 0;
	size_t			rep_find_equal;
	size_t			rep_find_et;
	std::string		copy_body;
	int				check = false;

	_body = _request.substr(_request.find("\r\n\r\n") + 4);
	copy_body = _body;
	char *ptr = strtok((char *)copy_body.c_str(), " ");
	while (ptr != NULL)
	{
		std::string body_tmp(ptr);
		while (body_tmp.find("&", dep) != std::string::npos)
		{
			check = true;
			rep_find_equal = body_tmp.find("=", dep);
			rep_find_et = body_tmp.find("&", rep_find_equal + 1);
			_query_string.push_back(std::make_pair(Utils::transform_query_char(body_tmp.substr(dep, rep_find_equal - dep)), Utils::transform_query_char(body_tmp.substr(rep_find_equal + 1, rep_find_et - rep_find_equal - 1))));
			dep = rep_find_et + 1;
		}
		if (check)
		{
			rep_find_equal = body_tmp.find("=", dep);
			rep_find_et = body_tmp.find(" ", rep_find_equal + 1);
			_query_string.push_back(std::make_pair(Utils::transform_query_char(body_tmp.substr(dep, rep_find_equal - dep)), Utils::transform_query_char(body_tmp.substr(rep_find_equal + 1, rep_find_et - rep_find_equal - 1))));
			check = false;
		}
		ptr = strtok(NULL, " ");
	}
}

bool				Request::fill_query_string_aux(size_t &last, size_t &dep, std::string &tmp_path, bool id)
{
	std::pair<std::string, std::string>	tmp_pair;
	size_t								tmp_occ = tmp_path.find('=', dep);
	
	if (tmp_occ == std::string::npos)
		return (false);
	tmp_pair.first = tmp_path.substr(dep, tmp_occ - dep);
	dep = tmp_occ + 1;
	if (id)
		tmp_pair.second = tmp_path.substr(dep, last - dep);
	else
		tmp_pair.second = tmp_path.substr(dep);
	dep = last + 1;
	_query_string.push_back(std::make_pair(Utils::transform_query_char(tmp_pair.first), Utils::transform_query_char(tmp_pair.second)));
	return (true);
}

void				Request::fill_query_string(void)
{
	size_t			occ = _path_query.find("?");

	if (occ == std::string::npos || _path_query.length() - 1 == occ)
		return ;

	size_t			dep = (_path_query.find("?")) + 1;
	std::string 	tmp_path = _path_query.substr(dep);
	size_t			last = tmp_path.find("&");

	dep = 0;
	while (last != std::string::npos)
	{
		if (!fill_query_string_aux(last, dep, tmp_path, true))
			break ;
		last = tmp_path.find("&", dep);
	}
	fill_query_string_aux(last, dep, tmp_path, false);
}

void				Request::fill_map_request(void)
{
	size_t								last = _request.find("\n");
	size_t								dep = 0;
	size_t								dep_two = 0;
	std::pair<std::string, std::string>	pair;
	std::string							line;

	while (last != std::string::npos)
	{
		line = _request.substr(dep, last - dep - 1);
		dep = last + 1;
		last = _request.find("\n", dep);

		dep_two = line.find(":");
		pair.first = line.substr(0, dep_two);
		pair.second = line.substr(dep_two + 1, last);
		if (*(pair.second.begin()) == ' ')
			pair.second.erase(0, 1);
		_map_request[pair.first] = pair.second;
	}
}

void				Request::parse_request(void)
{
	std::stringstream			ss;
	size_t						last = 0;
	size_t						last_tmp = 0;
	size_t						dep = 0;
	size_t						dep_tmp = 0;
	std::string					first_line;
	
	last = _request.find("\n");
	first_line = _request.substr(dep, last - 1);
	fill_map_request();														//fill make_request
	last = first_line.find(" ");
	get_infos_space(first_line, _method, dep, last, ' ');					//get method
	dep_tmp = dep;
	last_tmp = first_line.find("?");
	if (last_tmp != std::string::npos)
		get_infos_space(first_line, _path, dep_tmp, last_tmp, '?');			//get path
	else
	{
		last_tmp = last;
		get_infos_space(first_line, _path, dep_tmp, last_tmp, ' ');			//get path
	}
	get_infos_space(first_line, _path_query, dep, last, '?');				//get path_query
	_host = _map_request["Host"];											//get host
	_content_type = _map_request["Content-Type"];							//get content-type
	_content_length = _map_request["Content-Length"];
	if (_method == "GET")
		fill_query_string();
	else if (_method == "POST")
		make_query_post_put();
	else if (_method == "PUT")
		make_query_post_put();
	ss << "http://" << _host << _path;
	_url_request = ss.str();
}

void				Request::is_valid(void)
{
	if (_method.empty())
		throw std::string("Methode invalid");
	if (_path.empty())
		throw std::string("Path invalid");
	if (_host.empty())
		throw std::string("Host invalid");
}

std::string			&Request::get_request(void)
{
	return (this->_request);
}

// std::map<int, std::string>							Request::get_map_request(void)
// {
// 	return (this->_map_request);
// }

std::string										&Request::get_method(void)
{
	return (this->_method);
}

std::string										&Request::get_path(void)
{
	return (this->_path);
}

std::string										&Request::get_path_query(void)
{
	return (this->_path_query);
}

std::string										&Request::get_host(void)
{
	return (this->_host);
}

std::string										&Request::get_body(void)
{
	return (this->_body);
}

std::string										&Request::get_content_type(void)
{
	return (this->_content_type);
}

std::string										&Request::get_content_length(void)
{
	return (this->_content_length);
}

std::string										&Request::get_url_request(void)
{
	return (this->_url_request);
}

std::map<std::string, std::string>				&Request::get_map_request(void)
{
	return (this->_map_request);
}

std::list<std::pair<std::string, std::string> >	&Request::get_query_string(void)
{
	return (this->_query_string);
}

void													Request::set_path(std::string path)
{
	this->_path = path;
}

void				Request::set_request(std::string &request)
{
	this->_request = request;
}

void				Request::clear(void)
{
	_query_string.clear();
	_map_request.clear();
	_url_request.clear();
	_request.clear();
	_method.clear();
	_path.clear();
	_host.clear();
	_body.clear();
	_content_type.clear();
}

std::ostream		&operator<<(std::ostream &os, Request &r)
{
	os << BOLDRED << "=================================================================" << std::endl;
	os << BOLDRED << "======================== INFOS REQUEST ==========================" << std::endl;
	os << BOLDRED << "=================================================================" << WHITE << std::endl;
	os << BOLDGREEN << "Method\t:  " << WHITE << r.get_method() << std::endl;
	os << BOLDGREEN << "Path\t:  " << WHITE << r.get_path() << std::endl;
	os << BOLDGREEN << "Path Query\t:  " << WHITE << r.get_path_query() << std::endl;
	os << BOLDGREEN << "Url request\t:  " << WHITE << r.get_url_request() << std::endl;
	os << BOLDGREEN << "Host\t:  " << WHITE << r.get_host() << std::endl;
	os << BOLDGREEN << "Content-Length\t:  " << WHITE << r.get_content_length() << std::endl;
	if (!r.get_query_string().empty())
	{
		os << BOLDGREEN << "Query string :" << WHITE << std::endl;
		std::list<std::pair<std::string, std::string> > tmp = r.get_query_string();
		std::list<std::pair<std::string, std::string> >::iterator	it_begin = tmp.begin();
		std::list<std::pair<std::string, std::string> >::iterator	it_end = tmp.end();
		while (it_begin != it_end)
		{
			os << BOLDCYAN << "  ➜ " << (*(it_begin)).first << WHITE << " = " << (*(it_begin)).second << std::endl;
			it_begin++;
		}
	}
	if (!r.get_body().empty())
		os << BOLDGREEN << "Full query\t:  " << WHITE << r.get_body() << std::endl;
	os << std::endl;
	os << BOLDGREEN << "REQUETE\t:" << WHITE << std::endl << "{" << std::endl << r.get_request() << "}" << std::endl;

	return (os);
}
