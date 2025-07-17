/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:30:43 by esellier          #+#    #+#             */
/*   Updated: 2025/07/16 13:59:33 by cle-tron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONF_HPP
#define SERVERCONF_HPP

#include "Utils.hpp"
#include "LocationConf.hpp"
#include "ABlockBase.hpp"

class LocationConf;

class ServerConf : public ABlockBase
{
	public:
		ServerConf();
		~ServerConf();
		ServerConf(ServerConf const& other);
		ServerConf&	operator=(ServerConf const& other);

		bool											getAutoindex() const;
		std::map<std::string, LocationConf>&			getLocations();
		std::map<std::string, LocationConf>:: iterator	getItLocations(std::string const& key);
		std::vector< std::pair<int, std::string> >		getListens();
		std::vector<std::string>						getServerName() const;
		size_t											fillListens(std::vector<std::string>& buffer, size_t i);
		size_t											fillServerName(std::vector<std::string>& buffer, size_t i);

	private:
		std::vector< std::pair<int, std::string> >		_listens;
		std::vector<std::string>						_serverName;
		std::map<std::string, LocationConf>				_locations;
};

#endif
