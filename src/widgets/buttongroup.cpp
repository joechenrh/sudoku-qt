#include "buttongroup.h"

ButtonGroup::ButtonGroup(int size, QWidget *parent)
    : QWidget(parent), m_keyPosition(-1)
{
    this->setFixedSize(size, size);

    int spacing = 5;
    int buttonSize = (size - spacing) / 2;

    m_undoButton = new QPushButton(this);
    m_undoButton->setFixedSize(buttonSize, buttonSize);
    m_undoButton->setStyleSheet(QString("QPushButton{border-top-left-radius:%1px;}").arg(buttonSize));
    m_undoButton->move(0, 0);
    m_undoButton->setEnabled(false);

    m_redoButton = new QPushButton(this);
    m_redoButton->setFixedSize(buttonSize, buttonSize);
    m_redoButton->setStyleSheet(QString("QPushButton{border-top-right-radius:%1px;}").arg(buttonSize));
    m_redoButton->move(buttonSize + spacing, 0);
    m_redoButton->setEnabled(false);

    m_undoKeyButton = new QPushButton(this);
    m_undoKeyButton->setFixedSize(buttonSize, buttonSize);
    m_undoKeyButton->setStyleSheet(QString("QPushButton{border-bottom-left-radius:%1px;}").arg(buttonSize));
    m_undoKeyButton->move(0, buttonSize + spacing);
    m_undoKeyButton->setEnabled(false);

    m_redoKeyButton = new QPushButton(this);
    m_redoKeyButton->setFixedSize(buttonSize, buttonSize);
    m_redoKeyButton->setStyleSheet(QString("QPushButton{border-bottom-right-radius:%1px;}").arg(buttonSize));
    m_redoKeyButton->move(buttonSize + spacing, buttonSize + spacing);
    m_redoKeyButton->setEnabled(false);

    m_setKeyButton = new QPushButton(this);
    m_setKeyButton->setFixedSize(buttonSize + 1, buttonSize + 1);
    m_setKeyButton->setStyleSheet(QString("QPushButton{border-radius:%1px;}").arg(buttonSize / 2));
    m_setKeyButton->move((size - buttonSize) / 2, (size - buttonSize) / 2);
    m_setKeyButton->setEnabled(false);

    connect(m_undoButton,    &QPushButton::clicked, [&](){buttonClicked("undo");});
    connect(m_redoButton,    &QPushButton::clicked, [&](){buttonClicked("redo");});
    connect(m_undoKeyButton, &QPushButton::clicked, [&](){buttonClicked("keyUndo");});
    connect(m_redoKeyButton, &QPushButton::clicked, [&](){buttonClicked("keyRedo");});
    connect(m_setKeyButton,  &QPushButton::clicked, [&](){buttonClicked("setKey");});

    setColorStyle(QJsonObject());
}

void ButtonGroup::setColorStyle(QJsonObject json)
{
    QString backgroundColor = "#BBBBBB";
    QString hoverColor = "#CCCCCC";
    QString pressedColor = "#DDDDDD";
    QString unabledColor = "#121212";

    //QString backgroundColor = json["background_color"].toString();
    //QString hoverColor = json["hover_color"].toString();
    //QString pressedColor = json["pressed_color"].toString();
    //QString unabledColor = json["unabled_color"].toString();

    this->setStyleSheet(QString("QPushButton{background-color: %1;}"
                                "QPushButton:hover{background-color: %2;}"
                                "QPushButton:pressed{background-color: %3;}"
                                "QPushButton:!enabled{background-color: %4;}")
                                .arg(backgroundColor, hoverColor, pressedColor, unabledColor));
}

void ButtonGroup::buttonClicked(QString type)
{
    if (type == "undo")
    {
        emit sendOps(undoButtonClicked());
    }
    else if (type == "redo")
    {
        emit sendOps(redoButtonClicked());
    }
    else if (type == "keyUndo")
    {
        emit sendOps(undoKeyButtonClicked());
    }
    else if (type == "keyRedo")
    {
        emit sendOps(redoKeyButtonClicked());
    }
    else
    {
        setKeyButtonClicked();
    }
}

void ButtonGroup::addOpeartion(Op op)
{
    // 如果keyPosition在redo栈中，就清除keyPosition
    // 否则就没有影响
    if (m_keyPosition >= m_undoStack.size())
    {
        m_keyPosition = -1;
        m_setKeyButton->setEnabled(true);
        m_undoKeyButton->setEnabled(false);
        m_redoButton->setEnabled(false);
    }

    m_undoStack.push(op);
    m_redoStack.clear();
    m_undoButton->setEnabled(true);
    m_redoButton->setEnabled(false);
}

QList<Op> ButtonGroup::undoButtonClicked()
{
    Op op = m_undoStack.pop();
    m_undoStack.push(op);

    m_redoButton->setEnabled(true);
    m_undoButton->setEnabled(m_undoStack.size() > 0);

    // 只有key没有设置，或在undo栈的最上方时，才能设置/取消key
    m_setKeyButton->setEnabled(m_undoStack.size() > 0 && (m_keyPosition == -1 || m_undoStack.top().isKeyOp));

    // 只有当key存在，且不在undo栈的最后一个时，才能向后回退到关键帧
    m_undoKeyButton->setEnabled(m_keyPosition > -1 && m_keyPosition < m_undoStack.size() - 1);

    // 只有当key存在，且不在redo栈的第一个时，才能向前回退到关键帧
    m_redoButton->setEnabled(m_keyPosition > -1 && m_keyPosition > m_undoStack.size());

    QList<Op> ops;
    ops.push_back(Op(op.row, op.col, op.after, op.before));
    return ops;
}

QList<Op> ButtonGroup::redoButtonClicked()
{
    Op op = m_redoStack.pop();
    m_redoStack.push(op);

    m_undoButton->setEnabled(true);
    m_redoButton->setEnabled(m_redoStack.size() > 0);

    // 只有key没有设置，或在undo栈的最上方时，才能设置/取消key
    m_setKeyButton->setEnabled(m_undoStack.size() > 0 && (m_keyPosition == -1 || m_undoStack.top().isKeyOp));

    // 只有当key存在，且不在undo栈的最后一个时，才能向后回退到关键帧
    m_undoKeyButton->setEnabled(m_keyPosition > -1 && m_keyPosition < m_undoStack.size() - 1);

    // 只有当key存在，且不在redo栈的第一个时，才能向前回退到关键帧
    m_redoButton->setEnabled(m_keyPosition > -1 && m_keyPosition > m_undoStack.size());

    QList<Op> ops;
    ops.push_back(op);
    return ops;
}

QList<Op> ButtonGroup::undoKeyButtonClicked()
{
    QList<Op> ops;
    while (m_keyPosition != m_undoStack.size() - 1)
    {
        Op op = m_undoStack.pop();
        ops.push_back(Op(op.row, op.col, op.after, op.before));
        m_redoStack.push(op);
    }
    m_undoKeyButton->setEnabled(false);
    m_setKeyButton->setEnabled(true);
    m_redoButton->setEnabled(true);
    return ops;
}

QList<Op> ButtonGroup::redoKeyButtonClicked()
{
    QList<Op> ops;
    while (m_keyPosition != m_undoStack.size() - 1)
    {
        Op op = m_redoStack.pop();
        ops.push_back(op);
        m_undoStack.push(op);
    }
    m_undoKeyButton->setEnabled(false);
    m_setKeyButton->setEnabled(true);
    m_undoButton->setEnabled(true);
    m_redoButton->setEnabled(m_redoStack.size() > 0);
    return ops;
}

void ButtonGroup::setKeyButtonClicked()
{
    Op op = m_undoStack.pop();
    if (m_keyPosition && op.isKeyOp)
    {
        op.isKeyOp = false;
        m_keyPosition = -1;
    }
    else if(!m_keyPosition && !op.isKeyOp)
    {
        op.isKeyOp = true;
        m_keyPosition = m_undoStack.size();
    }
    m_undoStack.push(op);
}
