#include "hoverbutton.h"

#include <QGraphicsEffect>

HoverButton::HoverButton(QWidget *parent) : QPushButton(parent)
{
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
    opacityEffect->setOpacity(0.999);
    this->setGraphicsEffect(opacityEffect);

    m_animation = new QPropertyAnimation(opacityEffect, "opacity", this);
    m_animation->setDuration(100);
    m_animation->setStartValue(0.999);
    m_animation->setEasingCurve(QEasingCurve::Linear);
}

void HoverButton::hide()
{
    if (m_animation->state() != QAbstractAnimation::Stopped)
    {
        m_animation->stop();
    }

    m_animation->setStartValue(m_animation->currentValue());
    m_animation->setEndValue(0.0);
    m_animation->start();
}

void HoverButton::reveal()
{
    if (m_animation->state() != QAbstractAnimation::Stopped)
    {
        m_animation->stop();
    }

    m_animation->setStartValue(m_animation->currentValue());
    m_animation->setEndValue(0.99);
    m_animation->start();
}

void HoverButton::enterEvent(QEvent*)
{
    emit hovered();
}

void HoverButton::leaveEvent(QEvent*)
{
    emit leaved();
}

void HoverButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
    {
        emit rightClicked();
    }
    else if(e->button() == Qt::LeftButton)
    {
        emit clicked();
    }
}
