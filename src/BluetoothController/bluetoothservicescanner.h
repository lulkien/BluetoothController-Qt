#ifndef BLUETOOTHSERVICESCANNER_H
#define BLUETOOTHSERVICESCANNER_H

#include <QObject>

class BluetoothServiceScanner : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothServiceScanner(QObject *parent = nullptr);

signals:

};

#endif // BLUETOOTHSERVICESCANNER_H
