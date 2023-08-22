QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    customplaintextedit.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    customplaintextedit.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

PRO_DIR = $$PWD

LIBS += -L$$PWD/libs/x64 -llibaikit

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SOURCE_DIR = $$OUT_PWD/release
DEST_DIR = $$PWD/../bin

# 定义创建目录的命令
MKDIR_COMMAND = $$QMAKE_MKDIR $$shell_path($$DEST_DIR)

# 在构建之前先创建目标文件夹
!exists($$DEST_DIR){
    QMAKE_PRE_LINK += $$MKDIR_COMMAND
}

# 定义拷贝文件的命令
COPY_COMMAND_EXE = $$QMAKE_COPY $$shell_path($$SOURCE_DIR/iFlySpark.exe) $$shell_path($$DEST_DIR/iFlySpark.exe)
COPY_COMMAND_DLL = $$QMAKE_COPY $$shell_path($$PWD/libs/x64/libaikit.dll) $$shell_path($$DEST_DIR/libaikit.dll)

# 在构建结束后执行拷贝文件的命令
QMAKE_POST_LINK += $$COPY_COMMAND_EXE
QMAKE_POST_LINK += & $$COPY_COMMAND_DLL
