#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>
#include <QJsonObject>
#include <QPushButton>
#include <QStack>

struct Op
{
    int row;    // 操作的行数
    int col;    // 操作的列数
    int before; // 更改前的值
    int after;  // 更改后的值
    bool isKeyOp;

    Op(int _row = 0, int _col = 0, int _before = 0, int _after = 0)
        : row(_row), col(_col), before(_before), after(_after), isKeyOp(false) {}
};

class ButtonGroup : public QWidget
{

    Q_OBJECT

public:
    ButtonGroup(int size, QWidget *parent = nullptr);

    void setColorStyle(QJsonObject json);

    void addOpeartion(Op op);

    void clear();

signals:
    void sendOps(QList<Op>);

private slots:
    QList<Op> undoButtonClicked();

    QList<Op> redoButtonClicked();

    QList<Op> undoKeyButtonClicked();

    QList<Op> redoKeyButtonClicked();

    void setKeyButtonClicked();

    void buttonClicked(QString type);

private:
    QPushButton *m_undoButton;

    QPushButton *m_redoButton;

    QPushButton *m_undoKeyButton;

    QPushButton *m_redoKeyButton;

    QPushButton *m_setKeyButton;

    QStack<Op> m_redoStack;

    QStack<Op> m_undoStack;

    int m_keyPosition;

public slots:
};

#endif // BUTTONGROUP_H
