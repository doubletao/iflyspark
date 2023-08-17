#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

#include "include/aikit_spark_api.h"
#include "include/aikit_biz_config.h"
#include "include/aikit_err.h"

//事件
static void chatTokenCount(AIKIT::AIChat_Handle* handle, const int& completionTokens, const int& promptTokens, const int& totalTokens)
{
    if (handle && handle->usrContext)
    {
        MainWindow *pThis = static_cast<MainWindow *>(handle->usrContext);
        QString strTxt = QString("---handleID[%1]abilityID[%2]completionTokens[%3]promptTokens[%4]totalTokens[%5]---\n")
                .arg(handle->handleID).arg(handle->abilityID)
                .arg(completionTokens).arg(promptTokens).arg(totalTokens);
        emit pThis->sigAppendRecv(strTxt);
    }
}
static void chatOnOutput(AIKIT::AIChat_Handle* handle, const char* role, const  char* content, const int& index)
{
    if (handle && handle->usrContext)
    {
        MainWindow *pThis = static_cast<MainWindow *>(handle->usrContext);
        QString strTxt = QString("content[%1]handleID[%2]abilityID[%3]role[%4]index[%5]\n")
                .arg(content).arg(handle->handleID).arg(handle->abilityID)
                .arg(role).arg(index);
        emit pThis->sigAppendRecv(strTxt);
    }
}
static void chatOnError(AIKIT::AIChat_Handle* handle, const int& err, const char* errDesc)
{
    if (handle && handle->usrContext)
    {
        MainWindow *pThis = static_cast<MainWindow *>(handle->usrContext);
        QString strTxt = QString("errDesc[%1]handleID[%2]abilityID[%3]err[%4]\n")
                .arg(errDesc).arg(handle->handleID).arg(handle->abilityID)
                .arg(err);
        emit pThis->sigAppendRecv(strTxt);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //SDK初始化参数配置
    AIKIT::AIKIT_Configurator::builder()
        .log()
        .logLevel(100) //设置日志等级，0:verbose
        .app()
        .appID("yourAppID")
        .apiKey("yourApiKey")
        .apiSecret("yourApiSecret");
    //SDK初始化
    int ret = AIKIT::AIKIT_Init();
    if (ret != 0) {
        QString strMsg = QString("AIKIT_Init failed:%1").arg(ret);
        QMessageBox::warning(this, "warning", strMsg);
        return;
    }
    //异步回调注册
    AIKIT::AIKIT_ChatCallback({ chatOnOutput, chatTokenCount, chatOnError });

    connect(this, &MainWindow::sigAppendRecv, this, &MainWindow::slotAppendRecv);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_clear_clicked()
{
    ui->txt_recv->clear();
    ui->txt_send->clear();
}


void MainWindow::on_btn_send_clicked()
{
    QString strSend = ui->txt_send->toPlainText();

    //请求参数配置
    AIKIT::ChatParam* config = AIKIT::ChatParam::builder();
    config->domain("general")
        ->uid("xxx_uid")
        ->maxToken(4096);

    int ret = AIKIT::AIKIT_AsyncChat(config, strSend.toStdString().c_str(), this);//文本数据输入输出以UTF-8编码格式
    if (ret != 0) {
        QString strMsg = QString("AIKIT_AsyncChat failed:%1,check appid key secret whether been set").arg(ret);
        QMessageBox::warning(this, "warning", strMsg);
    }
}

void MainWindow::slotAppendRecv(const QString &strTxt)
{
    ui->txt_recv->appendPlainText(strTxt);
}

