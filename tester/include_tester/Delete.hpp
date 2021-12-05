/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 12:08:44 by elie              #+#    #+#             */
/*   Updated: 2021/12/05 11:16:06 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_HPP
# define DELETE_HPP

# include "Utils_tester.hpp"
# include "Requete_tester.hpp"

class Delete : public Requete_tester
{
	public:
		Delete(void);
		virtual ~Delete();

		void			delete_file_exist(std::string &request);
		void			delete_file_not_exist(std::string &request);
		void			delete_file_no_perm(std::string &request);
		void			delete_dir(std::string &request);

	private:
};

#endif