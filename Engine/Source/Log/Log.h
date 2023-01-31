#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

    
class Log {
public:
    static void Init();
    
    static inline std::shared_ptr<spdlog::logger>& GetCoreLogger() {
        return s_CoreLogger;
    }
    
    static inline std::shared_ptr<spdlog::logger>& GetClientLogger() {
        return s_ClientLogger;
    }
    
    static inline std::shared_ptr<spdlog::logger>& GetFileLogger() {
        return s_FileLogger;
    }
    
private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
    static std::shared_ptr<spdlog::logger> s_FileLogger;
};


// Core Log Macros
#define LOG_FATAL(...) {::Log::GetCoreLogger()->fatal(__VA_ARGS__);\
	::Log::GetFileLogger()->fatal(__VA_ARGS__);}
#define LOG_ERROR(...) {::Log::GetCoreLogger()->error(__VA_ARGS__);\
	::Log::GetFileLogger()->error(__VA_ARGS__);}
#define LOG_WARN(...) {::Log::GetCoreLogger()->warn(__VA_ARGS__);\
	::Log::GetFileLogger()->warn(__VA_ARGS__);}
#define LOG_INFO(...) {::Log::GetCoreLogger()->info(__VA_ARGS__);\
	::Log::GetFileLogger()->info(__VA_ARGS__);}
#define LOG_TRACE(...) {::Log::GetCoreLogger()->trace(__VA_ARGS__);\
	::Log::GetFileLogger()->trace(__VA_ARGS__);}

// Core Client Macros
#define CLIENT_FATAL(...) ::Log::GetClientLogger()->fatal(__VA_ARGS__);
#define CLIENT_ERROR(...) ::Log::GetClientLogger()->error(__VA_ARGS__);
#define CLIENT_WARN(...) ::Log::GetClientLogger()->warn(__VA_ARGS__);
#define CLIENT_INFO(...) ::Log::GetClientLogger()->info(__VA_ARGS__);
#define CLIENT_TRACE(...) ::Log::GetClientLogger()->trace(__VA_ARGS__);

#ifdef _DEBUG
#define CLIENT_ASSERT(x, ...) { if(!(x)) { CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define CORE_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define CLIENT_ASSERT(x, ...)
#define CORE_ASSERT(x, ...)
#endif
#define KT_THROW_EXCEPTION(...) { throw std::runtime_error(__VA_ARGS__);}