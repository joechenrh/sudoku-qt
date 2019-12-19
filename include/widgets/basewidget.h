/**
 * @file hoverbutton.h
 * @brief Contains button widget
 * @author Joe chen <joechenrh@gmail.com>
 */

#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QLabel>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

/**
 * @brief The HoverButton class 按钮控件
 * @details 相对于基础的鼠标控件，增加了右键事件和淡入淡出效果
 * 主要是为了GridWidget服务
 */
class BaseWidget : public QLabel
{
    Q_OBJECT

    Q_PROPERTY(double fontPointSize READ fontSize WRITE setFontSize)

public:
    BaseWidget(QWidget *parent = nullptr);


    double fontSize() const;

    void setFont(const QFont &font);

    void setFontSize(double size);

    void zoomIn();

    void zoomOut();


    void setOpacity(double opacity);

    double opacity() const;

    void hide();

    void reveal();

protected:
    /**
     * @brief 鼠标移入触发的事件
     * @param e 鼠标事件
     */
    void enterEvent(QEvent *);

    /**
     * @brief 鼠标移出触发的事件
     * @param e 鼠标事件
     */
    void leaveEvent(QEvent *);

private slots:
    /**
     * @brief 鼠标点击事件
     * @param e 鼠标事件
     * @details 重写以支持右键点击事件
     */
    void mousePressEvent(QMouseEvent *e);

signals:
    /**
     * @brief 鼠标进入时触发的信号
     */
    void hovered();

    /**
     * @brief 鼠标移出时触发的信号
     */
    void leaved();

    /**
     * @brief 鼠标右击触发的信号
     */
    void rightClicked();

    /**
     * @brief 鼠标左击触发的事件
     */
    void clicked();

    /**
     * @brief 鼠标中击触发的事件
     */
    void midClicked();

private:
    /**
     * @brief 透明度
     */
    QGraphicsOpacityEffect *m_opacity;

    /**
     * @brief m_fontSize
     */
    double m_fontSize;

    /**
     * @brief 淡入淡出动画
     */
    QPropertyAnimation *m_animation;

    /**
     * @brief 缩放动画
     */
    QPropertyAnimation *m_scaleAnimation;
};

#endif // BASEWIDGET_H
