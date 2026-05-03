//
// Created by k3rnel1x on 10.04.2026.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <stdexcept>
#include <QString>
#include <qtextedit.h>
#include "config.h"
#include <chrono>

class Logger {
public:
    inline static Logger& get_instance()
    {
        static Logger instance;
        return instance;
    }

    void logDebug(const char* message)
    {
        open_debug_file();
        writeLog(debug_stream, message);
        fflush(debug_stream);
    }

    void logDebug(QString message)
    {
        if (message.isEmpty()) return;

        open_debug_file();
        QByteArray bytearray = message.toUtf8();
        const char* c_str = bytearray.constData();
        writeLog(debug_stream, c_str);
    }

protected:
    static void writeLog(FILE* f, const char* message) noexcept
    {
        if (!f || !message) return;
        fprintf(f, "%zu) %s\n", ++counter, message);
        fflush(f);
    }

    void open_debug_file()
    {
        if (!debug_stream)
        {
            std::string path = logs_path_dir + std::string(debug_log_filename);
            debug_stream = fopen(path.data(), "a");
            if (!debug_stream)
                throw std::runtime_error("failed to open log file: " + path);

            fprintf(debug_stream, "----------------- New log session started... --------------\n");
        }
    }

private:
    ~Logger() noexcept
    {
        if (debug_stream)
            fclose(debug_stream);
    }

    Logger() = default;
    Logger(const Logger&) = delete;
    Logger(const Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    FILE* debug_stream = nullptr;
    static inline size_t counter = 0;
    const char* debug_log_filename = "debug.log";
    const char* logs_path_dir = LOG_DIR;
};

#endif //LOGGER_HPP
