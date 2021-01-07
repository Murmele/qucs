
#ifndef QUCS_L_LIB
#define QUCS_L_LIB
/*--------------------------------------------------------------------------*/
#include <string>
#include "platform.h"
#include "io_trace.h"
#include "io_error.h"
#include <cstring> // strdup?
#include <assert.h>
/*--------------------------------------------------------------------------*/
void error(int,const char*,...);
void error(int,const std::string&);
std::string to_string(int);
/*--------------------------------------------------------------------------*/
// temporary stuff.
namespace OS{
  inline std::string getenv(const std::string& s) {
    char* ev = ::getenv(s.c_str());
    if (ev) {
      return ev;
    }else{itested();
      return "";
    }
  }
  inline void setenv(const std::string& Name, const std::string& Value, bool Overwrite=true) {
    if (Name == "") {untested();
      throw Exception("setenv: bad name " + Name);
    }else if (Value == "") {untested();
      throw Exception("setenv: bad value " + Value);
    }else if (!Overwrite && getenv(Name) != "") {
      error(bDEBUG, "setenv: " + Name + " overwrite prohibited");
    }else{
      std::string ev = Name + "=" + Value;
      char *es = ::strdup(ev.c_str());	//BUG// memory leak
      assert(es);
      if (::putenv(es) != 0) {untested();
	throw Exception("");
      }else{
      }
    }
  }
}
#endif
