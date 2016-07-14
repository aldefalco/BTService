#pragma once

#include "targetver.h"

// C run-time
#include <stdio.h>
#include <tchar.h>

// STL
#include <string>
#include <set>
#include <iostream>
#include <mutex>
#include <fstream>  
#include <functional>
#include <memory>


// Boost asio
#include <boost\asio.hpp>

// Boost log
#include <boost\log\common.hpp>
#include <boost\log\sources\severity_channel_logger.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/support/date_time.hpp>

// Boost lexical cast
#include <boost\lexical_cast.hpp>

// Boost bind
#include <boost\bind.hpp>