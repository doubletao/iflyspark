#ifndef CUSTOMPLAINTEXTEDIT_H
#define CUSTOMPLAINTEXTEDIT_H

#include <QPlainTextEdit>

class CustomPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    CustomPlainTextEdit(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

public:
    signals:
    void sendButtonClicked();
};

#endif // CUSTOMPLAINTEXTEDIT_H
