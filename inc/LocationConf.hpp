/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConf.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:35:03 by esellier          #+#    #+#             */
/*   Updated: 2025/08/28 19:57:19 by esellier         ###   ########.fr       */
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

		virtual bool								getAutoindex() const;
		std::string	const&							getKey() const;
		void										setKey(std::string const value);
	
	private:
		std::string									_key;
};

#endif
