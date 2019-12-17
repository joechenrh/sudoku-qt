#include "gridmarker.h"

#include <QDebug>

GridMarker::GridMarker(int size, QWidget *parent)
    : QLabel(parent), m_indent(2)
{
    int start = static_cast<int>(size * 0.2) - m_indent;
    m_maxSize = QRect(start, start, size - 2 * start, size - 2 * start);
    m_minSize = QRect(size / 2, size / 2, 1, 1);

    m_showAnimation = new QPropertyAnimation(this, "geometry");
    m_showAnimation->setEasingCurve(QEasingCurve::InOutSine);
    m_showAnimation->setStartValue(m_minSize);
    m_showAnimation->setEndValue(m_maxSize);

    m_hideAnimation = new QPropertyAnimation(this, "geometry");
    m_hideAnimation->setEasingCurve(QEasingCurve::InOutSine);
    m_hideAnimation->setStartValue(m_maxSize);
    m_hideAnimation->setEndValue(m_minSize);

}

void GridMarker::setMarkerColor(const QColor &color)
{
    m_markerColor = color;
}

void GridMarker::setShadowColor(const QColor &color)
{
    m_shadowColor = color;
}

void GridMarker::setDuration(const int &duration)
{
    m_duration = duration;
    m_showAnimation->setDuration(duration);
    m_showAnimation->setCurrentTime(0);
    m_hideAnimation->setDuration(duration);
    m_hideAnimation->setCurrentTime(duration);
}

void GridMarker::hide()
{
    m_showAnimation->stop();
    m_hideAnimation->start();
    m_hideAnimation->setCurrentTime(m_duration - m_showAnimation->currentTime());
}

void GridMarker::show()
{  
    m_hideAnimation->stop();
    m_showAnimation->start();
    m_showAnimation->setCurrentTime(m_duration - m_hideAnimation->currentTime());
}

void GridMarker::paintEvent(QPaintEvent*)
{
    int size = width();
    if (size < m_indent * 2 + 1)
    {
        return;
    }

    int alpha = 255 * size / m_maxSize.width();
    m_shadowColor.setAlpha(alpha);
    m_markerColor.setAlpha(alpha);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(m_shadowColor));
    painter.drawEllipse(m_indent, m_indent * 2, size - 2 * m_indent, size - 2 * m_indent);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(m_markerColor));
    painter.drawEllipse(m_indent, m_indent, size - 2 * m_indent, size - 2 * m_indent);
}
