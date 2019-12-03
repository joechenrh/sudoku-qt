#include "gridwidget.h"

#include <QDebug>
#include <QFont>
#include <QStackedLayout>
#include <QGraphicsOpacityEffect>
#include <QGraphicsDropShadowEffect>

// TODO: 创建json文件，方便的配置颜色
#define BORDER_COLOR_ENABLED "#FB78A5"        // border color when enabled
#define BORDER_COLOR_UNABLED "#5F5F5F"        // border color when disabled

#define BORDER_RADIUS_ENABLED 3               // border radius when no conflict
#define BORDER_RADIUS_UNABLED 0               // border radius when have conflict

#define FONT_COLOR_UNABLED "#5F5F5F"          // font color when disabled
#define FONT_COLOR_HOVERED "#FFFFFF"          // font color when enabled and mouse hovered
#define FONT_COLOR_NORMAL "#FB78A5"           // font color when enabled and mose not hovered

#define BACKGROUND_COLOR_HOVERED "#FADFE8"    // background color when mouse hovered
#define BACKGROUND_COLOR_UNHOVERED "#FAFAFA"  // background color when mouse not hovered
#define BACKGROUND_SHADOW_COLOR "#C9C9C9"     // background shadow color


GridWidget::GridWidget(int row, int col, int size, QWidget *parent)
    : QWidget(parent), enabled(true), m_value(0), m_numConflict(1)
{
    this->setFixedSize(size, size);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(2, 2);
    shadow->setBlurRadius(2);
    shadow->setColor(BACKGROUND_SHADOW_COLOR);
    this->setGraphicsEffect(shadow);

    QString gridStyle = QString("background-color:%1;border:1px solid #C9C9C9;");

    if (row == 0 && col == 0)
    {
        gridStyle += "border-top-left-radius:15px;";
    }
    else if (row == 0 && col == 8)
    {
        gridStyle += "border-top-right-radius:15px;";
    }
    else if (row == 8 && col == 0)
    {
        gridStyle += "border-bottom-left-radius:15px;";
    }
    else if (row == 8 && col == 8)
    {
        gridStyle += "border-bottom-right-radius:15px;";
    }

    QFont buttonFont("华文新魏", 20);
    buttonFont.setPointSize(25);
    buttonFont.setBold(true);

    m_marker = new GridMarker(size);

    m_foreground = new HoverButton;
    m_foreground->setStyleSheet(gridStyle.arg(BACKGROUND_COLOR_UNHOVERED));
    m_foreground->setFixedSize(size, size);

    m_background = new QLabel;
    m_background->setStyleSheet(gridStyle.arg(BACKGROUND_COLOR_HOVERED));
    m_background->setFixedSize(size, size);

    // 一定要按照这个顺序写
    QStackedLayout *layout = new QStackedLayout;
    this->setLayout(layout);
    layout->addWidget(m_marker);
    layout->addWidget(m_foreground);
    layout->addWidget(m_background);
    layout->setStackingMode(QStackedLayout::StackAll);

    int bias = size * 0.125;
    m_fstyle = QString("border-radius:%1px;border:%2px solid %3;"
                       "background-color:transparent;color:%4").arg(size / 2 - bias );

    // 初始风格
    m_borderRadius = BORDER_RADIUS_UNABLED;
    m_borderColor = BORDER_COLOR_ENABLED;
    m_fontColor = FONT_COLOR_NORMAL;

    m_button = new HoverButton(this);
    m_button->setText("");
    m_button->setFont(buttonFont);
    m_button->setFixedSize(size - 2 * bias, size - 2 * bias);
    m_button->move(bias, bias);
    m_button->setStyleSheet(m_fstyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));

    connect(m_button, SIGNAL(clicked()), this, SLOT(childrenClicked()));
    connect(m_button, SIGNAL(rightClicked()), this, SLOT(childrenRightClicked()));
}

void GridWidget::setEnabled(bool flag)
{
    enabled = flag;

    m_fontColor = enabled ? FONT_COLOR_NORMAL : FONT_COLOR_UNABLED;
    m_borderColor = enabled ? BORDER_COLOR_ENABLED : BORDER_COLOR_UNABLED;

    m_button->setStyleSheet(m_fstyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
    m_button->setEnabled(flag);
}

bool GridWidget::getEnabled()
{
    return enabled;
}

void GridWidget::setValue(int value)
{
    m_value = value;
    m_button->setText(value == 0 ? "" : QString::number(value));
}

int GridWidget::getValue()
{
    return m_value;
}

void GridWidget::enterEvent( QEvent* e )
{
    if (!enabled)
    {
        return;
    }

    m_foreground->hide();
    m_marker->reveal();

    emit hovered();

    QWidget::enterEvent( e );

    m_fontColor = FONT_COLOR_HOVERED;
    m_button->setStyleSheet(m_fstyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));

}

void GridWidget::leaveEvent( QEvent* e )
{
    if (!enabled)
    {
        return;
    }

    m_foreground->vis();
    m_marker->hide();

    emit leave();

    QWidget::enterEvent( e );

    m_fontColor = FONT_COLOR_NORMAL;
    m_button->setStyleSheet(m_fstyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
}

void GridWidget::hideButton()
{
    m_foreground->hide();
}

void GridWidget::revealButton()
{
    m_foreground->vis();
}

void GridWidget::childrenClicked()
{
    emit clicked();
}

void GridWidget::childrenRightClicked()
{
    emit rightClicked();
}

void GridWidget::addConflict(int num)
{
    m_numConflict += num;
    m_borderRadius = BORDER_RADIUS_ENABLED;
    m_button->setStyleSheet(m_fstyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
}

void GridWidget::removeConflict(int num)
{
    m_numConflict -= num;
    if (m_numConflict == 1)
    {
        m_borderRadius = BORDER_RADIUS_UNABLED;
        m_button->setStyleSheet(m_fstyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
    }
}

void GridWidget::clearConflict()
{
    m_numConflict = 1;
    m_borderRadius = BORDER_RADIUS_UNABLED;
    m_button->setStyleSheet(m_fstyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
}
