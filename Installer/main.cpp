#include <iostream>
#include <boost/program_options.hpp>
#include "Utils.h"
#include "SystemInfo.h"
#include "Screens.h"
#include "Packages.h"
#include <iomanip>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <process.h>
#include <sstream>
#include <wininet.h>
#pragma comment(lib,"Wininet.lib")

enum versions { Win2k16, Win2k16R2, Win2k19, Win2k19R2};

Utils *utils;
Packages *packages;
FileLogger *logger;

std::string repo_server = "repo.fpanel.net";
std::string test_repo_server = "test.repo.fpanel.net";
std::string panel_dir;
std::string panel_tmp_dir;
std::string systemDrive;
static std::string db_root_password;
static std::string sql_admin_password;
static std::string socket_key;
static std::string socket_iv;
static std::string dotnet_path;
std::map<std::string, std::string> files;
std::string main_ip = "127.0.0.1";

bool server_version_core = false;
bool fast = false;
bool test_mode = false;

bool debug = false;
bool cloud_client = false;
bool _exec = false;

int random(int min, int max) {
	return min + (rand() % static_cast<int>(max - min + 1));
}

void init() {
	char *sysDrive = getenv("SystemDrive");
	if (sysDrive == NULL) {
		systemDrive = "c:";
	}
	else {
		systemDrive = std::string(sysDrive);
	}

	db_root_password = utils->createPassword(10);
	//utils->consoleWrite(db_root_password, utils.DARKYELLOW, true);

	socket_key = utils->createPassword(32);
	//utils->consoleWrite(socket_key, utils.DARKYELLOW, true);

	socket_iv = utils->createPassword(32);
	//utils->consoleWrite(socket_iv, utils.DARKYELLOW, true);

	sql_admin_password = utils->createPassword(32);
	//utils->consoleWrite(sql_admin_password, utils.DARKYELLOW, true);

	panel_dir = systemDrive + "\\fpanel";
	panel_tmp_dir = panel_dir + "\\tmp\\";
}

void setcolor(unsigned short color) {
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}

void gotoxy(int x, int y) {
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;
	std::cout.flush();
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

void installMessage(std::string message, int columns) {
	//HANDLE chandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (_exec) {
		utils->consoleWrite(message, utils->DARKTEAL, true);
	}
	else {
		setcolor(utils->DARKTEAL);
		//gotoxy(1, 7);
		printf("%s", (message + std::string(columns - strlen(message.c_str()), ' ')).c_str());
		Sleep(100);
	}
}

boost::program_options::variables_map parseParameters(int argc, char** argv)
{
	boost::program_options::variables_map vm;
	try {
		bool flag = false;
		boost::program_options::options_description desc("Allowed options");
		desc.add_options()
			("help", "Print usage message")
			("verbose", boost::program_options::bool_switch(&flag)->default_value(false), "Verbose mode")
			("test", boost::program_options::bool_switch(&flag)->default_value(false), "Test mode")
			("fast", boost::program_options::bool_switch(&flag)->default_value(false), "Fast Install Mode (Default false)")
			("exec", boost::program_options::bool_switch(&flag)->default_value(false), "Executable from another process")

			("standalone", boost::program_options::bool_switch(&flag), "Standalone mode")
			("cloud", boost::program_options::bool_switch(&flag), "Cloud mode")
			("main-ip", boost::program_options::value<std::string>(), "Cloud Main Server IP")

			("iis", boost::program_options::bool_switch(&flag)->default_value(false), "Select IIS Http Server (Default true)")
			
			("perl", boost::program_options::bool_switch(&flag)->default_value(false), "Install Perl (Default true)")
			("python27", boost::program_options::bool_switch(&flag)->default_value(false), "Install Python 2.7 (Default true)")
			("python39", boost::program_options::bool_switch(&flag)->default_value(false), "Install Python 3.9 (Default true)")
			("php56", boost::program_options::bool_switch(&flag)->default_value(false), "Install PHP 5.6 (Default true)")
			("php72", boost::program_options::bool_switch(&flag)->default_value(false), "Install PHP 7.2 (Default true)")
			("php74", boost::program_options::bool_switch(&flag)->default_value(false), "Install PHP 7.4 (Default true)")

			("mysql57", boost::program_options::bool_switch(&flag)->default_value(false), "Install MySQL 5.7 Database Server (Default true)")
			("mysql80", boost::program_options::bool_switch(&flag)->default_value(false), "Install MySQL 8.0 Database Server (Default false)")

			("mssql12", boost::program_options::bool_switch(&flag)->default_value(false), "Select Microsoft SQL Server 2012 (Default true)")
			("mssql16", boost::program_options::bool_switch(&flag)->default_value(false), "Select Microsoft SQL Server 2016 (Default false)")

			("postgresql11", boost::program_options::bool_switch(&flag)->default_value(false), "Install PostgreSQL 11 Database Server (Default false)")
			("postgresql12", boost::program_options::bool_switch(&flag)->default_value(false), "Install PostgreSQL 12 Database Server (Default false)")
			("postgresql13", boost::program_options::bool_switch(&flag)->default_value(false), "Install PostgreSQL 13 Database Server (Default true)")

			("bind", boost::program_options::bool_switch(&flag)->default_value(false), "Install BIND DNS Server (Default true)")
			("msdns", boost::program_options::bool_switch(&flag)->default_value(false), "Install Microsoft DNS Server (Default false)")

			("mailenable", boost::program_options::bool_switch(&flag)->default_value(false), "Install MailEnable MTA Server (Default true)")
			("hmail", boost::program_options::bool_switch(&flag)->default_value(false), "Install HMail MTA Server (Default false)")
			("spamassassin", boost::program_options::bool_switch(&flag)->default_value(false), "Install MTA Spama Service (Default false)")
			
			("roundcube", boost::program_options::bool_switch(&flag)->default_value(false), "Install Roundcube Webmail (Default true)")
			("phpmyadmin49", boost::program_options::bool_switch(&flag)->default_value(false), "Install phpMyAdmin 4.9 (Default true)")
			("phpmyadmin50", boost::program_options::bool_switch(&flag)->default_value(false), "Install phpMyAdmin 5.0 (Default false)")
			("mylittleadmin", boost::program_options::bool_switch(&flag)->default_value(false), "Install MyLittleAdmin (Default true)")
			
			("awstats", boost::program_options::bool_switch(&flag)->default_value(false), "Install Awstats Log Analyzer (Default true)")
			("webalizer", boost::program_options::bool_switch(&flag)->default_value(false), "Install Webalizer Log Analyzer (Default true)")
			
			("server", boost::program_options::value<std::string>(), "Server Address")
			("hostname", boost::program_options::value<std::string>(), "Hostname")
			;
		//boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).run(), vm);
		//boost::program_options::notify(vm);
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);


		if (vm.count("verbose") && (vm["verbose"].as<bool>())) {
			std::cout << "Debug mode activated" << std::endl;
			debug = true;
		}

		if (vm.count("exec") && (vm["exec"].as<bool>())) {
			utils->consoleWrite("Executed from another process.", utils->RED, true);
			_exec = true;
		}

		if (vm.count("help") && (vm["help"].as<bool>())) {
			std::cout << "FPanel Installer Ver 1.0.0, for Linux (amd64)" << std::endl;
			std::cout << "Copyright (c) 2021 Kovengo Sotware Limited. All rights reserved." << std::endl;
			std::cout << "" << std::endl;
			std::cout << "FPanel is a registered trademark of Kovengo Software Limited. Other names may be trademarks of their respective owners." << std::endl;
			std::cout << "" << std::endl;
			std::cout << "Allowed options:\n"
				"  --help                Print usage message\n"
				"  --fast                Fast Install Mode (Default false)\n"
				"  --standalone          Standalone Install Mode (Default true)\n"
				"  --cloud               Cloud Install Mode (Default false)\n"
				"  --main-ip             Cloud Main Server IP\n"

				"  --iis                 Select IIS Web Server (Default true)\n"
				"  --perl                Install Perl (Default true)\n"
				"  --python27            Install Python (Default true)\n"
				"  --python39            Install Python (Default true)\n"
				"  --php56               Install PHP 5.6 (Default true)\n"
				"  --php72               Install PHP 7.2 (Default true)\n"
				"  --php74               Install PHP 7.4 (Default true)\n"
				"  --mysql57             Install MySQL 5.7 Database Server (Default true)\n"
				"  --mysql80             Install MySQL 8.0 Database Server (Default false)\n"
				"  --mssql12             Install Microsoft SQL Server 2012 (Default true)\n"
				"  --mssql16             Install Microsoft SQL Server 2016 (Default false)\n"
				"  --postgresql11        Install PostgreSQL 11 Database Server (Default false)\n"
				"  --postgresql12        Install PostgreSQL 12 Database Server (Default false)\n"
				"  --postgresql13        Install PostgreSQL 13 Database Server (Default true)\n"
				"  --bind                Install BIND DNS Server (Default true)\n"
				"  --mailenable          Install MailEnable MTA Server (Default true)\n"
				"  --hmail               Install HMail Server (Default false)\n"
				"  --spamassassin        Install Spamassassin Service (Default false)\n"
				"  --roundcube           Install Roundcube Webmail (Default true)\n"
				"  --phpmyadmin49        Install PHP MyAdmin 4.9 (Default true)\n"
				"  --phpmyadmin50        Install PHP MyAdmin 5.0 (Default false)\n"
				"  --mylittleadmin       Install MyLittleAdmin (Default true)\n"
				"  --awstats             Install Awstats Log Analyzer (Default true)\n"
				"  --webalizer           Install Webalizer Log Analyzer (Default true)\n"
				"  --hostname=<value>    Hostname" << std::endl;
			exit(1);
		}

		if (vm.count("fast") && (vm["fast"].as<bool>())) {
			fast = true;
		}

		if (vm.count("test") && (vm["test"].as<bool>())) {
			std::cout << "Test mode activated" << std::endl;
			repo_server = test_repo_server;
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	return vm;
}

void isFast(Items &items, bool fast, boost::program_options::variables_map vm)
{
	if (fast) {
		items.setAwstatsStatus(" ");
		items.setBindStatus(" ");
		items.setHMailStatus(" ");
		items.setHostname(" ");
		items.setIisStatus(" ");
		items.setMailEnableEntStatus(" ");
		items.setMailEnablePreStatus(" ");
		items.setMailEnableProStatus(" ");
		items.setMailEnableStatus(" ");
		items.setMsDNSStatus(" ");
		items.setMssql2k12Status(" ");
		items.setMssql2k16Status(" ");
		items.setMyLittleAdminStatus(" ");
		items.setMySql57Status(" ");
		items.setMySql80Status(" ");
		items.setNetStatus(" ");
		items.setPerlStatus(" ");
		items.setPhp56Status(" ");
		items.setPhp72Status(" ");
		items.setPhp74Status(" ");
		items.setphpMyAdmin49Status(" ");
		items.setphpMyAdmin50Status(" ");
		items.setPhpStatus(" ");
		items.setPostgre11Status(" ");
		items.setPostgre12Status(" ");
		items.setPostgre13Status(" ");
		items.setPython27Status(" ");
		items.setPython39Status(" ");
		items.setRoundCubeStatus(" ");
		items.setSpamAssassinStatus(" ");
		items.setWebalizerStatus(" ");

		if (vm.count("iis") && vm["iis"].as<bool>()) {
			items.setIisStatus("X");
		}


		if (vm.count("perl") && vm["perl"].as<bool>()) {
			items.setPerlStatus("X");
		}


		if (vm.count("python39") && vm["python39"].as<bool>()) {
			items.setPython39Status("X");
		}

		if (vm.count("python27") && vm["python27"].as<bool>()) {
			items.setIisStatus("X");
			items.setPython27Status("X");
		}

		if (vm.count("php56") && vm["php56"].as<bool>()) {
			items.setIisStatus("X");
			items.setPhp56Status("X");
		}

		if (vm.count("php72") && vm["php72"].as<bool>()) {
			items.setIisStatus("X");
			items.setPhp72Status("X");
		}

		if (vm.count("php74") && vm["php74"].as<bool>()) {
			items.setIisStatus("X");
			items.setPhp74Status("X");
		}

		if (vm.count("mysql57") && vm["mysql57"].as<bool>()) {
			items.setMySql57Status("X");
			if (vm.count("phpmyadmin49") && vm["phpmyadmin49"].as<bool>()) {
				items.setPhp72Status("X");
				items.setphpMyAdmin49Status("X");
			}
			if (vm.count("phpmyadmin50") && vm["phpmyadmin50"].as<bool>()) {
				items.setPhp72Status("X");
				items.setphpMyAdmin49Status("X");
			}
		}
		else {
			if (vm.count("mysql80") && vm["mysql80"].as<bool>()) {
				items.setMySql80Status("X");
				if (vm.count("phpmyadmin49") && vm["phpmyadmin49"].as<bool>()) {
					items.setPhp72Status("X");
					items.setphpMyAdmin49Status("X");
				}
				else {
					if (vm.count("mysql80") && vm["mysql80"].as<bool>()) {
						if (vm.count("phpmyadmin50") && vm["phpmyadmin50"].as<bool>()) {
							items.setPhp72Status("X");
							items.setphpMyAdmin50Status("X");
						}
					}
					else {
						items.setPhp72Status("X");
						items.setphpMyAdmin49Status("X");
					}
				}
			}
		}


		if (vm.count("mssql12") && vm["mssql12"].as<bool>()) {
			items.setMssql2k12Status("X");
		}
		else {
			if (vm.count("mssql16") && vm["mssql16"].as<bool>()) {
				items.setMssql2k16Status("X");
			}
			else {
				items.setMssql2k16Status(" ");
			}
		}

		if (vm.count("postgresql13") && vm["postgresql13"].as<bool>()) {
			items.setPostgre13Status("X");
		}
		else {
			if (vm.count("postgresql12") && vm["postgresql12"].as<bool>()) {
				items.setPostgre12Status("X");
			}
			else {
				if (vm.count("postgresql11") && vm["postgresql11"].as<bool>()) {
					items.setPostgre11Status("X");
				}
			}
		}

		if (vm.count("bind") && vm["bind"].as<bool>()) {
			items.setBindStatus("X");
		}
		else {
			if (vm.count("msdns") && vm["msdns"].as<bool>()) {
				items.setMsDNSStatus("X");
			}
		}

		if (vm.count("mailenable") && vm["mailenable"].as<bool>()) {
			items.setMailEnableStatus("X");
		}
		else {
			if (vm.count("hmail") && vm["hmail"].as<bool>()) {
				items.setHMailStatus("X");
			}
		}

		if ((vm.count("mailenable") && vm["mailenable"].as<bool>()) || (vm.count("hmail") && vm["hmail"].as<bool>())) {
			if (vm.count("spamassassin") && vm["spamassassin"].as<bool>()) {
				items.setPerlStatus("X");
				items.setSpamAssassinStatus("X");
			}
		}

		if (vm.count("awstats") && vm["awstats"].as<bool>()) {
			items.setPerlStatus("X");
			items.setAwstatsStatus("X");
		}

		if (vm.count("webalizer") && vm["webalizer"].as<bool>()) {
			items.setPerlStatus("X");
			items.setWebalizerStatus("X");
		}

		if (vm.count("roundcube") && vm["roundcube"].as<bool>()) {
			items.setPhp72Status("X");
			items.setRoundCubeStatus("X");
		}


		if (vm.count("mylittleadmin") && vm["mylittleadmin"].as<bool>()) {
			items.setMyLittleAdminStatus("X");
		}

		if (vm.count("hostname") && !vm["hostname"].as<std::string>().empty()) {
			items.setHostname(vm["hostname"].as<std::string>());
		}
		else {
			utils->consoleWrite("Please enter a hostname for fast installation", utils->RED, true);
			exit(1);
		}
	}
}

void isCloud(Items &items, boost::program_options::variables_map vm)
{
	if (vm.count("cloud") && (vm["cloud"].as<bool>())) {
		cloud_client = true;
		if (vm.count("main-ip") && !vm["main-ip"].as<std::string>().empty()) {
			main_ip = vm["main-ip"].as<std::string>();
			items.setMainServerIp(main_ip);
		}
		else {
			utils->consoleWrite("Please enter a cloud main server ip for fast installation.", utils->RED, true);
			exit(1);
		}
	}
}


void initPackages()
{
	std::string packages_response = utils->getPackageList(repo_server);
	std::stringstream ssp(packages_response.c_str());
	std::string line;
	while (std::getline(ssp, line)) {
		if (!line.empty()) {
			std::vector<std::string> pkg_arr;
			pkg_arr = utils->strSplit(line, ',');
			files.insert({ pkg_arr[0], pkg_arr[pkg_arr.size() -1] });
		}
	}
}

int main(int argc, char** argv) {
	utils = new Utils();
	boost::program_options::variables_map vm = parseParameters(argc, argv);
	packages = new Packages();
	logger = new FileLogger();


	HINSTANCE hDLL = LoadLibraryW(L"user32.dll");
	if (hDLL == NULL) {
		fprintf(stderr, "Failed to load user32.dll, error code %d\n", GetLastError());
		return 1;
	}
	utils->consoleWrite("", utils->DARKTEAL, true);
	init();

	SetConsoleTitle("FPanel Installer. (C) FPanel 2020");

	utils->setDebug(debug);


	utils->setLogger(logger);
	utils->setExec(_exec);
	packages->setDebug(debug);
	packages->setCloudClient(cloud_client);
	packages->setUtils(utils);
	packages->setLogger(logger);

	int cols, rows;
	utils->getSizeOfTerminal(&cols, &rows);

	SystemInfo systemInfo;
	Screens screens;
	if (!fast) {
		system("CLS");
		utils->SetWindow(cols, rows, cols, 9999);
	}

	bool is_server = systemInfo.isServer();
	bool is_valid = false;
	std::string default_ip = utils->defaultIpAddress();
	int system_version = systemInfo.getVersion();

	HKEY hKey;
	LONG lRes = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &hKey);
	std::wstring strKeyDefaultValue;
	utils->getStringRegKey(hKey, L"InstallationType", strKeyDefaultValue, L"NOT_EXISTS");
	std::string str(strKeyDefaultValue.begin(), strKeyDefaultValue.end());

	bool system_core = systemInfo.getCore(str);

	if (!utils->isAdmin()) {
		utils->consoleWrite("You don't have permission for access. Please execute as administrator.", utils->DARKYELLOW, true);
		logger->errorLog("You don't have permission for access. Please execute as administrator.", 1000);
		getch();
		exit(EXIT_FAILURE);
	}

	if (!is_server) {
		utils->consoleWrite("Your server version is not correct. Supported versions Windows Server 2016-2019 x64/R2/Core", utils->DARKYELLOW, true);
		logger->errorLog("Your server version is not correct. Supported versions Windows Server 2016-2019 x64/R2/Core.", 1000);
		getch();
		exit(EXIT_FAILURE);
	}

	char url[128];
	strcat(url, std::string("https://" + repo_server).c_str());
	bool conn = InternetCheckConnection(url, FLAG_ICC_FORCE_CONNECTION, 0);

	if (!conn) {
		utils->consoleWrite("There is no internet connection or repo server connection failed. Please check your configuration and try again.", utils->RED, true);
		logger->errorLog("There is no internet connection or repo server connection failed. Please check your configuration and try again.", 1000);
		getch();
		exit(EXIT_FAILURE);
	}

	utils->exec("cmd /c powershell -Command Add-MpPreference -ExclusionPath \"c:\\fpanel\\core\"");

	switch (system_version){
		case versions::Win2k16:
			is_valid = true;
			break;
		case versions::Win2k16R2:
			is_valid = true;
			break;
		case versions::Win2k19:
			is_valid = true;
			break;
		case versions::Win2k19R2:
			is_valid = true;
			break;
	}

	if (!is_valid) {
		if (debug) {
			utils->consoleWrite("Your server version is " + std::to_string(system_version), utils->DARKYELLOW, true);
		}
		utils->consoleWrite("Your server version is not correct. Supported versions Windows Server 2016-2019 x64/R2/Core", utils->DARKYELLOW, true);
		logger->errorLog("Your server version is not correct. Supported versions Windows Server 2016-2019 x64/R2/Core.", 1000);
		getch();
		exit(EXIT_FAILURE);
	}


	if (!fast) {
		system("CLS");
	}
	bool php_status = false;
	bool mail = false;


	Items items = screens.mainScreen(system_core, false, fast);
	isFast(items, fast, vm);
	isCloud(items, vm);

	utils->executeForWindows("powershell -command \"Set-MpPreference -DisableRealtimeMonitoring $true\"", true);

	logger->infoLog("Required directories are being creating.", 1000);
	utils->consoleWrite("Required directories are being creating.", utils->DARKYELLOW, true);

	utils->createDirectory(panel_dir + "\\tmp");
	utils->createDirectory(panel_dir + "\\backup");
	utils->createDirectory(panel_dir + "\\etc");
	utils->createDirectory(panel_dir + "\\migration");
	utils->createDirectory(panel_dir + "\\var\\log\\fpanel");
	utils->createDirectory(panel_dir + "\\configs\\domains");
	utils->trim(main_ip);


	initPackages(); // Packages list downloading
	packages->initPackages(system_version, repo_server, panel_dir, db_root_password, sql_admin_password, system_core); // Uncomment
	packages->setFiles(files);

	logger->infoLog("Required configs are being creating.", 1000);
	utils->consoleWrite("Required configs are being creating.", utils->DARKYELLOW, true);
	
	utils->createFile(panel_dir + "\\etc\\panel.conf");
	std::string panel_conf_content = socket_key + " " + socket_iv + " " + sql_admin_password;
	utils->writeFile(panel_dir + "\\etc\\panel.conf", panel_conf_content);

	std::string system_conf_content = "[server]\n";
	system_conf_content += "address = " + utils->defaultIpAddress() + "\n";
	system_conf_content += "[statistics]\n";
	system_conf_content += "random = 1\n";
	system_conf_content += "time = 00:00\n";

	system_conf_content += "[update]\n";
	system_conf_content += "random = 0\n";
	system_conf_content += "time = 00:00\n";
	system_conf_content += "auto = 1\n";
	utils->writeFile(panel_dir + "\\etc\\system.conf", system_conf_content);


	utils->createFile(panel_dir + "\\etc\\.sql_pass");
	utils->writeFile(panel_dir + "\\etc\\.sql_pass", db_root_password);


	utils->secEditInit();

	logger->infoLog("Required packages are installing and configuring.", 1000);
	utils->consoleWrite("Required packages are installing and configuring.", utils->DARKYELLOW, true);
	

	//////////////// .NET FRAMEWORK 3.5 /////////////////////

	logger->infoLog("Installing .NET Framework 3.5", 1000);
	installMessage("Installing .NET Framework 3.5", cols);
	std::string command = "DISM /Online /Enable-Feature /FeatureName:NetFx3 /All /norestart";
	utils->executeForWindows(command.c_str(), true);
	Sleep(1000);

	//////////////// VISUAL C++ REDIST /////////////////////
	logger->infoLog("VC++ Redist Packages installing", 1000);
	installMessage("VC++ Redist Packages installing", cols);
	packages->vcRedist(); 

	//////////////// .NET FRAMEWORKS /////////////////////
	logger->infoLog("Installing .NET Framework", 1000);
	installMessage("Installing .NET Framework", cols);
	packages->dotNet();


	logger->infoLog("System core installing and configuring", 1000);
	installMessage("Installing FPanel system packages", cols);

	//////////////// FPANEL CORE /////////////////////
	logger->infoLog("FPanel Core Binaries installing", 1000);
	installMessage("FPanel Core Binaries installing", cols);

	packages->panelCore();



	//////////////// FPANEL HTTP FRONTEND PHP REDIS /////////////////////
	if (!cloud_client) {
		logger->infoLog("FPanel HTTPD Server installing", 1000);
		installMessage("FPanel HTTPD Server installing", cols);

		packages->panelHttpd(); // Uncomment

		logger->infoLog("FPanel PHP Binaries installing", 1000);
		installMessage("FPanel PHP Binaries installing", cols);

		packages->panelPhp(); // Uncomment

		logger->infoLog("FPanel Frontend Files installing", 1000);
		installMessage("FPanel Frontend Files installing", cols);

		packages->panelHtdocs();

		logger->infoLog("FPanel Redis Server Binaries installing", 1000);
		installMessage("FPanel Redis Server Binaries installing", cols);

		packages->redis();  // Uncomment
	}


	//////////////// MAILS /////////////////////
	if (items.getHMailStatus() == "X") {
		logger->infoLog("Installing HMail Server (Mail Transfer Agent Service)", 1000);
		installMessage("Installing HMail Server (Mail Transfer Agent Service)", cols);
		packages->hMail();
		mail = true;

	}
	else if (items.getMailEnableStatus() == "X") {
		logger->infoLog("Installing MailEnable (Mail Transfer Agent Service)", 1000);
		installMessage("Installing MailEnable (Mail Transfer Agent Service)", cols);
		packages->mailEnable();
		mail = true;
	}

	if (mail) {
		if (items.getSpamAssassinStatus() == "X") {
			logger->infoLog("Installing Spamassassin (Mail Transfer Agent Spam Service)", 1000);
			installMessage("Installing Spamassassin (Mail Transfer Agent Spam Service)", cols);

			packages->spamAssassin();
			packages->spamAssassinService();
			packages->spamAssassinRegistry();
		}
	}



	//////////////// BNS /////////////////////
	if (items.getBindStatus() == "X") {
		logger->infoLog("Installing Bind (Domain Name Server)", 1000);
		installMessage("Installing Bind (Domain Name Server)", cols);

		packages->bind();
	}
	else if (items.getMsDNSStatus() == "X") {
		logger->infoLog("Installing Microsoft DNS Server", 1000);
		installMessage("Installing Microsoft DNS Server", cols);

		packages->msdns();
	}




	//////////////// MYSQL /////////////////////

	bool mysql_install = false;
	if (items.getMySql57Status() == "X") {
		logger->infoLog("Installing MySQL Server", 1000);
		installMessage("Installing MySQL Server", cols);

		packages->mysql(57);

		logger->infoLog("Configuring MySQL Server", 1000);
		utils->consoleWrite("Configuring MySQL Server", utils->DARKYELLOW, true);

		if (cloud_client) {
			std::string main_ip = items.getMainServerIp();
			utils->trim(main_ip);
			packages->firstInitMySQLForCloud(57, main_ip);
		}
		else {
			packages->firstInitMySQL(57);
		}
		mysql_install = true;
	}
	else if (items.getMySql80Status() == "X") {
		logger->infoLog("Installing MySQL Server", 1000);
		installMessage("Installing MySQL Server", cols);

		packages->mysql(80);

		logger->infoLog("Configuring MySQL Server", 1000);
		utils->consoleWrite("Configuring MySQL Server", utils->DARKYELLOW, true);

		if (cloud_client) {
			utils->trim(main_ip);
			packages->firstInitMySQLForCloud(80, main_ip);
		}
		else {
			packages->firstInitMySQL(80);
		}
		mysql_install = true;
	}


	//////////////// POSTGRESQL /////////////////////
	if (items.getPostgre11Status() == "X") {
		logger->infoLog("Installing Postgre SQL Server 11", 1000);
		installMessage("Installing Postgre SQL Server 11", cols);

		packages->postgreSql(11, main_ip, cloud_client);
	}
	else if (items.getPostgre12Status() == "X") {
		logger->infoLog("Installing Postgre SQL Server 12", 1000);
		installMessage("Installing Postgre SQL Server 12", cols);

		packages->postgreSql(12, main_ip, cloud_client);
	}
	else if (items.getPostgre13Status() == "X") {
		logger->infoLog("Installing Postgre SQL Server 13", 1000);
		installMessage("Installing Postgre SQL Server 13", cols);

		packages->postgreSql(13, main_ip, cloud_client);
	}



	//////////////// MICROSOFT SQL /////////////////////
	if ((items.getMssql2k12Status() == "X") || (items.getMssql2k16Status() == "X")) {
		int mssql_version = 0;
		if (items.getMssql2k16Status() == "X") {
			installMessage("Installing Microsoft SQL Server 2016", cols);
			logger->infoLog("Installing Microsoft SQL Server 2016", 1000);
			mssql_version = 1;
		}
		else {
			installMessage("Installing Microsoft SQL Server 2012", cols);
			logger->infoLog("Installing Microsoft SQL Server 2012", 1000);
		}

		packages->microsoftSql(mssql_version, main_ip, cloud_client);

	}


	//////////////// IIS /////////////////////
	if (items.getIisStatus() == "X") {
		logger->infoLog("Installing IIS (Internet Information Server Service)", 1000);
		installMessage("Installing IIS (Internet Information Server Service)", cols);
		packages->iis();

		if (items.getMyLittleAdminStatus() == "X") {
			packages->myLittleAdmin();
		}

		if ((items.getPhp56Status() == "X") || (items.getPhp72Status() == "X") || (items.getPhp74Status() == "X")) {
			if (items.getRoundCubeStatus() == "X") {
				logger->infoLog("Installing Webmail (Roundcube)", 1000);
				installMessage("Installing Webmail (Roundcube)", cols);
				packages->roundCube();
			}
		}

		if ((items.getPhp56Status() == "X") && (items.getphpMyAdmin49Status() == "X")) {
			logger->infoLog("Installing PHP MyAdmin", 1000);
			installMessage("Installing PHP MyAdmin", cols);
			packages->phpMyAdmin(49);
		}
		else if (((items.getPhp72Status() == "X") || (items.getPhp74Status() == "X")) && (items.getphpMyAdmin50Status() == "X")) {
			logger->infoLog("Installing PHP MyAdmin", 1000);
			installMessage("Installing PHP MyAdmin", cols);
			packages->phpMyAdmin(50);
		}

		if (items.getAwstatsStatus() == "X") {
			packages->awstats();
		}
		if (items.getWebalizerStatus() == "X") {
			packages->webalizer();
		}

		logger->infoLog("Installing .NET Core Framework", 1000);
		installMessage("Installing .NET Core Framework", cols);
		packages->dotNetCoreWebHosting();
	}


	//////////////// PERL /////////////////////
	if (items.getPerlStatus() == "X") {
		logger->infoLog("Installing Perl Script Binaries", 1000);
		installMessage("Installing Perl Script Binaries", cols);
		packages->perl();
	}


	//////////////// PYTHON /////////////////////
	if (items.getPython27Status() == "X") {
		logger->infoLog("Installing Python 2.7 Script Binaries", 1000);
		installMessage("Installing Python 2.7 Script Binaries", cols);
		packages->python(27);
	}

	if (items.getPython39Status() == "X") {
		logger->infoLog("Installing Python 3.9 Script Binaries", 1000);
		installMessage("Installing Python 3.9 Script Binaries", cols);
		packages->python(39);
	}


	//////////////// PHP /////////////////////
	if (items.getPhp56Status() == "X") {
		logger->infoLog("Installing PHP 5.6 Script Binaries", 1000);
		installMessage("Installing PHP 5.6 Script Binaries", cols);
		packages->php56();
	}
	
	if (items.getPhp72Status() == "X") {
		logger->infoLog("Installing PHP 7.2 Script Binaries", 1000);
		installMessage("Installing PHP 7.2 Script Binaries", cols);
		packages->php72();
	}
	
	if (items.getPhp74Status() == "X") {
		logger->infoLog("Installing PHP 7.4 Script Binaries", 1000);
		installMessage("Installing PHP 7.4 Script Binaries", cols);
		packages->php74();
	}
	

	//////////////// OPENSSL /////////////////////
	logger->infoLog("OpenSSL Binaries installing", 1000);
	installMessage("OpenSSL Binaries installing", cols);
	packages->openSsl(); // Uncomment



	installMessage("Finishing install", cols);
	logger->infoLog("System reconfiguring for endusers", 1000);
	utils->consoleWrite("System reconfiguring for endusers", utils->DARKYELLOW, true);


	//////////////// SYSTEM SETTINGS /////////////////////
	packages->initSystem(mysql_install, cloud_client, main_ip);


	//////////////// DNS SETTINGS /////////////////////
	if (items.getBindStatus() == "X") {
		packages->bindService();
		packages->startBindService();
	}

	//////////////// SPAMASSASSIN /////////////////////
	if (mail) {
		if (items.getSpamAssassinStatus() == "X") {
			packages->startSpamassassinService();
		}
	}

	//////////////// SYS PACKAGES /////////////////////
	std::string dns_pack;
	std::string mail_pack;
	std::string spam_pack;
	std::string statistics_pack;
	std::string database_pack = "";
	if (items.getBindStatus() == "X")
		dns_pack += "bind";
	else if (items.getMsDNSStatus() == "X")
		dns_pack += "msdns";

	if (items.getMailEnableStatus() == "X")
		mail_pack += "mail_enable";
	else if (items.getHMailStatus() == "X")
		mail_pack += "hmail";

	if ((items.getMySql57Status() == "X") || (items.getMySql80Status() == "X"))
		database_pack += "mysql,";

	if ((items.getPostgre11Status() == "X") || (items.getPostgre12Status() == "X") || (items.getPostgre13Status() == "X"))
		database_pack += "postgresql,";
	
	if ((items.getMssql2k12Status() == "X") || (items.getMssql2k16Status() == "X"))
		database_pack += "mssql,";

	utils->RemoveLastChar(database_pack);

	if (items.getAwstatsStatus() == "X")
		statistics_pack += "awstats";

	if (items.getWebalizerStatus() == "X") {
		if(statistics_pack.empty())
			statistics_pack += "webalizer";
		else
			statistics_pack += ",webalizer";
	}
	if (items.getSpamAssassinStatus() == "X") {
		spam_pack = "spamassassin";
	}

	packages->setSystemPackages("iis", dns_pack, mail_pack, database_pack, statistics_pack, spam_pack);



	//////////////// FINISH /////////////////////
	logger->infoLog("Installation is finished", 1000);
	utils->consoleWrite("Installation is finished", utils->DARKYELLOW, true);

	Sleep(200);
	utils->showConsoleCursor(true);
	char delimeter = '.';
	std::vector<std::string> arr = utils->strSplit(items.getHostname(), delimeter);
	std::string domainname = items.getHostname();
	utils->replaceString(domainname, arr[0] + ".", "");
	utils->setHostname(arr[0], domainname);
	//utils->initAdmin();
	if (!fast && !cloud_client) {
		screens.complete(db_root_password);
	}
	return 0;
}

// installer --fast --verbose --iis --perl --python39 --php56 --php72 --php74 --mysql80 --mssql16 --bind --hmail --spamassassin --roundcube --postgresql13 --phpmyadmin50 --mylittleadmin --awstats --webalizer --hostname=windows.demo.fpanel.net
// installer --fast --cloud --exec --verbose --mysql80 --mssql16 --postgresql13 --hostname=windows.demo.fpanel.net --main-ip=-192.168.139.53
