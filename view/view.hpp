//
// Created by user on 2022/10/18.
//

#ifndef LIBTEST_VIEW_VIEW_HPP_
#define LIBTEST_VIEW_VIEW_HPP_

#include <memory>

#include "VBAO/VBaoBase.hpp"

class Displayer;

//更像一个notifier
class View {
public:
    View();

    std::shared_ptr<vbao::CommandVBase> cmd_add_;
    Displayer* displayer_;

    void setContent(Displayer &displayer);
    void init();
};

#endif //LIBTEST_VIEW_VIEW_HPP_
