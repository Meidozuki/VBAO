#include "view.h"
#include "ui_view.h"

#include <string>

#include <QTime>
#include <QString>
#include <QPushButton>

View::View(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::View)
{
    ui->setupUi(this);

    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    number_ = text.toStdString();
    ui->lcdNumber->display(text);

    connect(ui->pushButton,
            &QPushButton::clicked,
            this,
            &View::addNumber);
}

View::~View()
{
    delete ui;
}

void View::addNumber() {
    QTime time = QTime::fromString(
        QString::fromStdString(number_));
    time = time.addSecs(60);
    QString text = time.toString("hh:mm");
    number_ = text.toStdString();
    ui->lcdNumber->display(text);

}


