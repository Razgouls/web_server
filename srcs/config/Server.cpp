/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 12:24:59 by elie              #+#    #+#             */
/*   Updated: 2021/11/11 19:13:16 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include <errno.h>


# define RECV_SIZE 131072

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
	this->_list_server = s._list_server;
	this->_address = s._address;
	this->_server_fd = s._server_fd;
}


/*
** Opérateur d'assignation
*/
Server					&Server::operator=(const Server &s)
{
	this->_list_server = s._list_server;
	this->_address = s._address;
	this->_server_fd = s._server_fd;
	return (*this);
}


/*
** Destructeur
*/
Server::~Server()
{
	
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
			listen_fd = -1;
		setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		memset(&add, 0, sizeof(add));
		add.sin_family = AF_INET;
		add.sin_port = htons(_list_server[i].get_listen());
		add.sin_addr.s_addr = inet_addr(_list_server[0].get_host().c_str());
		fcntl(listen_fd, F_SETFL, O_NONBLOCK);
		if (bind(listen_fd, (struct sockaddr *)&add, sizeof(add)) < 0)
			listen_fd = -1;
		if (listen(listen_fd, 10) < 0)
			listen_fd = -1;
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
	int		len = sizeof(_address);
	int		new_fd;

	if ((new_fd = accept(_vect_listen_fd[j], (struct sockaddr *)&_vect_address[j], (socklen_t *)&len)) < 0)
		perror("1accept");
	return (new_fd);
}


/*
** Cette fonction permet 2 choses :
**		1.	Initialiser la socket _listen_fd (socket qui va ecouter les connexion entrantes)
**		2.	Initialiser la struct pollfd :
**			-> On malloc (200 correspond a la taille du tableau de socket -> socket a surveiller)
**			-> On initialise la premiere case :
**					- .fd : correspond au socket
**					- .events : aux evenements (POLLIN : Données en attente de lecture)
*/
void					Server::init_pfds(void)
{
	int		size;
	int		i = 0;

	init_listen_fd();
	bzero(_pfds, 200);
	size = _vect_listen_fd.size();
	while (i < size)
	{
		if (_vect_listen_fd[i] >= 0)
		{
			_pfds[i].fd = _vect_listen_fd[i];
			_pfds[i].events = POLLIN;
		}
		i++;
	}
}


/*
** Cette fonction va appeler la fonction poll qui permet de verifier s'il y a des données en attente de lecture (POLLIN) -> Si oui la fonction poll(...) nous avertira
**		1.	_pfds : correspond au tableau de sockets
**		2.	*nfds : correspond au nombre de socket present dans _pfds (donc a ca size)
**		3.	-1 : delai d'attente en millisecondes (-1 correspond a un delai d'attente infini)
**		4.	nbr_count : la fonction poll(...) renvoie le nombre d'éléments du tableau ayant eu un évènement
*/
void					Server::init_poll(int *nfds)
{
	int		nbr_count = poll(_pfds, *nfds, -1);

	if (nbr_count < 0)
		throw std::string("Erreur lors du pull");
	if (nbr_count == 0)
		throw std::string("Timeout lors du pull");
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
std::pair<bool, bool>	Server::gestion_auto_index(void)
{
	std::ifstream		myfile;
	std::string			index;

	index = _current_req.get_path().append("index.html");
	myfile.open(index.c_str());
	if (myfile.good() && !_current_route.get_index().empty())
		return (std::make_pair(true, true));
	else if (_current_route.get_auto_index() == "on")
		return (std::make_pair(false, true));
	return (std::make_pair(false, false));
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
	std::string			mess;

	myfile.open(_current_req.get_path().c_str());
	if (!myfile.good())
		_current_rep.build_body_response(_list_server[_index].get_map_error()[404], 404, _current_req);
	else
	{
		remove(_current_req.get_path().c_str());
		mess.append("File ").append(_current_req.get_path()).append(" is deleted");
		_current_rep.build_response_string(mess);
	}
}

/*
** Cette fonction est appelee si la methode utilisée eatit une methode POST (notammenent pour le formulaire)
*/
void					Server::post_resource(void)
{
	std::string tmp_path = _current_req.get_path();
	_current_rep.build_response_file(_list_server[_index].get_map_error(), tmp_path, _current_req);
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
void					Server::get_resource(void)
{
	std::list<std::pair<std::string, unsigned char> >	files;
	std::pair<bool, bool>								auto_index;
	DIR													*dir;
	std::string											path = _current_req.get_path();

	dir = opendir(path.c_str());
	auto_index = gestion_auto_index();
	fill_current_rep();
	if (auto_index.first)
		_current_rep.build_body_response(path.append("index.html"), 200, _current_req);
	else if (!dir)
	{
		_current_rep.build_response_file(_list_server[_index].get_map_error(), path, _current_req);
	}
	else if (auto_index.second)
	{
		files = get_file_in_dir(dir);
		_current_rep.set_content_location(path);
		if (*(path.rbegin()) != '/')
		{
			_current_rep.set_code_etat(301);
			std::size_t found = path.find_last_of("/");
			_current_rep.set_content_location(path.substr(found + 1).append("/"));
		}
		else
			_current_rep.set_code_etat(200);
		_current_rep.build_response_dir(files);
	}
	else
		_current_rep.build_body_response(_list_server[_index].get_map_error()[404], 404, _current_req);
}


/*
** Cette fonction fait plusieurs choses importantes :
**		1.	On initialise une route et on recupere le bon block location (par default cest location /) du fichier de config
**		2.	On regarde si la methode existe (fonction : gestion_valid_method())
**				-> OUI : On passe au 3.
**				-> NON : On build le body de la reponse avec le fichier 405.html et on sort de la fonction (Method Not Allowed)
**		3.	On appelle la fonction adequat en fonction de la methode (GET, POST ou DELETE)
*/
void					Server::gestion_file_dir()
{

	get_req_route();
	if (!gestion_valid_method())
	{
		_current_rep.build_body_response(_list_server[_index].get_map_error()[405], 405, _current_req);
	}
	else if (_current_req.get_method() == "GET")
	{
		get_resource();
	}
	else if (_current_req.get_method() == "POST")
	{
		post_resource();
	}
	else
	{
		delete_resource();
	}
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
int						Server::s_send(int i, int *nfds)
{
	(void)nfds;
	std::string	reponse;

	gestion_file_dir();
	reponse = _current_rep.get_reponse();
	std::cout << reponse << std::endl;
	if (send(_pfds[i].fd, reponse.c_str(), reponse.size(), 0) < 0)
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

int					Server::s_recv(int fd, int i, int *nfds)
{
	std::string			request;
	char				requete[131072] = { 0 };
	int					ret_read;

	ret_read = recv(fd, requete, RECV_SIZE - 1, 0);
	if (ret_read < 0)
	{
		close(_pfds[i].fd);
		_pfds[i].fd = -1;
		// compress_array(nfds);
		return (-1);
	}
	request = requete;
	if (ret_read == 0)
		return (-1);
	if (!request.empty())
	{
		// (void)nfds;
		// char	hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 32\n\nHello et Bonjour tout le monde !" ;
		// send(_pfds[i].fd, hello, 100, 0);
		_current_req.set_request(request);
		_current_req.parse_request();
		get_index();
		_current_req.set_path(_list_server[_index].get_root() + _current_req.get_path());
		std::cout << _current_req << std::endl;
		if (s_send(i, nfds) == -1)
			return (-1);
	}
	return (0);
}

void					Server::compress_array(int *nfds)
{
	int		i;
	int		j;

	for (i = 0; i < *nfds; i++)
	{
		if (_pfds[i].fd == -1)
		{
			for(j = i; j < *nfds; j++)
			{
				_pfds[j].fd = _pfds[j + 1].fd;
			}
			i--;
			(*nfds)--;
		}
	}
}

void					Server::run(void)
{
	int				nfds;
	int				i;
	int				j;

	_index = 0;
	init_pfds();
	nfds = _vect_listen_fd.size();
	while (true)
	{
		i = 0;
		init_poll(&nfds);
		while (i < nfds)
		{
			if (_pfds[i].revents == POLLIN)
			{
				j = 0;
				int size = _vect_listen_fd.size();
				while (j < nfds)
				{
					if (j < size && _pfds[i].fd == _vect_listen_fd[j])
					{
						_pfds[nfds].fd = s_accept(j);
						_pfds[nfds].events = POLLIN;
						nfds++;
						break ;
					}
					j++;
				}
				if (j == nfds)
				{
					s_recv(_pfds[i].fd, i, &nfds);
					close(_pfds[i].fd);
					_pfds[i].fd = -1;
					// compress_array(&nfds);
				}
			}
			i++;
		}
	}
	
}




/*
** GETTEURS ET SETTEURS
*/

int						Server::get_server_fd(void) const
{
	return (this->_server_fd);
}

std::vector<ServerConf>	Server::get_list_server(void) const
{
	return (this->_list_server);
}

struct sockaddr_in		Server::get_address(void) const
{
	return (this->_address);
}

void					Server::set_server_fd(int server_fd)
{
	this->_server_fd = server_fd;
}

void					Server::set_list_server(std::vector<ServerConf> &list_server)
{
	this->_list_server = list_server;
}

void					Server::set_address(struct sockaddr_in &address)
{
	this->_address = address;
}