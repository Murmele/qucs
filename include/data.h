/***************************************************************************
    copyright            : (C) 2015, 2020 by Felix Salfelder
    email                : felix@salfelder.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/* -------------------------------------------------------------------------------- */
#ifndef QUCS_SIM_DATA_H__
#define QUCS_SIM_DATA_H__
#include <assert.h>
#include <complex>
#include <list>
#include <QDateTime>
#include "element.h"
/* -------------------------------------------------------------------------------- */

/*!
 * \brief The CommonData class
 */
class CommonData : public Object{
private:
	CommonData(CommonData const&) = delete;
public:
        explicit CommonData() : Object() {}
	virtual ~CommonData();
public:
	virtual CommonData* clone() { return NULL; }

public:
	virtual bool  operator==(const CommonData&x)const;
	bool operator!=(const CommonData& x)const {return !(*this == x);}

protected:
	virtual CommonData* resolve(const std::string&){assert(false); return nullptr;}

public:
	static void attach(CommonData*, CommonData**);
	static void detach(CommonData**);
	virtual CommonData const* refresh(){unreachable(); return nullptr;}

private:
        unsigned _attach_count{0};
};



#if 0 // not yet?
// a sequence of doubles (with a name on it)
class SimOutputSeq : public SimOutputData{
public:
	class const_iterator{
	};

public:
	const_iterator begin() const;
	const_iterator end() const;
};

// a sequence of double pairs (with a name on it)
class SimOutputWave : public SimOutputData{
private:
	SimOutputSeq* dep{nullptr};
};
#endif

/* -------------------------------------------------------------------------------- */
class Data : public Element{
protected:
	explicit Data() : Element(), _common(nullptr){}
	~Data();
public:
	CommonData const* common()const{ return _common; }

private:
protected: // hmm
	CommonData* _common;
};
/* -------------------------------------------------------------------------------- */
inline bool CommonData::operator==(const CommonData&)const
{
	incomplete();
	return false;
}
/* -------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------- */
#endif
