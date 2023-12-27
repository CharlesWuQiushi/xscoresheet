#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"

int main() 
{
    auto shared_sink = std::make_shared <spdlog::sinks::wincolor_stderr_sink_mt> ();
    auto parser_logger = std::make_shared<spdlog::logger> ("parser", shared_sink);
    auto writer_logger = std::make_shared<spdlog::logger> ("writer", shared_sink);

    shared_sink->set_pattern ("[%Y/%m/%d %T.%e] [%L] [%s:%#] [%n] %v");
    fmt::format_error;

    return 0;
}