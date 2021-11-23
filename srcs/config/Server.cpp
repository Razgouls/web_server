/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 12:24:59 by elie              #+#    #+#             */
/*   Updated: 2021/11/23 18:15:12 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
** FORME COPLIEN
*/

/*
** Constructeur par default
*/
Server::Server()
{

}

/*
** Constructeur par copie
*/
Server::Server(const Server &s)
{
	this->_current_rep = s._current_rep;
	this->_current_route = s._current_route;
	this->_server_fd = s._server_fd;
	this->_vect_address = s._vect_address;
	this->_vect_listen_fd = s._vect_listen_fd;
	this->_index = s._index;
	this->_list_server = s._list_server;
	this->_server_fd = s._server_fd;
	
	int i = 0;
	while (i < SIZE_PFDS)
	{
		if ((size_t)i < _vect_listen_fd.size())
			_pfds[i] = s._pfds[i];
		else
			_pfds[i].fd = -1;
		i++;
	}
}


/*
** Opérateur d'assignation
*/
Server					&Server::operator=(const Server &s)
{
	this->_vect_address.clear();
	this->_vect_listen_fd.clear();
	this->_list_server.clear();
	this->_current_rep = s._current_rep;
	this->_current_route = s._current_route;
	this->_server_fd = s._server_fd;
	this->_vect_address = s._vect_address;
	this->_vect_listen_fd = s._vect_listen_fd;
	this->_index = s._index;
	this->_list_server = s._list_server;
	this->_server_fd = s._server_fd;
	
	int i = 0;
	while (i < SIZE_PFDS)
	{
		if ((size_t)i < _vect_listen_fd.size())
			_pfds[i] = s._pfds[i];
		else
			_pfds[i].fd = -1;
		i++;
	}
	return (*this);
}


/*
** Destructeur
*/
Server::~Server()
{
	memset(_pfds, 0, SIZE_PFDS);
}


/*
** INIT / ACCEPT (SOCKET)
*/

/*
** Cette fonction va tout simplement initialiser la socket qui va ecouter si connexions entrantes ya
**		1.	La fonction socket va creer un point de communication et va renvoyer un fd
**		2.	La fonction setsockopt va permettre de reutiliser un port utilisé lors du dernier lancement du serveur (on evite les Erreur bin car le port est deja utilisé)
**		3.	On initialise la structure _address (struct sockaddr _address) à 0
**		4.	On la remplit par 3 informations :
**				-> sin_family	: type de famille : AT_INET pour IPv4 (principalement utilisée avec le protocole TCP/IP)
**				-> sin_port		: correspond au port à contacter (htons va convertir le port pour l'envoyer sur le reseau (pas besoin d'utiliser htons ou autre si on n'utilise pas le reseau -> donc ici inutile))
**				-> sin_addr.s_addr : correspond à l'addresse IP utilisée
**		5.	La fonction fcntl va rendre la socket non bloquante
**		6.	La fonction bind va etre utilisée pour attribuer au socket une addresse et un port (grace a la structure sock_addr)
**		7.	La fonction listen met le socket (listen_fd) en attente de connexion (ne marche qu'avec le protocle TCP/IP)
*/
void					Server::init_listen_fd(void)
{
	int		i = 0;
	int		size = _list_server.size();

	while (i < size)
	{
		int		on = 1;
		struct sockaddr_in add;

		int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (listen_fd < 0)
			continue ;
		setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		memset(&add, 0, sizeof(add));
		add.sin_family = AF_INET;
		add.sin_port = htons(_list_server[i].get_listen());
		add.sin_addr.s_addr = inet_addr(_list_server[0].get_host().c_str());
		fcntl(listen_fd, F_SETFL, O_NONBLOCK);
		if (bind(listen_fd, (struct sockaddr *)&add, sizeof(add)) < 0)
		{
			close(listen_fd);
			continue ;
		}
		if (listen(listen_fd, MAX_CONN) < 0)
		{
			close(listen_fd);
			continue ;
		}
		_vect_address.push_back(add);
		_vect_listen_fd.push_back(listen_fd);
		i++;
	}
}


/*
** La fonction accept permet la connexion en acceptant un appel (_address stock l'adresse de l'appelant)
** Elle va retourner une nouvelle socket qui a son va etre non bloquant grace a la fonction fcntl
*/
int						Server::s_accept(int j)
{
	int		len = sizeof(sockaddr_in);
	int		new_fd;

	if ((new_fd = accept(_vect_listen_fd[j], (struct sockaddr *)&_vect_address[j], (socklen_t *)&len)) < 0)
		perror("accept");
	fcntl(new_fd, F_SETFL, O_NONBLOCK);
	return (new_fd);
}


/*
** Cette fonction permet 2 choses :
**		1.	Initialiser la socket _listen_fd (socket qui va ecouter les connexion entrantes)
**		2.	Initialiser la struct pollfd :
**			-> On malloc (200 correspond a la taille du tableau de socket -> socket a surveiller)
**			-> On initialise la premiere case :
**					- .fd : correspond au socket
**					- .events : aux evenements (POLLRDNORM : Données en attente de lecture)
*/
void					Server::init_pfds(void)
{
	int		size;
	int		i = 0;

	init_listen_fd();
	bzero(_pfds, SIZE_PFDS);
	size = _vect_listen_fd.size();
	while (i < SIZE_PFDS)
	{
		if (i < size)
		{
			_pfds[i].fd = _vect_listen_fd[i];
			_pfds[i].events = POLLIN;
			_pfds[i].revents = 0;
		}
		else
			_pfds[i].fd = -1;
		i++;
	}
}


/*
** Cette fonction va appeler la fonction poll qui permet de verifier s'il y a des données en attente de lecture (POLLRDNORM) -> Si oui la fonction poll(...) nous avertira
**		1.	_pfds : correspond au tableau de sockets
**		2.	*nfds : correspond au nombre de socket present dans _pfds (donc a ca size)
**		3.	-1 : delai d'attente en millisecondes (-1 correspond a un delai d'attente infini)
**		4.	nbr_count : la fonction poll(...) renvoie le nombre d'éléments du tableau ayant eu un évènement
*/
int					Server::init_poll(int *nfds)
{
	int		nbr_count = poll(_pfds, *nfds, -1);

	if (nbr_count < 0)
		throw std::string("");
	if (nbr_count == 0)
		throw std::string("Timeout lors du pull");
	return (nbr_count);
}


/*
** GESTION REPONSE EN FONCTION DU TYPE (FILE OU DIR)
*/


/*
** Cette fonction va permettre preparer le path de la requete et va etre comparer pour savoir de quelle location elle correspond, si aucune par default ca sera la location /
** On stocke dans _current_route, la location qui va lui correspondre (par default cest location /)
** EX : path : ./www	-- PREPARATION -->	/www
** EX : path : ./www/	-- PREPARATION -->	/www
*/
void						Server::get_req_route(void)
{
	std::list<Route>			list_route = _list_server[_index].get_list_routes();
	std::list<Route>::iterator	it_begin = list_route.begin();
	std::list<Route>::iterator	it_end = list_route.end();
	std::string					path_search_route;
	Route						tmp_route;

	path_search_route = _current_req.get_path().substr(1);
	if ((*path_search_route.rbegin()) == '/')
		path_search_route.erase(path_search_route.length() - 1);
	while (it_begin != it_end)
	{
		if (path_search_route == (*it_begin).get_path())
		{
			_current_route = *it_begin;
			break ;
		}
		if ((*it_begin).get_path() == "/")
			tmp_route = (*it_begin);
		it_begin++;
	}
	if (it_begin != it_end)
		_current_route = *it_begin;
	else
		_current_route = tmp_route;
}

/*
** Cette fonction va regarder si la methode utilisé par la requete (GET / POST / DELETE) est présente dans la location
** Si elle l'est alors on return true, sinon on retrourne false, indiquant que la methode n'est pas présente --> Error 405 : Method Not Allowed
*/
bool					Server::gestion_valid_method(void)
{
	std::list<std::string>				list_method = _current_route.get_list_methods();
	std::list<std::string>::iterator	it_begin = list_method.begin();
	std::list<std::string>::iterator	it_end = list_method.end();
	
	while (it_begin != it_end)
	{
		if ((*it_begin) == _current_req.get_method())
			return (true);
		it_begin++;
	}
	return (false);
}


/*
** Cette fonction va verifier 2 choses :
** 		--> Premierement si le fichier index.html existe	-> Si oui on affiche index.html, sinon on passe au deuxieme tiret.
**		--> Deuxiemement si le fichier index.html n'existe pas on regarde si l'autoindex est à on ou off. Si oui, on affiche l'arborescence, sinon Error 404
**
** Nous utilisons une pair : 
**		--> Le premier bool indique si le fichier index.html existe
**		--> Le deuxieme bool indique si l'autoindex est à on ou off
*/
int		Server::gestion_auto_index(void)
{
	std::ifstream		myfile;
	std::string			index;
	std::string			&path = _current_req.get_path();

	index = path.append("index.html");
	myfile.open(index.c_str());
	if (myfile.good() && !_current_route.get_index().empty())
		return (INDEX);
	else if (_current_route.get_auto_index() == "on")
		return (AUTOINDEX);
	return (NONE);
}


/*
** Cette fonction va permettre de stocker chaque nom de fichiers / dossiers / ... dans une list
**		-> Elle prend en parametre un DIR (le dossier a lire)
** 		-> Avec une boucle et readdir, on va parcourir ce dossier et stocker les noms dans une list
**		-> Nous stockons des pair :
**			-> Le premier est un std::string qui stocke le nom du fichier / dossiers / ...
**			-> Le deuxieme stocke le type (DT_DIR -> dossiers, DT_REG -> fichiers etc)
*/
std::list<std::pair<std::string, unsigned char> >	Server::get_file_in_dir(DIR *dir)
{
	std::list<std::pair<std::string, unsigned char> >	files;
	struct dirent										*dirread;

	files.clear();
	if (dir)
	{
		while ((dirread = readdir(dir)) != NULL)
		{
			std::stringstream	tmp;
			tmp << dirread->d_name;
			files.push_back(std::make_pair(tmp.str(), dirread->d_type));
		}
	}
	closedir(dir);
	return (files);
}


/*
** Cette fonction permet juste de remplir la _current_rep (donc la reponse qui va etre envoyee au client / navigateur)
*/
void					Server::fill_current_rep(void)
{
	_current_rep.set_code_etat(200, "OK");
	_current_rep.set_method(_current_req.get_method());
	_current_rep.set_host(_current_req.get_host());
	_current_rep.set_url_request(_current_req.get_url_request());
	_current_rep.set_version_http("HTTP/1.1");
	_current_rep.set_content_type(_current_req.get_content_type());
	if (_current_req.get_content_type().empty())
		_current_rep.set_content_type("text/html");
	_current_rep.set_name_server("web_server/1.10.0 (Ubuntu)");
	_current_rep.set_limit_body_size(_list_server[_index].get_limite_body_size());
}


/*
** Cette fonction est appelee si la methode utilisee est DELETE
** On essaie d'open la ressource (si une facond e voir si la ressource en question existe tout simplement)
** 		SI FAIL : alors on affiche la page Page Not Found car la ressource n'existe pas
**		SINON OK : on remove la ressource (fonction remove(path_ressource)), et on indique qu'elle a bien ete supprimé grace a une page html (File is deleted)
*/
void					Server::delete_resource(void)
{
	std::ifstream		myfile;
	std::string			mess("");

	myfile.open(_current_req.get_path().c_str());
	if (!myfile.good())
	{
		_current_rep.set_code_etat(404, "Not Found");
		_current_rep.build_body_response(std::make_pair(FILE, _current_map_error[404]));
	}
	else
	{
		remove(_current_req.get_path().c_str());
		mess = "File " + _current_req.get_path() + " is deleted";
		_current_rep.set_code_etat(200, "OK");
		_current_rep.build_body_response(std::make_pair(MESSAGE, mess));
	}
}

/*
** Cette fonction est appelee si la methode utilisée eatit une methode POST (notammenent pour le formulaire)
*/
//A RETRAIVAILLER AVEC LE CGI
void					Server::post_resource(void)
{
	std::string tmp_path = _current_req.get_path();
	std::cout << "TMP PATH : [" << tmp_path << "]" << std::endl;
	// _current_rep.build_body_response(std::make_pair(MESSAGE, "Le contenu a ete ajouté a " + tmp_path));
}

void					Server::put_resource(void)
{
	std::string		tmp_path(_current_req.get_path());
	int				last_slash = tmp_path.find("/");
	std::ofstream	myfile;

	tmp_path.erase(0, last_slash + 1);
	if (Utils::is_file(tmp_path))
	{
		myfile.open(tmp_path.c_str(), std::ofstream::in);
		if (myfile.good())
		{
			if (_current_req.get_content_length() == "0")
				_current_rep.set_code_etat(204, "No Content");
			else
				myfile << _current_req.get_body();
			_current_rep.set_code_etat(200, "OK");
			_current_rep.build_body_response(std::make_pair(MESSAGE, "Le contenu a ete ajouté a " + tmp_path));
			myfile.close();
		}
		else
		{
			_current_rep.set_code_etat(403, "Forbidden");
			_current_rep.build_body_response(std::make_pair(FILE, _current_map_error[403]));
		}
	}
	else if (Utils::is_dir(tmp_path))
	{
		_current_rep.set_code_etat(403, "Forbidden");
		_current_rep.build_body_response(std::make_pair(FILE, _current_map_error[403]));
	}
	else
	{
		myfile.open(tmp_path.c_str(), std::ofstream::out);
		if (!myfile.is_open() || !myfile.good())
		{
			_current_rep.set_code_etat(403, "Forbidden");
			_current_rep.build_body_response(std::make_pair(FILE, _current_map_error[403]));
		}
		else
		{
			myfile << _current_req.get_body();
			_current_rep.set_code_etat(201, "Created");
			_current_rep.build_body_response(std::make_pair(MESSAGE, "Fichier " + tmp_path + " creer et contenu ajouté"));
			myfile.close();
		}
	}
}


/*
**		1.	On ouvre le dossier correspondant au path
**		2.	On appelle la fonction qui va gerer l'autoindex et le fichier index.html (gestion_auto_index())
**		3.	Si fichier index.html existe :
**				-> OUI : alors on build le body avec le fichier index.html
**				-> SINON :	SI on opendir un fichier (fail du coup)
**								OUI : alors on build le body avec le fichier correspondant
**							SINON : Si l'autoindex est a on :
**								OUI : On build l'arborescence du dossier correspondant (on effectue eventuellement une redirection 301 s'il manque le '/' a la fin alors que cest un dossier)
**							SINON : On build une erreur 404 (car on opendir un dossier mais il n'y avait pas d'index.html et l'autoindex etait a off)
*/
void					Server::get_resource(char **env)
{
	std::list<std::pair<std::string, unsigned char> >	files;
	int													ret;
	DIR													*dir;
	std::string											path = _current_req.get_path();

	dir = opendir(path.c_str());
	ret = gestion_auto_index();
	_current_rep.set_content_type(_mime[Utils::get_extension(path)]);




	if (Utils::get_extension(path) == _current_route.get_cgi_extension())
	{
		CGI		cgi;

		cgi.set_cgi_bin(_current_route.get_cgi_bin());
		cgi.set_cgi_extension(_current_route.get_cgi_extension());
		cgi.add_var_env("GATEWAY_INTERFACE", "CGI/1.1");
		cgi.add_var_env("HTTP_ACCEPT", _current_req.get_map_request()["Accept"]);
		cgi.add_var_env("HTTP_ACCEPT_LANGUAGE", _current_req.get_map_request()["Accept-Language"]);
		cgi.add_var_env("HTTP_ACCEPT_ENCODING", _current_req.get_map_request()["Accept-Encoding"]);
		cgi.add_var_env("HTTP_CONNECTION", _current_req.get_map_request()["Connection"]);
		// cgi.add_var_env("HTTP_HOST", _h)

		pid_t	pid;
		int		status;

		pid = fork();
		if (pid == 0)
		{
			char **t = (char **)malloc(sizeof(char *) * 3);
			t[0] = strdup("/usr/bin/php-cgi");
			t[1] = strdup(path.c_str());
			t[2] = 0;
			std::cout << "PATH : [" << path << "]" << std::endl;
			if (execve("/usr/bin/php-cgi", t, env)  == -1)
				perror("ERROR EXECVE");
			exit(0);
		}
		else
			waitpid(pid, &status, 0);
	}





	else if (ret == INDEX)
		_current_rep.build_body_response(std::make_pair(FILE, path.append("index.html")));
	else if (!dir)
	{
		std::string tmp_path = path.substr(0, path.find("?"));
		std::string new_path = tmp_path;
		if (!Utils::is_file(new_path))
		{
			_current_rep.set_code_etat(404, "Not Found");
			new_path = _current_map_error[404];
		}
		if (!Utils::permission_read(new_path))
		{
			_current_rep.set_code_etat(301, "Forbidden");
			new_path = _current_map_error[403];
		}
		_current_rep.build_body_response(std::make_pair(FILE, new_path));
	}
	else if (ret == AUTOINDEX)
	{
		files = get_file_in_dir(dir);
		_current_rep.set_content_location(path);
		if (*(path.rbegin()) != '/')
		{
			_current_rep.set_code_etat(301, "Moved Permanently");
			std::size_t found = path.find_last_of("/");
			_current_rep.set_content_location(path.substr(found + 1).append("/"));
		}
		else
			_current_rep.set_code_etat(200, "OK");
		_current_rep.build_response_dir(files);
	}
	else
	{
		_current_rep.set_code_etat(404, "Not Found");
		_current_rep.build_body_response(std::make_pair(FILE, _current_map_error[404]));
	}
}


/*
** Cette fonction fait plusieurs choses importantes :
**		1.	On initialise une route et on recupere le bon block location (par default cest location /) du fichier de config
**		2.	On regarde si la methode existe (fonction : gestion_valid_method())
**				-> OUI : On passe au 3.
**				-> NON : On build le body de la reponse avec le fichier 405.html et on sort de la fonction (Method Not Allowed)
**		3.	On appelle la fonction adequat en fonction de la methode (GET, POST ou DELETE)
*/
void					Server::gestion_req(char **env)
{
	std::string			&method = _current_req.get_method();
	get_req_route();
	fill_current_rep();
	if (!gestion_valid_method())
	{
		_current_rep.set_code_etat(405, "Method Not Allowed");
		_current_rep.build_body_response(std::make_pair(FILE, _current_map_error[405]));
	}
	else if (method == "GET")
		get_resource(env);
	else if (method == "POST")
		post_resource();
	else if (method == "PUT")
		put_resource();
	else
		delete_resource();
	_current_req.clear();
}


/*
** RECV ET SEND
*/

/*
** Cette fonction va permettre 2 choses :
**		1.	Build la reponse correspond (head + body) en fonction de la requete envoyée par le navigateur (donc le client)
**		2.	Envoyer cette dernière au navigateur (donc au client) pour qu'il l'affiche
*/
int						Server::s_send(int i, char **env)
{
	std::string	reponse;
	int			ret = 0;

	gestion_req(env);
	reponse = _current_rep.fill_reponse();
	std::cout << reponse.substr(0, 2000) << std::endl;
	if ((ret = send(_pfds[i].fd, reponse.c_str(), reponse.size(), 0)) < 0)
		perror("send");
	return (0);
}

void					Server::get_index()
{
	size_t		i = 0;

	while (i < _list_server.size())
	{
		std::stringstream	tmp_host;
		std::string			tmp_host2("127.0.0.1");

		tmp_host2.append(_current_req.get_host().substr(_current_req.get_host().find(":"), std::string::npos));
		tmp_host << _list_server[i].get_host() << ":" << _list_server[i].get_listen();
		if ((_current_req.get_host() == tmp_host.str()) || (tmp_host2 == tmp_host.str()))
		{
			_index = i;
			break ;
		}
		i++;
	}
}

// int					Server::s_recv(int &fd, int i, int *nfds)
// {
// 	std::string			request;
// 	int					ret = 0;
// 	char				requete[131072] = { 0 };
// 	int					ret_read;
// 	(void)nfds;
// 	(void)i;

// 	ret_read = recv(fd, requete, RECV_SIZE, 0);
// 	if (ret_read == -1)
// 		return (-1);
// 	if (ret_read == 0)
// 		return (0);
// 	request = requete;
// 	if (!request.empty())
// 	{
// 		_current_req.set_request(request);
// 		_current_req.parse_request();
// 		try {
// 			_current_req.is_valid();
// 		}
// 		catch(const std::string& e) {
// 			throw ;
// 		}
// 		get_index();
// 		_current_map_error = _list_server[_index].get_map_error();
// 		_current_req.set_path(_list_server[_index].get_root() + _current_req.get_path());
// 		std::cout << _current_req << std::endl;
// 		if (s_send(i, nfds) == -1)
// 			return (-1);
// 	}
// 	return (ret_read);
// }

int					Server::s_recv(int &fd, int i, int *nfds, char **env)
{
	std::string			request;
	int					ret = 0;
	char				requete[RECV_SIZE] = { 0 };
	int					ret_read;
	(void)nfds;
	(void)i;

	ret_read = recv(fd, requete, RECV_SIZE, 0);
	if (ret_read == -1)
		return (-1);
	if (ret_read == 0)
		return (0);
	_requests[fd] += requete;
	if (_requests[fd].find("\r\n\r\n") != std::string::npos)
	{
		if (_requests[fd].find("Transfer-Encoding: chunked") != std::string::npos)
		{
			std::cout << "ICI" << std::endl;
			ret = Utils::last_line_chunked(_requests[fd]);
		}
		if (ret == 0)
		{
			// char hello[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\n\r\n26\r\nVoici les donnees du premier morceau\r\n\r\n1C\r\net voici un second morceau\r\n\r\n20\r\net voici deux derniers morceaux \r\n12\r\nsans saut de ligne\r\n0\r\n\r\n";
			// send(_pfds[i].fd, hello, 1024, 0);
			_current_req.set_request(_requests[fd]);
			_current_req.parse_request();
			try {
				_current_req.is_valid();
			}
			catch(const std::string& e) {
				throw ;
			}
			get_index();
			_current_map_error = _list_server[_index].get_map_error();
			_current_req.set_path(_list_server[_index].get_root() + _current_req.get_path());
			std::cout << _current_req << std::endl;
			s_send(i, env);
			_requests[fd].clear();
			_requests.erase(fd);
		}
	}
	return (ret);
}

int						Server::get_pos_socket(void)
{
	int		i = 0;

	while (i < SIZE_PFDS)
	{
		if (_pfds[i].fd == -1)
			return (i);
		i++;
	}
	return (0);
}

void					Server::clear(void)
{
	int		i = -1;
	int		size = _list_server.size();
	
	_current_req.clear();
	_current_rep.clear();
	_current_route.clear();
	_current_map_error.clear();
	while (++i < size)
		_list_server[i].clear();
	_vect_address.clear();
	_vect_listen_fd.clear();
	_mime.clear();
	i = -1;
	while (++i < SIZE_PFDS)
		if (Utils::fd_is_valid(_pfds[i].fd))
			close(_pfds[i].fd);
}

void					Server::run(char **env)
{
	int				nfds;
	int				i;
	int				j;
	int				nbr;

	_index = 0;
	init_pfds();
	nfds = _vect_listen_fd.size();
	while (true)
	{
		i = 0;
		nbr = init_poll(&nfds);
		while (i < nfds)
		{
			if (_pfds[i].revents == POLLIN)
			{
				j = 0;
				nbr--;
				int size = _vect_listen_fd.size();
				int test = nfds;
				while (j < nfds)
				{
					if (j < size && _pfds[i].fd == _vect_listen_fd[j])
					{
						int index = get_pos_socket();
						if (index == 0)
							index = nfds;
						_pfds[index].fd = s_accept(j);
						_pfds[index].events = POLLIN;
						_pfds[index].revents = 0;
						if (index >= nfds)
							nfds++;
						break ;
					}
					j++;
				}
				if (j == test)
				{
					int ret;
					try {
						ret = s_recv(_pfds[i].fd, i, &nfds, env);
					}
					catch(const std::exception& e) {
						throw ;
					}
					if (ret < 0)
					{
						struct pollfd new_p;
						new_p.fd = -1;
						new_p.events = POLLIN;
						_pfds[i] = new_p;
					}
					else if (ret == 0)
					{
						close(_pfds[i].fd);
						struct pollfd new_p;
						new_p.fd = -1;
						new_p.events = POLLIN;
						_pfds[i] = new_p;
					}
				}
			}
			if (nbr == 0)
				break ;
			i++;
		}
	}
}

void					Server::init_mime(void)
{
	_mime[".acc"] = "audio/aac";
	_mime[".abw"] = "application/x-abiword";
	_mime[".arc"] = "application/octet-stream";
	_mime[".avi"] = "video/x-msvideo";
	_mime[".azw"] = "application/vnd.amazon.ebook";
	_mime[".bin"] = "application/octet-stream";
	_mime[".bmp"] = "image/bmp";
	_mime[".bz"] = "application/x-bzip";
	_mime[".bz2"] = "application/x-bzip2";
	_mime[".csh"] = "application/x-csh";
	_mime[".css"] = "text/css";
	_mime[".csv"] = "text/csv";
	_mime[".doc"] = "application/msword";
	_mime[".eot"] = "application/vnd.ms-fontobject";
	_mime[".epub"] = "application/epub+zip";
	_mime[".gif"] = "image/gif";
	_mime[".htm"] = "text/html";
	_mime[".html"] = "text/html";
	_mime[".ico"] = "image/x-icon";
	_mime[".ics"] = "text/calendar";
	_mime[".jar"] = "application/java-archive";
	_mime[".jpeg"] = "image/jpeg";
	_mime[".jpg"] = "image/jpeg";
	_mime[".js"] = "application/javascript";
	_mime[".json"] = "application/json";
	_mime[".mid"] = "audio/midi";
	_mime[".midi"] = "audio/midi";
	_mime[".mpeg"] = "video/mpeg";
	_mime[".mpkg"] = "application/vnd.apple.installer+xml";
	_mime[".odp"] = "application/vnd.oasis.opendocument.presentation";
	_mime[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	_mime[".odt"] = "application/vnd.oasis.opendocument.text";
	_mime[".oga"] = "audio/ogg";
	_mime[".ogv"] = "video/ogg";
	_mime[".ogx"] = "application/ogg";
	_mime[".otf"] = "font/otf";
	_mime[".png"] = "image/png";
	_mime[".pdf"] = "application/pdf";
	_mime[".ppt"] = "application/vnd.ms-powerpoint";
	_mime[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	_mime[".rar"] = "application/x-rar-compressed";
	_mime[".rtf"] = "application/rtf";
	_mime[".sh"] = "application/x-sh";
	_mime[".svg"] = "image/svg+xml";
	_mime[".swf"] = "application/x-shockwave-flash";
	_mime[".tar"] = "application/x-tar";
	_mime[".ts"] = "application/typescript";
	_mime[".wav"] = "audio/x-wav";
	_mime[".weba"] = "audio/webm";
	_mime[".webm"] = "video/webm";
	_mime[".webp"] = "image/webp";
	_mime[".xhtml"] = "application/xhtml+xml";
	_mime[".xml"] = "application/xml";
	_mime[".zip"] = "application/zip";
	_mime[".7z"] = "application/x-7z-compressed";
}


/*
** GETTEURS ET SETTEURS
*/

int						&Server::get_server_fd(void)
{
	return (this->_server_fd);
}

std::vector<ServerConf>	&Server::get_list_server(void)
{
	return (this->_list_server);
}

void					Server::set_server_fd(int server_fd)
{
	this->_server_fd = server_fd;
}

void					Server::set_list_server(std::vector<ServerConf> &list_server)
{
	this->_list_server = list_server;
}

void					Server::set_map_error(std::map<int, std::string> &_map_error)
{
	this->_current_map_error = _map_error;
}