#include "panelbase.h"

#include <QPainter>

PanelBase::PanelBase(int size, QWidget *parent)
    : QLabel(parent), m_size(size) { }

void PanelBase::setColor(QString color)
{
    m_color = QColor(color);
}

void PanelBase::paintEvent(QPaintEvent*)
{
    int alpha = std::min(255 * width() / m_size + 0, 255);
    // int alpha = std::min(255 * width() / 45 + 0, 255);
    m_color.setAlpha(alpha);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(m_color));
    painter.drawEllipse(0, 0, width(), width());

}
