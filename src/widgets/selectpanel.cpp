#include "selectpanel.h"

#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QApplication>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

SelectPanel* SelectPanel::m_instance = nullptr;
QMutex SelectPanel::m_mutex;

SelectPanel::SelectPanel(int size, QWidget *parent)
    : QDialog(parent), opened(false)
{
    m_buttons.resize(9);
    m_selected.resize(9);

    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFixedSize(size, size);

    QFont hoverFont("华文新魏", 16);
    hoverFont.setBold(true);

    QFont normalFont("华文新魏", 12);

    QString enterStylesheet = "QWidget{color:#FAFAFA; background-color:#FB78A5; border:0px solid black;}";
    QString leaveStylesheet = "QWidget{color:#FBDFE8; background-color:#FB78A5; border:0px solid black;}";

    int buttonSize = size / 3;
    for( int r = 0; r < 3; r++)
    {
        for(int c = 0; c < 3; c++)
        {
            HoverButton *button = new HoverButton(this);
            button->setFont(normalFont);
            button->setText(QString::number(r * 3 + c + 1));
            button->setStyleSheet(leaveStylesheet);
            button->move(buttonSize * c, buttonSize * r);
            button->setFixedSize(buttonSize, buttonSize);
            m_buttons[r * 3 + c] = button;

            connect(button, &HoverButton::hovered,      [=](){ button->setFont(hoverFont); button->setStyleSheet(enterStylesheet); });
            connect(button, &HoverButton::leaved,       [=](){ button->setFont(normalFont); button->setStyleSheet(leaveStylesheet); });
            connect(button, &HoverButton::clicked,      [=](){ emit finish(QList<int>{r * 3 + c + 1, m_row, m_col}); this->close(); });
            connect(button, &HoverButton::rightClicked, [=](){ m_selected[r * 3 + c] = 1; });
        }
    }

    m_dummylabel = new QLabel(this);
    m_dummylabel->setStyleSheet("background-color:#FB78A5;");
    m_dummylabel->setGeometry(15, 15, 25, 25);
    m_dummylabel->hide();
    drawMask();

    // 进入和退出的动画效果
    m_animation = new QPropertyAnimation(m_dummylabel, "geometry");
    m_animation->setDuration(100);
    m_animation->setEasingCurve(QEasingCurve::OutQuad);
    m_animation->setStartValue(QRect(15, 15, 25, 25));
    m_animation->setEndValue(QRect(0, 0, 75, 75));
    connect(m_animation, &QPropertyAnimation::valueChanged, [&](){ drawMask(); });
    //connect(m_animation, &QPropertyAnimation::finished, [&](){if (!opened) {QDialog::close();}});
}

SelectPanel* SelectPanel::instance(int size, QWidget* parent)
{
    if(!m_instance)
    {
        QMutexLocker lock(&m_mutex);
        if(!m_instance)
        {
            SelectPanel *pInstance = new SelectPanel(size, parent);  // 修改处
            m_instance = pInstance;                                  // 修改处
        }
    }
    return m_instance;
}


int SelectPanel::exec()
{
    opened = true;
    selected = 0;
    if (m_animation->state() != QAbstractAnimation::Stopped)
    {
        m_animation->stop();
    }

    m_dummylabel->setGeometry(QRect(15, 15, 45, 45));
    m_animation->setStartValue(QRect(15, 15, 45, 45));
    m_animation->setEndValue(QRect(0, 0, 75, 75));
    drawMask();
    m_animation->start();

    return QDialog::exec();
}

bool SelectPanel::close()
{
    // TODO: 第一次关闭时不要淡出
    opened = false;

    /*
    if (m_animation->state() != QAbstractAnimation::Stopped)
    {
        m_animation->stop();
    }

    m_animation->setStartValue(m_animation->currentValue());
    m_animation->setEndValue(QRect(20, 20, 35, 35));
    m_animation->start();
    */

    return QDialog::close();
}

bool SelectPanel::isOpened() const
{
    return opened;
}


void SelectPanel::enterEvent(QEvent *e)
{
    if (e)
    {
        QApplication::sendEvent(m_base, e);
    }
}

void SelectPanel::leaveEvent(QEvent *e)
{
    if (e)
    {
        QApplication::sendEvent(m_base, e);
    }
}

void SelectPanel::setBase(QWidget *widget, int row, int col)
{
    m_base = widget;
    m_row = row;
    m_col = col;
}

void SelectPanel::drawMask()
{
    int size = this->geometry().width();

    QImage img = QImage(QSize(size, size), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOut);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(0, 0, 0, 0)));
    painter.drawRect(QRect(0, 0, 75, 75));

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(0, 0, 0, 255)));
    painter.drawRect(m_dummylabel->geometry());

    this->setMask(QPixmap::fromImage(img).mask());
}
