#include "logs.h"
#include "ui_logs.h"

#include <QAbstractItemView>
#include <QSqlError>
#include <QSqlQuery>

Logs::Logs(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Logs)
    , model_(new QSqlTableModel(this))
{
    ui->setupUi(this);

    // Используем соединение SQLite, созданное эмулятором.
    if (!QSqlDatabase::contains()) {
        qWarning() << "Журнал: соединение с базой данных не найдено";
        ui->pushButtonClearLogs->setEnabled(false);
        return;
    }

    db_ = QSqlDatabase::database();
    if (!db_.isOpen() && !db_.open()) {
        qWarning() << "Журнал: не удалось открыть базу данных:"
                   << db_.lastError().text();
        ui->pushButtonClearLogs->setEnabled(false);
        return;
    }

    model_->setTable("Logs");
    model_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableViewDB->setModel(model_);
    ui->tableViewDB->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewDB->setColumnHidden(0, true);
    ui->tableViewDB->setColumnWidth(1, 300);
    ui->tableViewDB->setColumnWidth(2, 80);
    updateLogs();
}

Logs::~Logs()
{
    delete ui;
}

void Logs::on_pushButtonExit_clicked()
{
    close();
}

void Logs::updateLogs()
{
    if (!db_.isOpen()) {
        qWarning() << "Журнал: база данных не открыта";
        return;
    }

    if (!model_->select()) {
        qWarning() << "Журнал: ошибка загрузки записей:"
                   << model_->lastError().text();
    }
}

void Logs::on_pushButtonClearLogs_clicked()
{
    if (!db_.isOpen()) {
        qWarning() << "Журнал: невозможно очистить записи без подключения к БД";
        return;
    }

    QSqlQuery query(db_);
    if (!query.exec("DELETE FROM Logs")) {
        qWarning() << "Журнал: ошибка очистки:"
                   << query.lastError().text();
        return;
    }

    updateLogs();
}
