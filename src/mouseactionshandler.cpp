#include "mouseactionshandler.h"
#include "mouseactions.h"
#include "io_trace.h"
#include "schematic_doc.h"
#include "qucsdoc.h"

#include "QGraphicsSceneMouseEvent"
#include <QApplication>
#include <QClipboard>
#include <QMenu>


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

// TODO: move to schematic_doc or _scene
// what does it return?
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

// only mouse actions? how about undoable keyboard actions?
MouseActionsHandler::MouseActionsHandler(QucsDoc &d) :
    mDoc(d)
{
    itested();
  //selElem  = 0;  // no component/diagram is selected
  //focusElement.clear(); //element being interacted with mouse

  // ...............................................................
  // initialize menu appearing by right mouse button click on component
  ComponentMenu = new QMenu();
  // focusMEvent   = new QMouseEvent(QEvent::MouseButtonPress, QPoint(0,0),
  //                 Qt::NoButton, Qt::NoButton, Qt::NoModifier);

}

MouseActionsHandler::~MouseActionsHandler()
{itested();
  delete ComponentMenu;
//  delete focusMEvent;
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
  if(e->isAccepted()){itested();
  }else{itested();
    // drop event?
  }
  assert(e);
  e->ignore(); // but why??
  return false; // don't filter
}
