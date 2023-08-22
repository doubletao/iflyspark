#include "customplaintextedit.h"

CustomPlainTextEdit::CustomPlainTextEdit(QWidget *parent) : QPlainTextEdit(parent) {}

void CustomPlainTextEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return)
    {
        if (event->modifiers() == Qt::ControlModifier)
        {
            // Ctrl+回车，换行
            insertPlainText("\n");
        }
        else
        {
            // 单独的回车，发送
            emit sendButtonClicked();
        }
    }
    else
    {
        QPlainTextEdit::keyPressEvent(event);
    }
}
