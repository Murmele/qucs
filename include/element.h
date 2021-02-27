/***************************************************************************
    copyright            : (C) 2003 by Michael Margraf
                               2018, 2020 Felix Salfelder
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/** \file element.h
  * \brief Defines drawing elements for schematics
  *
  * element.h contains definitions of various drawing elements used
  * used to render schematics and the schematic symbols. The following
  * structs are defined to hold information on various drawing types:
  *
  *    Line
  *    Arc
  *    Area
  *    Port
  *    Text
  *
  * The Element class is also defined here which is a superclass
  * of every component symbol.
  *
  */

// Element: Superclass of all schematic drawing elements

#ifndef QUCS_ELEMENT_H
#define QUCS_ELEMENT_H
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#include <assert.h>
#include "object.h"
#include "io_trace.h"
#include "port.h"
#include "geometry.h"
#include "qt_compat.h"

#include "QRect"

class Node;
class QucsDoc;
class QPainter;
class WireLabel;
class SchematicDoc;
class SchematicModel;
class Symbol;
class QDialog;


// valid values for Element.Type
// The 4 least significant bits of each value are reserved for special
// additionals !!!
#define isDummyElement    0
#define isSpecialMask    -16

#define isComponent        0x30000
#define isComponentText    0x30002
#define isAnalogComponent  0x10000
#define isDigitalComponent 0x20000

#define isGraph            0x0020
//#define isNode             0x0040
#define isMarker           0x0080
//#define isWire             0x0100

#if 0
#define isPainting         0x2000
#define isPaintingResize   0x2001

#define isLabel            0x4000
#define isMovingLabel      0x4001
#define isHMovingLabel     0x4002
#define isVMovingLabel     0x4004

#define isDiagram          0x8000
#define isDiagramResize    0x8001
#define isDiagramHScroll   0x8002
#define isDiagramVScroll   0x8003
#endif



class NetLang;
class ViewPainter;

static QString incomplete_description="incomplete_description";
// will be gone soon.
// class Component;
class Label;
class WireLabel;
class Diagram;
class Painting;
class Graph;
class Marker;
class Node;
class ViewPainter;
/*--------------------------------------------------------------------------*/
class SchematicModel;
/*--------------------------------------------------------------------------*/

/*!
 * \brief The Element class
 * Base object of all elements. This class handles the position ... TODO: what does this class exactly
 *
 * An element is an object in a schematic.
 * It has a position and a bounding rectangle
 */
class Element : public Object {
public:
	friend class ElementGraphics;
	typedef rect_t Rect;
protected:
	Element(Element const&);
public:
	Element();
	Element(int cx, int cy) : _position(cx, cy) { unreachable(); }
	virtual ~Element();

public: // make old variables accessible
        int const& cx() const { return _position.first; }
        int const& cy() const { return _position.second; }

	int const& x1_() const { return x1; }
	int const& y1_() const { return y1; }
	void snapToGrid(SchematicDoc& s);

	void setObsoleteType(int){
		unreachable();
	}
	virtual rect_t bounding_rect() const;

public: // other stuff
	virtual bool showLabel() const{ return true; }
	//virtual bool showParam(int i) const{ return true; } // later

        void setPosition(pos_t const& c);
	virtual void getPosition(int&, int&) const; // BUG
	virtual void paint(ViewPainter*) const = 0;
	virtual std::unique_ptr<QDialog> createSchematicWidget(QucsDoc*) const;

	// really?
	virtual QWidget* newWidget() {return nullptr;}

	pos_t position()const{
		return _position;
	}

public:
        /*!
         * \brief clone
         * Clones the Element. The undostack must not be cloned!
         * \return
         */
	virtual Element* clone()const = 0;
	virtual Element* clone_instance()const{
		return clone();
	}
	virtual QString const& description() const{return incomplete_description;}
	virtual char const* iconBasename() const{return nullptr;}

public:
	// BUG; abused in taskElement
	//  mutable int x2, y2;  // center and relative boundings

	// create a declaration, e.g. subcircuit definition or include directive
	virtual Symbol const* proto(SchematicModel const*) const{return nullptr;}
	SchematicModel const* scope() const;
	virtual SchematicModel* scope();

public: // friend ElementGraphics?
	//  void attachToModel();
	void detachFromModel();

public:
	Object const* owner() const{return _owner;}
	Object* mutable_owner() const{return _owner;}

protected:
	Object* owner(){ return _owner;}

public:
	void setOwner(Object* e) { assert(!_owner || e==_owner || !e); _owner=e;}
	const Element* find_looking_out(const std::string& name)const;
	const Element* find_in_parent_scope(const std::string& name)const;
	const Element* find_in_my_scope(const std::string& name)const;

private:
  Object* _owner{nullptr}; // should probably be const all the way
  pos_t _position{0, 0}; // Position of the element. The position is the center of the element and must lie on the grid

protected: // BUG in Painting
  int x1, y1; // Top left corner

private:
}; // Element
/*--------------------------------------------------------------------------*/
inline SchematicModel const* Element::scope() const
{
	auto e=const_cast<Element*>(this);
	return e->scope();
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#endif
