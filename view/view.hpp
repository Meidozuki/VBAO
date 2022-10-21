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
    std::shared_ptr<Displayer> displayer_;

    void setContent(Displayer &displayer);
};

#endif //LIBTEST_VIEW_VIEW_HPP_
