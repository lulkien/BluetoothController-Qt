#ifndef COMMON_H
#define COMMON_H

#include <QDebug>

#define LOGGING_PATTERN "[%{type}] %{function}:%{line} - %{message}"
#define LOG_DEBUG       qDebug()
#define LOG_INFO        qInfo()
#define LOG_WARNING     qWarning()
#define LOG_ERROR       qCritical()

#endif // COMMON_H
