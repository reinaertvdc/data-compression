#ifndef PROJECT_LOGGER_H
#define PROJECT_LOGGER_H


#include <iostream>
#include <string>
#include <fstream>

class Logger {
public:
    enum Level {
        ALL, FINEST, FINER, FINE, CONFIG, INFO, WARNING, SEVERE, OFF
    };

    static Level consoleLevel;
    static Level fileLevel;

    static std::ofstream file;

    static void finest(const char *message);

    static void finest(const std::string &message);

    static void finer(const char *message);

    static void finer(const std::string &message);

    static void fine(const char *message);

    static void fine(const std::string &message);

    static void config(const char *message);

    static void config(const std::string &message);

    static void info(const char *message);

    static void info(const std::string &message);

    static void warning(const char *message);

    static void warning(const std::string &message);

    static void severe(const char *message);

    static void severe(const std::string &message);

private:
    static const char *PREFIX_FINEST;
    static const char *PREFIX_FINER;
    static const char *PREFIX_FINE;
    static const char *PREFIX_CONFIG;
    static const char *PREFIX_INFO;
    static const char *PREFIX_WARNING;
    static const char *PREFIX_SEVERE;

    static void log(Level level, const char *message);
};


#endif //PROJECT_LOGGER_H
