#include "addmachineinlist.h"
#include "ui_addmachineinlist.h"

#include <QIntValidator>
#include <QMessageBox>
#include <limits>

AddMachineInList::AddMachineInList(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddMachineInList)
{
    ui->setupUi(this);
    ui->lineID->setValidator(
        new QIntValidator(1, std::numeric_limits<int>::max(), this));
    ui->lineID->setPlaceholderText(tr("Например, 101"));
    ui->lineID->setFocus();
}

AddMachineInList::~AddMachineInList()
{
    delete ui;
}

void AddMachineInList::on_pushButtonAddInList_clicked()
{
    bool ok = false;
    const int id = ui->lineID->text().toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, tr("Неверный ID"),
                             tr("Введите положительный числовой ID."));
        return;
    }

    emit addInMachine(QString::number(id));
    accept();
}

void AddMachineInList::on_pushButtonCancel_clicked()
{
    reject();
}
