﻿#include "selectpanel.h"
#include "hoverbutton.h"

#include <QDebug>
#include <QPainter>
#include <QPropertyAnimation>

SelectPanel::SelectPanel(int size, QWidget *parent)
    : QDialog(parent)
{
    m_buttons.resize(9);

    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFixedSize(size, size);

    QFont hoverFont("华文新魏", 16);
    hoverFont.setBold(true);

    QFont normalFont("华文新魏", 12);

    QString enterStylesheet = "QWidget{color:#FAFAFA; background-color:#FB78A5; border:0px solid black;}";
    QString leaveStylesheet = "QWidget{color:#FBDFE8; background-color:#FB78A5; border:0px solid black;}";

    int buttonSize = size / 3;
    for( int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            HoverButton *button = new HoverButton(this);
            button->move(buttonSize * j, buttonSize * i);
            button->setFixedSize(buttonSize, buttonSize);
            button->setFont(normalFont);
            button->setStyleSheet(leaveStylesheet);
            button->setText(QString::number(i * 3 + j + 1));
            m_buttons[i * 3 + j] = button;

            connect(button, &HoverButton::hovered, [=](){button->setFont(hoverFont); button->setStyleSheet(enterStylesheet);});
            connect(button, &HoverButton::leave,   [=](){button->setFont(normalFont); button->setStyleSheet(leaveStylesheet);});
            connect(button, &HoverButton::clicked, [=](){selected = i * 3 + j + 1; this->close();});
            connect(button, &HoverButton::rightClicked, [=](){selected = 0; this->close();});
        }
    }
}

int SelectPanel::exec()
{
    /*
    this->setWindowOpacity(0.0);
    QPropertyAnimation* anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(300);
    anim->setEasingCurve(QEasingCurve::OutQuad);
    anim->setStartValue(0.5);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    */
    return QDialog::exec();
}

bool SelectPanel::close()
{
    /*
    this->setWindowOpacity(1.0);
    QPropertyAnimation* anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(100);
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->setStartValue(1.0);
    anim->setEndValue(0.5);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    bool res;
    connect(anim, &QPropertyAnimation::destroyed, [&](){res = QDialog::close();});
    return res
    */
    return QDialog::close();
}

int SelectPanel::number()
{
    return selected;
}