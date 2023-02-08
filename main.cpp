/*
 The current running version is built on top of Qt 6.4.0
 The comments have expicitly referred to code that belong in other versions of Qt (Qt 5.15.2)
 Comment/uncomment relative code before testing and deployment

 @Author:   Yuelin Xin
 @file:     main.cpp
 @time:     2022-10-04(yyyy-mm-dd)
*/



#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    static const char ENV_VAR_QT_DEVICE_PIXEL_RATIO[] = "QT_DEVICE_PIXEL_RATIO";
    if (!qEnvironmentVariableIsSet(ENV_VAR_QT_DEVICE_PIXEL_RATIO)
            && !qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR")
            && !qEnvironmentVariableIsSet("QT_SCALE_FACTOR")
            && !qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS")) {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    }
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/appicon/videos.icns"));
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    MainWindow w;
    w.setWindowTitle("CV Pro Player");
    w.show();

    return a.exec();
}
