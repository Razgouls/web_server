/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 12:07:55 by elie              #+#    #+#             */
/*   Updated: 2021/12/05 19:03:58 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Delete.hpp"

Delete::Delete() : Requete_tester()
{

}

Delete::~Delete()
{

}

void			Delete::delete_file_exist(std::string &request)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "File Deleted";
	request = "DELETE /delete/file.txt HTTP/1.1\r\n";
	request += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n";
	request += "\r\n";
	_requete = request;
	_entete = "DELETE FILE EXISTS";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Delete::delete_file_not_exist(std::string &request)
{
	_code.first = 404;
	_code.second = "Not Found";
	_attendu = "File Not Found";
	request = "DELETE /delete/file.txt HTTP/1.1\r\n";
	request += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n";
	request += "\r\n";
	_requete = request;
	_entete = "DELETE FILE NOT EXISTS";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Delete::delete_file_no_perm(std::string &request)
{
	_code.first = 403;
	_code.second = "Forbidden";
	_attendu = "No Perm to delete file";
	request = "DELETE /delete/noperm.html HTTP/1.1\r\n";
	request += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n";
	request += "\r\n";
	_requete = request;
	_entete = "DELETE FILE NO PERM";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}

void			Delete::delete_dir(std::string &request)
{
	_code.first = 200;
	_code.second = "OK";
	_attendu = "Dir isn't deleted";
	request = "DELETE /delete/ HTTP/1.1\r\n";
	request += "Host: localhost:" + int_to_string(PORT) + "\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n";
	request += "\r\n";
	_requete = request;
	_entete = "DELETE DIR";
	_first_line = "HTTP/1.1 " + int_to_string(_code.first) + " " + _code.second;
}
