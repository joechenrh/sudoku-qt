#include "circlewidget.h"

#include <QLabel>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QGraphicsOpacityEffect>

Counter::Counter(int num, int size, QWidget *parent) : QWidget(parent), m_count(9)
{
    m_baseStyle = "min-width: %1px;"
                  "min-height: %2px;"
                  "max-width:%3px;"
                  "max-height: %4px;"
                  "border-radius: %5px;"
                  "background: %6;"
                  "color:#5F5F5F";

    m_lr = size * 0.5;
    int d1 = m_lr * 2;
    m_sr = size * 0.2;
    int d2 = m_sr * 2;

    QFont cntFont("华文新魏", 15);
    cntFont.setBold(true);

    QFont numFont("华文新魏", 10);
    numFont.setBold(true);

    QString numStyle = m_baseStyle.arg(d2).arg(d2).arg(d2).arg(d2).arg(m_sr).arg("#E66F99");
    QString cntStyle = m_baseStyle.arg(d1).arg(d1).arg(d1).arg(d1).arg(m_lr).arg("#F1F1F1");

    this->setFixedSize(size, size);

    m_cntLabel = new QLabel(this);
    m_cntLabel->setText("9");
    m_cntLabel->setFont(cntFont);
    m_cntLabel->setStyleSheet(cntStyle);
    m_cntLabel->setAlignment(Qt::AlignCenter);
    m_cntLabel->setFixedSize(m_lr, m_lr);
    m_cntLabel->move(0, size - d1);

    m_numLabel = new QLabel(this);
    m_numLabel->setText(QString::number(num));
    m_numLabel->setFont(numFont);
    m_numLabel->setStyleSheet(numStyle);
    m_numLabel->setAlignment(Qt::AlignCenter);
    m_numLabel->setFixedSize(m_sr, m_sr);
    m_numLabel->move(size - d2, 0);

    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
    m_numLabel->setGraphicsEffect(opacityEffect);
    opacityEffect->setOpacity(1.0);
}

void Counter::enterEvent(QEvent *e)
{
    // count为0时，鼠标移动时不会修改颜色
    if (m_count > 0)
    {
        int d = m_lr * 2;
        QString cntStyle = m_baseStyle.arg(d).arg(d).arg(d).arg(d).arg(m_lr).arg("#FADFE8");
        m_cntLabel->setStyleSheet(cntStyle);
    }
    emit hovered();

    QWidget::enterEvent( e );
}

void Counter::leaveEvent(QEvent *e)
{
    if (m_count > 0)
    {
        int d = m_lr * 2;
        QString cntStyle = m_baseStyle.arg(d).arg(d).arg(d).arg(d).arg(m_lr).arg("#F1F1F1");
        m_cntLabel->setStyleSheet(cntStyle);
    }
    emit leave();

    QWidget::enterEvent( e );
}

void Counter::minus()
{
    --m_count;
    if (m_count < 1)
    {
        m_cntLabel->setText("0");
        int d = m_lr * 2;
        QString numStyle = m_baseStyle.arg(d).arg(d).arg(d).arg(d).arg(m_lr).arg("#E5E0E5");
        m_cntLabel->setStyleSheet(numStyle);
    }
    else
    {
        m_cntLabel->setText(QString::number(m_count));
    }

    this->updateOpacity();
}

void Counter::plus()
{
    ++m_count;
    if (m_count < 1)
    {
        m_cntLabel->setText("0");
    }
    else
    {
        m_cntLabel->setText(QString::number(m_count));
        int d = m_lr * 2;
        QString numStyle = m_baseStyle.arg(d).arg(d).arg(d).arg(d).arg(m_lr).arg("#F1F1F1");
        m_cntLabel->setStyleSheet(numStyle);
    }

    this->updateOpacity();
}

void Counter::setValue(int value)
{
    m_count = value;
    m_cntLabel->setText(QString::number(m_count));
    this->updateOpacity();
    if (m_count == 0)
    {
        int d = m_lr * 2;
        QString numStyle = m_baseStyle.arg(d).arg(d).arg(d).arg(d).arg(m_lr).arg("#E5E0E5");
        m_cntLabel->setStyleSheet(numStyle);
    }
    else
    {
        int d = m_lr * 2;
        QString numStyle = m_baseStyle.arg(d).arg(d).arg(d).arg(d).arg(m_lr).arg("#F1F1F1");
        m_cntLabel->setStyleSheet(numStyle);
    }
}

void Counter::clear()
{
    m_count = 9;
    m_cntLabel->setText(QString::number(m_count));
    this->updateOpacity();
}

void Counter::updateOpacity()
{
    if (m_count <= 0)
    {
        ((QGraphicsOpacityEffect*)(m_numLabel->graphicsEffect()))->setOpacity(0.0);
    }
    else
    {
        ((QGraphicsOpacityEffect*)(m_numLabel->graphicsEffect()))->setOpacity(1.0);
    }
}
