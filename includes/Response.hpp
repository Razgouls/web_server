/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 18:01:48 by elie              #+#    #+#             */
/*   Updated: 2021/11/08 11:34:51 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "lib.hpp"
# include "Request.hpp"

class Response
{
	public:
		Response();
		Response(const Response &r);
		~Response();

		Response					&operator=(const Response &r);
		void						build_response_dir(std::list<std::pair<std::string, unsigned char> > &files);
		void						build_response_file(std::map<int, std::string> map_error, std::string &path, Request &req);
		void						get_length_body(void);
		void						gestion_errors(std::string &path);
		void						build_head_response(void);
		void						build_body_response(std::string &path, int code_etat, Request &req);
		void						build_response_string(std::string mess);
		void						add_header_index(void);
		void						clear();


		void						set_url_request(std::string url_request);
		void						set_method(std::string method);
		void						set_host(std::string host);
		void						set_content_location(std::string content_location);
		void						set_content_type(std::string content_type);
		void						set_name_server(std::string name_server);
		void						set_transfer_encoding(std::string transfer_encoding);
		void						set_version_http(std::string version_http);
		void						set_date(time_t date);
		void						set_content_length(int content_length);
		void						set_code_etat(int code_etat);
		void						set_limit_body_size(int limit_body_size);

		std::string					get_url_request(void) const;
		std::string					get_method(void) const;
		std::string					get_host(void) const;
		std::string					get_content_location(void) const;
		std::string					get_content_type(void) const;
		std::string					get_name_server(void) const;
		std::string					get_transfer_encoding(void) const;
		std::string					get_version_http(void) const;
		time_t						get_date(void) const;
		int							get_content_length(void) const;
		int							get_code_etat(void) const;
		std::list<std::string>		get_body(void) const;
		std::string					get_reponse(void);


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
		int							_code_etat;
		int							_limit_body_size;
		std::list<std::string>		_body;
};

std::ostream						&operator<<(std::ostream &os, Response &r);

#endif