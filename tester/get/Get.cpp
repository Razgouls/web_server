/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 12:09:25 by elie              #+#    #+#             */
/*   Updated: 2021/12/03 16:31:16 by elie             ###   ########.fr       */
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
	_attendu = get_file_content("./www/get/basic.html");
	requete = "GET /get/basic.html HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n";
	requete += "Content-Length: 10\r\n\r\n";
	requete += "hello=test\r\n\r\n";
	_requete = requete;
	_entete = "GET BASIC FILE";
}

void			Get::get_file_basic_chunked(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = get_file_content("./www/get/basic.html");
	requete = "GET /get/basic.html HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	requete += "Transfer-Encoding: chunked\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	requete += "a\r\nhello=test\r\n0\r\n\r\n";
	_requete = requete;
	_entete = "GET BASIC FILE CHUNKED";
}

void			Get::get_file_no_perm(std::string &requete)
{
	_code.first = 403;
	_code.second = "Forbidden";
	_attendu = "No Perm, Error 403";
	requete = "GET /get/no_perm.html HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET FILE NO PERM";
}

void			Get::get_file_no_found(std::string &requete)
{
	_code.first = 404;
	_code.second = "Not Found";
	_attendu = "No Found, Error 404";
	requete = "GET /get/noexists.html HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET FILE NOT FOUND";
}

void			Get::get_dir_index_exist(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = get_file_content("./www/get/index/index.html");
	requete = "GET /get/index/ HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET DIR INDEX EXISTS";
}

void			Get::get_dir_autoindex_on(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = get_dir_content("./www/get/");
	requete = "GET /get/ HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET DIR AUTOINDEX ON";
}

void			Get::get_dir_autoindex_off(std::string &requete)
{
	_code.first = 404;
	_code.second = "Not Found";
	_attendu = "No Found, Error 404";
	requete = "GET /autoindex/ HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET DIR AUTOINDEX OFF";
}

void			Get::get_dir_redirection(std::string &requete)
{
	_code.first = 301;
	_code.second = "Moved Permanently";
	_attendu = get_dir_content("./www/get/");
	requete = "GET /get HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET DIR REDIRECTION";
}

void			Get::get_cgi_empty(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "";
	requete = "GET /cgi/script_sh.sh HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET CGI EMPTY";
}

void			Get::get_cgi_data_sh(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "Bonjour=Hello";
	requete = "GET /cgi/script_sh.sh?Bonjour=Hello HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET CGI DATA";
}

void			Get::get_cgi_hello(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "Hello";
	requete = "GET /cgi/hello.sh HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET CGI HELLO";
}

void			Get::get_cgi_long(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "1..100000";
	requete = "GET /cgi/long.sh HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = requete;
	_entete = "GET CGI LONG OUTPUT";
}

void			Get::get_cgi_chunked(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "hello=test";
	requete = "GET /cgi/script_sh.sh HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	requete += "Transfer-Encoding: chunked\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	requete += "a\r\nhello=test\r\n0\r\n\r\n";
	_requete = requete;
	_entete = "GET BASIC FILE CHUNKED";
}

void			Get::get_cgi_multi_chunked(std::string &requete)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "hello=test&hell01=test1&hello22=test22";
	requete = "GET /cgi/script_sh.sh HTTP/1.1\r\nHost: localhost: " + int_to_string(PORT) + "\r\n";
	requete += "Transfer-Encoding: chunked\r\n";
	requete += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	requete += "a\r\nhello=test\r\nc\r\nhell01=test1\r\ne\r\nhello22=test22\r\n0\r\n\r\n";
	_requete = requete;
	_entete = "GET BASIC FILE MULTI CHUNKED";
}
