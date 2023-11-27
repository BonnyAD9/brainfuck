#ifndef DEBUG_PARSER_H_INCLUDED
#define DEBUG_PARSER_H_INCLUDED

#include "debugger.h" // Debugger

typedef enum {
    DA_NONE,
    DA_HELP,
    DA_QUIT,
    DA_CLEAR_SCREEN,
    DA_LIST,
    DA_STEP,
    DA_SYSTEM,
} DbgAct;

typedef struct {
    DbgAct action;
} DbgCmd;

DbgCmd dbg_parse_cmd(Debugger *dbg);

#endif // DEBUG_PARSER_H_INCLUDED
