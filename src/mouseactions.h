/***************************************************************************
    copyright            : (C) 2003 by Michael Margraf
                               2020 Felix Salfelder
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MOUSEACTIONS_H
#define MOUSEACTIONS_H
#include "qt_compat.h"
#include <QEvent>
#include <QAction>

class Label;
class MouseActionsHandler;
class QAction;
class QMenu;
class QMouseEvent;
class QPainter;
class QUndoCommand;
class Schematic;

// something happens to the mouse on a schematic
// BUG: wrong file. schematic_mouse.h maybe?
// FIXME: why Mouse? User??
/*!
 * \brief The MouseAction class
 * This is the base MouseAction class. Every mouse action is derived from this one.
 * This class implements default actions and stores some information
 *
 */
class MouseAction {
public:
	typedef QUndoCommand cmd;
protected:
public:
	explicit MouseAction(MouseActionsHandler& ctx)
		:_ctx(ctx), _sender(nullptr){}
	MouseAction(MouseAction const&) = delete;

public:
	virtual ~MouseAction(){}

public:
	/*!
	 * \brief handle
	 * Handles the current event and returns a new Undo command if event is handled in this function
	 * The Undo command must be freed when not used
	 * Override this function to handle different types
	 * \return
	 */
	virtual cmd* handle(QEvent*);

	virtual cmd* activate(QObject* sender);
	virtual cmd* deactivate();
// private: TODO
	// TODO: only use POS in those
	virtual cmd* move(QEvent*) { return nullptr; }
	virtual cmd* press(QEvent*) { return nullptr; }
	// virtual cmd* grab(QGraphicsSceneEvent*) { return nullptr; }
	virtual cmd* release(QEvent*) { return nullptr; }
	/*!
	 * \brief dblclk
	 * Double click
	 * \return
	 */
	virtual cmd* dblclk(QEvent*) { return nullptr; }

	virtual cmd* generic(QEvent*) { return nullptr; } // remove
	virtual cmd* enter(QEvent*) {itested(); return nullptr; }
	virtual cmd* leave(QEvent*) {itested(); return nullptr; }

	void uncheck();

protected:
        MouseActionsHandler& ctx() const {return _ctx;}

private:
	MouseActionsHandler& _ctx;
	QAction* _sender{nullptr};
};


// a mouse action on an element (first attempt)
// formerly, a mouse action was implemented by means of messing with element
// internals.
#if 0
class ElementMouseAction {
public:
	explicit ElementMouseAction(ElementGraphics* e)
		: _e(e)
	{
	}
public:
	ElementGraphics* element() { return _e; }
	ElementGraphics const* element() const { return _e; }

	void clear(){
		_e=nullptr;
	}
public: // compat with old code
	bool operator==(ElementGraphics const* e) const{
		return _e==e;
	}
	bool operator!=(ElementGraphics const* e) const{
		return _e!=e;
	}
	bool operator==(Element const* e) const{
		return ::element(_e)==e;
	}
	bool operator!=(Element const* e) const{
		return *_e!=e;
	}
	operator bool() const{
		return _e;
	}
	void setSelected(bool x=true);
	bool isSelected() const;

public: // access coordinates from old code.
	     // newer code might use Qt conventions...?
//	int const& cx_() const { assert(_e); return _e->cx_(); }
//	int const& cy_() const { assert(_e); return _e->cy_(); }
//	int const& x1_() const { assert(_e); return _e->x1_(); }
//	int const& y1_() const { assert(_e); return _e->y1_(); }
//	int const& x2_() const { assert(_e); return _e->x2_(); }
//	int const& y2_() const { assert(_e); return _e->y2_(); }
public:
	// don't use. just to compile
	int Type; // BUG BUG
#if 1
	ElementMouseAction* operator->(){
		unreachable(); // complain loudly.
		return this;
	}
#endif
	void setObsoleteType(int t){
		_action_type = t;
	}

private:
	int _action_type; // the legacy way.
	                  //  might need cleanup
	 ElementGraphics* _e{nullptr};
};

// enable access to attached elements.
// this might be temporary
inline ElementGraphics* element(ElementMouseAction e)
{
  return e.element();
}
inline Component* component(ElementMouseAction e)
{
  return component(element(e));
}
inline WireLabel* wireLabel(ElementMouseAction e)
{
  return wireLabel(element(e));
}
inline Diagram* diagram(ElementMouseAction e)
{
  return diagram(element(e));
}
inline Painting* painting(ElementMouseAction e)
{
  return painting(element(e));
}
inline Graph* graph(ElementMouseAction e)
{
  return graph(element(e));
}
inline Marker* marker(ElementMouseAction e)
{
  return marker(element(e));
}
inline Node* node(ElementMouseAction e)
{
  return node(element(e));
}
Label* label(ElementMouseAction e);
#endif


extern QAction *formerAction;

class Label;

#if 0
ElementGraphics* element(ElementMouseAction);
Component* component(ElementMouseAction);
WireLabel* wireLabel(ElementMouseAction);
Diagram* diagram(ElementMouseAction);
Painting* painting(ElementMouseAction);
Graph* graph(ElementMouseAction);
Marker* marker(ElementMouseAction);
Node* node(ElementMouseAction);
Label* label(ElementMouseAction);
WireLabel* wireLabel(ElementMouseAction);
#endif
#endif
