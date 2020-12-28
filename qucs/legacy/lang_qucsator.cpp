/***************************************************************************
    copyright            : (C) 2015, 2019, 2020 Felix Salfelder
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// qucsator "language".

#include "sckt_base.h"
#include "net.h"
#include "docfmt.h" // <<
#include "globals.h"
#include "settings.h" //??
#include "schematic_doc.h"
//#include "qucsator.h"
//#include "dat.h"
#include "painting.h"
#include "components/component.h" // yikes
/* -------------------------------------------------------------------------------- */
namespace {
/* -------------------------------------------------------------------------------- */
static const std::string typesep(":");
static const char _typesep = ':';

// not sure what this is about
/// #ifdef __MINGW32__ // -> platform.h
/// #include <windows.h>
/// static QString pathName(QString longpath) {
///   const char * lpath = QDir::toNativeSeparators(longpath).toAscii();
///   char spath[2048];
///   int len = GetShortPathNameA(lpath,spath,sizeof(spath)-1);
///   spath[len] = '\0';
///   return QString(spath);
/// }
/// #else
/// static QString pathName(QString longpath) {
///   return longpath;
/// }
/// #endif
/* -------------------------------------------------------------------------------- */
// temporary kludge.
class QucsatorScktHack : public Symbol {
private:
	QucsatorScktHack(QucsatorScktHack const&) = default;
public:
	QucsatorScktHack() : Symbol() {}
private:
	Element* clone() const override{return new QucsatorScktHack(*this);}
private: // Symbol
	pos_t portPosition(unsigned) const {unreachable(); return pos_t(0,0);}
	unsigned numPorts() const  override{ return 0;}
	Port& port(unsigned) override{unreachable(); return *new Port();}
	void setParameter(std::string const& name, std::string const& value){
		if(name == "qucsatorsckthack"){
			_text = value;
		}else{ untested();
			Symbol::setParameter(name, value);
		}
	}
	std::string paramValue(std::string const& name) const override{
		if(name == "qucsatorsckthack"){
			return _text;
		}else{ untested();
			return Symbol::paramValue(name);
		}
	}

private:
	std::string _text;
}d0;
static Dispatcher<Symbol>::INSTALL p0(&symbol_dispatcher, "qucsatorScktHack", &d0);
/* -------------------------------------------------------------------------------- */
static std::string netLabel(Node const* nn)
{
	if(!nn){
		return "(null)";
	}else{

	}
	Net const* n = nn->net();

	if(!n){ untested();
		unreachable();
		return "(null)";
	}else if(n->hasLabel()){
		return n->label();
	}else{
		return "_net" + std::to_string(n->pos());
	}
}
/* -------------------------------------------------------------------------------- */
static int notalnum(char c)
{
	return !std::isalnum(c);
}
/* -------------------------------------------------------------------------------- */
static std::string mangleType(std::string& t)
{
	if(t == "_BJT"){
		// for some reason...
		t = "BJT";
	}else{
	}
	trace1("mangle", t);
	auto pos = t.find(typesep);
	std::string ret="";
	if(pos == std::string::npos){
	}else{
		auto sub = t.substr(pos+1);
		std::replace_if(sub.begin(), sub.end(), notalnum, '_');
		ret = " Type=\"" + sub + "\"";
	}
	t = t.substr(0, pos);
	if(t=="Lib"){
		t="Sub";
	}else{
	}
	return ret;
}
/* -------------------------------------------------------------------------------- */
// qucslang language implementation
class QucsatorLang : public NetLang {
private: // NetLang
  // inline void printItem(Element const* c, stream_t& s) const;
  std::string findType(istream_t&) const override {incomplete(); return "incomplete";}

private: // local
  void printTaskElement(TaskElement const*, ostream_t&) const; // override?
  void printSymbol(Symbol const*, ostream_t&) const override;
  void printSubckt(SubcktBase const*, ostream_t&) const;
  void printComponent(Component const*, ostream_t&) const;
  void printPainting(Painting const*, ostream_t&) const override {incomplete();}
  void printDiagram(Symbol const*, ostream_t&) const override {incomplete();}
}qucslang;
static Dispatcher<DocumentFormat>::INSTALL p(&language_dispatcher, "qucsator", &qucslang);
/* -------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------- */
// non-callback print.
static void printSymbol_(Symbol const* c, ostream_t& s)
{
	// todo: mfactor/active?
	assert(c);
	trace2("pc", c->label(), c->typeName());

	{
		std::string type = c->typeName();
		std::string hack_type = mangleType(type);

		s << type << ":" << c->label();

		Symbol const* sym=c;
		trace3("print", c->label(), sym->numPorts(), sym->label());
		for(unsigned i=0; i<sym->numPorts(); ++i){
			std::string N = netLabel(sym->portValue(i));
			s << " " << N;
		}

		for(unsigned ii=0; ii<sym->paramCount(); ++ii) {
			trace3("param", c->label(), ii, sym->paramCount());
			std::string name = sym->paramName(ii);
			//trace2("param", name, value);

			if(name.at(0)=='$'){
				// hmmm
//			}else if(!sym->paramIsPrintable(ii)){ untested();
			}else if(name==""){itested();
				incomplete();
			}else if(name == "Component") {
				// hack
			}else if(name == "File") { untested();
				// hack
			}else if(name == "Lib") {
				// hack
			}else if(name == "Symbol") { untested();
				// hack??
			}else{
				std::string value = sym->paramValue(ii);
				s << " " << name << "=\"" << value << "\"";
			}
		}

		s << hack_type;
		s << '\n';
	}
}
/* -------------------------------------------------------------------------------- */
void QucsatorLang::printSymbol(Symbol const* d, ostream_t& s) const
{
	// is_device??
	if(!d){ untested();
		incomplete();
	}else if(auto c=dynamic_cast<SubcktBase const*>(d)){
		if(c->is_device()){
			// here?
		   s << "# got sckt device " << d->label() << "\n";
			printSubckt(c, s);
		}else{
		   s << "# skip non-device " << d->label() << "\n";
		}
//	}else if(auto c=dynamic_cast<SubcktProto const*>(d)){ untested();
//		// why is this a Symbol??
//		printSubckt(c, s);
	}else if(auto c=dynamic_cast<TaskElement const*>(d)){ untested();
		// why is this a Symbol??
		printTaskElement(c, s);
	}else if(auto c=dynamic_cast<Component const*>(d)){
		// HACK
		printComponent(c, s);
	}else if(dynamic_cast<Conductor const*>(d)){
		// possibly a wire.
	}else if(d){
		// TODO. all symbols here.
		printSymbol_(d, s);
	}else{ untested();
		assert(false);
		incomplete();
	}
}
/* -------------------------------------------------------------------------------- */
static void printDefHack(Symbol const* p, ostream_t& s)
{
	std::string hack = p->paramValue("qucsatorsckthack");
	s << hack;
}
/* -------------------------------------------------------------------------------- */
// partly from Schematic::createSubnetlistplain
void QucsatorLang::printSubckt(SubcktBase const* p, ostream_t& s) const
{
	s << "# sckt " + p->label() + "\n";
	if(p->label()[0] == ':'){
		return;
	}else{
	}
//	assert(!p->is_device());
	Symbol const* sym = p;
	SchematicModel const* sckt;
	if(p->makes_own_scope()){
		s << "# sckt own " + p->label() + "\n";
		sckt = p->scope();
	}else{
		s << "# sckt sckt " + p->label() + "\n";
		sckt = p->subckt();
	}
	assert(sckt);
	std::string label = p->label();

	auto h = sckt->find_(":qucsatorsckthack:");
	if(h == sckt->end()){
	}else if(auto p = dynamic_cast<Symbol const*>(*h)){
		return printDefHack(p, s);
	}else{
	}
	h = sckt->find_("main");
	if(h == sckt->end()){
	}else{
		sckt = (*h)->scope();
		sym = prechecked_cast<Symbol const*>(*h);
		assert(sym);
	}

	s << "\n"; //?
	if(label.c_str()[3] == _typesep){
		s << ".Def:" << label.substr(4);
	}else{
		incomplete();
	}

	{ // print_ports();
		for(unsigned i=0; sym->portExists(i); ++i){
			std::string N = netLabel(sym->portValue(i));
			s << " " << N;
		}
		s << "\n";
	}

	// somehow parameters are stashed as paintings.
	// let's see.
	if(!p->symbolPaintings()){
		s << "# Missing ID & params " << p->label() << "\n";
	}else{
		for(auto pi : *p->symbolPaintings()){
			incomplete();
#if 0
			if(pi->name() == ".ID ") {
				incomplete();
				s << "# TODO ID & params" << pi->label() << pi->name() << "\n";
		//		ID_Text *pid = (ID_Text*)pi;
		//		QList<SubParameter *>::const_iterator it;
		//		for(it = pid->Parameter.constBegin(); it != pid->Parameter.constEnd(); it++) { untested();
		//			s = (*it)->Name; // keep 'Name' unchanged
		//			(*tstream) << " " << s.replace("=", "=\"") << '"';
		//			}
			}else{
			}
#endif
		//		break;
		}
	}
	//(*tstream) << '\n';
	//
	s << "# " << sckt->size() << "\n";
	for(auto it_ : *sckt){
		auto i = dynamic_cast<Symbol const*>(it_);
		if(it_){
		}else{
			incomplete();
			continue;
		}

      if(!i){
			incomplete();
		}else if(i->typeName() == "Port"){
		}else if(i->typeName() == "Wire"){ // is Conductor?
		}else if(i->typeName() == "GND"){
		}else{
			// s << "# ps" << i->typeName() << " " << i->label() << "\n";
			printSymbol(i, s);
		}
	}

	s << ".Def:End\n"; //  << p->label() << "\n";
}

void QucsatorLang::printTaskElement(TaskElement const* c, ostream_t& s) const
{
	assert(c);
	s << "." << c->typeName() << ":" << c->label();

	//for(auto p2 : c->params())
	for(auto p2 : c->Props){ // BUG
		if(p2->name() == "Symbol") { // hack??
		}else if(p2->name()=="p" && p2->value()==""){itested();
			// unreachable
		}else{
			s << " " << p2->name() << "=\"" << p2->value() << "\"";
		}
	}
	s << '\n';
}

// print Component in qucsator language
// BUG: callback: untangle isShort, isActive (mfactor?).
void QucsatorLang::printComponent(Component const* c, ostream_t& s) const
{
	if(c->isActive != COMP_IS_ACTIVE){
		// comment out?
		incomplete();
	}else{
	}
	assert(c);
	trace2("pc", c->label(), c->typeName());

	if(c->isOpen()) {
		// nothing.
	}else if(c->isShort()){ untested();
		// replace by some resistors (hack?)
		incomplete();
		int z=0;
		QListIterator<ComponentPort*> iport(c->ports());
		iport.next(); // BUG
		unsigned k=0;
		std::string Node1 = netLabel(c->portValue(k));
		while (iport.hasNext()){ untested();
			++k;
			s << "R:" << c->label() << "." << QString::number(z++) << " "
				<< Node1 << " " << netLabel( c->portValue(k) ) << " R=\"0\"\n";
		}
	}else{
		std::string type = c->typeName();
		std::string hack_type = mangleType(type);

		s << type << ":" << c->label();

		Symbol const* sym=c;
		trace3("print", c->label(), sym->numPorts(), sym->label());
		for(unsigned i=0; i<sym->numPorts(); ++i){
			std::string N = netLabel(sym->portValue(i));

			s << " " << N;
		}

		for(unsigned ii=0; ii<sym->paramCount(); ++ii) {
			trace3("param", c->label(), ii, sym->paramCount());
			std::string name = sym->paramName(ii);
			//trace2("param", name, value);

			if(name.at(0)=='$'){
				// hmmm
//			}else if(!sym->paramIsPrintable(ii)){ untested();
			}else if(name==""){itested();
				incomplete();
			}else if(name == "File") {
				// hack
			}else if(name == "Symbol") {
				// hack??
			}else{
				std::string value = sym->paramValue(ii);
				s << " " << name << "=\"" << value << "\"";
			}
		}

		s << hack_type;
		s << '\n';
	}
}
/* -------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------- */

}//namespace
/* -------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------- */
