#ifndef SCOPE_H
#define SCOPE_H
#include "AST.h"

typedef struct SCOPE_STRUCT
{
    AST_T** variable_definitions;
    size_t variable_definitions_size;

    AST_T** func_definitions;
    size_t func_definitions_size;
} scope_T;

scope_T* init_scope();

AST_T* scope_add_func_definition(scope_T* scope, AST_T* fdef);

AST_T* scope_get_func_definition(scope_T* scope, const char* name);

AST_T* scope_find_func(scope_T* scope, const char* name);

AST_T* scope_add_variable_definition(scope_T* scope, AST_T* vdef);

AST_T* scope_change_variable_definition(scope_T* scope, const char* name, const char* fname, AST_T* value);

AST_T* scope_get_variable_definition(scope_T* scope, const char* name, const char* fname);

#endif