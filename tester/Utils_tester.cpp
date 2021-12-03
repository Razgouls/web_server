/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_tester.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 14:20:46 by elie              #+#    #+#             */
/*   Updated: 2021/12/03 15:23:38 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils_tester.hpp"

std::string								get_file_content(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (file.is_open() && file.good())
	{
		std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
		return (content);
	}
	return (NULL);
}

std::string								get_dir_content(const std::string &path)
{
	DIR				*dir;
	struct dirent	*ent;
	std::string		ret;
	
	if ((dir = opendir(path.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			ret += ent->d_name;
			ret += "\r\n";
		}
		closedir (dir);
	}
	return (ret);
}