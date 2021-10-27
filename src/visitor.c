#include "include/visitor.h"
#include "include/scope.h"
#include "include/AST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
        case AST_IF: return visitor_visit_if(visitor, node); break;
        case AST_COMPARE: return visitor_visit_compare(visitor, node); break;
        case AST_WHILE: return visitor_visit_while(visitor, node); break;
        case AST_BINOP_INC_DEC: return visitor_visit_binop_inc_dec(visitor, node); break;
        case AST_STRING: return visitor_visit_string(visitor, node); break;
        case AST_INT: return visitor_visit_int(visitor, node); break;
        case AST_BOOL: return visitor_visit_bool(visitor, node); break;
        case AST_COMPOUND: return visitor_visit_compound(visitor, node); break;
        case AST_NOOP: return node; break;
    }

    printf("Error: uncaught statement of type '%d'\n", node->type);
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
            case AST_INT: printf("%ld", visited_ast->int_value); break;
            case AST_BOOL: printf("%s", visited_ast->bool_value); break;
            default: printf("%p", visited_ast); break;
        }
    }
    printf("\n");

    return init_ast(AST_NOOP);
}

static AST_T* builtin_function_sleep(visitor_T* visitor, AST_T** args, int args_size)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T* visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
            case AST_STRING: printf("ERROR: mismatched types\n"); exit(1); break;
            case AST_INT: sleep(visited_ast->int_value); break;
            case AST_BOOL: printf("ERROR: mismatched types\n"); exit(1); break;
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

    if (strcmp(node->function_call_name, "add") == 0)
    {
        AST_T* visited_ast_left = visitor_visit(visitor, node->args[0]);
        AST_T* visited_ast_right = visitor_visit(visitor, node->args[0]);
        long int int_value = visited_ast_left->int_value + visited_ast_right->int_value;
        AST_T* ast_int = init_ast(AST_INT);
        ast_int->int_value = int_value;

        return ast_int;
    }

    if (strcmp(node->function_call_name, "minus") == 0)
    {
        AST_T* visited_ast_left = visitor_visit(visitor, node->args[0]);
        AST_T* visited_ast_right = visitor_visit(visitor, node->args[0]);
        long int int_value = visited_ast_left->int_value - visited_ast_right->int_value;
        AST_T* ast_int = init_ast(AST_INT);
        ast_int->int_value = int_value;

        return ast_int;
    }

    if (strcmp(node->function_call_name, "mul") == 0)
    {
        AST_T* visited_ast_left = visitor_visit(visitor, node->args[0]);
        AST_T* visited_ast_right = visitor_visit(visitor, node->args[0]);
        long int int_value = visited_ast_left->int_value * visited_ast_right->int_value;
        AST_T* ast_int = init_ast(AST_INT);
        ast_int->int_value = int_value;

        return ast_int;
    }

    if (strcmp(node->function_call_name, "div") == 0)
    {
        AST_T* visited_ast_left = visitor_visit(visitor, node->args[0]);
        AST_T* visited_ast_right = visitor_visit(visitor, node->args[0]);
        long int int_value = visited_ast_left->int_value / visited_ast_right->int_value;
        AST_T* ast_int = init_ast(AST_INT);
        ast_int->int_value = int_value;

        return ast_int;
    }

    if (strcmp(node->function_call_name, "rem") == 0)
    {
        AST_T* visited_ast_left = visitor_visit(visitor, node->args[0]);
        AST_T* visited_ast_right = visitor_visit(visitor, node->args[1]);
        long int int_value = visited_ast_left->int_value % visited_ast_right->int_value;
        AST_T* ast_int = init_ast(AST_INT);
        ast_int->int_value = int_value;

        return ast_int;
    }

    if (strcmp(node->function_call_name, "sleep") == 0)
    {
        return builtin_function_sleep(visitor, node->args, node->args_size);
    }

    if (strcmp(node->function_call_name, "len") == 0)
    {
        if (node->args_size != 1)
        {
            printf("ERROR: function len expected one argument\n");
            exit(1);
        }
        AST_T* visited_value = visitor_visit(visitor, node->args[0]);
        if (visited_value->type != AST_STRING)
        {
            printf("ERROR: function len expected type string\n");
            exit(1); 
        }

        long int int_value = strlen(visited_value->string_value);
        AST_T* ast_int = init_ast(AST_INT);
        ast_int->int_value = int_value; 
        
        return ast_int;
    }

    AST_T* fdef = scope_get_func_definition(
        node->scope,
        node->function_call_name
    );

    if (fdef == (void*) 0)
    {
        printf("Error: undefined function %s\n", node->function_call_name);
        exit(1);
    }

    for (int i = 0; i < (int) node->args_size; i++)
    {
        AST_T* ast_var = (AST_T*) fdef->function_definition_args[i];

        AST_T* ast_value = (AST_T*) node->args[i];

        AST_T* ast = init_ast(AST_VARIABLE_DEFINITION);

        ast->variable_definition_value = ast_value;

        ast->variable_definition_variable_name = calloc(strlen(ast_var->variable_name) + 1, sizeof(char));
        ast->variable_definition_func_name = calloc(strlen(node->function_call_name) + 1, sizeof(char));
        strcpy(ast->variable_definition_variable_name, ast_var->variable_name);

        scope_add_variable_definition(fdef->function_definition_body->scope, ast);
    }

    visitor_visit(visitor, fdef->function_definition_body);

    return node;
}

AST_T* visitor_visit_variable_definition(visitor_T* visitor, AST_T* node)
{
    AST_T* vdef = scope_get_variable_definition(
        node->scope,
        node->variable_definition_variable_name,
        node->variable_definition_func_name
    );

    if (vdef != (void*) 0)
    {
        scope_change_variable_definition(
            node->scope,
            node->variable_definition_variable_name,
            node->variable_definition_func_name,
            node->variable_definition_value
        );
        return node;
    }

    scope_add_variable_definition(
        node->scope,
        node
    );

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

AST_T* visitor_visit_if(visitor_T* visitor, AST_T* node)
{
    AST_T* visited_ast = visitor_visit(visitor, node->op);
    if (visited_ast->type == AST_BOOL && strcmp(visited_ast->bool_value, "true") == 0)
    {
        return visitor_visit(visitor, node->if_body);
    }

    for (int i = 0; i < node->elif_size; i++)
    {
        AST_T* visited_else = visitor_visit(visitor, node->elifop[i]);
        if (strcmp(visited_else->bool_value, "true") == 0)
        {
            return visitor_visit(visitor, node->elifbody[i]);
        }
    }
    
    if (visited_ast->type == AST_BOOL && strcmp(visited_ast->bool_value, "false") == 0)
    {
        if (node->else_body != (void*) 0)
        {
            return visitor_visit(visitor, node->else_body);
        }
    }

    return node;
}

AST_T* visitor_visit_compare(visitor_T* visitor, AST_T* node)
{
    char* value;

    AST_T* visited_left = visitor_visit(visitor, node->left);
    AST_T* visited_right = visitor_visit(visitor, node->right);

    if (node->compare_op == 12)
    {
        if (visited_left->type == AST_STRING && visited_right->type == AST_STRING)
        {
            if (strcmp(visited_left->string_value, visited_right->string_value) == 0)
            {
                value = "true";
            }
            else
            {
                value = "false";
            }
        }
        else
        if (visited_left->type == AST_INT && visited_right->type == AST_INT)
        {
            if (visited_left->int_value == visited_right->int_value)
            {
                value = "true";
            }
            else
            {
                value = "false";
            }
        }
        else
        if (visited_left->type == AST_BOOL && visited_right->type == AST_BOOL)
        {
            if (strcmp(visited_left->bool_value, visited_right->bool_value) == 0)
            {
                value = "true";
            }
            else
            {
                value = "false";
            }
        }
        else
        {
            printf("ERROR: mismatched types\n");
            exit(1);
        }
    }
    else
    if (node->compare_op == 10)
    {
        if (visited_left->type == AST_INT && visited_right->type == AST_INT)
        {
            if (visited_left->int_value > visited_right->int_value)
            {
                value = "true";
            }
            else
            {
                value = "false";
            }
        }
        else
        {
            printf("ERROR: mismatched types\n");
            exit(1);
        }
    }
    else
    if (node->compare_op == 11)
    {
        if (visited_left->type == AST_INT && visited_right->type == AST_INT)
        {
            if (visited_left->int_value < visited_right->int_value)
            {
                value = "true";
            }
            else
            {
                value = "false";
            }
        }
        else
        {
            printf("ERROR: mismatched types\n");
            exit(1);
        }
    }
    else
    if (node->compare_op == 13)
    {
        if (visited_left->type == AST_STRING && visited_right->type == AST_STRING)
        {
            if (strcmp(visited_left->string_value, visited_right->string_value) != 0)
            {
                value = "true";
            }
            else
            {
                value = "false";
            }
        }
        else
        if (visited_left->type == AST_INT && visited_right->type == AST_INT)
        {
            if (visited_left->int_value != visited_right->int_value)
            {
                value = "true";
            }
            else
            {
                value = "false";
            }
        }
        else
        if (visited_left->type == AST_BOOL && visited_right->type == AST_BOOL)
        {
            if (strcmp(visited_left->bool_value, visited_right->bool_value) != 0)
            {
                value = "true";
            }
            else
            {
                value = "false";
            }
        }
        else
        {
            printf("ERROR: mismatched types\n");
            exit(1);
        }
    }

    AST_T* ast_bool = init_ast(AST_BOOL);
    ast_bool->bool_value = value;

    return ast_bool;
}

AST_T* visitor_visit_while(visitor_T* visitor, AST_T* node)
{
    AST_T* visited_op = visitor_visit(visitor, node->op);

    if (visited_op->type != AST_BOOL)
    {
        printf("ERROR: while loop unexpected value\n");
        exit(1);
    }

    while (1) {
        visited_op = visitor_visit(visitor, node->op);
        if (strcmp(visited_op->bool_value, "true") == 0)
        {
            visitor_visit(visitor, node->while_body);
        }
        else
        {
            return node;
        }
    }
    return node;
}

AST_T* visitor_visit_binop_inc_dec(visitor_T* visitor, AST_T* node)
{
    AST_T* vdef = scope_get_variable_definition(
        node->scope,
        node->binop_inc_dec_variable,
        node->binop_inc_dec_func_name
    );

    if (vdef == (void*) 0)
    {
        printf("ERROR: undifined variable '%s'\n", node->variable_name);
        exit(1);
    }
    
    AST_T* visited_value = visitor_visit(visitor, vdef->variable_definition_value);

    if (visited_value->type != AST_INT)
    {
        printf("ERROR: mismatched types\n");
        exit(1);
    }

    long int int_value;
    if (node->binop_inc_dec_op == 19)
    {
        int_value = visited_value->int_value + 1;
    }
    else
    if (node->binop_inc_dec_op == 20)
    {
        int_value = visited_value->int_value - 1;
    }
    
    AST_T* ast_int = init_ast(AST_INT);
    ast_int->int_value = int_value;

    AST_T* ast = init_ast(AST_VARIABLE_DEFINITION);
    ast->variable_definition_value = ast_int;

    scope_change_variable_definition(
        node->scope,
        node->binop_inc_dec_variable,
        node->binop_inc_dec_func_name,
        ast->variable_definition_value
    );

    return ast_int;
}

AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node)
{
    return node;
}

AST_T* visitor_visit_int(visitor_T* visitor, AST_T* node)
{
    return node;
}

AST_T* visitor_visit_bool(visitor_T* visitor, AST_T* node)
{
    return node;
}

AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node)
{
    AST_T* vdef = scope_get_variable_definition(
        node->scope,
        node->variable_name,
        node->variable_func_name
    );

    if (vdef != (void*) 0)
        return visitor_visit(visitor, vdef->variable_definition_value);

    printf("ERROR: undifined variable '%s'\n", node->variable_name);
    exit(1);
}

AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node)
{
    for (int i = 0; i < node->compound_size; i++)
    {
        visitor_visit(visitor, node->compound_value[i]);
    }

    return init_ast(AST_NOOP);
}
