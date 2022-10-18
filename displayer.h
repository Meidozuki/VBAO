#ifndef VIEW_H
#define VIEW_H

#include <memory>
#include <string>

#include <QWidget>
#include <QLCDNumber>
#include <utility>

class View;


namespace Ui {
class Displayer;
}

class Displayer : public QWidget
{
    Q_OBJECT


public:
    explicit Displayer(QWidget *parent = nullptr);
    ~Displayer();

    View *view_;
    std::string init();

    void setNumber(std::shared_ptr<std::string> sp) {pNumber_=std::move(sp);}
    void updateNumber();
    std::shared_ptr<std::string> pNumber_;

public slots:
    void onClick();

private:
    Ui::Displayer *ui;
};

#endif // VIEW_H
