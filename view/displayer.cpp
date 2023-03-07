#include "displayer.h"
#include "ui_displayer.h"

#include <iostream>
#include <string>
#include <cassert>

#include <QString>
#include <QPushButton>

Displayer::Displayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Displayer)
{
    ui->setupUi(this);

    connect(ui->pushButton,
            &QPushButton::clicked,
            this,
            &Displayer::onClick);

    connect(ui->sliderA,
            &QSlider::valueChanged,
            this,
            &Displayer::slideNumA);

    connect(ui->sliderB,
            &QSlider::valueChanged,
            this,
            &Displayer::slideNumB);

    connect(ui->sliderA,
            &QSlider::sliderReleased,
            this,
            &Displayer::onSlideEnd);
    connect(ui->sliderB,
            &QSlider::sliderReleased,
            this,
            &Displayer::onSlideEnd);

    ui->sliderA->setMaximum(100);
    ui->sliderB->setMaximum(200);
}

Displayer::~Displayer()
{
    delete ui;
}

void Displayer::onClick() {
    //不清楚能不能多态，先写个wrapper
    this->cmd_add_->execute();
}

void Displayer::updateTime() {
    QString text=QString::fromStdString(*pNumber_);
    ui->lcdNumber->display(text);
}

void Displayer::slideNumA(int value) {
    *pNumberA_ = value;
    ui->numberA->display(value);
}
void Displayer::slideNumB(int value) {
    *pNumberB_ = value;
    ui->numberB->display(value);
}
void Displayer::updateSumResult() {
    int s = *pNumber_sum_;
    ui->numberSum->display(s);
}

void Displayer::onSlideEnd() {
    this->cmd_sum_->execute();
}

