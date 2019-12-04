#ifndef HOVERBUTTON_H
#define HOVERBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QPropertyAnimation>

class HoverButton : public QPushButton
{
    Q_OBJECT

public:
    HoverButton(QWidget *parent = NULL);

    void setOpacity(double opacity);

    void hide();

    void reveal();

protected:
    void enterEvent(QEvent *e);

    void leaveEvent(QEvent *e);

private:
    QPropertyAnimation *m_animation;

private slots:
    void mousePressEvent(QMouseEvent *e);

signals:
    void hovered();

    void leaved();

    void rightClicked();

};

#endif // HOVERBUTTON_H
