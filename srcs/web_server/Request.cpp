/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 15:23:07 by elie              #+#    #+#             */
/*   Updated: 2021/12/03 16:32:57 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{
	_uri_request = "";
	_request = "";
	_map_request.clear();
	_method = "";
	_path = "";
	_path_query = "";
	_host = "";
	_body = "";
	_content_type = "";
	_content_length = "";
	_error = false;
}

Request::Request(const Request &r)
{
	this->_request = r._request;
	this->_method = r._method;
	this->_path = r._path;
	this->_path_query = r._path_query;
	this->_host = r._host;
}

Request::Request(std::string uri_request, std::string request, std::string method, std::string path, std::string path_query, std::string host, std::string body) :
					_uri_request(uri_request), _request(request), _method(method), _path(path), _path_query(path_query), _host(host), _body(body)
{
	
}

Request::~Request()
{
	_map_request.clear();
}

Request				&Request::operator=(Request &r)
{
	this->_map_request.clear();
	this->_request = r._request;
	this->_method = r._method;
	this->_path = r._path;
	this->_path_query = r._path_query;
	this->_host = r._host;

	return (*this);
}

void				Request::fill_map_request(void)
{
	std::vector<std::string>			elements;
	std::pair<std::string, std::string>	pair;
	size_t								dep = 0;

	UtilsString::split(_request, "\r\n", elements);
	std::vector<std::string>::iterator	it_begin = elements.begin();
	std::vector<std::string>::iterator	it_end = elements.end();
	while (it_begin != it_end)
	{
		if (it_begin->empty())
			break ;
		dep = it_begin->find(":");
		pair.first = it_begin->substr(0, dep);
		pair.second = it_begin->substr(dep + 1);
		if (*(pair.second.begin()) == ' ')
			pair.second.erase(0, 1);
		if (_map_request[pair.first].empty())
			_map_request[pair.first] = pair.second;
		else
			_error = true;
		it_begin++;
	}
}

void				Request::parse_body(void)
{
	_body = _request.substr(_request.find("\r\n\r\n") + 4);
	if (!_map_request["Transfer-Encoding"].empty())
	{
		std::vector<std::pair<int, std::string> >	v_pair;
		std::vector<std::string>	elements;
		UtilsString::split(_body, "\r\n", elements);
		UtilsString::vector_to_listpair(v_pair, elements);
		_body.clear();

		std::vector<std::pair<int, std::string> >::iterator		it_begin = v_pair.begin();
		std::vector<std::pair<int, std::string> >::iterator		it_end = v_pair.end();
		while (it_begin != it_end)
		{
			if (it_begin->second.find("=") != std::string::npos)
			{
				_path_query += it_begin->second;
				if ((it_begin + 1)->second.find("=") != std::string::npos)
					_path_query += "&";
			}
			_body += it_begin->second;
			it_begin++;
		}
	}
	else
	{
		if (_body.find("=") != std::string::npos)
			_path_query = _body;
	}
}

void				Request::parse_request(void)
{
	std::vector<std::string>		elements;

	UtilsString::split(_request.substr(0, _request.find("\n")), ' ', elements);
	fill_map_request();														//fill make_request
	if (elements.size() > 0)
		_method = elements[0];
	if (elements.size() > 1)
		_path = elements[1].substr(0, elements[1].find("?"));
	if (elements.size() > 2)
		_version = elements[2].substr(0, elements[2].find("\r"));
	if (elements.size() > 3)
		_error = true;
	_path_query = "";
	if (elements.size() > 2 && _method == "GET")
	{
		if (elements[1].find("?") != std::string::npos)
			_path_query = elements[1].substr(elements[1].find("?") + 1);
	}
	_host = _map_request["Host"];											//get host
	_content_type = _map_request["Content-Type"];							//get content-type
	_content_length = _map_request["Content-Length"];
	_uri_request = _path;
	parse_body();
}

int					Request::is_valid(void)
{
	if (_version != "HTTP/1.1")
		return (-1);
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
		return (-1);
	if (_path.empty() || _path == ".")
		return (-1);
	if (_host.empty() || _host.substr(0, _host.find(":")) != "localhost")
		return (-1);
	if (_error)
		return (-1);
	return (0);
}

std::string			&Request::get_request(void)
{
	return (this->_request);
}

void				Request::set_body(std::string &body)
{
	this->_body = body;
}

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

std::string										&Request::get_uri_request(void)
{
	return (this->_uri_request);
}

std::map<std::string, std::string>				&Request::get_map_request(void)
{
	return (this->_map_request);
}

void													Request::set_path(std::string path)
{
	this->_path = path;
}

void				Request::set_request(std::string &request)
{
	this->_request = request;
}

std::ostream		&operator<<(std::ostream &os, Request &r)
{
	os << BOLDRED << "=================================================================" << std::endl;
	os << BOLDRED << "======================== INFOS REQUEST ==========================" << std::endl;
	os << BOLDRED << "=================================================================" << WHITE << std::endl;
	os << BOLDGREEN << "Method\t:  " << BOLDYELLOW << "[" << WHITE << r.get_method() << BOLDYELLOW << "]" << WHITE << std::endl;
	os << BOLDGREEN << "Path\t:  " << BOLDYELLOW << "[" << WHITE << r.get_path() << BOLDYELLOW << "]" << WHITE << std::endl;
	os << BOLDGREEN << "Path Query\t:  " << BOLDYELLOW << "[" << WHITE << r.get_path_query() << BOLDYELLOW << "]" << WHITE << std::endl;
	os << BOLDGREEN << "Url request\t:  " << BOLDYELLOW << "[" << WHITE << r.get_uri_request() << BOLDYELLOW << "]" << WHITE <<std::endl;
	os << BOLDGREEN << "Host\t:  " << BOLDYELLOW << "[" << WHITE << r.get_host() << BOLDYELLOW << "]" << WHITE << std::endl;
	os << BOLDGREEN << "Content-Length\t:  " << BOLDYELLOW << "[" << WHITE << r.get_content_length() << BOLDYELLOW << "]" << WHITE << std::endl;
	os << BOLDGREEN << "Content-Type\t:  " << BOLDYELLOW << "[" << WHITE << r.get_content_type() << BOLDYELLOW << "]" << WHITE << std::endl;
	os << BOLDGREEN << "Body\t:  " << BOLDYELLOW << "[" << WHITE << r.get_body() << BOLDYELLOW << "]" << BOLDYELLOW << std::endl;
	os << BOLDGREEN << "REQUETE\t:" << std::endl << BOLDYELLOW << "[" << WHITE << r.get_request() << BOLDYELLOW << "]" << WHITE << std::endl;

	return (os);
}
