//
// Created by user on 2022/10/13.
//

#include "viewmodel_command.hpp"

#include "viewmodel.hpp"

ViewmodelAddTimeCommand::ViewmodelAddTimeCommand(Viewmodel *vm) : TCommand(vm)
{
}

void ViewmodelAddTimeCommand::execute() {
    pVM_->addNumber_vm();
    pVM_->triggerCommandNotification("add",true);
}

ViewmodelSumCommand::ViewmodelSumCommand(Viewmodel *vm) : TCommand(vm)
{
}

void ViewmodelSumCommand::execute() {
    pVM_->calSum_vm();
    pVM_->triggerCommandNotification("sum",true);
}
