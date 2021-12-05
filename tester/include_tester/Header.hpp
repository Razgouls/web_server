/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 12:10:35 by elie              #+#    #+#             */
/*   Updated: 2021/12/05 12:15:56 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
# define HEADER_HPP

# include "Utils_tester.hpp"
# include "Requete_tester.hpp"

class Header : public Requete_tester
{
	public:
		Header(void);
		virtual ~Header();

		/*
		** METHOD
		*/
		void				header_bad_method(std::string &request);
		void				header_empty_method(std::string &request);

	   /*
	   ** VERSION
	   */
	  void					header_bad_version(std::string &request);
	  void					header_empty_version(std::string &request);
	  
	  /*
	  ** FIRST LINE FORMAT
	  */
	 void					header_empty_first_line(std::string &request);
	 void					header_two_infos(std::string &request);
	 void					header_four_infos(std::string &request);
	 void					header_two_first_line(std::string &request);

	/*
	** HOST IP
	*/
	void					header_host_bad_ip(std::string &request);
	void					header_host_empty_ip(std::string &request);
	
	/*
	** HOST FORMAT
	*/
	void					header_host_no_exist(std::string &request);
	void					header_empty_host(std::string &request);
	void					header_host_three_infos(std::string &request);
	void					header_two_host(std::string &request);

	private:
};

#endif