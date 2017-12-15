#ifndef INC_3_VIDEO_COMPRESSION_LOGGER_H
#define INC_3_VIDEO_COMPRESSION_LOGGER_H


#include <fstream>

/**
 * Logging class that logs messages to console and file, and that supports different verbosity levels for both.
 */
class Logger {
public:
    /**
     * Different possible levels of verbosity, ALL being the highest and OFF being the lowest
     */
    enum Level {
        ALL, FINEST, FINER, FINE, CONFIG, INFO, WARNING, SEVERE, OFF
    };

    /**
     * Verbosity level to use when logging to the console
     */
    static Level consoleLevel;
    /**
     * Verbosity level to use when logging to the log file
     */
    static Level fileLevel;

    /**
     * The file to log to. This must be opened externally to enable file logging, and externally closed afterwards.
     */
    static std::ofstream file;

    /**
     * Log the given message at the finest verbosity level
     * @param message the message to log
     */
    static void finest(const char *message);

    /**
     * Log the given message at the finest verbosity level
     * @param message the message to log
     */
    static void finest(const std::string &message);

    /**
     * Log the given message at the finer verbosity level
     * @param message the message to log
     */
    static void finer(const char *message);

    /**
     * Log the given message at the finer verbosity level
     * @param message the message to log
     */
    static void finer(const std::string &message);

    /**
     * Log the given message at the fine verbosity level
     * @param message the message to log
     */
    static void fine(const char *message);

    /**
     * Log the given message at the fine verbosity level
     * @param message the message to log
     */
    static void fine(const std::string &message);

    /**
     * Log the given message at the config verbosity level
     * @param message the message to log
     */
    static void config(const char *message);

    /**
     * Log the given message at the config verbosity level
     * @param message the message to log
     */
    static void config(const std::string &message);

    /**
     * Log the given message at the info verbosity level
     * @param message the message to log
     */
    static void info(const char *message);

    /**
     * Log the given message at the info verbosity level
     * @param message the message to log
     */
    static void info(const std::string &message);

    /**
     * Log the given message at the warning verbosity level
     * @param message the message to log
     */
    static void warning(const char *message);

    /**
     * Log the given message at the warning verbosity level
     * @param message the message to log
     */
    static void warning(const std::string &message);

    /**
     * Log the given message at the severe verbosity level
     * @param message the message to log
     */
    static void severe(const char *message);

    /**
     * Log the given message at the severe verbosity level
     * @param message the message to log
     */
    static void severe(const std::string &message);

private:
    static const char *PREFIX_FINEST;
    static const char *PREFIX_FINER;
    static const char *PREFIX_FINE;
    static const char *PREFIX_CONFIG;
    static const char *PREFIX_INFO;
    static const char *PREFIX_WARNING;
    static const char *PREFIX_SEVERE;

    /**
     * Log the given message at the given verbosity level
     * @param level the verbosity level to use
     * @param message the message to log
     */
    static void log(Level level, const char *message);
};


#endif //INC_3_VIDEO_COMPRESSION_LOGGER_H
