/***************************************************************************
    copyright            : (C) 2020 Felix Salfelder

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*!
 * user interaction with the schematic, undo commands
 * derived from various Qucs "MouseAction" etc. *
 */

#include <QAction>
#include <QMessageBox> // BUG
// #include <QLineEdit> // BUG?
// #include <QRegExp> // BUG?
#include <QRegExpValidator>
#include <QFileDialog>
#include <QUndoCommand>
#include <QGraphicsSceneEvent>
#include <QGraphicsItem>
#include <QMenu>

#include "schematic_doc.h"
#include "qucs.h"
#include "misc.h"
#include "globals.h"
#include "schematic_action.h"
#include "schematic_dialog.h"
#include "schematic_edit.h"
#include "globals.h"
#include "mouseactionschematic.h"
#include "qucsdoc.h"

#include "changedialog.h"
//#include "components/component.h" // BUG

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#include "action/zoom.cpp" // for now.
/*--------------------------------------------------------------------------*/
#include "action/wire.cpp" // for now.
/*--------------------------------------------------------------------------*/

class MouseActionSelect : public MouseActionSchematic{
public:
    explicit MouseActionSelect(MouseActionsHandler& ctx)
        : MouseActionSchematic(ctx) {}

private: // override
//	cmd* activate(QAction* sender) override;
	cmd* move(QEvent*) override;
	cmd* press(QEvent*) override;
    cmd* release(QEvent*) override;
	//	cmd* release2(QMouseEvent*); // what is this?
	// cmd* enter(QEvent*) override;
	cmd* dblclk(QEvent*) override;

#if 0
private: // rectangles?  // this was in MouseActionsHandler. BUG. remove
	int MAx1;
	int MAy1;
	int MAx2;
	int MAy2;
#endif
private:
    void showSchematicWidget(ElementGraphics *gfx);
    cmd* release_left(QEvent*);

protected:
	void setPos1(QPointF pos){ untested();
		_pos1 = pos;
	}
	QPointF const& pos1() const{ untested();
		return _pos1;
	}

private: // more decoupling
	//ElementMouseAction focusElement;
    bool isMoveEqual{false}; // mouse cursor move x and y the same way
	QPointF _pos1;
}; // MouseActionSelect
/*--------------------------------------------------------------------------*/
/*!
 * \brief MouseActionSelect::dblclk
 * Double click. This method handles the itemevents from the graphicsitems.
 * \param evt
 * \return
 */
QUndoCommand* MouseActionSelect::dblclk(QEvent* evt)
{ untested();
	incomplete();
	//  Doc->releaseKeyboard();  // allow keyboard inputs again
	//  QucsMain->editText->setHidden(true);
	//  editElement(Doc, Event);
	ElementGraphics* gfx = nullptr;
	//
	if(auto i = dynamic_cast<ItemEvent*>(evt)){ untested();
		// QList<ElementGraphics*> l;
		gfx = &i->item();
		// l.push_back(&i->item());
        assert(gfx);
        showSchematicWidget(gfx);
    }

    return nullptr;
}
/*--------------------------------------------------------------------------*/

/*!
 * \brief MouseActionSelect::showSchematicWidget
 * Shows the element schematic widget to edit the properties of the element
 * \param gfx The graphicsitem to which the element is attached. From this the element is extracted
 */
void MouseActionSelect::showSchematicWidget(ElementGraphics* gfx)
{
    Element* elt = element(gfx);
    if (!elt) {
        unreachable();
        trace() << "Element is not valid.";
        return;
    }

    std::unique_ptr<QDialog> ew = elt->createSchematicWidget(&doc());
    if (!ew) {
        trace() << "no editElement";
        incomplete();
        return;
    }

    trace() << "got editElement";
    assert(gfx);

    if(auto eew=prechecked_cast<SchematicDialog*>(ew.get())){ untested();
		assert(gfx);
		eew->attach(gfx);
		if(eew->exec() != 1){ untested();
			// done=true;   // dialog is WDestructiveClose
		}else{ untested();
			incomplete();
		}
	}else{
	}
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
// stange glue that should perhaps go into a class derived from QAction and
// replace the button in the toolbar.
template<class CMD>
class MouseActionSelCmd : public MouseActionSchematic{
public:
    explicit MouseActionSelCmd(MouseActionsHandler& ctx)
        : MouseActionSchematic(ctx){}

private:
	cmd* activate(QObject* sender) override;
	cmd* deactivate() override;
//	cmd* move(QEvent*) override;
	cmd* press(QEvent*) override;
    cmd* release(QEvent*) override;
//	cmd* generic(QEvent*) override;
private:
	QCursor _oldcursor;
};
/*--------------------------------------------------------------------------*/
template<class CMD>
QUndoCommand* MouseActionSelCmd<CMD>::deactivate()
{itested();
	setCursor(_oldcursor);
	return MouseAction::deactivate();
}
/*--------------------------------------------------------------------------*/
template<class CMD>
QUndoCommand* MouseActionSelCmd<CMD>::activate(QObject* sender)
{itested();
	MouseAction::activate(sender); // ...

	_oldcursor = doc().cursor();
	setCursor(Qt::CrossCursor);

	auto s = doc().selectedItems();
	bool selected = !s.empty();

	if(selected){itested();
		auto cmd = new CMD(doc(), s);
		return cmd;
	}else{ untested();
		return nullptr;
	}
}
/*--------------------------------------------------------------------------*/
template<class CMD>
QUndoCommand* MouseActionSelCmd<CMD>::press(QEvent* e)
{ untested();
	if(!e){ untested();
		return nullptr;
	}else if(auto i = dynamic_cast<ItemEvent*>(e)){ untested();
		QList<ElementGraphics*> l;
		l.push_back(&i->item());
		return new CMD(doc(), l);
	}else{ untested();
		trace1("delete::scene unknown sender", e->type());
		return nullptr;
	}
} // select::press
/*--------------------------------------------------------------------------*/
template<class CMD>
QUndoCommand* MouseActionSelCmd<CMD>::release(QEvent*)
{ untested();
	incomplete(); // why?
	return nullptr;
} // select::release
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
// swapSelection?
class DeleteSelection : public SchematicEdit {
public:
	template<class IT>
	DeleteSelection(SchematicDoc& ctx, IT deletelist)
	  : SchematicEdit(*ctx.sceneHACK()) { untested();
        int k = 0;

		qDelete(deletelist);

		setText("delete " + QString::number(k) + " items");
	}
}; // DeleteSelection
typedef MouseActionSelCmd<DeleteSelection> MouseActionDelete;
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void transformElement(Element* e, qucsSymbolTransform a, pos_t pivot)
{itested();
	trace1("..", a.degrees_int());
	assert(!(a.degrees_int()%90));
	assert(e);
	if(auto* s=dynamic_cast<Symbol*>(e)){ untested();
		int angle = 0;
		int vflip = 0;
		int hflip = 0;
		int mx = 0;
		int my = 0;
		unsigned r = 0;
		try {itested();
			std::string mxs = s->paramValue("$hflip");
			hflip = atoi(mxs.c_str()); // \pm 1
			//assert(hflip == 1); // for now.
			// 1  |-> 0
			// -1 |-> 1
			mx = (1 - hflip) / 2;
		}catch(ExceptionCantFind const&){ untested();
			incomplete();
		}
		try {itested();
			// this is what legacy used ("mirrorX", vertical flip);
			std::string mys = s->paramValue("$vflip");
			vflip = atoi(mys.c_str());
			// 1  |-> 0
			// -1 |-> 1
			my = (1 - vflip) / 2;
		}catch(ExceptionCantFind const&){ untested();
			unreachable();
		}
		try {itested();
			std::string rs = s->paramValue("$angle");
			angle = atoi(rs.c_str());
			assert(!(angle%90));
			assert(angle<360);
			r = angle/90;
		}catch(ExceptionCantFind const&){ untested();
			unreachable();
		}

		trace3("pretransform", hflip, vflip, angle);

		assert(mx==0 || mx==1);
		assert(my==0 || my==1);
		assert(r < 4); // yikes //

		//assert(hflip==1);

		rotate_after_mirror current(int(r*90), bool(mx), bool(my));
		assert(!(current.degrees_int()%90));
		rotate_after_mirror1_t new_mr = a * current;
		assert(!(new_mr.degrees_int()%90));

		vflip = -2 * int(new_mr.mirror()) + 1;

		s->setParameter(std::string("$hflip"), std::string("1"));
		s->setParameter(std::string("$vflip"), std::to_string(vflip));
		s->setParameter(std::string("$angle"), std::to_string(new_mr.degrees_int()));

		trace3("posttransform", hflip, vflip, new_mr.degrees_int());

        auto p = e->position();
        //trace1("posttransform setpos0", p);
		int x = getX(p);
		int y = getY(p);

        x -= pivot.x();
        y -= pivot.y();

		pos_t new_xy(x,y);
		new_xy = a.apply(new_xy);

        x = pivot.x() + new_xy.x();
        y = pivot.y() + new_xy.y();

		// todo: rounding errors.

		p = pos_t(x, y);
        //trace1("posttransform setpos1", p);
		e->setPosition(p);
	// prepareGeometryChange(); // needed??
	}else{ untested();
	}
}
/*--------------------------------------------------------------------------*/
template<class T>
class TransformSelection : public SchematicEdit {
public:
	template<class IT>
	TransformSelection(SchematicDoc& ctx, IT selection, T const& t)
	: SchematicEdit(*ctx.sceneHACK()){itested();
        int k = 0;
		// TODO: bounding box center?
		QRectF bb;
		std::vector<std::pair<ElementGraphics*,Element*>> buf;
		for(auto i : selection){itested();
			++k;
			QGraphicsItem const* g = i;//huh?
			if(auto eg=dynamic_cast<ElementGraphics*>(i)){itested();
				auto br = g->boundingRect();
				bb |= i->mapToScene(br).boundingRect(); // translate(eg->pos());
				Element* e = eg->cloneElement();
				buf.push_back(std::make_pair(eg, e));
				// qSwap(eg, e);
			}else{ untested();
				unreachable(); // really? use prechecked_cast then.
			}
		}

		auto center = bb.center();
		pos_t pivot(getX(center), getY(center));
        //trace1("transform", pivot);

		for(auto i : buf){
			transformElement(i.second, t, pivot);
			qSwap(i.first, i.second);
		}

		setText("transform " + QString::number(k) + " items");
	}
}; // TransformSelection
/*--------------------------------------------------------------------------*/
static const rotate_after_mirror1_t ninety_degree_transform(90, false);
class RotateSelectionTransform : public TransformSelection<qucsSymbolTransform>{
	typedef TransformSelection<qucsSymbolTransform> base;
public:
	template<class IT>
	RotateSelectionTransform(SchematicDoc& ctx, IT selection)
	  : base(ctx, selection, ninety_degree_transform) {}
};
/*--------------------------------------------------------------------------*/
class MirrorXaxisSelection : public TransformSelection<qucsSymbolTransform>{
	typedef TransformSelection<qucsSymbolTransform> base;
public:
	template<class IT>
	MirrorXaxisSelection(SchematicDoc& ctx, IT selection)
	  : base(ctx, selection, transformFlipHorizontally) {}
};
/*--------------------------------------------------------------------------*/
class MirrorYaxisSelection : public TransformSelection<qucsSymbolTransform>{
	typedef TransformSelection<qucsSymbolTransform> base;
public:
	template<class IT>
	MirrorYaxisSelection(SchematicDoc& ctx, IT selection)
	  : base(ctx, selection, transformFlipVertically) {}
};
/*--------------------------------------------------------------------------*/
typedef MouseActionSelCmd<DeleteSelection> MouseActionDelete;
typedef MouseActionSelCmd<DeleteSelection> MouseActionActivate; // TODO
typedef MouseActionSelCmd<RotateSelectionTransform> MouseActionRotate;
typedef MouseActionSelCmd<MirrorXaxisSelection> MouseActionMirrorXaxis;
typedef MouseActionSelCmd<MirrorYaxisSelection> MouseActionMirrorYaxis;
/*--------------------------------------------------------------------------*/

/*!
 * \brief The MouseActionNewElement class
 * Handles all actions related to creating new elements on the schematic
 */
class MouseActionNewElement : public MouseActionSchematic{
public:
    explicit MouseActionNewElement(MouseActionsHandler& ctx, Element const* proto=nullptr)
        : MouseActionSchematic(ctx), _proto(proto)
  	{}
private:
	cmd* activate(QObject* sender) override;
	cmd* deactivate() override;
	cmd* move(QEvent*) override;
	cmd* press(QEvent*) override;
	cmd* enter(QEvent*) override;
	cmd* leave(QEvent*) override;
    cmd* release(QEvent*) override;

private:
    cmd* makeNew(QEvent*);
	cmd* rotate(QEvent*);

private:
    ElementGraphics* _gfx{nullptr};
    /*! Current selected element. This is the element which will be placed in the schematic. */
    Element const* _proto{nullptr};
};
/*--------------------------------------------------------------------------*/
/*!
 * \brief The NewElementCommand class
 * Gets executed when a new element will be placed on the schematic
 *
 * This element gets then the undostack of the doc so undo/redo is possible.
 */
class NewElementCommand : public SchematicEdit {
public:
	NewElementCommand(SchematicDoc& ctx, ElementGraphics* gfx)
	: SchematicEdit(*ctx.sceneHACK()) { untested();
		assert(gfx->scene());
		assert(!element(gfx)->mutable_owner());
        gfx->hide(); // why?
        element(gfx)->setUndoStack(ctx.undoStack());
//		ctx.takeOwnership(element(gfx)); // BUG?
		// elment->setOwner(ctx)...?
		setText("NewElement" /*element(gfx)->label()*/); // tr?
		trace0("NewElementCommand::NewElementCommand");
		qInsert(gfx);
	}
	~NewElementCommand(){ untested();
		// _gfx owns element(_gfx)
		// ctx owns _gfx
	}
}; // NewElementCommand
/*--------------------------------------------------------------------------*/
#include <component_widget.h> // not here.
QUndoCommand* MouseActionNewElement::activate(QObject* sender)
{
	if(auto s=dynamic_cast<ComponentListWidgetItem*>(sender)){
		_proto = s->proto(); // better clone?
	}else{
	}
	return MouseAction::activate(sender);
}
/*--------------------------------------------------------------------------*/
/*!
 * \brief MouseActionNewElement::release
 * Called when the mouse button was released
 * A new element will be created on the schematic if the left mouse button
 * was released
 * \param ev
 * \return
 */
QUndoCommand* MouseActionNewElement::release(QEvent* ev)
{ untested();
	QUndoCommand* cmd = nullptr;
    auto m = dynamic_cast<QGraphicsSceneMouseEvent*>(ev);
	if(!m){ untested();
	}else if(m->button() == Qt::LeftButton){ untested();
		cmd = makeNew(ev);
	}else if(m->button() == Qt::RightButton){ untested();
	}
	return cmd;
}
/*--------------------------------------------------------------------------*/
/*!
 * \brief MouseActionNewElement::makeNew
 * Creates a new ElementCommand with the current graphicsitem.
 * The current graphicsitem gets cloned so another one can
 * be placed on the schematic
 * \param ev
 * \return
 */
QUndoCommand* MouseActionNewElement::makeNew(QEvent* ev)
{ untested();
	// assert(ev->widget=doc->scene()) // or so.
	trace1("RELEASE", ev->type());
    if(ev->type() == QEvent::GraphicsSceneMouseRelease){ itested();
	}else{ untested();
		unreachable();
	}

    assert(element(_gfx));

	if(auto se=dynamic_cast<QGraphicsSceneMouseEvent*>(ev)){ untested();
		QPointF pos = se->scenePos();
		QPoint xx = doc().snapToGrid(pos);
		_gfx->setPos(xx);
	}else{
	}

	cmd* c = new NewElementCommand(doc(), _gfx);

    itested();
    // clone the element, because the current one gets part of the schematic
    _gfx = _gfx->clone(); // new ElementGraphics(elt);
    doc().sceneAddItem(_gfx); // does not attach.
    assert(!element(_gfx)->scope());

	ev->accept();
	return c;
}
/*--------------------------------------------------------------------------*/
/*!
 * \brief MouseActionNewElement::deactivate
 * Called when the action is changed. So the graphicselement
 * attached to the mouse cursor is anymore needed and we can
 * delete it
 * \return
 */
QUndoCommand* MouseActionNewElement::deactivate()
{ untested();
	// assert(!attached);
	doc().sceneRemoveItem(_gfx);
	delete _gfx; // CHECK: who owns _elt?
	_gfx = nullptr;
	incomplete();
	return MouseAction::deactivate();
}
/*--------------------------------------------------------------------------*/

/*!
 * \brief MouseActionNewElement::move
 * Moves the current element attached to the mouse around.
 * \param ev
 * \return
 */
QUndoCommand* MouseActionNewElement::move(QEvent* ev)
{ untested();
	QPointF sp;
	trace1("move", ev->type());
	if(auto ee=dynamic_cast<QMouseEvent*>(ev)){ untested();
		unreachable();
		QPointF wp;
		wp = ee->localPos(); // use oldPos?
		sp = mapToScene(wp.toPoint());
	}else if(auto ee=dynamic_cast<QGraphicsSceneMouseEvent*>(ev)){ untested();
		sp = ee->scenePos();

		QPoint xx = doc().snapToGrid(sp);
		sp = xx;
	}else{ untested();
		unreachable();
	}

	if(_gfx){ untested();
		_gfx->setPos(sp.x(), sp.y());
	}else{ untested();
		unreachable();
	}

	ev->accept();
	return nullptr;
}
/*--------------------------------------------------------------------------*/
/*!
 * \brief MouseActionNewElement::enter
 * Called when entering the schematic scene.
 * This function creates a new element from the selected one and also a
 * graphics item and assign the element. The graphicsitem is stored in this
 * class, as long as the element was not placed on the schematic
 * \param ev
 * \return
 */
QUndoCommand* MouseActionNewElement::enter(QEvent* ev)
{ untested();
	trace1("new enter", ev->type());
	auto ee = prechecked_cast<QEnterEvent*>(ev);
	assert(ee);
	
	auto wp = ee->localPos();

	SchematicDoc* d = &doc();
	auto sp = d->mapToScene(wp.toPoint());

	Element* elt;
    if(!_gfx){ untested(); // should never occur.
		assert(_proto);
		elt = _proto->clone_instance();
		if(auto sym=dynamic_cast<Symbol const*>(elt)){
			elt->setLabel(sym->typeName());
		}else{
		}
		elt->setPosition(pos_t(sp.x(), sp.y()));
		_gfx = new ElementGraphics(elt); // BUG
	}else{ untested();
		_gfx->setPos(sp.x(), sp.y());
	}
	
	doc().sceneAddItem(_gfx);

	ev->accept();
	return nullptr;
}
/*--------------------------------------------------------------------------*/
QUndoCommand* MouseActionNewElement::leave(QEvent* ev)
{ untested();
	sceneRemoveItem(_gfx);
	ev->accept();
	return nullptr;
}
/*--------------------------------------------------------------------------*/
QUndoCommand* MouseActionNewElement::rotate(QEvent*)
{ untested();
    trace() << "Rotate";
	if(!_gfx){ untested();
		unreachable();
	}else if(dynamic_cast<Symbol*>(element(_gfx))){ untested();
		// always do this?
		_gfx->transform(ninety_degree_transform);
	}else{ untested();
		unreachable();
	}
	return nullptr;
}
/*--------------------------------------------------------------------------*/
QUndoCommand* MouseActionNewElement::press(QEvent* ev)
{ untested();
	auto a = dynamic_cast<QMouseEvent*>(ev);
	auto m = dynamic_cast<QGraphicsSceneMouseEvent*>(ev);
	QUndoCommand* cmd = nullptr;
	if(a){ untested();
		unreachable();
		// somehow it is a scene event??
	}else if(!m){ untested();
		trace1("MouseActionNewElement::press", ev->type());
		unreachable();
	}else if(m->button() == Qt::LeftButton){ untested();
	}else if(m->button() == Qt::RightButton){ untested();
		cmd = MouseActionNewElement::rotate(ev);
		ev->accept(); // really?
	}else{ untested();
		unreachable();
	}
	return cmd;
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#include "action/move.cpp" // for now.
#include "action/paste.cpp" // for now.
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
// was: MouseActionsHandler::MMoveSelect
QUndoCommand* MouseActionSelect::move(QEvent *)
{itested();
	// obsolete?
	if(isMoveEqual) {itested();
		// square?
	}else{itested();
	}

	return nullptr;
}
/*--------------------------------------------------------------------------*/
// was: MouseActionsHandler::MPressSelect
QUndoCommand* MouseActionSelect::press(QEvent*)
{itested();

	incomplete();
	return nullptr;
#if 0
	QMouseEvent* e;
	SchematicDoc* Doc = &doc();
	assert(Doc);
	QPointF pos = Doc->mapToScene(e->pos());
	trace2("sel::press", pos, e->pos());
	setPos1(pos);
	float fX=pos.x();
	float fY=pos.y();

	bool Ctrl = e->modifiers().testFlag(Qt::ControlModifier);

	int No=0;

	// memorise first click position
	//MAx1 = int(fX);
	//MAy1 = int(fY);

	focusElement = ctx().selectElement(e->pos(), Ctrl, &No);
	isMoveEqual = false;   // moving not neccessarily square

	incomplete(); //this does not add up.
#if 0
	if(!focusElement){ untested();
	}else if(focusElement->Type == isDiagramHScroll){ untested();
		// BUG: move to selectElement? what is MAy1?!
		MAy1 = MAx1;
	}else{ untested();
	}
#endif

	if(!focusElement){ untested();
		qDebug() << "MPressSelect miss" << e->pos() << pos;
	}else if(focusElement->Type == isPaintingResize){ untested();
		incomplete(); // delegate. how?
#if 0
		focusElement->Type = isPainting;
        QucsMain->MouseReleaseAction = &MouseActionsHandler::MReleaseResizePainting;
        QucsMain->MouseMoveAction = &MouseActionsHandler::MMoveResizePainting;
		QucsMain->MousePressAction = 0;
		QucsMain->MouseDoubleClickAction = 0;
		Doc->grabKeyboard();  // no keyboard inputs during move actions
		// Update matching wire label highlighting
		Doc->highlightWireLabels ();
		return nullptr;
	}else if(focusElement->Type == isDiagramResize){ untested();
		incomplete();

		if(((Diagram*)focusElement)->name().left(4) != "Rect")
			if(((Diagram*)focusElement)->name().at(0) != 'T')
				if(((Diagram*)focusElement)->name() != "Curve")
					/* if(((Diagram*)focusElement)->name() != "Waveac")
						if(((Diagram*)focusElement)->name() != "Phasor")*/
					isMoveEqual = true;  // diagram must be square

		focusElement->Type = isDiagram;
		MAx1 = focusElement->cx_();
		MAx2 = focusElement->x2_();

		focusElement->someDiagramStateCallback()

			// old:
			// if(diagram(focusElement)->State & 1) { untested();
			//   MAx1 += MAx2;
			//   MAx2 *= -1;
			// }

			MAy1 =  focusElement->cy_();
		MAy2 = -focusElement->y2_();
		if(((Diagram*)focusElement)->State & 2) { untested();
			MAy1 += MAy2;
			MAy2 *= -1;
		}

		 // diagram_action?
        QucsMain->MouseReleaseAction = &MouseActionsHandler::MReleaseResizeDiagram;
        QucsMain->MouseMoveAction = &MouseActionsHandler::MMoveSelect;
		QucsMain->MousePressAction = 0;
		QucsMain->MouseDoubleClickAction = 0;
		Doc->grabKeyboard(); // no keyboard inputs during move actions
		// Update matching wire label highlighting
		Doc->highlightWireLabels ();
		return nullptr;

	}else if(focusElement->Type == isDiagramHScroll
			|| focusElement->Type == isDiagramVScroll){ untested();
		incomplete();

		focusElement->Type = isDiagram; // reset happens here. FIXME.

		auto d=diagram(focusElement); // is this necessary?!
		assert(d);
		No = d->scroll(MAy1);

		switch(No) {
		case 1:
			Doc->setChanged(true, true, 'm'); // 'm' = only the first time
			break;
		case 2:  // move scroll bar with mouse cursor
            QucsMain->MouseMoveAction = &MouseActionsHandler::MMoveScrollBar;
			QucsMain->MousePressAction = 0;
			QucsMain->MouseDoubleClickAction = 0;
			Doc->grabKeyboard();  // no keyboard inputs during move actions

			// Remember inital scroll bar position.
			MAx2 = int(d->xAxis_limit_min());
			// Update matching wire label highlighting
			Doc->highlightWireLabels ();
			return;
		}
		// Update matching wire label highlighting
		Doc->highlightWireLabels ();
		Doc->viewport()->update();
		drawn = false;
		return;

	}else if(focusElement->Type == isComponentText){ untested();
		incomplete();

		focusElement->Type &= (~isComponentText) | isComponent;

		MAx3 = No;
		QucsMain->slotApplyCompText();
		// Update matching wire label highlighting
		Doc->highlightWireLabels ();
		return;

	}else if(auto n=node(focusElement)){ untested();
		(void)n;
		if (QucsSettings.NodeWiring) { untested();
			incomplete();

			MAx1 = 0;   // paint wire corner first up, then left/right
			MAx3 = focusElement->cx_();  // works even if node is not on grid
			MAy3 = focusElement->cy_();
            QucsMain->MouseMoveAction = &MouseActionsHandler::MMoveWire2;
            QucsMain->MousePressAction = &MouseActionsHandler::MPressWire2;
			QucsMain->MouseReleaseAction = 0; // if function is called from elsewhere
			QucsMain->MouseDoubleClickAction = 0;

			formerAction = QucsMain->select; // to restore action afterwards
			QucsMain->activeAction = QucsMain->insWire;

			QucsMain->select->blockSignals(true);
			QucsMain->select->setChecked(false);
			QucsMain->select->blockSignals(false);

			QucsMain->insWire->blockSignals(true);
			QucsMain->insWire->setChecked(true);
			QucsMain->insWire->blockSignals(false);
			// Update matching wire label highlighting
			Doc->highlightWireLabels ();
			return;
		}else{ untested();
		}
#endif
	}else{ untested();
		// default case
		// unreachable?
	}

	QucsMain->MousePressAction = 0;
	QucsMain->MouseDoubleClickAction = 0;
	Doc->grabKeyboard();  // no keyboard inputs during move actions
	Doc->viewport()->update();
	//setDrawn(false);

	// Update matching wire label highlighting
	assert(Doc);
	// Doc->highlightWireLabels ();
//	e->ignore(); // handle in QGraphicsView?
	return nullptr;
#endif
} // select::press
/*--------------------------------------------------------------------------*/
// was MouseActionsHandler::MReleaseSelect(SchematicDoc *Doc, QMouseEvent *Event)
QUndoCommand* MouseActionSelect::release(QEvent *ev)
{itested();
	QUndoCommand* cmd = nullptr;
    auto m = dynamic_cast<QGraphicsSceneMouseEvent*>(ev);
	if(!m){ untested();
	}else if(m->button() == Qt::LeftButton){itested();
		cmd = release_left(ev);
	}else if(m->button() == Qt::RightButton){ untested();
	}
	return cmd;
}
/*--------------------------------------------------------------------------*/
static QPoint getDelta(ElementGraphics* e)
{itested();
	auto p = e->pos().toPoint();
	assert(element(e));
    auto p1_ = element(e)->position();
	auto p1 = QPoint(getX(p1_), getY(p1_));
	return p - p1;
}
/*--------------------------------------------------------------------------*/
// it's a "wire" if it has two ports that connect to the same net.
static bool isWire(Symbol const* e)
{
	assert(e);
	if(e->numPorts()!=2){
		return false;
	}else{
	}

	assert(e->portValue(0));
	assert(e->portValue(1));

	return e->portValue(0)->net() == e->portValue(1)->net();
}
/*--------------------------------------------------------------------------*/
inline Symbol* symbol(Element* e)
{
	return dynamic_cast<Symbol*>(e);
}
inline Symbol* symbol(QGraphicsItem* g)
{
	auto e=dynamic_cast<ElementGraphics*>(g);
	if(!e) return nullptr;
	return symbol(e->operator->());
}
inline Symbol* symbol(ElementGraphics* e)
{
	if(!e) return nullptr;
	return symbol(e->operator->());
}

/*--------------------------------------------------------------------------*/
static void selectWireLine(ElementGraphics *g)
{
	Symbol* s = symbol(g);
	assert(isWire(s));
	auto scn = g->scene();
	assert(scn);
	
	for(unsigned i=0; i<s->numPorts(); ++i){
        auto pos = s->nodePosition(i);
        auto items = scn->items(pos.x(), pos.y());
		if(items.size()==2){

			for(auto ii : items){
				Symbol* si = symbol(ii);
				if(!si){
				}else if(g->isSelected()==ii->isSelected()){
				}else if(si == s){
				}else if(isWire(si)){
					ii->setSelected(g->isSelected());
					selectWireLine(ii);
				}else{
				}
			}
		}else{ untested();
		}
	}
}
/*--------------------------------------------------------------------------*/
QUndoCommand* MouseActionSelect::release_left(QEvent *e)
{itested();
    auto m =prechecked_cast<QGraphicsSceneMouseEvent*>(e);
    bool ctrl = m->modifiers().testFlag(Qt::ControlModifier);

	if(!ctrl) {itested();
		incomplete();
	}else{itested();
	}

	cmd* c = nullptr;

	auto s = doc().selectedItems();
	if(s.isEmpty()){ untested();
	}else{itested();

		auto delta = getDelta(s.first());
#ifndef NDEBUG
		for(auto i : s){
			trace2("check delta", delta, getDelta(i));
			assert(delta == getDelta(i));
		}
#endif
		int fX = int(delta.x());
		int fY = int(delta.y());

		if(fX || fY){itested();
			trace1("possible move", delta);
			c = new MoveSelection(delta, doc(), s);
		}else{itested();
		}
	}

	if(c){
	}else if(s.size()!=1){
	}else if(!symbol(s.front())){
    }else if(m->button() == Qt::LeftButton){itested();
			// if it's a wire, select the whole thing?
			// (what is a wire?)
		if(isWire(symbol(s.front()))) { untested();
			incomplete();
#if 1
			selectWireLine(s.front());
#endif
		}else{itested();
		}
	}else{itested();
	}

	doc().releaseKeyboard();
//	Doc->highlightWireLabels ();
	updateViewport();
	// drawn = false;
	return c;
} // select::release
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
SchematicActions::SchematicActions(SchematicDoc* doc)
  : MouseActionsHandler(*doc)
{itested();

	// not entirely clear how to refactor this
	// maybe dispatch mouse actions.
	// or merge into QAction buttons (connect as needed?)

	maDelete = new MouseActionDelete(*this);
	maSelect = new MouseActionSelect(*this);
	maWire = new MouseActionWire(*this);
	maZoomIn = new MouseActionZoomIn(*this);
//	maZoomOut = new MouseActionZoomOut(*this); // not a mouseaction.

	//  maMove = new MouseActionMove(*this);
	Element const* gnd = symbol_dispatcher["GND"];
	assert(gnd);
	maInsertGround = new MouseActionNewElement(*this, gnd);

	Element const* port = symbol_dispatcher["Port"];
	assert(port);
	maInsertPort = new MouseActionNewElement(*this, port);

	Element const* eqn = symbol_dispatcher["Eqn"];
	assert(eqn);
	maInsertEqn = new MouseActionNewElement(*this, eqn);

	maInsertElement = new MouseActionNewElement(*this);

	maActivate = new MouseActionActivate(*this);
	maMirrorXaxis = new MouseActionMirrorXaxis(*this);
	maMirrorYaxis = new MouseActionMirrorYaxis(*this);
	maRotate = new MouseActionRotate(*this);
	maEditPaste = new MouseActionPaste(*this);

	// this was in App previously, and scattered across a couple of pointer hacks.
	// possibly initialised to "select". recheck.
	_maCurrent = maSelect;

    //selElem  = 0;  // no component/diagram is selected
    //focusElement.clear(); //element being interacted with mouse

    // ...............................................................
    // initialize menu appearing by right mouse button click on component
    ComponentMenu = new QMenu();
    // focusMEvent   = new QMouseEvent(QEvent::MouseButtonPress, QPoint(0,0),
    //                 Qt::NoButton, Qt::NoButton, Qt::NoModifier);

}
/*--------------------------------------------------------------------------*/
SchematicActions::~SchematicActions()
{itested();
	delete maActivate;
	delete maDelete;
	delete maInsertGround;
	delete maInsertPort;
	delete maMirrorXaxis;
	delete maMirrorYaxis;
	delete maRotate;
	delete maSelect;
	delete maWire;
	delete maZoomIn;

    delete ComponentMenu;
  //  delete focusMEvent;
}
/*--------------------------------------------------------------------------*/
const SchematicScene* SchematicActions::scene() const
{ untested();
}
/*--------------------------------------------------------------------------*/
SchematicDoc* SchematicActions::doc()
{ untested();
    auto d=dynamic_cast<SchematicDoc*>(&MouseActionsHandler::doc());
	assert(d);
	return d;
}
/*--------------------------------------------------------------------------*/
void SchematicActions::updateViewport()
{ untested();
	doc()->updateViewport();	
}

QPoint SchematicActions::snapToGrid(QPointF const&p) const{
    return scene()->snapToGrid(p);
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

QRegExp Expr_CompProp;
QRegExpValidator Val_CompProp(Expr_CompProp, 0);

// forward to mouseActions... TODO rearrange.
void SchematicDoc::actionInsertGround(QAction* sender)
{ untested();
    auto actions = dynamic_cast<SchematicActions*>(mouseActions());
    if (!actions)
        return;
    activateAction(actions->maInsertGround, sender);
}

void SchematicDoc::setDrawn(bool b){
    auto actions = dynamic_cast<SchematicActions*>(mouseActions());
    if (actions)
        actions->setDrawn(b);
}
/*--------------------------------------------------------------------------*/
void SchematicDoc::actionInsertEquation(QAction* sender)
{
    auto actions = dynamic_cast<SchematicActions*>(mouseActions());
    assert(actions);
    activateAction(actions->maInsertEqn, sender);
}
/*--------------------------------------------------------------------------*/
void SchematicDoc::actionInsertPort(QAction* sender)
{ untested();
    auto actions = dynamic_cast<SchematicActions*>(mouseActions());
    assert(actions);
  activateAction(actions->maInsertPort, sender);
}
/*--------------------------------------------------------------------------*/
void SchematicDoc::actionSelect(QAction* sender)
{itested();
  // sender is a button. maSelect is an action. connect the two.
  // this looks a bit redundant (but later...)
    auto actions = dynamic_cast<SchematicActions*>(mouseActions());
    assert(actions);
  activateAction(actions->maSelect, sender);

}
/*--------------------------------------------------------------------------*/
void SchematicDoc::actionOnGrid(QAction* sender)
{ untested();
    auto actions = dynamic_cast<SchematicActions*>(mouseActions());
    assert(actions);
  activateAction(actions->maOnGrid, sender);
//  performToggleAction(on, App->onGrid, &SchematicDoc::elementsOnGrid,
//		&MouseActionsHandler::MMoveOnGrid, &MouseActionsHandler::MPressOnGrid);
}

void SchematicDoc::actionEditRotate(QAction* sender)
{ untested();
    auto actions = dynamic_cast<SchematicActions*>(mouseActions());
    assert(actions);
  activateAction(actions->maRotate, sender);
//  performToggleAction(on, App->editRotate, &SchematicDoc::rotateElements,
//		&MouseActionsHandler::MMoveRotate, &MouseActionsHandler::MPressRotate);
}

void SchematicDoc::actionEditMirrorX(QAction* sender)
{ untested();
    auto actions = dynamic_cast<SchematicActions*>(mouseActions());
    assert(actions);
  activateAction(actions->maMirrorYaxis, sender);
//  performToggleAction(on, App->editMirror, &SchematicDoc::mirrorXComponents,
//		&MouseActionsHandler::MMoveMirrorX, &MouseActionsHandler::MPressMirrorX);
}

void SchematicDoc::actionEditMirrorY(QAction* sender)
{ untested();
    auto actions = dynamic_cast<SchematicActions*>(mouseActions());
    assert(actions);
  activateAction(actions->maMirrorXaxis, sender);
//  performToggleAction(on, App->editMirrorY, &SchematicDoc::mirrorYComponents,
//		&MouseActionsHandler::MMoveMirrorY, &MouseActionsHandler::MPressMirrorY);
}

void SchematicDoc::actionEditActivate(QAction* sender)
{ untested();
    auto actions = dynamic_cast<SchematicActions*>(mouseActions());
    assert(actions);
  activateAction(actions->maActivate, sender);
}

void SchematicDoc::actionEditDelete(QAction* sender)
{ untested();
    auto actions = dynamic_cast<SchematicActions*>(mouseActions());
    assert(actions);
  activateAction(actions->maDelete, sender);

  updateViewport();
//  assert(mouseActions());
//  mouseActions()->setDrawn(false);
}

void SchematicDoc::actionSetWire(QAction* sender)
{itested();
    auto actions = dynamic_cast<SchematicActions*>(mouseActions());
    assert(actions);
  activateAction(actions->maWire, sender);
}

void SchematicDoc::actionInsertLabel(QAction* sender)
{ untested();
//    auto actions = dynamic_cast<SchematicActions*>(mouseActions());
//    assert(actions);
//  activateAction(actions->maInsertLabel, sender);
  incomplete();
//  performToggleAction(on, App->insLabel, 0,
//		&MouseActionsHandler::MMoveLabel, &MouseActionsHandler::MPressLabel);
  // mouseAction = mouseActions().maInsLabel;
}

void SchematicDoc::actionSetMarker(QAction* sender)
{ untested();
  incomplete();
//  performToggleAction(on, App->setMarker, 0,
//		&MouseActionsHandler::MMoveMarker, &MouseActionsHandler::MPressMarker);
  // mouseAction = mouseActions().maSetMarker;
}

void SchematicDoc::actionMoveText(QAction*)
{ untested();
  incomplete();
//  performToggleAction(on, App->moveText, 0,
//		&MouseActionsHandler::MMoveMoveTextB, &MouseActionsHandler::MPressMoveText);
  // mouseAction = mouseActions().maMoveText;
}

void SchematicDoc::actionZoomIn(QAction* sender)
{ untested();
    auto actions = dynamic_cast<SchematicActions*>(mouseActions());
    assert(actions);
  activateAction(actions->maZoomIn, sender);
}

#if 0 // obsolete.
void SchematicDoc::actionInsertEquation(QAction* sender)
{ untested();
  App->hideEdit(); // disable text edit of component property
  App->MouseReleaseAction = 0;
  App->MouseDoubleClickAction = 0;

  if(!on) { untested();
    App->MouseMoveAction = 0;
    App->MousePressAction = 0;
    App->activeAction = 0;   // no action active
    return;
  }
  if(App->activeAction) { untested();
    App->activeAction->blockSignals(true); // do not call toggle slot
    App->activeAction->setChecked(false);       // set last toolbar button off
    App->activeAction->blockSignals(false);
  }
  App->activeAction = App->insEquation;

  if(mouseActions().hasElem()){ untested();
    incomplete(); // undo??
    delete mouseActions().getElem();  // delete previously selected component
  }else{ untested();
  }

  Symbol* sym=symbol_dispatcher.clone("Eqn");
  assert(sym);
  mouseActions().setElem(prechecked_cast<Component*>(sym));
  assert(mouseActions().hasElem());

  if(mouseActions().wasDrawn()){ untested();
    updateViewport();
  }else{ untested();
  }
  mouseActions().setDrawn(false);
  App->MouseMoveAction = &MouseActionsHandler::MMoveElement;
  App->MousePressAction = &MouseActionsHandler::MPressElement;
}
#endif

void SchematicDoc::actionEditPaste(QAction* sender)
{ untested();
  //activateAction(schematicActions().maEditPaste, sender);
#if 0
	// if it's not a text doc, prevent the user from editing
	// while we perform the paste operation
	App->hideEdit();

	if(!on) { untested();
		App->MouseMoveAction = 0;
		App->MousePressAction = 0;
		App->MouseReleaseAction = 0;
		App->MouseDoubleClickAction = 0;
		App->activeAction = 0;   // no action active
		if(mouseActions().wasDrawn()) { untested();
			viewport()->update();
		}
		return;
	}else{ untested();
        }

	if(!mouseActions()->pasteElements(this)) { untested();
		App->editPaste->blockSignals(true); // do not call toggle slot
		App->editPaste->setChecked(false);       // set toolbar button off
		App->editPaste->blockSignals(false);
		return;   // if clipboard empty
	}else{ untested();
        }

	uncheckActive();
	App->activeAction = App->editPaste;

        mouseActions().setDrawn(false)
    App->MouseMoveAction = &MouseActionsHandler::MMovePaste;
	mouseActions()->movingRotated = 0;
	App->MousePressAction = 0;
	App->MouseReleaseAction = 0;
	App->MouseDoubleClickAction = 0;

#endif
}

void SchematicDoc::actionSelectElement(QObject* sender)
{ untested();
  auto actions = dynamic_cast<SchematicActions*>(mouseActions());
  assert(actions);
  actions->maInsertElement->activate(sender);
  activateAction(actions->maInsertElement, nullptr);
}

// is this still in use?
void SchematicDoc::actionEditUndo(QAction* sender)
{itested();
  // really?
	assert(_app);
  _app->hideEdit(); // disable text edit of component property

  updateViewport();
  assert(mouseActions());
  setDrawn(false);
}

// is this still in use?
void SchematicDoc::actionEditRedo(QAction* sender)
{ untested();
	assert(_app);
  _app->hideEdit(); // disable text edit of component property

  updateViewport();
  setDrawn(false);
}

void SchematicDoc::actionAlign(int what)
{ untested();
	assert(_app);
  _app->hideEdit(); // disable text edit of component property

  if(!aligning(what)){ untested();
    QMessageBox::information(this, tr("Info"),
		      tr("At least two elements must be selected !"));
  }
  updateViewport();
  assert(mouseActions());
  setDrawn(false);
}

void SchematicDoc::actionDistrib(int d)
{ untested();
  assert(_app);
  _app->hideEdit(); // disable text edit of component property

  if (d==0){ untested();
	  distributeHorizontal();
  }else if(d==1){ untested();
	  distributeVertical();
  }else{ untested();
	  unreachable();
  }
  updateViewport();
  setDrawn(false);
}

void SchematicDoc::actionSelectAll(QAction*)
{ untested();
	incomplete();
    // selectElements(INT_MIN, INT_MIN, INT_MAX, INT_MAX, true);
    updateViewport();
}

void SchematicDoc::actionSelectMarker()
{ untested();
	assert(_app);
  _app->hideEdit(); // disable text edit of component property

  selectMarkers();
  updateViewport();
  setDrawn(false);
}

void SchematicDoc::actionChangeProps(QAction* sender)
{ untested();
	ChangeDialog *d = new ChangeDialog(this);
	if(d->exec() == QDialog::Accepted) { untested();
		setChanged(true, true);
		updateViewport();
	}
}

void SchematicDoc::actionCursor(arrow_dir_t)
{ untested();
	incomplete();
#ifdef USE_SCROLLVIEW
	int sign = 1;

	if(dir==arr_left){ untested();
		sign = -1;
	}

	if(App->editText->isHidden()) { untested();
		// for edit of component property ?
		auto movingElements = cropSelectedElements();
		int markerCount=0;
		for(auto const& i : movingElements){ untested();
			if(marker(i)){ untested();
				++markerCount;
			}
		}

		if((movingElements.count() - markerCount) < 1) { // all selections are markers
			incomplete();
#if 0
			if(markerCount > 0) {  // only move marker if nothing else selected
				markerMove(dir, &movingElements);
			} else if(dir==arr_up) { untested();
				// nothing selected at all
				if(scrollUp(verticalScrollBar()->singleStep()))
					scrollBy(0, -verticalScrollBar()->singleStep());
			} else if(dir==arr_down) { untested();
				if(scrollDown(-verticalScrollBar()->singleStep()))
					scrollBy(0, verticalScrollBar()->singleStep());
			} else if(dir==arr_left) { untested();
				if(scrollLeft(horizontalScrollBar()->singleStep()))
					scrollBy(-horizontalScrollBar()->singleStep(), 0);
			} else if(dir==arr_right) { untested();
				if(scrollRight(-horizontalScrollBar()->singleStep()))
					scrollBy(horizontalScrollBar()->singleStep(), 0);
			}else{ untested();
				// unreachable. TODO: switch
			}

			updateViewport();
			mouseActions()->drawn = false;
#endif
		}else if(dir==arr_up || dir==arr_down){ untested();
			// some random selection, put it back
			mouseActions()->moveElements(movingElements, 0, ((dir==arr_up)?-1:1) * GridY);
			mouseActions()->MAx3 = 1;  // sign for moved elements
			mouseActions()->endElementMoving(this, &movingElements);
		}else if(dir==arr_left || dir==arr_right){ untested();
			mouseActions()->moveElements(movingElements, sign*GridX, 0);
			mouseActions()->MAx3 = 1;  // sign for moved elements
			mouseActions()->endElementMoving(this, &movingElements);
		}else{ untested();
			//unreachable(); //TODO: switch.
		}

	}else if(dir==arr_up){ // BUG: redirect.
		if(mouseActions()->MAx3 == 0) return;  // edit component namen ?
		Component *pc = component(mouseActions()->focusElement);
		Property *pp = pc->Props.at(mouseActions()->MAx3-1);  // current property
		int Begin = pp->Description.indexOf('[');
		if(Begin < 0) return;  // no selection list ?
		int End = pp->Description.indexOf(App->editText->text(), Begin); // current
		if(End < 0) return;  // should never happen
		End = pp->Description.lastIndexOf(',', End);
		if(End < Begin) return;  // was first item ?
		End--;
		int Pos = pp->Description.lastIndexOf(',', End);
		if(Pos < Begin) Pos = Begin;   // is first item ?
		Pos++;
		if(pp->Description.at(Pos) == ' ') Pos++; // remove leading space
		App->editText->setText(pp->Description.mid(Pos, End-Pos+1));
		App->editText->selectAll();
	}else if(dir==arr_down) { // BUG: redirect.
		if(mouseActions()->MAx3 == 0) return;  // edit component namen ?
		Component *pc = component(mouseActions()->focusElement);
		Property *pp = pc->Props.at(mouseActions()->MAx3-1);  // current property
		int Pos = pp->Description.indexOf('[');
		if(Pos < 0) return;  // no selection list ?
		Pos = pp->Description.indexOf(App->editText->text(), Pos); // current list item
		if(Pos < 0) return;  // should never happen
		Pos = pp->Description.indexOf(',', Pos);
		if(Pos < 0) return;  // was last item ?
		Pos++;
		if(pp->Description.at(Pos) == ' ') Pos++; // remove leading space
		int End = pp->Description.indexOf(',', Pos);
		if(End < 0) {  // is last item ?
			End = pp->Description.indexOf(']', Pos);
			if(End < 0) return;  // should never happen
		}
		App->editText->setText(pp->Description.mid(Pos, End-Pos));
		App->editText->selectAll();
	}else{ untested();

	}
#endif
} // actionCursor

void SchematicDoc::actionApplyCompText()
{ untested();
#if 0 // what is focusElement??
	auto Doc=this;
	auto editText=App->editText;

	QString s;
	QFont f = QucsSettings.font;
	f.setPointSizeF(Scale * float(f.pointSize()) );
	editText->setFont(f);

	Property  *pp = 0;
	Component *pc = component(mouseActions()->focusElement);
	if(!pc) return;  // should never happen
	mouseActions()->MAx1 = pc->cx_() + pc->tx;
	mouseActions()->MAy1 = pc->cy_() + pc->ty;

	int z, n=0;  // "n" is number of property on screen
	pp = pc->Props.first();
	for(z=mouseActions()->MAx3; z>0; z--) {  // calculate "n"
		if(!pp) {  // should never happen
			App->hideEdit();
			return;
		}
		if(pp->display) n++;   // is visible ?
		pp = pc->Props.next();
	}

	pp = 0;
	if(mouseActions()->MAx3 > 0)  pp = pc->Props.at(mouseActions()->MAx3-1); // current property
	else s = pc->name();

	if(!editText->isHidden()) {   // is called the first time ?
		// no -> apply value to current property
		if(mouseActions()->MAx3 == 0) {   // component name ?
			if(!editText->text().isEmpty())
				if(pc->name() != editText->text()) { untested();
					auto pc2=Doc->find_component( editText->text());
					if(!pc2) { untested();
						pc->obsolete_name_override_hack( editText->text() );
						setChanged(true, true);  // only one undo state
					}
				}
		}
		else if(pp) {  // property was applied
			if(pp->Value != editText->text()) { untested();
				pp->Value = editText->text();
				recreateSymbol(pc);  // because of "Num" and schematic symbol
				setChanged(true, true); // only one undo state
			}
		}

		n++;     // next row on screen
		(mouseActions()->MAx3)++;  // next property
		pp = pc->Props.at(mouseActions()->MAx3-1);  // search for next property

		viewport()->update();
                mouseActions().setDrawn(false);

		if(!pp) {     // was already last property ?
			App->hideEdit();
			return;
		}


		while(!pp->display) {  // search for next visible property
			(mouseActions()->MAx3)++;  // next property
			pp = pc->Props.next();
			if(!pp) {     // was already last property ?
				App->hideEdit();
				return;
			}
		}
	}

	// avoid seeing the property text behind the line edit
	if(pp)  // Is it first property or component name ?
		s = pp->Value;
	editText->setMinimumWidth(editText->fontMetrics().width(s)+4);

  incomplete();
  /// Doc->contentsToViewport(int(Doc->Scale * float(view->MAx1 - Doc->ViewX1)),
  ///			 int(Doc->Scale * float(view->MAy1 - Doc->ViewY1)),
  ///	 view->MAx2, view->MAy2);
	editText->setReadOnly(false);
	if(pp) {  // is it a property ?
		s = pp->Value;
		mouseActions()->MAx2 += editText->fontMetrics().width(pp->Name+"=");
		if(pp->Description.indexOf('[') >= 0)  // is selection list ?
			editText->setReadOnly(true);
		Expr_CompProp.setPattern("[^\"]*");
		if(!pc->showName) n--;
	}
	else   // it is the component name
		Expr_CompProp.setPattern("[\\w_]+");
	Val_CompProp.setRegExp(Expr_CompProp);
	editText->setValidator(&Val_CompProp);

	z = editText->fontMetrics().lineSpacing();
	mouseActions()->MAy2 += n*z;
	editText->setText(s);
	misc::setWidgetBackgroundColor(editText, QucsSettings.BGColor);
	editText->setFocus();
	editText->selectAll();
	// make QLineEdit editable on mouse click
	QPoint p = QPoint(mouseActions()->MAx2, mouseActions()->MAy2);
	editText->setParent(Doc->viewport());
	App->editText->setGeometry(p.x(), p.y(), App->editText->width(), App->editText->height());
	App->editText->show();
#endif
}

// -----------------------------------------------------------
#if 0
/**
 * @brief MouseActionsHandler::MMoveWire2 Paint wire as it is being drawn with mouse.
 * @param Doc
 * @param Event
 */
void MouseActionsHandler::MMoveWire2(SchematicDoc* doc, QEvent* e)
{ untested();
  Set2(Event, Doc);
  Doc->setOnGrid(MAx2, MAy2);
  /// \todo paint aim
  /*paintAim(Doc,MAx2,MAy2); //let we paint aim cross

  //because cross slightly masks a wire, let we make wire thicker
  //better to make it by increasing of pen, but here we cannot access
  //pen
  if(MAx1 == 0) { untested();
    paintGhostLineV(Doc,MAx3,MAy3,MAy2);
    paintGhostLineH(Doc,MAx3,MAy2,MAx2);
  }
  else { untested();
    paintGhostLineH(Doc,MAx3,MAy3,MAx2);
    paintGhostLineV(Doc,MAx2,MAy3,MAy2);
  }
  */
  QucsMain->MouseDoubleClickAction = &MouseActionsHandler::MDoubleClickWire2;
  Doc->viewport()->update();
}

void MouseActionsHandler::Set1(QMouseEvent* Event, SchematicDoc*)
{
  SchematicDoc* s = prechecked_cast<SchematicDoc*>(&_doc);
  assert(s);
  QPointF pos=s->mapToScene(Event->pos());
  MAx1 = pos.x();
  MAy1 = pos.y();
}

void MouseActionsHandler::Set2(QMouseEvent* Event, SchematicDoc*)
{
  SchematicDoc* s = prechecked_cast<SchematicDoc*>(&_doc);
  assert(s);
  QPointF pos=s->mapToScene(Event->pos());
  MAx2 = pos.x();
  MAy2 = pos.y();
}

void MouseActionsHandler::Set3(QMouseEvent* Event, SchematicDoc*)
{
  SchematicDoc* s = prechecked_cast<SchematicDoc*>(&_doc);
  assert(s);
  QPointF pos=s->mapToScene(Event->pos());
  MAx3 = pos.x();
  MAy3 = pos.y();
}
#endif

#if 0
/**
 * @brief MouseActionsHandler::MMoveWire1 Paint hair cross for "insert wire" mode
 * @param Doc
 * @param Event
 */
void MouseActionsHandler::MMoveWire1(SchematicDoc* doc, QEvent* e)
{ untested();
  Set3(Event);
  Doc->setOnGrid(MAx3, MAy3);
  /// \todo paint aim
  //paintAim(Doc,MAx3,MAy3);
  //MAx2 = DOC_X_POS(Doc->contentsX()+Doc->viewport()->width()-1-2);
  //MAx2 = DOC_Y_POS(Doc->contentsY()+Doc->viewport()->height()-1-2);
  Doc->viewport()->update();
}
#endif



// -----------------------------------------------------------

void SchematicActions::setPainter(SchematicDoc* doc)
{
    Q_UNUSED(doc);
}

void SchematicActions::MMoveResizePainting(SchematicDoc* Doc, QEvent *)
{
  setPainter(Doc);
#if 0

  Set1(Event, Doc);
  Doc->setOnGrid(MAx1, MAy1);

  if(auto p=painting(focusElement)){ untested();
    p->MouseResizeMoving(MAx1, MAy1, Doc);
  }else{ untested();
    // why not always?
    // e->MouseResizeMoving(MAx1, MAy1, Doc);
  }
#endif
}

// -----------------------------------------------------------
#if 0 // obsolete.
// Moves components by keeping the mouse button pressed.
void SchematicActions::MMoveMoving(SchematicDoc* doc, QEvent* e)
{
  unreachable(); // obsolete
  setPainter(Doc);

  Set2(Event, Doc);

  Doc->setOnGrid(MAx2, MAy2);
  MAx3 = MAx1 = MAx2 - MAx1;
  MAy3 = MAy1 = MAy2 - MAy1;

  // assert(movingElements.empty); //?
  movingElements.clear();
  movingElements=Doc->cropSelectedElements(); // obsolete.
  Doc->viewport()->repaint();

//  currenttaskElement = new MoveElementsCommand(selectedItems, startposition);

  // Changes the position of all moving elements by dx/dy
  for(auto pe : movingElements) { untested();
    if(auto pw=wire(pe)){
      // connecting wires are not moved completely
    incomplete();

#if 0
      if(((uintptr_t)pw->portValue(0)) > 3) { pw->x1__() += MAx1;  pw->y1__() += MAy1; }
      else {  if((uintptr_t)(pw->portValue(0)) & 1) { pw->x1__() += MAx1; }
              if((uintptr_t)(pw->portValue(0)) & 2) { pw->y1__() += MAy1; } }

      if(((uintptr_t)pw->portValue(1)) > 3) { pw->x2__() += MAx1;  pw->y2__() += MAy1; }
      else {  if((uintptr_t)(pw->portValue(1)) & 1) pw->x2__() += MAx1;
              if((uintptr_t)(pw->portValue(1)) & 2) pw->y2__() += MAy1; }

      if(pw->Label) {
    // root of node label must lie on wire
        if(pw->Label->cx_() < pw->x1_()) pw->Label->cx__() = pw->x1_();
        if(pw->Label->cy_() < pw->y1_()) pw->Label->cy__() = pw->y1_();
        if(pw->Label->cx_() > pw->x2_()) pw->Label->cx__() = pw->x2_();
        if(pw->Label->cy_() > pw->y2_()) pw->Label->cy__() = pw->y2_();
      }else{ untested();
      }
#endif

    }else{ untested();
      assert(!pw);
      pe->setPosition(MAx1, MAy1, true);
    }

  pe->paintScheme(Doc);
  }

  setDrawn();
  MAx1 = MAx2;
  MAy1 = MAy2;
  QucsMain->MouseMoveAction = &SchematicActions::MMoveMoving2;
  QucsMain->MouseReleaseAction = &SchematicActions::MReleaseMoving;

}

// -----------------------------------------------------------
// Moves components by keeping the mouse button pressed.
void SchematicActions::MMoveMoving2(SchematicDoc* doc, QEvent* e)
{
  setPainter(Doc);

  Set2(Event, Doc);

#if 0
  if(drawn){
    // erase old scheme
    for(auto pe : movingElements) {
      pe->paintScheme(Doc);
//      if(pe->Type == isWire)  if(((Wire*)pe)->Label)
//        if(!((Wire*)pe)->Label->isSelected)
//          ((Wire*)pe)->Label->paintScheme(&painter);
    }
  }
#endif

  setDrawn();
  if (!Event->modifiers().testFlag(Qt::ControlModifier))
    Doc->setOnGrid(MAx2, MAy2);  // use grid only if CTRL key not pressed
  MAx1 = MAx2 - MAx1;
  MAy1 = MAy2 - MAy1;
  MAx3 += MAx1;  MAy3 += MAy1;   // keep track of the complete movement

  moveElements(movingElements, MAx1, MAy1);  // moves elements by MAx1/MAy1

  // paint afterwards to avoid conflict between wire and label painting
  for(auto pe : movingElements) {
    pe->paintScheme(Doc);
//    if(pe->Type == isWire)  if(((Wire*)pe)->Label)
//      if(!((Wire*)pe)->Label->isSelected)
//        ((Wire*)pe)->Label->paintScheme(&painter);
  }

  MAx1 = MAx2;
  MAy1 = MAy2;
}
#endif


/**
 * @brief SchematicActions::MMoveLabel Paints a label above the mouse cursor for "set wire label".
 * @param Doc
 * @param Event
 */
void SchematicActions::MMoveLabel(SchematicDoc *doc, QEvent *e)
{
#if 0
  Set3(Event, Doc);

  // paint marker
  Doc->PostPaintEvent (_Line, MAx3, MAy3, MAx3+10, MAy3-10);
  Doc->PostPaintEvent (_Line, MAx3+10, MAy3-10, MAx3+20, MAy3-10);
  Doc->PostPaintEvent (_Line, MAx3+10, MAy3-10, MAx3+10, MAy3-17);

  // paint A
  Doc->PostPaintEvent (_Line, MAx3+12, MAy3-12, MAx3+15, MAy3-23);
  Doc->PostPaintEvent (_Line, MAx3+14, MAy3-17, MAx3+17, MAy3-17);
  Doc->PostPaintEvent (_Line, MAx3+19, MAy3-12, MAx3+16, MAy3-23);
#endif
}


/**
 * @brief SchematicActions::MMoveMarker Paints a triangle above the mouse for "set marker on graph"
 * @param Doc
 * @param Event
 */
void SchematicActions::MMoveMarker(SchematicDoc* doc, QEvent* e)
{
#if 0
  Set3(Event, Doc);

  Doc->PostPaintEvent (_Line, MAx3, MAy3-2, MAx3-8, MAy3-10);
  Doc->PostPaintEvent (_Line, MAx3+1, MAy3-3, MAx3+8, MAy3-10);
  Doc->PostPaintEvent (_Line, MAx3-7, MAy3-10, MAx3+7, MAy3-10);
#endif
}


/**
 * @brief SchematicActions::MMoveMirrorX Paints rounded "mirror about y axis" mouse cursor
 * @param Doc
 * @param Event
 */
void SchematicActions::MMoveMirrorY(SchematicDoc* doc, QEvent* e)
{
#if 0
  Set3(Event, Doc);

  Doc->PostPaintEvent (_Line, MAx3-11, MAy3-4, MAx3-9, MAy3-9);
  Doc->PostPaintEvent (_Line, MAx3-11, MAy3-3, MAx3-6, MAy3-3);
  Doc->PostPaintEvent (_Line, MAx3+11, MAy3-4, MAx3+9, MAy3-9);
  Doc->PostPaintEvent (_Line, MAx3+11, MAy3-3, MAx3+6, MAy3-3);
  Doc->PostPaintEvent (_Arc, MAx3-10, MAy3-8, 21, 10, 16*20, 16*140,false);
#endif
}


/**
 * @brief SchematicActions::MMoveMirrorX Paints rounded "mirror about x axis" mouse cursor
 * @param Doc
 * @param Event
 */
void SchematicActions::MMoveMirrorX(SchematicDoc* doc, QEvent* e)
{
#if 0
  Set3(Event, Doc);

  Doc->PostPaintEvent (_Line, MAx3-4, MAy3-11, MAx3-9, MAy3-9);
  Doc->PostPaintEvent (_Line, MAx3-3, MAy3-11, MAx3-3, MAy3-6);
  Doc->PostPaintEvent (_Line, MAx3-4, MAy3+11, MAx3-9, MAy3+9);
  Doc->PostPaintEvent (_Line, MAx3-3, MAy3+11, MAx3-3, MAy3+6);
  Doc->PostPaintEvent (_Arc, MAx3-8, MAy3-10, 10, 21, 16*110, 16*140,false);
#endif
}

/**
 * @brief SchematicActions::MMoveMirrorX Paints "rotate" mouse cursor
 * @param Doc
 * @param Event
 */
void SchematicActions::MMoveRotate(SchematicDoc *, QEvent *)
{
#if 0
  Set3(Event, Doc);

  Doc->PostPaintEvent (_Line, MAx3-6, MAy3+8, MAx3-6, MAy3+1);
  Doc->PostPaintEvent (_Line, MAx3-7, MAy3+8, MAx3-12, MAy3+8);
  Doc->PostPaintEvent (_Arc, MAx3-10, MAy3-10, 21, 21, -16*20, 16*240,false);
#endif
}

/**
 * @brief SchematicActions::MMoveActivate Paints a crossed box mouse cursor to "(de)activate" components.
 * @param Doc
 * @param Event
 */
void SchematicActions::MMoveActivate(SchematicDoc* doc, QEvent* e)
{
#if 0
  Set3(Event, Doc);

  Doc->PostPaintEvent (_Rect, MAx3, MAy3-9, 14, 10);
  Doc->PostPaintEvent (_Line, MAx3, MAy3-9, MAx3+13, MAy3);
  Doc->PostPaintEvent (_Line, MAx3, MAy3, MAx3+13, MAy3-9);
#endif
}


/**
 * @brief SchematicActions::MMoveOnGrid Paints a grid beside the mouse cursor, put "on grid" mode.
 * @param Event
 * @param Doc
 */
void SchematicActions::MMoveOnGrid(SchematicDoc* doc, QEvent* e)
{
#if 0
  Set3(Event, Doc);

  Doc->PostPaintEvent (_Line, MAx3+10, MAy3+ 3, MAx3+25, MAy3+3);
  Doc->PostPaintEvent (_Line, MAx3+10, MAy3+ 7, MAx3+25, MAy3+7);
  Doc->PostPaintEvent (_Line, MAx3+10, MAy3+11, MAx3+25, MAy3+11);
  Doc->PostPaintEvent (_Line, MAx3+13, MAy3, MAx3+13, MAy3+15);
  Doc->PostPaintEvent (_Line, MAx3+17, MAy3, MAx3+17, MAy3+15);
  Doc->PostPaintEvent (_Line, MAx3+21, MAy3, MAx3+21, MAy3+15);
#endif
}


/**
 * @brief SchematicActions::MMoveMoveTextB Paints mouse symbol for "move component text" mode.
 * @param Doc
 * @param Event
 */
void SchematicActions::MMoveMoveTextB(SchematicDoc* doc, QEvent* e)
{
#if 0
  Set3(Event, Doc);

  Doc->PostPaintEvent (_Line, MAx3+14, MAy3   , MAx3+16, MAy3);
  Doc->PostPaintEvent (_Line, MAx3+23, MAy3   , MAx3+25, MAy3);
  Doc->PostPaintEvent (_Line, MAx3+13, MAy3   , MAx3+13, MAy3+ 3);
  Doc->PostPaintEvent (_Line, MAx3+13, MAy3+ 7, MAx3+13, MAy3+10);
  Doc->PostPaintEvent (_Line, MAx3+14, MAy3+10, MAx3+16, MAy3+10);
  Doc->PostPaintEvent (_Line, MAx3+23, MAy3+10, MAx3+25, MAy3+10);
  Doc->PostPaintEvent (_Line, MAx3+26, MAy3   , MAx3+26, MAy3+ 3);
  Doc->PostPaintEvent (_Line, MAx3+26, MAy3+ 7, MAx3+26, MAy3+10);
#endif
}


/**
 * @brief SchematicActions::MMoveMoveText Paint rectangle around component text being mouse moved
 * @param Doc
 * @param Event
 */
void SchematicActions::MMoveMoveText(SchematicDoc* doc, QEvent* e)
{
#if 0
  QPointF pos=Doc->mapToScene(Event->pos());
  int newX=pos.x();
  int newY=pos.y();

  MAx1 += newX - MAx3;
  MAy1 += newY - MAy3;
  MAx3  = newX;
  MAy3  = newY;

  Doc->PostPaintEvent (_Rect, MAx1, MAy1, MAx2, MAy2);
#endif
}


/**
 * @brief SchematicActions::MMoveZoomIn Paints symbol beside the mouse to show the "Zoom in" modus.
 * @param Doc
 * @param Event
 */
void SchematicActions::MMoveZoomIn(SchematicDoc* doc, QEvent* e)
{
#if 0 // TODO
  Set3(Event, Doc);

  Doc->PostPaintEvent (_Line, MAx3+14, MAy3   , MAx3+22, MAy3);
  Doc->PostPaintEvent (_Line, MAx3+18, MAy3-4 , MAx3+18, MAy3+4);
  Doc->PostPaintEvent (_Ellipse, MAx3+12, MAy3-6, 13, 13,0,0,false);
  Doc->viewport()->update();
#endif
}


// ************************************************************************
// **********                                                    **********
// **********    Functions for serving mouse button clicking     **********
// **********                                                    **********
// ************************************************************************

#if 0
// Is called from several MousePress functions to show right button menu.
void SchematicActions::rightPressMenu(QMouseEvent *Event)
{ untested();
  Set1(Event);

  SchematicDoc* Doc = prechecked_cast<SchematicDoc*>(&_doc);
  assert(Doc);
  focusElement = selectElement(Event->pos(), false);

  if(focusElement){  // remove special function (4 least significant bits)
    incomplete();
    // what's this??
    // focusElement->Type &= isSpecialMask;
  }else{ untested();
  }


  // define menu
  ComponentMenu->clear();
  { untested();
    if(focusElement) { untested();
      focusElement.setSelected(true);
      QAction *editProp = new QAction(QObject::tr("Edit Properties"), QucsMain);
      QucsMain->connect(editProp, SIGNAL(triggered()), SLOT(slotEditElement()));
      ComponentMenu->addAction(editProp);

      if(!component(focusElement)){ untested();
      }else if(!QucsMain->moveText->isChecked()){ untested();
    ComponentMenu->addAction(QucsMain->moveText);
      }else{ untested();
      }
    }else{ untested();
      ComponentMenu->addAction(QucsMain->symEdit);
      ComponentMenu->addAction(QucsMain->fileSettings);

      if(!QucsMain->moveText->isChecked()){ untested();
    ComponentMenu->addAction(QucsMain->moveText);
      }else{ untested();
      }
    }
  }

  // possibly, we just want to call focuselement->somethign_menu() ?!
  while(true) { untested();
    if(focusElement){ untested();
      if(graph(focusElement)) break;
    }else{ untested();
    }
    if(!QucsMain->onGrid->isChecked())
      ComponentMenu->addAction(QucsMain->onGrid);
    ComponentMenu->addAction(QucsMain->editCopy);
    if(!QucsMain->editPaste->isChecked())
      ComponentMenu->addAction(QucsMain->editPaste);
    break;
  }

  // possibly, we just want to call focuselement->rightMenuAction() ?!
  { untested();
    if (focusElement) { untested();
      if (diagram(focusElement)) { untested();
        QAction *imgExport = new QAction(QObject::tr("Export as image"), QucsMain);
        QucsMain->connect(imgExport, SIGNAL(triggered()), SLOT(slotSaveDiagramToGraphicsFile()));
        ComponentMenu->addAction(imgExport);
      }else{ untested();
      }
    }
  }

  if(!QucsMain->editDelete->isChecked()){ untested();
    // still ElementGraphics->rightMenuAction?!
    ComponentMenu->addAction(QucsMain->editDelete);
  }else{
  }

  if(!focusElement){ untested();
  }else if(auto m=marker(focusElement)) { untested();
    ComponentMenu->addSeparator();
    QString s = QObject::tr("power matching");
    if(m->pGraph->Var == "Sopt" ){ // BUG
      s = QObject::tr("noise matching");
    }

    QAction *powerMatch = new QAction(s, QucsMain);
    QucsMain->connect(powerMatch, SIGNAL(triggered()), SLOT(slotPowerMatching()));
    ComponentMenu->addAction(powerMatch);
    if(m->pGraph->Var.left(2) == "S[" ) { // BUG
      QAction *power2Match = new QAction(QObject::tr("2-port matching"), QucsMain);
      QucsMain->connect(power2Match, SIGNAL(triggered()), SLOT(slot2PortMatching()));
      ComponentMenu->addAction(power2Match);
    }else{ untested();
    }
  }
  do { untested();
    if(!focusElement) { untested();
    }else if(diagram(focusElement)){ untested();
      break;
    }else if(graph(focusElement)){ untested();
        ComponentMenu->addAction(QucsMain->graph2csv);
        break;
    }else{ untested();
    }
    ComponentMenu->addSeparator();
    if(focusElement) if(component(focusElement))
      if(!QucsMain->editActivate->isChecked())
        ComponentMenu->addAction(QucsMain->editActivate);
    if(!QucsMain->editRotate->isChecked())
      ComponentMenu->addAction(QucsMain->editRotate);
    if(!QucsMain->editMirror->isChecked())
      ComponentMenu->addAction(QucsMain->editMirror);
    if(!QucsMain->editMirrorY->isChecked())
      ComponentMenu->addAction(QucsMain->editMirrorY);

    // right-click menu to go into hierarchy
    if(focusElement) { untested();
      if(auto c=component(focusElement)){ untested();
      if(c->obsolete_model_hack() != "Sub"){ untested();
      }else if(!QucsMain->intoH->isChecked())
    // BUG: c->addAction.. later.
    ComponentMenu->addAction(QucsMain->intoH);
      }
    }
    // right-click menu to pop out of hierarchy
    if(!focusElement)
      if(!QucsMain->popH->isChecked())
        ComponentMenu->addAction(QucsMain->popH);
  } while(false);

  *focusMEvent = *Event;  // remember event for "edit component" action
  ComponentMenu->popup(Event->globalPos());
  updateViewport();
  setDrawn(false);
}
#endif

// -----------------------------------------------------------
void SchematicActions::MPressLabel(SchematicDoc* doc, QEvent* e)
{ untested();
#if 0
  QPointF pos=Doc->mapToScene(Event->pos());
  float fX=pos.x();
  float fY=pos.y();

  int x = int(fX), y = int(fY);
  Wire *pw = 0;
  WireLabel *pl=0;
  Node *pn = Doc->selectedNode(x, y);
  if(!pn) { untested();
    pw = Doc->selectedWire(x, y);
    if(!pw) return;
  }

  QString Name, Value;
  Element *pe=0;
  // is wire line already labeled ?
#if 0
  if(pw){
    pe = Doc->getWireLabel(pw->portValue(0));
  }else{
    pe = Doc->getWireLabel(pn);
  }
#endif
  if(pe) { untested();
    if(pe->Type & isComponent) { untested();
      QMessageBox::information(0, QObject::tr("Info"),
                 QObject::tr("The ground potential cannot be labeled!"));
      return;
    }
    pl = ((Conductor*)pe)->Label;
  }

  LabelDialog *Dia = new LabelDialog(pl, Doc);
  if(Dia->exec() == 0) return;

  Name  = Dia->NodeName->text();
  Value = Dia->InitValue->text();
  delete Dia;

  if(Name.isEmpty() && Value.isEmpty() ) { // if nothing entered, delete name
    if(pe) { untested();
      if(((Conductor*)pe)->Label)
        delete ((Conductor*)pe)->Label; // delete old name
      ((Conductor*)pe)->Label = 0;
    }
    else { untested();
      if(pw) pw->setName("", "");   // delete name of wire
      else pn->setName("", "");
    }
  }
  else { untested();
/*    Name.replace(' ', '_');	// label must not contain spaces
    while(Name.at(0) == '_') Name.remove(0,1);  // must not start with '_'
    if(Name.isEmpty()) return;
*/
    if(pe) { untested();
      if(((Conductor*)pe)->Label)
        delete ((Conductor*)pe)->Label; // delete old name
      ((Conductor*)pe)->Label = 0;
    }

    int xl = x+30;
    int yl = y-30;
    Doc->setOnGrid(xl, yl);
    // set new name
    if(pw) pw->setName(Name, Value, x-pw->x1_() + y-pw->y1_(), xl, yl);
    else pn->setName(Name, Value, xl, yl);
  }

  Doc->sizeOfAll(Doc->UsedX1, Doc->UsedY1, Doc->UsedX2, Doc->UsedY2);
  updateViewport();
  setDrawn(false);
  Doc->setChanged(true, true);
#endif
}

// -----------------------------------------------------------
// yikes.

// -----------------------------------------------------------

// -----------------------------------------------------------
#if 0
void SchematicActions::MPressActivate(SchematicDoc* doc, QEvent* e)
{ untested();
  QPointF pos=Doc->mapToScene(Event->pos());
  float fX=pos.x();
  float fY=pos.y();

  MAx1 = int(fX);
  MAy1 = int(fY);
  if(!Doc->activateSpecifiedComponent(MAx1, MAy1)) { untested();
//    if(Event->button() != Qt::LeftButton) return;
    MAx2 = 0;  // if not clicking on a component => open a rectangle
    MAy2 = 0;
    QucsMain->MousePressAction = 0;
    QucsMain->MouseReleaseAction = &SchematicActions::MReleaseActivate;
    QucsMain->MouseMoveAction = &SchematicActions::MMoveSelect;
  }
  updateViewport();
  setDrawn(false);
}
#endif

// -----------------------------------------------------------
#if 0
// insert component, diagram, painting into schematic ?!
void SchematicActions::MPressElement(SchematicDoc* doc, QEvent* e)
{
  // QPointF pos=Doc->mapToScene(Event->pos());

  if(selElem == 0) return;

  int x1, y1, x2, y2, rot;
  if(selElem->Type & isComponent) { untested();
    Component *Comp = (Component*)selElem;
//    qDebug() << "+-+ got to switch:" << Comp->name();
    QString entryName = Comp->name();

    const Component* cComp = Comp;
    switch(Event->button()) {
      case Qt::LeftButton :
    // left mouse button inserts component into the schematic
    // give the component a pointer to the schematic it's a
    // part of
    Comp->textSize(x1, y1);
    Doc->insertElement(Comp);
//	assert(&Doc->DocModel == &cComp->getScope());
    Comp->textSize(x2, y2);
    if(Comp->tx < Comp->x1_()) Comp->tx -= x2 - x1;

    // Note: insertCopmponents does increment  name1 -> name2
//    qDebug() << "  +-+ got to insert:" << Comp->name();

    // enlarge viewarea if component lies outside the view
    Comp->entireBounds(x1,y1,x2,y2, Doc->textCorr());
    Doc->enlargeView(x1, y1, x2, y2);

    setDrawn(false);
    updateViewport();
    Doc->setChanged(true, true);
    rot = Comp->rotated;

    // handle static and dynamic components
//    QucsApp::CompChoose;
    if (Module::vaComponents.contains(entryName)){ untested();
      QString filename = Module::vaComponents[entryName];
//      qDebug() << "   ===+ recast";
      incomplete(); // myust be Comp->pressElement (or so).
      //Comp = dynamic_cast<vacomponent*>(Comp)->newOne(filename); //va component
      qDebug() << "   => recast = Comp;" << Comp->name() << "filename: " << filename;
    }else{
      // static component is used, so create a new one
      Comp = prechecked_cast<Component*>(Comp->clone());
    }
    rot -= Comp->rotated;
    rot &= 3;
    while(rot--) Comp->rotate(); // keep last rotation for single component
    break;

      case Qt::RightButton :  // right mouse button rotates the component
    if(Comp->Ports.count() == 0)
      break;  // do not rotate components without ports
    Comp->paintScheme(Doc); // erase old component scheme
    Comp->rotate();
    Comp->paintScheme(Doc); // paint new component scheme
    break;

      default: ;   // avoids compiler warnings
    }
//    qDebug() << "   => selElem = Comp;" << Comp->name();
    // comp it geting empty
    selElem = Comp;
    return;

  }  // of "if(isComponent)"
  else if(auto d=diagram(selElem)) { untested();

    d->pressElement(Doc, element(selElem), Event);

    Doc->paintings().append((Painting*)selElem);
    ((Painting*)selElem)->Bounding(x1,y1,x2,y2);
    //Doc->enlargeView(x1, y1, x2, y2);
    selElem = prechecked_cast<Element*>(selElem->clone());
    assert(selElem);

    Doc->viewport()->update();
    Doc->setChanged(true, true);

    MMoveElement(Doc, Event);  // needed before next mouse pressing
    setDrawn(false);
  }else{
  }
}
#endif



// -----------------------------------------------------------
// Is called for setting a marker on a diagram's graph
void SchematicActions::MPressMarker(SchematicDoc* doc, QEvent* e)
{ untested();
#if 0
  QPointF pos=Doc->mapToScene(Event->pos());
  float fX=pos.x();
  float fY=pos.y();

  MAx1 = int(fX);
  MAy1 = int(fY);
  Marker *pm = Doc->setMarker(MAx1, MAy1);

  if(pm) { untested();
    assert(pm->diag());
    int x0 = pm->diag()->cx();
    int y0 = pm->diag()->cy();
    incomplete();
//    Doc->enlargeView(x0+pm->x1_(), y0-pm->y1_()-pm->y2_(), x0+pm->x1_()+pm->x2_(), y0-pm->y1_());
  }
  updateViewport();
  setDrawn(false);
#endif
}

// -----------------------------------------------------------
void SchematicActions::MPressOnGrid(SchematicDoc* doc, QEvent* e)
{
  //QPointF pos=Doc->mapToScene(Event->pos());
#if 0

  auto pe = selectElement(Event->pos(), false);
  if(pe)
  { untested();
    pe->Type &= isSpecialMask;  // remove special functions (4 lowest bits)

    // onGrid is toggle action -> no other element can be selected
    pe->setSelected();
    Doc->elementsOnGrid();

    Doc->sizeOfAll(Doc->UsedX1, Doc->UsedY1, Doc->UsedX2, Doc->UsedY2);
    // Update matching wire label highlighting
    Doc->highlightWireLabels ();
    Doc->viewport()->update();
    setDrawn(false);
  }

#endif
}

// -----------------------------------------------------------
void SchematicActions::MPressMoveText(SchematicDoc* doc, QEvent* e)
{ untested();
#if 0
  QPointF pos=Doc->mapToScene(Event->pos());
  float fX=pos.x();
  float fY=pos.y();

  MAx1 = int(fX);
  MAy1 = int(fY);

#ifndef USE_SCROLLVIEW
  incomplete();
#else
  Component* c=selectCompText(Doc, MAx1, MAy1, MAx2, MAy2);
  focusElement = ElementMouseAction(c);

  auto C=component(focusElement);

  if(focusElement) { untested();
    MAx3 = MAx1;
    MAy3 = MAy1;
    MAx1 = C->cx_() + C->tx;
    MAy1 = C->cy_() + C->ty;
    Doc->viewport()->update();
    setDrawn(false);
    QucsMain->MouseMoveAction = &SchematicActions::MMoveMoveText;
    QucsMain->MouseReleaseAction = &SchematicActions::MReleaseMoveText;
    Doc->grabKeyboard();  // no keyboard inputs during move actions
  }
#endif
#endif
}

// -----------------------------------------------------------
void SchematicActions::MPressZoomIn(SchematicDoc* doc, QEvent* e )
{ untested();
  unreachable();
#if 0
  QPointF pos=Doc->mapToScene(Event->pos());
  float fX=pos.x();
  float fY=pos.y();

  qDebug() << "zoom into box";
  MAx1 = int(fX);
  MAy1 = int(fY);
  MAx2 = 0;  // rectangle size
  MAy2 = 0;

  QucsMain->MouseMoveAction = &SchematicActions::MMoveSelect;
  QucsMain->MouseReleaseAction = &SchematicActions::MReleaseZoomIn;
  Doc->grabKeyboard();  // no keyboard inputs during move actions
  Doc->viewport()->update();
  setDrawn(false);
#endif
}


// ***********************************************************************
// **********                                                   **********
// **********    Functions for serving mouse button releasing   **********
// **********                                                   **********
// ***********************************************************************
void SchematicActions::MReleaseSelect(SchematicDoc* doc, QEvent* e)
{ untested();
  incomplete(); // obsolete
}

// -----------------------------------------------------------
void SchematicActions::MReleaseActivate(SchematicDoc* doc, QEvent* e)
{ untested();
#if 0
  if(Event->button() != Qt::LeftButton) return;

  // activates all components within the rectangle
  Doc->activateCompsWithinRect(MAx1, MAy1, MAx1+MAx2, MAy1+MAy2);

  QucsMain->MouseMoveAction = &SchematicActions::MMoveActivate;
  QucsMain->MousePressAction = &SchematicActions::MPressActivate;
  QucsMain->MouseReleaseAction = 0;
  QucsMain->MouseDoubleClickAction = 0;
  Doc->highlightWireLabels ();
  Doc->viewport()->update();
  setDrawn(false);
#endif
}

// -----------------------------------------------------------
// Is called after moving selected elements.
void SchematicActions::MReleaseMoving(SchematicDoc* doc, QEvent* e)
{ untested();
  // Allow all mouse buttons, because for others than the left one,
  // a menu has already created.
  // endElementMoving(Doc, &movingElements);
  doc->releaseKeyboard();  // allow keyboard inputs again

#if 0
  QucsMain->MouseMoveAction = 0;
  QucsMain->MousePressAction = &SchematicActions::MPressSelect;
  QucsMain->MouseReleaseAction = &SchematicActions::MReleaseSelect;
  QucsMain->MouseDoubleClickAction = &SchematicActions::MDoubleClickSelect;
#else
  // MouseAction = actionSelect;
#endif
}

// -----------------------------------------------------------
#if 0
void SchematicActions::MReleaseResizeDiagram(SchematicDoc* doc, QEvent* e)
{ untested();
  unreachable();
  if(Event->button() != Qt::LeftButton){ untested();
    return;
  }
  Diagram* d=diagram(focusElement);
  if(!d){ untested();
    assert(false && "we are in trouble here");
    return;
  }

  MAx3  = d->cx__();
  MAy3  = d->cy__();
  if(MAx2 < 0) {    // resize diagram
    if(MAx2 > -10) MAx2 = -10;   // not smaller than 10 pixels
    d->x2__() = -MAx2;
    d->cx__() = MAx1+MAx2;
  }
  else { untested();
    if(MAx2 < 10) MAx2 = 10;
    d->x2__() = MAx2;
    d->cx__() = MAx1;
  }
  if(MAy2 < 0) { untested();
    if(MAy2 > -10) MAy2 = -10;
    d->y2__() = -MAy2;
    d->cy__() = MAy1;
  }
  else { untested();
    if(MAy2 < 10) MAy2 = 10;
    d->y2__() = MAy2;
    d->cy__() = MAy1+MAy2;
  }
  MAx3 -= focusElement->cx_();
  MAy3 -= focusElement->cy_();

  Diagram *pd = d;
  pd->updateGraphData();
  foreach(Graph *pg, pd->Graphs)
    foreach(Marker *pm, pg->Markers) { untested();
      pm->x1__() += MAx3;      // correct changes due to move of diagram corner
      pm->y1__() += MAy3;
    }

  int x1, x2, y1, y2;
  pd->Bounding(x1, x2, y1, y2);
  Doc->enlargeView(x1, x2, y1, y2);

#if 0
  QucsMain->MouseMoveAction = 0;
  QucsMain->MousePressAction = &SchematicActions::MPressSelect;
  QucsMain->MouseReleaseAction = &SchematicActions::MReleaseSelect;
  QucsMain->MouseDoubleClickAction = &SchematicActions::MDoubleClickSelect;
#else
  // MouseAction = actionSelect;
#endif
  Doc->releaseKeyboard();  // allow keyboard inputs again

  Doc->viewport()->update();
  setDrawn(false);
  Doc->setChanged(true, true);
}
#endif

// -----------------------------------------------------------
void SchematicActions::MReleaseResizePainting(SchematicDoc* doc, QEvent* e)
{ untested();
  auto m = dynamic_cast<QGraphicsSceneMouseEvent*>(e);
  if (!m)
      return;

  if(m->button() != Qt::LeftButton) return;

#if 0
  QucsMain->MouseMoveAction = 0;
  QucsMain->MousePressAction = &SchematicActions::MPressSelect;
  QucsMain->MouseReleaseAction = &SchematicActions::MReleaseSelect;
  QucsMain->MouseDoubleClickAction = &SchematicActions::MDoubleClickSelect;
#else
  // MouseAction = actionSelect;
#endif


  doc->releaseKeyboard();  // allow keyboard inputs again

  doc->viewport()->update();
  setDrawn(false);
  doc->setChanged(true, true);
}

// -----------------------------------------------------------
#if 0
void SchematicActions::moveElements(SchematicDoc *Doc, int& x1, int& y1)
{ untested();
  unreachable(); // obsolete
  Doc->setOnGrid(x1, y1);

  for(auto pe : movingElements) {
    auto L=dynamic_cast<WireLabel*>(pe);
    if(pe->Type & isLabel) { untested();
      assert(L);
      L->cx__() += x1;  L->x1__() += x1;
      L->cy__() += y1;  L->y1__() += y1;
    }else{ untested();
      assert(!L);
      pe->setPosition(x1, y1, true);
    }
  }
}
#endif

// -----------------------------------------------------------
#if 0
void SchematicActions::rotateElements(SchematicDoc *Doc, int& x1, int& y1)
{ untested();
  (void)Doc;
  incomplete();
  int x2, y2;
  Doc->setOnGrid(x1, y1);

  for(auto pe=movingElements.first(); pe != 0; pe = movingElements.next()) { untested();
    auto W=dynamic_cast<Wire*>(pe);
    switch(pe->Type) {
    case isComponent:
    case isAnalogComponent:
    case isDigitalComponent:
      ((Component*)pe)->rotate(); // rotate !before! rotating the center
      x2 = x1 - pe->cx_();
      pe->setPosition(pe->cy_() - y1 + x1, x2 + y1);
      break;
    case isWire:
      x2     = pe->x1_();
      W->x1__() = pe->y1_() - y1 + x1;
      W->y1__() = x1 - x2 + y1;
      x2     = pe->x2_();
      W->x2__() = pe->y2_() - y1 + x1;
      W->y2__() = x1 - x2 + y1;
      break;
    case isPainting:
      ((Painting*)pe)->rotate(); // rotate !before! rotating the center
      ((Painting*)pe)->getPosition(x2, y2);
      pe->setPosition(y2 - y1 + x1, x1 - x2 + y1);
          break;
    default:
      x2 = x1 - pe->cx_();   // if diagram -> only rotate cx/cy
      pe->setPosition(pe->cy_() - y1 + x1, x2 + y1);
      break;
    }
  }
}
#endif

// -----------------------------------------------------------
#if 1
void SchematicActions::MReleasePaste(SchematicDoc* doc, QEvent* e)
{ untested();
#if 0
  int x1, y1, x2, y2, rot;
  QFileInfo Info(Doc->docName());
  //QPainter painter(Doc->viewport());
  QPointF pos=Doc->mapToScene(Event->pos());

  switch(Event->button()) {
  case Qt::LeftButton :
    // insert all moved elements into document
    for(auto pe : movingElements){
      pe->setSelected(false);
      switch(pe->Type) {
    case isWire:
      if(pe->x1_() == pe->x2_()){ untested();
        if(pe->y1_() == pe->y2_())  break;
      }
      Doc->insertWire((Wire*)pe);
      if (Doc->wires().containsRef ((Wire*)pe))
        Doc->enlargeView(pe->x1_(), pe->y1_(), pe->x2_(), pe->y2_());
      else pe = NULL;
      break;
    case isDiagram:
      Doc->pushBack((Diagram*)pe);
      ((Diagram*)pe)->loadGraphData(Info.path() + QDir::separator() +
                    Doc->DataSet);
      Doc->enlargeView(pe->cx_(), pe->cy_()-pe->y2_(), pe->cx_()+pe->x2_(), pe->cy_());
      break;
    case isPainting:
      Doc->paintings().append((Painting*)pe);
      ((Painting*)pe)->Bounding(x1,y1,x2,y2);
      Doc->enlargeView(x1, y1, x2, y2);
      break;
    case isMovingLabel:
      incomplete();
//	  pe->Type = isNodeLabel;
      Doc->placeNodeLabel((WireLabel*)pe);
      break;
//	case isComponent:
//	case isAnalogComponent:
//	case isDigitalComponent:
    default:
      incomplete();
      }
      if(Component* C=dynamic_cast<Component*>(pe)){
    // legacy component
    Doc->insertElement(C);
    C->entireBounds(x1,y1,x2,y2, Doc->textCorr());
    Doc->enlargeView(x1, y1, x2, y2);
      }else if(Symbol* S=dynamic_cast<Symbol*>(pe)){ untested();
    Doc->insertElement(S);
    incomplete();
    // S->entireBounds(x1,y1,x2,y2, Doc->textCorr());
    Doc->enlargeView(x1, y1, x2, y2);
      }
    }

    pasteElements(Doc);
    // keep rotation sticky for pasted elements
    // rot = movingRotated;
    x1 = y1 = 0;
    while(rot--) { incomplete();
      // rotateElements(Doc,x1,y1);
    }

    QucsMain->MouseMoveAction = &SchematicActions::MMovePaste;
    QucsMain->MousePressAction = 0;
    QucsMain->MouseReleaseAction = 0;
    QucsMain->MouseDoubleClickAction = 0;

    setDrawn(false);
    Doc->viewport()->update();
    Doc->setChanged(true, true);
    break;

  // ............................................................
  case Qt::RightButton :  // right button rotates the elements

    if(wasDrawn()){
      // erase old scheme
      // paintElementsScheme(Doc);
    }else{
    }
    setDrawn();

    x1 = pos.x();
    y1 = pos.y();
    // rotateElements(Doc,x1,y1);
    // paintElementsScheme(Doc);
    // save rotation
    movingRotated++;
    movingRotated &= 3;
    break;

  default: ;    // avoids compiler warnings
  }
#endif
}
#endif

// -----------------------------------------------------------
#if 0
void SchematicActions::MReleaseMoveText(SchematicDoc* doc, QEvent* e)
{ untested();
  if(Event->button() != Qt::LeftButton) return;

  QucsMain->MouseMoveAction = &SchematicActions::MMoveMoveTextB;
  QucsMain->MouseReleaseAction = 0;
  Doc->releaseKeyboard();  // allow keyboard inputs again

  auto c=component(focusElement);
  assert(c);

  c->tx = MAx1 - c->cx();
  c->ty = MAy1 - c->cy();
  Doc->viewport()->update();
  setDrawn(false);
  Doc->setChanged(true, true);
}
#endif

// -----------------------------------------------------------
void SchematicActions::MReleaseZoomIn(SchematicDoc* doc, QEvent* e)
{ untested();
  unreachable();
  return;
}


// ***********************************************************************
// **********                                                   **********
// **********    Functions for mouse button double clicking     **********
// **********                                                   **********
// ***********************************************************************
#if 0
void SchematicActions::editElement(SchematicDoc* doc, QEvent* e)
{ untested();
  assert(false); // obsolete;
//    qDebug() << "+double click, editElement";
//
// BUG: focusElement is a parameter
  if(focusElement == (Element*)nullptr){
    incomplete();
    return;
  }else{
  }

//  qDebug() << "+focusElement->Type" << focusElement->Type;

  int x1, y1, x2, y2;

  QFileInfo Info(Doc->docName());

  QPointF pos=Doc->mapToScene(Event->pos());
  float fX=pos.x();
  float fY=pos.y();

//  incomplete();
 // Element* E=element();
  ElementGraphics* EG=focusElement->element();
  Element* E=element(EG);
  E->createSchematicWidget(Doc);

  // BUG. move to respective classes. 1 at a time...
  if(auto c=component(focusElement)){ untested();
//         qDebug() << "cast focusElement into" << c->Name;
         bool done=false;
         if(c->obsolete_model_hack() == "GND") { // BUG
       return;
     }else if(c->obsolete_model_hack() == "SPICE") { // BUG. use cast
       incomplete();
           // SpiceDialog *sd = new SpiceDialog(App, (SpiceFile*)c, Doc);
           // if(sd->exec() != 1) done=true;   // dialog is WDestructiveClose
         } else if(c->obsolete_model_hack() == ".Opt") { // BUG again...
       incomplete();
           // OptimizeDialog *od = new OptimizeDialog((Optimize_Sim*)c, Doc);
           // if(od->exec() != 1) done=true;   // dialog is WDestructiveClose
         } else {
       incomplete();
#if 0
           ComponentDialog * cd = new ComponentDialog(c, Doc);
           if(cd->exec() != 1){ untested();
         done=true;   // dialog is WDestructiveClose
       }else{ untested();
         incomplete();
         Doc->Components->findRef(c);
         Doc->Components->take();
         Doc->setComponentNumber(c); // for ports/power sources
         Doc->Components->append(c);
       }
#endif
         }

     if(!done){ untested();
       Doc->setChanged(true, true);
       c->entireBounds(x1,y1,x2,y2, Doc->textCorr());
       Doc->enlargeView(x1,y1,x2,y2);
     }else{ untested();
     }
  }else if(auto dia=diagram(focusElement)){ untested();
         bool done=false;
         if(dia->name().at(0) == 'T') { // don't open dialog on scrollbar
           if(dia->name() == "Time") {
             if(dia->cy() < int(fY)) {
           if(dia->scroll(MAx1))
             Doc->setChanged(true, true, 'm'); // 'm' = only the first time
           done=true;
             }
       }else if(dia->cx() > int(fX)) { untested();
           if(dia->scroll(MAy1)){ untested();
             Doc->setChanged(true, true, 'm'); // 'm' = only the first time
           }
           done=true;
       }
     }

     if(!done){ untested();
       incomplete();
#if 0
       auto ddia=new DiagramDialog(dia, Doc);
       if(ddia->exec() != QDialog::Rejected)   // is WDestructiveClose
         Doc->setChanged(true, true);

       dia->Bounding(x1, x2, y1, y2);
       Doc->enlargeView(x1, x2, y1, y2);
#endif
     }else{ untested();
     }
  }else if(auto pg=graph(focusElement)){ untested();
     // searching diagram for this graph
     // BUG: a graph should know its parent.
     for(auto d : Doc->diagrams()){
       if(dia->Graphs.indexOf(pg) >= 0){
         dia=d;
         break;
       }
     }

     if(dia){ untested();

       incomplete();
#if 0
       ddia = new DiagramDialog(dia, Doc, pg);
       if(ddia->exec() != QDialog::Rejected)   // is WDestructiveClose
         Doc->setChanged(true, true);
#endif
     }else{ untested();
     }

  }else if(auto w=wire(focusElement)){
    (void) w;
         MPressLabel(Doc, Event);

  }else if(auto l=wireLabel(focusElement)){ untested();
         editLabel(Doc, l);
         // update highlighting, labels may have changed
         Doc->highlightWireLabels ();
  }else if(auto p=painting(focusElement)){ untested();

         if( p->Dialog() )
           Doc->setChanged(true, true);

  }else if(auto m=marker(focusElement)){
    (void) m;
    incomplete();
        // mdia = new MarkerDialog(m, Doc);
        // if(mdia->exec() > 1)
        //   Doc->setChanged(true, true);
  }else{
  }

  // Very strange: Now an open VHDL editor gets all the keyboard input !?!
  // I don't know why it only happens here, nor am I sure whether it only
  // happens here. Anyway, I hope the best and give the focus back to the
  // current document.
  Doc->setFocus();

  Doc->viewport()->update();
  setDrawn(false);
}
#endif

// -----------------------------------------------------------
#if 0
void SchematicActions::MDoubleClickSelect(SchematicDoc* doc, QEvent* e)
{
  qDebug() << "doubleclick";
  Doc->releaseKeyboard();  // allow keyboard inputs again
  QucsMain->editText->setHidden(true);
  editElement(Doc, Event);
}
#endif


/**
 * @brief SchematicActions::MDoubleClickWire2  Double click terminates wire insertion.
 * @param Doc
 * @param Event
 */
void SchematicActions::MDoubleClickWire2(SchematicDoc* doc, QEvent* e)
{
  incomplete();
  // MPressWire2(Doc, Event);
#if 0

  if(formerAction)
    QucsMain->select->setChecked(true);  // restore old action
  else { untested();
//    QucsMain->MouseMoveAction = &SchematicActions::MMoveWire1;
//    QucsMain->MousePressAction = &SchematicActions::MPressWire1;
    QucsMain->MouseDoubleClickAction = 0;
  }
#endif
}

#if 0
void ElementMouseAction::setSelected(bool x)
{ untested();
  if(_e){
    _e->setSelected(x);
  }else{
    unreachable();
  }
}

bool ElementMouseAction::isSelected() const
{
  assert(_e);
  return _e->isSelected();
}
#endif

// -----------------------------------------------------------
bool SchematicActions::pasteElements(SchematicDoc *)
{ untested();
  assert(false);
  QClipboard *cb = QApplication::clipboard();   // get system clipboard
  QString s = cb->text(QClipboard::Clipboard);
//  DocumentStream stream(&s, QIODevice::ReadOnly);
#if 0
  movingElements.clear();
  if(!Doc->paste(&stream, &movingElements)){
    // something went wrong during parse.
    // throw?!
    return false;
  }else{
  }

  int xmax, xmin, ymax, ymin;
  xmin = ymin = INT_MAX;
  xmax = ymax = INT_MIN;
  // First, get the max and min coordinates of all selected elements.
  for(auto pe : movingElements){ untested();
    if(pe->Type == isWire) { untested();
      if(pe->x1_() < xmin) xmin = pe->x1_();
      if(pe->x2_() > xmax) xmax = pe->x2_();
      if(pe->y1_() < ymin) ymin = pe->y1_();
      if(pe->y2_() > ymax) ymax = pe->y2_();
    }
    else { untested();
      if(pe->cx_() < xmin) xmin = pe->cx_();
      if(pe->cx_() > xmax) xmax = pe->cx_();
      if(pe->cy_() < ymin) ymin = pe->cy_();
      if(pe->cy_() > ymax) ymax = pe->cy_();
    }
  }

  xmin = -((xmax+xmin) >> 1);   // calculate midpoint
  ymin = -((ymax+ymin) >> 1);
  Doc->setOnGrid(xmin, ymin);

  // moving with mouse cursor in the midpoint
  for(auto pe : movingElements){ untested();
    if(pe->Type & isLabel) { untested();
      auto L=dynamic_cast<WireLabel*>(pe);
      //pe->cx += xmin;  pe->x1 += xmin;
      //pe->cy += ymin;  pe->y1 += ymin;
      // TODO.
      auto oldtype=L->Type;
      L->Type = isMovingLabel;
      L->setPosition(xmin, ymin, true /*relative*/);
      L->Type = oldtype;
    } else{
      pe->setPos(xmin, ymin, true);
    }
  }

#endif
  return true;
}

// -----------------------------------------------------------
// TODO: graphical input texts
void SchematicActions::editLabel(SchematicDoc *, WireLabel *)
{ untested();
  unreachable();
#if 0
  LabelDialog *Dia = new LabelDialog(pl, Doc);
  int Result = Dia->exec();
  if(Result == 0) return;

  QString Name  = Dia->NodeName->text();
  QString Value = Dia->InitValue->text();
  delete Dia;

  if(Name.isEmpty() && Value.isEmpty()) { // if nothing entered, delete label
    pl->pOwner->Label = 0;   // delete name of wire
    delete pl;
  } else { untested();
/*    Name.replace(' ', '_');	// label must not contain spaces
    while(Name.at(0) == '_') Name.remove(0,1);  // must not start with '_'
    if(Name.isEmpty()) return;
    if(Name == pl->Name) return;*/
    if(Result == 1) return;  // nothing changed

    int old_x2 = pl->x2_();
    pl->setName(Name);   // set new name
    pl->initValue = Value;
    if(pl->cx() > (pl->x1_()+(pl->x2_()>>1))){ untested();

      // pl->x1_() -= pl->x2_() - old_x2; // don't change position due to text width
      pl->moveLeft(pl->x2_() - old_x2);
    }
  }

  Doc->sizeOfAll(Doc->UsedX1, Doc->UsedY1, Doc->UsedX2, Doc->UsedY2);
  updateViewport();
  setDrawn(false);
  Doc->setChanged(true, true);
#endif
}

// -----------------------------------------------------------
// Reinserts all elements (moved by the user) back into the schematic.
// but why?!
#if 0
void SchematicActions::endElementMoving(SchematicDoc *Doc, EGPList *movElements)
{ untested();
  unreachable(); // obsolete
  for(auto pe : *movElements){ untested();
//    pe->setSelected(false);  // deselect first (maybe afterwards pe == NULL)
    if(wire(pe)){ untested();
        if(pe->x1_() == pe->x2_()){ untested();
          if(pe->y1_() == pe->y2_()) { untested();
            // Delete wires with zero length, but preserve label.
#if 0
            if(((Wire*)pe)->Label) { untested();
              Doc->insertNodeLabel((WireLabel*)((Wire*)pe)->Label);
              ((Wire*)pe)->Label = 0;
            }else{
        }
#endif
            delete (Wire*)pe;
            break;
          }
    }else{
    }

    // Doc->insertWire((Wire*)pe); wtf?
    break; // why?
    }else if(diagram(pe)){ untested();
    //already there. d'uh
    }else if(painting(pe)){ untested();
    Doc->paintings().append((Painting*)pe);
    }else if(auto c=component(pe)){ untested();
      qDebug() << "type" << pe->Type << c->name();
    Doc->insertRawComponent(c, false);
//      Doc->connect(c); // TODO.
    }else if(auto w=wireLabel(pe)){ untested();
    Doc->insertNodeLabel(w);
    }else if(marker(pe)){ untested();
      //?
    }else{ untested();
    }
  }

  movElements->clear();
  if((MAx3 != 0) || (MAy3 != 0))  // moved or put at the same place ?
    Doc->setChanged(true, true);

  // enlarge viewarea if components lie outside the view
  Doc->sizeOfAll(Doc->UsedX1, Doc->UsedY1, Doc->UsedX2, Doc->UsedY2);
  Doc->enlargeView(Doc->UsedX1, Doc->UsedY1, Doc->UsedX2, Doc->UsedY2);
  updateViewport();
  setDrawn(false);
}
#endif

// -----------------------------------------------------------
// Moves elements in "movElements" by x/y
#if 0
void SchematicActions::moveElements(EGPList& , int, int)
{ itested();
  unreachable(); // obsolete
  auto movElements=&what;
  Wire *pw;
  (void)pw;
  for(auto pe : *movElements) { itested();
    if(pe->Type == isWire) { untested();
      pw = (Wire*)pe;   // connected wires are not moved completely

      incomplete();
      if(((uintptr_t)pw->portValue(0)) > 3) { // wtf?
    pw->move1(x, y);
#if 0
    if(pw->Label) { untested();
      pw->Label->moveCenter(x, y);
    }else{ untested();
    }
#endif
      }else{ untested();
        if((uintptr_t)(pw->portValue(0)) & 1) { pw->move1(x,0); }
    if((uintptr_t)(pw->portValue(0)) & 2) { pw->move1(0,y); }
      }

      if(((uintptr_t)pw->portValue(1)) > 3) { pw->move2(x, y);
      }else{ untested();
    if((uintptr_t)(pw->portValue(1)) & 1) { pw->move2(x,0); }
    if((uintptr_t)(pw->portValue(1)) & 2) { pw->move2(0,y); }
      }

#if 0
      if(pw->Label) { untested();
    // root of node label must lie on wire
        if(pw->Label->cx_() < pw->x1_()) pw->Label->moveTo(pw->x1_(), pw->Label->cy_());
        if(pw->Label->cy_() < pw->y1_()) pw->Label->moveTo(pw->Label->cx_(), pw->y1_());
        if(pw->Label->cx_() > pw->x2_()) pw->Label->moveTo(pw->x2_(), pw->Label->cy_());
        if(pw->Label->cy_() > pw->y2_()) pw->Label->moveTo(pw->Label->cx_(), pw->y2_());
      }else{ untested();
      }
#endif

    } else{
      pe->setPosition(x, y, true);
    }
  }
}
#endif

// ***********************************************************************
// **********                                                   **********
// **********       Functions for serving mouse moving          **********
// **********                                                   **********
// ***********************************************************************
#if 0 // obsolete?
/*!
 * \brief SchematicActions::MMoveElement
 * \param Doc
 * \param Event
 *
 * Event handler, an Element selected for insertion is moved
 * on the View.
 *
 * Set the mouse decoration/cursor to represent the selElem.
 * Motion snaps to grid points.
 *
 * selElem is an Element object, see QucsApp::slotSelectComponent and
 * other similar slots.
 *
 * When mouse moves over the scene, the Element must be added and
 * updated acordingly.
 */
void SchematicActions::MMoveElement(SchematicDoc* doc, QEvent* e)
{ untested();
  if(selElem == 0) return;

//  qDebug() << "MMoveElement got selElem";

  int x  = Event->pos().x();
  int y  = Event->pos().y();
  (void) (x+y);

  QPointF pos=Doc->mapToScene(Event->pos());
  int fx = pos.x();
  int fy = pos.y();
  int gx = fx;
  int gy = fy;
  Doc->setOnGrid(gx, gy);


  //QPainter painter(Doc->viewport());
  if(auto P=painting(selElem)) { untested();
    Doc->PostPaintEvent (_NotRop, 0,0,0,0);
    incomplete();
#if 0
    x -= Doc->contentsX();
    y -= Doc->contentsY();
#endif
    incomplete();
    (void)P;
//    P->MouseMoving(Doc, x, y, gx, gy, Doc, x, y, drawn);
    setDrawn();
    updateViewport();
  }else{

    // ********** it is a component or diagram
    if(_drawn){
      // erase old scheme
      selElem->paintScheme(Doc);
    }else{
      setDrawn();
    }

  //  Component *comp = (Component*)selElem;
    //qDebug() << "desc" << comp->Description << "gx" << gx << "gy" << gy;

    selElem->setPosition(gx, gy);
    selElem->paintScheme(Doc); // paint scheme at new position
    Doc->viewport()->update();
  }
}
#endif

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
