/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnexionServer.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:52:40 by elie              #+#    #+#             */
/*   Updated: 2021/12/01 21:44:04 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnexionServer.hpp"

ConnexionServer::ConnexionServer()
{
	int		i = -1;

	while (++i < SIZE_PFDS)
		_pfds[i].fd = -1;
}

ConnexionServer::~ConnexionServer()
{
	int		i = -1;

	while (++i < SIZE_PFDS)
		if (UtilsFile::fd_is_valid(_pfds[i].fd))
			close(_pfds[i].fd);
}

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
void					ConnexionServer::init_listen_fd(void)
{
	int		i = 0;
	int		size = _servers.size();

	while (i < size)
	{
		int		on = 1;
		struct sockaddr_in add;

		int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (listen_fd < 0)
			throw std::string("Erreur bind");
		setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		memset(&add, 0, sizeof(add));
		add.sin_family = AF_INET;
		add.sin_port = htons(_servers[i].get_port());
		add.sin_addr.s_addr = inet_addr(_servers[0].get_host().c_str());
		fcntl(listen_fd, F_SETFL, O_NONBLOCK);
		if (bind(listen_fd, (struct sockaddr *)&add, sizeof(add)) < 0)
		{
			close(listen_fd);
			throw std::string("Erreur bind");
		}
		if (listen(listen_fd, MAX_CONN) < 0)
		{
			close(listen_fd);
			throw std::string("Erreur bind");
		}
		_vect_address.push_back(add);
		_vect_listen_fd.push_back(listen_fd);
		i++;
	}
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
void					ConnexionServer::init_pfds(void)
{
	int		size;
	int		i = 0;

	try {
		init_listen_fd();
	}
	catch(const std::string &error) {
		throw ;
	}
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
int					ConnexionServer::init_poll(int *nfds)
{
	int		nbr_count = poll(_pfds, *nfds, -1);

	if (nbr_count < 0)
		throw std::string("");
	if (nbr_count == 0)
		throw std::string("Timeout lors du pull");
	return (nbr_count);
}

int						ConnexionServer::get_pos_socket(void)
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

/*
** La fonction accept permet la connexion en acceptant un appel (_address stock l'adresse de l'appelant)
** Elle va retourner une nouvelle socket qui a son va etre non bloquant grace a la fonction fcntl
*/
int						ConnexionServer::s_accept(int j)
{
	int		len = sizeof(sockaddr_in);
	int		new_fd;

	if ((new_fd = accept(_vect_listen_fd[j], (struct sockaddr *)&_vect_address[j], (socklen_t *)&len)) < 0)
		throw std::string("Error accept");
	fcntl(new_fd, F_SETFL, O_NONBLOCK);
	_map_fd_server[new_fd] = _servers[j];
	return (new_fd);
}

int					ConnexionServer::manage_connexion(int &fd)
{
	char				requete[RECV_SIZE] = { 0 };
	int					ret_read;
	std::string			reponse;

	ret_read = recv(fd, requete, RECV_SIZE - 1, 0);
	if (ret_read == -1)
		return (-1);
	if (ret_read == 0)
		return (0);
	_requests[fd] += requete;
	_map_fd_server[fd].c_recv(_requests[fd]);
	reponse = _map_fd_server[fd].get_reponse().fill_reponse();
	send(fd, reponse.c_str(), reponse.size(), 0);

	std::cout << std::endl;
	std::cout << BOLDRED << "=================================================================" << std::endl;
	std::cout << BOLDRED << "======================== INFOS RESPONSE =========================" << std::endl;
	std::cout << BOLDRED << "=================================================================" << WHITE << std::endl;
	std::cout << reponse << std::endl;
	return (0);
}

void signal_callback_handler(int signum)
{
	std::cout << "VOUS AVEZ QUITTE LE PROGRAMME AVEC LE SIGNAL " << signum << std::endl;
}

void					ConnexionServer::run(void)
{
	int				nfds;
	int				i;
	int				j;
	int				nbr;

	signal(SIGINT, signal_callback_handler);
	_index = 0;
	try {
		init_pfds();
	}
	catch(const std::string &error) {
		throw ;
	}
	
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
						try {
							_pfds[index].fd = s_accept(j);
						}
						catch(const std::string &error) {
							throw ;
						}
						
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
					int ret = manage_connexion(_pfds[i].fd);
					if (ret < 0)
					{
						struct pollfd new_p;
						new_p.fd = -1;
						new_p.events = POLLIN;
						_pfds[i] = new_p;
					}
					else if (ret == 0)
					{
						_map_fd_server.erase(_pfds[i].fd);
						_requests.erase(_pfds[i].fd);
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

void							ConnexionServer::set_servers(std::vector<Server> &servers)
{
	this->_servers = servers;
}
