/*
This source file is part of DupliTrace
For the latest info, see https://github.com/SwatKat1977/DupliTrace

Copyright 2024 DupliTrace Development Team

    This program is free software : you can redistribute it and /or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see < https://www.gnu.org/licenses/>.
*/
#include <chrono>
#include <memory>
#include <signal.h>
#include <stdexcept>
#include <string>
#include <vector>
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "Service.h"
#include "Logger.h"
#include "LoggerSettings.h"
#include "Platform.h"
#include "Version.h"

#define LOGGER_THREAD_SIZE  8192
#define LOGGER_THREAD_COUNT 1
#define LOGGER_NAME         "logger"

namespace duplitrace { namespace indexer {

using namespace std::chrono_literals;

static Service* service_instance;

static void StopSignalHandler(int signal) {
    LOGGER->info("Service shutdown signal has been caught...");
    service_instance->NotifyShutdownRequested();
}

Service::Service() : initialised_(false),
                     config_layout_(nullptr),
                     shutdown_requested_(false) {
}

bool Service::Initialise(common::SectionsMap* layout, std::string file) {
    config_layout_ = layout;

    config_file_ = file;

    if (!ReadConfiguration()) {
        return false;
    }

    if (!InitialiseLogger()) {
        return false;
    }

    LOGGER->info ("ITEMS Gateway Microservice V{0}.{1}.{2}-{3}",
        common::VERSION_MAJOR, common::VERSION_MINOR,
        common::VERSION_PATCH, common::VERSION_TAG);
    LOGGER->info ("Copyright 2024 DupliTrace Development Team");
    LOGGER->info ("Licensed under the GNU General Public License");

    PrintConfigurationItems();

    initialised_ = true;

    return initialised_;
}

void Service::Execute() {
    // Windows does not handle SIGINT properly, so disable... see MSDN:
    // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/signal?view=msvc-170
#if DUPLITRACE_PLATFORM == DUPLITRACE_PLATFORM_LINUX
    LOGGER->info ("Setting up signal handler...");
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = StopSignalHandler;
    sigemptyset (&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction (SIGINT, &sigIntHandler, NULL);
#endif

    while (!shutdown_requested_) {
        std::this_thread::sleep_for(1ms);
    }

    Shutdown();
}

void Service::Shutdown() {
}

bool Service::ReadConfiguration() {
    common::ConfigSetup layout = common::ConfigSetup(*config_layout_);

    config_manager_.Configure(&layout, config_file_, true);

    return config_manager_.processConfig();
}

bool Service::InitialiseLogger() {
    try {
        GET_LOGGING_LOG_TO_CONSOLE;
        GET_LOGGING_LOG_FILENAME;
        GET_LOGGING_MAX_FILE_SIZE;
        GET_LOGGING_MAX_FILE_COUNT;
        GET_LOGGING_LOG_FORMAT;
    }
    catch (const std::invalid_argument& ex) {
        printf("[FATAL ERROR] %s\n", ex.what());
        return false;
    }

    spdlog::init_thread_pool(LOGGER_THREAD_SIZE, LOGGER_THREAD_COUNT);

    std::vector<spdlog::sink_ptr> sinks;

    if (GET_LOGGING_LOG_TO_CONSOLE == LOGGING_LOG_TO_CONSOLE_YES) {
        auto stdout_sink = \
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        sinks.push_back(stdout_sink);
    }

    if (!GET_LOGGING_LOG_FILENAME.empty()) {
        if (GET_LOGGING_MAX_FILE_SIZE) {
            try {
                auto sink = std::make_shared<
                    spdlog::sinks::rotating_file_sink_mt> (
                        GET_LOGGING_LOG_FILENAME,
                        GET_LOGGING_MAX_FILE_SIZE,
                        GET_LOGGING_MAX_FILE_COUNT);
                sink->set_level(spdlog::level::debug);
                sinks.push_back(sink);
            }
            catch (const spdlog::spdlog_ex& ex) {
                std::string errStr =
                    std::string("Unable to initialise rotating file log "
                                "sink, reason: ") +
                    std::string(ex.what());
                throw std::runtime_error(errStr);
            }
        } else {
            try {
                const bool truncate = true;
                auto sink = std::make_shared<
                    spdlog::sinks::basic_file_sink_mt> (
                        GET_LOGGING_LOG_FILENAME.c_str(),
                        truncate);
                sink->set_level(spdlog::level::debug);
                sinks.push_back(sink);
            }
            catch (const spdlog::spdlog_ex& ex) {
                std::string errStr =
                    std::string(
                        "Unable to initialise file log sink, reason: ") +
                    std::string(ex.what());
                throw std::runtime_error(errStr);
            }
        }
    }

    auto logger = std::make_shared<spdlog::async_logger> (
        LOGGER_NAME,
        sinks.begin(),
        sinks.end(),
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::block);
    spdlog::register_logger(logger);

    // Set the log format, based on the formatting pattern flags:
    // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
    spdlog::set_pattern(GET_LOGGING_LOG_FORMAT);

    return true;
}

void Service::PrintConfigurationItems() {
    LOGGER->info("|=====================|");
    LOGGER->info("|=== Configuration ===|");
    LOGGER->info("|=====================|");

    LOGGER->info("[LOGGING]");
    LOGGER->info("-> Log Level      : {0}", config_manager_.GetStringEntry(
                 "logging", "log_level").c_str());
    LOGGER->info("-> Log To Console : {0}", config_manager_.GetStringEntry(
                 "logging", "log_to_console").c_str ());
    LOGGER->info("-> Log Filename   : {0}", config_manager_.GetStringEntry(
                 "logging", "log_filename").c_str());
    LOGGER->info("-> Max File Size  : {0:d} bytes", config_manager_.GetIntEntry(
                 "logging", "max_file_size"));
    LOGGER->info("-> Max File Count : {0:d}", config_manager_.GetIntEntry(
                 "logging", "max_file_count"));
    LOGGER->info("-> Log Format     : {0}", config_manager_.GetStringEntry(
                 "logging", "log_format").c_str());
}

}   // namespace indexer
}   // namespace duplitrace
