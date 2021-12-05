/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requete_tester.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 12:26:07 by elie              #+#    #+#             */
/*   Updated: 2021/12/05 10:13:41 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUETE_TESTER_HPP
# define REQUETE_TESTER_HPP

# include "Utils_tester.hpp"

class Requete_tester
{
	public:
		Requete_tester();
		Requete_tester(const Requete_tester &rt);
		virtual ~Requete_tester();

		virtual void		affiche(void) const;
		std::string			int_to_string(int value);
		void				clear(void);

	protected:
		std::pair<int, std::string>				_code;
		std::string								_requete;
		std::string								_attendu;
		std::string								_entete;
		std::string								_first_line;
};

#endif