#ifndef FONT_H
#define FONT_H

#include <QDebug>
#include <QString>
#include <QFont>
#include <QFontDatabase>
#include <QStringList>

QFont loadFont()
{
    QFont font;
    qDebug() << "load font0";
    int nIndex = QFontDatabase::addApplicationFont(":/sudoku/fonts/ARLRDBD.TTF");
    if (nIndex != -1)
    {
        qDebug() << "load font1";
        QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
        if (strList.count() > 0)
        {
           qDebug() << "load font2";
           font = QFont(strList.at(0));
        }
    }
    return font;
}

#endif // FONT_H
