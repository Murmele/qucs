#ifndef MOUSEACTIONSHANDLER_H
#define MOUSEACTIONSHANDLER_H

#include <QObject>

class ElementGraphics;
class SchematicDoc;
class WireLabel;
class QucsDoc;
class MouseAction;

class QMenu;
class QEvent;
class QUndoCommand;

/*!
 * \brief The MouseActionsHandler class
 * Main class for mouse actions
 *
 * must be QObject so it can receive/filter events
 *
 * This class stores which action is the current one, because only one action can be
 * active at the time.
 * The current action can be set by using setActive()
 *
 * QEvent was used instead of QMouseEvent or QGraphicsSceneMouseEvent, because which one
 * should be used can be freely decided by everybody else when deriving from this class
 */
class MouseActionsHandler : public QObject {
	Q_OBJECT
public:
  typedef QList<ElementGraphics*> EGPList;
public:
  MouseActionsHandler(QucsDoc &);
  virtual ~MouseActionsHandler();

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

public: // really?
  QucsDoc& doc() const;
  void updateViewport();

public:
  // obsolete
//  void rightPressMenu(QMouseEvent*);

  bool eventFilter(QObject *obj, QEvent *event);
  /*!
   * \brief handle
   * Executes the related MouseAction
   * \return
   */
  virtual bool handle(QEvent*);
  MouseAction* activeAction(){ return _maCurrent; }
  void setActive(MouseAction* a);
  void undo();
  void redo();

public:
  /*!
   * \brief executeCommand
   * Executes the undo command indirectly by pushing it on the undostack
   * If execution was not successful, the command must be deleted manually
   * \param c
   * \return
   */
  bool executeCommand(QUndoCommand* c); // TODO: maybe making private?

protected:
  /*!
   * \brief _maCurrent
   * This object stores the current action, because only one action can be active at the time.
   */
  MouseAction* _maCurrent;
private:
  // QUndoStack* _undoStack; // Doc
private:
  QucsDoc& mDoc;
}; // MouseActions
#endif // MOUSEACTIONSHANDLER_H
