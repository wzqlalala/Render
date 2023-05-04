#pragma once
#include <memory>
#include "rendbase_global.h"

#define CORE_INFO(...)  mxr::Log::GetLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)  mxr::Log::GetLogger()->warn(__VA_ARGS__)
#define CORE_DEBUG(...) mxr::Log::GetLogger()->debug(__VA_ARGS__)
#define CORE_TRACE(...) mxr::Log::GetLogger()->trace(__VA_ARGS__)
#define CORE_ERROR(...) mxr::Log::GetLogger()->error(__VA_ARGS__)

namespace spdlog 
{
	class logger;
}
namespace mxr
{
	class RENDBASE_EXPORT Log
	{
	private:
		static std::shared_ptr<spdlog::logger> logger;

	public:
		static std::shared_ptr<spdlog::logger> GetLogger();

	public:
		static void Init();
		static void Shutdown();

	};



	

	

}

