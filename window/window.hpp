//
// Created by user on 2022/10/13.
//

#ifndef LIBTEST_WINDOW_WINDOW_HPP_
#define LIBTEST_WINDOW_WINDOW_HPP_

#include <memory>

#include "VBAO/VBaoBase.hpp"

#include "view/view.hpp"
#include "view/displayer.h"
#include "window_listener.hpp"

class Window  {
public:
    View view_;

    std::shared_ptr<vbao::CommandVBase> cmd_add_;

    std::shared_ptr<WindowPropertyListener> prop_sink_;
    std::unique_ptr<WindowCommandListener> cmd_sink_;

    vbao::CommandNotificationVBase *getCmdSink() const {
        return cmd_sink_.get();
    }
    vbao::PropertyNotificationVBase* getPropSink() const {
        return prop_sink_.get();
    }

    Window() :cmd_add_(nullptr) {
        prop_sink_=std::make_shared<WindowPropertyListener>(this);
        cmd_sink_=std::make_unique<WindowCommandListener>(this);
    }

    void setCmdAdd(std::shared_ptr<vbao::CommandVBase> cmd) {
        cmd_add_ = std::move(cmd);
    }


};

#endif //LIBTEST_WINDOW_WINDOW_HPP_
