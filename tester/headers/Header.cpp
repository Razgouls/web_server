/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 12:10:23 by elie              #+#    #+#             */
/*   Updated: 2021/12/05 19:04:23 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"

Header::Header() : Requete_tester()
{
	
}

Header::~Header()
{

}

void			Header::header_bad_method(std::string &request)
{
	_code.first = 400;
	_code.second = "Bad Request";
	_attendu = "Bad Request (Bad Method)";
	request = "BAD /get/basic.html HTTP/1.1\r\n";
	request += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = request;
	_entete = "HEADER BAD METHOD";
	_first_line = "Not Specified";
}

void			Header::header_empty_method(std::string &request)
{
	_code.first = 400;
	_code.second = "Bad Request";
	_attendu = "Bad Request (Empty Method)";
	request = " /get/basic.html HTTP/1.1\r\n";
	request += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = request;
	_entete = "HEADER EMPTY METHOD";
	_first_line = "Not Specified";
}

void			Header::header_bad_version(std::string &request)
{
	_code.first = 505;
	_code.second = "Version Not Supported";
	_attendu = "Bad Request (Bad Version)";
	request = "GET /get/basic.html HTTP/1.4\r\n";
	request += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = request;
	_entete = "HEADER BAD VERSION";
	_first_line = "Not Specified";
}

void			Header::header_empty_version(std::string &request)
{
	_code.first = 505;
	_code.second = "Version Not Supported";
	_attendu = "Bad Request (Empty Version)";
	request = "GET /get/basic.html \r\n";
	request += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = request;
	_entete = "HEADER EMPTY VERSION";
	_first_line = "Not Specified";
}

void			Header::header_empty_first_line(std::string &request)
{
	_code.first = 400;
	_code.second = "Bad Request";
	_attendu = "Bad Request (Empty First Line)";
	request = " \r\n";
	request += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = request;
	_entete = "HEADER EMPTY FIRST LINE";
	_first_line = "Not Specified";
}

void			Header::header_two_infos(std::string &request)
{
	_code.first = 400;
	_code.second = "Bad Request";
	_attendu = "Bad Request (Two Infos)";
	request = "GET HTTP/1.1\r\n";
	request += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = request;
	_entete = "HEADER TWO INFOS";
	_first_line = "Not Specified";
}

void			Header::header_four_infos(std::string &request)
{
	_code.first = 400;
	_code.second = "Bad Request";
	_attendu = "Bad Request (Four Infos)";
	request = "GET /get/basic.html HTTP/1.1 NEW_INFOS\r\n";
	request += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = request;
	_entete = "HEADER FOUR INFOS";
	_first_line = "Not Specified";
}

void			Header::header_two_first_line(std::string &request)
{
	_code.first = 400;
	_code.second = "Bad Request";
	_attendu = "Bad Request (Two First Line)";
	request = "GET /get/basic.html HTTP/1.1\r\n";
	request += "GET /get/basic.html HTTP/1.1\r\n";
	request += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = request;
	_entete = "HEADER TWO FIRST LINE";
	_first_line = "Not Specified";
}

void			Header::header_host_bad_ip(std::string &request)
{
	_code.first = 400;
	_code.second = "Bad Request";
	_attendu = "Bad Request (Bad IP)";
	request = "GET /get/basic.html HTTP/1.1\r\n";
	request += "Host: local:" + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = request;
	_entete = "HEADER BAD IP";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Header::header_host_empty_ip(std::string &request)
{
	_code.first = 400;
	_code.second = "Bad Request";
	_attendu = "Bad Request (Empty Ip)";
	request = "GET /get/basic.html HTTP/1.1\r\n";
	request += "Host: " + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = request;
	_entete = "HEADER EMPTY IP";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Header::header_host_no_exist(std::string &request)
{
	_code.first = 400;
	_code.second = "Bad Request";
	_attendu = "Bad Request (Host No Exist)";
	request = "GET /get/basic.html HTTP/1.1\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = request;
	_entete = "HEADER HOST NO EXIST";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Header::header_empty_host(std::string &request)
{
	_code.first = 400;
	_code.second = "Bad Request";
	_attendu = "Bad Request (Empty Host)";
	request = "GET /get/basic.html HTTP/1.1\r\n";
	request += "Host: \r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = request;
	_entete = "HEADER EMPTY HOST";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Header::header_host_three_infos(std::string &request)
{
	_code.first = 400;
	_code.second = "Bad Request";
	_attendu = "Bad Request (Three Infos)";
	request = "GET /get/basic.html HTTP/1.1\r\n";
	request += "Host: localhost:" + int_to_string(PORT) + " NEW_INFO" + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = request;
	_entete = "HEADER THREE INFOS";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Header::header_two_host(std::string &request)
{
	_code.first = 400;
	_code.second = "Bad Request";
	_attendu = "Bad Request (Two Host)";
	request = "GET /get/basic.html HTTP/1.1\r\n";
	request += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	request += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
	_requete = request;
	_entete = "HEADER TWO HOST";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}
