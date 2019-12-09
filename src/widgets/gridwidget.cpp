#include "gridwidget.h"

#include <QDebug>
#include <QFontDatabase>
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
    : QWidget(parent), m_value(0), m_numConflict(1)
{
    // 设置单元格大小和阴影
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(2, 2);
    shadow->setBlurRadius(2);
    shadow->setColor(BACKGROUND_SHADOW_COLOR);
    this->setGraphicsEffect(shadow);
    this->setFixedSize(size, size);

    // 单元格的样式，四角的单元格要加圆角
    QString gridStyle = QString("background-color:%1;border:1px solid #C9C9C9;");
    if ((row == 0 || row == 8) && (col == 0 || col == 8))
    {
        QString borderStyle = "border-%1-%2-radius:15px;";
        gridStyle += borderStyle.arg(row == 0 ? "top" : "bottom")
                                .arg(col == 0 ? "left" : "right");
    }

    int nIndex = QFontDatabase::addApplicationFont(":/sudoku/fonts/ARLRDBD.TTF");
    QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));

    // 底层的背景
    QLabel *background = new QLabel(this);
    background->setStyleSheet(gridStyle.arg(BACKGROUND_COLOR_HOVERED));
    background->setFixedSize(size, size);

    m_foreground = new HoverButton(this);
    m_foreground->setStyleSheet(gridStyle.arg(BACKGROUND_COLOR_UNHOVERED));
    m_foreground->setFixedSize(size, size);

    m_marker = new GridMarker(size, this);
    m_marker->setGeometry(QRect(size / 2, size / 2, 0, 0));

    int bias = static_cast<int>(size * 0.125);
    m_buttonStyle = QString("border-radius:%1px;border:%2px solid %3;"
                            "background-color:transparent;color:%4").arg(size / 2 - bias);

    // 初始风格
    m_borderRadius = BORDER_RADIUS_UNABLED;
    m_borderColor  = BORDER_COLOR_ENABLED;
    m_fontColor    = FONT_COLOR_NORMAL;

    QFont buttonFont = QFont(strList.at(0), 25);
    buttonFont.setBold(true);

    m_button = new HoverButton(this);
    m_button->setFont(buttonFont);
    m_button->setFixedSize(size - 2 * bias, size - 2 * bias);
    m_button->move(bias, bias);
    m_button->setStyleSheet(m_buttonStyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));

    connect(m_button, SIGNAL(clicked()),      this, SLOT(buttonClicked()));
    connect(m_button, SIGNAL(rightClicked()), this, SLOT(buttonRightClicked()));
}

void GridWidget::setEnabled(bool flag)
{
    m_fontColor = flag ? FONT_COLOR_NORMAL : FONT_COLOR_UNABLED;
    m_borderColor = flag ? BORDER_COLOR_ENABLED : BORDER_COLOR_UNABLED;

    m_button->setStyleSheet(m_buttonStyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
    m_button->setEnabled(flag);
}

bool GridWidget::isEnabled() const
{
    return m_button->isEnabled();
}

void GridWidget::setValue(int value)
{
    m_value = value;
    m_button->setText(value == 0 ? "" : QString::number(value));
}

int GridWidget::value() const
{
    return m_value;
}

void GridWidget::changeConflict(int num)
{
    if (m_numConflict == 1 && num > 0)
    {
        m_borderRadius = BORDER_RADIUS_ENABLED;
        m_button->setStyleSheet(m_buttonStyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
    }
    else if (m_numConflict > 1 && m_numConflict + num == 1)
    {
        m_borderRadius = BORDER_RADIUS_UNABLED;
        m_button->setStyleSheet(m_buttonStyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
    }

    m_numConflict += num;
}

void GridWidget::clearConflict()
{
    m_numConflict = 1;
    m_borderRadius = BORDER_RADIUS_UNABLED;
    m_button->setStyleSheet(m_buttonStyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
}


void GridWidget::buttonClicked()
{
    emit clicked();
}

void GridWidget::buttonRightClicked()
{
    emit rightClicked();
}

void GridWidget::enterEvent(QEvent*)
{
    emit hovered();
}

void GridWidget::leaveEvent(QEvent*)
{
    emit leaved();
}


void GridWidget::hideButton()
{
    m_foreground->hide();
}

void GridWidget::revealButton()
{
    m_foreground->reveal();
}

void GridWidget::enter()
{
    m_foreground->hide();
    m_marker->reveal();
    m_fontColor = FONT_COLOR_HOVERED;
    m_button->setStyleSheet(m_buttonStyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
}

void GridWidget::leave()
{
    m_foreground->reveal();
    m_marker->hide();
    m_fontColor = FONT_COLOR_NORMAL;
    m_button->setStyleSheet(m_buttonStyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
}
