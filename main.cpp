#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("System");

    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
    MainWindow w;
    w.setWindowTitle("快捷设置时间");
    a.setObjectName("star");
    a.setOrganizationName("Stars");
    w.setFixedSize(295,243);
    w.show();
    return a.exec();
}
