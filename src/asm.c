#include "include/asm.h"
#include "include/scope.h"
#include "include/AST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ↓ change this later */
char mainfile[1000];
char datafile[1000];
char* contents;

static AST_T* builtin_function_print(visitor_T* visitor, AST_T** args, int args_size, char* name)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T* visited_ast = visitor_visit(visitor, args[i]);
        if (visited_ast->type == AST_STRING)
        {
            contents = "\nmov rax, 0x2000004\n"
                       "mov rdi, 1\n"
                       "mov rsi, ";
            strcat(mainfile, contents);
            strcat(mainfile, name);
            
            contents = "\nmov rdx, ";
            strcat(mainfile, contents);
            strcat(mainfile, name);

            contents = "len\n"
                       "\nsyscall\n";
            strcat(mainfile, contents);
        }
    }
    return init_ast(AST_NOOP);
}

visitor_T* init_visitor()
{
    // Main Section
    contents = "; Assembly Main Section\n";
    strcat(mainfile, contents);

    // Data Section
    contents = "section .data\n";
    strcat(datafile, contents);

    visitor_T* visitor = calloc(1, sizeof(struct VISITOR_STRUCT));

    return visitor;
}

AST_T* visitor_visit(visitor_T* visitor, AST_T* node)
{
    switch (node->type)
    {
        case AST_FUNCTION_DEFINITION: return visitor_visit_function_definition(visitor, node); break;
        case AST_VARIABLE_DEFINITION: return visitor_visit_variable_definition(visitor, node); break;
        case AST_FUNCTION_CALL: return visitor_visit_function_call(visitor, node); break;
        case AST_VARIABLE: return visitor_visit_variable(visitor, node); break;
        case AST_STRING: return visitor_visit_string(visitor, node); break;
        case AST_COMPOUND: return visitor_visit_compound(visitor, node); break;
        case AST_NOOP: return node; break;
    }

    printf("\x1b[31m");
    printf("Error: Uncaught statement of type '%d'\n", node->type);
    exit(1);

    return init_ast(AST_NOOP);
}

AST_T* visitor_visit_function_call(visitor_T* visitor, AST_T* node)
{
    if (strcmp(node->function_call_name, "print") == 0)
    {
        return builtin_function_print(visitor, node->function_call_arguments, node->function_call_arguments_size, node->function_call_args_name);
    }

    return node;
}

AST_T* visitor_visit_variable_definition(visitor_T* visitor, AST_T* node)
{
    scope_add_variable_definition(
        node->scope,
        node
    );

    AST_T* visited_ast = visitor_visit(visitor, node->variable_definition_value);

    contents = node->variable_definition_variable_name;
    strcat(datafile, contents);

    contents = " db "
               "\"";
    strcat(datafile, contents);

    contents = visited_ast->string_value;
    strcat(datafile, contents);

    contents = "\", 0x0a\n";
    strcat(datafile, contents);

    contents = node->variable_definition_variable_name;
    strcat(datafile, contents);

    contents = "len"
               ": equ $ - ";
    strcat(datafile, contents);
    
    contents = node->variable_definition_variable_name;
    strcat(datafile, contents);

    contents = "\n";
    strcat(datafile, contents);

    return node;
}

AST_T* visitor_visit_function_definition(visitor_T* visitor, AST_T* node)
{
    scope_add_func_definition(
        node->scope,
        node
    );

    contents ="GLOBAL _main\n\n"
               "section .text\n"
               "global start\n\n"
               "_main:\n";
    strcat(mainfile, contents);

    visitor_visit(visitor, node->function_definition_body);

    contents = "\nmov rax, 0x2000001\n"
               "mov rdi, 0\n"
               "syscall\n";
    strcat(mainfile, contents);

    return node;
}

AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node)
{
    return node;
}

AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node)
{
    AST_T* vdef = scope_get_variable_definition(
        node->scope,
        node->variable_name
    );

    if (vdef != (void*) 0)
        return visitor_visit(visitor, vdef->variable_definition_value);
    
    printf("\x1b[31m");
    printf("Error: Undifined variable '%s'\n", node->variable_name);
    exit(1);
}

AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node)
{
    for (int i = 0; i < node->compound_size; i++)
    {
        visitor_visit(visitor, node->compound_value[i]);
    }

    /* ↓ change this later */
    char finalcontents[1000];

    strcpy(finalcontents, mainfile);
    strcat(finalcontents, datafile);

    FILE *fp;
    fp = fopen("main.asm", "w");
    if (fp)
    {
        fprintf(fp, "%s", finalcontents);
        fclose(fp);
    }

    return init_ast(AST_NOOP);    
}