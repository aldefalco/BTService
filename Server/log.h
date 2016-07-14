#pragma once

// The Log adapter

namespace BTService { namespace Server { namespace Log {

	enum class Level
	{
		Trace,
		Debug,
		Info,
		Warning,
		Error,
		Critical
	};

	using Logger = boost::log::sources::severity_channel_logger_mt<Level, std::string>;
	
	void setup();
	Logger create(const std::string& name);

#define LOG_INFO(logger) BOOST_LOG_SEV(logger, Log::Level::Info)
#define LOG_TRACE(logger) BOOST_LOG_SEV(logger, Log::Level::Trace)
#define LOG_WARNING(logger) BOOST_LOG_SEV(logger, Log::Level::Warning)
#define LOG_ERROR(logger) BOOST_LOG_SEV(logger, Log::Level::Error)

#define LINFO() BOOST_LOG_SEV(logger, Log::Level::Info)
#define LDEBUG() BOOST_LOG_SEV(logger, Log::Level::Debug)
#define LTRACE() BOOST_LOG_SEV(logger, Log::Level::Trace)
#define LWARNING() BOOST_LOG_SEV(logger, Log::Level::Warning)
#define LERROR() BOOST_LOG_SEV(logger, Log::Level::Error)

}}}