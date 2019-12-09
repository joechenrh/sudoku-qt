#include "selectpanel.h"

#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QApplication>
#include <QPropertyAnimation>
#include <QFontDatabase>
#include <QGraphicsOpacityEffect>

SelectPanel::SelectPanel(int size, QWidget *parent)
    : QWidget(parent)
{
    m_buttons.resize(9);
    m_selected.resize(9);
    this->setFixedSize(size, size);

    int nIndex = QFontDatabase::addApplicationFont(":/sudoku/fonts/ARLRDBD.TTF");
    QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));

    QFont hoverFont = QFont(strList.at(0), 16);
    hoverFont.setBold(true);

    QFont normalFont = QFont(strList.at(0), 12);

    QString enterStylesheet = "QWidget{color:#FAFAFA; background-color:#FB78A5; border:0px solid black;}";
    QString leaveStylesheet = "QWidget{color:#FBDFE8; background-color:#FB78A5; border:0px solid black;}";

    QWidget *container = new QWidget(this);
    container->setFixedSize(size, size);

    int buttonSize = size / 3;
    for( int r = 0; r < 3; r++)
    {
        for(int c = 0; c < 3; c++)
        {
            HoverButton *button = new HoverButton(container);
            button->setFont(normalFont);
            button->setText(QString::number(r * 3 + c + 1));
            button->setStyleSheet(leaveStylesheet);
            button->move(buttonSize * c, buttonSize * r);
            button->setFixedSize(buttonSize, buttonSize);
            m_buttons[r * 3 + c] = button;

            connect(button, &HoverButton::hovered,      [=](){ button->setFont(hoverFont); button->setStyleSheet(enterStylesheet); });
            connect(button, &HoverButton::leaved,       [=](){ button->setFont(normalFont); button->setStyleSheet(leaveStylesheet); });
            connect(button, &HoverButton::clicked,      [=](){ emit finish(r * 3 + c + 1); this->close(); });
            connect(button, &HoverButton::rightClicked, [=](){ m_selected[r * 3 + c] = 1; });
        }
    }

    m_dummylabel = new QLabel(this);
    m_dummylabel->setStyleSheet("background-color:#FB78A5;");
    m_dummylabel->setGeometry(15, 15, 45, 45);
    //m_dummylabel->hide();
    container->hide();
    drawMask();

    // 进入和退出的动画效果
    m_animation = new QPropertyAnimation(m_dummylabel, "geometry");
    m_animation->setDuration(100);
    m_animation->setEasingCurve(QEasingCurve::OutQuad);
    m_animation->setStartValue(QRect(15, 15, 45, 45));
    m_animation->setEndValue(QRect(0, 0, 75, 75));
    connect(m_animation, &QPropertyAnimation::valueChanged, [&](){ drawMask(); });
    //connect(m_animation, &QPropertyAnimation::finished, [&](){if (!this->isVisible()) {QWidget::hide();}});
}

void SelectPanel::show()
{
    if (m_animation->state() != QAbstractAnimation::Stopped)
    {
        m_animation->stop();
    }

    //m_dummylabel->hide();
    m_dummylabel->setGeometry(QRect(15, 15, 45, 45));
    m_animation->setStartValue(QRect(15, 15, 45, 45));
    m_animation->setEndValue(QRect(0, 0, 75, 75));
    m_animation->start();
    //drawMask();

    QWidget::show();
}

void SelectPanel::hide()
{
    if (m_animation->state() != QAbstractAnimation::Stopped)
    {
        m_animation->stop();
    }

    m_dummylabel->show();
    m_animation->setStartValue(m_animation->currentValue());
    m_animation->setEndValue(QRect(15, 15, 45, 45));
    m_animation->start();
    QWidget::hide();
}

void SelectPanel::setSelected(QList<int> list)
{
    for (auto &v : m_selected)
    {
        v = 0;
    }
    for (auto v : list)
    {
        m_selected[v - 1] = 1;
        m_buttons[v - 1];
    }
}

void SelectPanel::drawMask()
{
    return;
    int size = this->geometry().width();

    QImage img = QImage(QSize(size, size), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOut);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(0, 0, 0, 0)));
    painter.drawRect(QRect(0, 0, size, size));

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(0, 0, 0, 255)));
    painter.drawRect(m_dummylabel->geometry());

    this->setMask(QPixmap::fromImage(img).mask());
}
