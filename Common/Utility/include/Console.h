#pragma once

#include <string>
#include <Windows.h>

class Console
{
protected:
	static FILE* _out;
	static FILE* _in;
	static FILE* _err;
	static bool _opened;

public:
	static bool Open();
	static void Close();

	static void Write(const std::string& msg);
	static void Write(const char* msg);
	static void WriteLine(const std::string& msg);
	static void WriteLine(const char* msg);
	static void WriteFormat(const char* format, ...);
};

