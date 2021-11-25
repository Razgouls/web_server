/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 18:01:38 by elie              #+#    #+#             */
/*   Updated: 2021/11/25 15:07:28 by elie             ###   ########.fr       */
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
		Request(std::string url_request, std::string request, std::string method, std::string path, std::string path_query, std::string host, std::string body, std::list<std::pair<std::string, std::string> > query_string);
		~Request();

		Request												&operator=(Request &r);
		void												set_request(std::string &r);

		std::string											&get_request(void);
		std::string											&get_url_request(void);
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
		void												fill_query_string(void);
		bool												fill_query_string_aux(size_t &last, size_t &dep, std::string &tmp_path, bool id);
		void												get_infos_space(std::string &str, std::string &new_str, size_t &dep, size_t &last, char delim);
		void												make_query_post_put();
		void												fill_map_request(void);
		void												make_query_post_put2(std::string body_tmp);
		int													is_valid(void);
		void												clear(void);
	

	private:
		std::string											_url_request;
		std::string											_request;
		std::map<std::string, std::string>					_map_request;
		std::string											_method;
		std::string											_path;
		std::string											_path_query;
		std::string											_host;
		std::string											_body;
		std::string											_content_type;
		std::string											_content_length;
		std::list<std::pair<std::string, std::string> >		_query_string;
};

std::ostream				&operator<<(std::ostream &os, Request &r);

#endif

