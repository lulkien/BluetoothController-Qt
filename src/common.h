#ifndef COMMON_H
#define COMMON_H

#include <QDebug>

#define LOGGING_PATTERN "[%{type}] %{function}:%{line} - %{message}"
#define LOG_DBG         qDebug().noquote() << QString("[DEBUG] %1:%2 -").arg( __FUNCTION__).arg(__LINE__)
#define LOG_INF         qInfo().noquote() << QString("[INFO] %1:%2 -").arg( __FUNCTION__).arg(__LINE__)
#define LOG_WRN         qWarning().noquote() << QString("[WARN] %1:%2 -").arg( __FUNCTION__).arg(__LINE__)
#define LOG_ERR         qCritical().noquote() << QString("[ERROR] %1:%2 -").arg( __FUNCTION__).arg(__LINE__)

#define SERVICE_UUID    QBluetoothUuid(QLatin1String("e8e10f95-1a70-4b27-9ccf-02010264e9c8"))

#endif // COMMON_H
