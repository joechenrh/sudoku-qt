#include "basewidget.h"

const int duration = 200;
const double fullOpacity = 0.999;

BaseWidget::BaseWidget(QWidget *parent)
    : QLabel(parent)
{
    this->setAlignment(Qt::AlignCenter);

    m_opacity = new QGraphicsOpacityEffect;
    m_opacity->setOpacity(fullOpacity);
    this->setGraphicsEffect(m_opacity);

    m_animation = new QPropertyAnimation(m_opacity, "opacity", this);
    m_animation->setDuration(duration);
    m_animation->setStartValue(m_opacity->opacity());
    m_animation->setEasingCurve(QEasingCurve::InOutQuad);

    m_scaleAnimation = new QPropertyAnimation(this, "fontPointSize");
    m_scaleAnimation->setDuration(duration / 2);
    m_scaleAnimation->setEasingCurve(QEasingCurve::OutCubic);
}


void BaseWidget::setFont(const QFont &f)
{
    QWidget::setFont(f);
    m_fontSize = font().pointSizeF();
    m_scaleAnimation->setStartValue(m_fontSize);
    m_scaleAnimation->setEndValue(m_fontSize * 1.25);
}

double BaseWidget::fontSize() const
{
    return font().pointSizeF();
}

void BaseWidget::setFontSize(double size)
{
    QFont newFont = font();
    newFont.setPointSizeF(size);
    QWidget::setFont(newFont);
}

void BaseWidget::zoomIn()
{
    m_scaleAnimation->stop();
    m_scaleAnimation->setStartValue(m_scaleAnimation->currentValue());
    m_scaleAnimation->setEndValue(m_fontSize * 1.25);
    m_scaleAnimation->start();
}

void BaseWidget::zoomOut()
{
    m_scaleAnimation->stop();
    m_scaleAnimation->setStartValue(m_scaleAnimation->currentValue());
    m_scaleAnimation->setEndValue(m_fontSize);
    m_scaleAnimation->start();
}


void BaseWidget::hide()
{
    m_animation->stop();
    m_animation->setStartValue(m_opacity->opacity());
    m_animation->setEndValue(0.0);
    m_animation->start();

    /*
    auto opacity = m_opacity->opacity();
    m_animation->stop();
    m_animation->setStartValue(fullOpacity);
    m_animation->setEndValue(0.0);
    m_animation->start();
    m_animation->setCurrentTime(duration * (1.0 - opacity));
    */
}

void BaseWidget::reveal()
{
    m_animation->stop();
    m_animation->setStartValue(m_opacity->opacity());
    m_animation->setEndValue(fullOpacity);
    m_animation->start();

    /*
    auto opacity = m_opacity->opacity();
    m_animation->stop();
    m_animation->setStartValue(0.0);
    m_animation->setEndValue(fullOpacity);
    m_animation->start();
    m_animation->setCurrentTime(duration * (1.0 - opacity));
    */
}

double BaseWidget::opacity() const
{
    return m_opacity->opacity();
}

void BaseWidget::setOpacity(double opacity)
{
    m_opacity->setOpacity(opacity);
}


void BaseWidget::enterEvent(QEvent*)
{
    emit hovered();
}

void BaseWidget::leaveEvent(QEvent*)
{
    emit leaved();
}

void BaseWidget::mousePressEvent(QMouseEvent *e)
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
