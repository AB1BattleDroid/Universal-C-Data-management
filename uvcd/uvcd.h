#ifndef UVCD_H
#define UVCD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//---String manipulation functions---
//char *Remove_SubString(const char *str, const char *substr);
//char *Remove_Spaces(const char *str);
//char *Remove_Spaces_WC(const char *str, int *str_len);
//int strstrcmp(const char *str, const char *substr);
//char *Before_String(char *input, const char *substring);
//char *After_String(char *input, const char *substring);
//char *replace_value(const char *str, void *new_value, const char *type);

//---Struct and file handling functions---

//--Begin/End a structure--
void UVCD_Struct_Begin(char *filename, int indent, char *type, char *name);

void UVCD_Struct_End(char *filename, int indent, char *type, char *name);

//--Create Elements/Items--

//-(UVCD_Item_Element_Scalar & UVCD_Item_Element_Array create arrays and single data units respetively while UVCD_Create_Element is generalized)-
//-(UVCD_Struct_Simple creates a simple structure, that can only have scalar values and arrays)-
void UVCD_Item_Element_Scalar(char *filename, int indent_level, const char *vartype, void *variable, char *name);

void UVCD_Item_Element_Array(char *filename, int indent_level, char *vartype, char *name, void *variable, int length_ptr);

void UVCD_Create_Element(char *filename, int indent_level, char *vartype, char *name, void *variable, int length_ptr);

void UVCD_Struct_Simple(char *filename, char *structype, char *name, char **vartype, char **names, void **variables, int *lengths, int length);

//--Read Elements/Items--
char **UVCD_Load_Contents(char *filename, int *rows, int *cols);
void **UVCD_Make_Array(void *input, int *size);
int UVCD_Get_Item_Amount(char *filename, char *type, char *name);

void *UVCD_Read_Element(char *filename, char *type, char *name, int ignore_amount);

void *UVCD_Read_Item(char *filename, char *type, char*name, int ignore_amount, int *size);

void **UVCD_Read_Struct(char *filename, char *struct_type, char *name, int *size, int ignore_amount);

void *UVCD_Read_Element_In_Struct(char *filename, char *struct_type, char *struct_name, char *type, char*name, int ignore_amount);

//--File Manipulation--
void UVCD_Remove_Element(char *filename, char *type, char*name, int ignore_amount);
void UVCD_Replace_Element(char *filename, char *type, char *name, void *new_data, int ignore_amount);
void UVCD_Empty_File(char *filename);

#endif /* UVCD_H */

