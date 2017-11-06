#include "Logger.h"

Logger::Level Logger::consoleLevel = Level::INFO;
Logger::Level Logger::fileLevel = Level::ALL;

std::ofstream Logger::file = std::ofstream();

const char *Logger::PREFIX_FINEST = " FINEST";
const char *Logger::PREFIX_FINER = "  FINER";
const char *Logger::PREFIX_FINE = "   FINE";
const char *Logger::PREFIX_CONFIG = " CONFIG";
const char *Logger::PREFIX_INFO = "   INFO";
const char *Logger::PREFIX_WARNING = "WARNING";
const char *Logger::PREFIX_SEVERE = " SEVERE";

void Logger::finest(const char *message) {
    log(Level::FINEST, message);
}

void Logger::finer(const char *message) {
    log(Level::FINER, message);
}

void Logger::fine(const char *message) {
    log(Level::FINE, message);
}

void Logger::config(const char *message) {
    log(Level::CONFIG, message);
}

void Logger::info(const char *message) {
    log(Level::INFO, message);
}

void Logger::warning(const char *message) {
    log(Level::WARNING, message);
}

void Logger::severe(const char *message) {
    log(Level::SEVERE, message);
}

void Logger::log(Logger::Level level, const char *message) {
    if (level < Logger::consoleLevel && level < Logger::fileLevel ) { return; }

    const char *prefix = nullptr;

    if (level == Level::FINEST) { prefix = PREFIX_FINEST; }
    else if (level == Level::FINER) { prefix = PREFIX_FINER; }
    else if (level == Level::FINE) { prefix = PREFIX_FINE; }
    else if (level == Level::CONFIG) { prefix = PREFIX_CONFIG; }
    else if (level == Level::INFO) { prefix = PREFIX_INFO; }
    else if (level == Level::WARNING) { prefix = PREFIX_WARNING; }
    else if (level == Level::SEVERE) { prefix = PREFIX_SEVERE; }

    std::string out = std::string(prefix) + ": " + message + ".";

    if (level >= Logger::fileLevel) {
        file << out << std::endl;
    }

    if (level >= Logger::consoleLevel) {
        ((level >= Level::WARNING) ? std::cerr : std::cout) << out << std::endl;
    }
}
