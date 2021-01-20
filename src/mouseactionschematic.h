#ifndef MOUSEACTIONSCHEMATIC_H
#define MOUSEACTIONSCHEMATIC_H

#include "mouseactions.h"
#include "geometry.h"

class SchematicDoc;
class SchematicScene;
class Node;

/*!
 * \brief The MouseActionSchematic class
 * Base class for mouse actions related to a
 * schematic document
 */
class MouseActionSchematic: public MouseAction
{
public:
    MouseActionSchematic(MouseActionsHandler& ctx);

protected:
//    const SchematicDoc &doc() const;
    SchematicScene const* scene() const;
    SchematicDoc &doc() const; // BUG _ctx.

    void sceneAddItem(ElementGraphics*);
    void sceneRemoveItem(ElementGraphics*);

    QList<ElementGraphics*> selectedItems(); // BUG. copies.
    QPointF mapToScene(QPoint const& p) const;
    void updateViewport(); // why?
    void setCursor(QCursor const& c);
    bool isNode(pos_t const&) const; // needed??
    bool isConductor(pos_t const&) const;

    //template<class T>
    //void possibly_merge_symbols(pos_t remove_at, T& rem, T& add);
    QList<ElementGraphics*> items(const QPointF &pos,
                             Qt::ItemSelectionMode mode=Qt::IntersectsItemShape,
                             Qt::SortOrder order = Qt::DescendingOrder) const;

    Node const* nodeAt(pos_t) const;
};

#endif // MOUSEACTIONSCHEMATIC_H
