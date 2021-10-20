#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/asm.h"
#include "include/scope.h"
#include "include/AST.h"

assembly_T* init_assembly()
{
    assembly_T* assembly = calloc(1, sizeof(struct ASM_STRUCT));

    init_asm_file("dataSec.asm");
    init_asm_file("main.asm");

    add_to_main_section(
        "; Generated Assembly\n\n"
        "GLOBAL _main\n\n"
        "section .text\n"
        "global start\n\n"
    );

    add_to_data_section("section .data\n");

    return assembly;
}

int init_asm_file(char* fileName)
{
    FILE *fp;
    fp = fopen(fileName, "w");
    if (fp)
    {
        fprintf(fp, "%s", "");
        fclose(fp);
        return 0;
    }
    return 0;
}

char* add_to_main_section(char* contents)
{
    FILE *fp;
    fp = fopen("main.asm", "a+");
    if (fp)
    {
        fprintf(fp, "%s", contents);
        fclose(fp);
        return 0;
    }

    return 0;
}

char* add_to_data_section(char* contents)
{
    FILE *fp;
    fp = fopen("dataSec.asm", "a+");
    if (fp)
    {
        fprintf(fp, "%s", contents);
        fclose(fp);
        return 0;
    }
    return 0;
}

char* int_to_string(int x)
{
    int length = snprintf( NULL, 0, "%d", x );
    char* str = malloc( length + 1 );
    snprintf( str, length + 1, "%d", x );
    return str;
}

AST_T* assembly_visit(assembly_T* assembly, AST_T* node, char* operation)
{
    switch (node->type)
    {
        case AST_FUNCTION_DEFINITION: return assembly_visit_function_definition(assembly, node); break;
        case AST_VARIABLE_DEFINITION: return assembly_visit_variable_definition(assembly, node); break;
        case AST_FUNCTION_CALL: return assembly_visit_function_call(assembly, node); break;
        case AST_VARIABLE: return assembly_visit_variable(assembly, node, operation); break;
        case AST_STRING: return assembly_visit_string(assembly, node, operation); break;
        case AST_COMPOUND: return assembly_visit_compound(assembly, node); break;
        case AST_NOOP: return node; break;
    }

    printf("\x1b[31m");
    printf("Error: Uncaught statement of type '%d'\n", node->type);
    exit(1);

    return init_ast(AST_NOOP);
}

AST_T* assembly_visit_function_call(assembly_T* assembly, AST_T* node)
{
    if (strcmp(node->function_call_name, "print") == 0)
    {
        for (int i = 0; i < node->args_size; i++)
        {
            AST_T* visited = assembly_visit(assembly, node->args[i], "print");
        }
        return init_ast(AST_NOOP);
    }

    scope_find_func(node->scope, node->function_call_name);

    add_to_main_section("call ");
    add_to_main_section(node->function_call_name);
    add_to_main_section("\n\n");

    return 0;
}

AST_T* assembly_visit_variable_definition(assembly_T* assembly, AST_T* node)
{
    scope_add_variable_definition(
        node->scope,
        node
    );

    AST_T* value = assembly_visit(assembly, node->variable_definition_value, "NULL");

    add_to_data_section(node->variable_definition_variable_name);
    add_to_data_section(" db \"");
    add_to_data_section(value->string_value);
    add_to_data_section("\", 0xa\n");

    add_to_data_section(node->variable_definition_variable_name);
    add_to_data_section("_len: equ $ - ");
    add_to_data_section(node->variable_definition_variable_name);
    add_to_data_section("\n");

    return node;
}

AST_T* assembly_visit_function_definition(assembly_T* assembly, AST_T* node)
{
    scope_add_func_definition(
        node->scope,
        node
    );

    if (strcmp(node->function_definition_name, "main") == 0)
    {
        add_to_main_section(
            "_main:\n"
        );

        assembly_visit(assembly, node->function_definition_body, "NULL");

        add_to_main_section(
            "mov rax, 0x2000001\n"
            "mov rdi, 0\n"
            "syscall\n\n"
        );

        return node;
    }
    else
    {
        add_to_main_section(node->function_definition_name);
        add_to_main_section(":\n");

        assembly_visit(assembly, node->function_definition_body, "NULL");

        add_to_main_section("ret\n\n");
    }

    return node;
}

int str_count = 0;
AST_T* assembly_visit_string(assembly_T* assembly, AST_T* node, char* operation)
{
    if (strcmp(operation, "print") == 0)
    {
        add_to_main_section(
            "mov rax, 0x2000004\n"
            "mov rdi, 1\n"
            "mov rsi, str_"
        );
        add_to_main_section(int_to_string(str_count));
        add_to_main_section("\n");
        add_to_main_section("mov rdx, str_");
        add_to_main_section(int_to_string(str_count));
        add_to_main_section(
            "_len\n"
            "syscall\n\n"
        );
        
        // Data Section Assmebly
        add_to_data_section("str_");
        add_to_data_section(int_to_string(str_count));
        add_to_data_section(" db \"");
        add_to_data_section(node->string_value);
        add_to_data_section("\", 0xa\n");
        add_to_data_section("str_");
        add_to_data_section(int_to_string(str_count));
        add_to_data_section("_len: equ $ - ");
        add_to_data_section("str_");
        add_to_data_section(int_to_string(str_count));
        add_to_data_section("\n");

        str_count+=1;
    }

    return node;
}

AST_T* assembly_visit_variable(assembly_T* assembly, AST_T* node, char* operation)
{
    AST_T* vdef = scope_get_variable_definition(
        node->scope,
        node->variable_name
    );

    if (strcmp(operation, "print") == 0)
    {
        add_to_main_section("mov rax, 0x2000004\n");
        add_to_main_section("mov rdi, 1\n");
        add_to_main_section("mov rsi, ");
        add_to_main_section(node->variable_name);
        add_to_main_section("\n");
        add_to_main_section("mov rdx, ");
        add_to_main_section(node->variable_name);
        add_to_main_section("_len\n");
        add_to_main_section("syscall\n\n");

        return node;
    }

    if (vdef != (void*) 0)
        return assembly_visit(assembly, vdef->variable_definition_value, "NULL");
    
    printf("\x1b[31m");
    printf("Error: Undifined variable '%s'\n", node->variable_name);
    exit(1);

    return node;
}

AST_T* assembly_visit_compound(assembly_T* assembly, AST_T* node)
{
    for (int i = 0; i < node->compound_size; i++)
    {
        assembly_visit(assembly, node->compound_value[i], "NULL");
    }

    return init_ast(AST_NOOP);
}