#pragma once

#include <iostream>
#include <string>

#include "DebugMacro.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
#define L2DE_LOG_WARNING_COLOR "\033[33m"
#define L2DE_LOG_ERROR_COLOR "\033[31m"
#define L2DE_LOG_CLEAR_COLOR "\033[0m"

#if L2DE_DEBUG
#define L2DE_LOG_INFO(message) Learning2DEngine::DebugTool::Log::Info(message,__FILE__, __LINE__)
#define L2DE_LOG_WARNING(message) Learning2DEngine::DebugTool::Log::Warning(message,__FILE__, __LINE__)
#define L2DE_LOG_ERROR(message) Learning2DEngine::DebugTool::Log::Error(message,__FILE__, __LINE__)
#else
#define L2DE_LOG_INFO(message)
#define L2DE_LOG_WARNING(message)
#define L2DE_LOG_ERROR(message)
#endif


		/// <summary>
		/// Use this Log class only, if you want to the logs everytime.
		/// The L2DE_LOG_INFO, L2DE_LOG_WARNING, L2DE_LOG_ERROR macros are defined.
		/// These macros run only, if L2DE_DEBUG macro are defined as 1.
		/// </summary>
		class Log final
		{
		private:
			Log() {}

			static std::string GetMessage(const std::string& message, const std::string& filePath, int fileLine)
			{
				return message + "\n " + filePath + " Line: " + std::to_string(fileLine);
			}
		public:

			static void Info(const std::string& message, const std::string& filePath, int fileLine)
			{
				std::cout << "INFO::" << GetMessage(message, filePath, fileLine) << std::endl;
			}

			static void Warning(const std::string& message, const std::string& filePath, int fileLine)
			{
				std::cout << L2DE_LOG_WARNING_COLOR << "WARNING::" << GetMessage(message, filePath, fileLine) << L2DE_LOG_CLEAR_COLOR << std::endl;
			}

			static void Error(const std::string& message, const std::string& filePath, int fileLine)
			{
				std::cout << L2DE_LOG_ERROR_COLOR << "ERROR::" << GetMessage(message, filePath, fileLine) << L2DE_LOG_CLEAR_COLOR << std::endl;
			}
		};
	}
}