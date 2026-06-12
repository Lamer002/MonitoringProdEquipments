#ifndef CHECKMACHINESTATUS_H
#define CHECKMACHINESTATUS_H

#include "machine.h"
#include "emulatormanager.h"

#include <QDialog>

namespace Ui {
class CheckMachineStatus;
}

class CheckMachineStatus : public QDialog
{
    Q_OBJECT

public:
    explicit CheckMachineStatus(QWidget *parent = nullptr);
    ~CheckMachineStatus();

    void setMachineId(int id, EmulatorManager* emul);
    void updateUI();
    void updateStatusDisplay(Machine::Status st);

private slots:
    void on_pushButtonStart_clicked();

    void on_pushButtonStop_clicked();

private:
    Ui::CheckMachineStatus *ui;
    int machineId_;
    EmulatorManager* emul_;
};

#endif // CHECKMACHINESTATUS_H
