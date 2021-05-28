#include <iostream>
#include <Windows.h>
#include <string>
char _x2c(
	__in char hex_up,
	__in char hex_low
) {
	char digit;
	digit = 16 * (hex_up >= 'A' ? ((hex_up & 0xdf) - 'A') + 10 : (hex_up - '0'));
	digit += (hex_low >= 'A' ? ((hex_low & 0xdf) - 'A') + 10 : (hex_low - '0'));
	return digit;
}
DWORD url_decode(
	__out std::string& output_string,
	__in std::string& encode_string
) {
	DWORD error = ERROR_SUCCESS;
	do {
		if (encode_string.empty()) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}
		std::string temp_output_string;
		for (size_t i = 0; i < encode_string.size(); i++) {
			switch (encode_string.at(i)) {
			case '%':
			{
				temp_output_string.push_back(_x2c(encode_string.at(i + 1), encode_string.at(i + 2)));
				i += 2;
			}
			break;
			default:
			{
				temp_output_string.push_back(encode_string.at(i));
			}
			break;
			}
		}
		if (false == temp_output_string.empty()) {
			output_string.assign(temp_output_string);
		}
	} while (false);
	return error;
}
DWORD url_encode(
	__out std::string& output_string,
	__in std::string& plain_string
) {
	DWORD error = ERROR_SUCCESS;
	do {
		if (plain_string.empty()) {
			error = ERROR_INVALID_PARAMETER;
			break;
		}
		std::string temp_output_string;
		for (size_t i = 0; i < plain_string.size(); i++) {
			char ch = plain_string.at(i);
			if (isdigit(ch)) {
				temp_output_string.push_back(ch);
			}
			else if (isalpha(ch)) {
				temp_output_string.push_back(ch);
			}
			else if (ch == '$' || // Safe characters 이지만 모두 encoding 해도 무관함.
				ch == '-' ||
				ch == '_' ||
				ch == '.' ||
				ch == '+' ||
				ch == '!' ||
				ch == '*' ||
				ch == '\'' ||
				ch == '(' ||
				ch == ')' ||
				ch == ',') {
				temp_output_string.push_back(ch);
			}
			else {
				char temp[3] = "";
				sprintf_s(temp, "%02x", ch);
				temp_output_string.push_back('%');
				temp_output_string.push_back(temp[0]);
				temp_output_string.push_back(temp[1]);
			}
		}
		if (false == temp_output_string.empty()) {
			output_string.assign(temp_output_string);
		}
	} while (false);
	return error;
}
int main()
{
	do {
		std::string plain_string = "plain & text !@#$%^&*()_+-=,.[]{};:\'\"`\n";
		std::string encode_string = "%ec%97%b0%eb%b4%89%ec%8b%a0";
		DWORD error = url_encode(encode_string, plain_string);
		if (ERROR_SUCCESS != error) {
			break;
		}
		std::string decode_string = "";
		error = url_decode(decode_string, encode_string);
		if (ERROR_SUCCESS != error) {
			break;
		}
		printf(decode_string.c_str());
	} while (false);
	system("pause");
}

std::string s2;
s2.reserve(m_RealUrl.size());
for (size_t i = 0; i < m_RealUrl.size(); i++)
{
	if (m_RealUrl[i] == '%')
	{
		if (!strncmp(&m_RealUrl[i], "%3A", 3))
			s2 += ':';
		else if (!strncmp(&m_RealUrl[i], "%2F", 3))
			s2 += '/';
		else if (!strncmp(&m_RealUrl[i], "%3F", 3))
			s2 += '?';
		else if (!strncmp(&m_RealUrl[i], "%3D", 3))
			s2 += '=';
		else if (!strncmp(&m_RealUrl[i], "%26", 3))
			s2 += '&';
		else if (!strncmp(&m_RealUrl[i], "%25", 3))
			s2 += '%';
		i += 2;
	}
	else
		s2 += m_RealUrl[i];
}