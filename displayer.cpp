#include "displayer.h"
#include "ui_displayer.h"

#include <string>
#include <cassert>

#include <QTime>
#include <QString>
#include <QPushButton>

#include "view/view.hpp"

Displayer::Displayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Displayer)
{
    ui->setupUi(this);

    view_= nullptr;

    connect(ui->pushButton,
            &QPushButton::clicked,
            this,
            &Displayer::onClick);
}

Displayer::~Displayer()
{
    delete ui;
}

void Displayer::onClick() {
    //不清楚能不能多态，先写个wrapper
    assert(view_ != nullptr);
    assert(view_->cmd_add_ != nullptr);
    view_->cmd_add_->execute();
}

void Displayer::updateNumber() {
    QString text=QString::fromStdString(*pNumber_);
    ui->lcdNumber->display(text);
}

std::string Displayer::init() {
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    ui->lcdNumber->display(text);
    return text.toStdString();
}


