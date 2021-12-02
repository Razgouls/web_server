/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 18:01:38 by elie              #+#    #+#             */
/*   Updated: 2021/12/02 22:08:47 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Utils.hpp"

class Request
{
	public:
		Request();
		Request(const Request &r);
		Request(std::string uri_request, std::string request, std::string method, std::string path, std::string path_query, std::string host, std::string body);
		~Request();

		Request												&operator=(Request &r);
		void												set_request(std::string &r);
		void												set_body(std::string &body);

		std::string											&get_request(void);
		std::string											&get_uri_request(void);
		std::map<std::string, std::string>					&get_map_request(void);
		std::string											&get_method(void);
		std::string											&get_path(void);
		std::string											&get_path_query(void);
		std::string											&get_host(void);
		std::string											&get_body(void);
		std::string											&get_content_type(void);
		std::string											&get_content_length(void);
		std::list<std::pair<std::string, std::string> >		&get_query_string(void);

		void												set_path(std::string path);

		void												parse_request(void);
		void												fill_map_request(void);
		int													is_valid(void);
		void												clear(void);
		void												parse_body(void);
		void												multipart(void);
		void												get_content_multipart(std::string &element, std::string &rep);

	

	private:
		std::string											_version;
		std::string											_uri_request;
		std::string											_request;
		std::map<std::string, std::string>					_map_request;
		std::string											_method;
		std::string											_path;
		std::string											_path_query;
		std::string											_host;
		std::string											_body;
		std::string											_content_type;
		std::string											_content_length;

		bool												_error;
};

std::ostream				&operator<<(std::ostream &os, Request &r);

#endif

