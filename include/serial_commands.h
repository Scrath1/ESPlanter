#ifndef SERIAL_COMMANDS_H
#define SERIAL_COMMANDS_H
#include "config_table.h"

#include "stint.h"

extern Stint stint;

void print_config(const ConfigTable_t& cfg_table);

#endif // SERIAL_COMMANDS_H