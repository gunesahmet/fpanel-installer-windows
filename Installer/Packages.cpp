#include "Packages.h"

static bool is_core;
enum versions { Win2k16, Win2k16R2, Win2k12, Win2k12R2, Win2k19, Win2k19R2 };

void Packages::setCloudClient(bool _cloud_client)
{
	cloud_client = _cloud_client;
}

void Packages::setDebug(bool _debug)
{
	debug = _debug;
}

void Packages::setUtils(Utils *_utils)
{
	utils = _utils;
}

void Packages::setLogger(FileLogger *_logger) {
	logger = _logger;
}

void Packages::InitializeInstanceFields() {
	char *sysDrive = getenv("SystemDrive");
	if (sysDrive == NULL) {
		systemDrive = "c:";
	}
	else {
		systemDrive = std::string(sysDrive);
	}
}

void Packages::initPackages(int server_version, std::string repo_server, std::string panel_directory, std::string sql_root_password, std::string sql_admin_password, bool server_core)
{
	server_version_c = server_version;
	panel_dir = panel_directory;
	panel_tmp_dir = panel_dir + "\\tmp\\";
	repo_server_link = repo_server;

	char *cstr = new char[sql_admin_password.length() + 1];
	strcpy(cstr, sql_admin_password.c_str());

	encrypted_sql_admin_password = md5.digestString(cstr);
	delete[] cstr;

	//if ((server_version == versions::Win2k16) || (server_version == versions::Win2k16R2)) {
	//	repo_server_link += "2016/";
	//}
	//else if ((server_version == versions::Win2k12) || (server_version == versions::Win2k12R2)) {
	//	repo_server_link += "2012/";
	//}
	//else if ((server_version == versions::Win2k19) || (server_version == versions::Win2k19R2)) {
	//	repo_server_link += "2019/";
	//}
	sql_root_password_c = sql_root_password;
	sql_admin_password_c = sql_admin_password;
	is_core = server_core;
}

void Packages::setFiles(std::map<std::string, std::string> files_md5)
{
	files = files_md5;
}

void Packages::dotNet()
{
	std::string command = panel_tmp_dir + dotnet48_msi + " /q /norestart";
	utils->downloadFile(repo_server_link, dotnet48_msi, files[dotnet48_msi], panel_tmp_dir + dotnet48_msi);
	executeWindows(command, panel_tmp_dir + dotnet48_msi, true);
}

void Packages::dotNetCoreWebHosting()
{
	std::string command = panel_tmp_dir + dotnetcore_msi + " /install /quiet /norestart";
	utils->downloadFile(repo_server_link, dotnetcore_msi, files[dotnetcore_msi], panel_tmp_dir + dotnetcore_msi);
	executeWindows(command, panel_tmp_dir + dotnet48_msi, true);
}

void Packages::vcRedist() {
	// 2008
	utils->downloadFile(repo_server_link, vcredistsx86_2008, files[vcredistsx86_2008], panel_tmp_dir + vcredistsx86_2008);
	std::string command = panel_tmp_dir + vcredistsx86_2008 + std::string(" /q /norestart");
	execute(command, panel_tmp_dir + vcredistsx86_2008);

	utils->downloadFile(repo_server_link, vcredistsx64_2008, files[vcredistsx64_2008], panel_tmp_dir + vcredistsx64_2008);
	command = panel_tmp_dir + vcredistsx64_2008 + std::string(" /q /norestart");
	execute(command, panel_tmp_dir + vcredistsx64_2008);

	// 2012
	utils->downloadFile(repo_server_link, vcredistsx86_2012, files[vcredistsx86_2012], panel_tmp_dir + vcredistsx86_2012);

	command = panel_tmp_dir + vcredistsx86_2012 + std::string(" /q /norestart");
	execute(command, panel_tmp_dir + vcredistsx86_2012);

	utils->downloadFile(repo_server_link, vcredistsx64_2012, files[vcredistsx64_2012], panel_tmp_dir + vcredistsx64_2012);
	command = panel_tmp_dir + vcredistsx64_2012 + std::string(" /q /norestart");
	execute(command, panel_tmp_dir + vcredistsx64_2012);

	// 2013
	utils->downloadFile(repo_server_link, vcredistsx86_2013, files[vcredistsx86_2013], panel_tmp_dir + vcredistsx86_2013);
	command = panel_tmp_dir + vcredistsx86_2013 + std::string(" /q /norestart");
	execute(command, panel_tmp_dir + vcredistsx86_2013);

	utils->downloadFile(repo_server_link, vcredistsx64_2013, files[vcredistsx64_2013], panel_tmp_dir + vcredistsx64_2013);
	command = panel_tmp_dir + vcredistsx64_2013 + std::string(" /q /norestart");
	execute(command, panel_tmp_dir + vcredistsx64_2013);

	// 2015-2019
	utils->downloadFile(repo_server_link, vcredistsx86_2015_19, files[vcredistsx86_2015_19], panel_tmp_dir + vcredistsx86_2015_19);
	command = panel_tmp_dir + vcredistsx86_2015_19 + std::string(" /q /norestart");
	execute(command, panel_tmp_dir + vcredistsx86_2015_19);

	utils->downloadFile(repo_server_link, vcredistsx64_2015_19, files[vcredistsx64_2015_19], panel_tmp_dir + vcredistsx64_2015_19);
	command = panel_tmp_dir + vcredistsx64_2015_19 + std::string(" /q /norestart");
	execute(command, panel_tmp_dir + vcredistsx64_2015_19);
}



void Packages::panelHttpd() {
	utils->downloadFile(repo_server_link, panel_httpd_msi, files[panel_httpd_msi], panel_tmp_dir + panel_httpd_msi);
	std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + panel_httpd_msi + " /qn";
	executeWindows(command, panel_tmp_dir + panel_httpd_msi, true);
	utils->createDirectory(panel_dir + "\\httpd\\var\\logs");
}

void Packages::panelCore() {
	utils->downloadFile(repo_server_link, core_msi, files[core_msi], panel_tmp_dir + core_msi);
	std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + core_msi + " /qn";
	executeWindows(command, panel_tmp_dir + core_msi, true);
	utils->copyDirTo(panel_dir + "\\core\\templates\\openssl\\share", panel_dir + "\\openssl\\share");
}

void Packages::panelHtdocs() {
	utils->downloadFile(repo_server_link, htdocs_msi, files[htdocs_msi], panel_tmp_dir + htdocs_msi);
	std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + htdocs_msi + " /qn";
	executeWindows(command, panel_tmp_dir + htdocs_msi, true);
	utils->createFile("c:\\fpanel\\httpd\\htdocs\\.firstboot");
}

void Packages::panelPhp() {
	utils->downloadFile(repo_server_link, panel_php_msi, files[panel_php_msi], panel_tmp_dir + panel_php_msi);
	std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + panel_php_msi + " /qn";
	executeWindows(command, panel_tmp_dir + panel_php_msi, true);

	utils->downloadFile(repo_server_link, msodbcsql_msi, files[msodbcsql_msi], panel_tmp_dir + msodbcsql_msi);
	command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + msodbcsql_msi + " /qn IACCEPTMSODBCSQLLICENSETERMS=YES";
	executeWindows(command, panel_tmp_dir + msodbcsql_msi, true);


	utils->downloadFile(repo_server_link, rewrite_msi, files[rewrite_msi], panel_tmp_dir + rewrite_msi);
	command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + rewrite_msi + " /qn IACCEPTMSODBCSQLLICENSETERMS=YES";
	executeWindows(command, panel_tmp_dir + rewrite_msi, true);
}

void Packages::openSsl() {
	utils->downloadFile(repo_server_link, openssl_msi, files[openssl_msi], panel_tmp_dir + openssl_msi);
	std::string command = panel_tmp_dir + openssl_msi + " /VERYSILENT /DIR=" + panel_dir + "\\openssl";
	executeWindows(command, panel_tmp_dir + openssl_msi, true);
}

void Packages::redis() {
	utils->downloadFile(repo_server_link, redis_msi, files[redis_msi], panel_tmp_dir + redis_msi);
	std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + redis_msi + " /qn";
	executeWindows(command, panel_tmp_dir + redis_msi, true);
}



void Packages::mysql(int sql_ver) {
	if (sql_ver == 57) {
		utils->downloadFile(repo_server_link, mysql57_msi, files[mysql57_msi], panel_tmp_dir + mysql57_msi);
		std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + mysql57_msi + " INSTALLDIR=\"" + panel_dir + "\\mysql\" /qn";
		execute(command, panel_tmp_dir + mysql57_msi);
	}
	else if (sql_ver == 80) {
		utils->downloadFile(repo_server_link, mysql80_msi, files[mysql80_msi], panel_tmp_dir + mysql80_msi);
		std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + mysql80_msi + " INSTALLDIR=\"" + panel_dir + "\\mysql\" /qn";
		execute(command, panel_tmp_dir + mysql80_msi);
	}
	Sleep(2000);
}

void Packages::microsoftSql(int sql_ver, std::string main_ip, bool cloud)
{
	std::string command;
	std::string package;

	std::string command2k12 = panel_tmp_dir + sql_2012_exp_msi + " /Q /IACCEPTSQLSERVERLICENSETERMS /ACTION=\"Install\" /FEATURES=SQLENGINE,REPLICATION,CONN,IS /INSTANCENAME=\"SQLEXPRESS\" /HELP=\"False\" /INDICATEPROGRESS=\"False\" /ERRORREPORTING=\"False\" /SQMREPORTING=\"False\" /SQLSVCSTARTUPTYPE=\"Automatic\" /SQLSVCACCOUNT=\"NT AUTHORITY\\SYSTEM\" /SQLSYSADMINACCOUNTS=\"BUILTIN\\Administrators\" /SECURITYMODE=\"SQL\" /ADDCURRENTUSERASSQLADMIN=\"True\" /TCPENABLED=\"1\" /NPENABLED=\"1\" /UPDATEENABLED=False /SQLCOLLATION=\"Latin1_General_CI_AS\" /INSTALLSHAREDDIR=c:\\fpanel\\mssql\\ /INSTALLSHAREDWOWDIR=c:\\fpanel\\mssqlx86\\ /ASBACKUPDIR=c:\\fpanel\\mssql\\backup\\ /ASCONFIGDIR=c:\\fpanel\\mssql\\Config /ASDATADIR=c:\\fpanel\\mssql\\Data\\ /ASLOGDIR=c:\\fpanel\\mssql\\Log\\ /ASTEMPDIR=c:\\fpanel\\mssql\\Temp\\ /INSTALLSQLDATADIR=c:\\fpanel\\mssql\\ /SQLBACKUPDIR=c:\\fpanel\\mssql\\backup\\ /SQLTEMPDBDIR=c:\\fpanel\\mssql\\SysData\\ /SQLUSERDBDIR=c:\\fpanel\\mssql\\Data\\ /SQLUSERDBLOGDIR=c:\\fpanel\\mssql\\Data\\ /INSTANCEDIR=c:\\fpanel\\mssql\\ /SAPWD=\"" + sql_admin_password_c + "\" /ENABLERANU=\"True\" /HIDECONSOLE";

	std::string command2k16 = panel_tmp_dir + sql_2016SP2_exp_msi + " /Q /IACCEPTSQLSERVERLICENSETERMS /ACTION=\"Install\" /FEATURES=SQLENGINE,REPLICATION,CONN,IS /INSTANCENAME=\"SQLEXPRESS\" /HELP=\"False\" /INDICATEPROGRESS=\"False\" /ERRORREPORTING=\"False\" /SQMREPORTING=\"False\" /SQLSVCSTARTUPTYPE=\"Automatic\" /SQLSVCACCOUNT=\"NT AUTHORITY\\SYSTEM\" /SQLSYSADMINACCOUNTS=\"BUILTIN\\Administrators\" /SECURITYMODE=\"SQL\" /ADDCURRENTUSERASSQLADMIN=\"True\" /TCPENABLED=\"1\" /NPENABLED=\"1\" /UPDATEENABLED=False /SQLCOLLATION=\"Latin1_General_CI_AS\" /INSTALLSHAREDDIR=c:\\fpanel\\mssql\\ /INSTALLSHAREDWOWDIR=c:\\fpanel\\mssqlx86\\ /ASBACKUPDIR=c:\\fpanel\\mssql\\backup\\ /ASCONFIGDIR=c:\\fpanel\\mssql\\Config /ASDATADIR=c:\\fpanel\\mssql\\Data\\ /ASLOGDIR=c:\\fpanel\\mssql\\Log\\ /ASTEMPDIR=c:\\fpanel\\mssql\\Temp\\ /INSTALLSQLDATADIR=c:\\fpanel\\mssql\\ /SQLBACKUPDIR=c:\\fpanel\\mssql\\backup\\ /SQLTEMPDBDIR=c:\\fpanel\\mssql\\SysData\\ /SQLUSERDBDIR=c:\\fpanel\\mssql\\Data\\ /SQLUSERDBLOGDIR=c:\\fpanel\\mssql\\Data\\ /INSTANCEDIR=c:\\fpanel\\mssql\\ /SAPWD=\"" + sql_admin_password_c + "\" /ENABLERANU=\"True\" /HIDECONSOLE";

	if (sql_ver == 0) {
		command = command2k12;
		package = sql_2012_exp_msi;
	}
	else if (sql_ver == 1) {
		command = command2k16;
		package = sql_2016SP2_exp_msi;
	}
	utils->downloadFile(repo_server_link, package, files[package], panel_tmp_dir + package);
	std::string url = repo_server_link + package;
	std::string file = panel_tmp_dir + package;
	executeWindows(command, file, true);
	Sleep(2000);

	if (cloud) {
		utils->executeForWindows("netsh advfirewall firewall add rule name=\"TCP Port 1433\" dir=in action=allow protocol=TCP localport=1433", true);
	}
	std::string folder_name;
	if (sql_ver == 0) {
		setTcpPort("SOFTWARE\\Microsoft\\Microsoft SQL Server\\MSSQL11.SQLEXPRESS\\MSSQLServer\\SuperSocketNetLib\\Tcp\\IP5");
		setTcpPort("SOFTWARE\\Microsoft\\Microsoft SQL Server\\MSSQL11.SQLEXPRESS\\MSSQLServer\\SuperSocketNetLib\\Tcp\\IPAll");
		folder_name = "sql2012ExpSP2_x64";
	}
	else if (sql_ver == 1) {
		setTcpPort("SOFTWARE\\Microsoft\\Microsoft SQL Server\\MSSQL13.SQLEXPRESS\\MSSQLServer\\SuperSocketNetLib\\Tcp\\IP5");
		setTcpPort("SOFTWARE\\Microsoft\\Microsoft SQL Server\\MSSQL13.SQLEXPRESS\\MSSQLServer\\SuperSocketNetLib\\Tcp\\IPAll");
		folder_name = "sql2016ExpSP2_x64";
		std::string current_path = utils->getCurrentPath() + folder_name;
		utils->removeFolder(current_path);
	}
	else if (sql_ver == 2) {
		setTcpPort("SOFTWARE\\Microsoft\\Microsoft SQL Server\\MSSQL15.SQLEXPRESS\\MSSQLServer\\SuperSocketNetLib\\Tcp\\IP5");
		setTcpPort("SOFTWARE\\Microsoft\\Microsoft SQL Server\\MSSQL15.SQLEXPRESS\\MSSQLServer\\SuperSocketNetLib\\Tcp\\IPAll");
	}
	utils->stopService("MSSQL$SQLEXPRESS");
	Sleep(1500);
	utils->startService("MSSQL$SQLEXPRESS");



}

void Packages::postgreSql(int sql_ver, std::string main_ip, bool cloud)
{
	std::string postgresql_msi = "";
	if (sql_ver == 11) {
		postgresql_msi = postgresql11_msi;
	}
	else if (sql_ver == 12) {
		postgresql_msi = postgresql12_msi;
	}
	else if (sql_ver == 13) {
		postgresql_msi = postgresql13_msi;
	}

	std::string pg_dir = panel_dir + "\\postgresql";
	std::string pg_bin_dir = pg_dir + "\\bin\\";
//        
	//sql_root_password_c = "ENTER_SECRET_KEY";
	//sql_admin_password_c = "ENTER_SECRET_KEY";

	std::string command = "start /w " + panel_tmp_dir + postgresql_msi + " --prefix \"" + pg_dir + "\" --datadir \"" + pg_dir + "\\data\" --superpassword " + sql_root_password_c + " --superaccount postgres --servicename PostgreSQL --enable_acledit 1 --create_shortcuts 0 --mode unattended --unattendedmodeui none --disable-components pgAdmin";
	utils->downloadFile(repo_server_link, postgresql_msi, files[postgresql_msi], panel_tmp_dir + postgresql_msi);

	execute(command, panel_tmp_dir + postgresql_msi);
	Sleep(500);
	utils->stopService("PostgreSQL");

	std::string trusted_default_conf;
	trusted_default_conf = "# TYPE   DATABASE        USER            ADDRESS                 METHOD\n";
	trusted_default_conf += "local   all             all                                     trust\n";
	trusted_default_conf += "host    all             all             127.0.0.1/32            trust\n";
	trusted_default_conf += "host    all             all             ::1/128                 trust\n";

	utils->removeFile(pg_dir + "\\data\\pg_hba.conf");
	Sleep(500);
	utils->writeFile(pg_dir + "\\data\\pg_hba.conf", trusted_default_conf);
	Sleep(500);
	utils->replaceStringInFile(panel_dir + "\\postgresql\\data\\postgresql.conf", "password_encryption = scram-sha-256", "password_encryption = md5");
	if (cloud) {
		utils->replaceStringInFileWithRegex(panel_dir + "\\postgresql\\data\\postgresql.conf", "listen_addresses.*=.*", "listen_addresses = '*'");
		utils->replaceStringInFileWithRegex(panel_dir + "\\postgresql\\data\\postgresql.conf", "#.*listen_addresses.*=.*", "listen_addresses = '*'");
	}
	utils->startService("PostgreSQL");

	std::string postgres_alter_command = pg_bin_dir + "psql -U postgres -d template1 -h localhost -c \"ALTER USER postgres with password '" + sql_admin_password_c + "';\"";
	std::string fpanel_admin_create_command = pg_bin_dir + "psql -U postgres -d template1 -h localhost -c \"CREATE USER fpaneladmin WITH PASSWORD '" + sql_admin_password_c + "';\"";
	std::string fpanel_admin_alter_command = pg_bin_dir + "psql -U postgres -d template1 -h localhost -c \"ALTER USER fpaneladmin WITH SUPERUSER;\"";
	std::string fpanel_admin_alter_grant_command = pg_bin_dir + "psql -U postgres -d template1 -h localhost -c \"GRANT ALL ON SCHEMA public TO fpaneladmin;\"";

	utils->executeForWindows(postgres_alter_command, true);
	utils->executeForWindows(fpanel_admin_create_command, true);
	utils->executeForWindows(fpanel_admin_alter_command, true);
	utils->executeForWindows(fpanel_admin_alter_grant_command, true);
	Sleep(1500);


	if (cloud) {
		utils->executeForWindows("netsh advfirewall firewall add rule name=\"TCP Port 5432\" dir=in action=allow protocol=TCP localport=5432", true);
	}

	std::string default_conf;
	default_conf = "# TYPE   DATABASE        USER            ADDRESS                 METHOD\n";
	default_conf += "local   all             all                                     md5\n";
	default_conf += "host    all             all             127.0.0.1/32            md5\n";
	default_conf += "host    all             all             ::1/128                 md5\n";

	if (cloud) {
		default_conf += "host    all             fpaneladmin     " + main_ip + "/32            md5\n";
		default_conf += "host    all             all             0.0.0.0/0               md5\n";
	}

	default_conf += "# Replication\n";
	default_conf += "host    replication     all             127.0.0.1/32            md5\n";
	default_conf += "host    replication     all             ::1/128                 md5\n";
	if (cloud) {
		default_conf += "host    replication     all             " + main_ip + "/32            md5\n";
	}


	utils->removeFile(pg_dir + "\\data\\pg_hba.conf");
	Sleep(500);
	utils->writeFile(pg_dir + "\\data\\pg_hba.conf", default_conf);
	Sleep(500);

	// Eski dosya siliniyor yerine trusted olan yaziliyor
	Sleep(200);
	utils->stopService("PostgreSQL");
	Sleep(500);
	utils->startService("PostgreSQL");

	// fpaneladmin icin kullanici olusturuluyor
	//std::string db_query_command = pg_bin_dir + "psql -U postgres -d template1 -c \"CREATE USER fpaneladmin WITH PASSWORD '" + sql_admin_password_c + "';\"";
	//utils->executeForWindows(db_query_command, true);
	//db_query_command = pg_bin_dir + "psql -U postgres -d template1 -c \"ALTER USER fpaneladmin WITH SUPERUSER;\"";
	//utils->executeForWindows(db_query_command, true);
	Sleep(500);
}

void Packages::firstInitMySQL(int sql_ver) {
	utils->createDirectory(panel_dir + "\\mysql\\var");
	utils->killProcessByName("mysqld");

	if (sql_ver == 57) {
		utils->copyFile(panel_dir + "\\core\\templates\\mysql\\my.ini", panel_dir + "\\mysql\\etc\\my.ini");
		Sleep(1000);

		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysqld.exe --initialize-insecure --user=mysql", true);
		Sleep(2000);
		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysqld.exe --defaults-file=\"" + panel_dir + "\\mysql\\etc\\my.ini\"", false);
		Sleep(1000);
		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysql.exe -u root -e \"ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '" + sql_root_password_c + "';\"", false);
		Sleep(1000);
		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysqld.exe" + " --install MySQLServer --defaults-file=\"" + panel_dir + "\\mysql\\etc\\my.ini\"", true);
	}
	else if (sql_ver == 80) {
		utils->copyFile(panel_dir + "\\core\\templates\\mysql\\my80.ini", panel_dir + "\\mysql\\etc\\my.ini");
		Sleep(1000);

		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysqld.exe --initialize-insecure --user=mysql", true);
		Sleep(2000);
		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysqld.exe --defaults-file=\"" + panel_dir + "\\mysql\\etc\\my.ini\"", false);
		Sleep(1000);
		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysql.exe -u root -e \"ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '" + sql_root_password_c + "';\"", false);
		Sleep(1000);
		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysqld.exe" + " --install MySQLServer --defaults-file=\"" + panel_dir + "\\mysql\\etc\\my.ini\"", true);
	}


	utils->killProcessByName("mysqld");
	utils->removeFile(panel_dir + "\\mysql\\init.txt");

	utils->startService("MySQLServer");
	Sleep(1000);
}

void Packages::firstInitMySQLForCloud(int sql_ver, std::string main_ip)
{
	utils->executeForWindows("netsh advfirewall firewall add rule name=\"TCP Port 3306\" dir=in action=allow protocol=TCP localport=3306", true);
	utils->createDirectory("c:\\fpanel\\mysql\\var");
	utils->killProcessByName("mysqld");
	if (sql_ver == 57) {
		utils->createDirectory("c:\\fpanel\\mysql\\etc"); // Only for 5.7
		
		std::string mysql_ini_content = "[client]\n";
		mysql_ini_content += "port = 3306\n";
		mysql_ini_content += "socket = c:/fpanel/mysql/tmp/mysql.sock\n\n";
		mysql_ini_content += "[mysqld]\n";
		mysql_ini_content += "bind-address = 0.0.0.0\n";
		mysql_ini_content += "port = 3306\n";
		mysql_ini_content += "socket = c:/fpanel/mysql/tmp/mysql.sock\n";
		mysql_ini_content += "skip-external-locking\n";
		mysql_ini_content += "key_buffer_size = 16M\n";
		mysql_ini_content += "max_allowed_packet = 1M\n";
		mysql_ini_content += "table_open_cache = 64\n";
		mysql_ini_content += "sort_buffer_size = 512K\n";
		mysql_ini_content += "net_buffer_length = 8K\n";
		mysql_ini_content += "read_buffer_size = 256K\n";
		mysql_ini_content += "read_rnd_buffer_size = 512K\n";
		mysql_ini_content += "myisam_sort_buffer_size = 8M\n";
		mysql_ini_content += "log-bin=mysql-bin\n";
		mysql_ini_content += "binlog_format = mixed\n";
		mysql_ini_content += "server-id	= 1\n";
		mysql_ini_content += "log_error = c:/fpanel/mysql/var/error.log\n";
		mysql_ini_content += "general_log_file = c:/fpanel/mysql/var/mysql.log\n";
		mysql_ini_content += "general_log = 1\n\n";
		mysql_ini_content += "[mysqldump]\n";
		mysql_ini_content += "quick\n";
		mysql_ini_content += "max_allowed_packet = 16M\n\n";
		mysql_ini_content += "[mysql]\n";
		mysql_ini_content += "no-auto-rehash\n\n";
		mysql_ini_content += "[myisamchk]\n";
		mysql_ini_content += "key_buffer_size = 20M\n";
		mysql_ini_content += "sort_buffer_size = 20M\n";
		mysql_ini_content += "read_buffer = 2M\n";
		mysql_ini_content += "write_buffer = 2M\n\n";
		mysql_ini_content += "[mysqlhotcopy]\n";
		mysql_ini_content += "interactive-timeout";

		utils->writeFile(panel_dir + "\\mysql\\etc\\my.ini", mysql_ini_content);
	

		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysqld.exe --initialize-insecure --user=mysql", true);
		Sleep(2000);
		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysqld.exe --defaults-file=\"" + panel_dir + "\\mysql\\etc\\my.ini\"", false);
		Sleep(1000);
		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysql.exe -u root -e \"ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '" + sql_root_password_c + "';\"", false);
		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysql.exe -u root -p" + sql_root_password_c + " -e \"UPDATE mysql.user SET host='%' WHERE user='root';\"", false);
		Sleep(1000);
		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysqld.exe" + " --install MySQLServer --defaults-file=\"" + panel_dir + "\\mysql\\etc\\my.ini\"", true);
	}
	else if (sql_ver == 80) {
		std::string mysql_ini_content = "[client]\n";
		mysql_ini_content += "port = 3306\n";
		mysql_ini_content += "socket = c:/fpanel/mysql/tmp/mysql.sock\n\n";
		mysql_ini_content += "[mysqld]\n";
		mysql_ini_content += "bind-address = 0.0.0.0\n";
		mysql_ini_content += "port = 3306\n";
		mysql_ini_content += "socket = c:/fpanel/mysql/tmp/mysql.sock\n";
		mysql_ini_content += "skip-external-locking\n";
		mysql_ini_content += "key_buffer_size = 16M\n";
		mysql_ini_content += "max_allowed_packet = 1M\n";
		mysql_ini_content += "table_open_cache = 64\n";
		mysql_ini_content += "sort_buffer_size = 512K\n";
		mysql_ini_content += "net_buffer_length = 8K\n";
		mysql_ini_content += "read_buffer_size = 256K\n";
		mysql_ini_content += "read_rnd_buffer_size = 512K\n";
		mysql_ini_content += "myisam_sort_buffer_size = 8M\n";
		mysql_ini_content += "default_authentication_plugin = mysql_native_password\n";
		mysql_ini_content += "log-bin = mysql-bin\n";
		mysql_ini_content += "binlog_format = mixed\n";
		mysql_ini_content += "server-id	= 1\n";
		mysql_ini_content += "log_error = c:/fpanel/mysql/var/error.log\n";
		mysql_ini_content += "general_log_file = c:/fpanel/mysql/var/mysql.log\n";
		mysql_ini_content += "general_log = 1\n\n";
		mysql_ini_content += "[mysqldump]\n";
		mysql_ini_content += "quick\n";
		mysql_ini_content += "max_allowed_packet = 16M\n\n";
		mysql_ini_content += "[mysql]\n";
		mysql_ini_content += "no-auto-rehash\n\n";
		mysql_ini_content += "[myisamchk]\n";
		mysql_ini_content += "key_buffer_size = 20M\n";
		mysql_ini_content += "sort_buffer_size = 20M\n";
		mysql_ini_content += "read_buffer = 2M\n";
		mysql_ini_content += "write_buffer = 2M\n\n";
		mysql_ini_content += "[mysqlhotcopy]\n";
		mysql_ini_content += "interactive-timeout";

		utils->writeFile(panel_dir + "\\mysql\\etc\\my.ini", mysql_ini_content);

		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysqld.exe --initialize-insecure --user=mysql", true);
		Sleep(2000);

		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysqld.exe --defaults-file=\"" + panel_dir + "\\mysql\\etc\\my.ini\"", false);
		Sleep(1000);
		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysql.exe -u root -e \"ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '" + sql_root_password_c + "';\"", false);
		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysql.exe -u root -p" + sql_root_password_c + " -e \"UPDATE mysql.user SET host='%' WHERE user='root';\"", false);
		Sleep(1000);
		utils->executeForWindows(panel_dir + "\\mysql\\bin\\mysqld.exe" + " --install MySQLServer --defaults-file=\"" + panel_dir + "\\mysql\\etc\\my.ini\"", true);
	}
	utils->killProcessByName("mysqld");

	//utils->consoleWrite("Starting service...", colour::DARKYELLOW, true);
	utils->startService("MySQLServer");
	Sleep(1000);


	std::string mysql_command = panel_dir + "\\mysql\\bin\\mysql.exe -u root -p" + sql_root_password_c;
	std::string create_fpanel_user_query;
	std::string remote_create_fpanel_user_query;
	if (sql_ver == 57) {
		create_fpanel_user_query = "CREATE USER 'fpaneladmin'@'localhost' IDENTIFIED BY '" + sql_admin_password_c + "';";
		create_fpanel_user_query += "GRANT ALL PRIVILEGES ON *.* TO 'fpaneladmin'@'localhost' WITH GRANT OPTION;";
		remote_create_fpanel_user_query = "CREATE USER 'fpaneladmin'@'" + main_ip + "' IDENTIFIED BY '" + sql_admin_password_c + "';";
		remote_create_fpanel_user_query += " alter user 'fpaneladmin'@'" + main_ip + "' identified with mysql_native_password by '" + sql_admin_password_c + "';";
		remote_create_fpanel_user_query += "GRANT ALL PRIVILEGES ON *.* TO 'fpaneladmin'@'" + main_ip + "' WITH GRANT OPTION;";
	}
	else {
		create_fpanel_user_query = "CREATE USER 'fpaneladmin'@'localhost' IDENTIFIED WITH mysql_native_password BY '" + sql_admin_password_c + "';";
		create_fpanel_user_query += "GRANT ALL PRIVILEGES ON *.* TO 'fpaneladmin'@'localhost' WITH GRANT OPTION;";
		remote_create_fpanel_user_query = "CREATE USER 'fpaneladmin'@'" + main_ip + "' IDENTIFIED WITH mysql_native_password BY '" + sql_admin_password_c + "';";
		remote_create_fpanel_user_query += "GRANT ALL PRIVILEGES ON *.* TO 'fpaneladmin'@'" + main_ip + "' WITH GRANT OPTION;";
	}

	create_fpanel_user_query += "FLUSH PRIVILEGES;";

	if (cloud_client) {
		utils->executeForWindows(mysql_command + " -e \"" + remote_create_fpanel_user_query + "\" mysql", true);
	}
	utils->executeForWindows(mysql_command + " -e \"" + create_fpanel_user_query + "\" mysql", true);
}




void Packages::mailEnable()
{
	if (!is_core) {
		std::string mail_dir = panel_dir + "\\mail";
		utils->openKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Mail Enable");
		utils->openKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Mail Enable\\Mail Enable");
		HKEY mailEnableKey = utils->openKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Mail Enable\\Mail Enable");

		setMailEnableRegistry("SOFTWARE\\Wow6432Node\\Mail Enable\\Mail Enable", "Program Directory", mail_dir);
		setMailEnableRegistry("SOFTWARE\\Wow6432Node\\Mail Enable\\Mail Enable", "Debug Log File Directory", mail_dir + "\\Logging");
		setMailEnableRegistry("SOFTWARE\\Wow6432Node\\Mail Enable\\Mail Enable", "W3C Logging File Directory", mail_dir + "\\Logging");
		setMailEnableRegistry("SOFTWARE\\Wow6432Node\\Mail Enable\\Mail Enable", "Activity Log Directory", mail_dir + "\\Logging");
		setMailEnableRegistry("SOFTWARE\\Wow6432Node\\Mail Enable\\Mail Enable", "Bad Mail Directory", mail_dir + "\\Bad Mail");
		setMailEnableRegistry("SOFTWARE\\Wow6432Node\\Mail Enable\\Mail Enable", "Configuration Directory", mail_dir + "\\CONFIG");
		setMailEnableRegistry("SOFTWARE\\Wow6432Node\\Mail Enable\\Mail Enable", "Application Directory", mail_dir + "\\Bin");
		setMailEnableRegistry("SOFTWARE\\Wow6432Node\\Mail Enable\\Mail Enable", "Data Directory", mail_dir);
		setMailEnableRegistry("SOFTWARE\\Wow6432Node\\Mail Enable\\Mail Enable", "Quarantine Directory", mail_dir + "\\Quarantine");
		setMailEnableRegistry("SOFTWARE\\Wow6432Node\\Mail Enable\\Mail Enable", "Mail Root", mail_dir + "\\POSTOFFICES");

		Sleep(1000);
		std::string command = panel_tmp_dir + mailenable_msi + " /s /B";
		utils->downloadFile(repo_server_link, mailenable_msi, files[mailenable_msi], panel_tmp_dir + mailenable_msi);

		executeWindows(command, panel_tmp_dir + mailenable_msi, true);
		utils->copyFile("C:\\fpanel\\mail\\bin\\MailEnable.Administration.dll", "C:\\fpanel\\core\\bin");
	}
}

void Packages::hMail()
{
	utils->downloadFile(repo_server_link, hmail_msi, files[hmail_msi], panel_tmp_dir + hmail_msi);
	std::string command = panel_tmp_dir + hmail_msi + " /verysilent /NOCANCEL /SUPPRESSMSGBOXES /NORESTART /DIR=\"C:\\fpanel\\mail\" /PASSWORD=" + sql_admin_password_c;
	executeWindows(command, panel_tmp_dir + hmail_msi, true);
	Sleep(1000);
	utils->copyFile("C:\\fpanel\\mail\\Bin\\Interop.hMailServer.dll", "C:\\fpanel\\core\\bin");
}

void Packages::spamAssassin()
{
	std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + spamassassin_msi + " /qn";
	utils->downloadFile(repo_server_link, spamassassin_msi, files[spamassassin_msi], panel_tmp_dir + spamassassin_msi);
	execute(command, panel_tmp_dir + spamassassin_msi);
}

void Packages::spamAssassinService()
{
	utils->executeForWindows("sc.exe create \"FPanel SpamAssassin Service\" binPath= \"" + panel_dir + "\\core\\services\\FPanelSpamAssassin.exe \\\"" + panel_dir + "\\spamassassin\\spamd.exe\" type= own start= auto error= ignore", true);
}

void Packages::spamAssassinRegistry()
{
	std::string spam_dir = panel_dir + "\\spamassassin";
	utils->openKey(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\SpamAssassin\\Parameters");
	spamAssassinRegister("SYSTEM\\CurrentControlSet\\Services\\SpamAssassin\\Parameters", "Application", "\"c:\\fpanel\\spamassassin\\spamd.exe\" -l -s \"c:\\fpanel\\spamassassin\\logs\\spamd.log\" --max-spare=2 --max-children=20 --timeout-child=85");
	spamAssassinRegister("SYSTEM\\CurrentControlSet\\Services\\SpamAssassin\\Parameters", "AppDirectory", spam_dir);

	std::string mta_agent = spam_dir + "\\MESpamC.exe";
	HKEY mailEnableKey = utils->openKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Mail Enable\\Mail Enable");
	setMailEnableRegistry("SOFTWARE\\Wow6432Node\\Mail Enable\\Mail Enable\\Agents\\MTA", "Pickup Event Command", mta_agent);
	setMailEnableRegistryDword("SOFTWARE\\Wow6432Node\\Mail Enable\\Mail Enable\\Agents\\MTA", "Pickup Event Enabled", 1);

}

void Packages::startSpamassassinService()
{
	utils->startService("FPanel SpamAssassin Service");
}



void Packages::iis()
{
	std::string main_command = "DISM ";
	std::string features = "/Online /Enable-Feature ";
	features.append("/FeatureName:IIS-WebServer /all ");
	features.append("/norestart");
	utils->exec((main_command + features).c_str());


	features = "/Online /Enable-Feature ";
	features.append("/FeatureName:IIS-ApplicationDevelopment ");
	features.append("/FeatureName:IIS-ASP ");
	features.append("/FeatureName:IIS-ASPNET ");
	features.append("/FeatureName:IIS-ASPNET45 ");
	features.append("/FeatureName:IIS-NetFxExtensibility45 ");
	features.append("/FeatureName:NetFx4Extended-ASPNET45 ");
	features.append("/FeatureName:IIS-CertProvider ");
	features.append("/FeatureName:IIS-ApplicationInit ");
	features.append("/FeatureName:IIS-BasicAuthentication ");
	features.append("/FeatureName:IIS-CGI ");
	features.append("/FeatureName:IIS-ClientCertificateMappingAuthentication ");
	features.append("/FeatureName:IIS-CommonHttpFeatures ");
	features.append("/FeatureName:IIS-CustomLogging ");
	features.append("/FeatureName:IIS-DefaultDocument ");
	features.append("/FeatureName:IIS-DigestAuthentication ");
	features.append("/FeatureName:IIS-DirectoryBrowsing ");
	features.append("/FeatureName:IIS-FTPExtensibility ");
	features.append("/FeatureName:IIS-FTPServer ");
	features.append("/FeatureName:IIS-FTPSvc ");
	features.append("/FeatureName:IIS-HealthAndDiagnostics ");
	features.append("/FeatureName:IIS-HostableWebCore ");
	features.append("/FeatureName:IIS-HttpCompressionDynamic ");
	features.append("/FeatureName:IIS-HttpCompressionStatic ");
	features.append("/FeatureName:IIS-HttpErrors ");
	features.append("/FeatureName:IIS-HttpLogging ");
	features.append("/FeatureName:IIS-HttpRedirect ");
	features.append("/FeatureName:IIS-HttpTracing ");
	features.append("/FeatureName:IIS-IIS6ManagementCompatibility ");
	features.append("/FeatureName:IIS-IISCertificateMappingAuthentication ");
	features.append("/FeatureName:IIS-IPSecurity ");
	features.append("/FeatureName:IIS-ISAPIExtensions ");
	features.append("/FeatureName:IIS-ISAPIFilter ");
	features.append("/FeatureName:IIS-LegacyScripts ");
	features.append("/FeatureName:IIS-LegacySnapIn ");
	features.append("/FeatureName:IIS-LoggingLibraries ");
	features.append("/FeatureName:IIS-ManagementConsole ");
	features.append("/FeatureName:IIS-ManagementScriptingTools ");
	features.append("/FeatureName:IIS-ManagementService ");
	features.append("/FeatureName:IIS-Metabase ");
	features.append("/FeatureName:IIS-NetFxExtensibility ");
	features.append("/FeatureName:IIS-ODBCLogging ");
	features.append("/FeatureName:IIS-Performance ");
	features.append("/FeatureName:IIS-RequestFiltering ");
	features.append("/FeatureName:IIS-RequestMonitor ");
	features.append("/FeatureName:IIS-Security ");
	features.append("/FeatureName:IIS-ServerSideIncludes ");
	features.append("/FeatureName:IIS-StaticContent ");
	features.append("/FeatureName:IIS-URLAuthorization ");
	features.append("/FeatureName:IIS-WebDAV ");
	features.append("/FeatureName:IIS-WebServerManagementTools ");
	features.append("/FeatureName:IIS-WebServerRole ");
	features.append("/FeatureName:IIS-WindowsAuthentication ");
	features.append("/FeatureName:IIS-WMICompatibility ");
	features.append("/FeatureName:WAS-ConfigurationAPI ");
	features.append("/FeatureName:WAS-NetFxEnvironment ");
	features.append("/FeatureName:WAS-ProcessModel ");
	features.append("/FeatureName:WAS-WindowsActivationService ");
	features.append("/all ");
	features.append("/norestart");
	utils->exec((main_command + features).c_str());

	Sleep(1000);
	//utils->exec("start /wait pkgmgr /iu:IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;IIS-;WAS-;WAS-;WAS-NetFxEnvironment;WAS-ConfigurationAPI /norestart");

}


void Packages::perl() {
	std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + perl_msi + " INSTALLDIR=" + panel_dir + "\\perl PERL_PATH=\"Yes\" /qn";
	utils->downloadFile(repo_server_link, perl_msi, files[perl_msi], panel_tmp_dir + perl_msi);
	executeWindows(command, panel_tmp_dir + perl_msi, true);
}

void Packages::python(int version) {
	if (version == 27) {
		std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + python27_msi + " /qn";
		utils->downloadFile(repo_server_link, python27_msi, files[python27_msi], panel_tmp_dir + python27_msi);
		executeWindows(command, panel_tmp_dir + python27_msi, true);
	}
	
	if (version == 39) {
		std::string command = panel_tmp_dir + python39_exe + " /quiet Include_lib=1 Include_pip=1 Include_exe=1 TargetDir=" + panel_dir + "\\python\\39\\ PrependPath=1";
		utils->downloadFile(repo_server_link, python39_exe, files[python39_exe], panel_tmp_dir + python39_exe);
		executeWindows(command, panel_tmp_dir + python39_exe, true);
	}
}

void Packages::php72() {
	std::string command_72 = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + php72_msi + " /qn";
	utils->downloadFile(repo_server_link, php72_msi, files[php72_msi], panel_tmp_dir + php72_msi);
	executeWindows(command_72, panel_tmp_dir + php72_msi, true);
}

void Packages::php74() {
	std::string command_74 = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + php74_msi + " /qn";
	utils->downloadFile(repo_server_link, php74_msi, files[php74_msi], panel_tmp_dir + php74_msi);
	executeWindows(command_74, panel_tmp_dir + php74_msi, true);
}

void Packages::php56() {
	std::string command_56 = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + php56_msi + " /qn";
	utils->downloadFile(repo_server_link, php56_msi, files[php56_msi], panel_tmp_dir + php56_msi);
	executeWindows(command_56, panel_tmp_dir + php56_msi, true);
}

void Packages::myLittleAdmin()
{
	//command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + mylittleadmin_msi + " /qn IACCEPTMSODBCSQLLICENSETERMS=YES";
	//utils->downloadFile(repo_server_link, mylittleadmin_msi, files[mylittleadmin_msi], panel_tmp_dir + mylittleadmin_msi);
	//executeWindows(command, panel_tmp_dir + mylittleadmin_msi, true);

	std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + mylittleadmin_msi + " /qn";
	utils->downloadFile(repo_server_link, mylittleadmin_msi, files[mylittleadmin_msi], panel_tmp_dir + mylittleadmin_msi);
	executeWindows(command, panel_tmp_dir + mylittleadmin_msi, true);
}

void Packages::phpMyAdmin(int version)
{
	std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + dbmyadmin_msi + " /qn";
	utils->downloadFile(repo_server_link, dbmyadmin_msi, files[dbmyadmin_msi], panel_tmp_dir + dbmyadmin_msi);
	executeWindows(command, panel_tmp_dir + dbmyadmin_msi, true);
}

void Packages::roundCube()
{
	std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + roundcube_msi + " /qn";
	utils->downloadFile(repo_server_link, roundcube_msi, files[roundcube_msi], panel_tmp_dir + roundcube_msi);
	execute(command, panel_tmp_dir + roundcube_msi);
}

void Packages::awstats()
{
	std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + awstats_msi + " /qn";
	utils->downloadFile(repo_server_link, awstats_msi, files[awstats_msi], panel_tmp_dir + awstats_msi);
	execute(command, panel_tmp_dir + awstats_msi);
}

void Packages::webalizer()
{
	std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + webalizer_msi + " /qn";
	utils->downloadFile(repo_server_link, webalizer_msi, files[webalizer_msi], panel_tmp_dir + webalizer_msi);
	execute(command, panel_tmp_dir + webalizer_msi);
}


void Packages::startPanelServices(bool cloud)
{
	if (!cloud) {
		utils->startService("FPanel HTTPD Service");
		utils->startService("FPanel Redis Service");
	}
	utils->startService("FPanel Agent Service");
	utils->startService("FPanel Core Service");
};

void Packages::initSystem(bool mysql_install, bool cloud, std::string main_ip) {
	if (mysql_install) {
		initMySQL(cloud, main_ip);
	}
	if (!cloud) {
		initRedis();
	}
	startPanelServices(cloud);
	initSystemEnvironments();
	initServices();
	allowPorts();
}

void Packages::setSystemPackages(std::string ftp, std::string dns, std::string smtp, std::string databases, std::string statistics, std::string spam)
{
	std::string content = "";
	content += "FTP_SERVER=iis\n";
	content += "HTTPD_SERVER=iis\n";
	content += "DNS_SERVER=" + dns + "\n";
	content += "SMTP_SERVER=" + smtp + "\n";
	content += "IMAP_SERVER=" + smtp + "\n";
	content += "DATABASE_SERVERS=" + databases + "\n";
	content += "STATISTICS=" + statistics + "\n";
	content += "SPAM_SERVICE=" + spam;

	utils->writeFile(panel_dir + "\\etc\\.sys.packages", content);
}

void Packages::setTcpPort(std::string path) {
	HKEY hKey;
	std::string new_data = "1433";
	std::string value = "TcpPort";

	LPCTSTR keyPath = TEXT((LPCSTR)path.c_str());
	LSTATUS lOpenStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_ALL_ACCESS, &hKey);
	if (lOpenStatus == ERROR_SUCCESS) {
		LSTATUS lSetStatus = RegSetValueEx(hKey, (LPCSTR)value.c_str(), 0, REG_SZ, (LPBYTE)new_data.c_str(), strlen(new_data.c_str()) + 1);
		RegCloseKey(hKey);

		if (lSetStatus == ERROR_SUCCESS) {
			return;
			//SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_BLOCK, 100, NULL);
		}
	}
}

void Packages::bind()
{
	utils->downloadFile(repo_server_link, bind_msi, files[bind_msi], panel_tmp_dir + bind_msi);
	std::string command = "c:\\windows\\system32\\msiexec.exe /i " + panel_tmp_dir + bind_msi + " /qn";
	execute(command, panel_tmp_dir + bind_msi);

	utils->removeFile(panel_tmp_dir + bind_msi);
	utils->createFile(panel_dir + "\\bind\\etc\\rndc.conf");
	utils->exec((panel_dir + "\\bind\\bin\\rndc-confgen > " + panel_dir + "\\bind\\etc\\rndc.conf").c_str());
	Sleep(2000);

	std::string new_key = "";
	std::string regex_search = "secret.*\"";

	std::ifstream infile(panel_dir + "\\bind\\etc\\rndc.conf");
	std::string line;
	while (std::getline(infile, line)) {
		std::istringstream iss(line);
		int a, b;
		if (!(iss >> a >> b)) {
			continue;
		}
		else if (utils->regexMatch(line, regex_search)) {
			utils->replaceString(line, "secret", "");
			utils->replaceString(line, "\"\"", "");
			new_key = line;
		}
	}
	Sleep(1000);

	utils->replaceStringInFile(panel_dir + "\\bind\\etc\\named.conf", "4IBgzyBW84q8DxC8+vxOqA==", new_key);
	utils->createDirectory(panel_dir + "\\bind\\etc\\domains");
	Sleep(2000);

}

void Packages::bindService()
{
	utils->executeForWindows("sc.exe create \"Bind DNS Service\" binPath= \"" + panel_dir + "\\core\\services\\FpanelDns.exe \\\"" + panel_dir + "\\bind\\bin\\named.exe -f -c " + panel_dir + "\\bind\\etc\\named.conf\\\"\" type= own start= auto error= ignore", true);
}

void Packages::startBindService()
{
	utils->startService("Bind DNS Service");
}

void Packages::msdns()
{
	if (is_core) {
		std::string main_command = "start /w ocsetup DNS-Server-Full-Role ";
		utils->exec(main_command.c_str());
	}
	else {
		std::string main_command = "start /w ocsetup DNS-Server-Full-Role ";
		utils->exec(main_command.c_str());
	}
}