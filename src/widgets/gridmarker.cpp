#include "gridmarker.h"

#include <QDebug>
#include <QPainter>
#include <QGraphicsOpacityEffect>

#define MARKER_COLOR "#FB78A5"
#define MARKER_SHADOW_COLOR "#E6CED6"

GridMarker::GridMarker(int size, QWidget *parent)
    : QLabel(parent), m_size(size), m_indent(2)
{
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
    opacityEffect->setOpacity(0.0);
    this->setGraphicsEffect(opacityEffect);

    int duration = 200;

    m_scaleAnimation = new QPropertyAnimation(this, "geometry");
    m_scaleAnimation->setStartValue(QRect(size / 2, size / 2, 0, 0));
    // 不加下面这句第一次显示错误，估计是reveal里currentValue获取的值不对
    m_scaleAnimation->setEndValue(QRect(size / 2, size / 2, 0, 0));
    m_scaleAnimation->setDuration(duration);
    m_scaleAnimation->setEasingCurve(QEasingCurve::OutQuad);

    // 透明度其实不是必须的
    // 但是控件在初始化时一定会绘制一次，而且控件大小就是layout大小
    // 所以要么在paintevent里加个flag，第一次不绘制，要么就加个初始透明度为0
    // 加透明度显示效果也很不错，就加上吧
    m_opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity", this);
    m_opacityAnimation->setDuration(duration);
    m_opacityAnimation->setStartValue(0.0);
    m_opacityAnimation->setEasingCurve(QEasingCurve::OutQuad);
}

void GridMarker::hide()
{
    if (m_scaleAnimation->state() != QAbstractAnimation::Stopped)
    {
        m_scaleAnimation->stop();
    }

    if (m_opacityAnimation->state() != QAbstractAnimation::Stopped)
    {
        m_opacityAnimation->stop();
    }

    m_scaleAnimation->setStartValue(m_scaleAnimation->currentValue());
    m_scaleAnimation->setEndValue(QRect(m_size / 2, m_size / 2, 0, 0));
    m_scaleAnimation->start();

    m_opacityAnimation->setStartValue(m_opacityAnimation->currentValue());
    m_opacityAnimation->setEndValue(0.0);
    m_opacityAnimation->start();
}

void GridMarker::reveal()
{
    if (m_scaleAnimation->state() != QAbstractAnimation::Stopped)
    {
        m_scaleAnimation->stop();
    }

    if (m_opacityAnimation->state() != QAbstractAnimation::Stopped)
    {
        m_opacityAnimation->stop();
    }

    int start = m_size * 0.2 - m_indent;

    m_scaleAnimation->setStartValue(m_scaleAnimation->currentValue());
    m_scaleAnimation->setEndValue(QRect(start, start, m_size - 2 * start, m_size - 2 * start));
    m_scaleAnimation->start();

    m_opacityAnimation->setStartValue(m_opacityAnimation->currentValue());
    m_opacityAnimation->setEndValue(0.999);
    m_opacityAnimation->start();
}

void GridMarker::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (width() < m_indent * 2)
    {
        painter.drawEllipse(1, 1, 0, 0);  // draw nothing
    }
    else
    {
        painter.setPen(QPen(QBrush(MARKER_SHADOW_COLOR), 1));
        painter.setBrush(QBrush(QColor(MARKER_SHADOW_COLOR)));
        painter.drawEllipse(m_indent, m_indent + 2, width() - 2 * m_indent, width() - 2 * m_indent);

        painter.setPen(QPen(QBrush(MARKER_COLOR), 1));
        painter.setBrush(QBrush(QColor(MARKER_COLOR)));
        painter.drawEllipse(m_indent, m_indent, width() - 2 * m_indent, width() - 2 * m_indent);

    }
}
