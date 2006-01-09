/***************************************************************************
                               qf_cauer.h
                             --------------
    begin                : Mon Jan 02 2006
    copyright            : (C) 2006 by Vincent Habchi, F5RCS
    email                : 10.50@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _QF_CAUER_H
#define _QF_CAUER_H

#define	_QF_CAUER_DEBUG

const double SN_ACC = 1e-5;	// Accuracy of sn (x) is SN_ACC²
const double K_ERR1 = 1e-8;	// Accuracy of K(k)
const double K_ERR2 = 1e-20;	// Accuracy of K(k)
const double K_ERR3 = 1e-6;	// Accuracy of K(k)

class qf_cauer : public qf_filter
{
 private:
  // Standard paramaters

  double rho;			// Reflection coeff.
  double th;			// Modular angle

  // Zeros of transmission

  double * a;

public:
  qf_cauer (unsigned, double, double);
  qf_cauer (double, double, double, double, double, double, qft);
  virtual ~qf_cauer (void);

  // Computes standard form
  void normalize (double, double, double, qft);

  void xfer (void);		// Computes xfer fctn
  void values (void);		// Computes norm values
  virtual void synth (qft);	// Standard -> Actual form
  void dump (void);		// Dumps to cout
};

#endif // _QF_CAUER_H
