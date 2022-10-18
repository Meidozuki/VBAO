//
// Created by user on 2022/10/13.
//

#ifndef LIBTEST_WINDOW_WINDOW_LISTENER_HPP_
#define LIBTEST_WINDOW_WINDOW_LISTENER_HPP_

#include "VBAO/listener.hpp"

class Window;

class WindowPropertyListener : public vbao::PropertyListener<Window> {
public:
    explicit WindowPropertyListener(Window *p);

    void onPropertyChanged(const std::string& prop_name) override;
};

class WindowCommandListener : public vbao::CommandListener<Window> {
public:
    explicit WindowCommandListener(Window *p);

    void onCommandComplete(const std::string& cmd_name, bool bOK) override {}
};

#endif //LIBTEST_WINDOW_WINDOW_LISTENER_HPP_
