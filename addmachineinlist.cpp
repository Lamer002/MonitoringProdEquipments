#include "addmachineinlist.h"
#include "ui_addmachineinlist.h"


AddMachineInList::AddMachineInList(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddMachineInList)
{
    ui->setupUi(this);
}

AddMachineInList::~AddMachineInList()
{
    delete ui;
}

void AddMachineInList::on_pushButtonAddInList_clicked()
{
    int tmpID = ui->lineID->text().toInt();

    if(tmpID > 0)
    {
        emit addInMachine(QString::number(tmpID));
    }
    this->close();
}

void AddMachineInList::on_pushButtonCancel_clicked()
{
    this->close();
}

