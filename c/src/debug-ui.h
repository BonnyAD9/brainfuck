#ifndef DEBUG_UI_H_INCLUDED
#define DEBUG_UI_H_INCLUDED

#include "debugger.h" // Debugger

void dbg_ui_start(Debugger *dbg);

void dbg_ui_prompt(Debugger *dbg);

void dbg_ui_list_code(Debugger *dbg);

void dbg_ui_list_tape(Debugger *dbg);

void dbg_ui_help(Debugger *dbg);

#endif // DEBUG_UI_H_INCLUDED
