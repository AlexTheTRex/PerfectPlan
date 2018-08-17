#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QDate>
#include "mymainscreen.h"

#include <iostream>

int main (int argc, char** argv) {

    QApplication app(argc, argv);
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&translator);
    MyMainScreen* fenetre = new MyMainScreen(QDate::currentDate());
    fenetre->showMaximized();
    return app.exec();
}
