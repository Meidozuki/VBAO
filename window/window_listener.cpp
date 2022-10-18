//
// Created by user on 2022/10/13.
//

#include "window_listener.hpp"

#include "window.hpp"

WindowPropertyListener::WindowPropertyListener(Window *p) : PropertyListener(p)
{
}

void WindowPropertyListener::onPropertyChanged(const std::string &prop_name) {
    if (prop_name == "number") {
        pNotify_->view_.displayer_->updateNumber();
    }
}
WindowCommandListener::WindowCommandListener(Window *p) : CommandListener(p)
{

}
