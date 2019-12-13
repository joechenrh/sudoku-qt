#include "hoverbutton.h"

#include <QGraphicsEffect>

const int duration = 200;
const double fullOpacity = 0.999;

HoverButton::HoverButton(QWidget *parent) : QLabel(parent)
{
    this->setAlignment(Qt::AlignCenter);

    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
    opacityEffect->setOpacity(fullOpacity);
    this->setGraphicsEffect(opacityEffect);

    m_animation = new QPropertyAnimation(opacityEffect, "opacity", this);
    m_animation->setDuration(duration);
    m_animation->setStartValue(opacityEffect->opacity());
    m_animation->setEasingCurve(QEasingCurve::InOutQuad);
}

void HoverButton::hide()
{
    m_animation->stop();
    m_animation->setStartValue(m_animation->currentValue());
    m_animation->setEndValue(0.0);
    m_animation->start();
}

void HoverButton::reveal()
{
    m_animation->stop();
    m_animation->setStartValue(m_animation->currentValue());
    m_animation->setEndValue(fullOpacity);
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
    else if (e->button() == Qt::MidButton)
    {
        emit midClicked();
    }
}
