#include "stdafx.h"

#include "log.h"

namespace BTService { namespace Client { namespace Log {

	namespace expr = boost::log::expressions;
	namespace keywords = boost::log::keywords;
	namespace attrs = boost::log::attributes;

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

	void setup(int id)
	{
		using boost::format;
		boost::log::add_console_log(
			std::clog, 
			keywords::filter = expr::attr< Level >("Severity") >= Level::Trace,
			keywords::format = expr::format("[%1%] [%2%] <%3%> [%4%] %5%")
			% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
			% expr::attr< int >("ID")
			% expr::attr< Level >("Severity")
			% expr::attr< std::string >("Module")
			% expr::message
		);

		boost::log::add_file_log(
			str(format("client-%1%.log") % id),
			keywords::filter = expr::attr< Level >("Severity") >= Level::Trace,
			keywords::format = expr::format("[%1%] [%2%] <%3%> [%4%] %5%")
			% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
			% expr::attr< int >("ID")
			% expr::attr< Level >("Severity")
			% expr::attr< std::string >("Module")
			% expr::message
		);

		boost::log::add_common_attributes();
		auto core = boost::log::core::get();
		core->add_thread_attribute("Scope", boost::log::attributes::named_scope());
		core->add_global_attribute("ID", attrs::constant<int>(id));
	}

	Logger create(const std::string& name)
	{
		Logger logger(boost::log::keywords::channel = name);
		logger.add_attribute("Module", attrs::constant<std::string>(name) );
		return logger;
	}
	
}}}