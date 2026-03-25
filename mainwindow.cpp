#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "entrypoint.h"
#include "logic.h"

#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <qmessagebox.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ctx = get_ctx();
}

MainWindow::~MainWindow()
{
    delete ui;
    del_ctx(ctx);
}

void MainWindow::on_loadButton_clicked()
{
    ctx->opened_file = fopen("/home/k3rnel1x/Programing/qt-op-labs-bmstu/russian_demography.csv", "r");
    // qDebug() << ctx->opened_file;
    ctx->choised_collum = 3;
    ctx->choised_region = "asd";

    switch(perform_operation(LOAD_TABLE, ctx))
    {
    case FILE_EMPTY:
        QMessageBox::critical(this, "Error", "FILE_EMPTY");
        return;

    case TABLE_EMPTY:
        QMessageBox::critical(this, "Error", "TABLE_EMPTY");
        return;

    case ERROR:
        QMessageBox::critical(this, "Error", "RUNTIME ERROR");
        return;

    case SUCCESS:
    case NAME_COLLUM:
        break;
    }

    model = new QStandardItemModel(ctx->lines.count, 6);
    Vector* v = &ctx->lines;
    for(int i = 0; i < v->count; i++)
    {
        Line* line = v_item(v, i);

        model->setItem(i, 0, new QStandardItem( QString::number(i+1)  ));
        model->setItem(i, 1, new QStandardItem( QString::number(line->year)         ));
        model->setItem(i, 2, new QStandardItem( line->region ));
        model->setItem(i, 3, new QStandardItem( QString::number(line->natural_grow) ));
        model->setItem(i, 4, new QStandardItem( QString::number(line->birth_rate)   ));
        model->setItem(i, 5, new QStandardItem( QString::number(line->death_rate)   ));
        model->setItem(i, 6, new QStandardItem( QString::number(line->urbanization) ));
    }

    model->setHeaderData(0, Qt::Horizontal, "Number");
    model->setHeaderData(1, Qt::Horizontal, "Year");
    model->setHeaderData(2, Qt::Horizontal, "Region");
    model->setHeaderData(3, Qt::Horizontal, "Natural grow");
    model->setHeaderData(4, Qt::Horizontal, "Birth rate");
    model->setHeaderData(5, Qt::Horizontal, "Death rate");
    model->setHeaderData(6, Qt::Horizontal, "Urbanization");

    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

