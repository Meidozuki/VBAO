//
// Created by user on 2022/10/13.
//

#ifndef LIBTEST_VIEWMODEL_VIEWMODEL_COMMAND_HPP_
#define LIBTEST_VIEWMODEL_VIEWMODEL_COMMAND_HPP_

#include "VBAO/command.hpp"

class Viewmodel;

class ViewmodelCommand : public vbao::TCommand<Viewmodel,void> {
public:
    explicit ViewmodelCommand(Viewmodel *vm);

    void execute() override;
};

#endif //LIBTEST_VIEWMODEL_VIEWMODEL_COMMAND_HPP_
