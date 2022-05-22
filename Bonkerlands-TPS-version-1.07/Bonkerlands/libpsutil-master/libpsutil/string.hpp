#pragma once
#include <string>
#ifdef vector
#undef vector
#endif
#include <vector>
#include <symbol.hpp>
#include <system/imports.hpp>

static libpsutil::symbol<int(wchar_t*, char*, size_t)> mbctowc{ 0xC1B45C };

namespace libpsutil
{
	namespace string
	{
		static char va_buffer[0x1000];
		
		template<typename... Parameters>
		wchar_t* va(const char* fmt, Parameters... params)
		{
			static wchar_t buff[0x4000];

			_sys_sprintf(va_buffer, fmt, params...);
			mbctowc(buff, va_buffer, strlen(va_buffer) + 1);
			return buff;
		}

		std::string to_lower(const std::string& text);
		std::string to_upper(const std::string& text);
		bool begins_with(const std::string& text, const std::string& search);
		bool ends_with(const std::string& text, const std::string& search);
		std::vector<std::string> split(const std::string& text, char delimiter);
	};
}