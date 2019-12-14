#include "gridmarker.h"

GridMarker::GridMarker(int size, QWidget *parent)
    : QLabel(parent), m_indent(2)
{
    int start = static_cast<int>(size * 0.2) - m_indent;
    m_maxSize = QRect(start, start, size - 2 * start, size - 2 * start);
    m_minSize = QRect(size / 2, size / 2, 1, 1);

    m_scaleAnimation = new QPropertyAnimation(this, "geometry");
    m_scaleAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    m_scaleAnimation->setStartValue(m_minSize);
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
    m_scaleAnimation->setDuration(duration);
}

void GridMarker::hide()
{
    m_scaleAnimation->stop();
    m_scaleAnimation->setStartValue(m_scaleAnimation->currentValue());
    m_scaleAnimation->setEndValue(m_minSize);
    m_scaleAnimation->start();
}

void GridMarker::show()
{
    m_scaleAnimation->stop();
    m_scaleAnimation->setStartValue(m_scaleAnimation->currentValue());
    m_scaleAnimation->setEndValue(m_maxSize);
    m_scaleAnimation->start();
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
