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

#ifndef QUCSDOC_H
#define QUCSDOC_H

#include "object.h"
#include <QString>
#include <QDateTime>
#include <QMouseEvent>
#include <QTextStream> // tmp: used as baseclass, should be member.
#include <assert.h>

#include "object.h"
#include "actions.h"
#include "io_trace.h"

class QucsApp;
class QAction;
class QPrinter;
class QPainter;
class MouseActionsHandler;
class MouseAction;
class QUndoStack;
class QUndoCommand;
class Element;
class CommonData;
class Simulator;

class QucsDoc : public Object {
protected:
	QucsDoc(const QucsDoc&);
public:
  explicit QucsDoc(QucsApp*, const QString&, QWidget* owner);
  virtual ~QucsDoc();

public:
  bool saveAs();

  virtual void  setName(const QString&) {};
  virtual bool  load() { return true; };
  virtual int   save() { return 0; };
  virtual void  print(QPrinter*, QPainter*, bool, bool) {};
  virtual void  becomeCurrent(bool){ unreachable(); }
  virtual float zoomBy(float) { return 1.0; };
  virtual void  showAll() {};
  virtual void  showNoZoom() {};
  /*!
   * \brief pushUndoStack
   * Pushes the cmd on the undostack if it is available. If it was not possible to
   * push the command on the undo stack, the command \p cmd must be deleted manually
   *
   * Maybe inheriting this class from qobject, so that signal
   * slots can be used.
   * \param cmd
   * \return Returns true if the cmd was pushed successfully, else false
   */
  virtual bool pushUndoStack(QUndoCommand* cmd);

  static QString fileSuffix (const QString&);
  QString fileSuffix (void);
  static QString fileBase (const QString&);
  QString fileBase (void);
  void setDocName(QString x){ DocName=x; }
  QString docName() const{ return DocName; }

  void installElement(Element const*);
  Element const* find_proto(std::string const& name) const;

protected:
  Simulator* simulatorInstance(std::string const& which="");

protected: // TODO: why not directly connect to undostack slots?!
  virtual bool undo();
  virtual bool redo();
  virtual void signalFileChanged(bool){incomplete();}

protected:
//  void toggleAction(QAction* sender);

public: // actions: These somehow correspond to buttons.
        // needs cleanup... better use qt signals, but where?
	virtual void actionSelect(QAction*) { unreachable(); }
	virtual void actionCopy(QAction*) { unreachable(); }
	virtual void actionCut(QAction*) { unreachable(); }
	virtual void actionEditUndo(QAction*) { unreachable(); }
	virtual void actionEditRedo(QAction*) { unreachable(); }
	virtual void actionSelectAll(QAction*) { unreachable(); }
        virtual void actionEditActivate(QAction*) { unreachable(); }
	virtual void actionChangeProps(QAction*) { unreachable(); }
        virtual void actionEditDelete(QAction*) {unreachable();}
        virtual void actionEditPaste(QAction*) {unreachable();}
        virtual void actionZoomIn(QAction*) { unreachable(); }
        virtual void actionInsertEntity(QAction*) {unreachable();}


	void uncheckActive();

	virtual void slotSimulate(); // why "slot"? maybe later.
	virtual void slotDCbias(); // why "slot"? maybe later.

        void activateAction(MouseAction* a, QAction* sender);
	MouseAction const* activeAction() const;

	CommonData* qucsData(std::string const& key);
	QWidget* ownerWidget(){return _owner;}
	void setOwner(QWidget* o){_owner=o;}

	void setParameter(std::string const&, std::string const&);
	//virtual QUndoStack* undoStack(){return _undoStack;}
	virtual void updateViewport() {}
	virtual void reloadGraphs() {} // fix later.

	MouseActionsHandler const* mouseActions() const;
	virtual QMouseEvent snapToGrid(QMouseEvent* e) const{
		assert(e);
		return QMouseEvent(*e);
	}
	/*!
	 * \brief executeCommand
	 * Executes the undo command
	 * If the command was not executed successfully, the undo command must be deleted manually
	 * \return Returns true if the command was executed successfully, otherwise false.
	 */
	bool executeCommand(QUndoCommand*);

protected: // cleaning up debris
	QAction* selectAction();
	virtual MouseActionsHandler* mouseActions();
private:
	void setActiveAction(MouseAction* a);
	MouseAction* activeAction();


public:
        // TODO: move out all non generic data and put it into the
        // docs which need them
	QString DataSet;     // name of the default dataset
	QString DataDisplay; // name of the default data display
	QString Script;
	QString SimTime;     // used for VHDL simulation, but stored in datadisplay
	QDateTime lastSaved;

	//  float Scale;
	QucsApp* _app{nullptr};
	bool DocChanged;
	bool SimOpenDpl;   // open data display after simulation ?
	bool SimRunScript; // run script after simulation ?
	int  showBias;     // -1=no, 0=calculation running, >0=show DC bias points
	bool GridOn;
	int  tmpPosX, tmpPosY;
protected:
        QUndoStack* _undoStack{nullptr}; // Every document has its own undostack
private:
	friend class Simulator;

	std::map<std::string, Element const*> _protos;
	std::map<std::string, CommonData*> _data;
	std::map<std::string, Simulator*> _simulators;
	QString DocName;

	QWidget* _owner{nullptr};
}; // QucsDoc

#endif
