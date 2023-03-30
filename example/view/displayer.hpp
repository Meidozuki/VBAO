#ifndef VIEW_H
#define VIEW_H

#include <memory>
#include <string>
#include <utility>

#include <QWidget>
#include <QLCDNumber>
#include <QSlider>

#include "VBAO/VBaoBase.hpp"

namespace Ui {
class Displayer;
}

class Displayer : public QWidget
{
    Q_OBJECT


public:
    explicit Displayer(QWidget *parent = nullptr);
    ~Displayer();

    void updateTime();
    void updateSumResult();

    std::shared_ptr<std::string> pNumber_;
    std::shared_ptr<int> pNumberA_,pNumberB_,pNumber_sum_;

    std::shared_ptr<vbao::CommandVBase> cmd_time_increase_, cmd_sum_;

    //默认的setter是const&，会在末端引用计数增加，仅绑定一次，接收右值，改用&&
    void setNumber(std::shared_ptr<std::string> &&sp) {
        pNumber_=sp;
    }

    void setPNumberA(const std::shared_ptr<int> &p_number_a) {
        pNumberA_ = p_number_a;
    }
    void setPNumberB(const std::shared_ptr<int> &p_number_b) {
        pNumberB_ = p_number_b;
    }
    void setPNumberSum(const std::shared_ptr<int> &p_number_sum) {
        pNumber_sum_ = p_number_sum;
    }


public slots:
    void onClick();
    void slideNumA(int value);
    void slideNumB(int value);
    void onSlideEnd();

private:
    Ui::Displayer *ui;
};

class View : public Displayer {

};


#endif // VIEW_H
