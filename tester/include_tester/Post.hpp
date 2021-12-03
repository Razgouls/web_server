/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 12:11:06 by elie              #+#    #+#             */
/*   Updated: 2021/12/03 17:37:57 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HPP
# define POST_HPP

# include "Utils_tester.hpp"
# include "Requete_tester.hpp"

class Post : public Requete_tester
{
	public:
		Post(void);
		virtual ~Post();

		/*
		** CREATE / UPDATE FILE
		*/
		void			post_no_file(std::string &request);
		void			post_create_file_empty(std::string &request);
		void			post_create_file_data(std::string &request);
		void			post_update_file(std::string &request);
		void			post_update_file_no_data(std::string &request);

		/*
		** UPLOAD FILE
		*/
		void			post_upload_file(std::string &request);
		void			post_upload_filepath_route(std::string &request);

		/*
		** CGI
		*/
		void			post_cgi_no_data(std::string &request);
		void			post_cgi_data(std::string &request);
		void			post_cgi_form(std::string &request);

		/*
		** CHUNKED
		*/
		void			post_create_file_data_chunked(std::string &request);
		void			post_create_file_data_multi_chunked(std::string &request);
		void			post_cgi_data_chunked(std::string &request);
		void			post_cgi_data_multi_chunked(std::string &request);

	private:
};

#endif