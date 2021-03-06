/**
 * \file Logger.hpp
 * \brief Logger
 * \author Cyprien.R
**/

#ifndef ZIA_LOGGER_HPP
#define ZIA_LOGGER_HPP


#if defined(_WIN32) || defined(WIN32)

#include <winsock2.h>
#include <Windows.h>
#include <cstdio>

//#define _WIN32_WINNT  0x0601

#define WINDOWS_COLOR_ISSUE true // enable me if you re facing issues with color print on windows

#define BLUE 9
#define GREEN 10
#define RED 12
#define YELLOW 14
#define WHITE 15

#else

#include <dlfcn.h>
#define WINDOWS_COLOR_ISSUE false // don't TOUCH !

#endif

#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <thread>

/**
 * \brief logging namespace
 **/
namespace logging {

    //the log levels we support
    enum class log_level : uint8_t {
        TRACE = 0, DEBUG = 1, INFO = 2, WARN = 3, ERR = 4
    };

    struct enum_hasher {
        template<typename T>
        std::size_t operator()(T t) const { return static_cast<std::size_t>(t); }
    };

    const std::unordered_map<log_level, std::string, enum_hasher> uncolored
            {
                    {log_level::ERR,   " [ERROR] "},
                    {log_level::WARN,  " [WARN] "},
                    {log_level::INFO,  " [INFO] "},
                    {log_level::DEBUG, " [DEBUG] "},
                    {log_level::TRACE, " [TRACE] "}
            };
    const std::unordered_map<log_level, std::string, enum_hasher> colored
            {
                    {log_level::ERR,   " \x1b[31;1m[ERROR]\x1b[0m "},
                    {log_level::WARN,  " \x1b[33;1m[WARN]\x1b[0m "},
                    {log_level::INFO,  " \x1b[32;1m[INFO]\x1b[0m "},
                    {log_level::DEBUG, " \x1b[34;1m[DEBUG]\x1b[0m "},
                    {log_level::TRACE, " \x1b[37;1m[TRACE]\x1b[0m "}
            };

#if defined(_WIN32) || defined(WIN32)
    const std::unordered_map<log_level, int> colored_wd
            {
                    {log_level::ERR,   RED},
                    {log_level::WARN,  YELLOW},
                    {log_level::INFO,  GREEN},
                    {log_level::DEBUG, BLUE},
                    {log_level::TRACE, WHITE}
            };
#endif

    //all, something in between, none or default to info
#if defined(LOGGING_LEVEL_ALL) || defined(LOGGING_LEVEL_TRACE)
    constexpr log_level LOG_LEVEL_CUTOFF = log_level::TRACE;
#elif defined(LOGGING_LEVEL_DEBUG)
    constexpr log_level LOG_LEVEL_CUTOFF = log_level::DEBUG;
#elif defined(LOGGING_LEVEL_WARN)
    constexpr log_level LOG_LEVEL_CUTOFF = log_level::WARN;
#elif defined(LOGGING_LEVEL_ERROR)
    constexpr log_level LOG_LEVEL_CUTOFF = log_level::ERR;
#elif defined(LOGGING_LEVEL_NONE)
    constexpr log_level LOG_LEVEL_CUTOFF = log_level::ERR + 1;
#else
    constexpr log_level LOG_LEVEL_CUTOFF = log_level::TRACE;
#endif

    //Logger base class, not pure virtual so you can use as a null Logger if you want
    using logging_config_t = std::unordered_map<std::string, std::string>;

    class Logger {
    public:
        Logger() = delete;

        explicit Logger(const logging_config_t &) {};

        virtual ~Logger() = default;

        virtual void log([[maybe_unused]]const std::string &, [[maybe_unused]]const log_level) {};

        virtual void log([[maybe_unused]]const std::string &) {};

        virtual void log([[maybe_unused]]const std::string &message, [[maybe_unused]]int color) {};
    protected:
        std::mutex lock;
    };

    //Logger that writes to standard out
    class std_out_logger : public Logger {
    public:
        std_out_logger() = delete;

        explicit std_out_logger(const logging_config_t &config) : Logger(config),
        levels(((config.find("color") != config.end()) && !WINDOWS_COLOR_ISSUE) ? colored : uncolored) {}


        /**
         * \brief log : display log message with header
         *
         * \param level log level of the message (has to be higher than LOG_LEVEL_CUTOFF to be printed
         * \param message : message to be printed
        **/
        void log(const std::string &message, const log_level level) override {
            if (level < LOG_LEVEL_CUTOFF)
                return;
            std::string output;
            output.reserve(message.length() + 64);
            output.append(levels.find(level)->second);
            output.append(message);
            output.push_back('\n');

            if (WINDOWS_COLOR_ISSUE) {
#if defined(_WIN32) || defined(WIN32)
                log(output, colored_wd.find(level)->second);
#endif
                log(output);
            } else
                log(output);
        }

        /**
         * \brief display and flush message
         *
         * \param message : message to be printed
        **/
        void log(const std::string &message) override {
            std::cout << message;
            std::cout.flush();
        }

#ifdef _WIN32

        void log(const std::string &message, int color) override {
            HANDLE hConsole;
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            FlushConsoleInputBuffer(hConsole); \
            SetConsoleTextAttribute(hConsole, color);
            std::cout << message;
            std::cout.flush();
            SetConsoleTextAttribute(hConsole, 7);
        };
#endif

    protected:
        const std::unordered_map<log_level, std::string, enum_hasher> levels;
    };


    using logger_creator = Logger *(*)(const logging_config_t &);

    class logger_factory {
    public:
        /**
         * \brief logger_factory constructor : create loggers
        **/
        logger_factory() {
            creators.emplace("std_out", [](const logging_config_t &config) -> Logger * { return new std_out_logger(config); });
        }

        /**
         * \brief produce : return a logger according to the type given in config
         *
         * \param config : informations about the logger to return
         *
         * \return the logger asked for
        **/
        [[nodiscard]] Logger *produce(const logging_config_t &config) const {
            //grab the type
            auto type = config.find("type");
            if (type == config.end())
                throw std::runtime_error("Logging factory configuration requires a type of Logger");
            //grab the Logger
            auto found = creators.find(type->second);
            if (found != creators.end())
                return found->second(config);
            //couldn't get a Logger
            throw std::runtime_error("Couldn't produce Logger for type: " + type->second);
        }

    protected:
        std::unordered_map<std::string, logger_creator> creators;
    };

    /**
     * \brief get_factory, return a factory (singleton)
     *
     * \return the logger factory
    **/
    inline logger_factory &get_factory() {
        static logger_factory factory_singleton{};
        return factory_singleton;
    }

    /**
     * \brief get_logger, return a singleton logger according to the params in config
     *
     * \param config, config for the logger
     *
     * \return the logger
    **/
    inline Logger &get_logger(const logging_config_t &config = {{"type",  "std_out"},
                                                                {"color", ""}}) {
        static std::unique_ptr<Logger> singleton(get_factory().produce(config));
        return *singleton;
    }

    /**
     * \brief log, call log function of the currently used logger
     *
     * \param message, message to print
     * \param level, log level
    **/
    inline void log(const std::string &message, const log_level level) {
        get_logger().log(message, level);
    }

    /**
     * \brief log, call log function of the currently used logger (doesn't consider the level)
     *
     * \param message, message to print
    **/
    inline void log(const std::string &message) {
        get_logger().log(message);
    }

    /**
     * \brief TRACE, lowest log level
     *
     * \param message, message to print
    **/
    inline void TRACE(const std::string &message) {
        get_logger().log(message, log_level::TRACE);
    }

    /**
     * \brief DEBUG, second log level
     *
     * \param message, message to print
    **/
    inline void DEBUG(const std::string &message) {
        get_logger().log(message, log_level::DEBUG);
    }

    /**
     * \brief INFO, third log level
     *
     * \param message, message to print
    **/
    inline void INFO(const std::string &message) {
        get_logger().log(message, log_level::INFO);
    }

    /**
     * \brief WARN, fourth log level
     *
     * \param message, message to print
    **/
    inline void WARN(const std::string &message) {
        get_logger().log(message, log_level::WARN);
    }

    /**
     * \brief ERR, higher log level
     *
     * \param message, message to print
    **/
    inline void ERR(const std::string &message) {
        get_logger().log(message, log_level::ERR);
    }
}

#endif //ZIA_LOGGER_HPP
