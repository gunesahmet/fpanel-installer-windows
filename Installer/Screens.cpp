#include "Screens.h"


Headers headers;
Items items;
Utils utils;
bool no_option = false;
int columns, rows;
bool server_version_core_c;
bool complete_install = false;
using namespace std;

void updateScreen(bool no_option);
bool systemScreen(bool no_option);
void selectPackageHttp(bool no_option);
void selectPackageDns(bool no_option);
void selectPackageMail(bool no_option);
void selectPackageDatabase(bool no_option);
bool continueInstall(bool no_hostname);

void Screens::InitializeInstanceFields() {
	utils.getSizeOfTerminal(&columns, &rows);
	
	items.setIisStatus("X");
	
	items.setPhpStatus("X");
	items.setPhp56Status("X");
	items.setPhp72Status("X");
	items.setPhp74Status("X");
	items.setPerlStatus("X");
	items.setPython27Status("X");
	items.setPython39Status("X");
	
	items.setMssql2k12Status("X");
	items.setMssql2k16Status(" ");
	items.setMySql57Status("X");
	items.setMySql80Status(" ");
	items.setPostgre11Status("X");
	items.setPostgre12Status(" ");
	items.setPostgre13Status(" ");
	
	items.setNetStatus("X");
	items.setBindStatus("X");
	items.setMsDNSStatus(" ");

	if (server_version_core_c) {
		items.setMailEnableStatus(" ");
		items.setMailEnableProStatus(" ");
		items.setMailEnableEntStatus(" ");
		items.setMailEnablePreStatus(" ");
		items.setHMailStatus("X");
	}
	else {
		items.setMailEnableStatus("X");
		items.setMailEnableProStatus(" ");
		items.setMailEnableEntStatus(" ");
		items.setMailEnablePreStatus(" ");
		items.setHMailStatus(" ");
	}
	items.setSpamAssassinStatus(" ");

	items.setphpMyAdmin49Status("X");
	items.setphpMyAdmin50Status(" ");
	items.setMyLittleAdminStatus("X");
	items.setRoundCubeStatus("X");

	items.setAwstatsStatus("X");
	items.setWebalizerStatus("X");

	items.setHostname("#HOSTNAME#");
	//utils.showConsoleCursor(false);
}

void restartQ(std::string answer) {
	if ((answer == "Y") || (answer == "y") || (answer == "yes") || (answer == "Yes")) {
		utils.exec("shutdown -t 0 -r -f");
	}
}

void updateScreen(bool no_option) {
	system("CLS");
	headers.setHeader("default", columns);
	std::cout << "" << std::endl;
	utils.consoleWrite("Update systems not ready...", utils.DARKTEAL, true);
	utils.consoleWrite("Press enter to return to the previous screen.", utils.DARKTEAL, true);
	getch();
	system("CLS");
}

void selectScriptPackage(bool no_option)
{
	system("CLS");
	int line;
	headers.setHeader("script", columns);
	std::cout << "" << std::endl;
	utils.consoleWrite("What would you like to do? (Enter the number of your choice)", utils.DARKTEAL, true);
	std::cout << "" << std::endl;
	utils.consoleWrite("1. phpMyAdmin 4.9        [" + items.getphpMyAdmin49Status() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("2. phpMyAdmin 5.0        [" + items.getphpMyAdmin50Status() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("3. MylittleAdmin         [" + items.getMyLittleAdminStatus() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("4. Roundcube Webmail     [" + items.getRoundCubeStatus() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("5. AwStats               [" + items.getAwstatsStatus() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("6. Webalizer             [" + items.getWebalizerStatus() + "]", utils.DARKTEAL, true);

	std::cout << "" << std::endl;
	utils.consoleWrite("9. Return ?", utils.DARKYELLOW, true);
	utils.consoleWrite("0. Quit ?", utils.DARKRED, true);
	std::cout << "" << std::endl;
	if (no_option)
		utils.consoleWrite("Wrong option, please choose a valid option.", utils.RED, true);
	std::cout << "Enter choise: ";
	std::cin >> line;
	if (cin.fail()) {
		line = 10;
		cin.clear(); cin.ignore(INT_MAX, '\n');
	}
	switch (line) {
	case 1:
		if (items.getPhp56Status() == "X") {
			if (items.getphpMyAdmin49Status() == "X") {
				items.setphpMyAdmin49Status(" ");
			}
			else {
				items.setphpMyAdmin49Status("X");
				items.setphpMyAdmin50Status(" ");
			}
		}
		else if((items.getPhp72Status() == "X") || (items.getPhp74Status() == "X")) {
			if (items.getphpMyAdmin50Status() == "X") {
				items.setphpMyAdmin50Status(" ");
			}
			else {
				items.setphpMyAdmin50Status("X");
				items.setphpMyAdmin49Status(" ");
			}
		}
		selectScriptPackage(false);
		break;
	case 2:
		if ((items.getPhp72Status() == "X") || (items.getPhp74Status() == "X")) {
			if (items.getphpMyAdmin50Status() == "X") {
				items.setphpMyAdmin50Status(" ");
			}
			else {
				items.setphpMyAdmin50Status("X");
				items.setphpMyAdmin49Status(" ");
			}
		}
		else if(items.getPhp56Status() == "X") {
			if (items.getphpMyAdmin49Status() == "X") {
				items.setphpMyAdmin49Status(" ");
			}
			else {
				items.setphpMyAdmin49Status("X");
				items.setphpMyAdmin50Status(" ");
			}
		}
		selectScriptPackage(false);
		break;
	case 3:
		if (items.getMyLittleAdminStatus() == "X")
			items.setMyLittleAdminStatus(" ");
		else
			items.setMyLittleAdminStatus("X");
		selectScriptPackage(false);
		break;
	case 4:
		if ((items.getPhp56Status() == "X") || (items.getPhp72Status() == "X") || (items.getPhp74Status() == "X")) {
			if (items.getRoundCubeStatus() == "X")
				items.setRoundCubeStatus(" ");
			else
				items.setRoundCubeStatus("X");
		}
		selectScriptPackage(false);
		break;
	case 5:
		if (items.getAwstatsStatus() == "X")
			items.setAwstatsStatus(" ");
		else
			items.setAwstatsStatus("X");
		selectScriptPackage(false);
		break;
	case 6:
		if (items.getWebalizerStatus() == "X")
			items.setWebalizerStatus(" ");
		else
			items.setWebalizerStatus("X");
		selectScriptPackage(false);
		break;

	case 9:
		systemScreen(false);
		break;
	case 0:
		//utils.showConsoleCursor(true);
		exit(EXIT_SUCCESS);
		break;
	default:
		selectScriptPackage(true);
		break;
	}
	return;
}

void selectPackageHttp(bool no_option) {
	system("CLS");
	int line;
	headers.setHeader("http", columns);
	std::cout << "" << std::endl;
	utils.consoleWrite("What would you like to do? (Enter the number of your choice)", utils.DARKTEAL, true);
	std::cout << "" << std::endl;
	utils.consoleWrite("1. IIS            [" + items.getIisStatus() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("2. Perl           [" + items.getPerlStatus() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("3. Python 2.7     [" + items.getPython27Status() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("4. Python 3.9     [" + items.getPython39Status() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("5. PHP 5.6        [" + items.getPhp56Status() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("6. PHP 7.2        [" + items.getPhp72Status() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("7. PHP 7.4        [" + items.getPhp74Status() + "]", utils.DARKTEAL, true);

	std::cout << "" << std::endl;
	utils.consoleWrite("9. Return ?", utils.DARKYELLOW, true);
	utils.consoleWrite("0. Quit ?", utils.DARKRED, true);
	std::cout << "" << std::endl;
	if (no_option)
		utils.consoleWrite("Wrong option, please choose a valid option.", utils.RED, true);
	std::cout << "Enter choise: ";
	std::cin >> line;
	if (cin.fail()) {
		line = 10;
		cin.clear(); cin.ignore(INT_MAX, '\n');
	}
	switch (line) {
		case 1:
			if (items.getIisStatus() == "X")
				items.setIisStatus(" ");
			else
				items.setIisStatus("X");
			selectPackageHttp(false);
			break;
		case 2:
			if (items.getPerlStatus() == "X")
				items.setPerlStatus(" ");
			else
				items.setPerlStatus("X");
			selectPackageHttp(false);
			break;
		case 3:
			if (items.getPython27Status() == "X")
				items.setPython27Status(" ");
			else
				items.setPython27Status("X");
			selectPackageHttp(false);
			break;
		case 4:
			if (items.getPython39Status() == "X")
				items.setPython39Status(" ");
			else
				items.setPython39Status("X");
			selectPackageHttp(false);
			break;
		case 5:
			if (items.getPhp56Status() == "X")
				items.setPhp56Status(" ");
			else
				items.setPhp56Status("X");
			selectPackageHttp(false);
			break;
		case 6:
			if (items.getPhp72Status() == "X")
				items.setPhp72Status(" ");
			else
				items.setPhp72Status("X");
			selectPackageHttp(false);
			break;
		case 7:
			if (items.getPhp74Status() == "X")
				items.setPhp74Status(" ");
			else
				items.setPhp74Status("X");
			selectPackageHttp(false);
			break;
		case 9:
			systemScreen(false);
			break;
		case 0:
			//utils.showConsoleCursor(true);
			exit(EXIT_SUCCESS);
			break;
		default:
			selectPackageHttp(true);
			break;
	}
	return;
}

void selectPackageDatabase(bool no_option) {
	int line;
	system("CLS");
	headers.setHeader("database", columns);
	std::cout << "" << std::endl;
	utils.consoleWrite("What would you like to do? (Enter the number of your choice)", utils.DARKTEAL, true);
	std::cout << "" << std::endl;
	utils.consoleWrite("1. Microsoft SQL Server 2012 Express       [" + items.getMssql2k12Status() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("2. Microsoft SQL Server 2016 Express       [" + items.getMssql2k16Status() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("3. MySQL 5.7 Database Server               [" + items.getMySql57Status() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("4. MySQL 8.0 Database Server               [" + items.getMySql80Status() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("5. PostgreSQL 11 Database Server           [" + items.getPostgre11Status() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("6. PostgreSQL 12 Database Server           [" + items.getPostgre12Status() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("7. PostgreSQL 13 Database Server           [" + items.getPostgre13Status() + "]", utils.DARKTEAL, true);
	std::cout << "" << std::endl;
	utils.consoleWrite("9. Return ?", utils.DARKYELLOW, true);
	utils.consoleWrite("0. Quit ?", utils.DARKRED, true);
	std::cout << "" << std::endl;
	if (no_option)
		utils.consoleWrite("Wrong option, please choose a valid option.", utils.RED, true);
	std::cout << "Enter choise: ";
	std::cin >> line;
	if (cin.fail()) {
		line = 6;
		cin.clear(); cin.ignore(INT_MAX, '\n');
	}
	no_option = false;
	switch (line) {
		case 1:
			if (items.getMssql2k12Status() == "X") {
				items.setMssql2k12Status(" ");
			}
			else {
				items.setMssql2k12Status("X");
				items.setMssql2k16Status(" ");
			}
			selectPackageDatabase(false);
			break;
		case 2:
			if (items.getMssql2k16Status() == "X") {
				items.setMssql2k16Status(" ");
			}
			else {
				items.setMssql2k16Status("X");
				items.setMssql2k12Status(" ");
			}
			selectPackageDatabase(false);
			break;
		case 3:
			if (items.getMySql57Status() == "X") {
				items.setMySql57Status(" ");
				items.setMySql80Status("X");
			}
			else {
				items.setMySql57Status("X");
				items.setMySql80Status(" ");
			}
			selectPackageDatabase(false);
			break;
		case 4:
			if (items.getMySql80Status() == "X") {
				items.setMySql80Status(" ");
				items.setMySql57Status("X");
			}
			else {
				items.setMySql80Status("X");
				items.setMySql57Status(" ");
			}
			selectPackageDatabase(false);
			break;
		case 5:
			if (items.getPostgre11Status() == "X") {
				items.setPostgre11Status(" ");
			}
			else {
				items.setPostgre11Status("X");
				items.setPostgre12Status(" ");
				items.setPostgre13Status(" ");
			}
			selectPackageDatabase(false);
			break;
		case 6:
			if (items.getPostgre12Status() == "X") {
				items.setPostgre12Status(" ");
			}
			else {
				items.setPostgre12Status("X");
				items.setPostgre11Status(" ");
				items.setPostgre13Status(" ");
			}
			selectPackageDatabase(false);
			break;
		case 7:
			if (items.getPostgre13Status() == "X") {
				items.setPostgre13Status(" ");
			}
			else {
				items.setPostgre13Status("X");
				items.setPostgre12Status(" ");
				items.setPostgre11Status(" ");
			}
			selectPackageDatabase(false);
			break;
		case 9:
			systemScreen(false);
			break;
		case 0:
			exit(EXIT_SUCCESS);
			break;
		default:
			selectPackageDatabase(true);
			break;
	}
}

void selectPackageDns(bool no_option) {
	int line;
	system("CLS");
	headers.setHeader("dns", columns);
	std::cout << "" << std::endl;
	utils.consoleWrite("What would you like to do? (Enter the number of your choice)", utils.DARKTEAL, true);
	std::cout << "" << std::endl;
	utils.consoleWrite("1. Bind DNS Server       [" + items.getBindStatus() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("2. Microsoft DNS Server  [" + items.getMsDNSStatus() + "]", utils.DARKTEAL, true);
	std::cout << "" << std::endl;
	utils.consoleWrite("9. Return ?", utils.DARKYELLOW, true);
	utils.consoleWrite("0. Quit ?", utils.DARKRED, true);
	std::cout << "" << std::endl;
	if (no_option)
		utils.consoleWrite("Wrong option, please choose a valid option.", utils.RED, true);
	std::cout << "Enter choise: ";
	std::cin >> line;
	if (cin.fail()) {
		line = 2;
		cin.clear(); cin.ignore(INT_MAX, '\n');
	}
	switch (line) {
		case 1:
			if (items.getBindStatus() == "X") {
				items.setBindStatus(" ");
			}
			else {
				items.setBindStatus("X");
				items.setMsDNSStatus(" ");
			}
			selectPackageDns(false);
			break;
		case 2:
			if (items.getMsDNSStatus() == "X") {
				items.setMsDNSStatus(" ");
			}
			else {
				items.setBindStatus(" ");
				items.setMsDNSStatus("X");
			}
			selectPackageDns(false);
			break;
		case 9:
			systemScreen(false);
			break;
		case 0:
			exit(EXIT_SUCCESS);
			break;
		default:
			selectPackageDns(true);
			break;
	}
}

void selectPackageMail(bool no_option) {
	int line;
	system("CLS");
	headers.setHeader("mail", columns);
	std::cout << "" << std::endl;
	utils.consoleWrite("What would you like to do? (Enter the number of your choice)", utils.DARKTEAL, true);
	std::cout << "" << std::endl;
	utils.consoleWrite("1. MailEnable SMTP/POP3 Server                 [" + items.getMailEnableStatus() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("2. HMail SMTP/POP3 Server			           [" + items.getHMailStatus() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("3. MailEnable Professional SMTP/POP3 Server    [" + items.getMailEnableProStatus() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("4. MailEnable Enterprise SMTP/POP3 Server      [" + items.getMailEnableEntStatus() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("5. MailEnable Premium SMTP/POP3 Server         [" + items.getMailEnablePreStatus() + "]", utils.DARKTEAL, true);
	utils.consoleWrite("6. Spam Assassin                               [" + items.getSpamAssassinStatus() + "]", utils.DARKTEAL, true);
	std::cout << "" << std::endl;
	utils.consoleWrite("9. Return ?", utils.DARKYELLOW, true);
	utils.consoleWrite("0. Quit ?", utils.DARKRED, true);
	std::cout << "" << std::endl;
	if (no_option)
		utils.consoleWrite("Wrong option, please choose a valid option.", utils.RED, true);
	std::cout << "Enter choise: ";
	std::cin >> line;
	if (cin.fail()) {
		line = 5;
		cin.clear(); cin.ignore(INT_MAX, '\n');
	}
	switch (line) {
		case 1:
			if (!server_version_core_c) {
				if (items.getMailEnableStatus() == "X") {
					items.setMailEnableStatus(" ");
					items.setSpamAssassinStatus(" ");
				}
				else {
					items.setMailEnableStatus("X");
					items.setMailEnableProStatus(" ");
					items.setMailEnableEntStatus(" ");
					items.setMailEnablePreStatus(" ");
					items.setHMailStatus(" ");
				}
			}
			utils.consoleWrite("You can't select this package on Server Core Edition", utils.RED, true);
			selectPackageMail(false);
			break;
		case 2:
			if (items.getHMailStatus() == "X") {
				items.setHMailStatus(" ");
				items.setSpamAssassinStatus(" ");
			}
			else {
				items.setMailEnableStatus(" ");
				items.setMailEnableProStatus(" ");
				items.setMailEnableEntStatus(" ");
				items.setMailEnablePreStatus(" ");
				items.setHMailStatus("X");
			}
			selectPackageMail(false);
			break;
		case 3:
			if (!server_version_core_c) {
				if (items.getMailEnableProStatus() == "X") {
					items.setMailEnableProStatus(" ");
					items.setSpamAssassinStatus(" ");
				}
				else {
					items.setMailEnableStatus(" ");
					items.setMailEnableProStatus("X");
					items.setMailEnableEntStatus(" ");
					items.setMailEnablePreStatus(" ");
					items.setHMailStatus(" ");
				}
			}
			utils.consoleWrite("You can't select this package on Server Core Edition", utils.RED, true);
			selectPackageMail(false);
			break;
		case 4:
			if (!server_version_core_c) {
				if (items.getMailEnableEntStatus() == "X") {
					items.setMailEnableEntStatus(" ");
					items.setSpamAssassinStatus(" ");
				}
				else {
					items.setMailEnableStatus(" ");
					items.setMailEnableProStatus(" ");
					items.setMailEnableEntStatus("X");
					items.setMailEnablePreStatus(" ");
					items.setHMailStatus(" ");
				}
			}
			utils.consoleWrite("You can't select this package on Server Core Edition", utils.RED, true);
			selectPackageMail(false);
			break;
		case 5:
			if (!server_version_core_c) {
				if (items.getMailEnablePreStatus() == "X") {
					items.setMailEnablePreStatus(" ");
					items.setSpamAssassinStatus(" ");
				}
				else {
					items.setMailEnableStatus(" ");
					items.setMailEnableProStatus(" ");
					items.setMailEnableEntStatus(" ");
					items.setMailEnablePreStatus("X");
					items.setHMailStatus(" ");
				}
			}
			utils.consoleWrite("You can't select this package on Server Core Edition", utils.RED, true);
			selectPackageMail(false);
			break;
		case 6:
			if ((items.getHMailStatus() == "X") || (items.getMailEnableStatus() == "X") || (items.getMailEnableProStatus() == "X") || (items.getMailEnableEntStatus() == "X") || (items.getMailEnablePreStatus() == "X")) {
				if (items.getSpamAssassinStatus() == "X") {
					items.setSpamAssassinStatus(" ");
				}
				else {
					if (server_version_core_c) {
						items.setHMailStatus("X");
						items.setMailEnableStatus(" ");
						items.setMailEnableProStatus(" ");
						items.setMailEnableEntStatus(" ");
						items.setMailEnablePreStatus(" ");
					}
					else {
						items.setMailEnableStatus("X");
						items.setMailEnableProStatus(" ");
						items.setMailEnableEntStatus(" ");
						items.setMailEnablePreStatus(" ");
						items.setHMailStatus(" ");
					}
					items.setSpamAssassinStatus("X");
				}
			}
			selectPackageMail(false);
			break;
		case 9:
			systemScreen(false);
			break;
		case 0:
			exit(EXIT_SUCCESS);
			break;
		default:
			selectPackageMail(true);
			break;
	}
}

void Screens::complete(std::string db_root_password) {
	system("CLS");
	bool no_hostname = true;
	bool breaked = false;
	std::string answer;
	headers.setHeader("complete", columns);
	std::cout << "" << std::endl;
	utils.consoleWrite("You SQL Password:" + db_root_password, utils.YELLOW, true);
	std::cout << "" << std::endl;
	utils.consoleWrite("If you use any firewall dont forget allow to 88XX ports", utils.DARKRED, true);
	utils.consoleWrite("Required to system restart", utils.DARKTEAL, true);
	utils.consoleWrite("Do you want to restart now? Yes/Y: ", utils.DARKTEAL, false);
	std::cin >> answer;
	restartQ(answer);
}

bool continueInstall(bool no_hostname) {
	int cnf = 0;
	TCHAR buffer[256] = TEXT("");
	DWORD dwSize = sizeof(buffer);
	GetComputerNameEx((COMPUTER_NAME_FORMAT)cnf, buffer, &dwSize);
	
	system("CLS");
	std::string default_ip = utils.defaultIpAddress();
	std::string hostname;
	headers.setHeader("continue", columns);
	std::cout << "" << std::endl;
	utils.consoleWrite("Your Default IP Address         : " + default_ip, utils.DARKRED, true);
	utils.consoleWrite("Your Computername               : " + std::string(buffer), utils.DARKRED, true);
	utils.consoleWrite("Please Enter for continue", utils.DARKYELLOW, false);
	getch();
	items.setHostname(std::string(buffer));
	complete_install = true;
	return true;
}

bool systemScreen(bool no_option) {
	if(complete_install)
		return true;
	int line;
	system("CLS");
	headers.setHeader("default", columns);
	std::cout << "" << std::endl;
	utils.consoleWrite("What would you like to do? (Enter the number of your choice)", utils.DARKTEAL, true);
	std::cout << "" << std::endl;
	utils.consoleWrite("1. HTTP ?", utils.DARKTEAL, true);
	utils.consoleWrite("2. Database ?", utils.DARKTEAL, true);
	utils.consoleWrite("3. DNS ?", utils.DARKTEAL, true);
	utils.consoleWrite("4. Mail ?", utils.DARKTEAL, true);
	utils.consoleWrite("5. Scripts ?", utils.DARKTEAL, true);
	utils.consoleWrite("6. Continue ?", utils.DARKTEAL, true);
	std::cout << "" << std::endl;
	utils.consoleWrite("0. Quit ?", utils.DARKRED, true);
	std::cout << "" << std::endl;
	if (no_option)
		utils.consoleWrite("Wrong option, please choose a valid option.", utils.RED, true);
	std::cout << "Enter choise: ";
	std::cin >> line;

	if (cin.fail()) {
		line = 6;
		cin.clear(); cin.ignore(INT_MAX, '\n');
	}

	switch (line) {
		case 1:
			selectPackageHttp(false);
			systemScreen(false);
			break;
		case 2:
			selectPackageDatabase(false);
			systemScreen(false);
			break;
		case 3:
			selectPackageDns(false);
			systemScreen(false);
			break;
		case 4:
			selectPackageMail(false);
			systemScreen(false);
			break;
		case 5:
			selectScriptPackage(false);
			systemScreen(false);
			break;
		case 6:
			continueInstall(false);
			return true;
			break;
		case 0:
			exit(EXIT_SUCCESS);
			break;
		default:
			systemScreen(true);
			break;
	}
	return true;
}

Items Screens::mainScreen(bool server_version_core, bool no_option, bool fast) {
	if (complete_install) {
		system("CLS");
		headers.setHeader("install", columns);
		std::cout << "" << std::endl;
		return items;
	}
	if (!fast) {
		system("CLS");
		server_version_core_c = server_version_core;
		int line;
		headers.setHeader("default", columns);
		std::cout << "" << std::endl;
		utils.consoleWrite("What would you like to do? (Enter the number of your choice)", utils.DARKTEAL, true);
		std::cout << "" << std::endl;
		utils.consoleWrite("1. Installation ?", utils.DARKTEAL, true);
		std::cout << "" << std::endl;
		utils.consoleWrite("0. Quit ?", utils.DARKRED, true);
		std::cout << "" << std::endl;
		if (no_option)
			utils.consoleWrite("Wrong option, please choose a valid option.", utils.RED, true);
		std::cout << "Enter choise: ";
		std::cin >> line;

		if (cin.fail()) {
			line = 3;
			cin.clear(); cin.ignore(INT_MAX, '\n');
		}

		no_option = false;

		switch (line) {
		case 1:
			systemScreen(false);
			system("CLS");
			headers.setHeader("install", columns);
			std::cout << "" << std::endl;
			return items;
			break;
		case 0:
			exit(EXIT_SUCCESS);
			break;
		default:
			line = 3;
			mainScreen(server_version_core, true, fast);
			break;
		}
		system("CLS");
		headers.setHeader("install", columns);
		std::cout << "" << std::endl;
	}

	return items;
	
}
