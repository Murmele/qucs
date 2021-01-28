#include "componentdialog.h"
#include "component.h"
#include "property.h"
#include "qucs.h"
#include "schematic_doc.h"
#include "misc.h"

#include <cmath>

#include <QLabel>
#include <QLayout>
#include <QValidator>
#include <QTableWidget>
#include <QHeaderView>
#include <QFileDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>
#include "swap.h"

typedef enum {
   name = 0,
   value = 1,
   show = 2,
   desc = 3,
} PropTableColumns;
/*--------------------------------------------------------------------------*/
ComponentDialog::ComponentDialog(QucsDoc* d) : SchematicDialog(d)
{
  resize(450, 250);
  setWindowTitle(tr("Edit Component Properties"));
}
/*--------------------------------------------------------------------------*/
void ComponentDialog::attach(ElementGraphics* gfx)
{

  // If this method gets called multiple times, there is a memory leak.
  assert(!attached);
  attached = true;

  trace0("ComponentDialog::attach");
  auto Comp = dynamic_cast<Component*>(element(gfx));
  assert(Comp);
  if(_comp){
    incomplete();
  }else{
  }
  _comp = Comp;
  _gfx = gfx;
  QString s;
  setAllVisible = true; // state when toggling properties visibility

  _all = new QVBoxLayout; // to provide neccessary size
  this->setLayout(_all);
  _all->setContentsMargins(1,1,1,1);
  QGridLayout *gp1;

  ValInteger = new QIntValidator(1, 1000000, this);

  Expr.setPattern("[^\"=]*");  // valid expression for property 'edit'
  Validator = new QRegExpValidator(Expr, this);
  Expr.setPattern("[^\"]*");   // valid expression for property 'edit'
  Validator2 = new QRegExpValidator(Expr, this);
  Expr.setPattern("[\\w_]+");  // valid expression for property 'NameEdit'
  ValRestrict = new QRegExpValidator(Expr, this);
  
  Property *pp = nullptr; // last property shown elsewhere outside the properties table, not to put in TableView
  // ...........................................................
  //gp1 = new QGridLayout(0, 9,2,5,5);
  gp1 = new QGridLayout();
  _all->addLayout(gp1);


  gp1->addWidget(new QLabel(Comp->description(), this), 0,0,1,2);

  QHBoxLayout *h5 = new QHBoxLayout;
  h5->setSpacing(5);

  h5->addWidget(new QLabel(tr("Name:"), this) );

  CompNameEdit = new QLineEdit(this);
  h5->addWidget(CompNameEdit);

  CompNameEdit->setValidator(ValRestrict);
  connect(CompNameEdit, SIGNAL(returnPressed()), SLOT(slotButtOK()));

  showName = new QCheckBox(tr("display in schematic"), this);
  h5->addWidget(showName);

  QWidget *hTop = new QWidget(this);
  hTop->setLayout(h5);

  gp1->addWidget(hTop,1,0);

  QGroupBox *PropertyBox = new QGroupBox(tr("Properties"), this);
  gp1->addWidget(PropertyBox,2,0);

  // H layout inside the GroupBox
  QHBoxLayout *hProps = new QHBoxLayout;
  PropertyBox->setLayout(hProps);

  // left pane
  QWidget *vboxPropsL = new QWidget(this);
  QVBoxLayout *vL = new QVBoxLayout;
  vboxPropsL->setLayout(vL);

  /// \todo column min width
  propTable = new QTableWidget(0,4, this); //initialize
  vL->addWidget(propTable);
  propTable->verticalHeader()->setVisible(false);
  propTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  propTable->setSelectionMode(QAbstractItemView::SingleSelection);
  propTable->setMinimumSize(200, 150);
  propTable->horizontalHeader()->setStretchLastSection(true);
  // set automatic resize so all content will be visible, 
  //  horizontal scrollbar will appear if table becomes too large
  propTable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  propTable->horizontalHeader()->setClickable(false); // no action when clicking on the header

  connect(propTable->horizontalHeader(),SIGNAL(sectionDoubleClicked(int)),
              this, SLOT(slotHHeaderClicked(int)));

  QStringList headers;
  headers << tr("Name")
          << tr("Value")
          << tr("display")
          << tr("Description");
  propTable->setHorizontalHeaderLabels(headers);

  // right pane
  QWidget *vboxPropsR = new QWidget(this);
  QVBoxLayout *v1 = new QVBoxLayout;
  vboxPropsR->setLayout(v1);

  v1->setSpacing(3);

  hProps->addWidget(vboxPropsL, 5); // stretch the left pane (with the table) when resized
  hProps->addWidget(vboxPropsR);

  Name = new QLabel(this);
  v1->addWidget(Name);

  Description = new QLabel(this);
  v1->addWidget(Description);

  // hide, because it only replaces 'Description' in some cases
  NameEdit = new QLineEdit(this);
  v1->addWidget(NameEdit);
  NameEdit->setVisible(false);
  NameEdit->setValidator(ValRestrict);
  connect(NameEdit, SIGNAL(returnPressed()), SLOT(slotApplyPropName()));

  edit = new QLineEdit(this);
  v1->addWidget(edit);
  edit->setMinimumWidth(150);
  edit->setValidator(Validator2);
  connect(edit, SIGNAL(returnPressed()), SLOT(slotApplyProperty()));

  // hide, because it only replaces 'edit' in some cases
  ComboEdit = new QComboBox(this);
  v1->addWidget(ComboEdit);
  ComboEdit->setVisible(false);
  ComboEdit->installEventFilter(this); // to catch Enter keypress
  connect(ComboEdit, SIGNAL(activated(const QString&)),
	  SLOT(slotApplyChange(const QString&)));

  QHBoxLayout *h3 = new QHBoxLayout;
  v1->addLayout(h3);

  EditButt = new QPushButton(tr("Edit"), this);
  h3->addWidget(EditButt);
  EditButt->setEnabled(false);
  EditButt->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  connect(EditButt, SIGNAL(clicked()), SLOT(slotEditFile()));

  BrowseButt = new QPushButton(tr("Browse"), this);
  h3->addWidget(BrowseButt);
  BrowseButt->setEnabled(false);
  BrowseButt->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  connect(BrowseButt, SIGNAL(clicked()), SLOT(slotBrowseFile()));

  disp = new QCheckBox(tr("display in schematic"), this);
  v1->addWidget(disp);
  connect(disp, SIGNAL(stateChanged(int)), SLOT(slotApplyState(int)));

  // keep group above together
  v1->addStretch(5);

  QGridLayout *bg = new QGridLayout;
  v1->addLayout(bg);
  ButtAddNewProperty = new QPushButton(tr("Add New Property"), this);
  bg->addWidget(ButtAddNewProperty, 0, 0);
  ButtRemProperty = new QPushButton(tr("Remove Property"), this);
  ButtRemProperty->setToolTip(tr("Remove a non locked property"));
  bg->addWidget(ButtRemProperty, 0, 1);
  ButtRemProperty->setEnabled(false);
  connect(ButtAddNewProperty, SIGNAL(clicked()), SLOT(createNewProperty()));
  connect(ButtRemProperty, SIGNAL(clicked()), SLOT(slotButtRem()));
  // Buttons to move equations up/down on the list
  ButtUp = new QPushButton(tr("Move Up"), this);
  bg->addWidget(ButtUp, 1, 0);
  ButtDown = new QPushButton(tr("Move Down"), this);
  bg->addWidget(ButtDown, 1, 1);
  connect(ButtUp,   SIGNAL(clicked()), SLOT(slotButtUp()));
  connect(ButtDown, SIGNAL(clicked()), SLOT(slotButtDown()));


  // ...........................................................
  QHBoxLayout *h2 = new QHBoxLayout;
  QWidget * hbox2 = new QWidget(this);
  hbox2->setLayout(h2);
  h2->setSpacing(5);
  _all->addWidget(hbox2);
  QPushButton *ok = new QPushButton(tr("OK"), this);
  QPushButton *apply = new QPushButton(tr("Apply"), this);
  QPushButton *cancel = new QPushButton(tr("Cancel"), this);
  h2->addWidget(ok);
  h2->addWidget(apply);
  h2->addWidget(cancel);
  connect(ok,     SIGNAL(clicked()), SLOT(slotButtOK()));
  connect(apply,  SIGNAL(clicked()), SLOT(slotApplyInput()));
  connect(cancel, SIGNAL(clicked()), SLOT(slotButtCancel()));

  // ------------------------------------------------------------
  CompNameEdit->setText(QString::fromStdString(Comp->label()));
  showName->setChecked(Comp->showName);
  changed = false;

  Comp->textSize(tx_Dist, ty_Dist);
  int tmp = Comp->tx+tx_Dist - Comp->x1_();
  if((tmp > 0) || (tmp < -6))  tx_Dist = 0;  // remember the text position
  tmp = Comp->ty+ty_Dist - Comp->y1_();
  if((tmp > 0) || (tmp < -6))  ty_Dist = 0;

  /*! Insert all \a Comp properties into the dialog \a propTable list */
  int row=0; // row counter
  Comp->Props.findRef(pp);
  Property *p=Comp->Props.current();
  if(p){
    p = Comp->Props.next();
  }else{
    p = Comp->Props.first();
  }
  for(; p!=nullptr; p=Comp->Props.next()) {

      // do not insert if already on first tab
      // this is the reason it was originally from back to front...
      // the 'pp' is the lasted property stepped over while filling the Swep tab
  //    if(p == pp)
  //      break;
      if(p->display){
        s = tr("yes");
      }else{
        s = tr("no");
      }

      // add Props into TableWidget
      qDebug() << " Loading Comp->Props :" << p->Name << p->Value << p->display << p->Description ;

      propTable->setRowCount(propTable->rowCount()+1);

      QTableWidgetItem *cell;
      cell = new QTableWidgetItem(p->Name);
      cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);
      propTable->setItem(row, 0, cell);
      cell = new QTableWidgetItem(p->Value);
      cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);
      propTable->setItem(row, 1, cell);
      cell = new QTableWidgetItem(s);
      cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);
      propTable->setItem(row, 2, cell);
      cell = new QTableWidgetItem(p->Description);
      cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);
      propTable->setItem(row, 3, cell);

      row++;
    }

    if(propTable->rowCount() > 0) {
        propTable->setCurrentItem(propTable->item(0,0));
        slotSelectProperty(propTable->item(0,0));
    }


  /// \todo add key up/down brose and select propTable
  connect(propTable, SIGNAL(itemClicked(QTableWidgetItem*)),
                SLOT(slotSelectProperty(QTableWidgetItem*)));
}
/*--------------------------------------------------------------------------*/
ComponentDialog::~ComponentDialog()
{
  delete _all;
  delete Validator;
  delete Validator2;
  delete ValRestrict;
  delete ValInteger;
}
/*--------------------------------------------------------------------------*/
// check if Enter is pressed while the ComboEdit has focus
// in case, behave as for the LineEdits
// (QComboBox by default does not handle the Enter/Return key)
bool ComponentDialog::eventFilter(QObject *obj, QEvent *event)
{
  if (obj == ComboEdit) {
    if (event->type() == QEvent::KeyPress) {
      QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
      if ((keyEvent->key() == Qt::Key_Return) ||
          (keyEvent->key() == Qt::Key_Enter)) {
        slotApplyProperty();
        return true;
      }
    }
  }
  return QDialog::eventFilter(obj, event); // standard event processing
}
/*--------------------------------------------------------------------------*/
// Updates component property list. Useful for MultiViewComponents (really?)
void ComponentDialog::updateCompPropsList()
{
 auto Comp=_comp;
    int last_prop=0; // last property not to put in ListView

    QString s;
    int row=0; // row counter
    //for(Property *p = Comp->Props.first(); p != 0; p = Comp->Props.next()) {}
    for(Property *p = Comp->Props.at(last_prop); p != 0; p = Comp->Props.next()) {

      // do not insert if already on first tab
      // this is the reason it was originally from back to front...
      // the 'pp' is the lasted property stepped over while filling the Swep tab
  //    if(p == pp)
  //      break;
      if(p->display){
        s = tr("yes");
      }else{
        s = tr("no");
      }

      // add Props into TableWidget
      qDebug() << " Loading Comp->Props :" << p->Name << p->Value << p->display << p->Description ;

      if (row > propTable->rowCount()-1) { // Add new rows
          propTable->setRowCount(propTable->rowCount()+1);
      }

      QTableWidgetItem *cell;
      cell = new QTableWidgetItem(p->Name);
      cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);
      propTable->setItem(row, 0, cell);
      cell = new QTableWidgetItem(p->Value);
      cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);
      propTable->setItem(row, 1, cell);
      cell = new QTableWidgetItem(s);
      cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);
      propTable->setItem(row, 2, cell);
      cell = new QTableWidgetItem(p->Description);
      cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);
      propTable->setItem(row, 3, cell);

      row++;
    }

    if(propTable->rowCount() > 0) {
        propTable->setCurrentItem(propTable->item(0,0));
        slotSelectProperty(propTable->item(0,0));
    }else{
    }

    if (row < propTable->rowCount()-1) {
        propTable->setRowCount(row);
    }else{
    }
}
/*--------------------------------------------------------------------------*/
/*!
 * \brief ComponentDialog::slotSelectProperty
 * Is called if a property is selected.
 * Handle the Property editor tab.
 * It transfers the values to the right side for editing.
 * \param item
 */
void ComponentDialog::slotSelectProperty(QTableWidgetItem *item)
{
  auto Comp=_comp;
  if(item == 0) return;
  item->setSelected(true);  // if called from elsewhere, this was not yet done

  qDebug() << "row " << item->row(); //<< item->text()

  QString name  = propTable->item(item->row(),PropTableColumns::name)->text();
  QString value = propTable->item(item->row(),PropTableColumns::value)->text();
  QString show  = propTable->item(item->row(),PropTableColumns::show)->text();
  QString desc  = propTable->item(item->row(),PropTableColumns::desc)->text();

  if(show == tr("yes")){
    disp->setChecked(true);
  }else{
    disp->setChecked(false);
  }

  if(name == "File") {
    EditButt->setEnabled(true);
    EditButt->setToolTip(tr(""));
    BrowseButt->setEnabled(true);
    BrowseButt->setToolTip(EditButt->toolTip());
  }else{
    EditButt->setEnabled(false);
    EditButt->setToolTip(tr("The selected property is not a file"));
    BrowseButt->setEnabled(false);
    BrowseButt->setToolTip(EditButt->toolTip());
  }

  /// \todo enable edit of description anyway...
  /// empy or "-" (no comment from verilog-a)
  // TODO: why it depends on the description, if it is possible
  // to remove a property??
  // Mabey implementing something like lock, if some properties
  // are required
  if(desc.isEmpty()) {
    // show two line edit fields (name and value)
    // Possible to edit the name and the value of the property
    ButtRemProperty->setEnabled(true);

    Comp->dialgButtStuff(*this);
    Name->setText("");
    NameEdit->setText(name);
    edit->setText(value);

    edit->setVisible(true);
    NameEdit->setVisible(true);
    Description->setVisible(false);
    ComboEdit->setVisible(false);

    NameEdit->setFocus();   // edit QLineEdit
  }else{  // show standard line edit (description and value)
    ButtRemProperty->setEnabled(false);
    ButtUp->setEnabled(false);
    ButtDown->setEnabled(false);

    Name->setText(name);
    edit->setText(value);

    NameEdit->setVisible(false);
    NameEdit->setText(name); // perhaps used for adding properties
    Description->setVisible(true);

    // handle special combobox items
    QStringList List;
    int b = desc.indexOf('[');
    int e = desc.lastIndexOf(']');
    if (e-b > 2) { // means that there are not only empty brackets
      QString str = desc.mid(b+1, e-b-1);
      str.replace( QRegExp("[^a-zA-Z0-9_,]"), "" );
      List = str.split(',');
      qDebug() << "List = " << List;
    }

    // use the screen-compatible metric
    QFontMetrics metrics(QucsSettings.font, 0);   // get size of text
    qDebug() << "desc = " << desc << metrics.horizontalAdvance(desc);
    while(metrics.horizontalAdvance(desc) > 270) {  // if description too long, cut it nicely
      // so 270 above will be the maximum size of the name label and associated edit line widget 
      if (desc.lastIndexOf(' ') != -1)
        desc = desc.left(desc.lastIndexOf(' ')) + "....";
      else
        desc = desc.left(desc.length()-5) + "....";
    }
    Description->setText(desc);

    if(List.count() >= 1) {    // ComboBox with value list or line edit ?
      ComboEdit->clear();
      ComboEdit->insertItems(0,List);

      for(int i=ComboEdit->count()-1; i>=0; i--)
       if(value == ComboEdit->itemText(i)) {
         ComboEdit->setCurrentIndex(i);
	 break;
       }
      edit->setVisible(false);
      ComboEdit->setVisible(true);
      ComboEdit->setFocus();
    }else{
      edit->setVisible(true);
      ComboEdit->setVisible(false);
      edit->setFocus();   // edit QLineEdit
    }
  }
}
/*--------------------------------------------------------------------------*/
void ComponentDialog::slotApplyChange(const QString& Text)
{ untested();
  /// \bug what if the table have no items?
  // pick selected row
  QList<QTableWidgetItem *> items = propTable->selectedItems();
  Q_ASSERT(!items.isEmpty());
  QTableWidgetItem *item = items.first();
  
  int row = item->row();
  
  edit->setText(Text);
  // apply edit line
  propTable->item(row, 1)->setText(Text);

  ComboEdit->setFocus();

  // step to next item if not at the last line
  if ( row < (propTable->rowCount() - 1)) {
    propTable->setCurrentItem(propTable->item(row+1,0));
    slotSelectProperty(propTable->item(row+1,0));
  }
}
/*--------------------------------------------------------------------------*/
/*!
 Is called if the "RETURN" key is pressed in the "edit" Widget.
 The parameter is edited on the right pane.
 Return key commits the change, and steps to the next parameter in the list.
*/
void ComponentDialog::slotApplyProperty()
{
  // pick selected row
  QTableWidgetItem *item = propTable->currentItem();
  
  if(!item)
    return;
  
  int row = item->row();

  QString name  = propTable->item(row, PropTableColumns::name)->text();
  QString value = propTable->item(row, PropTableColumns::value)->text();

  if (!ComboEdit->isHidden())   // take text from ComboBox ?
    edit->setText(ComboEdit->currentText());

  // apply edit line
  if(value != edit->text()) {
       propTable->item(row, PropTableColumns::value)->setText(edit->text());
    }

  if (!NameEdit->isHidden())	// also apply property name ?
    if (name != NameEdit->text()) {
//      if(NameEdit->text() == "Export")
//        item->setText(0, "Export_");   // name must not be "Export" !!!
//      else
//      item->setText(0, NameEdit->text());  // apply property name
      propTable->item(row, PropTableColumns::name)->setText(NameEdit->text());
    }

  // step to next item
  if ( row < propTable->rowCount()-1) {
    propTable->setCurrentItem(propTable->item(row+1,PropTableColumns::name));
    slotSelectProperty(propTable->item(row+1,PropTableColumns::name));
  }
  else {
    slotButtOK();   // close dialog, if it was the last property
    return;
  }
}
/*--------------------------------------------------------------------------*/
// Is called if the "RETURN"-button is pressed in the "NameEdit" Widget.
void ComponentDialog::slotApplyPropName()
{
  // pick selected row
  QTableWidgetItem *item = propTable->selectedItems()[0];
  int row = item->row();

  QString name  = propTable->item(row, PropTableColumns::name)->text();

  if(name != NameEdit->text()) {
//    if(NameEdit->text() == "Export") {
//	item->setText(0, "Export_");   // name must not be "Export" !!!
//	NameEdit->setText("Export_");
//    }
//      else
    propTable->item(row, PropTableColumns::name)->setText(NameEdit->text());
  }
  edit->setFocus();   // cursor into "edit" widget
}
/*--------------------------------------------------------------------------*/
// Is called if the checkbox is pressed (changed).
void ComponentDialog::slotApplyState(int State)
{
  // pick selected row
  QTableWidgetItem *item = propTable->selectedItems()[0];
  int row = item->row();

  QString disp  = propTable->item(row, PropTableColumns::show)->text();

  if(item == 0) return;

  QString ButtonState;
  if(State)
    ButtonState = tr("yes");
  else
    ButtonState = tr("no");

  if(disp != ButtonState) {
    propTable->item(row, PropTableColumns::show)->setText(ButtonState);
  }
}
/*--------------------------------------------------------------------------*/
// Is called if the "OK"-button is pressed.
void ComponentDialog::slotButtOK()
{
  slotApplyInput();
  slotButtCancel();
}
/*--------------------------------------------------------------------------*/
// Is called if the "Cancel"-button is pressed.
void ComponentDialog::slotButtCancel()
{
  if(changed){
    // changed could have been done before
    done(1);
  } else{
    // (by "Apply"-button)
    done(0);	
  }
}
/*--------------------------------------------------------------------------*/
// To get really all close events (even <Escape> key).
void ComponentDialog::reject()
{
  slotButtCancel();
}
/*--------------------------------------------------------------------------*/
void ComponentDialog::slotApplyInput()
{
  assert(_comp);
  auto C = _comp->clone();
  C->setOwner( _comp->mutable_owner() );
  auto Comp = prechecked_cast<Component*>(C);
  assert(Comp);

  qDebug() << " \n == Apply ";
  if(Comp->showName != showName->isChecked()) {
    Comp->showName = showName->isChecked();
    changed = true;
  }else{
  }

  QString tmp;
  Component *pc = nullptr;
  if(CompNameEdit->text().isEmpty()){
    CompNameEdit->setText(Comp->label().c_str());
  }else if(CompNameEdit->text().toStdString() != Comp->label()) {
    trace2("Apply", Comp->label(), CompNameEdit->text());
#if 0
    for(pc = schematic()->components().first(); pc!=0;
        pc=schematic()->components().next()){
      if(pc->name() == CompNameEdit->text()){
        break;  // found component with this name?
      }else{
      }
    }
#endif
    if(pc){
      CompNameEdit->setText(Comp->label().c_str());
    } else if (Comp->label() != CompNameEdit->text().toStdString()) {
      Comp->setLabel(CompNameEdit->text().toStdString());
      changed = true;
    }
  }

  // pick selected row
  QTableWidgetItem *item = 0;

  //  make sure we have one item, take selected
  if (propTable->selectedItems().size()) {
    item = propTable->selectedItems()[0];
  }

  /*! Walk the dialog list of 'propTable'
   */
    Property *pp = Comp->Props.first();
   if(item != 0) {
     int row = item->row();
     QString name  = propTable->item(row, 0)->text();
     QString value = propTable->item(row, 1)->text();

     // apply edit line
     if(value != edit->text())
       propTable->item(row, PropTableColumns::value)->setText(edit->text());

     // apply property name
     if (!NameEdit->isHidden())
       if (name != NameEdit->text())
         propTable->item(row, PropTableColumns::name)->setText(NameEdit->text());

     // apply all the new property values in the ListView
     bool display;
     for( int row = 0; row < propTable->rowCount(); row++ ) {

       QString name  = propTable->item(row, PropTableColumns::name)->text();
       QString value = propTable->item(row, PropTableColumns::value)->text();
       QString disp = propTable->item(row, PropTableColumns::show)->text();
       QString desc = propTable->item(row, PropTableColumns::desc)->text();

       qDebug() << "====>" <<name << value
                << Comp->Props.count()
                << propTable->rowCount() +1
                << pp;

       display = (disp == tr("yes"));
       if( pp ) {

         if(pp->display != display) {
             pp->display = display;
             changed = true;
         }else{
         }

         if(pp->Value != value) {
            pp->Value = value;
            changed = true;
         }else{
         }

         if(pp->Name != name) {
           pp->Name = name;   // override if previous one was removed
           changed = true;
         }else{
         }

         pp->Description = desc;
       } else {
         // if properties where added in the dialog
         // -> create new on the Comp
         Q_ASSERT(propTable->rowCount() >= 0);
         if ( (int) Comp->Props.count() < propTable->rowCount() +1) {
             qDebug() << "adding to Comp ";
             Comp->Props.append(new Property(name, value, display, desc));
             changed = true;
         }
    }
    pp = Comp->Props.next();
  }

  // original Comp still has properties? (removed some in the dialog?)
  // if more properties than in ListView -> delete the rest
  if(pp) {
    pp = Comp->Props.prev();
    Comp->Props.last();
    while(pp != Comp->Props.current())
      Comp->Props.remove();
    changed = true;
  }

 } // end if (item !=0)

  if(changed) {
    int dx, dy;
    Comp->textSize(dx, dy);
    if(tx_Dist != 0) {
      Comp->tx += tx_Dist-dx;
      tx_Dist = dx;
    }else{
    }
    if(ty_Dist != 0) {
      Comp->ty += ty_Dist-dy;
      ty_Dist = dy;
    }else{
    }


    auto pos = _gfx->pos();

    auto cmd = new SwapSymbolCommand(_gfx, Comp);
    execute(cmd);

    assert(_gfx->pos() == pos); // for now.

    Comp = dynamic_cast<Component*>(element(_gfx));
    _comp = Comp;

    // BUG: cannot modify while shown.
    // maybe do in ElementGraohics::attach?
//    schematic()->recreateSymbol(Comp);
//
    auto V=schematic()->viewport();
    assert(V);
    V->repaint();
    if ( (int) Comp->Props.count() != propTable->rowCount()) { // If props count was changed after recreation
      Q_ASSERT(propTable->rowCount() >= 0);
      updateCompPropsList(); // of component we need to update properties
    }
  }else{
  }
}
/*--------------------------------------------------------------------------*/
void ComponentDialog::slotBrowseFile()
{
  incomplete();
  // current file name from the component properties
  QString currFileName = propTable->item(propTable->currentRow(), PropTableColumns::value)->text();
  QFileInfo currFileInfo(currFileName);
  // name of the schematic where component is instantiated (may be empty)
  QFileInfo schematicFileInfo; // = Comp->getSchematic()->getFileInfo();
  QString schematicFileName; //  = schematicFileInfo.fileName();
  // directory to use for the file open dialog
  QString currDir = "unknown_dir";

  if (!currFileName.isEmpty()) { // a file name is already defined
    if (currFileInfo.isRelative()) { // but has no absolute path
      if (!schematicFileName.isEmpty()) { // if schematic has a filename
        incomplete();
	// build the an absolute file name using the schematic path
	// currDir = schematicFileInfo.absolutePath() + 
	//        QDir::separator() +
        //        currFileInfo.fileName();
      } else { // no absolute paths around
	// use the WorkDir path
	currDir = QucsSettings.QucsWorkDir.path() + 
	          QDir::separator() +
	  currFileInfo.fileName();
      }
    } else { // current file name is absolute
      // use it
      currDir = currFileName;
    }
  } else { // a file name is not defined
    if (!schematicFileName.isEmpty()) { // if schematic has a filename
      // use the schematic absolute path
      currDir = schematicFileInfo.absolutePath();
    } else { // no absolute paths around
      // use the WorkDir path
      currDir = QucsSettings.QucsWorkDir.path();
    }
  }
  
  QString s = QFileDialog::getOpenFileName (
          this,
          tr("Select a file"),
          currDir,
          tr("All Files")+" (*.*);;"
            + tr("Touchstone files") + " (*.s?p);;"
            + tr("CSV files") + " (*.csv);;"
            + tr("SPICE files") + " (*.cir *.spi);;"
            + tr("VHDL files") + " (*.vhdl *.vhd);;"
            + tr("Verilog files")+" (*.v)"  );

  if(!s.isEmpty()) {
    // snip path if file in current directory
    QFileInfo file(s);
    if(QucsSettings.QucsWorkDir.exists(file.fileName()) &&
       QucsSettings.QucsWorkDir.absolutePath() == file.absolutePath()) s = file.fileName();
    edit->setText(s);
  }
  /* FIX
  propTable->currentIndex()->setText(1, s); */
}
/*--------------------------------------------------------------------------*/
void ComponentDialog::slotEditFile()
{
  schematic()->_app->editFile(QucsSettings.QucsWorkDir.filePath(edit->text()));
}
/*--------------------------------------------------------------------------*/

/*!
 * \brief ComponentDialog::uniquePropertyName
 * Returns the row of a property named \p name.
 * If \p name was not found, -1 is returned
 * \param name
 * \return
 */
bool ComponentDialog::propertyRow(const QString name) const
{
    for(int row=0; row < propTable->rowCount(); row++) {
      QString n  = propTable->item(row, PropTableColumns::name)->text();
      if( n == name) {
        return row;
      }
    }
    return -1;
}

/*!
 * \brief ComponentDialog::uniquePropertyName
 * Iterates over all properties and returns a new property name with
 * a prefix \p prefix
 * \param prefix
 * \return
 */
QString ComponentDialog::uniquePropertyName(const QString prefix) const
{
    int postfix = 1;
    for(int row=0; row < propTable->rowCount(); row++) {
      QString n  = propTable->item(row, PropTableColumns::name)->text();
      if (prefix + QString::number(postfix) == n)
          postfix++;
    }

    return prefix + QString::number(postfix);
}

/*!
  Add description if missing.
  Is called when the add button is pressed.

  Behavior:
   If Name already exists, set it to focus
   If new name, insert item after selected, set it to focus

*/
void ComponentDialog::createNewProperty()
{
  QString propertyName = uniquePropertyName("NewProperty");

  // by default the property is not displayed (TODO: in the schematic?)
  QString s = tr("no");

  // The new row will be added below the select row
  int curRow = propTable->currentRow();
  int insRow = curRow+1;
  propTable->insertRow(insRow);

  // append new row
  QTableWidgetItem *cell;
  cell = new QTableWidgetItem(propertyName);
  cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);
  propTable->setItem(insRow, PropTableColumns::name, cell);
  cell = new QTableWidgetItem("");
  cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);
  propTable->setItem(insRow, PropTableColumns::value, cell);
  cell = new QTableWidgetItem(s);
  cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);
  propTable->setItem(insRow, PropTableColumns::show, cell);
  // no description? add empty cell
  cell = new QTableWidgetItem("");
  cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);
  propTable->setItem(insRow, PropTableColumns::desc, cell);

  // select new row
  propTable->selectRow(insRow);
  slotSelectProperty(propTable->selectedItems()[0]);
}
/*--------------------------------------------------------------------------*/
// only eqn?
void ComponentDialog::slotButtRem()
{
  if(propTable->rowCount() < 3)
    return;  // the last property cannot be removed

  QTableWidgetItem *item = propTable->selectedItems()[0];
  int row = item->row();

  if(item == 0)
    return;

  // peek next, delete current, set next current
  if ( row < propTable->rowCount()) {
    propTable->setCurrentItem(propTable->item(row+1,PropTableColumns::name));
    slotSelectProperty(propTable->item(row+1,PropTableColumns::name));
    propTable->removeRow(row);
    }
}

/*--------------------------------------------------------------------------*/
// EQN only
void ComponentDialog::slotButtUp()
{
  qDebug() << "slotButtUp" << propTable->currentRow() << propTable->rowCount();

  int curRow = propTable->currentRow();
  if (curRow == 0)
    return;

  // swap current and row above it
  QTableWidgetItem *source = propTable->takeItem(curRow  ,PropTableColumns::name);
  QTableWidgetItem *target = propTable->takeItem(curRow-1,PropTableColumns::name);
  propTable->setItem(curRow-1, PropTableColumns::name, source);
  propTable->setItem(curRow, PropTableColumns::name, target);
  source = propTable->takeItem(curRow  ,PropTableColumns::value);
  target = propTable->takeItem(curRow-1,PropTableColumns::value);
  propTable->setItem(curRow-1, PropTableColumns::value, source);
  propTable->setItem(curRow, PropTableColumns::value, target);


  // select moved row
  propTable->selectRow(curRow-1);
}
/*--------------------------------------------------------------------------*/
// EQN only
// TODO: move to equation dialog
void ComponentDialog::slotButtDown()
{
  qDebug() << "slotButtDown" << propTable->currentRow() << propTable->rowCount();

  int curRow = propTable->currentRow();
  // Leave Export as last
  if (curRow == propTable->rowCount()-2)
    return;

  // swap current and row below it
  QTableWidgetItem *source = propTable->takeItem(curRow,PropTableColumns::name);
  QTableWidgetItem *target = propTable->takeItem(curRow+1,PropTableColumns::name);
  propTable->setItem(curRow+1, PropTableColumns::name, source);
  propTable->setItem(curRow, PropTableColumns::name, target);
  source = propTable->takeItem(curRow, PropTableColumns::value);
  target = propTable->takeItem(curRow+1, PropTableColumns::value);
  propTable->setItem(curRow+1, PropTableColumns::value, source);
  propTable->setItem(curRow, PropTableColumns::value, target);

  // select moved row
  propTable->selectRow(curRow+1);
}
/*--------------------------------------------------------------------------*/
void ComponentDialog::slotParamEntered()
{
  if(editValues->isEnabled()){ untested();
    editValues->setFocus();
  }else{ untested();
    //editStart->setFocus();
  }
}
/*--------------------------------------------------------------------------*/
void ComponentDialog::slotValuesEntered()
{
  slotButtOK();
}
/*--------------------------------------------------------------------------*/
void ComponentDialog::slotNumberEntered()
{
  slotButtOK();
}
/*--------------------------------------------------------------------------*/
void ComponentDialog::slotHHeaderClicked(int headerIdx)
{
  if (headerIdx != PropTableColumns::show)
      return; // clicked on header other than 'display'

  QString s;
  QTableWidgetItem *cell;

  if (setAllVisible) {
    s = tr("yes");
    disp->setChecked(true);
  } else {
    s = tr("no");
    disp->setChecked(false);
  }

  // go through all the properties table and set the visibility cell
  for (int row = 0; row < propTable->rowCount(); row++) {
    cell = propTable->item(row, PropTableColumns::show);
    cell->setText(s);
  }
  setAllVisible = not setAllVisible; // toggle visibility for the next double-click
}
/*--------------------------------------------------------------------------*/
void ComponentDialog::disableButtons()
{
  ButtUp->setEnabled(false);
  ButtDown->setEnabled(false);
}
/*--------------------------------------------------------------------------*/
void ComponentDialog::enableButtons()
{
  ButtUp->setEnabled(true);
  ButtDown->setEnabled(true);
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
