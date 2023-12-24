#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned int Shader;

void check_compile_errors(Shader shader, const char* type)
{
    int success;
    char info_log[1024];
    if (strcmp(type, "PROGRAM") != 0)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, info_log);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, info_log);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, info_log);
            printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n", type, info_log);
        }
    }
}

void shader_load(Shader* shader, const char* vertex_path, const char* fragment_path)
{
    char *vertex_source, *fragment_source;
    long file_size;
    FILE* fp;

    fp = fopen(vertex_path, "r");
    if (fp == NULL)
    {
        printf("File could not be opened\n");
        return;
    }
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp) + 1;
    rewind(fp);

    vertex_source = (char*)malloc(file_size * sizeof(char));
    if (vertex_source == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }
    fread(vertex_source, sizeof(char), file_size, fp);
    vertex_source[file_size - 1] = '\0';
    fclose(fp);

    fp = fopen(fragment_path, "r");
    if (fp == NULL)
    {
        printf("File could not be opened\n");
        return;
    }
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp) + 1;
    rewind(fp);

    fragment_source = (char*)malloc(file_size * sizeof(char));
    if (fragment_source == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }
    fread(fragment_source, sizeof(char), file_size, fp);
    fragment_source[file_size - 1] = '\0';
    fclose(fp);

    unsigned int vertex, fragment;
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const char**)&vertex_source, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, "VERTEX");
    free(vertex_source);
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const char**)&fragment_source, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, "FRAGMENT");
    free(fragment_source);

    unsigned int ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    check_compile_errors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    *shader = ID;
}

void shader_use(Shader shader)
{
    glUseProgram(shader);
}

void shader_set_bool(Shader shader, const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(shader, name), (int)value); 
}
void shader_set_int(Shader shader, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(shader, name), value); 
}
void shader_set_float(Shader shader, const char* name, float value)
{
    glUniform1f(glGetUniformLocation(shader, name), value); 
}
void shader_set_mat4(Shader shader, const char* name, glm::mat4* matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, &(*matrix)[0][0]);
}
void shader_set_vec3(Shader shader, const char* name, glm::vec3* vector)
{
    glUniform3fv(glGetUniformLocation(shader, name), 1, &(*vector)[0]);
}