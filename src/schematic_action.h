/***************************************************************************
    copyright            : (C) 2020 Felix Salfelder
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUCS_SCHEMATIC_ACTION
#define QUCS_SCHEMATIC_ACTION
/*--------------------------------------------------------------------------*/
#include "mouseactionshandler.h"
#include "schematic_edit.h"
/*--------------------------------------------------------------------------*/
class SchematicDoc;
/*--------------------------------------------------------------------------*/
class SchematicActions : public MouseActionsHandler{
public:
	explicit SchematicActions(SchematicScene* d);
	~SchematicActions();

private: // MouseActions
	// void handle(QEvent*) override;

#if 0 //gone now?
private:
public: // BUG? called from MouseAction.
  void Set1(QMouseEvent*, SchematicDoc*ignore=nullptr);
  void Set2(QMouseEvent*, SchematicDoc*ignore=nullptr);
  void Set3(QMouseEvent*, SchematicDoc*ignore=nullptr);
public: // BUG
  int MAx1, MAy1, MAx2, MAy2;
  int MAx3, MAy3;
private:
  QList<ElementGraphics*> movingElements;
public:
  int movingRotated;
#endif

  // menu appearing by right mouse button click on component
  QMenu *ComponentMenu;

private:
  // former SchematicDoc::select*
  // but that does not work, because ElementMouseAction lives here.
  // (does it matter?)
public:
  // Component* selectCompText(SchematicDoc*, int, int, int&, int&);
//  void     deselectElements(ElementMouseAction);
public:
  // obsolete
//  void rightPressMenu(QMouseEvent*);

protected: // Doc stuff
	SchematicDoc* doc(); // TODO: scene!!
	SchematicScene const* scene()const;
	void updateViewport();
	QPoint snapToGrid(QPointF const&p) const;

public:
	void setDrawn(bool b=true){_drawn = b;}
  bool wasDrawn() const{return _drawn;}

  void setPainter(SchematicDoc*);
  bool pasteElements(SchematicDoc*);
  void editElement(SchematicDoc* doc, QEvent* e);
  void editLabel(SchematicDoc*, WireLabel*);
  //  void MMoveSelect(SchematicDoc* doc, QEvent* e);
  //  void MMoveElement(SchematicDoc* doc, QEvent* e);
  //  void MMovePaste(SchematicDoc* doc, QEvent* e);
  void MMoveLabel(SchematicDoc* doc, QEvent* e);
	void MMoveMarker(SchematicDoc* doc, QEvent* e);
	void MMoveMirrorY(SchematicDoc* doc, QEvent* e);
	void MMoveMirrorX(SchematicDoc* doc, QEvent* e);
	void MMoveRotate(SchematicDoc* doc, QEvent* e);
	void MMoveActivate(SchematicDoc* doc, QEvent* e);
	void MMoveOnGrid(SchematicDoc* doc, QEvent* e);
	void MMoveResizePainting(SchematicDoc* doc, QEvent* e);
	void MMoveMoveText(SchematicDoc* doc, QEvent* e);
	void MMoveMoveTextB(SchematicDoc* doc, QEvent* e);
	void MMoveZoomIn(SchematicDoc* doc, QEvent* e);
	void MMoveScrollBar(SchematicDoc* doc, QEvent* e);

	void MPressSelect(QEvent*);
	//  void MPressActivate(SchematicDoc* doc, QEvent* e);
	void MPressMirrorX(SchematicDoc* doc, QEvent* e);
	void MPressMirrorY(SchematicDoc* doc, QEvent* e);
	void MPressRotate(SchematicDoc* doc, QEvent* e);
	void MPressLabel(SchematicDoc* doc, QEvent* e);
	void MPressPainting(SchematicDoc* doc, QEvent* e);
	void MPressMarker(SchematicDoc* doc, QEvent* e);
	void MPressOnGrid(SchematicDoc* doc, QEvent* e);
	void MPressMoveText(SchematicDoc* doc, QEvent* e);
	void MPressZoomIn(SchematicDoc* doc, QEvent* e);

	void MDoubleClickSelect(SchematicDoc* doc, QEvent* e);
	void MDoubleClickWire2(SchematicDoc* doc, QEvent* e);

	void MReleaseSelect(SchematicDoc* doc, QEvent* e);
	void MReleaseSelect2(SchematicDoc* doc, QEvent* e);
	void MReleaseActivate(SchematicDoc* doc, QEvent* e);
	void MReleaseMoving(SchematicDoc* doc, QEvent* e);
	void MReleaseResizeDiagram(SchematicDoc* doc, QEvent* e);
	void MReleasePaste(SchematicDoc* doc, QEvent* e);
	void MReleaseResizePainting(SchematicDoc* doc, QEvent* e);
	void MReleaseMoveText(SchematicDoc* doc, QEvent* e);
	void MReleaseZoomIn(SchematicDoc* doc, QEvent* e);

public: // actions... private?
	     // not yet. still toggled from outside
	MouseAction* maDelete{nullptr};
	MouseAction* maSelect{nullptr};
	MouseAction* maWire{nullptr};
	MouseAction* maMirror{nullptr};
	MouseAction* maActivate{nullptr};
	MouseAction* maRotate{nullptr};
	MouseAction* maEditPaste{nullptr};
	MouseAction* maOnGrid{nullptr};
	MouseAction* maMirrorXaxis{nullptr};
	MouseAction* maMirrorYaxis{nullptr};
	MouseAction* maInsertGround{nullptr};
	MouseAction* maInsertElement{nullptr};
	MouseAction* maInsertPort{nullptr};
	MouseAction* maInsertEqn{nullptr};
	MouseAction* maZoomIn{nullptr};
	// MouseAction* maZoomOut; // not a mouseaction

private:
	SchematicScene* mScene{nullptr};
	bool _drawn{false};  // indicates whether the scheme element was drawn last time
};
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#endif
