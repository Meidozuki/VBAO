#include <QApplication>
#include <QPushButton>
#include "view/displayer.h"

#include "model/model.hpp"
#include "viewmodel/viewmodel.hpp"
#include "window/window.hpp"
#include "common_parameter.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto pVM = std::make_shared<Viewmodel>();

    Window window;

    //bindings
    pVM->setModel(std::make_unique<Model>());
    //prop
    window.view_.displayer_->setNumber(pVM->getNumber());
    //command
    window.setCmdAdd(pVM->getCommand());
    window.view_.cmd_add_ = pVM->getCommand();
    //notification
    pVM->addPropertyListener(window.getPropSink());
    pVM->addCommandListener(window.getCmdSink());

    pVM->setInitNumber(init_time());
    //test
    window.view_.displayer_->onClick();

    window.view_.displayer_->show();
    return QApplication::exec();
}
