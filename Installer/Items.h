#pragma once
#ifndef INSTALLER_ITEMS_H
#define INSTALLER_ITEMS_H
#include <iostream>

class Items
{
	std::string iis_status;
	std::string php_status;
	std::string perl_status;
	std::string python27_status;
	std::string python39_status;
	std::string mssql_2016_status;
	std::string mssql_2012_status;
	std::string mysql57_server;
	std::string mysql80_server;
	std::string postgresql11_status;
	std::string postgresql12_status;
	std::string postgresql13_status;
	std::string dotnet_status;
	std::string bind_status;
	std::string ms_dns_status;
	std::string mailenable_status;
	std::string mailenable_pro_status;
	std::string mailenable_ent_status;
	std::string mailenable_pre_status;
	std::string spam_assasin_stastus;
	std::string phpmyadmin49_status;
	std::string phpmyadmin50_status;
	std::string mylittleadmin_status;
	std::string old_hostname;
	std::string hmail_status;
	std::string roundcube_status;
	std::string php56_status;
	std::string php72_status;
	std::string php74_status;
	std::string awstats_status;
	std::string webalizer_status;
	std::string ip_addr;

public:
	std::string getIisStatus()
	{
		return iis_status;
	}

	void setIisStatus(std::string value)
	{
		iis_status = value;
	}

	std::string getPhpStatus()
	{
		return php_status;
	}

	void setPhpStatus(std::string value)
	{
		php_status = value;
	}

	std::string getPython27Status()
	{
		return python27_status;
	}

	void setPython27Status(std::string value)
	{
		python27_status = value;
	}

	std::string getPython39Status()
	{
		return python39_status;
	}

	void setPython39Status(std::string value)
	{
		python39_status = value;
	}

	std::string getPerlStatus()
	{
		return perl_status;
	}

	void setPerlStatus(std::string value)
	{
		perl_status = value;
	}

	std::string getMssql2k16Status()
	{
		return mssql_2016_status;
	}

	void setMssql2k16Status(std::string value)
	{
		mssql_2016_status = value;
	}
	
	std::string getMssql2k12Status()
	{
		return mssql_2012_status;
	}

	void setMssql2k12Status(std::string value)
	{
		mssql_2012_status = value;
	}

	std::string getMySql57Status()
	{
		return mysql57_server;
	}

	void setMySql57Status(std::string value)
	{
		mysql57_server = value;
	}

	std::string getMySql80Status()
	{
		return mysql80_server;
	}

	void setMySql80Status(std::string value)
	{
		mysql80_server = value;
	}

	std::string getPostgre11Status()
	{
		return postgresql11_status;
	}

	void setPostgre11Status(std::string value)
	{
		postgresql11_status = value;
	}

	std::string getPostgre12Status()
	{
		return postgresql12_status;
	}

	void setPostgre12Status(std::string value)
	{
		postgresql12_status = value;
	}

	std::string getPostgre13Status()
	{
		return postgresql13_status;
	}

	void setPostgre13Status(std::string value)
	{
		postgresql13_status = value;
	}

	std::string getNetStatus()
	{
		return dotnet_status;
	}

	void setNetStatus(std::string value)
	{
		dotnet_status = value;
	}

	std::string getBindStatus()
	{
		return bind_status;
	}

	void setBindStatus(std::string value)
	{
		bind_status = value;
	}

	std::string getMsDNSStatus()
	{
		return ms_dns_status;
	}

	void setMsDNSStatus(std::string value)
	{
		ms_dns_status = value;
	}

	std::string getMailEnableStatus()
	{
		return mailenable_status;
	}

	void setMailEnableStatus(std::string value)
	{
		mailenable_status = value;
	}

	std::string getMailEnableProStatus()
	{
		return mailenable_pro_status;
	}

	void setMailEnableProStatus(std::string value)
	{
		mailenable_pro_status = value;
	}

	std::string getMailEnableEntStatus()
	{
		return mailenable_ent_status;
	}

	void setMailEnableEntStatus(std::string value)
	{
		mailenable_ent_status = value;
	}

	std::string getMailEnablePreStatus()
	{
		return mailenable_pre_status;
	}

	void setMailEnablePreStatus(std::string value)
	{
		mailenable_pre_status = value;
	}

	std::string getSpamAssassinStatus()
	{
		return spam_assasin_stastus;
	}

	void setSpamAssassinStatus(std::string value)
	{
		spam_assasin_stastus = value;
	}

	std::string getHostname()
	{
		return old_hostname;
	}

	void setHostname(std::string value)
	{
		old_hostname = value;
	}

	std::string getMainServerIp()
	{
		return ip_addr;
	}

	void setMainServerIp(std::string _ip_addr)
	{
		ip_addr = _ip_addr;
	}

	std::string getHMailStatus() {
		return hmail_status;
	}

	void setHMailStatus(std::string value) {
		hmail_status = value;
	}

	std::string getRoundCubeStatus() {
		return roundcube_status;
	}

	void setRoundCubeStatus(std::string value) {
		roundcube_status = value;
	}

	std::string getPhp56Status() {
		return php56_status;
	}

	void setPhp56Status(std::string value) {
		php56_status = value;
	}

	std::string getPhp72Status() {
		return php72_status;
	}

	void setPhp72Status(std::string value) {
		php72_status = value;
	}

	std::string getPhp74Status() {
		return php74_status;
	}

	void setPhp74Status(std::string value) {
		php74_status = value;
	}

	std::string getphpMyAdmin49Status() {
		return phpmyadmin49_status;
	}

	void setphpMyAdmin49Status(std::string value) {
		phpmyadmin49_status = value;
	}

	std::string getphpMyAdmin50Status() {
		return phpmyadmin50_status;
	}

	void setphpMyAdmin50Status(std::string value) {
		phpmyadmin50_status = value;
	}

	std::string getMyLittleAdminStatus()
	{
		return mylittleadmin_status;
	}

	void setMyLittleAdminStatus(std::string value)
	{
		mylittleadmin_status = value;
	}

	std::string getAwstatsStatus()
	{
		return awstats_status;
	}

	void setAwstatsStatus(std::string value)
	{
		awstats_status = value;
	}

	std::string getWebalizerStatus()
	{
		return webalizer_status;
	}

	void setWebalizerStatus(std::string value)
	{
		webalizer_status = value;
	}
};
#endif