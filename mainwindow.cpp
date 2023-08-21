#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QScrollBar>

#include "include/aikit_spark_api.h"
#include "include/aikit_biz_config.h"
#include "include/aikit_err.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

//事件
static void chatTokenCount(AIKIT::AIChat_Handle* handle, const int& completionTokens, const int& promptTokens, const int& totalTokens)
{
    if (handle && handle->usrContext)
    {
        MainWindow *pThis = static_cast<MainWindow *>(handle->usrContext);
        QString strTxt = QString("\n-------------handleID[%1]abilityID[%2]completionTokens[%3]promptTokens[%4]totalTokens[%5]-----------\n\n")
                .arg(handle->handleID).arg(handle->abilityID)
                .arg(completionTokens).arg(promptTokens).arg(totalTokens);
        emit pThis->sigAppendRecv(strTxt);
    }
}
static void chatOnOutput(AIKIT::AIChat_Handle* handle, const char* /*role*/, const  char* content, const int& /*index*/)
{
    if (handle && handle->usrContext)
    {
        MainWindow *pThis = static_cast<MainWindow *>(handle->usrContext);
        QString strTxt = QString("%1")
                .arg(content);
        emit pThis->sigAppendRecv(strTxt);
    }
}
static void chatOnError(AIKIT::AIChat_Handle* handle, const int& err, const char* errDesc)
{
    if (handle && handle->usrContext)
    {
        MainWindow *pThis = static_cast<MainWindow *>(handle->usrContext);

        QString strErrDesc = "Not Found";
        auto it = pThis->m_mapErrInfo.find(err);
        if (it != pThis->m_mapErrInfo.end())
        {
            strErrDesc = it->second;
        }

        QString strTxt = QString("errDesc[%1]handleID[%2]abilityID[%3]err[%4]Desc[%5]\n\n")
                .arg(errDesc).arg(handle->handleID).arg(handle->abilityID)
                .arg(err).arg(strErrDesc);
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
        .appID("595da6f8")
        .apiKey("09c6299749695df87e5b60f4f67d4f39")
        .apiSecret("6e900846f024d7de85e8c5b53f2dd9d4");
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

    m_mapErrInfo.insert(std::make_pair(0,"成功"));
    m_mapErrInfo.insert(std::make_pair(10000,"升级为ws出现错误"));
    m_mapErrInfo.insert(std::make_pair(10001,"通过ws读取用户的消息出错"));
    m_mapErrInfo.insert(std::make_pair(10002,"通过ws向用户发送消息出错"));
    m_mapErrInfo.insert(std::make_pair(10003,"用户的消息格式有错误"));
    m_mapErrInfo.insert(std::make_pair(10004,"用户数据的schema错误"));
    m_mapErrInfo.insert(std::make_pair(10005,"用户参数值有错误"));
    m_mapErrInfo.insert(std::make_pair(10006,"用户并发错误：当前用户已连接，同一用户不能多处同时连接。"));
    m_mapErrInfo.insert(std::make_pair(10007,"用户流量受限：服务正在处理用户当前的问题，需等待处理完成后再发送新的请求。（必须要等大模型完全回复之后，才能发送下一个问题）"));
    m_mapErrInfo.insert(std::make_pair(10008,"服务容量不足，联系工作人员"));
    m_mapErrInfo.insert(std::make_pair(10009,"和引擎建立连接失败"));
    m_mapErrInfo.insert(std::make_pair(10010,"接收引擎数据的错误"));
    m_mapErrInfo.insert(std::make_pair(10011,"发送数据给引擎的错误"));
    m_mapErrInfo.insert(std::make_pair(10012,"引擎内部错误"));
    m_mapErrInfo.insert(std::make_pair(10013,"输入内容审核不通过，涉嫌违规，请重新调整输入内容"));
    m_mapErrInfo.insert(std::make_pair(10014,"输出内容涉及敏感信息，审核不通过，后续结果无法展示给用户"));
    m_mapErrInfo.insert(std::make_pair(10015,"appid在黑名单中"));
    m_mapErrInfo.insert(std::make_pair(10016,"appid授权类的错误。比如：未开通此功能，未开通对应版本，token不足，并发超过授权 等等"));
    m_mapErrInfo.insert(std::make_pair(10017,"清除历史失败"));
    m_mapErrInfo.insert(std::make_pair(10019,"表示本次会话内容有涉及违规信息的倾向；建议开发者收到此错误码后给用户一个输入涉及违规的提示"));
    m_mapErrInfo.insert(std::make_pair(10110,"服务忙，请稍后再试"));
    m_mapErrInfo.insert(std::make_pair(10163,"请求引擎的参数异常 引擎的schema 检查不通过"));
    m_mapErrInfo.insert(std::make_pair(10222,"引擎网络异常"));
    m_mapErrInfo.insert(std::make_pair(10907,"token数量超过上限。对话历史+问题的字数太多，需要精简输入"));
    m_mapErrInfo.insert(std::make_pair(11200,"授权错误：该appId没有相关功能的授权 或者 业务量超过限制"));
    m_mapErrInfo.insert(std::make_pair(11201,"授权错误：日流控超限。超过当日最大访问量的限制"));
    m_mapErrInfo.insert(std::make_pair(11202,"授权错误：秒级流控超限。秒级并发超过授权路数限制"));
    m_mapErrInfo.insert(std::make_pair(11203,"授权错误：并发流控超限。并发路数超过授权路数限制"));
    m_mapErrInfo.insert(std::make_pair(18300,"sdk不可用"));
    m_mapErrInfo.insert(std::make_pair(18301,"sdk没有初始化"));
    m_mapErrInfo.insert(std::make_pair(18302,"sdk初始化失败"));
    m_mapErrInfo.insert(std::make_pair(18303,"sdk已经初始化"));
    m_mapErrInfo.insert(std::make_pair(18304,"sdk不合法参数"));
    m_mapErrInfo.insert(std::make_pair(18305,"sdk会话handle为空"));
    m_mapErrInfo.insert(std::make_pair(18306,"sdk会话未找到"));
    m_mapErrInfo.insert(std::make_pair(18307,"sdk会话重复终止"));
    m_mapErrInfo.insert(std::make_pair(18308,"超时错误"));
    m_mapErrInfo.insert(std::make_pair(18309,"sdk正在初始化中"));
    m_mapErrInfo.insert(std::make_pair(18310,"sdk会话重复开启"));
    m_mapErrInfo.insert(std::make_pair(18400,"工作目录无写权限"));
    m_mapErrInfo.insert(std::make_pair(18402,"文件打开失败"));
    m_mapErrInfo.insert(std::make_pair(18403,"内存分配失败"));
    m_mapErrInfo.insert(std::make_pair(18500,"未找到该参数key"));
    m_mapErrInfo.insert(std::make_pair(18501,"参数范围溢出，不满足约束条件"));
    m_mapErrInfo.insert(std::make_pair(18502,"sdk初始化参数为空"));
    m_mapErrInfo.insert(std::make_pair(18503,"sdk初始化参数中appid为空"));
    m_mapErrInfo.insert(std::make_pair(18504,"sdk初始化参数中apiKey为空"));
    m_mapErrInfo.insert(std::make_pair(18505,"sdk初始化参数中apiSecret为空"));
    m_mapErrInfo.insert(std::make_pair(18507,"input参数为空"));
    m_mapErrInfo.insert(std::make_pair(18509,"必填参数缺失"));
    m_mapErrInfo.insert(std::make_pair(18700,"通用网络错误"));
    m_mapErrInfo.insert(std::make_pair(18701,"网路不通"));
    m_mapErrInfo.insert(std::make_pair(18702,"网关检查不过"));
    m_mapErrInfo.insert(std::make_pair(18712,"http 404错误"));
    m_mapErrInfo.insert(std::make_pair(18801,"连接建立出错"));
    m_mapErrInfo.insert(std::make_pair(18802,"结果等待超时"));
    m_mapErrInfo.insert(std::make_pair(18311,"sdk同一能力并发路数超出最大限制"));
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

    QString strVersion = ui->comboBox_version->currentText();

    //请求参数配置
    AIKIT::ChatParam* config = AIKIT::ChatParam::builder();
    if (strVersion == "v1.5")
    {
        config->domain("general")
            ->uid("xxx_uid")
            ->url("ws://spark-api.xf-yun.com/v1.1/chat")
            ->maxToken(4096);
    }
    else if (strVersion == "v2")
    {
        config->domain("generalv2")
            ->uid("xxx_uid")
            ->url("ws://spark-api.xf-yun.com/v2.1/chat")
            ->maxToken(4096);
    }

    emit sigAppendRecv("<<<<<" + strSend + "\n>>>>>");
    ui->txt_send->clear();

    int ret = AIKIT::AIKIT_AsyncChat(config, strSend.toStdString().c_str(), this);//文本数据输入输出以UTF-8编码格式
    if (ret != 0) {
        QString strMsg = QString("AIKIT_AsyncChat failed:%1,check appid key secret whether been set").arg(ret);
        QMessageBox::warning(this, "warning", strMsg);
    }
}

void MainWindow::slotAppendRecv(const QString &strTxt)
{
    QString strText = ui->txt_recv->toPlainText();
    strText.append(strTxt);
    ui->txt_recv->setPlainText(strText);

    QScrollBar* scrollbar = ui->txt_recv->verticalScrollBar();
    scrollbar->setValue(scrollbar->maximum());
}


void MainWindow::on_MainWindow_destroyed()
{
    AIKIT::AIKIT_UnInit();
}

