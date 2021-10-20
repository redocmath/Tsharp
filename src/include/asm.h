#ifndef ASM_H
#define ASM_H
#include "AST.h"

typedef struct ASM_STRUCT
{

} assembly_T;

assembly_T* init_assembly();

int init_asm_file(char* fileName);

char* add_to_main_section(char* contents);

char* add_to_data_section(char* contents);

AST_T* assembly_visit(assembly_T* assembly, AST_T* node, char* operation);

AST_T* assembly_visit_function_call(assembly_T* assembly, AST_T* node);

AST_T* assembly_visit_variable_definition(assembly_T* assembly, AST_T* node);

AST_T* assembly_visit_function_definition(assembly_T* assembly, AST_T* node);

AST_T* assembly_visit_string(assembly_T* assembly, AST_T* node, char* operation);

AST_T* assembly_visit_variable(assembly_T* assembly, AST_T* node, char* operation);

AST_T* assembly_visit_compound(assembly_T* assembly, AST_T* node);

#endif