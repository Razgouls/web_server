/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 12:24:59 by elie              #+#    #+#             */
/*   Updated: 2021/11/28 14:15:37 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
** FORME COPLIEN
*/

/*
** Constructeur par default
*/
Server::Server()
{

}

Server				&Server::operator=(const Server &s)
{
	_port = s._port;
	_host = s._host;
	_server_name = s._server_name;
	_root = s._root;
	_list_routes = s._list_routes;
	_map_error = s._map_error;
	_limit_client_body_size = s._limit_client_body_size;
	_mime = s._mime;
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

/*
** Destructeur
*/
Server::~Server()
{

}


/*
** GESTION REPONSE EN FONCTION DU TYPE (FILE OU DIR)
*/


/*
** Cette fonction va permettre preparer le path de la requete et va etre comparer pour savoir de quelle location elle correspond, si aucune par default ca sera la location /
** On stocke dans _route, la location qui va lui correspondre (par default cest location /)
** EX : path : ./www	-- PREPARATION -->	/www
** EX : path : ./www/	-- PREPARATION -->	/www
*/
void						Server::get_path_location(void)
{
	std::list<Route>::iterator	it_begin = _list_routes.begin();
	std::list<Route>::iterator	it_end = _list_routes.end();
	std::string					path_search_route;
	Route						tmp_route;

	path_search_route = _request.get_path().substr(1);
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
}

/*
** Cette fonction va regarder si la methode utilisé par la requete (GET / POST / DELETE) est présente dans la location
** Si elle l'est alors on return true, sinon on retrourne false, indiquant que la methode n'est pas présente --> Error 405 : Method Not Allowed
*/
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


/*
** Cette fonction va verifier 2 choses :
** 		--> Premierement si le fichier index.html existe	-> Si oui on affiche index.html, sinon on passe au deuxieme tiret.
**		--> Deuxiemement si le fichier index.html n'existe pas on regarde si l'autoindex est à on ou off. Si oui, on affiche l'arborescence, sinon Error 404
**
** Nous utilisons une pair : 
**		--> Le premier bool indique si le fichier index.html existe
**		--> Le deuxieme bool indique si l'autoindex est à on ou off
*/
int		Server::manage_auto_index(void)
{
	std::ifstream		myfile;
	std::string			index;
	std::string			path = _request.get_path();

	index = path.append("index.html");
	myfile.open(index.c_str());
	if (myfile.good() && !_route.get_index().empty())
		return (INDEX);
	else if (_route.get_auto_index() == "on")
		return (AUTOINDEX);
	return (NONE);
}


/*
** Cette fonction va permettre de stocker chaque nom de fichiers / dossiers / ... dans une list
**		-> Elle prend en parametre un DIR (le dossier a lire)
** 		-> Avec une boucle et readdir, on va parcourir ce dossier et stocker les noms dans une list
**		-> Nous stockons des pair :
**			-> Le premier est un std::string qui stocke le nom du fichier / dossiers / ...
**			-> Le deuxieme stocke le type (DT_DIR -> dossiers, DT_REG -> fichiers etc)
*/
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


/*
** Cette fonction permet juste de remplir la _reponse (donc la reponse qui va etre envoyee au client / navigateur)
*/
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


/*
** Cette fonction est appelee si la methode utilisee est DELETE
** On essaie d'open la ressource (si une facond e voir si la ressource en question existe tout simplement)
** 		SI FAIL : alors on affiche la page Page Not Found car la ressource n'existe pas
**		SINON OK : on remove la ressource (fonction remove(path_ressource)), et on indique qu'elle a bien ete supprimé grace a une page html (File is deleted)
*/
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

void					Server::manage_cgi(void)
{
	std::string		path = _request.get_path();
	std::string		bin;

	init_var_cgi();
	bin = _route.get_cgi_bin(UtilsFile::get_extension(path));
	if (!bin.empty())
	{
		_cgi.execute(bin, path, _request.get_body());
		std::string body = UtilsFile::get_file_content("./output.txt");
		body = body.substr(body.find("\r\n\r\n") + 4);
		_reponse.build_body_response(std::make_pair(MESSAGE, body));
	}
}

/*
** Cette fonction est appelee si la methode utilisée eatit une methode POST (notammenent pour le formulaire)
*/
//A RETRAIVAILLER AVEC LE CGI
void					Server::post_resource(void)
{
	std::string path = _request.get_path();
	std::string	bin;
	_reponse.set_content_type(_mime[UtilsFile::get_extension(path)]);
	if (UtilsIterator::find_list(_route.get_list_cgi_extension(), UtilsFile::get_extension(path)) && !_request.get_path_query().empty())
		manage_cgi();
}

void					Server::put_resource(void)
{
	std::string		tmp_path(_request.get_path());
	int				last_slash = tmp_path.find("/");
	std::ofstream	myfile;

	tmp_path.erase(0, last_slash + 1);
	if (UtilsFile::is_file(tmp_path))
	{
		myfile.open(tmp_path.c_str(), std::ofstream::in);
		if (myfile.good())
		{
			if (_request.get_content_length() == "0")
				_reponse.set_code_etat(204, "No Content");
			else
				myfile << _request.get_body();
			_reponse.set_code_etat(200, "OK");
			_reponse.build_body_response(std::make_pair(MESSAGE, "Le contenu a ete ajouté a " + tmp_path));
			myfile.close();
		}
		else
		{
			_reponse.set_code_etat(403, "Forbidden");
			_reponse.build_body_response(std::make_pair(M_FILE, _map_error[403]));
		}
	}
	else if (UtilsDir::is_dir(tmp_path))
	{
		_reponse.set_code_etat(403, "Forbidden");
		_reponse.build_body_response(std::make_pair(M_FILE, _map_error[403]));
	}
	else
	{
		myfile.open(tmp_path.c_str(), std::ofstream::out);
		if (!myfile.is_open() || !myfile.good())
		{
			_reponse.set_code_etat(403, "Forbidden");
			_reponse.build_body_response(std::make_pair(M_FILE, _map_error[403]));
		}
		else
		{
			myfile << _request.get_body();
			_reponse.set_code_etat(201, "Created");
			_reponse.build_body_response(std::make_pair(MESSAGE, "Fichier " + tmp_path + " creer et contenu ajouté"));
			myfile.close();
		}
	}
}

/*
**		1.	On ouvre le dossier correspondant au path
**		2.	On appelle la fonction qui va gerer l'autoindex et le fichier index.html (manage_auto_index())
**		3.	Si fichier index.html existe :
**				-> OUI : alors on build le body avec le fichier index.html
**				-> SINON :	SI on opendir un fichier (fail du coup)
**								OUI : alors on build le body avec le fichier correspondant
**							SINON : Si l'autoindex est a on :
**								OUI : On build l'arborescence du dossier correspondant (on effectue eventuellement une redirection 301 s'il manque le '/' a la fin alors que cest un dossier)
**							SINON : On build une erreur 404 (car on opendir un dossier mais il n'y avait pas d'index.html et l'autoindex etait a off)
*/
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
	if (UtilsIterator::find_list(_route.get_list_cgi_extension(), UtilsFile::get_extension(path)) && !_request.get_path_query().empty())
		manage_cgi();
	else if (ret == INDEX)
		_reponse.build_body_response(std::make_pair(M_FILE, path + "index.html"));
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
			_reponse.set_code_etat(301, "Forbidden");
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
	if (method == "GET")
		get_resource();
	else if (method == "POST")
		post_resource();
	else if (method == "PUT")
		put_resource();
	else if (method == "DELETE")
		delete_resource();
}

void					Server::manage_request(std::string &request)
{
	_request.set_request(request);
	_request.parse_request();
	_request.set_path(_root + _request.get_path());
	if (_request.is_valid() == -1)
		throw std::string("Requete envoyée invalide.");
	get_path_location();
	if (!check_method_location())
	{
		_reponse.set_code_etat(405, "Method Not Allowed");
		_reponse.build_body_response(std::make_pair(M_FILE, _map_error[405]));
	}
}

void					Server::c_recv(std::string &request)
{
	int					ret = 0;

	if (request.find("\r\n\r\n") != std::string::npos)
	{
		if (request.find("Transfer-Encoding: chunked") != std::string::npos)
		{
			ret = UtilsString::last_line_chunked(request);
		}
		if (ret == 0)
		{
			// char hello[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\n\r\n26\r\nVoici les donnees du premier morceau\r\n\r\n1C\r\net voici un second morceau\r\n\r\n20\r\net voici deux derniers morceaux \r\n12\r\nsans saut de ligne\r\n0\r\n\r\n";
			// send(fd, hello, 1024, 0);
			manage_request(request);
			std::cout << _request << std::endl;
			manage_reponse();
		}
	}
}

void					Server::init_var_cgi(void)
{
	std::string											path = _request.get_path();

	// _cgi.set_cgi_bin(_route.get_cgi_bin());
	// _cgi.set_cgi_extension(_route.get_cgi_extension());
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
	_cgi.add_var_env("SERVER_SOFWARE", "web_server/1.10");
	_cgi.add_var_env("SERVER_PROTOCOL", "HTTP/1.1");
	_cgi.add_var_env("SERVER_PORT", UtilsString::to_string(_port));
	// _cgi.add_var_env("SCRIPT_NAME", _route.get_cgi_bin());
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
	this->_map_error.insert(std::pair<int, std::string>(401, "./www/errors/401.html"));
	this->_map_error.insert(std::pair<int, std::string>(403, "./www/errors/403.html"));
	this->_map_error.insert(std::pair<int, std::string>(404, "./www/errors/404.html"));
	this->_map_error.insert(std::pair<int, std::string>(405, "./www/errors/405.html"));
	this->_map_error.insert(std::pair<int, std::string>(409, "./www/errors/409.html"));
	this->_map_error.insert(std::pair<int, std::string>(500, "./www/errors/500.html"));
	this->_map_error.insert(std::pair<int, std::string>(505, "./www/errors/504.html"));
}


/*
** GETTEURS ET SETTEURS
*/

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