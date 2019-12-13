/**
 * @file gridmarker.h
 * @brief Contains widget used to highlight mouse enter event.
 * @author Joe chen <joechenrh@gmail.com>
 */

#ifndef GRIDMARKER_H
#define GRIDMARKER_H

#include <QLabel>
#include <QPainter>
#include <QPropertyAnimation>

/**
 * @brief The GridMarker class 鼠标进入九宫格时显示的圆形效果由这个控件绘制
 * @details 通过QPropertyAnimation变化geometry来达到放大缩小的效果
 */
class GridMarker : public QLabel
{
    Q_OBJECT

public:
    /**
     * @brief GridMarker 构造函数
     * @param size 控件的大小
     * @param parent 父控件
     */
    explicit GridMarker(int size, QWidget *parent = nullptr);

    /**
     * @brief 显示标记
     */
    void hide();

    /**
     * @brief 隐藏标记
     */
    void show();

    /**
     * @brief 设置动画持续时间
     */
    void setDuration(const int &duration);

    /**
     * @brief 设置标记颜色
     */
    void setMarkerColor(const QColor &color);

    /**
     * @brief 设置阴影颜色
     */
    void setShadowColor(const QColor &color);

private:
    /**
     * @brief 标记的颜色
     */
    QColor m_markerColor;

    /**
     * @brief 阴影的颜色
     */
    QColor m_shadowColor;

    /**
     * @brief 缩放的最大尺寸
     */
    QRect m_maxSize;

    /**
     * @brief 缩放的最小尺寸
     */
    QRect m_minSize;

    /**
     * @brief 控制绘制标记时的缩进
     * @details 由于需要绘制一层阴影，因此实际标记的半径需要比控件大小的一半小一点
     */
    int m_indent;

    /**
     * @brief 控制放大缩小的动画
     */
    QPropertyAnimation *m_scaleAnimation;

protected:
    /**
     * @brief 重载paintEvent函数
     * @details 实现在控件上绘制一个圆形和一层阴影
     */
    void paintEvent(QPaintEvent*);
};

#endif // GRIDMARKER_H
