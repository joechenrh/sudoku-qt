#include "gridmarker.h"

#define MARKER_COLOR "#FB78A5"           // 标记本身的颜色
#define MARKER_SHADOW_COLOR "#E6CED6"    // 标记阴影的颜色

const int duration = 150;

GridMarker::GridMarker(int size, QWidget *parent)
    : QLabel(parent), m_indent(2)
{
    int start = static_cast<int>(size * 0.2) - m_indent;
    m_maxSize = QRect(start, start, size - 2 * start, size - 2 * start);
    m_minSize = QRect(size / 2, size / 2, 0, 0);

    m_scaleAnimation = new QPropertyAnimation(this, "geometry");
    m_scaleAnimation->setStartValue(m_minSize);
    m_scaleAnimation->setDuration(duration);
}

void GridMarker::hide()
{
    if (m_scaleAnimation->state() != QAbstractAnimation::Stopped)
    {
        m_scaleAnimation->stop();
    }

    m_scaleAnimation->setStartValue(m_scaleAnimation->currentValue());
    m_scaleAnimation->setEndValue(m_minSize);
    m_scaleAnimation->start();
}

void GridMarker::reveal()
{
    if (m_scaleAnimation->state() != QAbstractAnimation::Stopped)
    {
        m_scaleAnimation->stop();
    }

    m_scaleAnimation->setStartValue(m_scaleAnimation->currentValue());
    m_scaleAnimation->setEndValue(m_maxSize);
    m_scaleAnimation->start();
}

void GridMarker::paintEvent(QPaintEvent*)
{
    int size = width();
    if (size < m_indent * 2)
    {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(MARKER_SHADOW_COLOR)));
    painter.drawEllipse(m_indent, m_indent * 2, size - 2 * m_indent, size - 2 * m_indent);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(MARKER_COLOR)));
    painter.drawEllipse(m_indent, m_indent, size - 2 * m_indent, size - 2 * m_indent);
}
