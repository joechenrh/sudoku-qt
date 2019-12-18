#include "counter.h"

#include <QFontDatabase>

Counter::Counter(int num, int size, QWidget *parent)
    : QWidget(parent), m_count(9)
{
    this->setUpdatesEnabled(false);

    this->setFixedSize(size, size);

    int lr = int(size * 0.45);  // radius of cntLabel
    int sr = int(size * 0.2);   // radius of numLabel
    int ld = lr * 2;
    int sd = sr * 2;

    setStyleSheet(QString("QLabel#cntLabel{border-radius: %1px;}"
                          "QLabel#numLabel{border-radius: %2px; color: #FBFBFB;};")
                          .arg(lr).arg(sr));

    int nIndex = QFontDatabase::addApplicationFont(":/fonts/ARLRDBD.TTF");
    QStringList fontList(QFontDatabase::applicationFontFamilies(nIndex));

    m_cntLabel = new QLabel(this);
    m_cntLabel->setObjectName("cntLabel");
    m_cntLabel->setText(QString::number(m_count));
    m_cntLabel->setFont(QFont(fontList.at(0), 20));
    m_cntLabel->setAlignment(Qt::AlignCenter);
    m_cntLabel->setFixedSize(ld, ld);
    m_cntLabel->move(0, size - ld - 2);

    m_numLabel = new QLabel(this);
    m_numLabel->setObjectName("numLabel");
    m_numLabel->setText(QString::number(num));
    m_numLabel->setFont(QFont(fontList.at(0), 10, QFont::Bold));
    m_numLabel->setAlignment(Qt::AlignCenter);
    m_numLabel->setFixedSize(sd, sd);
    m_numLabel->move(size - sd, 0);

    m_cntOpacity = new QGraphicsOpacityEffect;
    m_cntOpacity->setOpacity(0.99);
    m_cntLabel->setGraphicsEffect(m_cntOpacity);

    this->setUpdatesEnabled(true);
}


void Counter::enterEvent(QEvent*)
{
    emit hovered();
    m_cntLabel->setStyleSheet(QString("color: %1; background-color: %2;")
                              .arg(m_style.cnt_font_color_hovered, m_style.cnt_color_hovered));
}

void Counter::leaveEvent(QEvent*)
{
    emit leaved();
    m_cntLabel->setStyleSheet(QString("color: %1; background-color: #FBFBFB;")
                              .arg(m_style.cnt_font_color_unhovered));
}


void Counter::setColorStyle(QJsonObject json)
{
    m_style.cnt_color_hovered        = json.value("cnt_color_hovered").toString();
    m_style.cnt_font_color_unhovered = json.value("cnt_font_color").toString();
    m_style.cnt_font_color_hovered   = json.value("cnt_font_color").toString();

    m_cntLabel->setStyleSheet(QString("color: %1; background-color: #FBFBFB;")
                              .arg(m_style.cnt_font_color_unhovered));

    m_numLabel->setStyleSheet(QString("background-color:%1;")
                              .arg(json.value("num_color").toString()));
}

void Counter::modify(int value)
{
    m_count += value;

    this->setUpdatesEnabled(false);
    m_numLabel->setVisible(m_count < 1 ? false : true);
    m_cntLabel->setText(m_count < 1 ? "0" : QString::number(m_count));
    m_cntOpacity->setOpacity(m_count < 1 ? 0.5 : 1.0);
    this->setUpdatesEnabled(true);
}

void Counter::setCount(int value)
{
    m_count = value;

    this->setUpdatesEnabled(false);
    m_numLabel->setVisible(m_count < 1 ? false : true);
    m_cntLabel->setText(QString::number(m_count));
    m_cntOpacity->setOpacity(m_count < 1 ? 0.5 : 1.0);
    this->setUpdatesEnabled(true);
}

