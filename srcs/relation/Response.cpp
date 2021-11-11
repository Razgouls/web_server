/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 14:10:43 by elie              #+#    #+#             */
/*   Updated: 2021/11/08 18:44:03 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{
	_code_etat = 404;
}

Response::Response(const Response &r)
{
	this->_url_request = r._url_request;
	this->_method = r._method;
	this->_host = r._host;
	this->_content_length = r._content_length;
	this->_content_type = r._content_type;
	this->_name_server = r._name_server;
	this->_transfert_encoding = r._transfert_encoding;
	this->_content_length = r._content_length;
	this->_code_etat = r._code_etat;
}

Response::~Response()
{
	
}

Response					&Response::operator=(const Response &r)
{
	this->_url_request = r._url_request;
	this->_method = r._method;
	this->_host = r._host;
	this->_content_length = r._content_length;
	this->_content_type = r._content_type;
	this->_name_server = r._name_server;
	this->_transfert_encoding = r._transfert_encoding;
	this->_content_length = r._content_length;
	this->_code_etat = r._code_etat;
	
	return (*this);
}

void						Response::clear(void)
{
	_url_request.clear();
	_method.clear();
	_host.clear();
	_content_type.clear();
	_content_location.clear();
	_name_server.clear();
	_transfert_encoding.clear();
	_version_http.clear();
	_content_length = 0;
	_code_etat = 200;
	_body.clear();
}


/*
** Cette fonction va determiner la taille du body de la reponse que le serveur va envoyer au navigateur (le navigateur a besoin de cette info pour afficher le body)
*/
void					Response::get_length_body(void)
{
	std::list<std::string>::iterator	it_begin = _body.begin();
	std::list<std::string>::iterator	it_end = _body.end();
	std::stringstream					tmp_len;
	int									len = 0;

	while (*it_begin != "<html>")
		it_begin++;
	while (it_begin != it_end)
	{
		len += (*it_begin).length();
		it_begin++;
	}
	it_begin = _body.begin();
	while (*it_begin != ("Content-Type: " + _content_type + "\n"))
		it_begin++;
	it_begin++;
	if (len > _limit_body_size)
		len = _limit_body_size;
	tmp_len << "Content-Length: " << len << "\n";
	_body.insert(it_begin, tmp_len.str());
}


/*
** Cette fonction va transformer la list<std::string> en une seule string car le body d'une reponse est composé par une longue std::string
*/
std::string				Response::get_reponse(void)
{
	std::list<std::string>::iterator	it_begin = _body.begin();
	std::list<std::string>::iterator	it_end = _body.end();
	std::string							response;
	
	get_length_body();
	while (it_begin != it_end)
	{
		response.append(*it_begin);
		it_begin++;
	}
	_body.clear();
	return (response);
}


/*
** Cette fonction va initialisé la head de la reponse
*/
void				Response::build_head_response(void)
{
	std::stringstream	tmp_stream;
	std::stringstream	tmp_stream2;

	tmp_stream << _code_etat;
	_body.push_back(_version_http + " " + tmp_stream.str() + "\n");
	_body.push_back("Content-Type: " + _content_type + "\n");
	_body.push_back("Location: " + _content_location + "\n");
	_body.push_back("Server: " + _name_server + "\n");
	_body.push_back("\n");
	_body.push_back("<html>");
	_body.push_back("<head>");
	_body.push_back("<meta charset=\"utf-8\"/>");
	_body.push_back("<title>Page</title>");
	_body.push_back("</head>");
	_body.push_back("<body>");
}


/*
** Cette fonction prend en parametre un path (path qui correspond a un fichier ciblé)
** Elle va créer toute la reponse, à savoir composée de :
**		1.	L'en-tête
**		2.	Le body
** Pour faire cela elle va open le fichier en question et va lire une a une les lignes le composant.
** Elles vont etre stocké dans une list<std::string>
*/
void				Response::build_body_response(std::string &path, int code_etat, Request &req)
{
	std::ifstream				myfile;
	std::string					mytext;
	std::string					link_page_error;
	std::list<std::pair<std::string, std::string> >				tmp_query_string = req.get_query_string();

	_code_etat = code_etat;
	build_head_response();
	if (!tmp_query_string.empty())
	{
		if (!tmp_query_string.empty())
		{
			std::list<std::pair<std::string, std::string> >::iterator	it_begin = tmp_query_string.begin();
			std::list<std::pair<std::string, std::string> >::iterator	it_end = tmp_query_string.end();
			std::string ret;
			_body.push_back("INFORMATIONS FORMULAIRE");
			_body.push_back("<ul>");
			while (it_begin != it_end)
			{
				ret.append("<li>").append((*it_begin).first).append(" -> ").append((*it_begin).second).append("</li>");
				_body.push_back(ret);
				ret = "";
				it_begin++;
			}
			_body.push_back("</ul>");
		}
	}
	else
	{
		_body.push_back("<p>");
		myfile.open(path.c_str());
		if (!myfile.good())
			_body.push_back("Le path [" + path + "] de la page d'erreur est introuvable (erreur fichier de configuration)");
		else
		{
			while (getline(myfile, mytext))
				_body.push_back(mytext);
		}
		myfile.close();
		_body.push_back("</p>");
	}
}


/*
** Cette fonction va verifier :
**		1. Si le path est correct (on enlève la query string (elle commence a partir du premier "?"))
**		2. Si le fichier peut etre ouvert et lu sans soucis
** En fonction de cela, une erreur ou non sera retourné
*/
void				Response::gestion_errors(std::string &path)
{
	std::ifstream		myfile;

	myfile.open(path.substr(0, path.find("?")).c_str());
	_code_etat = 200;
	if (!myfile.good())
	{
		if (errno == ENOENT)
			_code_etat = 404;
		else if (errno == EACCES)
			_code_etat = 403;
	}
	myfile.close();
}


/*
** Cette fonction va appeler toutes les fonctions nécessaires pour creer la reponse
** Elle prend en parametre la path, correspondant au fichier en question et une map_error<int, std::string>. (int -> code_erreur et std::string -> path du fichier a afficher en reponse)
** A la fin de cette fonction, la reponse est completement build et peut etre envoyée au navigateur
*/
void				Response::build_response_string(std::string mess)
{
	_code_etat = 200;
	build_head_response();
	_body.push_back("<p>");
	_body.push_back(mess);
	_body.push_back("</p>");
	_body.push_back("</body>");
	_body.push_back("</html>");
}

void				Response::build_response_file(std::map<int, std::string> map_error, std::string &path, Request &req)
{
	std::stringstream	tmp;

	gestion_errors(path);
	if (_code_etat == 200)
	{
		if (path.find("favicon.ico") != std::string::npos || (_code_etat != 200 && _code_etat != 301))
		{
			build_head_response();
			_body.push_back("<p>");
			tmp << "ERROR: " << _code_etat;
			_body.push_back(tmp.str());
			_body.push_back("</p>");
		}
		else
			build_body_response(path, _code_etat, req);
		_body.push_back("</body>");
		_body.push_back("</html>");
	}
	else
		build_body_response(map_error[_code_etat], _code_etat, req);
}

/*
** Cette fonction va juste permettre de creer l'entete (design) de la page quand on affiche les repertoire (index.html non present et autonindex on)
** La voici :
** 
** Index
** -------------------------------------------------------------------
*/
void				Response::add_header_index(void)
{
	std::string		index_of;
	_body.push_back("<style>");
	_body.push_back("hr {");
	_body.push_back("width: 100%;");
	_body.push_back("margin-left: auto;");
	_body.push_back("margin-right: auto;");
	_body.push_back("}");
	_body.push_back("</style>");
	index_of.append("<h1>Index of ").append(_content_location).append("</h1>");
	_body.push_back(index_of);
	_body.push_back("<hr />");
}


/*
** Cette fonction va appeler toutes les fonctions nécessaires pour creer la reponse (composée de l'arborescende du dossier en question)
** Elle prend en parametre la liste des ficheirs, dossiers ... composant le dossier en question
** A la fin de cette fonction, la reponse est completement build et peut etre envoyée au navigateur
*/
void				Response::build_response_dir(std::list<std::pair<std::string, unsigned char> > &files)
{
	std::list<std::pair<std::string, unsigned char> >::iterator	it_begin_v = files.begin();
	std::list<std::pair<std::string, unsigned char> >::iterator	it_end_v = files.end();

	build_head_response();
	add_header_index();
	while (it_begin_v != it_end_v)
	{
		std::string line = "<p><a href=\"";
		line.append((*it_begin_v).first);
		if ((*it_begin_v).second == DT_DIR)
			line.append("/");
		line.append("\">").append(" ").append((*it_begin_v).first);
		if ((*it_begin_v).second == DT_DIR)
			line.append("/");
		line.append("</a></p>");
		_body.push_back(line);
		it_begin_v++;
	}
	_body.push_back("<hr />");
	_body.push_back("</body>");
	_body.push_back("</html>");
}



void				Response::set_url_request(std::string url_request)
{
	this->_url_request = url_request;
}


void				Response::set_method(std::string method)
{
	this->_method = method;
}

void				Response::set_host(std::string host)
{
	this->_host = host;
}

void				Response::set_content_location(std::string content_location)
{
	this->_content_location = content_location;
}

void				Response::set_content_type(std::string content_type)
{
	this->_content_type = content_type;
}

void				Response::set_name_server(std::string name_server)
{
	this->_name_server = name_server;
}

void				Response::set_transfer_encoding(std::string transfer_encoding)
{
	this->_transfert_encoding = transfer_encoding;
}

void				Response::set_limit_body_size(int limit_body_size)
{
	this->_limit_body_size = limit_body_size;
}

void				Response::set_version_http(std::string version_http)
{
	this->_version_http = version_http;
}

void				Response::set_content_length(int content_length)
{
	this->_content_length = content_length;
}

void				Response::set_code_etat(int code_etat)
{
	this->_code_etat = code_etat;
}


std::string			Response::get_url_request(void) const
{
	return (this->_url_request);
}

std::string			Response::get_method(void) const
{
	return (this->_method);
}

std::string			Response::get_host(void) const
{
	return (this->_host);
}

std::string			Response::get_content_location(void) const
{
	return (this->_content_location);
}

std::string			Response::get_content_type(void) const
{
	return (this->_content_type);
}

std::string			Response::get_name_server(void) const
{
	return (this->_name_server);
}

std::string			Response::get_transfer_encoding(void) const
{
	return (this->_transfert_encoding);
}

std::string			Response::get_version_http(void) const
{
	return (this->_version_http);
}

int					Response::get_content_length(void) const
{
	return (this->_content_length);
}

int					Response::get_code_etat(void) const
{
	return (this->_code_etat);
}

std::ostream		&operator<<(std::ostream &os, Response &r)
{
	(void)r;
	os << "HERE" << std::endl;
	return (os);
}