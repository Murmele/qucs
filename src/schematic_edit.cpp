/***************************************************************************
    copyright            : (C) 2020 Felix Salfelder

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/*--------------------------------------------------------------------------*/
#include "schematic_edit.h"
#include "platform.h"
#include "symbol.h"
#include "node.h"
#include "schematic_scene.h"
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*!
 * \brief collectPorts
 * Finds all port positions of an element and adds them to the vector \p p
 * \param e Element for which the ports are collected
 * \param p Position vector where all positions are stored
 */
static void collectPorts(ElementGraphics const* e, std::vector<pos_t>& p)
{
	if(auto s=dynamic_cast<Symbol const*>(element(e))){

		for(unsigned i=0; i<s->numPorts(); ++i){
			// if s->isConnected(i) ...
			auto pp = s->nodePosition(i);
			p.push_back(pp);
		}
	}else{
	}
}
/*--------------------------------------------------------------------------*/
struct{
	bool operator ()(ElementGraphics* a, ElementGraphics* b) const{
		return intptr_t(a) < intptr_t(b);
	}
}lt;
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
template<class T>
void SchematicEdit::save(T& del, T& ins)
{
	trace2("save", del.size(), ins.size());
	// return;
	assert(!_ins.size());
	assert(!_del.size());
	std::sort(ins.begin(), ins.end(), lt);
	std::sort(del.begin(), del.end(), lt);
	std::unique(ins.begin(), ins.end(), lt);
	std::unique(del.begin(), del.end(), lt);

	auto ii = ins.begin();
	auto di = del.begin();

	while(ii != ins.end() && di != del.end()){
		ElementGraphics* i = *ii;
		ElementGraphics* d = *di;
		// trace2("save", i, d);

		if(i==d){itested();
			if(i->isVisible()){
				// has been added eventually
			}else{
				// has been deleted eventually, but added before.
				// -> temporary? no it could have been in the initial insert q.
//				delete i; // crash.
			};
			++ii;
			++di;
		}else if(i<d){itested();
			if(i->isVisible()){
				delete i;
			}else{
				_ins.push_back(i);
			}
			++ii;
		}else{itested();
			if(d->isVisible()){
				_del.push_back(d);
			}else{
				delete d;
			}
			++di;
		}
	}

	_ins.insert(_ins.end(), ii, ins.end());
	_del.insert(_del.end(), di, del.end());

	trace2("saved", _del.size(), _ins.size());
}
/*--------------------------------------------------------------------------*/
// Perform an edit action for the first time. keep track of induced changes.
// This is a generic version of legacy implementation, and it still requires a
// scene implementing the geometry.
void SchematicEdit::do_it_first()
{itested();

	std::vector<ElementGraphics*> done_ins;
	std::vector<ElementGraphics*> done_del;
	std::vector<pos_t> pl;

	// remove ports and join adjacent wires. keep track.
	trace1("============ edit delete...", _del.size());
	while(_del.size()){itested();
		auto r = _del.front();
		trace1("remove", r);
		_del.pop_front();
		collectPorts(r, pl);
		r->hide();
		// queued delete.
		done_del.push_back(r);
	}
	// sort pl?
	// unique pl?

	for(auto portremove : pl){itested();
		// trace1("postremove", portremove);
		postRmPort(portremove, done_del);
	}

	trace1("============ edit insert...", _ins.size());
    while(_ins.size()){ // iterate over all new elements
		ElementGraphics* gfx = _ins.front();
		trace1("try insert...", element(gfx)->label());
		_ins.pop_front();

		Element* e = element(gfx);
		assert(e);
		SchematicScene* scn = gfx->scene();
		assert(scn);
        e->setLabel(scn->uniqueName(e->label()));
		if(addmerge(gfx, done_del)){
			trace0("merged");
		}else{
			trace2("done insert, show", e->label(), e->mutable_owner());
			// scn->possiblyRename(e); here?
			// scn->attachToModel(e); part of gfx->show()
			gfx->show();
			gfx->setSelected(true); // really?
			done_ins.push_back(gfx);
		}
	}

	save(done_ins, done_del);
} // do_it_first
/*--------------------------------------------------------------------------*/
Node const* SchematicEdit::nodeAt(pos_t const& p) const
{
	assert(scene());
	return scene()->nodeAt(p);
}
/*--------------------------------------------------------------------------*/
QList<ElementGraphics*> SchematicEdit::items(
           const QPointF &pos, Qt::ItemSelectionMode mode,
           Qt::SortOrder order) const
{
	return _scn.items(pos, mode, order);
}
/*--------------------------------------------------------------------------*/
/*!
 * \brief SchematicEdit::items
 * Returns a list of ElementGraphics within the rect \p r
 * \param r
 * \return
 */
QList<ElementGraphics*> SchematicEdit::items(QRectF const& r) const
{ untested();
	return _scn.items(r);
}
/*--------------------------------------------------------------------------*/
// merge new item into existing.
// return false if new item does not interfere with existing.
template<class T>
bool SchematicEdit::addmerge(ElementGraphics* new_elt, T& del_done)
{
	bool collision = false;
	assert(!new_elt->isVisible());
	assert(new_elt->scene());
	QRectF bb = new_elt->absoluteBoundingRect();
	trace2("addmerge candidates??", bb.topLeft(), bb.bottomRight());
	auto it = items(bb);
	trace3("addmerge candidates", new_elt, element(new_elt)->label(), it.size());
	for(auto gfxi : it){itested();
		assert(gfxi!=new_elt); // new_elt is invisible.
		trace1("addmerge coll?", element(gfxi)->label());

		// gfxi is already on scene.
		auto n = gfxi->newUnion(new_elt);
		if(n){ untested();
			collision = true;
			trace1("hiding", element(gfxi)->label());

			assert(element(gfxi)->mutable_owner());
			gfxi->hide();
			assert(!element(gfxi)->mutable_owner());

			// collision delete.
			del_done.push_back(gfxi);
			auto nc = n->childItems();
			trace1("got union", nc.size());

			size_t kk=0;

			// unpack q insert.
			for(auto c : nc){itested();
				if(auto g = dynamic_cast<ElementGraphics*>(c)){
					auto cc = g->clone();
					assert(cc);
					assert(!cc->isVisible());
					_scn.addItem(cc);
					assert(!cc->isVisible());
					assert(!element(cc)->scope());
					c->setParentItem(nullptr);
					_ins.push_front(cc); // BUG // need a different q for derived objects
					++kk;
				}else{
					trace0("not useful");
					// text, maybe
					// unreachable();
				}
			}
			if(kk){
				// found something useful
				trace2("unpacked", kk, nc.size());
				delete n; // does it delete them all?
			}else{
				_ins.push_front(n);
			}
			break;
		}else{
			trace0("no union");
		}
	}

	trace1("done addmerge", collision);
	return collision;
}
/*--------------------------------------------------------------------------*/
// remove stuff adjacent to degree-2 nodes. add back later.
template<class T>
void SchematicEdit::postRmPort(pos_t remove_at, T& del_done)
{itested();
	auto it = items(makeQPointF(remove_at));
	Node const* node = nodeAt(remove_at);

	if(!node){
		// trace1("no node at", remove_at);
	}else if(node->degree() == 2){
		// trace1("postrm, degree 2", remove_at);
		for(auto gfx : it){
			gfx->hide();
			// additional delete
			del_done.push_back(gfx);
			_ins.push_front(gfx); // hmm.
			trace1("queued", gfx);
		}
	}else{
	}
}
/*--------------------------------------------------------------------------*/
void SchematicEdit::do_it()
{
	trace2("do_it", _del.size(), _ins.size());
	for(auto& d : _del){itested();
		trace1("hide", d);
		assert(d->isVisible());
		d->hide();
	}
	for(auto& d : _ins){itested();
		trace1("show", d);
		assert(!d->isVisible());
		//d->update(); // really?
		d->show();
		d->setSelected(true); // BUG. this will select too many.
	}
	std::swap(_ins, _del);
}
/*--------------------------------------------------------------------------*/
void SchematicEdit::qDelete(ElementGraphics* gfx)
{
	assert(gfx->isVisible());
	_del.push_back(gfx);
}
/*--------------------------------------------------------------------------*/
/*!
 * \brief SchematicEdit::qInsert
 * Inserts a new ElementGraphics \p gfx to this undo command.
 * On these ElementGraphics the redo and undo will be
 * executed
 * \param gfx
 */
void SchematicEdit::qInsert(ElementGraphics* gfx)
{
	assert(!gfx->isVisible());
	assert(gfx->scene() == &_scn); // wrong?
	_ins.push_back(gfx);
}
/*--------------------------------------------------------------------------*/
// turn swap into add/delete
void SchematicEdit::qSwap(ElementGraphics* gfx, Element* e)
{
	assert(!e->mutable_owner());

	auto ng = new ElementGraphics(e);
	{// ?
		assert(!element(ng)->scope());
		_scn.addItem(ng);
		ng->hide(); // what??
		// e->setOwner(element(gfx)->mutable_owner()); // not here.
		// assert(e->mutable_owner());
		//	assert(element(ng)->scope());
	}

	qDelete(gfx);
	qInsert(ng);
}
/*--------------------------------------------------------------------------*/
void SchematicEdit::redo()
{itested();
	if(_first){
		do_it_first();
		_first = false;
	}else{
		do_it();
	}
}
/*--------------------------------------------------------------------------*/
