#pragma once
#ifndef INSTALLER_LOGGER_H
#define INSTALLER_LOGGER_H
#pragma warning(disable : 4996)
#include <string>
#include <stdexcept>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <stdarg.h>

class FileLogger
{
public:
	static void errorLog(const std::string message, int code);
	static void infoLog(const std::string message, int code);
	static void debugLog(const std::string message);

private:
	void InitializeInstanceFields();

public:
	FileLogger()
	{
		InitializeInstanceFields();
	}
};
#endif