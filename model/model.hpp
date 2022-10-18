//
// Created by user on 2022/10/11.
//

#ifndef LIBTEST_MODEL_MODEL_HPP
#define LIBTEST_MODEL_MODEL_HPP

#include <iostream>
#include <memory>
#include <string>

#include "VBAO/VBaoBase.hpp"
#include "VBAO/model.hpp"

class Model : public vbao::ModelBase {
public:
    std::string number_;

    const std::string &getNumber() const {
        return number_;
    }

    void setNumber(const std::string &s) {
        number_ = s;
        triggerPropertyNotification("number");
    }

    void addNumber();
};

#endif //LIBTEST_MODEL_MODEL_HPP
