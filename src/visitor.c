#include "include/visitor.h"
#include "include/scope.h"
#include "include/AST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static AST_T* builtin_function_print(visitor_T* visitor, AST_T** args, int args_size, char* name)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T* visited_ast = visitor_visit(visitor, args[i]);
        if (visited_ast->type == AST_STRING)
        {
            char* buffer = 0;
            buffer = visitor_read_current_comp_file();
            
            FILE *fp;
            fp = fopen("comp.c", "w");

            if (fp)
            {
                fprintf(fp, "%s", buffer);
                fprintf(fp, "%s", "\nprintf(\"%s\",");
                fprintf(fp, "%s", name);
                fprintf(fp, "%s", ");");
                fclose(fp);
            }
        }
    }
    return init_ast(AST_NOOP);
}

static AST_T* builtin_function_print_nonval(visitor_T* visitor, AST_T** args, int args_size)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T* visited_ast = visitor_visit(visitor, args[i]);
        if (visited_ast->type == AST_STRING)
        {
            char* buffer = 0;
            buffer = visitor_read_current_comp_file();
            
            FILE *fp;
            fp = fopen("comp.c", "w");

            if (fp)
            {
                fprintf(fp, "%s", buffer);
                fprintf(fp, "%s", "\nprintf(\"%s\",");
                fprintf(fp, "%s", "\"");
                fprintf(fp, "%s", visited_ast->string_value);
                fprintf(fp, "%s", "\"");
                fprintf(fp, "%s", ");");
                fclose(fp);
            }
        }
    }
    return init_ast(AST_NOOP);
}

char* visitor_read_current_comp_file()
{
    char* buffer = 0;
    long length;

    FILE*f = fopen("comp.c", "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);

        buffer = calloc(length, length);

        if (buffer)
            fread(buffer, 1, length, f);

        fclose(f);

        return buffer;
    }

    printf("File Does not exist!\n");

    exit(1);
}

visitor_T* init_visitor()
{
    FILE *fp;
    fp = fopen("comp.c", "w");
    fprintf(fp, "%s", "#include <stdio.h>\n");
    fprintf(fp, "%s", "#include <stdlib.h>\n");
    fprintf(fp, "%s", "#include <stdio.h>\n\n");
    fclose(fp);

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
        if (node->function_call_args_name == (void*) 0) {
            return builtin_function_print_nonval(visitor, node->function_call_arguments, node->function_call_arguments_size);
        }
        return builtin_function_print(visitor, node->function_call_arguments, node->function_call_arguments_size, node->function_call_args_name);
    }

    scope_find_func(node->scope, node->function_call_name);

    if (1) {
        char* buffer = 0;
        buffer = visitor_read_current_comp_file();
        
        FILE *fp;
        fp = fopen("comp.c", "w");

        if (fp)
        {
            fprintf(fp, "%s", buffer);

            fprintf(fp, "%s", "\n");
            fprintf(fp, "%s", node->function_call_name);
            fprintf(fp, "%s", "(");
            fprintf(fp, "%s", ");");
            
            fclose(fp);
        }
    }

    return node;
}

AST_T* visitor_visit_variable_definition(visitor_T* visitor, AST_T* node)
{
    scope_add_variable_definition(
        node->scope,
        node
    );
    
    char* buffer = 0;
    buffer = visitor_read_current_comp_file();

    AST_T* visited_ast = visitor_visit(visitor, node->variable_definition_value);
    
    FILE *fp;
    fp = fopen("comp.c", "w");

    if (fp)
    {
        fprintf(fp, "%s", buffer);
        if (visited_ast->type == AST_STRING) {
            fprintf(fp, "%s", "\nchar* ");
            fprintf(fp, "%s", node->variable_definition_variable_name);
            fprintf(fp, "%s", " = ");
            fprintf(fp, "%s", "\"");
            fprintf(fp, "%s", visited_ast->string_value);
            fprintf(fp, "%s", "\";");
        }
        
        fclose(fp);
    }

    return node;
}

AST_T* visitor_visit_function_definition(visitor_T* visitor, AST_T* node)
{
    scope_add_func_definition(
        node->scope,
        node
    );

    if (1) {
        char* buffer = 0;
        buffer = visitor_read_current_comp_file();
        
        FILE *fp;
        fp = fopen("comp.c", "w");

        if (fp)
        {
            fprintf(fp, "%s", buffer);

            fprintf(fp, "%s", "\nint ");
            fprintf(fp, "%s", node->function_definition_name);
            fprintf(fp, "%s", "() {");
            fclose(fp);
        }

        visitor_visit(visitor, node->function_definition_body);

        buffer = 0;
        buffer = visitor_read_current_comp_file();

        fp = fopen("comp.c", "w");

        if (fp)
        {
            fprintf(fp, "%s", buffer);

            fprintf(fp, "%s", "\nreturn 0;");
            fprintf(fp, "%s", "\n};\n\n");
            fclose(fp);
        }
    }

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

    return init_ast(AST_NOOP);    
}