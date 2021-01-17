/***************************************************************************
    copyright            : (C) 2003 by Michael Margraf
                               2014 Guilherme Brondani Torri
                               2020 Felix Salfelder

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "qucs.h"
#include "node.h"
#include "schematic_doc.h"
#include "misc.h"
#include "mouseactions.h"
#include "module.h"
//#include "components/spicedialog.h"
//#include "components/spicefile.h"
//#include "components/optimizedialog.h"
#include "labeldialog.h"
#include "io_trace.h"
#include "schematic_scene.h"
#include "wirelabel.h"

#include <QTextStream>
#include "qt_compat.h"
#include <QMouseEvent>
#include <QClipboard>
#include <QApplication>
#include <QMessageBox>
#include <QMenu>
#include <QEvent>
#include <QAction>
#include <QLineEdit>
#include <QDebug>
#include <QUndoStack>

#include <limits.h>
#include <stdlib.h>


void MouseAction::sceneAddItem(ElementGraphics* x)
{
  doc().sceneAddItem(x);
}
void MouseAction::sceneRemoveItem(ElementGraphics* x)
{
  doc().sceneRemoveItem(x);
}


QAction *formerAction;   // remember action before drag n'drop etc.


/**
 * @brief draws wire aiming cross on Document view
 * @param Doc - pointer to Schematics object
 * @param fx  - document x-coordinate of center
 * @param fy  - document x-coordinate of center
 *
 *  \todo Move aim (crosshair) to separate class
 *
static void paintAim(SchematicDoc *Doc, int fx,int fy){ untested();
	//let we reserve couple of points at the edges of lines for some aesthetics,
	//and visual check that our calculations has fit the widget window.
	const int ldelta = 2;

	//left and upper edges of our lines
	int lx0 = DOC_X_POS(Doc->contentsX()+ldelta);
	int ly0 = DOC_Y_POS(Doc->contentsY()+ldelta);

	//right and bottom edges
	int lx1 = DOC_X_POS(Doc->contentsX()+Doc->viewport()->width()-1-ldelta);
	int ly1 = DOC_Y_POS(Doc->contentsY()+Doc->viewport()->height()-1-ldelta);

	//post line paint events
	Doc->PostPaintEvent (_Line, lx0, fy, lx1, fy);
	Doc->PostPaintEvent (_Line, fx, ly0, fx, ly1);
}

//paint ghost line - horizontal
static void paintGhostLineH(SchematicDoc *Doc, int fx,int fy, int fxx){ untested();
	Doc->PostPaintEvent (_Line, fx, fy-1, fxx, fy-1);
	Doc->PostPaintEvent (_Line, fx, fy+1, fxx, fy+1);
}

//paint ghost line - vertical
static void paintGhostLineV(SchematicDoc *Doc, int fx, int fy, int fyy){ untested();
	Doc->PostPaintEvent (_Line, fx-1, fy, fx-1, fyy);
	Doc->PostPaintEvent (_Line, fx+1, fy, fx+1, fyy);
}
*/



// -----------------------------------------------------------
QList<ElementGraphics*> MouseAction::selectedItems()
{
   // BUG. copies.
  unreachable();
  return doc().selectedItems();
}
#if 0
// Is called after the rectangle for selection is released.
void MouseActionsHandler::MReleaseSelect2(SchematicDoc *Doc, QMouseEvent *Event)
{
  unreachable();
  // obsolete. Qt Scene does rectangle select for us
  if(Event->button() != Qt::LeftButton){
    unreachable(); // yikes.
    return;
  }else{
  }

  bool Ctrl = Event->modifiers().testFlag(Qt::ControlModifier);

  // selects all elements within the rectangle
  Doc->selectElements(MAx1, MAy1, MAx1+MAx2, MAy1+MAy2, Ctrl);

  Doc->releaseKeyboard();  // allow keyboard inputs again
  QucsMain->MouseMoveAction = 0;
  QucsMain->MousePressAction = &MouseActionsHandler::MPressSelect;
  QucsMain->MouseReleaseAction = &MouseActionsHandler::MReleaseSelect;
  QucsMain->MouseDoubleClickAction = &MouseActionsHandler::MDoubleClickSelect;
  // MouseAction = actionSelect;
  Doc->highlightWireLabels ();
  Doc->viewport()->update();
  setDrawn(false);
}
#endif

void MouseAction::setCursor(QCursor const& c)
{
  doc().setCursor(c);
}

// vim:ts=8:sw=2:noet
