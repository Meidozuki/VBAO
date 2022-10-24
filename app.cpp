#include "app.hpp"

#include "model/model.hpp"

void init(Viewmodel *pVM,Window *pWin){

    //bindings
    pVM->setModel(std::make_unique<Model>());
    //prop
    pWin->view_.displayer_->setNumber(pVM->getNumber());
    //command
    pWin->setCmdAdd(pVM->getCommand());
    pWin->view_.cmd_add_ = pVM->getCommand();
    //notification
    pVM->addPropertyListener(pWin->getPropSink());
    pVM->addCommandListener(pWin->getCmdSink());
};