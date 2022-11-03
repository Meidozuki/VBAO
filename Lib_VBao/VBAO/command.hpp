// This file is part of VBAO.
//
// VBAO is free software: you can redistribute it and/or modify it under the terms of
// the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// VBAO is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with VBAO.
// If not, see <https://www.gnu.org/licenses/>.

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