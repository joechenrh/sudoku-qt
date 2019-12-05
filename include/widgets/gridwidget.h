#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QLabel>
#include <QWidget>
#include <QString>

#include "hoverbutton.h"
#include "gridmarker.h"
#include "selectpanel.h"

class GridWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GridWidget(int row, int col, int size = 75, QWidget *parent = nullptr);

    void setEnabled(bool flag);

    bool isEnabled() const;

    void setValue(int value);

    int value() const;

    void hideButton();

    void revealButton();

    void addConflict(int num);

    void removeConflict(int num);

    void clearConflict();

protected:

    void enterEvent(QEvent *e);

    void leaveEvent(QEvent *e);

signals:
    void hovered();

    void leave();

    void rightClicked();

private slots:
    void buttonClicked();

    void buttonRightClicked();

private:
    SelectPanel *m_panel;

    HoverButton *m_button; // 按钮

    GridMarker *m_marker;  // 鼠标覆盖时的标记

    HoverButton *m_foreground; // 顶层

    QLabel *m_background;  // 底层背景

    QString m_fstyle;  // 顶层按钮的stylesheet

    bool m_enabled;  // 当前单元格是否可编辑

    int m_value;  // 当前单元格的值

    int m_numConflict;  // 冲突的单元格数量

    // style
    int m_borderRadius;
    QString m_borderColor;
    QString m_fontColor;
};

#endif // GRIDWIDGET_H
