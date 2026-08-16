#pragma once
#include <string>

std::wstring ConvertString(const std::string& str);
std::string ConvertString(const std::wstring& str);
std::string ToUTF8(const std::wstring& value);
