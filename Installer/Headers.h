#pragma once
#ifndef INSTALLER_HEADERS_H
#define INSTALLER_HEADERS_H
#include <iostream>

class Headers
{
	public:
	enum enums {
		defaultHeader,
	    httpHeader,
	    databaseHeader,
	    dnsHeader,
	    mailHeader,
	    installHeader,
		continueHeader,
	    completeHeader,
		scriptHeader
	};
	enums hashit (std::string const& inString);
  	void setHeader(std::string headerType, int columns);
};
#endif
