/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requete_tester.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 12:32:47 by elie              #+#    #+#             */
/*   Updated: 2021/12/05 10:23:15 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Requete_tester.hpp"

Requete_tester::Requete_tester() : _code(200, "OK"), _requete("")
{

}

Requete_tester::Requete_tester(const Requete_tester &rt)
{
	this->_code = rt._code;
	this->_requete = rt._requete;
	this->_attendu = rt._attendu;
	this->_first_line = rt._first_line;
}

Requete_tester::~Requete_tester()
{
	_requete.clear();
}

std::string		Requete_tester::int_to_string(int value)
{
	std::stringstream tmp;

	tmp << value;
	return (tmp.str());
}

void			Requete_tester::clear(void)
{
	_attendu.clear();
	_requete.clear();
	_first_line.clear();
}

void			Requete_tester::affiche(void) const
{
	std::cout << BOLDGREEN << "==================================================================" << std::endl;
	int i = -1;
	int size = _entete.length() + 2;
	int res = 66 - size;
	while (++i < res * 0.5)
		std::cout << BOLDGREEN << "=";
	std::cout << BOLDGREEN << " " << _entete << " ";
	i = -1;
	if (res % 2 == 1)
		res--;
	while (++i < res * 0.5)
		std::cout << BOLDGREEN << "=";
	std::cout << WHITE << std::endl;
	std::cout << BOLDGREEN << "==================================================================" << WHITE << std::endl;
	std::cout << BOLDYELLOW <<  "Expected Status Code : [" << WHITE <<  _code.first << " " << _code.second << BOLDYELLOW << "]" << WHITE << std::endl;
	std::cout << BOLDYELLOW <<  "Expected First Line Header : [" << WHITE << _first_line << BOLDYELLOW << "]" << WHITE << std::endl;
	std::cout << BOLDYELLOW <<  "Expected Body :" << std::endl << "[" << WHITE <<  _attendu << BOLDYELLOW << "]" << WHITE << std::endl << std::endl;
	std::cout << BOLDYELLOW << "Request Send :" << std::endl << "[" << WHITE << _requete << BOLDYELLOW << "]" << WHITE << std::endl << std::endl;
}