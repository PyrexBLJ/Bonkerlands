#include <algorithm>
#include <wchar.h>
#include "string.hpp"
#include "system/imports.hpp"
#include "symbol.hpp"

namespace libpsutil
{
	namespace string
	{
		std::string to_lower(const std::string& text)
		{
			auto output = text;

			std::transform(output.begin(), output.end(), output.begin(), [](const char input)
			{
				return static_cast<char>(std::tolower(input));
			});

			return output;
		}

		std::string to_upper(const std::string& text)
		{
			auto output = text;

			std::transform(output.begin(), output.end(), output.begin(), [](const char input)
			{
				return static_cast<char>(std::toupper(input));
			});

			return output;
		}

		bool begins_with(const std::string& text, const std::string& search)
		{
			return (text.size() >= search.size() && text.substr(0, search.size()) == search);
		}

		bool ends_with(const std::string& text, const std::string& search)
		{
			return (text.size() >= search.size() && text.substr(text.size() - search.size(), search.size()) == search);
		}

		std::vector<std::string> split(const std::string& text, char delimiter)
		{
			std::vector<std::string> out;
			std::string::size_type prev_pos = 0, pos = 0;

			while ((pos = text.find(delimiter, pos)) != std::string::npos)
			{
				std::string substring(text.substr(prev_pos, pos - prev_pos));

				out.push_back(substring);

				prev_pos = ++pos;
			}

			out.push_back(text.substr(prev_pos, pos - prev_pos));
			return out;
		}
	}
}