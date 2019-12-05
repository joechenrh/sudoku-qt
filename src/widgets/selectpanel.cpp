#include "selectpanel.h"

#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

SelectPanel* SelectPanel::m_instance = 0;
QMutex SelectPanel::m_mutex;

SelectPanel::SelectPanel(int size, QWidget *parent)
    : QDialog(parent), opened(false)
{
    m_buttons.resize(9);
    m_selected.resize(9);

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

            connect(button, &HoverButton::hovered,      [=](){button->setFont(hoverFont); button->setStyleSheet(enterStylesheet);});
            connect(button, &HoverButton::leaved,       [=](){button->setFont(normalFont); button->setStyleSheet(leaveStylesheet);});
            connect(button, &HoverButton::clicked,      [=](){emit finish(i * 3 + j + 1); this->close();});
            connect(button, &HoverButton::rightClicked, [=](){m_selected[i * 3 + j] = 1;});
        }
    }
}

SelectPanel* SelectPanel::instance(int size, QWidget* parent)
{
    if(!m_instance)
    {
        QMutexLocker lock(&m_mutex);
        if(!m_instance)
        {
            SelectPanel *pInstance = new SelectPanel(size, parent);  // 修改处
            m_instance = pInstance;                                  // 修改处
        }
    }
    return m_instance;
}

int SelectPanel::exec()
{
    opened = true;

    this->setWindowOpacity(0.0);
    QPropertyAnimation* anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(3000);
    anim->setEasingCurve(QEasingCurve::OutQuad);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

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
    anim->setEndValue(0.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    bool res;
    connect(anim, &QPropertyAnimation::destroyed, [&](){res = QDialog::close();});
    return res;
    */
    opened = false;
    QDialog::close();

    //this->setParent(nullptr);
    //m_base = nullptr;  // 加这句就会报错，暂时不清楚为什么
    return true;

}

bool SelectPanel::isOpened() const
{
    return opened;
}

int SelectPanel::number() const
{
    return selected;
}


void SelectPanel::enterEvent(QEvent *e)
{
    QApplication::sendEvent(m_base, e);
}

void SelectPanel::leaveEvent(QEvent *e)
{
    QApplication::sendEvent(m_base, e);
}

void SelectPanel::setBase(QWidget *widget)
{
    m_base = widget;
}
