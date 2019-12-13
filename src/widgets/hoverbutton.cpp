#include "hoverbutton.h"

#include <QGraphicsEffect>

const int duration = 200;
const double fullOpacity = 0.999;

HoverButton::HoverButton(QWidget *parent) : QPushButton(parent)
{
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
    opacityEffect->setOpacity(fullOpacity);
    this->setGraphicsEffect(opacityEffect);

    m_animation = new QPropertyAnimation(opacityEffect, "opacity", this);
    m_animation->setDuration(duration);
    m_animation->setStartValue(opacityEffect->opacity());
    m_animation->setEasingCurve(QEasingCurve::InOutQuad);

    m_baseStyleSheet = QString("%1 background-color: rgb(%1, %2, %3);");
    m_styleSheet = QString("");
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
    m_animation->setEndValue(fullOpacity);
    m_animation->start();
}

void HoverButton::setColor(const QColor &color)
{
    m_color = color;
    QWidget::setStyleSheet(m_baseStyleSheet
                          .arg(m_styleSheet)
                          .arg(color.red())
                          .arg(color.green())
                          .arg(color.blue()));
}

void HoverButton::setStyleSheet(const QString &styleSheet)
{
    m_styleSheet = styleSheet;
    QWidget::setStyleSheet(m_baseStyleSheet
                          .arg(styleSheet)
                          .arg(m_color.red())
                          .arg(m_color.green())
                          .arg(m_color.blue()));
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
