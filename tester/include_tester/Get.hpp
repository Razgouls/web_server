/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 12:10:08 by elie              #+#    #+#             */
/*   Updated: 2021/12/03 16:30:54 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
# define GET_HPP

# include "Utils_tester.hpp"
# include "Requete_tester.hpp"

class Get : public Requete_tester
{
	public:
		Get(void);
		virtual ~Get();

		/*
		** FILE
		*/
		void			get_file_basic(std::string &requete);
		void			get_file_basic_chunked(std::string &requete);
		void			get_file_basic_multi_chunked(std::string &requete);
		void			get_file_no_perm(std::string &requete);
		void			get_file_no_found(std::string &requete);

		/*
		** DIR
		*/
		void			get_dir_index_exist(std::string &requete);
		void			get_dir_autoindex_on(std::string &requete);
		void			get_dir_autoindex_off(std::string &requete);
		void			get_dir_redirection(std::string &requete);

		/*
		** CGI
		*/
		void			get_cgi_empty(std::string &requete);
		void			get_cgi_data_sh(std::string &requete);
		void			get_cgi_hello(std::string &requete);
		void			get_cgi_long(std::string &requete);
		void			get_cgi_chunked(std::string &requete);
		void			get_cgi_multi_chunked(std::string &requete);

	private:
};

#endif