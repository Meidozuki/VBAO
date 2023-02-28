//
// Created by user on 2022/10/18.
//

#ifndef LIBTEST_VIEW_VIEW_HPP_
#define LIBTEST_VIEW_VIEW_HPP_

#include <memory>

#include "VBAO/VBaoBase.hpp"

class Displayer;

//不清楚套入qt体系是否会引发error，额外写了一个view类
//更像一个notifier
class View {
public:
    View();

    std::shared_ptr<vbao::CommandVBase> cmd_add_, cmd_sum_;
    std::shared_ptr<Displayer> displayer_;

};

#endif //LIBTEST_VIEW_VIEW_HPP_
