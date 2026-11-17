#ifndef CHECKMACHINESTATUS_H
#define CHECKMACHINESTATUS_H

#include "emulatormanager.h"
#include <QDialog>

namespace Ui { class CheckMachineStatus; }

class CheckMachineStatus : public QDialog
{
    Q_OBJECT

public:
    explicit CheckMachineStatus(QWidget *parent = nullptr);
    ~CheckMachineStatus() override;

    void setMachineId(int id, EmulatorManager *emulator);
    void updateUI();
    void updateStatusDisplay(Machine::Status status);

private slots:
    void on_pushButtonStart_clicked();
    void on_pushButtonStop_clicked();

private:
    Ui::CheckMachineStatus *ui;
    int machineId_ = 0;
    EmulatorManager *emul_ = nullptr;
};

#endif // CHECKMACHINESTATUS_H
