#ifndef HOVERBUTTON_H
#define HOVERBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QTimer>
#include <QPropertyAnimation>

class HoverButton : public QPushButton
{
    Q_OBJECT

public:
    HoverButton(QWidget *parent = NULL);

    void setOpacity(double oppacity);

    void hide();

    void vis();

protected:
    void enterEvent( QEvent* e );

    void leaveEvent( QEvent* e );

private:
    bool entered;

    QTimer *timer;

    QPropertyAnimation *m_animation;

private slots:
    void mousePressEvent(QMouseEvent *e);

signals:

    void hovered();

    void leave();

    void rightClicked();

};

#endif // HOVERBUTTON_H
