#include "counter.h"

#define NUM_COLOR "#E66F99"                 // color of numLabel
#define CNT_COLOR_UNHOVERED "#F1F1F1"       // color of cntLabel when not hovered
#define CNT_COLOR_HOVERED "#FADFE8"         // color of cntLabel when hovered
#define CNT_COLOR_EMPTY "#E5E0E5"           // color of cntLabel when count is zero

Counter::Counter(int num, int size, QWidget *parent)
    : QWidget(parent), m_count(9)
{
    QString baseStyle = "min-width: %1px;"
                        "min-height: %1px;"
                        "max-width: %1px;"
                        "max-height: %1px;"
                        "border-radius: %5px;"
                        "background: %6;"
                        "color: #5F5F5F";

    int lr = size * 0.5;  // radius of cntLabel
    int sr = size * 0.2;  // radius of numLabel

    QFont cntFont("华文新魏", 15);
    cntFont.setBold(true);

    QFont numFont("华文新魏", 10);
    numFont.setBold(true);

    QString numStyle = baseStyle.arg(2 * sr).arg(sr).arg(NUM_COLOR);
    m_cntStyle = baseStyle.arg(2 * lr).arg(lr);

    this->setFixedSize(size, size);

    m_cntLabel = new QLabel(this);
    m_cntLabel->setText("9");
    m_cntLabel->setFont(cntFont);
    m_cntLabel->setStyleSheet(m_cntStyle.arg(CNT_COLOR_UNHOVERED));
    m_cntLabel->setAlignment(Qt::AlignCenter);
    m_cntLabel->setFixedSize(lr, lr);
    m_cntLabel->move(0, size - 2 * lr);

    QLabel *numLabel = new QLabel(this);
    numLabel->setText(QString::number(num));
    numLabel->setFont(numFont);
    numLabel->setStyleSheet(numStyle);
    numLabel->setAlignment(Qt::AlignCenter);
    numLabel->setFixedSize(sr, sr);
    numLabel->move(size - 2 * sr, 0);

    m_opacityEffect = new QGraphicsOpacityEffect;
    m_opacityEffect->setOpacity(1.0);
    numLabel->setGraphicsEffect(m_opacityEffect);
}

void Counter::enterEvent(QEvent *e)
{
    // count为0时，鼠标移动时不会修改颜色
    if (m_count > 0)
    {
        m_cntLabel->setStyleSheet(m_cntStyle.arg(CNT_COLOR_HOVERED));
    }
    emit hovered();

    QWidget::enterEvent(e);
}

void Counter::leaveEvent(QEvent *e)
{
    if (m_count > 0)
    {
        m_cntLabel->setStyleSheet(m_cntStyle.arg(CNT_COLOR_UNHOVERED));
    }
    emit leave();

    QWidget::enterEvent(e);
}

void Counter::minus(int value)
{
    m_count -= value;
    if (m_count < 1)
    {
        m_cntLabel->setText("0");
        m_cntLabel->setStyleSheet(m_cntStyle.arg(CNT_COLOR_EMPTY));
    }
    else
    {
        m_cntLabel->setText(QString::number(m_count));
    }

    this->updateOpacity();
}

void Counter::plus(int value)
{
    m_count += value;
    if (m_count < 1)
    {
        m_cntLabel->setText("0");
    }
    else
    {
        m_cntLabel->setText(QString::number(m_count));
        m_cntLabel->setStyleSheet(m_cntStyle.arg(CNT_COLOR_UNHOVERED));
    }

    this->updateOpacity();
}

void Counter::setCount(int value)
{
    m_count = value;
    m_cntLabel->setText(QString::number(m_count));
    if (m_count == 0)
    {
        m_cntLabel->setStyleSheet(m_cntStyle.arg(CNT_COLOR_EMPTY));
    }
    else
    {
        m_cntLabel->setStyleSheet(m_cntStyle.arg(CNT_COLOR_UNHOVERED));
    }

    this->updateOpacity();
}

void Counter::updateOpacity()
{
    m_opacityEffect->setOpacity(m_count < 1 ? 0.0 : 1.0);
}
