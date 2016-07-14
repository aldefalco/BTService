#pragma once

#include <boost\log\common.hpp>
#include <boost\log\sources\severity_channel_logger.hpp>
#include <string>

namespace BTService { namespace Client { namespace Log {

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

	// The formatting logic for the severity level
	template< typename CharT, typename TraitsT >
	inline std::basic_ostream< CharT, TraitsT >& operator<< (
		std::basic_ostream< CharT, TraitsT >& strm, Level lvl)
	{
		static const char* const str[] =
		{
			"Trace",
			"Debug",
			"Info",
			"Warning",
			"Error",
			"Critical"
		};
		if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
			strm << str[static_cast< int >(lvl)];
		else
			strm << static_cast< int >(lvl);
		return strm;
	}


}}}