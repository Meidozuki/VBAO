#pragma once

#include <pybind11/pybind11.h>
namespace py = pybind11;

#include "VBAO/VBaoBase.hpp"

class PyCommandBase : public vbao::CommandVBase {
public:
    using CommandVBase::CommandVBase;

    void setParameter(const std::any &param) override{
        PYBIND11_OVERRIDE_PURE(void, vbao::CommandVBase, setParameter, param);
    }
    void execute() override{
        PYBIND11_OVERRIDE_PURE(void, vbao::CommandVBase, execute,);
    }
};