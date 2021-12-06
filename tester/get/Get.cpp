/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 12:09:25 by elie              #+#    #+#             */
/*   Updated: 2021/12/06 00:52:59 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"

Get::Get() : Requete_tester()
{

}

Get::~Get()
{

}

void			Get::get_file_basic(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = get_file_content("./tester/www/get/basic.html");
	requete = "GET /get/basic.html HTTP/1.1\r\n";
	requete += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET BASIC FILE";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Get::get_file_basic_chunked(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = get_file_content("./tester/www/get/basic.html");
	requete = "GET /get/basic.html HTTP/1.1\r\n";
	requete += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	requete += "Transfer-Encoding: chunked\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	requete += "a\r\nhello=test\r\n0\r\n\r\n";
	_requete = requete;
	_entete = "GET BASIC FILE CHUNKED";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Get::get_file_no_perm(std::string &requete)
{
	_code.first = 403;
	_code.second = "Forbidden";
	_attendu = "No Perm, Error 403";
	requete = "GET /get/no_perm.html HTTP/1.1\r\n";
	requete += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET FILE NO PERM";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Get::get_file_no_found(std::string &requete)
{
	_code.first = 404;
	_code.second = "Not Found";
	_attendu = "No Found, Error 404";
	requete = "GET /get/noexists.html HTTP/1.1\r\n";
	requete += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET FILE NOT FOUND";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Get::get_dir_index_exist(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = get_file_content("./tester/www/get/index/index.html");
	requete = "GET /get/index/ HTTP/1.1\r\n";
	requete += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET DIR INDEX EXISTS";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Get::get_dir_autoindex_on(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = get_dir_content("./tester/www/get/");
	requete = "GET /get/ HTTP/1.1\r\n";
	requete += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET DIR AUTOINDEX ON";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Get::get_dir_autoindex_off(std::string &requete)
{
	_code.first = 404;
	_code.second = "Not Found";
	_attendu = "No Found, Error 404";
	requete = "GET /autoindex/ HTTP/1.1\r\n";
	requete += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET DIR AUTOINDEX OFF";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Get::get_dir_redirection(std::string &requete)
{
	_code.first = 301;
	_code.second = "Moved Permanently";
	_attendu = get_dir_content("./tester/www/get/");
	requete = "GET /get HTTP/1.1\r\n";
	requete += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET DIR REDIRECTION";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Get::get_cgi_empty(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "QS=";
	requete = "GET /cgi/script_sh.sh HTTP/1.1\r\n";
	requete += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET CGI EMPTY";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Get::get_cgi_data_sh(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "QS=Bonjour=Hello";
	requete = "GET /cgi/script_sh.sh?Bonjour=Hello HTTP/1.1\r\n";
	requete += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET CGI DATA";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Get::get_cgi_hello(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "Hello";
	requete = "GET /cgi/hello.sh HTTP/1.1\r\n";
	requete += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET CGI HELLO";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Get::get_cgi_long(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "1..100000";
	requete = "GET /cgi/long.sh HTTP/1.1\r\n";
	requete += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET CGI LONG OUTPUT";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Get::get_cgi_chunked(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "QS=hello=test";
	requete = "GET /cgi/script_sh.sh HTTP/1.1\r\n";
	requete += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	requete += "Transfer-Encoding: chunked\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	requete += "a\r\nhello=test\r\n0\r\n\r\n";
	_requete = requete;
	_entete = "GET BASIC FILE CHUNKED";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Get::get_cgi_chunked_bad_length(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "QS=hello=test";
	requete = "GET /cgi/script_sh.sh HTTP/1.1\r\n";
	requete += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	requete += "Transfer-Encoding: chunked\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	requete += "8\r\nhello=test\r\n0\r\n\r\n";
	_requete = requete;
	_entete = "GET BASIC FILE CHUNKED BAD LENGTH";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Get::get_cgi_multi_chunked(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "QS=hello=test&hell01=test1&hello22=test22";
	requete = "GET /cgi/script_sh.sh HTTP/1.1\r\n";
	requete += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	requete += "Transfer-Encoding: chunked\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	requete += "a\r\nhello=test\r\nc\r\nhell01=test1\r\ne\r\nhello22=test22\r\n0\r\n\r\n";
	_requete = requete;
	_entete = "GET BASIC FILE MULTI CHUNKED";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}
