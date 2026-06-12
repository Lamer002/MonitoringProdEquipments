#ifndef ADDMACHINEINLIST_H
#define ADDMACHINEINLIST_H

#include "emulatormanager.h"
#include "machine.h"

#include <QDialog>

namespace Ui {
class AddMachineInList;
}

class AddMachineInList : public QDialog
{
    Q_OBJECT

public:
    explicit AddMachineInList(QWidget *parent = nullptr);
    ~AddMachineInList();

signals:
    void addInMachine(QString value);

private slots:
    void on_pushButtonAddInList_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::AddMachineInList *ui;
};

#endif // ADDMACHINEINLIST_H
