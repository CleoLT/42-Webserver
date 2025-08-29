/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 16:30:43 by esellier          #+#    #+#             */
/*   Updated: 2025/08/29 10:58:43 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERDATA_HPP
#define SERVERDATA_HPP

#include "ABlockBase.hpp"
#include "LocationConf.hpp"
#include "Utils.hpp"

class LocationConf;

class ServerData : public ABlockBase  // INFO I was getting a bit confused so I
									  // changed to ServerData
{
public:
	ServerData();
	~ServerData();
	ServerData(ServerData const& other);
	ServerData& operator=(ServerData const& other);

	virtual std::string const&						getRoot() const;
	std::map<std::string, LocationConf>&			getLocations() const;
	std::map<std::string, LocationConf>::iterator	getItLocations(
		std::string const& key);
	std::vector<std::pair<int, std::string> >		getListens() const;
	std::vector<std::string>				  		getServerName() const;
	size_t 											fillListens(std::vector<std::string>& buffer, size_t i);
	size_t 											fillServerName(std::vector<std::string>& buffer, size_t i);

private:
	std::vector<std::pair<int, std::string> > _listens;
	std::vector<std::string>				  _serverName;
	std::map<std::string, LocationConf>		  _locations;
};

#endif
