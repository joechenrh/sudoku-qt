
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
    : QWidget(parent), m_selected(QVector<int>(10, 0))
{
    this->setFixedSize(size, size);
    QWidget::hide();

    int nIndex = QFontDatabase::addApplicationFont(":/fonts/ARLRDBD.TTF");
    QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));

    QFont hoverFont = QFont(strList.at(0), 18);
    hoverFont.setBold(true);

    QFont normalFont = QFont(strList.at(0), 14);
    normalFont.setPixelSize(14);
    normalFont.setPointSize(14);

    m_background = new PanelBase(size, this);

    m_container = new QWidget(this);
    m_container->setFixedSize(size, size);
    m_container->hide();

    int buttonSize = size / 3;
    for( int r = 0; r < 3; r++)
    {
        for(int c = 0; c < 3; c++)
        {
            int num = r * 3 + c + 1;

            BaseWidget *button = new BaseWidget(m_container);
            button->setFont(normalFont);
            button->setObjectName("panelButton");
            button->setText(QString::number(num));
            button->move(buttonSize * c, buttonSize * r);
            button->setFixedSize(buttonSize, buttonSize);
            button->setOpacity(0.7);
            m_buttons.push_back(button);

            connect(button, &BaseWidget::hovered, [=]()
            {
                if (m_showAnimation->state() == QAbstractAnimation::Stopped)
                    button->zoomIn();
            });
            connect(button, &BaseWidget::leaved,  [=]() { button->zoomOut();});
            connect(button, &BaseWidget::clicked, [=](){ emit finish(r * 3 + c + 1); });
            connect(button, &BaseWidget::rightClicked, [=]()
            {
                // Todo: 调整为用一个int型储存
                m_selected[num] = 1 - m_selected[num];
                button->setOpacity(m_selected[num] ? 1.0 : 0.7);
            });
        }
    }

    m_showAnimation = new QPropertyAnimation(m_background, "geometry");
    m_showAnimation->setEasingCurve(QEasingCurve::OutCubic);
    m_showAnimation->setDuration(duration);
    m_showAnimation->setStartValue(QRect(size / 2, size / 2, 1, 1));
    m_showAnimation->setEndValue(QRect(-size / 4, -size / 4, size + size / 4 * 2, size + size / 4 * 2));

    m_hideAnimation = new QPropertyAnimation(m_background, "geometry");
    m_hideAnimation->setEasingCurve(QEasingCurve::OutQuad);
    m_hideAnimation->setDuration(125);
    m_hideAnimation->setStartValue(QRect(0, 0, size, size));
    m_hideAnimation->setEndValue(QRect(size / 2 - 4, size / 2 - 4, 9, 9));

    setStyleSheet(QString("BaseWidget#panelButton{color:#FBFBFB;};"));
}

void SelectPanel::setColorStyle(QJsonObject json)
{
    m_background->setColor(json.value("background_color").toString());
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
