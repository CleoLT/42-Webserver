/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:35:03 by esellier          #+#    #+#             */
/*   Updated: 2025/08/29 11:28:30 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONF_HPP
#define LOCATIONCONF_HPP

#include "Utils.hpp"
#include "ABlockBase.hpp"

class ServerData;

class LocationConf : public ABlockBase
{
	public:
		LocationConf();
		LocationConf(ServerData const& S);
		~LocationConf();
		LocationConf(LocationConf const& other);
		LocationConf&								operator=(LocationConf const& other);

		virtual std::string const&					getRoot() const;
		bool										getAutoindex() const;
		std::string	const&							getKey() const;
		void										setKey(std::string const value);
		size_t										fillAutoIndex(std::vector<std::string>& buffer, size_t i);
	
	private:
		std::string									_key;
		bool    									_autoindex;
};

#endif
