#include "gridmarker.h"

#define MARKER_COLOR "#FB78A5"           // color of marker
#define MARKER_SHADOW_COLOR "#E6CED6"    // color of marker shadow

const int duration = 200;

GridMarker::GridMarker(int size, QWidget *parent)
    : QLabel(parent), m_indent(2)
{
    int start = static_cast<int>(size * 0.2) - m_indent;
    m_maxSize = QRect(start, start, size - 2 * start, size - 2 * start);
    m_minSize = QRect(size / 2, size / 2, 1, 1);

    m_scaleAnimation = new QPropertyAnimation(this, "geometry");
    m_scaleAnimation->setEasingCurve(QEasingCurve::InOutQuad);
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
    if (size < m_indent * 2 + 1)
    {
        return;
    }

    QColor markShadowColor = QColor(MARKER_SHADOW_COLOR);
    QColor markColor = QColor(MARKER_COLOR);
    int alpha = 255 * size / m_maxSize.width();
    markShadowColor.setAlpha(alpha);
    markColor.setAlpha(alpha);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(markShadowColor));
    painter.drawEllipse(m_indent, m_indent * 2, size - 2 * m_indent, size - 2 * m_indent);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(markColor));
    painter.drawEllipse(m_indent, m_indent, size - 2 * m_indent, size - 2 * m_indent);
}
