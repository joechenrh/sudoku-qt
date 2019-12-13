#include "selectpanel.h"

#include <QDebug>

#include <QFontDatabase>

#include <QEventLoop>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

const int duration = 200;

#define BACKGROUND_COLOR 0
#define FONT_COLOR 0

SelectPanel::SelectPanel(int size, QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(size, size);
    QWidget::hide();

    // TODO: 大小的设置需要根据其他控件的大小来设置
    QRect minSize = QRect(size / 2, size / 2, 1, 1);
    QRect maxSize = QRect(-size / 4, -size / 4, size + size / 4 * 2, size + size / 4 * 2);

    int nIndex = QFontDatabase::addApplicationFont(":/fonts/ARLRDBD.TTF");
    QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));

    QFont hoverFont = QFont(strList.at(0), 14);
    hoverFont.setBold(true);

    QFont normalFont = QFont(strList.at(0), 14);

    QString enterStylesheet = "color:#FAFAFA;";
    QString leaveStylesheet = "color:#FBDFE8;";
    setStyleSheet("#panelButton:{background-color:transparent}");

    m_background = new PanelBase(size, this);
    m_background->setColor("#FB78A5");
    m_background->setGeometry(minSize);

    m_container = new QWidget(this);
    m_container->setFixedSize(size, size);
    m_container->hide();

    m_buttons.resize(9);
    m_selected.resize(9);
    int buttonSize = size / 3;
    for( int r = 0; r < 3; r++)
    {
        for(int c = 0; c < 3; c++)
        {
            HoverButton *button = new HoverButton(m_container);
            button->setFont(normalFont);
            button->setObjectName("panelButton");
            button->setText(QString::number(r * 3 + c + 1));
            button->setStyleSheet(leaveStylesheet);
            button->move(buttonSize * c, buttonSize * r);
            button->setFixedSize(buttonSize, buttonSize);
            m_buttons[r * 3 + c] = button;

            connect(button, &HoverButton::hovered,      [=]()
            {
                QFont font = QFont(strList.at(0), 16);
                font.setBold(m_selected[r * 3 + c]);
                button->setFont(font);
                button->setStyleSheet(enterStylesheet);
            });
            connect(button, &HoverButton::leaved,       [=]()
            {
                QFont font = QFont(strList.at(0), 14);
                font.setBold(m_selected[r * 3 + c]);
                button->setFont(font);
                button->setStyleSheet(leaveStylesheet);
            });
            connect(button, &HoverButton::clicked,      [=](){ emit finish(r * 3 + c + 1); });
            connect(button, &HoverButton::rightClicked, [=]()
            {
                QFont font = QFont(strList.at(0), 14);
                font.setBold(true);
                button->setFont(font);
                m_selected[r * 3 + c] = 1;
            });
        }
    }

    m_showAnimation = new QPropertyAnimation(m_background, "geometry");
    m_showAnimation->setEasingCurve(QEasingCurve::OutCubic);
    m_showAnimation->setDuration(duration);
    m_showAnimation->setStartValue(minSize);
    m_showAnimation->setEndValue(maxSize);

    m_hideAnimation = new QPropertyAnimation(m_background, "geometry");
    m_hideAnimation->setEasingCurve(QEasingCurve::OutQuad);
    m_hideAnimation->setDuration(125);
    m_hideAnimation->setStartValue(QRect(0, 0, size, size));
    m_hideAnimation->setEndValue(QRect(size / 2 - 4, size / 2 - 4, 9, 9));
}


bool SelectPanel::isVisible() const
{
    return QWidget::isVisible();
}

void SelectPanel::initialize()
{

}

bool SelectPanel::show(int x, int y)
{
    // 如果正在打开，则什么也不做
    if (m_showAnimation->state() != QAbstractAnimation::Stopped)
    {
        return false;
    }

    // 如果正在关闭，则等待关闭完成
    if (m_hideAnimation->state() != QAbstractAnimation::Stopped)
    {
        QEventLoop eventLoop;
        connect(m_hideAnimation, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();
    }

    this->move(x, y);
    this->initialize();

    QWidget::show();
    m_container->show();
    m_showAnimation->start();
    return true;
}

bool SelectPanel::hide()
{
    // 如果正在打开或关闭，则直接退出
    if (m_showAnimation->state() != QAbstractAnimation::Stopped ||
        m_hideAnimation->state() != QAbstractAnimation::Stopped)
    {
        return false;
    }

    m_container->hide(); 
    m_hideAnimation->start();

    QEventLoop eventLoop;
    connect(m_hideAnimation, SIGNAL(finished()), &eventLoop, SLOT(quit()));
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



PanelBase::PanelBase(int size, QWidget *parent)
    : QLabel(parent), m_size(size) { }

void PanelBase::setColor(QString color)
{
    m_color = QColor(color);
}

void PanelBase::paintEvent(QPaintEvent*)
{
    int alpha = std::min(255 * width() / m_size, 255);
    m_color.setAlpha(alpha);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(m_color));
    painter.drawEllipse(0, 0, width(), width());

}
