/*
 * sp2deembed.cpp - two-port S-parameters de-embedding component
 *
 * Copyright (C) 2017 Qucs Team
 * based on sparamfile.h, (C) 2003 by Michael Margraf
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this package; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef SP2DEEMBED_H
#define SP2DEEMBED_H

#include "component.h"


class SP2DeEmbed : public Component  {
public:
  SP2DeEmbed();
 ~SP2DeEmbed() {};
  Component* newOne();
  static Element* info(QString&, char* &, bool getNewOne=false);

  QString getSubcircuitFile();

protected:
  QString netlist();
  void createSymbol();
};

#endif /* SP2DEEMBED_H */
