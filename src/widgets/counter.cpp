#include "counter.h"

#include <QFontDatabase>

Counter::Counter(int num, int size, QWidget *parent)
    : QWidget(parent), m_count(9), m_style(CounterStyle())
{
    this->setFixedSize(size, size);

    QString baseStyle = "border-radius: %5px;color: %6; background-color: %7;";

    int lr = int(size * 0.45);  // radius of cntLabel
    int sr = int(size * 0.2);   // radius of numLabel
    int ld = lr * 2;
    int sd = sr * 2;

    int nIndex = QFontDatabase::addApplicationFont(":/fonts/ARLRDBD.TTF");
    QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));

    QFont cntFont = QFont(strList.at(0));
    cntFont.setPointSize(20);

    QFont numFont = QFont(strList.at(0));
    numFont.setPointSize(10);
    numFont.setBold(true);

    m_numStyle = baseStyle.arg(sr);
    m_cntStyle = baseStyle.arg(lr);

    m_cntLabel = new QLabel(this);
    m_cntLabel->setText(QString::number(m_count));
    m_cntLabel->setFont(cntFont);
    m_cntLabel->setAlignment(Qt::AlignCenter);
    m_cntLabel->setFixedSize(ld, ld);
    m_cntLabel->move(0, size - ld);

    m_numLabel = new QLabel(this);
    m_numLabel->setText(QString::number(num));
    m_numLabel->setFont(numFont);

    m_numLabel->setAlignment(Qt::AlignCenter);
    m_numLabel->setFixedSize(sd, sd);
    m_numLabel->move(size - sd, 0);

    m_cntOpacity = new QGraphicsOpacityEffect;
    m_cntOpacity->setOpacity(0.99);
    m_cntLabel->setGraphicsEffect(m_cntOpacity);

    // 下面这段全部完成以后就能删除了
    /*
    m_style.num_color = "#E66F99" ;
    m_style.cnt_color_unhovered = "#FAFAFA";
    m_style.cnt_color_hovered = "FADFE8";
    m_style.cnt_font_color = "#5F5F5F";
    m_style.num_font_color = "#FAFAFA";
    m_cntLabel->setStyleSheet(m_cntStyle.arg(m_style.cnt_font_color).arg(m_style.cnt_color_unhovered));
    m_numLabel->setStyleSheet(m_numStyle.arg(m_style.num_font_color).arg(m_style.num_color));
    */
}


void Counter::enterEvent(QEvent*)
{
    emit hovered();
    m_cntLabel->setStyleSheet(m_cntStyle
                              .arg(m_style.cnt_font_color)
                              .arg(m_style.cnt_color_hovered));


}

void Counter::leaveEvent(QEvent*)
{
    emit leave();
    m_cntLabel->setStyleSheet(m_cntStyle
                              .arg(m_style.cnt_font_color)
                              .arg(m_style.cnt_color_unhovered));
}


void Counter::setColorStyle(QJsonObject json)
{
    m_style.num_color = json.value("num_color").toString();
    m_style.cnt_color_unhovered = json.value("cnt_color_unhovered").toString();
    m_style.cnt_color_hovered = json.value("cnt_color_hovered").toString();
    m_style.num_font_color = json.value("num_font_color").toString();
    m_style.cnt_font_color = json.value("cnt_font_color").toString();

    m_cntLabel->setStyleSheet(m_cntStyle
                              .arg(m_style.cnt_font_color)
                              .arg(m_style.cnt_color_unhovered));
    m_numLabel->setStyleSheet(m_numStyle
                              .arg(m_style.num_font_color)
                              .arg(m_style.num_color));
}

void Counter::modify(int value)
{
    m_count += value;
    m_numLabel->setVisible(m_count < 1 ? false : true);
    m_cntLabel->setText(m_count < 1 ? "0" : QString::number(m_count));
    m_cntOpacity->setOpacity(m_count < 1 ? 0.5 : 1.0);
}

void Counter::setCount(int value)
{
    m_count = value;
    m_numLabel->setVisible(m_count < 1 ? false : true);
    m_cntLabel->setText(QString::number(m_count));
    m_cntOpacity->setOpacity(m_count < 1 ? 0.5 : 1.0);
}

