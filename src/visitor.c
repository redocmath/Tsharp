#include "include/visitor.h"
#include "include/scope.h"
#include "include/AST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

visitor_T* init_visitor()
{
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

static AST_T* builtin_function_print(visitor_T* visitor, AST_T** args, int args_size)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T* visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
            case AST_STRING: printf("%s", visited_ast->string_value); break;
            default: printf("%p", visited_ast); break;
        }
    }
    printf("\n");

    return init_ast(AST_NOOP);
}

AST_T* visitor_visit_function_call(visitor_T* visitor, AST_T* node)
{
    if (strcmp(node->function_call_name, "print") == 0)
    {
        return builtin_function_print(visitor, node->args, node->args_size);
    }

    AST_T* fdef = scope_get_func_definition(
        node->scope,
        node->function_call_name
    );

    if (fdef == (void*) 0)
    {
        printf("\x1b[31m");
        printf("Error: '%s' function does not exist\n", node->function_call_name);
        exit(1);
    }

    visitor_visit(visitor, fdef->function_definition_body);

    return node;
}

AST_T* visitor_visit_variable_definition(visitor_T* visitor, AST_T* node)
{
    return node;
}

AST_T* visitor_visit_function_definition(visitor_T* visitor, AST_T* node)
{
    scope_add_func_definition(
        node->scope,
        node
    );

    if (strcmp(node->function_definition_name, "main") == 0)
    {
        return visitor_visit(visitor, node->function_definition_body);
    }

    return node;
}

AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node)
{
    return node;
}

AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node)
{
    return node;
}

AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node)
{
    for (int i = 0; i < node->compound_size; i++)
    {
        visitor_visit(visitor, node->compound_value[i]);
    }

    return init_ast(AST_NOOP);
}
