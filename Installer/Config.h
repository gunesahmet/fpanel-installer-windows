#pragma once
#ifndef INSTALLER_CONFIG_H
#define INSTALLER_CONFIG_H

#include <iostream>
#include <map>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>

#include <Logger.h>
#include <Ini/inireader.h>
#include <Crypto/Crypto.h>

class Config {
private:
    std::string config_location = "/etc/fpanel.conf";
    std::map<std::string, std::string> configurations;
    std::mutex conf_mutex;
public:
    Config()
    {
        configurations = std::map<std::string, std::string>();
    }

    ~Config()
    {
        configurations.clear();
    }

    void init(std::string system_type)
    {
        std::lock_guard<std::mutex> lockGuard(conf_mutex);

        configurations.empty();
        configurations.clear();
        std::string panel_directory = "c:/fpanel/";
        std::string panel_system_conf = panel_directory + "etc/system.conf";
        std::string panel_tmp_directory = panel_directory + "tmp/";
        std::string panel_template_directory = panel_directory + "core/templates/";
        std::string panel_database_admin_password = utils.GenerateHex(16);
        std::string panel_socket_key = _helpers.GenerateHex(16);
        std::string panel_socket_iv = _helpers.GenerateHex(16);
        std::string panel_sql_pass = _helpers.GenerateHex(6);

        if(!_helpers.IsDirExist(panel_directory)) {
            _helpers.CreateDirectory(panel_directory);
            _helpers.CreateDirectory(panel_tmp_directory);
        }
        std::string pkg_install_command;
        std::string pkg_remove_command;
        std::string pkg_manual_install_command;
        std::string pkg_extension;
        std::string source_list;
        std::string pkg_force_install_command;

        std::string pretty_name;
        std::string os_version;
        std::string os_id;

        std::string apache_user;
        std::string nogroup;

        bool valid = false;

        _helpers.trim(os_id);
        _helpers.trim(os_version);

        _helpers.CreateDirectory("/var/log/fpanel/");


        std::string panel_http_server = "repo.fpanel.net";
        std::string panel_distro_packages_server = "/windows/";
        std::string vhosts_dir = "c:/vhosts/";

        configurations.insert({ "panel.dir", panel_directory });
        configurations.insert({ "panel.system.conf", panel_system_conf });
        configurations.insert({ "panel.tmp.directory", panel_tmp_directory });
        configurations.insert({ "panel.template.directory", panel_template_directory });
        configurations.insert({ "panel.config.file", panel_directory + "etc/panel.conf" });
        configurations.insert({ "panel.sqlpass", panel_sql_pass });
        configurations.insert({ "panel.sqlpass.file", panel_directory + "etc/.sql_pass" });
        configurations.insert({ "panel.database.password", panel_database_admin_password });
        configurations.insert({ "panel.socket.key", panel_socket_key });
        configurations.insert({ "panel.socket.iv", panel_socket_iv });
        configurations.insert({ "vhosts.dir", vhosts_dir });

        configurations.insert({ "panel.http.server.address", panel_http_server });
        configurations.insert({ "panel.http.server.distro", panel_distro_packages_server });

        configurations.insert({ "system.os", os_id });
        configurations.insert({ "system.version", os_version });
        configurations.insert({ "system.pretty_name", pretty_name });

        configurations.insert({ "system.source.list", source_list });
        configurations.insert({ "system.package.extension", pkg_extension });
        configurations.insert({ "system.apache.user", apache_user });

        configurations.insert({ "system.install.command", pkg_install_command });
        configurations.insert({ "system.remove.command", pkg_remove_command });
        configurations.insert({ "system.manuel_install.command", pkg_manual_install_command });
        configurations.insert({ "system.force.install.command", pkg_force_install_command });

        configurations.insert({ "system.php.fpm.run_directory", php_fpm_run_directory });
        configurations.insert({ "system.php.fpm.init_file", php_fpm_init_file });


        configurations.insert({ "apache", "X" });
        configurations.insert({ "apachefpm", " " });
        configurations.insert({ "nginx", " " });
        configurations.insert({ "perl", "X" });
        configurations.insert({ "python", "X" });
        configurations.insert({ "php56", " " });
        configurations.insert({ "php72", "X" });
        configurations.insert({ "php73", " " });
        configurations.insert({ "php74", " " });
        configurations.insert({ "mysql", "X" });
        configurations.insert({ "proftpd", "X" });
        configurations.insert({ "mariadb", " " });
        configurations.insert({ "postgresql", " " });
        configurations.insert({ "bind", "X" });
        configurations.insert({ "postfix", "X" });
        configurations.insert({ "dovecot", "X" });
        configurations.insert({ "spamassassin", " " });
        configurations.insert({ "mailman", " " });
        configurations.insert({ "roundcube", "X" });
        configurations.insert({ "dbmyadmin", "X" });
        configurations.insert({ "dbpgadmin", " " });
        configurations.insert({ "awstats", "X" });
        configurations.insert({ "webalizer", "X" });
        configurations.insert({ "php", "X" });
        configurations.insert({ "hostname", "" });

    }

    void setValue(std::string key, std::string val) {
        configurations[key] = val;
    }

    std::map<std::string, std::string> getConfigs()
    {
        std::lock_guard<std::mutex> lockGuard(conf_mutex);
        return configurations;
    }

    std::string getValue(std::string key)
    {
        std::lock_guard<std::mutex> lockGuard(conf_mutex);
        return configurations.find(key)->second;
    }
};


#endif //FPANEL_CORE_CONFIG_H
