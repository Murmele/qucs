#include "mouseactionschematic.h"
#include "schematic_scene.h"
#include "element_graphics.h"
#include "schematic_doc.h"
#include "component_widget.h"

#include "QGraphicsSceneMouseEvent"
#include "QMimeData"

// #include "element.h"

MouseActionSchematic::MouseActionSchematic(MouseActionsHandler &ctx):
    MouseAction(ctx)
{

}

// TODO: neccesary?
//// SchematicMouseAction::doc?
//const SchematicDoc &MouseActionSchematic::doc() const
//{itested();
//    // TODO: why this is not a recursive call?
//  return doc();
//}

SchematicDoc& MouseActionSchematic::doc() const
{itested();

  QucsDoc* c=&ctx().doc();
  auto cc = dynamic_cast<SchematicDoc*>(c);
  assert(cc);
  return *cc;
}

QPointF MouseActionSchematic::mapToScene(QPoint const& p) const
{
    // TODO: move out of MouseAction!
    unreachable();
    QucsDoc* d=&ctx().doc();
    auto cc = dynamic_cast<SchematicDoc*>(d);
    assert(cc);
    return cc->mapToScene(p);
}

void MouseActionSchematic::updateViewport()
{itested();
  ctx().updateViewport(); // use a signal?
}

SchematicScene const* MouseActionSchematic::scene() const
{
    // TODO: why the Mouseaction must have access to the scene?
    unreachable();
//    return doc().scene();
    return nullptr;
}

QUndoCommand* MouseActionSchematic::handle(QEvent* e)
{itested();
  assert(e);

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

/*--------------------------------------------------------------------------*/
bool MouseActionSchematic::isNode(pos_t const& p) const
{
    // return scene()->isNode(fX, fY);?
    return doc().isNode(p);
}
/*--------------------------------------------------------------------------*/
bool MouseActionSchematic::isConductor(pos_t const&p) const
{itested();
    assert(scene());
    return scene()->isConductor(p);
}

void MouseActionSchematic::sceneAddItem(ElementGraphics* x)
{
  doc().sceneAddItem(x);
}
void MouseActionSchematic::sceneRemoveItem(ElementGraphics* x)
{
  doc().sceneRemoveItem(x);
}

QList<ElementGraphics*> MouseActionSchematic::selectedItems()
{
   // TODO: BUG. copies.
  return doc().selectedItems();
}

void MouseActionSchematic::setCursor(QCursor const& c)
{
  doc().setCursor(c);
}
