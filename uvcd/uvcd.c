#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define to_string(x) _Generic((x), \
    int*: (char*)(x), \
    float*: (char*)(x), \
    double*: (char*)(x), \
    char*: (char*)(x), \
    void*: (char*)(x), \
    bool*: (char*)(x), \
    int**: (char**)(x), \
    float**: (char**)(x), \
    double**: (char**)(x), \
    char**: (char**)(x), \
    void**: (char**)(x), \
    bool**: (char**)(x), \
    default: (char*)(x) \
)

#define nul 0


char *Remove_SubString(const char *str, const char *substr) {

    if (!str || !substr) {
        printf("Error: `Remove_SubString` received NULL pointer.\n");
        return NULL;
    }

    int str_len = strlen(str);
    int substr_len = strlen(substr);
    
    if (substr_len == 0) return strdup(str); // If substring is empty, return a copy of the original string
    
    // Find the first occurrence
    char *pos = strstr(str, substr);
    if (!pos) return strdup(str); // If substring not found, return a copy of the original string
    
    // Allocate memory for new string
    int new_len = str_len - substr_len;
    char *new_str = malloc(new_len + 1);
    
    if (!new_str) {
        perror("Memory allocation failed");
        return NULL;
    }

    // Copy parts before and after the substring
    int prefix_len = pos - str;
    strncpy(new_str, str, prefix_len);  // Copy part before the substring
    strcpy(new_str + prefix_len, pos + substr_len); // Copy part after the substring
    
    return new_str;
}

char *Remove_Spaces(const char *str) {
    // Find the first non-space character
    while (*str == ' ') {
        str++;
    }

    // Allocate memory for the new string
    char *new_str = malloc(strlen(str) + 1);
    if (!new_str) {
        perror("Memory allocation failed");
        return NULL;
    }

    // Copy the trimmed content
    strcpy(new_str, str);
    return new_str;
}

char *Remove_Spaces_WC(const char *str, int *str_len) {
    if (!str) {
        printf("Error: Input string is NULL.\n");
        return NULL;
    }

    int len = 0;
    while (*str == ' ') {
        str++;
        len++;
    }

    *str_len = len / 4;

    char *new_str = malloc(strlen(str) + 1);
    if (!new_str) {
        perror("Memory allocation failed");
        return NULL;
    }

    strcpy(new_str, str);
    return new_str;
}

int strstrcmp(const char *str, const char *substr) {
    return strstr(str, substr) ? 1 : 0;
}

char *Before_String(char *input, const char *substring) {
    const char *pos = strstr(input, substring);
    if (!input || !substring) { perror("Parameters to Before_string are NULL"); return NULL; }
    if (pos != NULL) {
        // Calculate the length of the part before the substring
        size_t length = pos - input;
        
        // Allocate memory for the result
        char *result = malloc(length + 1); // +1 for the null terminator
        if (result == NULL) {
            return NULL; // Memory allocation failed
        }
        
        // Copy the part before the substring
        strncpy(result, input, length);
        result[length] = '\0'; // Null-terminate the result
        return result; // Return the result
    } else {
        return input; // Substring not found
    }
}

char *After_String(char *input, const char *substring) {
    const char *pos = strstr(input, substring);
    
    if (pos != NULL) {
        pos += strlen(substring); // Move past the substring
        
        // Allocate memory for the result
        char *result = malloc(strlen(pos) + 1); // +1 for the null terminator
        if (result == NULL) {
            return NULL; // Memory allocation failed
        }
        
        strcpy(result, pos); // Copy the part after the substring
        return result; // Return the result
    } else {
        return input; // Substring not found
    }
}




void UVCD_Struct_Begin(char *filename, int indent, char *type, char *name) {
    FILE *file = fopen(filename, "a+");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    char indentation[(indent * 4) + 1];
    memset(indentation, 0, sizeof(indentation));
    for (int i = 0; i < indent; i++) {
        strcat(indentation, "    ");
    }
    fprintf(file, "%s%s %s { \n", indentation, type, name);
    fclose(file);
}

void UVCD_Struct_End(char *filename, int indent, char *type, char *name) {
    FILE *file = fopen(filename, "a+");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    char indentation[(indent * 4) + 1];
    memset(indentation, 0, sizeof(indentation));
    for (int i = 0; i < indent; i++) {
        strcat(indentation, "    ");
    }
    fprintf(file, "%s} %s %s \n\n", indentation, name, type);
    fclose(file);
}

void UVCD_Item_Element_Scalar(char *filename, int indent_level, const char *vartype, void *variable, char *name) {
    FILE *file = fopen(filename, "a+");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    char indentation[(indent_level * 4) + 1];
    memset(indentation, 0, sizeof(indentation));
    for (int i = 0; i < indent_level; i++) {
        strcat(indentation, "    ");
    }
    fprintf(file, "%s%s %s = ", indentation, vartype, name);
    if (strstr(vartype, "int")) {
        fprintf(file, "%d", *(int *)variable);
    } else if(strstr(vartype, "char")) { 
        fprintf(file, "\'%c\'", *(char *)variable);
    } else if(strstr(vartype, "double")) { 
        fprintf(file, "%f", *(double *)variable);
    } else if(strstr(vartype, "float")) { 
        fprintf(file, "%f", *(float *)variable);
    } else {
        fprintf(file, "%p", variable);
    }
    fprintf(file, ";\n\n");
    fclose(file);
}

void UVCD_Item_Element_Array(char *filename, int indent_level, char *vartype, char *name, void *variable, int length_ptr) {
    FILE *file = fopen(filename, "a+");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    char indentation[(indent_level * 4) + 1];
    memset(indentation, 0, sizeof(indentation));
    for (int i = 0; i < indent_level; i++) {
        strcat(indentation, "    ");
    }
    fprintf(file, "%s%s %s = { ", indentation, vartype, name); 
    for (size_t i = 0; i < length_ptr; i++) { 
        if (strcmp(vartype, "int *") == 0 ) {
            fprintf(file, "%d", ((int*)variable)[i]);
        } else if (strstrcmp(vartype, "double *") == 0) { 
            fprintf(file, "%f", ((double*)variable)[i]);
        } else if (strstrcmp(vartype, "float *") == 0) { 
            fprintf(file, "%f", ((float*)variable)[i]);
        } else if (strstrcmp(vartype, "int [") && strstrcmp(vartype, "]")) {
            fprintf(file, "%d", ((int*)variable)[i]);
        } else if (strstrcmp(vartype, "char [") && strstrcmp(vartype, "]")) {
            fprintf(file, "'%c'", ((char*)variable)[i]);
        } else if (strstrcmp(vartype, "double [") && strstrcmp(vartype, "]")) {
            fprintf(file, "%f", ((double*)variable)[i]);
        } else if (strstrcmp(vartype, "float [") && strstrcmp(vartype, "]")) {
            fprintf(file, "%f", ((float*)variable)[i]);
        } else {
            fprintf(file, "%p", variable);
        }
        if (i + 1 != length_ptr) {
            fprintf(file, ", ");
        }
    }
    if (strstrcmp(vartype, "char *") == 0) { 
        fprintf(file, "\"%s\"", ((char *)variable)); 
    } 
    fprintf(file, " };\n\n");
    fclose(file);
}

void UVCD_Create_Element(char *filename, int indent_level, char *vartype, char *name, void *variable, int length_ptr) {
    if ((strstrcmp(vartype, "[") && strstrcmp(vartype, "}")) || strstrcmp(vartype, "*")) {
        UVCD_Item_Element_Array(filename, indent_level, vartype, name, variable, length_ptr);
    } else {
        UVCD_Item_Element_Scalar(filename, indent_level, vartype, name, variable);
    }
}

void UVCD_Struct_Simple(char *filename, char *structype, char *name, char **vartype, char **names, void **variables, int *lengths, int length) {
    UVCD_Struct_Begin(filename, 0, structype, name);
    for (int I = 0; I < length; I++) {
        if ((strstrcmp(vartype[I], "[") && strstrcmp(vartype[I], "]")) || strstrcmp(vartype[I], "*")) {
            UVCD_Item_Element_Array(filename, 1, vartype[I], names[I], variables[I], sizeof(variables[I]));
        } else {
            UVCD_Item_Element_Scalar(filename, 1, vartype[I], variables[I], names[I]);
        }
    }
    UVCD_Struct_End(filename, 0, structype, name);
}

char **UVCD_Load_Contents(char *filename, int *rows, int *cols) {
    FILE *file = fopen(filename, "r+");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }
    rewind(file);

    char **content = NULL;
    char line[4096];
    *rows = 0;
    *cols = 0;

    while (fgets(line, sizeof(line), file)) {
        int length = strlen(line);

        if (line[length - 1] == '\n') {
            line[length - 1] = '\0';
            length--;
        }

        if (length > *cols) {
            *cols = length;
        }

        // Allocate memory safely
        char **temp = realloc(content, (*rows + 1) * sizeof(char *));
        if (!temp) {
            perror("Memory allocation failed");
            for (int i = 0; i < *rows; i++) free(content[i]);
            free(content);
            return NULL;
        }
        content = temp;

        content[*rows] = malloc((length + 1) * sizeof(char));
        if (!content[*rows]) {
            perror("Memory allocation failed");
            return NULL;
        }

        strcpy(content[*rows], line);
        (*rows)++;
    }
    fclose(file);
    return content;
}

void **UVCD_Make_Array(void *input, int *size) {
    if (!input) {
        *size = 0;
        return NULL;
    }

    char *str = (char *)input;
    const char *start = strchr(str, '{'); // Locate '{'
    const char *end = strchr(str, '}');   // Locate '}'

    if (!start || !end || start >= end) {
        *size = 0;
        return NULL;
    }

    start++; // Move past '{'
    int count = 0;
    void **values = malloc(10 * sizeof(void *)); // Initial allocation (adjust as needed)

    while (start < end) {
        double *num = malloc(sizeof(double)); // Allocate memory for each value
        if (sscanf(start, "%lf", num) == 1) {
            values[count++] = (void *)num;
        }

        start = strchr(start, ','); // Move to next comma
        if (!start) break;
        start++; // Move past ','
    }

    *size = count;
    return values;
}

void *UVCD_Read_Element(char *filename, char *type, char *name, int ignore_amount) {
    FILE *file = fopen(filename, "r+");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }
    int rows, col;
    char **contents = UVCD_Load_Contents(filename, &rows, &col);
    if (!contents) {
        return NULL;
    }
    for (int I = 0; I < rows; I++) {
        char *name_type = malloc(strlen(type) + strlen(name) + 3);
        if (!name_type) {
            perror("Memory allocation failed");
            free(contents);
            return NULL;
        } 
        snprintf(name_type, sizeof(name_type), "%s %s", type, name);
        char *query = malloc(strlen(name_type) + 4);
        if (!query) {
            perror("Memory allocation failed");
            free(name_type);
            free(contents);
            return NULL;
        }
        snprintf(query, sizeof(query), "%s = ", name_type);
        if (strstrcmp(contents[I], name_type)) {
            if (ignore_amount < 1) {
                void *result = Remove_Spaces(Remove_SubString(contents[I], query));
                
                free(name_type);
                free(query);
                free(contents);
                fclose(file);
                
                return result;
            } else {
                ignore_amount--;
            }
        }
        free(name_type);
        free(query);
    }
    free(contents);
    fclose(file);
    return NULL;
}


void *UVCD_Read_Item(char *filename, char *type, char*name, int ignore_amount, int *size) {
    if ((strstrcmp(type, "[") && strstrcmp(type, "]")) || strstrcmp(type, "*")) {
        return UVCD_Make_Array(UVCD_Read_Element(filename, type, name, ignore_amount), size);
    } else {
        void *ptr = UVCD_Read_Element(filename, type, name, ignore_amount);
        *size = sizeof(ptr);
        return ptr;
    }
    return NULL;
}






void **UVCD_Read_Struct_deap(char *filename, char *struct_type, char *name, char **types, char **names, int *size, int ignore_amount) { 
    int row, col, open_row = -1, close_row = -1; 
    char **contents = UVCD_Load_Contents(filename, &row, &col); 
    for (int I = 0; I < row; I++) { 
        char *open_statement = malloc(strlen(struct_type) + strlen(name) + 5); 
        char *close_statement = malloc(strlen(struct_type) + strlen(name) + 5); 
        snprintf(open_statement, (strlen(struct_type) + strlen(name) + 5), "%s %s {", struct_type, name); 
        snprintf(close_statement,  (strlen(name) + strlen(struct_type) + 5), "} %s %s", name, struct_type); 
        if(strstrcmp(contents[I], open_statement)) { 
            if (ignore_amount > 0) { 
                ignore_amount--; 
            } else {
                open_row = I + 1; 
            } 
        } else if (strstrcmp(contents[I], close_statement)) { 
            if (ignore_amount > 0) { 
                ignore_amount--; 
            } else {
                close_row = I; 
            }
        } 
        free(open_statement); 
        free(close_statement);
    } 
    if (open_row == -1 || close_row == -1 || open_row >= close_row) {return NULL;}
    if (open_row < close_row) {
        void **ptr = malloc((close_row - open_row) * sizeof(void *));
        int safety_var = 0;
        for (int I = open_row; I < close_row; I++) {

            char *ptr_hldr = malloc(strlen(types[safety_var]) + strlen(names[safety_var]) + 4);

            snprintf(ptr_hldr, (strlen(types[safety_var]) + strlen(names[safety_var]) + 4), "%s %s = ", types[safety_var], names[safety_var]);

            if (strlen(Remove_Spaces(contents[I])) < 1) { 
                printf("Debug: Nothing found in %s", Remove_Spaces(contents[I]));
                continue; 
            } else if (strstrcmp(contents[I], ptr_hldr)) { 
                char *sub = Remove_Spaces(Remove_SubString(Remove_SubString(Remove_Spaces(contents[I]), ptr_hldr), ";"));
                if (!sub) {
                    printf("Warning: Failed to extract substring in '%s'\n", contents[I]);
                    continue;
                } 
                printf("Debug: Results = %s\n", sub);
                ptr[safety_var] = (void *)(strdup(sub)); 
            } else { 
                printf("Debug: Matching '%s' with type '%s'\n", Remove_Spaces(contents[I]), types[safety_var]);
                printf("Debug: Matching '%s' with name '%s'\n", Remove_Spaces(contents[I]), names[safety_var]);
                if (!strstrcmp(Remove_Spaces(contents[I]), types[safety_var])) {
                    perror("Error: Type doesn't match");
                } else if (!strstrcmp(Remove_Spaces(contents[I]), names[safety_var])) {
                    perror("Error: Name doesn't match");
                }
            }
            safety_var++;
        }
        *size = close_row - open_row;
        return ptr; 
    }
    for (int i = 0; i < row; i++) {
        free(contents[i]);
    }
    free(contents);
    return NULL;
}






void **UVCD_Read_Struct(char *filename, char *struct_type, char *name, int *size, int ignore_amount) { 
    int row, col, open_row = -1, close_row = -1; 
    char **contents = UVCD_Load_Contents(filename, &row, &col); 
    for (int I = 0; I < row; I++) { 
        char *open_statement = malloc(strlen(struct_type) + strlen(name) + 5); 
        char *close_statement = malloc(strlen(struct_type) + strlen(name) + 5); 
        snprintf(open_statement, (strlen(struct_type) + strlen(name) + 5), "%s %s {", struct_type, name); 
        snprintf(close_statement,  (strlen(name) + strlen(struct_type) + 5), "} %s %s", name, struct_type); 
        if(strstrcmp(contents[I], open_statement)) { 
            if (ignore_amount > 0) { 
                ignore_amount--; 
            } else {
                open_row = I + 1; 
            } 
        } else if (strstrcmp(contents[I], close_statement)) { 
            if (ignore_amount > 0) { 
                ignore_amount--; 
            } else {
                close_row = I; 
            }
        } 
        free(open_statement); 
        free(close_statement);
    } 
    if (open_row == -1 || close_row == -1 || open_row >= close_row) {return NULL;}
    if (open_row < close_row) {
        void **ptr = malloc((close_row - open_row) * sizeof(void *));
        int count = 0;
        for (int I = open_row; I < close_row; I++) {
            char *temp_ptr = Remove_SubString(Remove_Spaces(After_String(contents[I], "=")), ";");
            if (strlen(temp_ptr) < 1) { 
                printf("Debug: Nothing found in %s", Remove_Spaces(contents[I]));
                continue; 
            } else {
                ptr[count] = (void *)temp_ptr;
                count++;
            }
        }
        *size = close_row - open_row;
        return ptr; 
    }
    for (int i = 0; i < row; i++) {
        free(contents[i]);
    }
    free(contents);
    return NULL;
}

void *UVCD_Read_Element_In_Struct(char *filename, char *struct_type, char *struct_name, char *type, char*name, int ignore_amount) { 
    int row, col, open_row = -1, close_row = -1; 
    char **contents = UVCD_Load_Contents(filename, &row, &col); 
    for (int I = 0; I < row; I++) { 
        char *open_statement = malloc(strlen(struct_type) + strlen(struct_name) + 5); 
        char *close_statement = malloc(strlen(struct_type) + strlen(struct_name) + 5); 
        snprintf(open_statement, (strlen(struct_type) + strlen(struct_name) + 5), "%s %s { ", struct_type, struct_name); 
        snprintf(close_statement,  (strlen(struct_name) + strlen(struct_type) + 5), "} %s %s", struct_name, struct_type); 
        
        if (strstrcmp(contents[I], open_statement)) {
            if (ignore_amount > 0) { 
                ignore_amount--; 
            } else {
                open_row = I + 1; 
            } 
        } else if (strstrcmp(contents[I], close_statement)) { 
            if (ignore_amount > 0) { 
                ignore_amount--; 
            } else {
                close_row = I; 
            }
        } 
        free(open_statement); 
        free(close_statement);
    } 
    if (open_row >= close_row || open_row == close_row) {return NULL;}
    if (open_row < close_row) {
        void *ptr = malloc(strlen(name) + strlen(type) + strlen("Item not found"));
        ptr = "Item not found";
        for (int I = open_row; I < close_row; I++) {
            char *temp_ptr = Before_String(Remove_Spaces(contents[I]), "=");
            char *name_type = malloc(strlen(name) + strlen(type) + 3);
            snprintf(name_type, (strlen(name) + strlen(type) + 3), "%s %s ", type, name);
            //printf("Name_type = %s, Temp = %s.\n", name_type, temp_ptr);
            if (strlen(temp_ptr) < 1) { 
                perror("Debug: Nothing found \n");
                continue; 
            } else if (strcmp(temp_ptr, name_type) == 0) {
                ptr = Remove_Spaces(After_String(Remove_Spaces(contents[I]), "="));
            }
        }
        return ptr; 
    }
    for (int i = 0; i < row; i++) {
        free(contents[i]);
    }
    free(contents);
    return NULL;
}

void UVCD_Remove_Element(char *filename, char *type, char*name, int ignore_amount) {
    int row, col;
    char **contents = UVCD_Load_Contents(filename, &row, &col);
    char *remove_line = malloc(sizeof(type) + sizeof(name) + 5);
    snprintf(remove_line, (sizeof(type) + sizeof(name) + 5), "%s %s = ", type, name);

    for (int I = 0; I < row; I++) {
        if (strstrcmp(contents[I], remove_line)) {
            if (ignore_amount < 1) {
                if (I > 0) { free(contents[I - 1]); }
                free(contents[I]);
                for (int j = I - 1; j < row - 2; j++) {
                    contents[j] = contents[j + 2];
                } 
                contents[row - 2] = NULL;
                contents[row - 1] = NULL;
                free(contents[row - 2]);
                free(contents[row - 1]);
                row -= 2;
            } else {
                ignore_amount--;
            }
        }
    }

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    for (int I = 0; I < row; I++) {
        fprintf(file, "%s\n", contents[I]);
    }

    fclose(file);
}

char *replace_value(const char *str, void *new_value, const char *type) {
    if (!str || !new_value) {
        fprintf(stderr, "Error: Invalid input (str or new_value is NULL)\n");
        return strdup(str); // Avoid crashing
    }

    char *equals_pos = strchr(str, '=');
    if (!equals_pos) {
        fprintf(stderr, "Error: '=' not found in string\n");
        return strdup(str); // No '=' found, return unchanged string
    }

    // Move past '=' and **remove any existing value**
    char *value_start = equals_pos + 1;
    while (*value_start == ' ') value_start++; // Skip spaces

    // Create a new formatted value
    char temp[512];
    if (strcmp(type, "int") == 0) {
        snprintf(temp, sizeof(temp), " %d", *(int*)new_value);  // **No extra '='**
    } else if (strcmp(type, "float") == 0) {
        snprintf(temp, sizeof(temp), " %.2f", *(float*)new_value);
    } else if (strcmp(type, "double") == 0) {
        snprintf(temp, sizeof(temp), " %lf", *(double*)new_value);
    } else {
        fprintf(stderr, "Error: Unknown type '%s'\n", type);
        return strdup(str);
    }

    // Allocate space for new modified string
    size_t prefix_length = equals_pos - str + 2; // Include '=' and space
    char *new_str = (char *)malloc(prefix_length + strlen(temp) + 1);
    if (!new_str) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }

    // Copy everything **before '='**, add '=' **only once**, then append new value
    strncpy(new_str, str, prefix_length - 1);  // Remove old value
    new_str[prefix_length - 1] = '\0';         // Null-terminate before concatenation
    strcat(new_str, temp);  // Append new value

    return strcat(new_str, ";");
}

void UVCD_Replace_Element(char *filename, char *type, char *name, void *new_data, int ignore_amount) {
    int row, col;
    char **contents = UVCD_Load_Contents(filename, &row, &col);
    
    if (!contents) {
        fprintf(stderr, "Error: Failed to load contents from file '%s'\n", filename);
        return;
    }

    char *remove_line = malloc(strlen(type) + strlen(name) + 5);
    if (!remove_line) {
        fprintf(stderr, "Error: Memory allocation failed for remove_line\n");
        return;
    }
    snprintf(remove_line, strlen(type) + strlen(name) + 5, "%s %s = ", type, name);

    for (int I = 0; I < row; I++) {
        if (!Before_String(contents[I], "=")) { continue; }
        if (contents[I] && strcmp(Before_String(contents[I], "="), Before_String(remove_line, "=")) == 0) {
            if (ignore_amount < 1) {
                char *updated_line = replace_value(contents[I], new_data, type);
                //printf("Contents = %s, updated line = %s\n", contents[I], updated_line);
                if (updated_line) {
                    free(contents[I]);  // Free old memory before replacing
                    contents[I] = updated_line;
                    break;
                }
            } else {
                ignore_amount--;
            }
        }
    }

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        free(remove_line);
        return;
    }

    for (int I = 0; I < row; I++) {
        fprintf(file, "%s\n", contents[I]);
    }

    fclose(file);
    free(remove_line);
}


int UVCD_Get_Item_Amount(char *filename, char *type, char *name) {
    int row, col, count = 0;
    char **contents = UVCD_Load_Contents(filename, &row, &col);
    
    if (!contents) {
        fprintf(stderr, "Error: Failed to load contents from file '%s'\n", filename);
        return -1;  // Return error value
    }

    for (int I = 0; I < row; I++) {

        if (!contents[I]) { continue; }

        char *name_type = malloc(strlen(name) + strlen(type) + 3);
        if (!name_type) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            continue;  // Skip to next iteration
        }

        snprintf(name_type, strlen(name) + strlen(type) + 3, "%s %s ", type, name);

        char *element = Before_String(contents[I], "=");
        if (!element) {
            continue; // Skip iteration
        }
        if (strncmp(Remove_Spaces(element), name_type, strlen(name_type)) == 0) {
            count++;
            //printf("Count + 1 for: %s\n", contents[I]);
        }

        free(name_type);  // Prevent memory leak
    }

    // Free loaded contents
    for (int i = 0; i < row; i++) {
        free(contents[i]);
    }
    free(contents);

    return count;
}

void UVCD_Empty_File(char *filename) {
    FILE *file = fopen(filename, "w");
    fclose(file);
}
