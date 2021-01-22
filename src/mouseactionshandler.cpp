#include "mouseactionshandler.h"
#include "mouseactions.h"
#include "io_trace.h"
#include "schematic_doc.h"
#include "qucsdoc.h"

#include "QGraphicsSceneMouseEvent"
#include <QApplication>
#include <QClipboard>


void MouseActionsHandler::updateViewport()
{itested();

  SchematicDoc* s = dynamic_cast<SchematicDoc*>(&doc());
  if(s){
    s->viewport()->update(); // use signal?
  }else{ untested();
  }
}

QucsDoc& MouseActionsHandler::doc() const
{
  return mDoc;
}

bool MouseActionsHandler::handle(QEvent*e)
{itested();
    QUndoCommand* c=nullptr;
    if(_maCurrent){itested();
    // creates a new undo command
    c = _maCurrent->handle(e);
    }else{
    }

    // emit signalUndoState(true);
    if (!executeCommand(c)) {
        delete c;
        return false;
    }
    return true;

}

bool MouseActionsHandler::executeCommand(QUndoCommand* c)
{
  return mDoc.pushUndoStack(c);
}

MouseActionsHandler::MouseActionsHandler(QucsDoc &d) :
    mDoc(d)
{
    itested();
}

MouseActionsHandler::~MouseActionsHandler()
{itested();
}
void MouseActionsHandler::setActive(MouseAction* a)
{
  if(_maCurrent){itested();
    _maCurrent->uncheck();
  }else{itested();
  }
  _maCurrent = a;
}

// not sure why this is needed
bool MouseActionsHandler::eventFilter(QObject *, QEvent *e)
{
    untested();
  if(e->isAccepted()){itested();
  }else{itested();
    // drop event?
  }
  assert(e);
  e->ignore(); // but why??
  return false; // don't filter
}
