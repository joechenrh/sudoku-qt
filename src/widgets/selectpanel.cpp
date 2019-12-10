#include "selectpanel.h"

#include <QDebug>
#include <QPropertyAnimation>
#include <QFontDatabase>
#include <QEventLoop>
#include <QGraphicsOpacityEffect>

SelectPanel::SelectPanel(int size, QWidget *parent)
    : QWidget(parent)
{
    m_minSize = QRect(size / 3, size / 3, size - size / 3 * 2, size - size / 3 * 2);
    m_maxSize = QRect(0, 0, size, size);

    m_buttons.resize(9);
    m_selected.resize(9);
    this->setFixedSize(size, size);
    QWidget::hide();

    int nIndex = QFontDatabase::addApplicationFont(":/sudoku/fonts/ARLRDBD.TTF");
    QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));

    QFont hoverFont = QFont(strList.at(0), 16);
    hoverFont.setBold(true);

    QFont normalFont = QFont(strList.at(0), 14);

    QString enterStylesheet = "QWidget{color:#FAFAFA; background-color:transparent; border:0px solid black;}";
    QString leaveStylesheet = "QWidget{color:#FBDFE8; background-color:transparent; border:0px solid black;}";

    m_background = new QLabel(this);
    m_background->setStyleSheet("background-color:#FB78A5;");
    m_background->setGeometry(m_minSize);

    m_container = new QWidget(this);
    m_container->setFixedSize(size, size);
    m_container->hide();

    int buttonSize = size / 3;
    for( int r = 0; r < 3; r++)
    {
        for(int c = 0; c < 3; c++)
        {
            HoverButton *button = new HoverButton(m_container);
            button->setFont(normalFont);
            button->setText(QString::number(r * 3 + c + 1));
            button->setStyleSheet(leaveStylesheet);
            button->move(buttonSize * c, buttonSize * r);
            button->setFixedSize(buttonSize, buttonSize);
            m_buttons[r * 3 + c] = button;

            connect(button, &HoverButton::hovered,      [=](){ button->setFont(hoverFont); button->setStyleSheet(enterStylesheet); });
            connect(button, &HoverButton::leaved,       [=](){ button->setFont(normalFont); button->setStyleSheet(leaveStylesheet); });
            connect(button, &HoverButton::clicked,      [=](){ emit finish(r * 3 + c + 1); this->hide(); });
            connect(button, &HoverButton::rightClicked, [=](){ m_selected[r * 3 + c] = 1; });
        }
    }

    m_animation = new QPropertyAnimation(m_background, "geometry");
    m_animation->setDuration(150);
    m_animation->setStartValue(m_minSize);
    m_animation->setEndValue(m_maxSize);
}

bool SelectPanel::isVisible() const
{
    return m_container->isVisible();
}

void SelectPanel::show(int x, int y)
{
    if (m_animation->state() != QAbstractAnimation::Stopped)
    {
        QEventLoop eventLoop;
        connect(m_animation, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();
    }

    this->move(x, y);

    QWidget::show();
    m_container->show();

    m_animation->setEasingCurve(QEasingCurve::OutCubic);
    m_animation->setStartValue(m_minSize);
    m_animation->setEndValue(m_maxSize);
    m_animation->start();
}

bool SelectPanel::hide()
{
    if (m_animation->state() != QAbstractAnimation::Stopped)
    {
        return false;
    }

    m_container->hide(); 

    m_animation->setEasingCurve(QEasingCurve::InOutCubic);
    m_animation->setStartValue(m_maxSize);
    m_animation->setEndValue(m_minSize);
    m_animation->start();

    QEventLoop eventLoop;
    connect(m_animation, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    QWidget::hide();

    return true;
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
