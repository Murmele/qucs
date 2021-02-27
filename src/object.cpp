
#include "object.h"
#include "undocommand.h"

void Object::setLabel(std::string const& l) {
    pushUndoStack(new UndoCommand<std::string>(_label, l, _label));
}
void Object::setLabel(char const* l) {
    setLabel(std::string(l));
}

// base error handler. this is for errors related to qucs
//
// has nothing to do with http://qt-project.org/doc/qt-4.8/qtglobal.html#qInstallMsgHandler
// which is meant for Qt debugging.
void Object::message(QucsMsgType t, const char* m) const
{
	incomplete();
	std::cerr << label() << ":" << t << " " << m << "\n";
	// if(gui)
	//   messagebox
	// else
	//   printf
	//
	// if logfile
	//   printf to logfile
}

void Object::message(QucsMsgType t, std::string const& m) const
{
	std::cerr << label() << ":" << t << " " << m << "\n";
}

bool Object::pushUndoStack(QUndoCommand* cmd) {

    //assert(mUndoStack); // I think every Object should have a valid undostack pointer
    if (!cmd)
        return false;

    if (mUndoStack) {
        mUndoStack->push(cmd);
        return true;
    }

    // Create dummy Undostack and push it on there.
    // So the property changes are asigned, but cannot reverted
    QUndoStack stack;
    stack.push(cmd);
    // TODO: when the stack is deleted will the UndoCommand deleted too or
    // must be done manually?

    return false;
}
