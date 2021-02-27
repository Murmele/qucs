#ifndef UNDOCOMMAND_H
#define UNDOCOMMAND_H

#include <QUndoCommand>

// TODO: how to check if the object still exists?
// At the moment, the object will also deleted with an undo command, so it is not possible to change
// a value and the object does not exist anymore.

template <class T>
class UndoCommand : public QUndoCommand
{
public:
    UndoCommand(const T& oldValue, const T& newValue, T& property): mOldValue(oldValue), mNewValue(newValue), mProperty(property) {
    }

    virtual void undo() override{
     mProperty = mOldValue;
    }

    virtual void redo() override{
      mProperty = mNewValue;
    }

private:
    T mOldValue;
    T mNewValue;
    T& mProperty;
};

#endif // UNDOCOMMAND_H
