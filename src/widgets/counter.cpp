#include "counter.h"
#include <QFontDatabase>

#define NUM_COLOR "#E66F99"                 // color of numLabel
#define CNT_COLOR_UNHOVERED "#FAFAFA"       // color of cntLabel when not hovered
#define CNT_COLOR_HOVERED "#FADFE8"         // color of cntLabel when hovered
#define CNT_FONT_COLOR "#5F5F5F"            // font color of cntLabel
#define NUM_FONT_COLOR "#FAFAFA"            // font color of numLabel

Counter::Counter(int num, int size, QWidget *parent)
    : QWidget(parent), m_count(9)
{
    this->setFixedSize(size, size);

    QString baseStyle = "min-width: %1px; min-height: %1px;"
                        "max-width: %1px; max-height: %1px;"
                        "border-radius: %5px;"
                        "color: %6; background: %7;";

    int lr = int(size * 0.45);  // radius of cntLabel
    int sr = int(size * 0.2);  // radius of numLabel

    int nIndex = QFontDatabase::addApplicationFont(":/sudoku/fonts/ARLRDBD.TTF");
    QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));

    QFont cntFont = QFont(strList.at(0));
    cntFont.setPointSize(20);

    QFont numFont = QFont(strList.at(0));
    numFont.setPointSize(10);
    numFont.setBold(true);

    QString numStyle = baseStyle.arg(2 * sr).arg(sr).arg(NUM_FONT_COLOR);
    m_cntStyle = baseStyle.arg(2 * lr).arg(lr).arg(CNT_FONT_COLOR);

    m_cntLabel = new QLabel(this);
    m_cntLabel->setText(QString::number(m_count));
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
    m_cntOpacity->setOpacity(0.99);
    m_cntLabel->setGraphicsEffect(m_cntOpacity);
}


void Counter::enterEvent(QEvent*)
{
    m_cntLabel->setStyleSheet(m_cntStyle.arg(CNT_COLOR_HOVERED));

    emit hovered();
}

void Counter::leaveEvent(QEvent*)
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
    // 下面这句话没有效果
    // m_cntLabel->setWindowOpacity(m_count < 1 ? 0.5 : 0.999);
    m_cntOpacity->setOpacity(m_count < 1 ? 0.5 : 1.0);
    m_numLabel->setVisible(m_count < 1 ? false : true);
}
