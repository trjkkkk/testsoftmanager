#ifndef LOGIN
#define LOGIN

#include <QWidget>
#include<QLineEdit>
//extern QString DatabaseName;
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

private:
    Ui::Form *fui;

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

    //QString DatabaseName;

    //QString getDabaName();

private slots:
    void on_InMainButton_clicked();
    void on_comboBox_currentTextChanged(const QString &arg1);


signals:
    void sendData(QString);   //用来传递数据的信号
};

#endif // LOGIN

