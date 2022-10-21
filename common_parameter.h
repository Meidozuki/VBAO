//
// Created by user on 2022/10/12.
//

#ifndef LIBTEST__SMART_PTR_H_
#define LIBTEST__SMART_PTR_H_

#include <string>
#include <memory>

#include <QTime>

inline std::string init_time() {
    QTime time = QTime::currentTime();
    return time.toString("hh:mm").toStdString();
}

#endif //LIBTEST__SMART_PTR_H_
