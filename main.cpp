#include <QApplication>
#include <QPushButton>
#include "displayer.h"

#include "model/model.hpp"
#include "viewmodel/viewmodel.hpp"
#include "window/window.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto pVM = std::make_shared<Viewmodel>();

    Window window;
    Displayer displayer;

    //bindings
    pVM->setModel(std::make_unique<Model>());
    window.view_.setContent(displayer);
    //prop
    window.view_.displayer_->setNumber(pVM->getNumber());
    //command
    window.setCmdAdd(pVM->getCommand());
    window.view_.cmd_add_ = pVM->getCommand();
    //notification
    pVM->addPropertyListener(window.getPropSink());
    pVM->addCommandListener(window.getCmdSink());

    auto &&init_val = window.view_.displayer_->init();
    pVM->setInitNumber(init_val);
    window.view_.displayer_->onClick();


    window.view_.displayer_->show();
    return QApplication::exec();
}
