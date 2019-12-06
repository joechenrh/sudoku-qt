#include "counter.h"

#define NUM_COLOR "#E66F99"                 // color of numLabel
#define CNT_COLOR_UNHOVERED "#F1F1F1"       // color of cntLabel when not hovered
#define CNT_COLOR_HOVERED "#FADFE8"         // color of cntLabel when hovered
#define CNT_FONT_COLOR "#5F5F5F"            // font color of cntLabel
#define NUM_FONT_COLOR "#5F5F5F"            // font color of numLabel

Counter::Counter(int num, int size, QWidget *parent)
    : QWidget(parent), m_count(9)
{
    this->setFixedSize(size, size);

    QString baseStyle = "min-width: %1px; min-height: %1px;"
                        "max-width: %1px; max-height: %1px;"
                        "border-radius: %5px;"
                        "color: %6; background: %7;";

    int lr = int(size * 0.5);  // radius of cntLabel
    int sr = int(size * 0.2);  // radius of numLabel

    QFont cntFont("华文新魏", 16);
    cntFont.setBold(true);

    QFont numFont("华文新魏", 10);
    numFont.setBold(true);

    QString numStyle = baseStyle.arg(2 * sr).arg(sr).arg(NUM_FONT_COLOR);
    m_cntStyle = baseStyle.arg(2 * lr).arg(lr).arg(CNT_FONT_COLOR);

    m_cntLabel = new QLabel(this);
    m_cntLabel->setText("9");
    m_cntLabel->setFont(cntFont);
    m_cntLabel->setStyleSheet(m_cntStyle.arg(CNT_COLOR_UNHOVERED));
    m_cntLabel->setAlignment(Qt::AlignCenter);
    m_cntLabel->setFixedSize(lr, lr);
    m_cntLabel->move(0, size - 2 * lr);

    m_numLabel = new QLabel(this);
    m_numLabel->setText(QString::number(num));
    m_numLabel->setFont(numFont);
    m_numLabel->setStyleSheet(numStyle.arg(NUM_COLOR));
    m_numLabel->setAlignment(Qt::AlignCenter);
    m_numLabel->setFixedSize(sr, sr);
    m_numLabel->move(size - 2 * sr, 0);

    m_cntOpacity = new QGraphicsOpacityEffect;
    m_cntOpacity->setOpacity(1.0);
    m_cntLabel->setGraphicsEffect(m_cntOpacity);
}


void Counter::enterEvent(QEvent *e)
{
    m_cntLabel->setStyleSheet(m_cntStyle.arg(CNT_COLOR_HOVERED));

    emit hovered();
}

void Counter::leaveEvent(QEvent *e)
{
    m_cntLabel->setStyleSheet(m_cntStyle.arg(CNT_COLOR_UNHOVERED));

    emit leave();
}


void Counter::minus(int value)
{
    m_count -= value;
    m_cntLabel->setText(m_count < 1 ? "0" : QString::number(m_count));
    this->updateOpacity();
}

void Counter::plus(int value)
{
    m_count += value;
    m_cntLabel->setText(m_count < 1 ? "0" : QString::number(m_count));
    this->updateOpacity();
}

void Counter::setCount(int value)
{
    m_count = value;
    m_cntLabel->setText(QString::number(m_count));
    this->updateOpacity();
}

void Counter::updateOpacity()
{
    m_cntOpacity->setOpacity(m_count < 1 ? 0.5 : 1.0);
    m_numLabel->setVisible(m_count < 1 ? 0.0 : 1.0);
}
