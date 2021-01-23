/* (c) 2020 Felix Salfelder
 * GPLv3+
 */
#include "mouseactions.h" // BUG
#include "io_trace.h"
#include "schematic_doc.h"
#include <QAction>
#include <QGraphicsSceneEvent> // probably bug
#include "component_widget.h" // bug
#include <QStringLiteral> //bug
#include <QMimeData> //bug
QUndoCommand* MouseAction::handle(QEvent* e)
{itested();
  assert(e);

  //TODO: this function content was moved to MouseActionSchematic::handle()
  // Is it still needed here?

  trace1("MouseAction::handle: event type:", e->type());
  auto* m = prechecked_cast<QGraphicsSceneMouseEvent*>(e);
//  auto* s = prechecked_cast<QGraphicsSceneEvent*>(e);
  auto a = ComponentWidget::itemMimeType();

  if(dynamic_cast<QDragLeaveEvent*>(e)){ untested();
    unreachable();
    return leave(m);
  }else if(auto de = dynamic_cast<QDragEnterEvent*>(e)){ untested();
    trace1("dragenter", de->mimeData()->formats()[0]);
    unreachable();

    if (de->mimeData()->hasFormat(a)){ untested();
      // got a pointer. possibly from ComponentWidget
      e->accept();
    // }else if (de->mimeData()->hasFormat(someThingElse)){ untested();
    //   unreachable();
    //   e->accept();
    } else{ untested();
      e->ignore();
    }

    return enter(m);

    return nullptr;
  }else if(auto de = dynamic_cast<QDragMoveEvent*>(e)){ untested();
    trace1("dragmove", de->mimeData()->formats()[0]);
  }else if(auto de = dynamic_cast<QDropEvent*>(e)){ untested();
    trace1("dragdrop", de->mimeData()->formats()[0]);
  }else{
  }

  // why??
  switch(e->type()){
//  case QEvent::MouseLeave:
//    assert(m);
//    return leave(m);
  case QEvent::Leave:
    return leave(e);
  case QEvent::Enter:
    return enter(e);
  case QEvent::GraphicsSceneMouseDoubleClick:
	 untested();
	 return dblclk(e);
  case QEvent::GraphicsSceneMouseMove:
    // getting here when moving elements.
    // fall through
  case QEvent::MouseMove:
    return move(e);
  case QEvent::GraphicsSceneMouseRelease:
    itested();
    return release(e);
  case QEvent::GrabMouse:itested();
    return nullptr;
//    return grab(s);
  case QEvent::DragEnter: untested();
    unreachable(); // proper type check above
    return nullptr;
    // fallthrough
  case QEvent::GraphicsSceneMousePress:itested();
    return press(e);
  case QEvent::MouseButtonPress: // fall through
  case QEvent::MouseButtonRelease:
      // should not occur
      trace0("Should never occur");
  default:
    // It is not a mouse action, so ignore it.
    return nullptr;
  }
}

QUndoCommand* MouseAction::activate(QObject* sender)
{itested();
  _sender = dynamic_cast<QAction*>(sender);
  return nullptr;
}

QUndoCommand* MouseAction::deactivate()
{itested();
  return nullptr;
}

void MouseAction::uncheck()
{
  if(_sender){itested();
    _sender->blockSignals(true); // do not call toggle slot
    _sender->setChecked(false);       // set last toolbar button off
    _sender->blockSignals(false);
  }else{itested();
  }

  deactivate(); // BUG?
}
