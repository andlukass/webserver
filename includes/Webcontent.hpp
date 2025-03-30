/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webcontent.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngtina1999 <ngtina1999@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:52:05 by ngtina1999        #+#    #+#             */
/*   Updated: 2025/03/31 01:03:40 by ngtina1999       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include <fstream>  // Ensure this is included for ifstream (to open file)

class Webcontent {
	
	private:
		Webcontent();
		Webcontent(const Webcontent& copy);
		Webcontent& operator=(const Webcontent& rhs);
		~Webcontent();

   public:
   		void	Webcontent::contentManager(int clientFd,std::string root);
		
};