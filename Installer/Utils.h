#pragma once
#ifndef INSTALLER_UTILS_H
#define INSTALLER_UTILS_H
#include <iostream>
#include <string>
#include <memory>
#include <cstdio>
#include <stdexcept>
#include <array>
#include <vector>
#include <fstream>
#include "Logger.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/foreach.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/regex.hpp>
#include <boost/asio/ip/host_name.hpp>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <experimental/filesystem>
#include <iterator>
#include <regex>
#include <intrin.h>
#include <atlstr.h>
#include <sstream>
#include <algorithm> 
#include <cctype>
#include <urlmon.h>
#include <Wincrypt.h>
#include <stdio.h>
#include "FileMd5.h"
# if defined(_WINSOCKAPI_) && !defined(_WINSOCK2API_)
#  error WinSock.h has already been included
# endif // defined(_WINSOCKAPI_) && !defined(_WINSOCK2API_)
#pragma comment(lib, "urlmon.lib")
#include <atlbase.h>
#include <atlsecurity.h>
#include <stdlib.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <Winreg.h>
#include <Shlobj.h>
#include "Utf16.h"
#include <random>
#include <functional>
#include <Shellapi.h>
#include <atlconv.h>
#include <filesystem>

#define BUFSIZE 1024
#define MD5LEN  16
#define INFO_BUFFER_SIZE 32767

#define DEBUG true
#define DELIM "."
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

using boost::asio::ip::tcp;
using namespace boost::asio;

static enum color { DARKBLUE = 1, DARKGREEN, DARKTEAL, DARKRED, DARKPINK, DARKYELLOW, GRAY, DARKGRAY, BLUE, GREEN, TEAL, RED, PINK, YELLOW, WHITE };

struct setColor
{
	color _c;
	HANDLE _console_handle;
	setColor(color c, HANDLE console_handle) : _c(c), _console_handle(0) {
		_console_handle = console_handle;
	}
};

std::basic_ostream<char> &operator<<(std::basic_ostream<char> &s, const setColor &ref)
{
	SetConsoleTextAttribute(ref._console_handle, ref._c);
	return s;
}

class Utils
{
private:

	std::string systemDirectory;

	void InitializeInstanceFields() {
		char *sysDrive = getenv("SystemDrive");
		if (sysDrive == NULL) {
			systemDirectory = "c:";
		}
		else {
			systemDirectory = std::string(sysDrive);
		}
	}

	std::wstring s2ws(const std::string& s) {
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}

public:
	bool debug;
	bool executed;

	FileLogger *logger;
	static enum colors { DARKBLUE = 1, DARKGREEN, DARKTEAL, DARKRED, DARKPINK, DARKYELLOW, GRAY, DARKGRAY, BLUE, GREEN, TEAL, RED, PINK, YELLOW, WHITE };

	Utils() {
		InitializeInstanceFields();
	}

	void setDebug(bool _debug)
	{
		debug = _debug;
	}

	void setExec(bool _exec)
	{
		executed = _exec;
	}

	void setLogger(FileLogger *_logger)
	{
		logger = _logger;
	}

	std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
	{
		str.erase(0, str.find_first_not_of(chars));
		return str;
	}

	std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
	{
		str.erase(str.find_last_not_of(chars) + 1);
		return str;
	}

	std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
	{
		return ltrim(rtrim(str, chars), chars);
	}

	void RemoveLastChar(std::string &str)
	{
		trim(str);
		str = str.substr(0, str.size() - 1);
	}

	void consoleWrite(std::string message, int color, bool end)
	{
		HANDLE chandle = GetStdHandle(STD_OUTPUT_HANDLE);
		switch (color) {
			case colors::DARKBLUE:
				if (end)
					std::cout << setColor(color::DARKBLUE, chandle) << message << setColor(color::GRAY, chandle) << std::endl;
				else
					std::cout << setColor(color::DARKBLUE, chandle) << message << setColor(color::GRAY, chandle);
				break;
			case colors::DARKGRAY:
				if (end)
					std::cout << setColor(color::DARKGRAY, chandle) << message << setColor(color::GRAY, chandle) << std::endl;
				else
					std::cout << setColor(color::DARKGRAY, chandle) << message << setColor(color::GRAY, chandle);
				break;
			case colors::DARKGREEN:
				if (end)
					std::cout << setColor(color::DARKGREEN, chandle) << message << setColor(color::GRAY, chandle) << std::endl;
				else
					std::cout << setColor(color::DARKGREEN, chandle) << message << setColor(color::GRAY, chandle);
				break;
			case colors::DARKPINK:
				if (end)
					std::cout << setColor(color::DARKPINK, chandle) << message << setColor(color::GRAY, chandle) << std::endl;
				else
					std::cout << setColor(color::DARKPINK, chandle) << message << setColor(color::GRAY, chandle);
				break;
			case colors::DARKRED:
				if (end)
					std::cout << setColor(color::DARKRED, chandle) << message << setColor(color::GRAY, chandle) << std::endl;
				else
					std::cout << setColor(color::DARKRED, chandle) << message << setColor(color::GRAY, chandle);
				break;
			case colors::DARKTEAL:
				if (end)
					std::cout << setColor(color::DARKTEAL, chandle) << message << setColor(color::GRAY, chandle) << std::endl;
				else
					std::cout << setColor(color::DARKTEAL, chandle) << message << setColor(color::GRAY, chandle);
				break;
			case colors::DARKYELLOW:
				if (end)
					std::cout << setColor(color::DARKYELLOW, chandle) << message << setColor(color::GRAY, chandle) << std::endl;
				else
					std::cout << setColor(color::DARKYELLOW, chandle) << message << setColor(color::GRAY, chandle);
				break;
			case colors::BLUE:
				if (end)
					std::cout << setColor(color::BLUE, chandle) << message << setColor(color::GRAY, chandle) << std::endl;
				else
					std::cout << setColor(color::BLUE, chandle) << message << setColor(color::GRAY, chandle);
				break;
			case colors::GRAY:
				if (end)
					std::cout << setColor(color::GRAY, chandle) << message << setColor(color::GRAY, chandle) << std::endl;
				else
					std::cout << setColor(color::GRAY, chandle) << message << setColor(color::GRAY, chandle);
				break;
			case colors::GREEN:
				if (end)
					std::cout << setColor(color::GREEN, chandle) << message << setColor(color::GRAY, chandle) << std::endl;
				else
					std::cout << setColor(color::GREEN, chandle) << message << setColor(color::GRAY, chandle);
				break;
			case colors::PINK:
				if (end)
					std::cout << setColor(color::PINK, chandle) << message << setColor(color::GRAY, chandle) << std::endl;
				else
					std::cout << setColor(color::PINK, chandle) << message << setColor(color::GRAY, chandle);
				break;
			case colors::RED:
				if (end)
					std::cout << setColor(color::RED, chandle) << message << setColor(color::GRAY, chandle) << std::endl;
				else
					std::cout << setColor(color::RED, chandle) << message << setColor(color::GRAY, chandle);
				break;
			case colors::TEAL:
				if (end)
					std::cout << setColor(color::TEAL, chandle) << message << setColor(color::GRAY, chandle) << std::endl;
				else
					std::cout << setColor(color::TEAL, chandle) << message << setColor(color::GRAY, chandle);
				break;
			case colors::YELLOW:
				if (end)
					std::cout << setColor(color::YELLOW, chandle) << message << setColor(color::GRAY, chandle) << std::endl;
				else
					std::cout << setColor(color::YELLOW, chandle) << message << setColor(color::GRAY, chandle);
				break;
			default:
				break;
		}
		std::cout << setColor(color::GRAY, chandle);
	}

	std::string getCurrentPath()
	{
		char buf[256];
		GetCurrentDirectoryA(256, buf);
		return std::string(buf) + '\\';
	}

	std::wstring stringToWideString(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}

	void removeFolder(std::string path)
	{
		std::experimental::filesystem::remove_all(path.c_str());
	}

	void displayError(LPCTSTR errorDesc, DWORD errorCode)
	{
		TCHAR errorMessage[1024] = TEXT("");

		DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_IGNORE_INSERTS
			| FORMAT_MESSAGE_MAX_WIDTH_MASK;

		FormatMessage(flags,
			NULL,
			errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			errorMessage,
			sizeof(errorMessage) / sizeof(TCHAR),
			NULL);

#ifdef _UNICODE
		wcerr << L"Error : " << errorDesc << endl;
		wcerr << L"Code    = " << errorCode << endl;
		wcerr << L"Message = " << errorMessage << endl;
#else
		std::cerr << "Error : " << errorDesc << std::endl;
		std::cerr << "Code    = " << errorCode << std::endl;
		std::cerr << "Message = " << errorMessage << std::endl;
#endif
	}

	int randInt(int min, int max)
	{
		return min + (rand() % static_cast<int>(max - min + 1));
	}

	std::wstring utf8ToUtf16(const std::string& utf8)
	{
		std::vector<unsigned long> unicode;
		size_t i = 0;
		while (i < utf8.size()) {
			unsigned long uni;
			size_t todo;
			bool error = false;
			unsigned char ch = utf8[i++];
			if (ch <= 0x7F) {
				uni = ch;
				todo = 0;
			}
			else if (ch <= 0xBF) {
				throw std::logic_error("not a UTF-8 string");
			}
			else if (ch <= 0xDF) {
				uni = ch & 0x1F;
				todo = 1;
			}
			else if (ch <= 0xEF) {
				uni = ch & 0x0F;
				todo = 2;
			}
			else if (ch <= 0xF7) {
				uni = ch & 0x07;
				todo = 3;
			}
			else {
				throw std::logic_error("not a UTF-8 string");
			}
			for (size_t j = 0; j < todo; ++j) {
				if (i == utf8.size())
					throw std::logic_error("not a UTF-8 string");
				unsigned char ch = utf8[i++];
				if (ch < 0x80 || ch > 0xBF)
					throw std::logic_error("not a UTF-8 string");
				uni <<= 6;
				uni += ch & 0x3F;
			}
			if (uni >= 0xD800 && uni <= 0xDFFF)
				throw std::logic_error("not a UTF-8 string");
			if (uni > 0x10FFFF)
				throw std::logic_error("not a UTF-8 string");
			unicode.push_back(uni);
		}
		std::wstring utf16;
		for (size_t i = 0; i < unicode.size(); ++i) {
			unsigned long uni = unicode[i];
			if (uni <= 0xFFFF) {
				utf16 += (wchar_t)uni;
			}
			else {
				uni -= 0x10000;
				utf16 += (wchar_t)((uni >> 10) + 0xD800);
				utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
			}
		}
		return utf16;
	}

	std::string genRandom(size_t length, std::mt19937& mt)
	{
		std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
		std::uniform_int_distribution<int> dist(0, characters.length() - 1);
		std::string randStr;
		for (size_t i = 0; i < length; ++i) {
			randStr += characters[dist(mt)];
		}
		return randStr;
	}

	int validDigit(char *ip_str)
	{
		while (*ip_str) {
			if (*ip_str >= '0' && *ip_str <= '9')
				++ip_str;
			else
				return 0;
		}
		return 1;
	}

	bool startsWith(const char *a, const char *b)
	{
		if (strncmp(a, b, strlen(b)) == 0) return 1;
		return 0;
	}

	bool isContains(std::string search, std::string needle)
	{
		if (search.find(needle) != std::string::npos) {
			return true;
		}
		return false;
	}

	std::string getfileContent(std::string file_path)
	{
		std::ifstream myReadFile;
		myReadFile.open(file_path);
		std::string line;
		std::string output;
		if (myReadFile.is_open()) {
			while (!myReadFile.eof()) {
				myReadFile >> line;
				output.append(line);
				output.append("\n");
			}
		}
		myReadFile.close();
		return output;
	}

	bool replaceString(std::string& str, const std::string& oldStr, const std::string& newStr)
	{
		size_t start_pos = str.find(oldStr);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, oldStr.length(), newStr);
		return true;
	}

	bool regexMatch(const std::string input, const std::string expression)
	{
		using namespace std::literals::string_literals;
		std::regex base_regex(expression);
		std::smatch base_match;
		std::string matched;
		if (std::regex_search(input, base_match, base_regex)) {
			if (base_match.size() == 2) {
				return true;
			}
		}
		return false;
	}

	bool isRegexMatch(const std::string input, const std::regex expression)
	{
		using namespace std::literals::string_literals;
		std::smatch base_match;
		if (std::regex_search(input, base_match, expression)) {
			return true;
		}
		return false;
	}

	std::string createPassword(std::size_t length)
	{
		std::mt19937 mt(std::random_device{}());
		return genRandom(length, mt);
	}

	std::string exec(const char* cmd)
	{
		FILE* pipe = _popen(cmd, "r");
		if (!pipe) return "ERROR";
		char buffer[262144];
		std::string data;
		std::string result;
		int dist = 0;
		int size;
		while (!feof(pipe)) {
			size = (int)fread(buffer, 1, 262144, pipe);
			data.resize(data.size() + size);
			memcpy(&data[dist], buffer, size);
			dist += size;
		}
		_pclose(pipe);
		return data;
	}

	void secEditInit()
	{

		std::string temp_file = "c:\\Windows\\temp\\sec.tmp";
		if (isFileExists(boost::filesystem::path(temp_file)))
			removeFile(temp_file);
		createFile(temp_file);
		std::string command = "c:\\Windows\\system32\\secedit.exe /export /cfg " + temp_file + " /quiet";
		exec(command.c_str());
		replaceStringInFile(temp_file, "PasswordComplexity = 1", "PasswordComplexity = 0");

		std::string temp_file_orig = temp_file + "_tmp";
		if (isFileExists(boost::filesystem::path(temp_file_orig)))
			removeFile(temp_file_orig);
		createWindowsFile(temp_file_orig);
		std::ifstream ifs(temp_file);
		std::string str;
		int i = 0;
		while (getline(ifs, str)) {
			if (i == 0) {
				i++;
				continue;
			}
			str.append("\n");
			std::wstring widestr = std::wstring(str.begin(), str.end());
			appendWindowsFile(temp_file_orig, widestr);
		}
		ifs.close();
		removeFile(temp_file);
		copyFile(temp_file_orig, temp_file);
		removeFile(temp_file_orig);

		std::string command2 = "c:\\Windows\\system32\\secedit.exe /configure /db secedit.sdb /cfg " + temp_file;
		exec(command2.c_str());

		if (isFileExists(boost::filesystem::path(temp_file)))
			removeFile(temp_file);

		removeFile("secedit.jfm");
		removeFile("secedit.sdb");
	}

	bool replaceStringInFile(std::string file_path, std::string search, std::string replace)
	{
		std::string file_tmp = file_path + ".orig";
		createFile(file_tmp);
		std::string output;
		std::ifstream ifs(file_path);
		std::string s;
		while (getline(ifs, s)) {
			replaceString(s, search, replace);
			appendFile(file_tmp, s);
		}
		ifs.close();
		removeFile(file_path);
		copyFile(file_tmp, file_path);
		removeFile(file_tmp);
		return true;
	}

	bool replaceStringInFileWithRegex(std::string file_path, std::string search, std::string replace)
	{
		std::string file_tmp = file_path + ".orig";
		createFile(file_tmp);
		std::string output;
		std::ifstream in(file_path);
		std::ofstream out;
		out.open(file_tmp, std::ios_base::app);



		if (!in) {
			logger->errorLog("Could not open " + file_path, 1002);
			return false;
		}

		std::string line;
		while (getline(in, line)) {
			if (isRegexMatch(line, std::regex(search))) {
				line = replace;
			}
			//std::cout << line << std::endl;
			out << line.c_str();
			out << "\n";
		}
		in.close();
		out.close();
		Sleep(500);
		removeFile(file_path);
		Sleep(500);
		rename(file_tmp.c_str(), file_path.c_str());
		return true;
	}

	bool removeFile(std::string file_path)
	{
		if (isFileExists(file_path)) {
			if (remove(file_path.c_str()) != 0) {
				logger->errorLog("Could not remove " + file_path, 1004);
				return false;
			}
		}
		return true;
	}

	bool isFileExists(boost::filesystem::path file_path)
	{
		if (boost::filesystem::exists(file_path)) {
			if (boost::filesystem::is_directory(file_path))
				return false;
			else if (boost::filesystem::is_regular_file(file_path))
				return true;
			else
				return false;
		}
		return false;
	}

	bool appendFile(std::string file_path, std::string data)
	{
		std::ofstream myfile;
		myfile.open(file_path, std::ios_base::app);
		if (myfile.is_open()) {
			myfile << data.c_str();
			myfile << "\n";
			myfile.close();
		}
		return true;
	}

	bool appendWindowsFile(std::string file_path, std::wstring w_data)
	{
		const wchar_t* widecstr = w_data.c_str();
		UTF16Facet *unicodeFacet = new UTF16Facet();
		const std::locale unicodeLocale(std::cout.getloc(), unicodeFacet);
		FILE* outFile = fopen(file_path.c_str(), "a+,ccs=UTF-16LE");
		fwrite(widecstr, wcslen(widecstr) * sizeof(wchar_t), 1, outFile);
		fclose(outFile);
		return true;
	}

	bool writeFile(std::string file_path, std::string data)
	{
		if (isFileExists(file_path)) {
			std::ofstream myfile(file_path, std::ofstream::out | std::ofstream::trunc);
			if (myfile.is_open()) {
				myfile << data.c_str();
				myfile.close();
			}
		}
		else {
			std::ofstream myfile(file_path);
			if (myfile.is_open()) {
				myfile << data.c_str();
				myfile.close();
			}
		}

		return true;
	}

	std::string defaultIpAddress()
	{
		std::string ip_lists = exec("c:\\windows\\system32\\route.exe PRINT 0.0.0.0 -4 | find \"0.0.0.0\" | findstr /V \"Default\"");
		std::string default_ip = "";
		wchar_t charsToTrim[3] = { '*', ' ', '\'' };
		std::string expression("0.0.0.0(.*)0.0.0.0");
		int i = 0;

		std::istringstream list_stream(ip_lists);
		std::string line;
		while (std::getline(list_stream, line)) {
			if (line != "") {
				if (regexMatch(line, expression) == true) {
					if (i == 0) {
						typedef std::vector<std::string> Tokens;
						Tokens tokens;
						boost::split(tokens, line, boost::is_any_of(" "));
						int k = 0;
						BOOST_FOREACH(const std::string& data, tokens)
						{
							if (data != "") {
								if (k == 3) {
									if (validateIpAddress(data.c_str())) {
										default_ip = data;
										trim(std::string(default_ip));
									}
								}
								k++;
							}
						}
						i++;
					}
				}
			}
		}
		return std::string(default_ip);
	}

	bool copyFile(std::string file_path, std::string new_path)
	{
		std::ifstream src(file_path, std::ios::binary);
		std::ofstream dst(new_path, std::ios::binary);
		dst << src.rdbuf();
		return true;
	}

	bool startService(std::string serviceName)
	{
		SC_HANDLE serviceDbHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		SC_HANDLE serviceHandle = OpenService(serviceDbHandle, serviceName.c_str(), SC_MANAGER_ALL_ACCESS);

		SERVICE_STATUS_PROCESS status;
		DWORD bytesNeeded;
		QueryServiceStatusEx(serviceHandle, SC_STATUS_PROCESS_INFO, (LPBYTE)&status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded);
		BOOL serviceControl;
		if (status.dwCurrentState == SERVICE_RUNNING) {
			stopService(serviceName);
		}
		serviceControl = StartService(serviceHandle, NULL, NULL);
		if (serviceControl) {
			return true;
		}
		else {
			logger->errorLog("Service failed to start. ServiceName: " + serviceName, 1002);
		}
		CloseServiceHandle(serviceHandle);
		CloseServiceHandle(serviceDbHandle);
		return true;
	}

	bool stopService(std::string serviceName)
	{
		SC_HANDLE serviceDbHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		SC_HANDLE serviceHandle = OpenService(serviceDbHandle, serviceName.c_str(), SC_MANAGER_ALL_ACCESS);

		SERVICE_STATUS_PROCESS status;
		DWORD bytesNeeded;
		QueryServiceStatusEx(serviceHandle, SC_STATUS_PROCESS_INFO, (LPBYTE)&status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded);
		BOOL serviceControl;
		if (status.dwCurrentState == SERVICE_RUNNING) {
			serviceControl = ControlService(serviceHandle, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&status);
			if (serviceControl) {
				return true;
			}
			else {
				logger->errorLog("Service failed to stop. ServiceName: " + serviceName, 1002);
			}
		}


		CloseServiceHandle(serviceHandle);
		CloseServiceHandle(serviceDbHandle);
		return true;
	}

	bool validateIpAddress(const std::string ip_address)
	{
		return isRegexMatch(ip_address, std::regex("(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})"));
	}

	bool validateHostname(const std::string hostname)
	{
		boost::regex re;

		try {
			re.assign("^[a-z0-9]+([\-\.]{1}[a-z0-9]+)*\.[a-z]{2,6}$", boost::regex_constants::icase);
		}
		catch (...) {
			return false;
		}
		if (boost::regex_match(hostname, re)) {
			return true;
		}
		return false;
	}

	bool isAdmin()
	{
		BOOL b;
		SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
		PSID AdministratorsGroup;
		b = AllocateAndInitializeSid(
			&NtAuthority,
			2,
			SECURITY_BUILTIN_DOMAIN_RID,
			DOMAIN_ALIAS_RID_ADMINS,
			0, 0, 0, 0, 0, 0,
			&AdministratorsGroup);
		if (b) {
			if (!CheckTokenMembership(NULL, AdministratorsGroup, &b)) {
				b = FALSE;
			}
			FreeSid(AdministratorsGroup);
		}
		return(b);
	}

	void getSizeOfTerminal(int *columns, int *rows)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		*columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		*rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	}

	void SetWindow(int Width, int Height, int WidthBuffer, int HeightBuffer)
	{
		_COORD coord;
		coord.X = WidthBuffer;
		coord.Y = HeightBuffer;

		_SMALL_RECT Rect;
		Rect.Top = 0;
		Rect.Left = 0;
		Rect.Bottom = Height - 1;
		Rect.Right = Width - 1;

		HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle 
		SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size 
		SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size 
	}

	void showConsoleCursor(bool showFlag)
	{
		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO info;
		info.dwSize = 100;
		info.bVisible = showFlag;
		SetConsoleCursorInfo(consoleHandle, &info);
	}

	void killProcessByName(const char *processName)
	{
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
		PROCESSENTRY32 pEntry;
		pEntry.dwSize = sizeof(pEntry);
		BOOL hRes = Process32First(hSnapShot, &pEntry);
		while (hRes) {
			if (strcmp(pEntry.szExeFile, processName) == 0) {
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
					(DWORD)pEntry.th32ProcessID);
				if (hProcess != NULL) {
					TerminateProcess(hProcess, 9);
					CloseHandle(hProcess);
				}
			}
			hRes = Process32Next(hSnapShot, &pEntry);
		}
		CloseHandle(hSnapShot);
	}

	bool IsProcessRunning(const char *processName)
	{
		TCHAR const * procName = TEXT(processName);

		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);

		const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (!Process32First(snapshot, &entry)) {
			CloseHandle(snapshot);
			return false;
		}

		do {
			if (!_tcsicmp(entry.szExeFile, procName)) {
				CloseHandle(snapshot);
				return true;
			}
		} while (Process32Next(snapshot, &entry));

		CloseHandle(snapshot);
		return false;
	}

	char *stringToChar(std::string str)
	{
		char * writable = new char[str.size() + 1];
		std::copy(str.begin(), str.end(), writable);
		writable[str.size()] = '\0'; // don't forget the terminating 0
		char *result = writable;
		delete[] writable;
		return result;
	}

	HKEY openKey(HKEY hRootKey, char* strKey)
	{
		HKEY hKey;
		LONG nError = RegOpenKeyEx(hRootKey, strKey, NULL, KEY_ALL_ACCESS, &hKey);

		if (nError == ERROR_FILE_NOT_FOUND) {
			nError = RegCreateKeyEx(hRootKey, strKey, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
		}

		if (nError) {
			logger->errorLog(("Could not find or create %s", strKey), 1002);
			return NULL;
		}

		return hKey;
	}

	bool setRegDword(HKEY hKey, char* subkey, char* StringName, DWORD dword_value)
	{
		RegOpenKeyEx(hKey, subkey, 0, KEY_ALL_ACCESS, &hKey);
		RegSetValueEx(hKey, TEXT(StringName), 0, REG_DWORD, (const BYTE*)&dword_value, sizeof(dword_value));
		RegCloseKey(hKey);
		return true;
	}

	bool setRegVal(HKEY hKey, char* subkey, char* StringName, char* Stringdata)
	{
		if (RegSetValueEx(hKey, TEXT(StringName), 0, REG_SZ, (LPBYTE)Stringdata, strlen(Stringdata) * sizeof(char)) != ERROR_SUCCESS) {
			RegCloseKey(hKey);
			return false;
		}
		else {
			return true;
		}
	}

	long getStringRegKey(HKEY hKey, const std::wstring &strValueName, std::wstring &strValue, const std::wstring &strDefaultValue)
	{
		strValue = strDefaultValue;
		WCHAR szBuffer[512];
		DWORD dwBufferSize = sizeof(szBuffer);
		ULONG nError;
		nError = RegQueryValueExW(hKey, strValueName.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
		if (ERROR_SUCCESS == nError) {
			strValue = szBuffer;
		}
		return nError;
	}

	bool setPermanentEnvironmentVariable(std::string value, std::string data)
	{
		HKEY hKey;
		LONG lRes = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_READ, &hKey);
		std::wstring strKeyDefaultValue;
		std::wstring path = stringToWideString(value);
		getStringRegKey(hKey, path, strKeyDefaultValue, L"NOT_EXISTS");

		//std::wcout << strKeyDefaultValue << std::endl;
		setlocale(LC_CTYPE, "");
		std::string newRegVal;

		const std::string s(strKeyDefaultValue.begin(), strKeyDefaultValue.end());
		std::stringstream ss;
		ss << s.c_str();
		newRegVal = ss.str();
		std::string new_data;
		if (newRegVal == "NOT_EXISTS") {
			new_data = data;
		}
		else {
			new_data = newRegVal + data;
		}

		LPCTSTR keyPath = TEXT("System\\CurrentControlSet\\Control\\Session Manager\\Environment");
		LSTATUS lOpenStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_ALL_ACCESS, &hKey);
		if (lOpenStatus == ERROR_SUCCESS) {
			LSTATUS lSetStatus = RegSetValueEx(hKey, (LPCSTR)value.c_str(), 0, REG_SZ, (LPBYTE)new_data.c_str(), strlen(new_data.c_str()) + 1);
			RegCloseKey(hKey);

			if (lSetStatus == ERROR_SUCCESS) {
				SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_BLOCK, 100, NULL);
				return true;
			}
		}

		return false;
	}

	bool createDirectory(std::string folder_path)
	{
		return SHCreateDirectoryEx(NULL, folder_path.c_str(), NULL);
	}

	bool createFile(std::string file_path)
	{
		boost::filesystem::ofstream(file_path.c_str());
		return true;
	}

	bool createWindowsFile(std::string file_path)
	{
		wchar_t* x = L"";
		FILE* outFile = fopen(file_path.c_str(), "w+,ccs=UTF-16LE");
		fwrite(x, wcslen(x) * sizeof(wchar_t), 1, outFile);
		fclose(outFile);
		return true;
	}

	std::vector<std::string> getFolderList(std::string folder)
	{
		std::vector<std::string> names;
		std::string search_path = folder + "\\*";

		boost::filesystem::directory_iterator end_itr;
		for (boost::filesystem::directory_iterator itr(folder); itr != end_itr; ++itr) {
			if (is_directory(itr->status())) {
				std::ostringstream oss;
				oss << *itr;
				std::string path = oss.str();
				names.push_back(path);
			}
		}
		return names;
	}

	bool executeForWindows(std::string command, bool wait)
	{
		PROCESS_INFORMATION processInfo;
		STARTUPINFO startupInfo;
		LPSTR cmdArgs = const_cast<char *>(command.c_str());

		ZeroMemory(&startupInfo, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);
		ZeroMemory(&processInfo, sizeof(processInfo));
		startupInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
		startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
		startupInfo.wShowWindow = SW_HIDE;

		if (CreateProcess(NULL, cmdArgs, NULL, NULL, false, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInfo)) {
			if (wait) {
				WaitForSingleObject(processInfo.hProcess, INFINITE);
				CloseHandle(processInfo.hThread);
				CloseHandle(processInfo.hProcess);
			}
			return true;
		}
		else {
			DWORD errorCode = GetLastError();
			logger->errorLog("Could not execute " + command, 1002);

			//displayError(TEXT("Unable to execute."), errorCode);
		}
		return false;
	}

	bool copyDirTo(std::string source_folder, std::string target_folder)
	{
		std::wstring wsource_folder(source_folder.begin(), source_folder.end());
		std::wstring wtarget_folder(target_folder.begin(), target_folder.end());

		std::wstring new_sf = wsource_folder + L"\\*";
		WCHAR sf[MAX_PATH + 1];
		WCHAR tf[MAX_PATH + 1];

		wcscpy_s(sf, MAX_PATH, new_sf.c_str());
		wcscpy_s(tf, MAX_PATH, wtarget_folder.c_str());

		sf[lstrlenW(sf) + 1] = 0;
		tf[lstrlenW(tf) + 1] = 0;

		SHFILEOPSTRUCTW s = { 0 };
		s.wFunc = FO_COPY;
		s.pTo = tf;
		s.pFrom = sf;
		s.fFlags = FOF_SILENT | FOF_NOCONFIRMMKDIR | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NO_UI;
		int res = SHFileOperationW(&s);

		return res == 0;
	}

	std::vector<std::string> strSplit(std::string str, char delimeter)
	{
		std::stringstream ss(str);
		std::string item;
		std::vector<std::string> splittedStrings;
		while (std::getline(ss, item, delimeter)) {
			splittedStrings.push_back(std::string(item));
		}
		return splittedStrings;
	}

	bool setHostname(std::string hostname, std::string domain)
	{
		std::string command = "c:\\fpanel\\core\\bin\\networkmng --hostname --host=" + hostname + " --domain=" + domain;
		executeForWindows(command, true);
		return true;
	}

	void initAdmin()
	{
		//createFile("c:/fpanel/core/bin/debug");
		std::string command = "c:/fpanel/core/bin/adminmng --init";
		executeForWindows(command, true);
	}

	std::string md5Checksum(std::string file_path)
	{
		MD5 md5;
		char *cstr = new char[file_path.length() + 1];
		strcpy(cstr, file_path.c_str());
		char *result = md5.digestFile(cstr);
		delete[] cstr;
		return std::string(result);
	}

	std::string getPackageList(const std::string &repo_address)
	{
		std::string packages_list;
		try {
			boost::asio::io_service io_service;
			// Get a list of endpoints corresponding to the server name.
			tcp::resolver resolver(io_service);
			boost::asio::ip::tcp::resolver::query query(repo_address, "80");

			tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
			tcp::resolver::iterator end;
			// Try each endpoint until we successfully establish a connection.
			tcp::socket socket(io_service);
			boost::system::error_code error = boost::asio::error::host_not_found;
			while (error && endpoint_iterator != end) {
				socket.close();
				socket.connect(*endpoint_iterator++, error);
			}
			if (error)
				throw boost::system::system_error(error);
			// Form the request. We specify the "Connection: close" header so that the
			// server will close the socket after transmitting the response. This will
			// allow us to treat all data up until the EOF as the content.
			boost::asio::streambuf request;
			std::ostream request_stream(&request);
			request_stream << "GET " << "/windows/" << "packages.list" << " HTTP/1.0\r\n";
			request_stream << "Host: " << repo_address << "\r\n";
			request_stream << "Accept: */*\r\n";
			request_stream << "Connection: close\r\n\r\n";
			// Send the request.
			boost::asio::write(socket, request);
			// Read the response status line.
			boost::asio::streambuf response;
			boost::asio::read_until(socket, response, "\r\n");
			// Check that response is OK.
			std::istream response_stream(&response);
			std::string http_version;
			response_stream >> http_version;
			unsigned int status_code;
			response_stream >> status_code;
			std::string status_message;
			std::getline(response_stream, status_message);
			if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
				logger->errorLog("Invalid response", 999);
				return {};
			}
			if (status_code != 200) {
				logger->errorLog("Response returned with status code", status_code);
				return {};
			}

			// Read the response headers, which are terminated by a blank line.
			boost::asio::read_until(socket, response, "\r\n\r\n");

			std::string header;
			while (std::getline(response_stream, header) && header != "\r") {
			}

			if (response.size() > 0) {
				std::istreambuf_iterator<char> eosf;
				std::string s(std::istreambuf_iterator<char>(&response), eosf);
				packages_list += s;
			}
			// Read until EOF, writing data to output as we go.
			while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) {
				std::istreambuf_iterator<char> eoss;
				std::string s(std::istreambuf_iterator<char>(&response), eoss);
				packages_list += s;
			}
		}
		catch (std::exception &e) {
			logger->errorLog("Exception " + std::string(e.what()), 999);
		}

		return packages_list;
	}

	void printProgress(double percentage)
	{
		if (percentage >= 0.99) {
			percentage = 1;
		}
		int val = (int)(percentage * 100);
		int lpad = (int)(percentage * PBWIDTH);
		int rpad = PBWIDTH - lpad;

		printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
		fflush(stdout);
	}

	bool downloadFile(const std::string &repo_address, const std::string &package, const std::string &file_md5_sum, const std::string &target)
	{
		std::string fpanel_repo_path = "/windows/";
		if (debug) {
			logger->debugLog("File downloading: " + repo_address + fpanel_repo_path + package);
		}

		std::ofstream outFile(target, std::ofstream::out | std::ofstream::binary);
		boost::asio::io_service io_service;

		// Get a list of endpoints corresponding to the server name.
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(repo_address, "http");
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		// Try each endpoint until we successfully establish a connection.
		tcp::socket socket(io_service);
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end) {
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}


		if (error)
			throw boost::system::system_error(error);

		boost::asio::streambuf request;
		std::ostream request_stream(&request);

		request_stream << "GET " << fpanel_repo_path << package << " HTTP/1.1\r\n";
		request_stream << "Host: " << repo_address << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";

		// Send the request.
		boost::asio::write(socket, request);

		// Read the response status line.
		boost::asio::streambuf response;
		boost::asio::read_until(socket, response, "\r\n");

		// Check that response is OK.
		std::istream response_stream(&response);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);


		// Read the response headers, which are terminated by a blank line.
		boost::asio::read_until(socket, response, "\r\n\r\n");

		int file_size = 0;
		std::string expression("Content-Length:(.*)");

		// Process the response headers.
		std::string header;
		while (std::getline(response_stream, header) && header != "\r") {
			if (regexMatch(header, expression) == true) {
				replaceString(header, "Content-Length:", "");
				trim(header);
				file_size = std::stoi(header);
			}
		}

		// Write whatever content we already have to output.
		if (response.size() > 0) {
			outFile << &response;
		}
		//std::cout << "Size: " << file_size << std::endl;
		// Read until EOF, writing data to output as we go.
		double total_downloaded = 0;
		while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) {
			if (error) {
				logger->errorLog("Read downloaded package error: " + std::string(error.message()), 999);
				return false;
			}
			if (debug && !executed) {
				total_downloaded += (&response)->size();
				printProgress(total_downloaded / file_size);
			}

			//std::cout << "Response Size: " << (&response)->size() << std::endl;

			outFile << &response;
		}

		if (debug && !executed) {
			std::cout << "\n";
		}


		outFile.close();

		//Calculate MD5 hash
		std::string md5sum = md5Checksum(target);
		if (file_md5_sum.compare(md5sum) != 0) {
			logger->errorLog("File download failed. System will trying again download as correctly. File is : " + package, 999);
			return downloadFile(repo_address, package, file_md5_sum, target);
		}

		return true;
	}
};
#endif