#ifndef COMMON_H
#define COMMON_H

#include <QDebug>

#define LOGGING_PATTERN "[%{type}] %{function}:%{line} - %{message}"
#define LOG_DBG         qDebug().noquote() << QString("[DEBUG] %1:%2 -").arg( __FUNCTION__).arg(__LINE__)
#define LOG_INF         qInfo().noquote() << QString("[INFO] %1:%2 -").arg( __FUNCTION__).arg(__LINE__)
#define LOG_WRN         qWarning().noquote() << QString("[WARN] %1:%2 -").arg( __FUNCTION__).arg(__LINE__)
#define LOG_ERR         qCritical().noquote() << QString("[ERROR] %1:%2 -").arg( __FUNCTION__).arg(__LINE__)

#endif // COMMON_H
