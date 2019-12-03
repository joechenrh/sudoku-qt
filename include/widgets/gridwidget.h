﻿#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QLabel>
#include <QWidget>
#include <QString>

#include <hoverbutton.h>
#include <gridmarker.h>

class GridWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GridWidget(int row, int col, int size = 75, QWidget *parent = nullptr);

    void setEnabled(bool flag);

    bool getEnabled();

    void setValue(int value);

    int getValue();

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

    void clicked();

    void rightClicked();

private slots:
    void childrenClicked();

    void childrenRightClicked();

private:
    HoverButton *m_button; // 按钮

    GridMarker *m_marker;  // 鼠标覆盖时的标记

    HoverButton *m_foreground; // 顶层

    QLabel *m_background;  // 底层背景

    QString m_fstyle;  // 顶层按钮的stylesheet

    bool enabled;  // 当前单元格是否可编辑

    int m_value;  // 当前单元格的值

    int m_numConflict;  // 冲突的单元格数量

    // style
    int m_borderRadius;
    QString m_borderColor;
    QString m_fontColor;
};

#endif // GRIDWIDGET_H