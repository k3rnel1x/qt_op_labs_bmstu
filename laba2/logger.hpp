f//
// Created by k3rnel1x on 10.04.2026.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <stdexcept>
#include <QString>
#include <qtextedit.h>
#include "config.h"

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
        fprintf(f, "%s\n", message);
    }

    void open_debug_file()
    {
        if (!debug_stream)
        {
            size_t path_len = strlen(logs_path_dir) + strlen(debug_log_filename) + 2;
            char* path = new char[path_len]{};
            memcpy(path, logs_path_dir, strlen(logs_path_dir));
            path[strlen(logs_path_dir)] = path[strlen(logs_path_dir)-1] == '/' ? 0 : '/';
            memcpy(path + strlen(logs_path_dir) + 1, debug_log_filename, strlen(debug_log_filename));

            debug_stream = fopen(path, "a");

            delete path;
            if (!debug_stream) throw std::runtime_error("failed to open log file");
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

    // FILE* error_stream = nullptr; // TODO ?
    // FILE* info_stream  = nullptr; // TODO ?
    FILE* debug_stream = nullptr;
    const char* debug_log_filename = "debug.log";
    const char* logs_path_dir = ABSOLUTE_LOG_DIR;
};

#endif //LOGGER_HPP
