#ifndef LOGINWINDOWS_H
#define LOGINWINDOWS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindows; }
QT_END_NAMESPACE

class LoginWindows : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindows(QWidget *parent = nullptr);
    ~LoginWindows();

public slots:
    void processUserInput();
private:
    Ui::LoginWindows *ui;
    static constexpr char USER_ID_LINE_EDIT_NAME[] = "userIdLineEdit";
};
#endif // LOGINWINDOWS_H
