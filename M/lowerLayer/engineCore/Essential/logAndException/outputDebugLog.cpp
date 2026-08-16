#include "../logAndException/outputDebugLog.h"
#include <Windows.h>
#include <strsafe.h>
#include <chrono>
#include <format>
#include <filesystem>

std::ofstream DebugLogInitialize()
{
	std::filesystem::create_directory("DebugLog");

	//現在時刻を取得
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	//ログファイルの名前にコンマ以下はいらないので削る
	std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>
		nowSeconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
	//日本時間に変換
	std::chrono::zoned_time localTime{ std::chrono::current_zone(),nowSeconds };
	//formatを使って年月日_時分秒の文字列に変換
	std::string dateString = std::format("{:%Y%m%d_%H%M%S}", localTime);
	//時刻を使ってファイル名を決定
	std::string logFilePath = std::string("DebugLog/") + dateString + "log";
	//ファイルを使って書き込み準備
	std::ofstream logStream(logFilePath);

	return logStream;	//ファイルそのものてきな
}

void Log(std::string const& str_)
{
	OutputDebugStringA(str_.c_str());
}

void Log(std::ostream& os, std::string const& message)
{

#ifdef _DEBUG
	os << message << std::endl;
	OutputDebugStringA(message.c_str());
#endif

}

