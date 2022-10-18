//
// Created by user on 2022/10/18.
//

#include "view.hpp"

#include <cassert>

#include "displayer.h"

View::View() {

}
void View::setContent(Displayer &displayer) {
    displayer_ = &displayer;
    displayer.view_ = this;
}

void View::init() {
    assert(displayer_ != nullptr);
    displayer_->init();
}
