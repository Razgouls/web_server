/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 12:24:59 by elie              #+#    #+#             */
/*   Updated: 2021/12/03 17:28:50 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{

}

Server				&Server::operator=(const Server &s)
{
	if (this != &s)
	{
		_port = s._port;
		_host = s._host;
		_server_name = s._server_name;
		_root = s._root;
		_list_routes = s._list_routes;
		_map_error = s._map_error;
		_limit_client_body_size = s._limit_client_body_size;
		_mime = s._mime;
	}
	return (*this);
}

Server::Server(const Server &s)
{
	_port = s._port;
	_host = s._host;
	_server_name = s._server_name;
	_root = s._root;
	_list_routes = s._list_routes;
	_map_error = s._map_error;
	_limit_client_body_size = s._limit_client_body_size;
	_mime = s._mime;
}

Server::~Server()
{

}

void						Server::get_path_location(void)
{
	std::list<Route>::iterator	it_begin = _list_routes.begin();
	std::list<Route>::iterator	it_end = _list_routes.end();
	std::string					path_search_route;
	Route						tmp_route;

	// path_search_route = _request.get_uri_request().substr(1);
	path_search_route = _request.get_uri_request();
	if ((*path_search_route.rbegin()) == '/')
		path_search_route.erase(path_search_route.length() - 1);
	while (it_begin != it_end)
	{
		if (path_search_route == (*it_begin).get_path())
		{
			_route = *it_begin;
			break ;
		}
		if ((*it_begin).get_path() == "/")
			tmp_route = (*it_begin);
		it_begin++;
	}
	if (it_begin != it_end)
		_route = *it_begin;
	else
		_route = tmp_route;
	std::cout << "PATH : [" << _route.get_path() << "]" << std::endl;
}

bool					Server::check_method_location(void)
{
	std::list<std::string>				list_method = _route.get_list_methods();
	std::list<std::string>::iterator	it_begin = list_method.begin();
	std::list<std::string>::iterator	it_end = list_method.end();
	
	while (it_begin != it_end)
	{
		if ((*it_begin) == _request.get_method())
			return (true);
		it_begin++;
	}
	return (false);
}

int		Server::manage_auto_index(void)
{
	std::ifstream		myfile;
	std::string			index;
	std::string			path = _request.get_path();

	index = path.append(_route.get_index());
	myfile.open(index.c_str());
	if (myfile.good() && !_route.get_index().empty())
		return (INDEX);
	else if (_route.get_auto_index() == "on")
		return (AUTOINDEX);
	return (NONE);
}

std::list<std::pair<std::string, unsigned char> >	Server::get_file_in_dir(DIR *dir)
{
	std::list<std::pair<std::string, unsigned char> >	files;
	struct dirent										*dirread;

	files.clear();
	if (dir)
	{
		while ((dirread = readdir(dir)) != NULL)
		{
			std::stringstream	tmp;
			tmp << dirread->d_name;
			files.push_back(std::make_pair(tmp.str(), dirread->d_type));
		}
	}
	return (files);
}

void					Server::fill_reponse(void)
{
	_reponse.set_code_etat(200, "OK");
	_reponse.set_method(_request.get_method());
	_reponse.set_host(_request.get_host());
	_reponse.set_uri_request(_request.get_uri_request());
	_reponse.set_version_http("HTTP/1.1");
	_reponse.set_content_type(_request.get_content_type());
	if (_request.get_content_type().empty())
		_reponse.set_content_type("text/html");
	_reponse.set_name_server("web_server/1.10.0 (Ubuntu)");
	_reponse.set_limit_body_size(_limit_client_body_size);
}


void					Server::manage_cgi(void)
{
	std::string		path = _request.get_path();
	std::string		bin;

	init_var_cgi();
	bin = _route.get_cgi_bin(UtilsFile::get_extension(path));
	if (!bin.empty() && UtilsFile::is_file(path))
	{
		_cgi.execute(bin, path, _request.get_body());
		std::string body = UtilsFile::get_file_content("./output.txt");
		if (_request.get_method() != "GET")
			body = body.substr(body.find("\r\n\r\n") + 4);
		_reponse.build_body_response(std::make_pair(MESSAGE, body));
		remove("./output.txt");
	}
	else
	{
		_reponse.set_code_etat(404, "Not Found");
		_reponse.build_body_response(std::make_pair(M_FILE, _map_error[404]));
	}
}

std::string				Server::get_upload_dir(void)
{
	std::string			path = _request.get_path();

	path = path.substr(0, path.find_last_of("/"));
	path = path.erase(0, 1);
	
	std::list<Route>::iterator	it_begin = _list_routes.begin();
	std::list<Route>::iterator	it_end = _list_routes.end();

	while (it_begin != it_end)
	{
		if (it_begin->get_path() == path)
		{
			if (!(it_begin->get_path_uploads().empty()))
				return (it_begin->get_path_uploads());
			else
				return ("");
		}
		it_begin++;
	}
	return ("");
}

void					Server::delete_resource(void)
{
	std::ifstream		myfile;
	std::string			mess("");

	myfile.open(_request.get_path().c_str());
	if (!myfile.good())
	{
		_reponse.set_code_etat(404, "Not Found");
		_reponse.build_body_response(std::make_pair(M_FILE, _map_error[404]));
	}
	else
	{
		remove(_request.get_path().c_str());
		mess = "File " + _request.get_path() + " is deleted";
		_reponse.set_code_etat(200, "OK");
		_reponse.build_body_response(std::make_pair(MESSAGE, mess));
	}
}

void					Server::get_content_multipart(std::string &element, std::string &tmp_path)
{
	std::vector<std::string>				elements;
	UtilsString::split(element, "\r\n", elements);

	std::vector<std::string>::iterator		it_begin = elements.begin();
	std::vector<std::string>::iterator		it_end = elements.end();
	while (it_begin != it_end)
	{
		if (it_begin->empty())
		{
			elements.erase(it_begin);
			break ;
		}
		it_begin++;
	}
	if (elements.size() > 2)
	{
		std::string name_file = elements[0].substr(elements[0].find("filename="));
		std::string name_file2 = name_file.substr(name_file.find("\"") + 1);
		std::string path;

		name_file2.erase(name_file2.end() - 1, name_file2.end());
		path = tmp_path + name_file2;
		_request.set_body(elements[2]);
		if (!update_file(path))
			create_file(path);
	}
}

void					Server::multipart(std::string &tmp_path)
{
	std::vector<std::string>				elements;
	std::string								delimiteur = _request.get_body().substr(0, _request.get_body().find("\r\n") + 2);

	if (delimiteur.empty())
		return ;
	UtilsString::split(_request.get_body(), delimiteur, elements);

	std::vector<std::string>::iterator		it_begin = elements.begin();
	std::vector<std::string>::iterator		it_end = elements.end();
	while (it_begin != it_end)
	{
		get_content_multipart(*it_begin, tmp_path);
		it_begin++;
	}
}

int						Server::update_file(std::string &tmp_path)
{
	std::ofstream	myfile;
	if (UtilsFile::is_file(tmp_path))
	{
		myfile.open(tmp_path.c_str(), std::ofstream::in | std::ios::app);
		if (myfile.good())
		{
			_reponse.set_code_etat(200, "OK");
			if (_request.get_content_length() == "0")
				_reponse.set_code_etat(204, "No Content");
			else
				myfile << _request.get_body();
			myfile.close();
			_reponse.build_body_response(std::make_pair(M_FILE, tmp_path));
		}
		else
		{
			_reponse.set_code_etat(403, "Forbidden");
			_reponse.build_body_response(std::make_pair(M_FILE, _map_error[403]));
		}
		return (1);
	}
	return (0);
}

void					Server::create_file(std::string &tmp_path)
{
	std::ofstream	myfile;

	myfile.open(tmp_path.c_str(), std::ofstream::out);
	if (!myfile.is_open() || !myfile.good())
	{
		_reponse.set_code_etat(403, "Forbidden");
		_reponse.build_body_response(std::make_pair(M_FILE, _map_error[403]));
	}
	else
	{
		if (_request.get_content_type() == "multipart/form-data")
			myfile << UtilsFile::get_file_content(_request.get_body());
		else
			myfile << _request.get_body();
		myfile.close();
		_reponse.set_code_etat(201, "Created");
		_reponse.build_body_response(std::make_pair(M_FILE, tmp_path));
	}
}

void					Server::post_resource(void)
{
	std::string		tmp_path(_request.get_path());
	int				last_slash = tmp_path.find("/");
	std::ofstream	myfile;
	std::string		path_uploads = get_upload_dir();

	_reponse.set_content_type(_mime[UtilsFile::get_extension(tmp_path)]);
	if (UtilsIterator::find_list(_route.get_list_cgi_extension(), UtilsFile::get_extension(tmp_path)))
		manage_cgi();
	else
	{
		tmp_path.erase(0, last_slash + 1);
		if (!path_uploads.empty())
			tmp_path = path_uploads + tmp_path.substr(tmp_path.find_last_of("/"));
		if ((tmp_path.empty() || UtilsDir::is_dir(tmp_path)) && _request.get_content_type().find("multipart/") != std::string::npos)
			multipart(tmp_path);
		else if ((tmp_path.empty() || UtilsDir::is_dir(tmp_path)) && _request.get_content_type().find("multipart/") == std::string::npos)
		{
			_reponse.set_code_etat(403, "Forbidden");
			_reponse.build_body_response(std::make_pair(M_FILE, _map_error[403]));
		}
		else if (!update_file(tmp_path))
			create_file(tmp_path);
	}
}

void					Server::get_resource(void)
{
	std::list<std::pair<std::string, unsigned char> >	files;
	int													ret;
	DIR													*dir;
	std::string											path = _request.get_path();
	std::string											bin;

	dir = opendir(path.c_str());
	ret = manage_auto_index();
	_reponse.set_content_type(_mime[UtilsFile::get_extension(path)]);
	if (UtilsIterator::find_list(_route.get_list_cgi_extension(), UtilsFile::get_extension(path)))
		manage_cgi();
	else if (ret == INDEX)
		_reponse.build_body_response(std::make_pair(M_FILE, path + _route.get_index()));
	else if (!dir)
	{
		std::string tmp_path = path.substr(0, path.find("?"));
		std::string new_path = tmp_path;
		if (!UtilsFile::is_file(new_path))
		{
			_reponse.set_code_etat(404, "Not Found");
			new_path = _map_error[404];
		}
		if (!UtilsFile::permission_read(new_path))
		{
			_reponse.set_code_etat(403, "Forbidden");
			new_path = _map_error[403];
		}
		_reponse.build_body_response(std::make_pair(M_FILE, new_path));
	}
	else if (ret == AUTOINDEX)
	{
		files = get_file_in_dir(dir);
		_reponse.set_content_location(path);
		if (*(path.rbegin()) != '/')
		{
			_reponse.set_code_etat(301, "Moved Permanently");
			std::size_t found = path.find_last_of("/");
			_reponse.set_content_location(path.substr(found + 1).append("/"));
		}
		else
			_reponse.set_code_etat(200, "OK");
		_reponse.build_response_dir(files);
	}
	else
	{
		_reponse.set_code_etat(404, "Not Found");
		_reponse.build_body_response(std::make_pair(M_FILE, _map_error[404]));
	}
	if (dir)
		closedir(dir);
}

void					Server::manage_reponse(void)
{
	std::string			&method = _request.get_method();

	fill_reponse();
	if (_request.is_valid() == -1)
	{
		_reponse.set_code_etat(400, "Bad Request");
		_reponse.build_body_response(std::make_pair(M_FILE, _map_error[400]));
	}
	else if (!check_method_location())
	{
		_reponse.set_code_etat(405, "Method Not Allowed");
		_reponse.build_body_response(std::make_pair(M_FILE, _map_error[405]));
	}
	else if (_request.get_body().length() > (size_t)_limit_client_body_size)
	{
		_reponse.set_code_etat(413, "Request Entity Too Large");
		_reponse.build_body_response(std::make_pair(M_FILE, _map_error[413]));
	}
	else if (method == "GET")
		get_resource();
	else if (method == "POST")
		post_resource();
	else if (method == "DELETE")
		delete_resource();
}

int						Server::manage_request(std::string &request)
{
	_request.set_request(request);
	_request.parse_request();
	_request.set_path(_root + _request.get_path());
	if (_request.is_valid() == -1)
		return (-1);
	get_path_location();
	return (0);
}

int						Server::c_recv(std::string &request)
{
	int					ret = 0;

	if (request.find("\r\n\r\n") != std::string::npos)
	{
		if (request.find("Transfer-Encoding: chunked") != std::string::npos)
			ret = UtilsString::last_line_chunked(request);
		if (ret == 0)
		{
			manage_request(request); 
			if (PRINT)
			{
				if (_request.get_path().find("favicon") == std::string::npos)
						std::cout << _request << std::endl;
				manage_reponse();
			}
			ret = 0;
		}
	}
	else
		ret = 1;
	return (ret);
}

void					Server::init_var_cgi(void)
{
	std::string											path = _request.get_path();

	_cgi.add_var_env("AUTH_TYPE", "");
	_cgi.add_var_env("GATEWAY_INTERFACE", "CGI/1.1");
	_cgi.add_var_env("HTTP_ACCEPT", _request.get_map_request()["Accept"]);
	_cgi.add_var_env("HTTP_ACCEPT_LANGUAGE", _request.get_map_request()["Accept-Language"]);
	_cgi.add_var_env("HTTP_ACCEPT_ENCODING", _request.get_map_request()["Accept-Encoding"]);
	_cgi.add_var_env("HTTP_USER_AGENT", _request.get_map_request()["User-Agent"]);
	_cgi.add_var_env("HTTP_CONNECTION", _request.get_map_request()["Connection"]);
	_cgi.add_var_env("HTTP_HOST", _host);
	_cgi.add_var_env("QUERY_STRING", _request.get_path_query());
	_cgi.add_var_env("REQUEST_METHOD", _request.get_method());
	_cgi.add_var_env("REQUEST_URI", _request.get_uri_request());
	_cgi.add_var_env("SERVER_NAME", _server_name);
	_cgi.add_var_env("SERVER_SOFWARE", "webserv/1.10");
	_cgi.add_var_env("SERVER_PROTOCOL", "HTTP/1.1");
	_cgi.add_var_env("SERVER_PORT", UtilsString::to_string(_port));
	_cgi.add_var_env("PATH_TRANSLATED", path);
	_cgi.add_var_env("REMOTE_ADDR", _host);
	_cgi.add_var_env("REMOTE_USER", "user");
	_cgi.add_var_env("CONTENT_TYPE", "text/html");
	if (!_request.get_map_request()["Content-Type"].empty())
		_cgi.add_var_env("CONTENT_TYPE", _request.get_map_request()["Content-Type"]);
	_cgi.add_var_env("CONTENT_LENGTH", UtilsString::to_string(_request.get_body().length()));
	_cgi.add_var_env("REDIRECT_STATUS", "200");
}

void					Server::init_mime(void)
{
	_mime[".acc"] = "audio/aac";
	_mime[".abw"] = "application/x-abiword";
	_mime[".arc"] = "application/octet-stream";
	_mime[".avi"] = "video/x-msvideo";
	_mime[".azw"] = "application/vnd.amazon.ebook";
	_mime[".bin"] = "application/octet-stream";
	_mime[".bmp"] = "image/bmp";
	_mime[".bz"] = "application/x-bzip";
	_mime[".bz2"] = "application/x-bzip2";
	_mime[".csh"] = "application/x-csh";
	_mime[".css"] = "text/css";
	_mime[".csv"] = "text/csv";
	_mime[".doc"] = "application/msword";
	_mime[".eot"] = "application/vnd.ms-fontobject";
	_mime[".epub"] = "application/epub+zip";
	_mime[".gif"] = "image/gif";
	_mime[".htm"] = "text/html";
	_mime[".html"] = "text/html";
	_mime[".ico"] = "image/x-icon";
	_mime[".ics"] = "text/calendar";
	_mime[".jar"] = "application/java-archive";
	_mime[".jpeg"] = "image/jpeg";
	_mime[".jpg"] = "image/jpeg";
	_mime[".js"] = "application/javascript";
	_mime[".json"] = "application/json";
	_mime[".mid"] = "audio/midi";
	_mime[".midi"] = "audio/midi";
	_mime[".mpeg"] = "video/mpeg";
	_mime[".mpkg"] = "application/vnd.apple.installer+xml";
	_mime[".odp"] = "application/vnd.oasis.opendocument.presentation";
	_mime[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	_mime[".odt"] = "application/vnd.oasis.opendocument.text";
	_mime[".oga"] = "audio/ogg";
	_mime[".ogv"] = "video/ogg";
	_mime[".ogx"] = "application/ogg";
	_mime[".otf"] = "font/otf";
	_mime[".png"] = "image/png";
	_mime[".pdf"] = "application/pdf";
	_mime[".ppt"] = "application/vnd.ms-powerpoint";
	_mime[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	_mime[".rar"] = "application/x-rar-compressed";
	_mime[".rtf"] = "application/rtf";
	_mime[".sh"] = "application/x-sh";
	_mime[".svg"] = "image/svg+xml";
	_mime[".swf"] = "application/x-shockwave-flash";
	_mime[".tar"] = "application/x-tar";
	_mime[".ts"] = "application/typescript";
	_mime[".wav"] = "audio/x-wav";
	_mime[".weba"] = "audio/webm";
	_mime[".webm"] = "video/webm";
	_mime[".webp"] = "image/webp";
	_mime[".xhtml"] = "application/xhtml+xml";
	_mime[".xml"] = "application/xml";
	_mime[".zip"] = "application/zip";
	_mime[".7z"] = "application/x-7z-compressed";
}

void					Server::init_page_error(void)
{
	this->_map_error.insert(std::pair<int, std::string>(400, "./www/errors/400.html"));
	this->_map_error.insert(std::pair<int, std::string>(401, "./www/errors/401.html"));
	this->_map_error.insert(std::pair<int, std::string>(403, "./www/errors/403.html"));
	this->_map_error.insert(std::pair<int, std::string>(404, "./www/errors/404.html"));
	this->_map_error.insert(std::pair<int, std::string>(405, "./www/errors/405.html"));
	this->_map_error.insert(std::pair<int, std::string>(409, "./www/errors/409.html"));
	this->_map_error.insert(std::pair<int, std::string>(413, "./www/errors/413.html"));
	this->_map_error.insert(std::pair<int, std::string>(500, "./www/errors/500.html"));
	this->_map_error.insert(std::pair<int, std::string>(505, "./www/errors/504.html"));
}

void					Server::set_port(std::string &port)
{
	int		tmp_port;

	tmp_port = atoi(port.c_str());
	if (tmp_port < 0)
		tmp_port = 0;
	this->_port = tmp_port;
}

void					Server::set_host(std::string &host)
{
	this->_host = host;
}

void					Server::set_server_name(std::string &server_name)
{
	this->_server_name = server_name;
}

void					Server::set_root(std::string &root)
{
	this->_root = root;
}

void					Server::set_list_routes(std::list<Route> list_route)
{
	this->_list_routes = list_route;
}

void					Server::set_map_error(std::map<int, std::string> &map_error)
{
	_map_error.clear();
	this->_map_error = map_error;
}

void					Server::set_limite_body_size(std::string &limit_body_size)
{
	int		tmp;

	tmp = atoi(limit_body_size.c_str());
	if (tmp < 0)
		tmp = 0;
	this->_limit_client_body_size = tmp;
}

int						&Server::get_port(void)
{
	return (this->_port);
}

std::string				&Server::get_host(void)
{
	return (this->_host);
}

std::string				&Server::get_server_name(void)
{
	return (this->_server_name);
}

std::string				&Server::get_root(void)
{
	return (this->_root);
}

std::list<Route>		&Server::get_list_routes(void)
{
	return (this->_list_routes);
}

std::map<int, std::string>	&Server::get_map_error(void)
{
	return (this->_map_error);
}

int						&Server::get_limite_body_size(void)
{
	return (this->_limit_client_body_size);
}

Response				&Server::get_reponse(void)
{
	return (this->_reponse);
}

std::ostream				&operator<<(std::ostream &os, Server &s)
{
	os << BOLDRED << "=================================================================" << std::endl;
	os << BOLDRED << "====================== INFOS SERVER_CONFIG ======================" << std::endl;
	os << BOLDRED << "=================================================================" << WHITE << std::endl;
	os << BOLDGREEN << "server :" << WHITE << std::endl;
	os << BOLDCYAN << "  ➜ listen : " << WHITE << s.get_port() << std::endl;
	os << BOLDCYAN << "  ➜ host : " << WHITE << s.get_host() << std::endl;
	os << BOLDCYAN << "  ➜ server_name : " << WHITE << s.get_server_name() << std::endl;
	os << BOLDCYAN << "  ➜ root : " << WHITE << s.get_root() << std::endl;
	os << BOLDCYAN << "  ➜ error : " << WHITE << std::endl;
	
	std::map<int, std::string> 				map_tmp = s.get_map_error();
	std::map<int, std::string>::iterator	it_begin = map_tmp.begin();
	std::map<int, std::string>::iterator	it_end = map_tmp.end();
	while (it_begin != it_end)
	{
		os << "    " << BOLDYELLOW << it_begin->first << WHITE << " " << it_begin->second << std::endl;
		it_begin++;
	}
	std::cout << std::endl;
	std::list<Route> tmp_routes = s.get_list_routes();
	std::list<Route>::iterator	it_begin_list = tmp_routes.begin();
	std::list<Route>::iterator	it_end_list = tmp_routes.end();
	while (it_begin_list != it_end_list)
	{
		std::cout << *it_begin_list << std::endl;
		it_begin_list++;
	}
	return (os);
}