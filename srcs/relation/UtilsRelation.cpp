/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilsRelation.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elie <elie@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 21:26:33 by elie              #+#    #+#             */
/*   Updated: 2021/11/05 18:26:02 by elie             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.hpp"

int		hex_to_dec(std::string hexVal)
{
	int len = hexVal.size();
	int base = 1;
	int dec_val = 0;
 
	for (int i = len - 1; i >= 0; i--) {
		if (hexVal[i] >= '0' && hexVal[i] <= '9') {
			dec_val += (int(hexVal[i]) - 48) * base;
			base = base * 16;
		}
		else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
			dec_val += (int(hexVal[i]) - 55) * base;
			base = base * 16;
		}
	}
	return (dec_val);
}

std::string			transform_query_char(std::string str)
{
	int		i;
	std::string	new_str;
	std::string	tmp;

	i = 0;
	while (str[i])
	{
		if (str[i] == '+')
			new_str += ' ';
		else if (str[i] == '%')
		{
			tmp += str[i + 1];
			tmp += str[i + 2];
			new_str += (char)hex_to_dec(tmp);
			i += 2;
		}
		else
			new_str += str[i];
		i++;
	}
	return (new_str);
}
