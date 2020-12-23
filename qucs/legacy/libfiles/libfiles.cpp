/**************************************************************************
    copyright            : 2020 Felix Salfelder
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// read components from legacy "lib" files.
// pieced together from various places.

#include "globals.h"
#include "module.h"
#include "schematic_lang.h"
#include "symbol.h"
#include "schematic_model.h" // tmp, debugging
#include "io_trace.h"
#include "qt_compat.h"
#include "docfmt.h"
#include "sckt_base.h"
#include <QList> // yikes.
#include "../qucs-lib/qucslib_common.h"

namespace{

class LIB{
public:
	LIB(){
		loadLibFiles();
	}
	~LIB(){
		incomplete();
#if 0
		for(auto i: stash){ untested();
			delete i->item;
			delete i;
		}
#endif
	}
private:
	void loadLibFiles();
	void stash(Dispatcher<Symbol>::INSTALL* i){
		_stash.push_back(i);
	}

	std::vector<Dispatcher<Symbol>::INSTALL*> _stash;

	std::vector<Module::INSTALL*> _mod;
}l;

void LIB::loadLibFiles()
{
    QList<QPair<QString, bool> > LibFiles;
    ComponentLibrary parsedlib;
    QString libPath;
//    libInfoStruct lineLibInfo;
//    QVariant v;

//    QPixmap userLibPixmap = QPixmap(":/bitmaps/home.png");
//    QPixmap sysLibPixmap = QPixmap(":/bitmaps/big.qucs.xpm");

    int UserLibCount = 0;

#if 0
    // user libraries
	 // here? or in QucsDoc?
    QStringList UserLibFiles = UserLibDir.entryList(QStringList("*.lib"), QDir::Files, QDir::Name);
    UserLibCount = UserLibFiles.count();
    foreach(QString s, UserLibFiles) // build list with full path
      LibFiles.append(qMakePair(UserLibDir.absoluteFilePath(s), false));
#endif

	 trace2("scanning", QucsSettings.libDir(), &QucsSettings);
	 QDir SysLibDir(QucsSettings.libDir());
    // system libraries
    QStringList SysLibFiles = SysLibDir.entryList(QStringList("*.lib"), QDir::Files, QDir::Name);
    foreach(QString s, SysLibFiles) {
		 // build list with relative path
      LibFiles.append(qMakePair(s, true));
	 }

    for (int i = 0; i < LibFiles.count(); ++i ) {
      libPath = LibFiles[i].first;
      libPath.chop(4); // remove extension
		parsedlib.components.clear();
      int result = parseComponentLibrary(libPath, parsedlib);

		trace3("got lib", parsedlib.name, libPath, parsedlib.components.size());
      switch (result) {
      case QUCS_COMP_LIB_IO_ERROR:
      { untested();
        QString filename = getLibAbsPath(LibFiles[i].first);
        error(3, "Cannot open \"%1\"." + filename.toStdString());
        return;
      }
      case QUCS_COMP_LIB_CORRUPT: untested();
        error(3, "Library is corrupt.");
        return;
      default:
        break;
      }

		// ComponentLibraryItem are not Elements, but just a concoction of QStrings.
		// turn it into Element... (do it here, for now, but that's silly).
		for(ComponentLibraryItem c : parsedlib){
			trace2("libcomp", parsedlib.name, c.name); // ->label());
			if(c.symbol==""){
				getSection("Symbol", c.definition, c.symbol); // GAAH
			}else{ untested();
			}

			if(c.symbol==""){
				c.definition += "\n<Symbol>" + parsedlib.defaultSymbol + "\n</Symbol>\n";
				trace1("attached symbol to defn", c.definition);
			}else{
			}

			auto D = language_dispatcher["legacy_lib"];
			auto L_ = dynamic_cast<SchematicLanguage const*>(D);
			auto C = command_dispatcher["leg_sch"];
			auto L = dynamic_cast<DocumentFormat const*>(C);
			assert(L);

			istream_t stream(istream_t::_STRING, c.modelString.toStdString());
			stream.read_line();
			std::string type = L_->findType(stream); // BUG?

			if(type=="Lib"){
				// possibly a subcircuit. parse and stash
				//
				// // stuff should already be parsed in, but isn't
				// BUG: parse c.definition. but not here.
				istream_t stream(istream_t::_STRING, c.definition.toStdString());
				Symbol* sym = symbol_dispatcher.clone("LegacyLibProto");
				auto ssym = prechecked_cast<SubcktBase*>(sym);
				std::string t = "Lib:" + parsedlib.name.toStdString() + ":" + c.name.toStdString();

				sym->setLabel(c.name.toStdString());

				assert(ssym);
				try{
					// trace1("parse", c.definition);
					L->load(stream, ssym);

					trace1("Lib stashing", t);
					stash(new Dispatcher<Symbol>::INSTALL(&symbol_dispatcher, t, ssym));
					new Module::INSTALL(parsedlib.name.toStdString(), ssym);
				}catch(Exception const&){ untested();
					trace0("not stashing");
					delete ssym;
				}

				// trace3("Lib", c.modelString, type, c.definition);
				// d'uh
			}else{
				// TODO: new__instance does this.
				Symbol* sym = symbol_dispatcher.clone("LegacyParamset");
				sym->setParameter("modelstring", c.modelString.toStdString());
				std::string t = "P:" + parsedlib.name.toStdString() + ":" + c.name.toStdString();
				sym->setLabel(t);

				if(symbol_dispatcher[type]){
					sym->setTypeName(type);
					L->load(stream, sym);
					new Module::INSTALL(parsedlib.name.toStdString(), sym);
				}else{
					trace1("paramset skip", type);
					// unreachable(); eventually
					// possibly not ported yet.
				}
			}
			// todo: memory leak.
		}

#if 0
      lineLibInfo = libInfoStruct{libPath};
      v.setValue(lineLibInfo);
      if (LibFiles[i].second) { // it's a system library ?
        Library->addItem(sysLibPixmap, parsedlib.name, v);
      } else { untested();
        Library->addItem(userLibPixmap, parsedlib.name, v);
      }
#endif
    }
    if (UserLibCount > 0) { untested();
      // add a separator to distinguish between user libraries and system libs
      // Library->insertSeparator(UserLibCount);
    }else{
	 }
    // slotSelectLibrary(0);
}

}
