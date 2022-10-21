//
// Created by user on 2022/10/18.
//

#include "view.hpp"

#include <cassert>

#include "displayer.h"

View::View():displayer_(std::make_shared<Displayer>()) {
    displayer_->view_ = this;

}
