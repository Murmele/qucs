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

public:
  QucsDoc& doc() const;
  void updateViewport();

  bool eventFilter(QObject *obj, QEvent *event);
  /*!
   * \brief handle
   * Executes the related MouseAction
   * \return
   */
  virtual bool handle(QEvent*);
  /*!
   * \brief activeAction
   * Returns the currently active action which is handled when handle()
   * is called
   * \return
   */
  MouseAction* activeAction(){ return _maCurrent; }
  /*!
   * \brief setActive
   * Set new mouse action as active. So when any mouse event occurs, the mouse action \p a gets it
   * \param a New active mouse action
   */
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
  QucsDoc& mDoc;
}; // MouseActions
#endif // MOUSEACTIONSHANDLER_H
