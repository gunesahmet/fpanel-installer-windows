#pragma once
#ifndef INSTALLER_PACKAGES_H
#define INSTALLER_PACKAGES_H
#include "Utils.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <tchar.h>
#include <strsafe.h>
#include <stdio.h>
#include "FileMd5.h"
#include "Logger.h"
#include <map>


class Packages
{
public:
	Utils *utils;
	FileLogger *logger;

	void initPackages(int server_version, std::string repo_server, std::string, std::string sql_root_password, std::string sql_admin_password, bool server_core);
	void setFiles(std::map<std::string, std::string> files_md5);
	void vcRedist();
	void dotNet();
	void dotNetCoreWebHosting();
	void panelHttpd();
	void panelCore();
	void panelHtdocs();
	void panelPhp();
	void openSsl();
	void redis();

	void mysql(int sql_ver);
	void firstInitMySQL(int sql_ver);
	void firstInitMySQLForCloud(int sql_ver, std::string main_ip);
	void postgreSql(int sql_ver, std::string main_ip, bool cloud);
	void microsoftSql(int sql_ver, std::string main_ip, bool cloud);

	void iis();

	void bind();
	void bindService();
	void startBindService();
	void msdns();

	void perl();
	void python(int version);

	void php56();
	void php72();
	void php74();

	void startPanelServices(bool cloud);
	void initSystem(bool mysql_install, bool cloud, std::string main_ip);
	void setSystemPackages(std::string ftp, std::string dns, std::string smtp, std::string databases, std::string statistics, std::string spam);

	void setTcpPort(std::string path);

	void mailEnable();
	void hMail();
	void spamAssassin();
	void spamAssassinService();
	void spamAssassinRegistry();
	void startSpamassassinService();

	void roundCube();
	void phpMyAdmin(int version);
	void myLittleAdmin();
	void awstats();
	void webalizer();
	void setDebug(bool _debug);
	void setCloudClient(bool _cloud_client);
	void setUtils(Utils *_utils);
	void setLogger(FileLogger *_logger);

private:
	MD5 md5;
	bool debug;
	bool cloud_client;
	// VC Redists
	std::string vcredistsx86_2008 = "vcredist_x86_2008.exe";
	std::string vcredistsx64_2008 = "vcredist_x86_2008.exe";
	std::string vcredistsx86_2012 = "vcredist_x86_2012.exe";
	std::string vcredistsx64_2012 = "vcredist_x64_2012.exe";
	std::string vcredistsx86_2013 = "vcredist_x86_2013.exe";
	std::string vcredistsx64_2013 = "vcredist_x64_2013.exe";
	std::string vcredistsx86_2015_19 = "vcredist_x86_2015-2022.exe";
	std::string vcredistsx64_2015_19 = "vcredist_x64_2015-2022.exe";

	// FPANEL CORES
	std::string panel_httpd_msi = "fpanel-httpd.msi";
	std::string htdocs_msi = "fpanel-htdocs.msi";
	std::string core_msi = "fpanel-core.msi";
	std::string redis_msi = "fpanel-redis.msi";
	std::string panel_php_msi = "fpanel-php.msi";

	// OpenSSL
	std::string openssl_msi = "openssl.exe";

	// MySQL
	std::string mysql57_msi = "mysql57.msi";
	std::string mysql80_msi = "mysql80.msi";

	// Scripts
	std::string dbmyadmin_msi = "phpmyadmin.msi";
	std::string roundcube_msi = "webmail_rc.msi";
	std::string mylittleadmin_msi = "mylittleadmin.msi";
	std::string awstats_msi = "awstats.msi";
	std::string webalizer_msi = "webalizer.msi";

	// Mail
	std::string mailenable_msi = "mailenable.exe";
	std::string mailenable_pro_msi = "mailenable_pro.exe";
	std::string mailenable_ent_msi = "mailenable_ent.exe";
	std::string mailenable_pre_msi = "mailenable_pre.exe";
	std::string hmail_msi = "hmail.exe";
	std::string spamassassin_msi = "spamassassin.msi";

	// IIS
	std::string rewrite_msi = "rewrite.msi";

	// DNS
	std::string bind_msi = "bind.msi";

	// SQL Servers
	std::string sql_2012_exp_msi = "sql2012Exp_x64.exe";
	std::string sql_2016SP2_exp_msi = "sql2016ExpSP2_x64.exe";
	std::string sql_2019_exp_msi = "sql2019Exp_x64.exe";

	// PostgreSQL
	std::string postgresql11_msi = "postgresql11.exe";
	std::string postgresql12_msi = "postgresql12.exe";
	std::string postgresql13_msi = "postgresql13.exe";

	std::string python27_msi = "python27.msi";
	std::string python39_exe = "python39.exe";
	std::string perl_msi = "perl.msi";

	// PHP
	std::string php56_msi = "php56.msi";
	std::string php72_msi = "php72.msi";
	std::string php74_msi = "php74.msi";
	std::string msodbcsql_msi = "msodbcsql.msi";

	std::string dotnet40_msi = "dotnet40.exe";
	std::string dotnet40_core_msi = "dotnet40_core.exe";
	std::string dotnet45_msi = "dotnet45.exe";
	std::string dotnet48_msi = "dotnet48.exe";
	std::string dotnetcore_msi = "dotnet-hosting-3.1.23-win.exe";

	std::string panel_dir;
	std::string panel_tmp_dir;
	std::string repo_server_link;
	std::string sql_root_password_c;
	std::string sql_admin_password_c;
	std::string systemDrive;
	int server_version_c;
	std::string encrypted_sql_admin_password;
	std::map<std::string, std::string> files;

	void InitializeInstanceFields();

	void initMySQL(bool cloud, std::string main_ip)
	{
		std::string command_b = panel_dir + "\\mysql\\bin\\mysql.exe -u root -p" + sql_root_password_c + " -e \"CREATE USER 'fpaneladmin'@'localhost' IDENTIFIED BY '" + sql_admin_password_c + "'; GRANT ALL PRIVILEGES ON *.* TO 'fpaneladmin'@'localhost' WITH GRANT OPTION; FLUSH PRIVILEGES;\" mysql";
		utils->executeForWindows(command_b, true);
		if (cloud) {
			std::string command_c = panel_dir + "\\mysql\\bin\\mysql.exe -u root -p" + sql_root_password_c + " -e \"CREATE USER 'fpaneladmin'@'" + main_ip + "' IDENTIFIED BY '" + sql_admin_password_c + "'; GRANT ALL PRIVILEGES ON *.* TO 'fpaneladmin'@'" + main_ip + "' WITH GRANT OPTION; FLUSH PRIVILEGES;\" mysql";
			utils->executeForWindows(command_c, true);
		}
		
		Sleep(1500);
		utils->removeFile(panel_dir + "\\mysql\\init.txt");
	};

	void initRedis()
	{
		utils->appendFile(panel_dir + "\\redis\\redis.conf", "requirepass " + sql_admin_password_c);
	};

	void initSystemEnvironments()
	{
		//utils->setPermanentEnvironmentVariable("Path", ";"+ panel_dir + "\\core\\bin");
		//utils->setPermanentEnvironmentVariable("Path", ";" + panel_dir + "\\core\\sbin");

		HKEY hKey;
		LONG lRes = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_READ, &hKey);
		std::wstring strKeyDefaultValue;
		utils->getStringRegKey(hKey, L"OPENSSL_CONF", strKeyDefaultValue, L"NOT_EXISTS");
		if (strKeyDefaultValue == L"NOT_EXISTS")
			utils->setPermanentEnvironmentVariable("OPENSSL_CONF", panel_dir + "/openssl/share/openssl.cnf");
	};

	void initServices()
	{
		utils->executeForWindows("sc.exe create \"FPanel Agent Service\" binPath=\"" + panel_dir + "\\core\\services\\FpanelAgent.exe \\\"" + panel_dir + "\\core\\sbin\\fpanel-agent.exe\\\"\" type=own start=auto error=ignore", true);
		utils->executeForWindows("sc.exe create \"FPanel Core Service\" binPath=\"" + panel_dir + "\\core\\services\\FpanelCore.exe \\\"" + panel_dir + "\\core\\sbin\\fpanel-core.exe -s\\\"\" type=own start=auto error=ignore", true);
		utils->executeForWindows("sc.exe create \"FPanel Redis Service\" binPath=\"" + panel_dir + "\\core\\services\\FpanelRedis.exe \\\"" + panel_dir + "\\redis\\redis-server.exe " + panel_dir + "\\redis\\redis.conf\\\"\" type=own start=auto error=ignore", true);

		// For Httpd Service
		utils->executeForWindows(panel_dir + "\\core\\services\\FPanelHttpd.exe install \"FPanel Httpd Service\" " + panel_dir + "\\httpd\\lighttpd.exe \\\"\\\"\\\"- D\\\"\\\"\\\" \\\"\\\"\\\"- f\\\"\\\"\\\" \\\"\\\"\\\"" + panel_dir + "\\httpd\\conf\\httpd.conf\\\"\\\"\\\"", true);
		utils->executeForWindows(panel_dir + "\\core\\services\\FPanelHttpd.exe set \"FPanel Httpd Service\" Application " + panel_dir + "\\httpd\\lighttpd.exe", true);
		utils->executeForWindows(panel_dir + "\\core\\services\\FPanelHttpd.exe set \"FPanel Httpd Service\" AppParameters \" -D -f " + panel_dir + "\\httpd\\conf\\httpd.conf", true);

	};

	void allowPorts()
	{
		std::string ports[] = { "8821" ,"8822", "8823", "8443", "8880", "8889", "8879" };
		//std::string ports[] = { "8821" ,"8822", "8823", "8443", "8879", "8880", "8889" };

		for (unsigned int i = 0; i < sizeof(ports) / sizeof(ports[0]); i++) {
			utils->executeForWindows("netsh firewall add portopening TCP " + ports[i] + " \"FPanel Service Port " + ports[i] + "\"", true);
		}
	};

	void setMailEnableRegistry(std::string path, std::string value, std::string new_data)
	{
		HKEY hKey;
		LPCTSTR keyPath = TEXT((LPCSTR)path.c_str());
		LSTATUS lOpenStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_ALL_ACCESS, &hKey);
		if (lOpenStatus == ERROR_SUCCESS) {
			LSTATUS lSetStatus = RegSetValueEx(hKey, (LPCSTR)value.c_str(), 0, REG_SZ, (LPBYTE)new_data.c_str(), strlen(new_data.c_str()) + 1);
			RegCloseKey(hKey);

			if (lSetStatus == ERROR_SUCCESS) {
				return;
			}
		}
	}

	void setMailEnableRegistryDword(std::string path, std::string key, DWORD value)
	{
		HKEY hKey;
		LPCTSTR keyPath = TEXT((LPCSTR)path.c_str());
		LSTATUS lOpenStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_ALL_ACCESS, &hKey);
		if (lOpenStatus == ERROR_SUCCESS) {
			LSTATUS lSetStatus = RegSetValueEx(hKey, (LPCSTR)key.c_str(), 0, REG_DWORD, (const BYTE*)&value, sizeof(value));
			RegCloseKey(hKey);

			if (lSetStatus == ERROR_SUCCESS) {
				return;
			}
		}
	}

	void spamAssassinRegister(std::string path, std::string value, std::string new_data)
	{
		HKEY hKey;
		LPCTSTR keyPath = TEXT((LPCSTR)path.c_str());
		LSTATUS lOpenStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_ALL_ACCESS, &hKey);
		if (lOpenStatus == ERROR_SUCCESS) {
			LSTATUS lSetStatus = RegSetValueEx(hKey, (LPCSTR)value.c_str(), 0, REG_SZ, (LPBYTE)new_data.c_str(), strlen(new_data.c_str()) + 1);
			RegCloseKey(hKey);

			if (lSetStatus == ERROR_SUCCESS) {
				return;
			}
		}
	}

	void consoleWriteLine(std::string message)
	{
		if (debug) {
			std::cout << message << std::endl;
		}
	}

	void execute(std::string command, std::string file)
	{
		std::string exec_result = utils->exec(command.c_str());
		if (exec_result == "ERROR")
			logger->errorLog("Execute failed Command: " + command, 1000);
		Sleep(2000);
		utils->removeFile(file);
		Sleep(1000);
	}

	void executeWindows(std::string command, std::string file, bool wait)
	{
		//std::cout << "File downloading" << std::endl;
		//FileLogger::infoLog("Completed download URL: " + url, 1000);
		//FileLogger::infoLog("Executing Command: " + command, 1000);
		Sleep(5000);

		bool exec_result = utils->executeForWindows(command, wait);
		if (!exec_result) {
			logger->errorLog("Execute failed Command: " + command, 1000);
			utils->removeFile(file);
			return;
		}

		Sleep(1000);
		//logger->infoLog("Removing file: " + file, 1000);
		utils->removeFile(file);
		Sleep(2000);
	}

public:
	Packages()
	{
		InitializeInstanceFields();
	}
};
#endif