#include "gridwidget.h"

#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>

const int duration = 200;

GridWidget::GridWidget(int row, int col, int size, QWidget *parent)
    : QWidget(parent), m_value(0), m_numConflict(0)
{   
    // 设置单元格大小和阴影
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(2, 2);
    shadow->setBlurRadius(2);
    this->setGraphicsEffect(shadow);
    this->setFixedSize(size, size);

    // 单元格的样式，四角的单元格要加圆角
    m_backgroundStyle = QString("background-color:%1;border:1px solid %2;");
    if ((row == 0 || row == 8) && (col == 0 || col == 8))
    {
        m_backgroundStyle += QString("border-%1-%2-radius:%3px;")
                                .arg(row ? "bottom" : "top")
                                .arg(col ? "right" : "left" )
                                .arg(size / 5);
    }

    // 底层的背景
    m_background = new QLabel(this);
    m_background->setFixedSize(size, size);

    m_foreground = new BaseWidget(this);
    m_foreground->setFixedSize(size, size);

    m_marker = new GridMarker(size, this);
    m_marker->setDuration(duration);
    m_marker->setGeometry(QRect(size / 2, size / 2, 1, 1));

    int buttonMargin = size / 5 - 5;
    int buttonSize = size - 2 * buttonMargin;
    // m_buttonStyle = QString("border:%1px solid %2;color:%3;");

    int nIndex = QFontDatabase::addApplicationFont(":/fonts/ARLRDBD.TTF");
    QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
    QFont buttonFont = QFont(strList.at(0), size / 4);

    m_button = new BaseWidget(this);
    m_button->setObjectName("buttonText");
    m_button->setFont(buttonFont);
    m_button->setFixedSize(buttonSize, buttonSize);
    m_button->move(buttonMargin, buttonMargin);

    setStyleSheet(QString("#buttonText{border-radius:%1px;background-color:transparent;}").arg(buttonSize / 2));

    connect(m_button, SIGNAL(clicked()),      this, SLOT(buttonClicked()));
    connect(m_button, SIGNAL(rightClicked()), this, SLOT(buttonRightClicked()));

    // 下面这段全部完成以后就能删除了
    /*
    m_style.border_color[0] = "#5F5F5F";
    m_style.border_color[1] = "#FB78A5";
    m_style.border_radius[0] = 3;
    m_style.border_radius[1] = 0;
    m_style.font_color[0] = "#5F5F5F";
    m_style.font_color[1] = "#FB78A5";
    m_style.font_color[2] = "#FFFFFF";

    m_style.background_color_hovered = "#FADFE8";
    m_style.background_color_unhovered = "#FAFAFA";
    m_style.background_shadow_color = "#C9C9C9";

    // 初始风格
    // m_borderRadius = m_style.border_radius[1];
    // m_borderColor  = m_style.border_color[1];
    // m_fontColor    = m_style.font_color[1];

    m_foreground->setStyleSheet(m_backgroundStyle.arg(m_style.background_color_unhovered));
    m_background->setStyleSheet(m_backgroundStyle.arg(m_style.background_color_hovered));
    shadow->setColor(m_style.background_shadow_color);
    m_marker->setMarkerColor("#FB78A5");
    m_marker->setShadowColor("#E6CED6");
    m_button->setStyleSheet(QString("border:%1px solid %2;color:%3;")
                            .arg(m_style.border_radius[1])
                            .arg(m_style.border_color[1])
                            .arg(m_style.font_color[1]));
                            */
}

void GridWidget::setColorStyle(QJsonObject json)
{
    m_style.border_color[0] = json.value("border_color_unabled").toString();
    m_style.border_color[1] = json.value("border_color_enabled").toString();

    m_style.border_radius[0] = json.value("border_radius_conflict").toInt();
    m_style.border_radius[1] = json.value("border_radius_noconflict").toInt();

    m_style.font_color[0] = json.value("font_color_unabled").toString();
    m_style.font_color[1] = json.value("font_color_normal").toString();
    m_style.font_color[2] = json.value("font_color_hovered").toString();

    m_style.background_color_hovered = json.value("background_color_hovered").toString();
    m_style.background_color_unhovered = json.value("background_color_unhovered").toString();
    m_style.background_shadow_color = json.value("background_shadow_color").toString();

    m_style.spacing_color = json.value("spacing_color").toString();

    //m_borderRadius = m_style.border_radius[m_numConflict == 0];
    //m_borderColor  = m_style.border_color[m_button->isEnabled()];
    //m_fontColor    = m_style.font_color[m_button->isEnabled()];

    ((QGraphicsDropShadowEffect*)graphicsEffect())->setColor(m_style.background_shadow_color);
    m_marker->setMarkerColor(json.value("marker_color").toString());
    m_marker->setShadowColor(json.value("marker_color_shadow").toString());
    m_foreground->setStyleSheet(m_backgroundStyle.arg(m_style.background_color_unhovered).arg(m_style.spacing_color));
    m_background->setStyleSheet(m_backgroundStyle.arg(m_style.background_color_hovered).arg(m_style.spacing_color));
    setButtonStyle(false);
    //m_button->setStyleSheet(m_buttonStyle
    //                        .arg(m_borderRadius)
    //                        .arg(m_borderColor)
    //                        .arg(m_fontColor));
}

void GridWidget::setEnabled(bool enabled)
{
    m_button->setEnabled(enabled);
    //m_fontColor = m_style.font_color[enabled];
    //m_borderColor = m_style.border_color[enabled];
    //m_button->setStyleSheet(m_buttonStyle
    //                        .arg(m_borderRadius)
    //                        .arg(m_borderColor)
    //                        .arg(m_fontColor));
    setButtonStyle(false);
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
    /*
    if (m_numConflict == 0 && num > 0)
    {
        m_borderRadius = m_style.border_radius[1];
        m_button->setStyleSheet(m_buttonStyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
    }
    else if (m_numConflict > 0 && m_numConflict + num == 0)
    {
        m_borderRadius = m_style.border_radius[0];
        m_button->setStyleSheet(m_buttonStyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
    }
    */

    m_numConflict += num;
    setButtonStyle(false);
}

void GridWidget::clearConflict()
{
    m_numConflict = 0;
    setButtonStyle(false);
    //m_borderRadius = m_style.border_radius[1];
    //m_button->setStyleSheet(m_buttonStyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
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


void GridWidget::showBackground()
{
    m_foreground->hide();
}

void GridWidget::hideBackground()
{
    m_foreground->reveal();
}

void GridWidget::enter()
{
    m_foreground->hide();
    m_marker->show();
    setButtonStyle(true);
    // m_fontColor = m_style.font_color[2];
    // m_button->setStyleSheet(m_buttonStyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
}

void GridWidget::leave()
{
    m_foreground->reveal();
    m_marker->hide();
    setButtonStyle(false);
    // m_fontColor = m_style.font_color[1];
    // m_button->setStyleSheet(m_buttonStyle.arg(m_borderRadius).arg(m_borderColor).arg(m_fontColor));
}

void GridWidget::setButtonStyle(int entered)
{
    // 只有isEnabled()为true时entered才会为1
    m_button->setStyleSheet(QString("border:%1px solid %2;color:%3;")
                            .arg(m_style.border_radius[m_numConflict == 0])
                            .arg(m_style.border_color[m_button->isEnabled()])
                            .arg(m_style.font_color[m_button->isEnabled() + entered]));
}
