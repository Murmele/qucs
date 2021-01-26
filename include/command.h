#ifndef QUCS_COMMAND_H
#define QUCS_COMMAND_H

#include "object.h"
#include "schematic_model.h"

#define INTERFACE
#define CARD_LIST SchematicModel // really?
#define CS istream_t

#include "c_comand.h"
#undef CARD_LIST
#undef CS

#endif
