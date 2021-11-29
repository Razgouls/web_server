/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 21:24:28 by elie              #+#    #+#             */
/*   Updated: 2021/11/28 23:00:27 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "FileConfig.hpp"

FileConfig::FileConfig(void)
{
	
}

FileConfig::~FileConfig(void)
{

}

FileConfig::FileConfig(std::string first_line, int port, std::string host, std::string server_name, std::string root, int limit_client_body_size,
					std::list<std::string> map_error, std::string methods, std::string path_location,
					std::string autoindex, std::string index, std::string upload_dir, std::string cgi_bin,
					std::string cgi_extension)
{
	_first_line = first_line;
	_port = port;
	_host = host;
	_server_name = server_name;
	_root = root;
	_limit_client_body_size = limit_client_body_size;
	_map_error = map_error;
	_methods = methods;
	_path_location = path_location;
	_autoindex = autoindex;
	_index = index;
	_upload_dir = upload_dir;
	_cgi_bin = cgi_bin;
	_cgi_extension = cgi_extension;
	create_file();
}

void			FileConfig::gen_path(std::string &s, const int len)
{
	s += "./config/testeur/";
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i)
        s += alphanum[rand() % (sizeof(alphanum) - 1)];
	s += ".conf";
}

void			FileConfig::create_file(void)
{
	std::ofstream						myfile;
	std::list<std::string>::iterator	it_begin = _map_error.begin();
	std::list<std::string>::iterator	it_end = _map_error.end();
	std::string							str;

	gen_path(str, 20);
	myfile.open(str.c_str(), std::ofstream::out | std::ofstream::trunc);
	if (!myfile.good())
		throw std::string("Impossible de creer le fichier de configuration.");
	std::cout << "[" << str << "]" << std::endl;
	myfile << _first_line << "\n";
	myfile << "\tlisten\t" << _port << ";\n";
	myfile << "\thost\t" << _host << ";\n";
	myfile << "\tserver_name\t" << _server_name << ";\n";
	myfile << "\troot\t" << _root << ";\n";
	myfile << "\tlimit_client_body_size\t" << _limit_client_body_size << ";\n";
	while (it_begin != it_end)
	{
		myfile << "\terror\t" << (*it_begin) << ";";
		it_begin++;
	}
	myfile << "\n";
	myfile << "\tlocation " << _path_location << "\n";
	myfile << "\t\tmethods\t" << _methods << ";\n";
	myfile << "\t\tautoindex\t" << _autoindex << ";\n";
	myfile << "\t\tupload_dir\t" << _upload_dir << ";\n";
	myfile << "\t\tcgi_extension\t" << _cgi_extension << ";\n";
	myfile << "\t\tcgi_bin\t" << _cgi_bin << ";\n";
	myfile << "\t}\n}\n\n";
}
