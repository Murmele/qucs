/***************************************************************************
                                   qucs.h
                                  --------
    begin                : Thu Aug 28 2003
    copyright            : (C) 2003 by Michael Margraf
    email                : michael.margraf@alumni.tu-berlin.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUCS_H
#define QUCS_H

#include <QMainWindow>
#include <QString>
#include <QHash>
#include <QStack>
#include <QDir>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>

#include "actions.h" // BUG
#include "platform.h"

/**
 * @file qucs.h
 * @brief definition of the main Qucs application class and other helper classes
 */

class QucsDoc;
class SchematicDoc;
class TextDoc;
class SimMessage; // BUG
class MouseActions;
class SearchDialog;
class OctaveWindow;
class MessageDock;
class ProjectView;
class QucsTabWidget;
class VersionTriplet;
class QucsApp;

class QLabel;
class QAction;
class QLineEdit;
class QComboBox;
class QTabWidget;
class QDir;
class QMouseEvent;
class QCloseEvent;
class QMenu;
class QToolBar;
class QSettings;
class QListWidgetItem;
class QTreeWidget;
class QTreeWidgetItem;
class QListWidget;
class QShortcut;
class QListView;
class QModelIndex;
class QPushButton;

class SimProcess; // really?
class Simulator;


/* pi */
static const double pi = 3.141592653589793238462643383279502884197169399375105820974944;

#include "settings.h"


// extern because nearly everywhere used
extern tQucsSettings QucsSettings;  // extern because nearly everywhere used
extern QString lastDir;    // to remember last directory for several dialogs
extern QStringList qucsPathList;
extern VersionTriplet QucsVersion;
extern QucsApp *QucsMain;  // the Qucs application itself

// TODO move these inside the QucsApp class?
bool loadSettings();
bool saveApplSettings();

// function pointers used with mouse actions handling
typedef void (MouseActions::*pMouseFunc) (SchematicDoc*, QMouseEvent*);
typedef void (MouseActions::*pMouseFunc2) (SchematicDoc*, QMouseEvent*);

class QucsFileSystemModel : public QFileSystemModel {
  Q_OBJECT
public:
  QucsFileSystemModel(QObject* parent = 0) : QFileSystemModel(parent) {};
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
};

class QucsSortFilterProxyModel : public QSortFilterProxyModel {
  Q_OBJECT

public:
  QucsSortFilterProxyModel(QObject *parent = 0) : QSortFilterProxyModel(parent) {};
protected:
  bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

class QucsApp : public QMainWindow {
  Q_OBJECT
public:
  QucsApp();
 ~QucsApp();
  bool closeTabsRange(int startTab, int stopTab, int exceptTab = -1);
  bool closeAllFiles(int exceptTab = -1);
  bool closeAllLeft(int);
  bool closeAllRight(int);
  bool gotoPage(const QString&);   // to load a document
  QucsDoc *getDoc(int No=-1);
  QucsDoc* findDoc (QString, int * Pos = 0);
  QString fileType (const QString&);
  static bool isTextDocument(QucsDoc const*);

  QString ProjName;   // name of the project, that is open
  QHash<QString,QString> schNameHash; // QHash for the schematic files lookup
  QHash<QString,QString> spiceNameHash; // QHash for the spice files lookup

  QLineEdit *editText{nullptr};  // for edit component properties on schematic
  SearchDialog *SearchDia{nullptr};  // global in order to keep values

  // current mouse methods
  void (MouseActions::*MouseMoveAction) (SchematicDoc*, QMouseEvent*);
  void (MouseActions::*MousePressAction) (SchematicDoc*, QMouseEvent*);
  void (MouseActions::*MouseDoubleClickAction) (SchematicDoc*, QMouseEvent*);
  void (MouseActions::*MouseReleaseAction) (SchematicDoc*, QMouseEvent*);

protected:
  void closeEvent(QCloseEvent*);

public slots:
  void slotFileNew(bool enableOpenDpl=true); // generate a new schematic in the view TabBar
  void slotFileNewNoDD(); // as above, but with "open Data Display" unchecked
  void slotTextNew();     // edit text in the built editor or user defined editor
  void slotFileOpen();    // open a document
  void slotFileSave();    // save a document
  void slotFileSaveAs();  // save a document under a different filename
  void slotFileSaveAll(); // save all open documents
  void slotFileClose();   // close the current file
  void slotFileCloseOthers();   // close all documents except the current one
  void slotFileCloseAllLeft();  // close all documents to the left of the current one
  void slotFileCloseAllRight(); // close all documents to the right of the current one
  void slotFileCloseAll();      //  close all documents
  void slotFileExamples();   // show the examples in a file browser
  void slotHelpTutorial();   // Open a pdf tutorial
  void slotHelpReport();   // Open a pdf report
  void slotHelpTechnical();   // Open a pdf technical document
  void slotFileClose (int); // close the file with given index
  void slotSymbolEdit();  // edit the symbol for the schematic
  void slotFileSettings();// open dialog to change file settings
  void slotFilePrint();   // print the current file
  void slotFilePrintFit();// Print and fit to page
  void slotFileQuit();    // exits the application
  void slotApplSettings();// open dialog to change application settings
  void slotRefreshSchPath(); // refresh the schematic path hash

  void slotIntoHierarchy();
  void slotPopHierarchy();

  void slotShowAll();
  void slotShowOne();
  void slotZoomOut(); // Zoom out by 2

  void slotToPage();
  void slotSelectComponent(QListWidgetItem*);
  void slotSearchComponent(const QString &);
  void slotSearchClear();

  void slotEditElement();
  void slotPowerMatching();
  void slot2PortMatching();

  // for menu that appears by right click in content ListView
  void slotShowContentMenu(const QPoint &);

  void slotCMenuOpen();
  void slotCMenuCopy();
  void slotCMenuRename();
  void slotCMenuDelete();
  void slotCMenuInsert();

  void slotMenuProjClose();

private slots:
  void slotMenuProjOpen();
  void slotMenuProjDel();
  void slotListProjOpen(const QModelIndex &);
  void slotSelectSubcircuit(const QModelIndex &);
  void slotSelectLibComponent(QTreeWidgetItem*);
  void slotOpenContent(const QModelIndex &);
  void slotSetCompView(int);
  void slotButtonProjNew();
  void slotButtonProjOpen();
  void slotButtonProjDel();
  void slotChangeView(QucsDoc*);
  void slotSimulate();
  void slotChangePage(QString&, QString&);
  void slotHideEdit();
  void slotFileChanged(bool);

public slots: // why? not here.
  void slotAfterSimulation(Simulator const*);

signals:
  void signalKillEmAll();

public: // called directly from Schematic... bug?
  void hideEdit(){ slotHideEdit(); }

public:
//  MouseActions *view;
  QucsTabWidget *DocumentTab{nullptr};
  QListWidget *CompComps{nullptr}; // Shows the parts which are in the section of CompChoose
  QTreeWidget *libTreeWidget{nullptr};

  // menu appearing by right mouse button click on content listview
  QMenu *ContentMenu{nullptr};

  // corresponding actions
  QAction *ActionCMenuOpen{nullptr}, *ActionCMenuCopy{nullptr}, *ActionCMenuRename{nullptr}, *ActionCMenuDelete{nullptr}, *ActionCMenuInsert{nullptr};

  QAction *fileNew{nullptr}, *fileNewNoDD{nullptr}, *textNew{nullptr}, *fileNewDpl{nullptr}, *fileOpen{nullptr}, *fileSave{nullptr}, *fileSaveAs{nullptr},
          *fileSaveAll{nullptr}, *fileClose{nullptr}, *fileCloseOthers{nullptr}, *fileCloseAllLeft{nullptr}, *fileCloseAllRight{nullptr},
          *fileCloseAll{nullptr}, *fileExamples{nullptr}, *fileSettings{nullptr}, *filePrint{nullptr}, *fileQuit{nullptr},
          *projNew{nullptr}, *projOpen{nullptr}, *projDel{nullptr}, *projClose{nullptr}, *applSettings{nullptr}, *refreshSchPath{nullptr},
          *editCut{nullptr}, *editCopy{nullptr}, *magAll{nullptr}, *magOne{nullptr}, *magMinus{nullptr}, *filePrintFit{nullptr},
          *symEdit{nullptr}, *intoH{nullptr}, *popH{nullptr}, *simulate{nullptr}, *dpl_sch{nullptr}, *undo{nullptr}, *redo{nullptr}, *dcbias{nullptr};

  QAction *exportAsImage{nullptr};

  QAction *activeAction{nullptr};    // pointer to the action selected by the user

private:
// ********* Widgets on the main area **********************************
  QDockWidget     *dock{nullptr}; // Main DockWidget on the right side with the Project list and the component list
  QTabWidget      *TabView{nullptr};
  QDockWidget     *octDock{nullptr};
  OctaveWindow    *octave{nullptr};
  MessageDock     *messageDock{nullptr};

  QListView       *Projects{nullptr};
  ProjectView     *Content{nullptr};

  QLineEdit       *CompSearch{nullptr};
  QPushButton     *CompSearchClear{nullptr};
  QComboBox       *CompChoose{nullptr};

// ********** Properties ************************************************
  QStack<QString> HierarchyHistory; // keeps track of "go into subcircuit"
  QString  QucsFileFilter;
  QucsFileSystemModel *m_homeDirModel{nullptr};
  QucsSortFilterProxyModel *m_proxyModel{nullptr};
  QFileSystemModel *m_projModel{nullptr};
  int ccCurIdx; // CompChooser current index (used during search)

// ********** Methods ***************************************************
  void initView();
  void initCursorMenu();

  void printCurrentDocument(bool);
  bool saveFile(QucsDoc *Doc=0);
  bool saveAs();
  void openProject(const QString &);
  bool deleteProject(const QString &);
  void updatePortNumber(QucsDoc*, int);
  void fillComboBox(bool);
  void switchSchematicDoc(bool);
  void switchEditMode(bool);
  void changeSchematicSymbolMode(SchematicDoc*);
  bool recurRemove(const QString &);
  void closeFile(int);

  void updateRecentFilesList(QString s);
  void successExportMessages(bool ok);
  void fillLibrariesTreeView (void);

public:

  void readProjects();
  void updatePathList(void); // update the list of paths, pruning non-existing paths
  void updatePathList(QStringList);
  void updateSchNameHash(void); // maps all schematic files in the path list
  void updateSpiceNameHash(void); // maps all spice files in the path list

/* **************************************************
   *****  The following methods are located in  *****
   *****  "qucs_init.cpp".                      *****
   ************************************************** */

public slots:
  void slotShowWarnings();
  void slotResetWarnings();
  void printCursorPosition(int, int);
  void slotUpdateUndo(bool);  // update undo available state
  void slotUpdateRedo(bool);  // update redo available state

private slots:
  void slotViewToolBar(bool toggle);    // toggle the toolbar
  void slotViewStatusBar(bool toggle);  // toggle the statusbar
  void slotViewBrowseDock(bool toggle); // toggle the dock window
  void slotViewOctaveDock(bool); // toggle the dock window
  void slotToggleOctave(bool);
  void slotToggleDock(bool);
  void slotHelpAbout();     // shows an about dialog

private:
  void initActions();    // initializes all QActions of the application
  void initMenuBar();    // creates the menu_bar and inserts the menuitems
  void initToolBar();    // creates the toolbars
  void initStatusBar();  // setup the statusbar

  QAction *helpAboutApp{nullptr}, *helpAboutQt{nullptr}, *viewToolBar{nullptr}, *viewStatusBar{nullptr},
          *viewBrowseDock{nullptr}, *viewOctaveDock{nullptr};

  // menus contain the items of their menubar
  enum { MaxRecentFiles = 8 };
  QMenu *fileMenu{nullptr}, *editMenu{nullptr}, *insMenu{nullptr}, *projMenu{nullptr}, *simMenu{nullptr}, *viewMenu{nullptr},
             *helpMenu{nullptr}, *alignMenu{nullptr}, *toolMenu{nullptr}, *recentFilesMenu{nullptr};
  QAction *fileRecentAction[MaxRecentFiles]{nullptr};
  QAction *fileClearRecent{nullptr};

  // submenus for the PDF documents
  QMenu *helpTechnical{nullptr}, *helpReport{nullptr}, *helpTutorial{nullptr};

  QToolBar *fileToolbar{nullptr}, *editToolbar{nullptr}, *viewToolbar{nullptr}, *workToolbar{nullptr};

  // Shortcuts for scolling schematic / TextEdit
  // This is rather cumbersome -> Make this with a QScrollView instead??
  QShortcut *cursorUp{nullptr}, *cursorLeft{nullptr}, *cursorRight{nullptr}, *cursorDown{nullptr};

  QLabel *WarningLabel{nullptr}, *PositionLabel{nullptr};  // labels in status bar

public: // Toolbar?
  void editFile(const QString&);

  QAction *insWire{nullptr}, *insLabel{nullptr}, *insGround{nullptr}, *insPort{nullptr}, *insEquation{nullptr}, *magPlus{nullptr},
          *editRotate{nullptr}, *editMirror{nullptr}, *editMirrorY{nullptr}, *editPaste{nullptr}, *select{nullptr},
          *editActivate{nullptr}, *wire{nullptr}, *editDelete{nullptr}, *setMarker{nullptr}, *onGrid{nullptr}, *moveText{nullptr},
          *helpOnline{nullptr}, *callEditor{nullptr}, *callFilter{nullptr}, *callLine{nullptr}, *callActiveFilter{nullptr},
          *showMsg{nullptr}, *showNet{nullptr}, *alignTop{nullptr}, *alignBottom{nullptr}, *alignLeft{nullptr}, *alignRight{nullptr},
          *distrHor{nullptr}, *distrVert{nullptr}, *selectAll{nullptr}, *callLib{nullptr}, *callMatch{nullptr}, *changeProps{nullptr},
          *addToProj{nullptr}, *editFind{nullptr}, *insEntity{nullptr}, *selectMarker{nullptr}, *callPowerComb{nullptr},
          *createLib{nullptr}, *importData{nullptr}, *graph2csv{nullptr}, *createPkg{nullptr}, *extractPkg{nullptr},
          *callAtt{nullptr}, *callRes{nullptr}, *centerHor{nullptr}, *centerVert{nullptr}, *loadModule{nullptr}, *buildModule{nullptr};

  QAction* selectAction(){ return select; }

public slots: // BUG. why is this here? (fix later)
  void slotEditRotate();  // rotate the selected items
  void slotEditMirrorX(); // mirror the selected items about X axis
  void slotEditMirrorY(); // mirror the selected items about Y axis
  void slotEditCut();         // put marked object into clipboard and delete it
  void slotEditCopy();        // put the marked object into the clipboard
  void slotEditPaste();   // paste the clipboard into the document
  void slotEditDelete();  // delete the selected items
  void slotInsertEquation();
  void slotInsertGround();
  void slotInsertPort();
  void slotInsertEntity();
  void slotSetWire();
  void slotEscape();
  void slotSelect();
  void slotEditActivate();
  void slotInsertLabel();
  void slotSetMarker();
  void slotOnGrid();      // set selected elements on grid
  void slotMoveText();    // move property text of components
  void slotZoomIn();
  void slotEditUndo();    // makes the last operation undone
  void slotEditRedo();    // makes the last undo undone
  void slotEditFind();    // searches for a piece of text
  void slotAlignTop();    // align selected elements with respect to top
  void slotAlignBottom(); // align selected elements with respect to bottom
  void slotAlignLeft();   // align selected elements with respect to left
  void slotAlignRight();  // align selected elements with respect to right
  void slotDistribHoriz();// distribute horizontally selected elements
  void slotDistribVert(); // distribute vertically selected elements
  void slotCenterHorizontal();
  void slotCenterVertical();
  void slotSelectAll();
  void slotSelectMarker();
  void slotShowLastMsg();
  void slotShowLastNetlist();
  void slotCallEditor();
  void slotCallFilter();
  void slotCallActiveFilter();
  void slotCallLine();
  void slotCallLibrary();
  void slotCallMatch();
  void slotCallAtt();
  void slotCallRes();
  void slotHelpOnline();
  void slotCallPowerComb();
  void slotChangeProps();
  void slotAddToProject();
  void slotApplyCompText();
  void slotOpenRecent();
  void slotSaveDiagramToGraphicsFile();
  void slotSaveSchematicToGraphicsFile(bool diagram = false);

private:
  void slotCursor(arrow_dir_t dir);

private slots:
  void slotCursorLeft() {return slotCursor(arr_left);}
  void slotCursorRight() {return slotCursor(arr_right);}
  void slotCursorUp() {return slotCursor(arr_up);}
  void slotCursorDown() {return slotCursor(arr_down);}

  void slotResizePropEdit(const QString&);
  void slotCreateLib();
  void slotImportData();
  void slotExportSchematic();
  void slotExportGraphAsCsv();
  void slotCreatePackage();
  void slotExtractPackage();
  void slotUpdateRecentFiles();
  void slotClearRecentFiles();
  void slotLoadModule();
  void slotBuildModule();

private:
  void showHTML(const QString&);
  void launchTool(const QString&, const QString&, const QString& = ""); // tool, description and args
  friend class SaveDialog;
  QString lastExportFilename;
}; // qucsApp


#endif /* QUCS_H */
