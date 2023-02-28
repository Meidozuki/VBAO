//
// Created by user on 2022/10/11.
//

#ifndef LIBTEST_MODEL_MODEL_HPP
#define LIBTEST_MODEL_MODEL_HPP

#include <iostream>
#include <memory>
#include <string>
using namespace std::literals::string_literals;

#include "VBAO/model.hpp"


class Model : public vbao::ModelBase {
public:
    std::string number_;

    const std::string &getTimeNumber() const {
        return number_;
    }

    void setNumber(const std::string &s) {
        number_ = s;
        triggerPropertyNotification("time"s);
    }

    void addNumber();

    int calSum(int a,int b);
};

#endif //LIBTEST_MODEL_MODEL_HPP
