/***************************************************************************
    copyright            : (C) 2016, 2020 Felix Salfelder
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUCS_OBJECT_H
#define QUCS_OBJECT_H
#include "io_trace.h"
#include "QUndoStack"
/*--------------------------------------------------------------------------*/
typedef unsigned index_t;
/*--------------------------------------------------------------------------*/

/*!
 * \brief The Object class
 * Base object of all objects in qucs.
 *
 * It holds the label of the object
 */
class Object{
public:
	enum QucsMsgType{
		QucsMsgTrace=0,
		QucsMsgLog=1,
		QucsMsgDebug=2,
		QucsMsgWarning=3,
		QucsMsgCritical=4,
		QucsMsgFatal=5
	};
protected:
	explicit Object(){}
	explicit Object(Object const&){}

public:
        virtual ~Object(){ delete mUndoStack; mUndoStack = nullptr;}
        void setUndoStack(QUndoStack* stack) {mUndoStack = stack;}
        QUndoStack* undoStack() {return mUndoStack;}
        std::string const& label() const { return _label;}
        std::string const& short_label()const {return _label;} // TODO: should be different?
        void setLabel(std::string const& l);
        void setLabel(char const* l);

        /*!
         * \brief pushUndoStack
         * Push undo command to Undostack if available, if no
         * undostack is available, create a local one to execute
         * the undo command, but the no undo is available
         * \param cmd
         * \return true if the command was pushed successfully to the undostack,
         * false if the command \p cmd is not valid or no valid undostack was available.
         * Returns also false if the command was pushed to the local undostack
         */
        bool pushUndoStack(QUndoCommand* cmd);



protected: // error handling
	void message(QucsMsgType, const char*) const;
	virtual void message(QucsMsgType, std::string const&) const;

private:
	std::string _label;
        QUndoStack* mUndoStack{nullptr};

        friend QUndoCommand; // so the Undocommand is directly able to access all private members and must not call the getter and setter functions
};
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#endif
