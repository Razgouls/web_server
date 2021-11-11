void					Server::run(void)
{
	int				listen_fd;
	int				new_fd = -2;
	int				nfds = 1;
	bool			finish = true;
	bool			kill_connec = false;
	char			buffer_client[1024] = { 0 };
	struct pollfd	pfds[200];

	listen_fd = get_socket_fd();
	if (listen_fd < 0)
		throw std::string("Erreur lors de la creation de la socket");
	
	memset(pfds, 0, sizeof(pfds));
	pfds[0].fd = listen_fd;
	pfds[0].events = POLLIN;

	while (finish)
	{
		int		nbr_count = poll(pfds, nfds, 150000);
		int		i = 0;

		if (nbr_count < 0)
			throw std::string("Erreur lors du pull");
		if (nbr_count == 0)
			throw std::string("Timeout lors du pull");

		//On passe en revue les connexions existantes a la recherche de données a lire
		while (i < nfds)
		{
			//On regarde s'il y a des données en attente de lecture
			if (pfds[i].revents == POLLIN)
			{
				//Si oui alors on rentre dans le if

				//On regarde si listen_fd est pret lire, si oui on creer une nouvelle connexion
				//listen_fd est en attente de connexions
				//On regarde ici si pfds[i].fd correspond bien au listen_fd, car seulement le serveur peut accepter des connexions
				//Si ca ne correspond alors ca signifie que cest un client et quil y a quelque chose a lire
				if (pfds[i].fd == listen_fd)
				{
					std::cout << "================== 11111111 ==================" << pfds[i].fd << std::endl;
					std::cout << "================== 11111111bis ================== " << listen_fd << ", " << i << std::endl;
					int len = sizeof(_address);

					//on creer une nouvelle connexion entre le socket server (listen_fd) et l'appelant (_address)
					if ((new_fd = accept(listen_fd, (struct sockaddr *)&_address, (socklen_t *)&len)) < 0)
						perror("accept");

					//Si tout s'est bien passé alors on ajoute cette nouvelle socket dans la structure du poll
					//En principe tant que le socket est dans la structure cest que la connexion est encore ouverte, une fois que la connexion est terminee, supprimée, elle doit y etre supprimee de la structure
					pfds[nfds].fd = new_fd;
					pfds[nfds].events = POLLIN;
					++nfds;
				}
				else
				{
					std::cout << "revent2 : " << pfds[i].revents << std::endl;
					std::cout << "================== 222222222bis ================== " << pfds[i].fd << ", " << i << std::endl;
					std::cout << "================== 222222222 ==================" << std::endl;

					//Si je suis ici cest que cest un client regulier donc son new_fd est deja present dans la structure du poll,
					// donc la connexion existe deja et peut etre lu

					//fd du client
					std::cout << "Descriptor readable : " << pfds[i].fd << std::endl;

					//on recoit une requete de pfds[i].fd que l'on stocke dans buffer_client
					int ret_read = recv(pfds[i].fd, buffer_client, sizeof(buffer_client), 0);
					// std::cout << inet_ntop(AF_INET, &_address.sin_addr, buffer_client, sizeof(buffer_client)) << std::endl;

					//nombre de caracteres lu dans la requete
					std::cout << "ret read : " << ret_read << std::endl;

					//on affiche la requete
					std::cout << "REQUETE : [ " << buffer_client << std::endl << " ]" << std::endl;

					//si le nombre de caractere est <= 0 alors cest une erreur
					if (ret_read <= 0 && (kill_connec = true))
					{
						perror("accept");
						close(pfds[i].fd);
						pfds[i] = pfds[nfds - 1];
						nfds--;
					}
					else
					{
						char hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 51\n\nHello et Bonjour tout le monde ! https://google.com" ;
						std::cout << "NATA" << std::endl;
						if (send(pfds[i].fd, hello, 1024, 0) < 0)
							perror("send");
					}
				}
				std::cout << "1 fini : " << i << std::endl;
				std::cout << "2 fini : " << nfds << std::endl;
			}
			i++;
		}
	}
	
}
