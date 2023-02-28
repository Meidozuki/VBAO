//
// Created by user on 2022/10/12.
//

#ifndef LIBTEST_VIEWMODEL_VIEWMODEL_HPP_
#define LIBTEST_VIEWMODEL_VIEWMODEL_HPP_

#include "common_parameter.h"

#include "VBAO/viewmodel.hpp"

#include "model/model.hpp"
#include "viewmodel_listener.hpp"
#include "viewmodel_command.hpp"

class Viewmodel : public vbao::TViewModelBase<Model> {
    friend class ViewmodelListener;
    friend class ViewmodelAddTimeCommand;
    friend class ViewmodelSumCommand;
    friend void init(Viewmodel *pVM,Window *pWin);
private:
    //viewmodel和window的数据用shared ptr绑定， model的数据通过sink事件传给vm
    std::shared_ptr<std::string> pNumber_;
    std::shared_ptr<int> pNumberA_,pNumberB_,pNumber_sum_;
    ViewmodelListener listener_;
    std::shared_ptr<ViewmodelAddTimeCommand> pCommand_;
    std::shared_ptr<ViewmodelSumCommand> pCommand_sum_;
private:
    //假设model持有数据的情况
    void addNumber_vm() {
        getModel()->addNumber();
    }
    //model只计算的情况
    void calSum_vm() {
        *pNumber_sum_ = getModel()->calSum(*pNumberA_, *pNumberB_);
        triggerPropertyNotification("sum");
    }
public:
    void setInitNumber(const std::string &num) {
        getModel()->setNumber(num);
    }

public:
    using VMBaseType::setModel;

    Viewmodel() : listener_(this) {
        pNumber_ = std::make_shared<std::string>();
        pNumberA_ = std::make_shared<int>();
        pNumberB_ = std::make_shared<int>();
        pNumber_sum_ = std::make_shared<int>();

        pCommand_ = std::make_shared<ViewmodelAddTimeCommand>(this);
        pCommand_sum_ = std::make_shared<ViewmodelSumCommand>(this);

        setListener(std::make_unique<ViewmodelListener>(this));
    }

    std::shared_ptr<std::string> getNumber() const {
        return pNumber_;
    }

    //下面两个不规范，先不写shared ptr了
    vbao::PropertyNotificationVBase* getListener() {
        return &listener_;
    }

    auto getCommand() {
        return std::static_pointer_cast<vbao::CommandVBase>(pCommand_);
    }
    auto getSumCommand() {
        return std::static_pointer_cast<vbao::CommandVBase>(pCommand_sum_);
    }
};

#endif //LIBTEST_VIEWMODEL_VIEWMODEL_HPP_
