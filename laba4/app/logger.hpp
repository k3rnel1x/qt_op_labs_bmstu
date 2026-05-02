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
    static Logger& get_instance()
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
    void writeLog(FILE* f, const char* message) const noexcept
    {
        if (!f || !message) return;
        fprintf(f, "%s\n",message);
    }

    void open_debug_file()
    {
        if (!debug_stream)
        {
            std::string path = logs_path_dir + std::string(debug_log_filename);
            debug_stream = fopen(path.data(), "a");
            if (!debug_stream)
                throw std::runtime_error("failed to open log file: " + path);
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
    const char* debug_log_filename = "debug.log";
    const char* logs_path_dir = LOG_DIR;
};

#endif //LOGGER_HPP
