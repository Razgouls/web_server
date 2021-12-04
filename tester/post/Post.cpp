/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 12:10:56 by elie              #+#    #+#             */
/*   Updated: 2021/12/04 17:29:34 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"

Post::Post() : Requete_tester()
{

}

Post::~Post()
{

}

void			Post::post_no_file(std::string &request)
{
	_code.first = 403;
	_code.second = "Forbidden";
	_attendu = "No file specified -> Error 403 Forbidden";
	request = "POST /post/ HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n";
	request += "Content-Length: 0\r\n";
	request += "\r\n";
	_requete = request;
	_entete = "POST NO FILE SPECIFIED";
}

void			Post::post_create_file_empty(std::string &request)
{
	_code.first = 201;
	_code.second = "Created";
	_attendu = "VIDE";
	request = "POST /post/empty.html HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n";
	request += "Content-Length: 0\r\n";
	request += "\r\n";
	_requete = request;
	_entete = "POST CREATE FILE EMPTY";
}

void			Post::post_create_file_data(std::string &request)
{
	_code.first = 201;
	_code.second = "Created";
	_attendu = "Bonjour tout le monde ";
	request = "POST /post/data.html HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n";
	request += "Content-Length: 22\r\n";
	request += "\r\n";
	request += "Bonjour tout le monde ";
	_requete = request;
	_entete = "POST CREATE FILE DATA";
}

void			Post::post_update_file(std::string &request)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "Bonjour tout le monde comment allez-vous ?";
	request = "POST /post/data.html HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n";
	request += "Content-Length: 20\r\n";
	request += "\r\n";
	request += "comment allez-vous ?";
	_requete = request;
	_entete = "POST UPDATE FILE DATA";
}

void			Post::post_update_file_no_data(std::string &request)
{
	_code.first = 204;
	_code.second = "No Content";
	_attendu = "Bonjour tout le monde comment allez-vous ?";
	request = "POST /post/data.html HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n";
	request += "Content-Length: 0\r\n";
	request += "\r\n";
	_requete = request;
	_entete = "POST UPDATE FILE NO BODY";
}

// void			Post::post_upload_filepath_route(std::string &request)
// {
	
// }

void			Post::post_cgi_no_data(std::string &request)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "";
	request = "POST /cgi/script_sh.sh HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n";
	request += "Content-Length: 0\r\n\r\n";
	_requete = request;
	_entete = "POST CGI EMPTY";
}

void			Post::post_cgi_data(std::string &request)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "Bonjour=Hello";
	request = "POST /cgi/script_sh.sh HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n";
	request += "Content-Length: 13\r\n\r\n";
	request += "Bonjour=Hello";
	_requete = request;
	_entete = "POST CGI DATA";
}

// void			Post::post_cgi_form(std::string &request)
// {

// }

void			Post::post_create_file_data_chunked(std::string &request)
{
	_code.first = 201;
	_code.second = "Created";
	_attendu = "hello=bonjour";
	request = "POST /post/data_chunked.html HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	request += "Transfer-Encoding: chunked\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	request += "d\r\nhello=bonjour\r\n0\r\n\r\n";
	_requete = request;
	_entete = "POST FILE BASIC CHUNKED";
}

void			Post::post_create_file_data_multi_chunked(std::string &request)
{
	_code.first = 201;
	_code.second = "Created";
	_attendu = "hello=test&hell01=test1&hello22=test22";
	request = "POST /post/data_multi_chunked.html HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	request += "Transfer-Encoding: chunked\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	request += "a\r\nhello=test\r\nc\r\nhell01=test1\r\ne\r\nhello22=test22\r\n0\r\n\r\n";
	_requete = request;
	_entete = "POST FILE MULTI CHUNKED";
}

void			Post::post_cgi_data_chunked(std::string &request)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "hello=bonjour";
	request = "POST /post/script_sh.sh HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	request += "Transfer-Encoding: chunked\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	request += "d\r\nhello=bonjour\r\n0\r\n\r\n";
	_requete = request;
	_entete = "POST CGI BASIC CHUNKED";
}

void			Post::post_cgi_data_multi_chunked(std::string &request)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "hello=test&hell01=test1&hello22=test22";
	request = "POST /post/script_sh.sh HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	request += "Transfer-Encoding: chunked\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	request += "a\r\nhello=test\r\nc\r\nhell01=test1\r\ne\r\nhello22=test22\r\n0\r\n\r\n";
	_requete = request;
	_entete = "POST CGI MULTI CHUNKED";
}

