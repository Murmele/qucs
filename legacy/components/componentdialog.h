#ifndef COMPONENTDIALOG_H
#define COMPONENTDIALOG_H

#include "schematic_dialog.h"

class QucsDoc;

class QVBoxLayout;
class QValidator;
class QIntValidator;
class QLineEdit;
class QComboBox;
class QLabel;
class QPushButton;
class QCheckBox;
class Component;
class QTableWidget;

/*!
 * \brief The ComponentDialog class
 * Dialog to modify the properties of a component
 */
class ComponentDialog : public SchematicDialog {
public:
  ComponentDialog(QucsDoc*);
  ~ComponentDialog();


  /*!
   * \brief attach
   * This method shall not be called multiple times! There is an internal variable which checks that.
   * TODO: maybe adding everything to constructor, or at least the creation of widgets
   * \param c
   */
  virtual void attach(ElementGraphics* c) override;

private: // slot overrides.
  void slotButtOK();
  void slotButtCancel();
  void slotSelectProperty(QTableWidgetItem *item);
  void slotApplyInput();
  void slotApplyState(int State);
  void slotBrowseFile();
  void slotEditFile();
  void slotApplyChange(const QString& Text);
  void slotApplyProperty();
  void slotApplyPropName();

  void createNewProperty(); // DONE: cleaned up
  void slotButtRem();

  void slotButtUp();
  void slotButtDown();

  void slotParamEntered();
  void slotValuesEntered();
  void slotNumberEntered();
  void slotHHeaderClicked(int headerIdx);

public:
  void enableButtons();
  void disableButtons();

protected slots:
    void reject();
    bool eventFilter(QObject *obj, QEvent *event);

private:
    bool propertyRow(const QString name) const; // DONE: cleaned up
    QString uniquePropertyName(const QString prefix) const; // DONE: cleanup

private:
  QVBoxLayout *_all;
  QValidator  *Validator, *ValRestrict, *Validator2;
  QRegExp     Expr;
  QIntValidator *ValInteger;
  QTableWidget  *propTable{nullptr};
  QLineEdit   *edit, *NameEdit, *CompNameEdit;
  QComboBox   *ComboEdit;
  QLabel      *Name, *Description;
  QPushButton *BrowseButt, *EditButt, *ButtAddNewProperty{nullptr}, *ButtRemProperty{nullptr};
  QPushButton *ButtUp, *ButtDown;
  QCheckBox   *disp;
  bool        changed;
  int         tx_Dist, ty_Dist;   // remember the text position
  bool        setAllVisible; // used for toggling visibility of properties

  // TODO: check with variables are still needed
  QLabel    *textType;
  QLabel    *textSim, *textParam, *textValues, *textStart, *textStop,
            *textStep, *textNumber;
  QLineEdit *editParam, *editValues, *editNumber{nullptr};
  QCheckBox *checkStart, *checkStop,
            *checkNumber, *checkType, *showName;
  void updateCompPropsList(void);

private:
  ElementGraphics* _gfx;
  Component const* _comp;
  bool attached{false}; // For checking that attached gets only called once
};

#endif /* COMPONENTDIALOG_H */
