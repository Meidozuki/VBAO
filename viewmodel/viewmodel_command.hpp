//
// Created by user on 2022/10/13.
//

#ifndef LIBTEST_VIEWMODEL_VIEWMODEL_COMMAND_HPP_
#define LIBTEST_VIEWMODEL_VIEWMODEL_COMMAND_HPP_

#include "VBAO/command.hpp"

class Viewmodel;

class ViewmodelAddTimeCommand : public vbao::TCommand<Viewmodel, void> {
public:
    explicit ViewmodelAddTimeCommand(Viewmodel *vm);

    void execute() override;
};

class ViewmodelSumCommand : public vbao::TCommand<Viewmodel, void> {
public:
    explicit ViewmodelSumCommand(Viewmodel *vm);

    void execute() override;
};

#endif //LIBTEST_VIEWMODEL_VIEWMODEL_COMMAND_HPP_
