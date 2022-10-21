//
// Created by user on 2022/9/19.
//

#pragma once

#include <stdexcept>

#include "VBaoBase.hpp"
namespace vbao {
using vbao::CommandVBase;

template<typename VMType, typename ArgType>
class TCommand : public vbao::CommandVBase{
public:
    using CommandType = TCommand<VMType,ArgType>;

    VMType *pVM_;
    ArgType arg_;

    explicit TCommand(VMType *vm): pVM_(vm), arg_() {}

    void setParameter(const std::any &param) override {
        arg_ = std::any_cast<ArgType>(param);
    }
//    virtual void execute() override;
};

template<typename VMType>
class TCommand<VMType,void>: public vbao::CommandVBase {

public:
    using CommandType = TCommand<VMType,void>;

    VMType *pVM_;

    explicit TCommand(VMType *vm): pVM_(vm) {}

    void setParameter(const std::any &param) override {
        throw std::runtime_error("You are not expected to set the parameter of `RetType func(void)`\n");
    }
//    virtual void execute() override;
};


}