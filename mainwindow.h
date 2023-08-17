#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sigAppendRecv(const QString &);

private slots:
    void on_btn_clear_clicked();
    void on_btn_send_clicked();

    void slotAppendRecv(const QString &strTxt);

    void on_MainWindow_destroyed();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
