#include "mouseactionschematic.h"
#include "schematic_scene.h"
#include "element_graphics.h"
#include "schematic_doc.h"

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
    unreachable();
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
