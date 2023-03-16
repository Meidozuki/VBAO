#include "app.hpp"

#include "model/model.hpp"

void init(Viewmodel *pVM,Window *pWin){

    //bindings
    pVM->setModel(std::make_unique<Model>());
    //prop
    pWin->view_.setNumber(pVM->getNumber());
    pWin->view_.setPNumberA(pVM->pNumberA_);
    pWin->view_.setPNumberB(pVM->pNumberB_);
    pWin->view_.setPNumberSum(pVM->pNumber_sum_);
    //command
    pWin->setCmdAdd(pVM->getCommand());
    pWin->view_.cmd_add_ = pVM->getCommand();
    pWin->view_.cmd_sum_ = pVM->getSumCommand();
    //notification
    pVM->addPropertyListener(pWin->getPropSink());
    pVM->addCommandListener(pWin->getCmdSink());
};