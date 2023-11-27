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
    size_t count;
} DbgStepCmd;

typedef struct {
    char *command;
} DbgSystemCmd;

typedef struct {
    DbgAct action;
    union {
        DbgStepCmd step;
        DbgSystemCmd system;
    };
} DbgCmd;

DbgCmd dbg_parse_cmd(Debugger *dbg);

#endif // DEBUG_PARSER_H_INCLUDED
