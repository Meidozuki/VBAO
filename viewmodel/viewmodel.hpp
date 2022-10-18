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
private:
    //viewmodel和window的数据用shared ptr绑定， model的数据通过sink事件传给vm
    std::shared_ptr<std::string> pNumber_;
    ViewmodelListener listener_;
    std::shared_ptr<ViewmodelCommand> pCommand_;
private:

    void addNumber_vm() {
        getModel()->addNumber();
    }
public:
    void setInitNumber(const std::string &num) {
        getModel()->setNumber(num);
    }

public:
    friend class ViewmodelListener;
    friend class ViewmodelCommand;
    using VMBaseType::setModel;

    Viewmodel() : listener_(this) {
        pNumber_ = std::make_shared<std::string>();
        pCommand_ = std::make_shared<ViewmodelCommand>(this);
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
};

#endif //LIBTEST_VIEWMODEL_VIEWMODEL_HPP_
