
#include "log.h"

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/support/date_time.hpp>

#include <string>

namespace BTService { namespace Client { namespace Log {

	namespace expr = boost::log::expressions;
	namespace keywords = boost::log::keywords;
	namespace attrs = boost::log::attributes;

	void setup()
	{
		boost::log::add_console_log(
			std::clog, 
			keywords::filter = expr::attr< Level >("Severity") >= Level::Trace,
			keywords::format = expr::format("[%1%] <%2%> [%3%] %4%")
			% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
			% expr::attr< Level >("Severity")
			% expr::attr< std::string >("Module")
			% expr::message
		);

		boost::log::add_file_log(
			"client.log",
			keywords::filter = expr::attr< Level >("Severity") >= Level::Trace,
			keywords::format = expr::format("[%1%] <%2%> [%3%] %4%")
			% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
			% expr::attr< Level >("Severity")
			% expr::attr< std::string >("Module")
			//% expr::format_named_scope("Scope", keywords::format = "%n (%f:%l)")
			% expr::message
		);

		boost::log::add_common_attributes();
		boost::log::core::get()->add_thread_attribute("Scope", boost::log::attributes::named_scope());
	}

	Logger create(const std::string& name)
	{
		Logger logger(boost::log::keywords::channel = name);
		logger.add_attribute("Module", attrs::constant<std::string>(name) );
		return logger;
	}
	
}}}