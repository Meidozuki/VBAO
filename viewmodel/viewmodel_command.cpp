//
// Created by user on 2022/10/13.
//

#include "viewmodel_command.hpp"

#include "viewmodel.hpp"

ViewmodelCommand::ViewmodelCommand(Viewmodel *vm) : TCommand(vm)
{
}

void ViewmodelCommand::execute() {
    pVM_->addNumber_vm();
    pVM_->triggerCommandNotification("add",true);
}
