/***************************************************************************
                               markerdialog.h
                              ----------------
    begin                : Wed April 21 2004
    copyright            : (C) 2003 by Michael Margraf
    email                : michael.margraf@alumni.tu-berlin.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MARKERDIALOG_H
#define MARKERDIALOG_H
#include "schematic_dialog.h"
#include "marker.h"
#include <QDialog>

class QLineEdit;
class QComboBox;
class QCheckBox;

class MarkerDialog : public SchematicDialog{
public:
  ~MarkerDialog();
  MarkerDialog(QucsDoc*); // Marker *pm_, QWidget *parent=0);

	void attach(Object *pm_);
private slots:
  void slotAcceptValues();

public:
  Marker *pMarker{nullptr};

  QComboBox  *NumberBox{nullptr};
  QLineEdit  *Precision{nullptr};
  QLineEdit  *SourceImpedance{nullptr};
  QCheckBox  *TransBox{nullptr};
  QWidget *ZYSelectBox{nullptr};
  QCheckBox *ZCheckBox{nullptr}, *YCheckBox{nullptr}, *ZSCheckBox{nullptr}, *ZPCheckBox{nullptr};
};

#endif