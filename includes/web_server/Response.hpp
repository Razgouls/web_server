/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 18:01:48 by elie              #+#    #+#             */
/*   Updated: 2021/11/25 15:07:21 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Utils.hpp"
# include "Request.hpp"

class Response
{
	public:
		/*
		** COPLIEN
		*/
		Response();
		Response(const Response &r);
		~Response();
		Response					&operator=(const Response &r);

		/*
		** FUNCTIONS
		*/
		void						clear();
		std::string					fill_reponse(void);
		void						build_head_response(void);
		int							gestion_errors(std::string &path);
		void						build_body_response_aux(std::string &path);
		void						build_body_response(std::pair<int, std::string> infos);
		void						add_header_index(void);
		void						build_response_dir(std::list<std::pair<std::string, unsigned char> > &files);


		/*
		** SETTERS
		*/
		void						set_url_request(std::string &url_request);
		void						set_method(std::string &method);
		void						set_host(std::string &host);
		void						set_content_location(std::string &content_location);
		void						set_content_type(std::string content_type);
		void						set_name_server(std::string name_server);
		void						set_transfer_encoding(std::string &transfer_encoding);
		void						set_version_http(std::string version_http);
		void						set_date(time_t date);
		void						set_content_length(int content_length);
		void						set_code_etat(int code_etat, std::string mess);
		void						set_limit_body_size(int limit_body_size);

		/*
		** GETTERS
		*/
		std::string					&get_url_request(void);
		std::string					&get_method(void);
		std::string					&get_host(void);
		std::string					&get_content_location(void);
		std::string					&get_content_type(void);
		std::string					&get_name_server(void);
		std::string					&get_transfer_encoding(void);
		std::string					&get_version_http(void);
		time_t						&get_date(void);
		int							&get_content_length(void);
		std::pair<int, std::string>	&get_code_etat(void);
		std::list<std::string>		&get_body(void);


	private:
		std::string					_url_request;
		std::string					_method;
		std::string					_host;
		std::string					_content_location;
		std::string					_content_type;
		std::string					_name_server;
		std::string					_transfert_encoding;
		std::string					_version_http;
		time_t						_date;
		int							_content_length;
		std::pair<int, std::string>	_code_etat;
		int							_limit_body_size;
		std::list<std::string>		_body;
		std::string					_body_head;
		std::string					_body_response;
};

std::ostream						&operator<<(std::ostream &os, Response &r);

#endif