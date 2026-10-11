#pragma once
#ifndef INSTALLER_SCREENS_H
#define INSTALLER_SCREENS_H
#include <windows.h>
#include <iostream>
#include "Headers.h"
#include "Utils.h"
#include "Items.h"
#include <conio.h>
#include <typeinfo>
#include <iostream>
#include <string>
#include <sstream>

class Screens
{
public:
	static Items mainScreen(bool server_version_core, bool no_option, bool fast);
	static void complete(std::string db_root_password);
private:
	void InitializeInstanceFields();

public:
	Screens()
	{
		InitializeInstanceFields();
	}
};
#endif