#include "../include/Console.h"

#include <stdio.h>
#include <io.h>

FILE* Console::_in = 0;
FILE* Console::_out = 0;
FILE* Console::_err = 0;
bool Console::_opened = false;

bool Console::Open()
{
	_opened = (AllocConsole() != 0);
	freopen_s(&_in, "CONIN$", "r", stdin);
	freopen_s(&_out, "CONOUT$", "w", stdout);
	freopen_s(&_err, "CONOUT$", "w", stderr);

	return true;
}

void Console::Close()
{
	if (_opened)
	{
		fclose(_in);
		fclose(_out);
		fclose(_err);

		FreeConsole();
	}
	_opened = false;
}

void Console::Write(const std::string& msg)
{
	if (_opened)
	{
		printf("%s", msg.c_str());
	}
}

void Console::Write(const char* msg)
{
	if (_opened)
	{
		printf("%s", msg);
	}
}

void Console::WriteLine(const std::string& msg)
{
	if (_opened)
	{
		printf("%s\n", msg.c_str());
	}
}

void Console::WriteLine(const char * msg)
{
	if (_opened)
	{
		printf("%s\n", msg);
	}
}

void Console::WriteFormat(const char* format, ...)
{
	if (_opened)
	{
		va_list argptr;
		va_start(argptr, format);
		vfprintf(stderr, format, argptr);
		va_end(argptr);
	}
}
