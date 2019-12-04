/**
 * @file counter.h
 * @brief Contains widget used to show remain count of certain digit.
 * @author Joe chen <joechenrh@gmail.com>
 */

#ifndef CIRCLEWIDGET_H
#define CIRCLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QGraphicsOpacityEffect>

/**
 * @brief Class used to control the number of digits.
 * @details This widget consists of two components, cntLabel which indicate which digit is chosen
 * and numLabel which show remaining count of selected digit
 */
class Counter : public QWidget
{
    Q_OBJECT

public:
    Counter(int num, int size, QWidget *parent = NULL);

    void setCount(int value);                 // 设置剩余数量

    void minus(int value = 1);                // 减少剩余数量

    void plus(int value = 1);                 // 添加剩余数量

protected:
    void enterEvent(QEvent *e);               // 鼠标移入时的事件

    void leaveEvent(QEvent *e);               // 鼠标移出时的事件

    void updateOpacity();                     // 更新透明度

signals:
    /**
     * @brief Signal emitted whenever mouse enters this widget.
     */
    void hovered();

    /**
     * @brief Signal emitted whenever mouse leaves this widget.
     */
    void leave();

private:
    /**
     * @brief Remaining number of current digit.
     */
    int m_count;

    /**
     * @brief Widget which visualize m_count.
     */
    QLabel *m_cntLabel;

    /**
     * @brief Control the stylesheet of cntLabel.
     * @details Use arg to change background color and font color of cntLabel.
     */
    QString m_cntStyle;

    /**
     * @brief Effect which controls the opacity of numLabel.
     * @details if count is zero, set opacity of numLabel to zero too.
     */
    QGraphicsOpacityEffect *m_opacityEffect;
};

#endif // CIRCLEWIDGET_H
