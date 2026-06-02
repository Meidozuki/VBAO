#ifndef VIEW_H
#define VIEW_H

#include <string>

#include <QWidget>
#include <QLCDNumber>

namespace Ui {
class View;
}

class View : public QWidget
{
    Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr);
    ~View();


private slots:
    void addNumber();

private:
    Ui::View *ui;
    std::string number_;
};

#endif // VIEW_H
