/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 14:10:43 by elie              #+#    #+#             */
/*   Updated: 2021/11/29 16:57:54 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{
	_code_etat.first = 200;
	_code_etat.second = "OK";
	_content_length = 0;
}

Response::Response(const Response &r)
{
	this->_uri_request = r._uri_request;
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
	clear();
	this->_uri_request = r._uri_request;
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
	_uri_request.clear();
	_method.clear();
	_host.clear();
	_content_type.clear();
	_content_location.clear();
	_name_server.clear();
	_transfert_encoding.clear();
	_version_http.clear();
	_content_length = 0;
	_code_etat.first = 200;
	_code_etat.second = "OK";
	_body.clear();
	_body_head.clear();
	_body_response.clear();
}

/*
** Cette fonction va transformer la list<std::string> en une seule string car le body d'une reponse est composé par une longue std::string
*/
std::string				Response::fill_reponse(void)
{
	std::string							response;
	
	_content_length = _body_response.length();
	build_head_response();
	response = _body_head.append(_body_response);
	clear();
	return (response);
}


/*
** Cette fonction va initialisé la head de la reponse
*/
void				Response::build_head_response(void)
{
	std::stringstream	s_code_etat;
	std::stringstream	s_content_length;
	std::stringstream	s_time_now;
	time_t				now = time(0);
	char				*dt = ctime(&now);

	s_code_etat << _code_etat.first << " " << _code_etat.second;
	s_content_length << _content_length;
	s_time_now << "Date: " << dt;

	_body_head.append(_version_http + " " + s_code_etat.str() + "\n");
	_body_head.append("Content-Type: " + _content_type + "\n");
	_body_head.append(s_time_now.str());
	_body_head.append("Content-Length: " + s_content_length.str() + "\n");
	_body_head.append("Location: " + _content_location + "\n");
	_body_head.append("Server: " + _name_server + "\n");
	_body_head.append("\n");
}


/*
** Cette fonction va verifier :
**		1. Si le path est correct (on enlève la query string (elle commence a partir du premier "?"))
**		2. Si le fichier peut etre ouvert et lu sans soucis
** En fonction de cela, une erreur ou non sera retourné
*/
int					Response::gestion_errors(std::string &path)
{
	std::ifstream		myfile;
	int					code = 200;

	if (UtilsFile::is_file(path))
	{
		myfile.open(path.substr(0, path.find("?")).c_str());
		if (!myfile.good())
		{
			if (errno == ENOENT)
				code = 404;
			else if (errno == EACCES)
				code = 403;
		}
		myfile.close();
	}
	else
		code = -1;
	return (code);
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
	_body_response.append("<style>");
	_body_response.append("hr {");
	_body_response.append("width: 100%;");
	_body_response.append("margin-left: auto;");
	_body_response.append("margin-right: auto;");
	_body_response.append("}");
	_body_response.append("</style>");
	_body_response.append("<h1>Index of ").append(_content_location).append("</h1>");
	_body_response.append("<hr />");
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
		_body_response.append(line);
		it_begin_v++;
	}
	_body_response.append("<hr />");
}


/*
** Cette fonction prend en parametre un path (path qui correspond a un fichier ciblé)
** Elle va créer toute la reponse, à savoir composée de :
**		1.	L'en-tête
**		2.	Le body
** Pour faire cela elle va open le fichier en question et va lire une a une les lignes le composant.
** Elles vont etre stocké dans une list<std::string>
*/
void				Response::build_body_response(std::pair<int, std::string> infos)
{
	std::ifstream				myfile;
	
	if (infos.first == MESSAGE)
		_body_response.append(infos.second);
	else if (infos.first == M_FILE)
	{
		myfile.open(infos.second.c_str());
		_body_response += (UtilsFile::get_file_content(infos.second));
		myfile.close();
	}	
}



void				Response::set_uri_request(std::string &uri_request)
{
	this->_uri_request = uri_request;
}


void				Response::set_method(std::string &method)
{
	this->_method = method;
}

void				Response::set_host(std::string &host)
{
	this->_host = host;
}

void				Response::set_content_location(std::string &content_location)
{
	this->_content_location = content_location;
}

void				Response::set_content_type(std::string content_type)
{
	if (!content_type.empty())
		this->_content_type = content_type;
}

void				Response::set_name_server(std::string name_server)
{
	this->_name_server = name_server;
}

void				Response::set_transfer_encoding(std::string &transfer_encoding)
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

void				Response::set_code_etat(int code_etat, std::string mess)
{
	this->_code_etat.first = code_etat;
	this->_code_etat.second = mess;
}


std::string			&Response::get_uri_request(void)
{
	return (this->_uri_request);
}

std::string			&Response::get_method(void)
{
	return (this->_method);
}

std::string			&Response::get_host(void)
{
	return (this->_host);
}

std::string			&Response::get_content_location(void)
{
	return (this->_content_location);
}

std::string			&Response::get_content_type(void)
{
	return (this->_content_type);
}

std::string			&Response::get_name_server(void)
{
	return (this->_name_server);
}

std::string			&Response::get_transfer_encoding(void)
{
	return (this->_transfert_encoding);
}

std::string			&Response::get_version_http(void)
{
	return (this->_version_http);
}

int					&Response::get_content_length(void)
{
	return (this->_content_length);
}
std::pair<int, std::string>	&Response::get_code_etat(void)
{
	return (this->_code_etat);
}

std::ostream		&operator<<(std::ostream &os, Response &r)
{
	(void)r;
	os << "HERE" << std::endl;
	return (os);
}