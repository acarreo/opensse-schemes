//
// Sophos - Forward Private Searchable Encryption
// Copyright (C) 2016 Raphael Bost
//
// This file is part of Sophos.
//
// Sophos is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// Sophos is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Sophos.  If not, see <http://www.gnu.org/licenses/>.
//


#include <sse/schemes/utils/logger.hpp>

#include <fstream>
#include <iostream>
#include <memory>

namespace sse {
namespace logger {
LoggerSeverity severity__ = INFO;
// NOLINTNEXTLINE(cert-err58-cpp)
std::ostream null_stream__(nullptr);

std::unique_ptr<std::ofstream> benchmark_stream__;

LoggerSeverity severity()
{
    return severity__;
}

void set_severity(LoggerSeverity s)
{
    severity__ = s;
}

bool set_benchmark_file(const std::string& path)
{
    if (benchmark_stream__) {
        benchmark_stream__->close();
    }

    std::unique_ptr<std::ofstream> stream_ptr(new std::ofstream(path));

    if (!stream_ptr->is_open()) {
        benchmark_stream__.reset();

        logger::log(logger::ERROR)
            << "Failed to set benchmark file: " << path << std::endl;

        return false;
    }
    benchmark_stream__ = std::move(stream_ptr);

    return true;
}

std::ostream& log(LoggerSeverity s)
{
    if (s >= severity__) {
        return (std::cout << severity_string(s));
    }
    return null_stream__;
}

std::ostream& log_benchmark()
{
    if (benchmark_stream__) {
        return *benchmark_stream__;
    }
    return std::cout;
}

std::string severity_string(LoggerSeverity s)
{
    switch (s) {
    case DBG:
        return "[DEBUG] - ";
        break;

    case TRACE:
        return "[TRACE] - ";
        break;

    case INFO:
        return "[INFO] - ";
        break;

    case WARNING:
        return "[WARNING] - ";
        break;

    case ERROR:
        return "[ERROR] - ";
        break;

    case CRITICAL:
        return "[CRITICAL] - ";
        break;

    default:
        return "[??] - ";
        break;
    }
}

} // namespace logger
} // namespace sse