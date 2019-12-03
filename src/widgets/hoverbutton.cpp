#include "hoverbutton.h"

#include <QGraphicsEffect>

HoverButton::HoverButton(QWidget *parent)
    : QPushButton(parent), entered(false)
{
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
    opacityEffect->setOpacity(0.99);
    this->setGraphicsEffect(opacityEffect);

    m_animation = new QPropertyAnimation(opacityEffect, "opacity", this);
    m_animation->setDuration(100);
    m_animation->setStartValue(0.99);
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

void HoverButton::vis()
{
    if (m_animation->state() != QAbstractAnimation::Stopped)
    {
        m_animation->stop();
    }
    m_animation->setStartValue(m_animation->currentValue());
    m_animation->setEndValue(0.99);
    m_animation->start();
}

void HoverButton::setOpacity(double oppacity)
{
    ((QGraphicsOpacityEffect*)(this->graphicsEffect()))->setOpacity(oppacity);
}

void HoverButton::enterEvent( QEvent* e )
{
    entered = true;

    emit hovered();

    // don't forget to forward the event
    QWidget::enterEvent( e );
}

void HoverButton::leaveEvent( QEvent* e )
{
    entered = false;

    emit leave();

    // don't forget to forward the event
    QWidget::enterEvent( e );
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
