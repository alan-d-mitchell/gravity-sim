#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Utility function to read shader file contents
static char* read_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return NULL;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate buffer (+1 for null terminator)
    char *buffer = (char*)malloc(size + 1);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }
    
    // Read file and add null terminator
    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    
    fclose(file);
    return buffer;
}

// Utility function to check shader compilation errors
static void check_shader_errors(GLuint shader, const char *type) {
    GLint success;
    GLchar info_log[1024];
    
    if (strcmp(type, "PROGRAM") != 0) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, info_log);
            fprintf(stderr, "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, info_log);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, info_log);
            fprintf(stderr, "ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n", type, info_log);
        }
    }
}

int shader_load_from_file(Shader *shader, const char *vertex_path, const char *fragment_path) {
    // Read shader source files
    char *vertex_source = read_file(vertex_path);
    char *fragment_source = read_file(fragment_path);
    
    if (!vertex_source || !fragment_source) {
        if (vertex_source) free(vertex_source);
        if (fragment_source) free(fragment_source);
        return 0;
    }
    
    // Compile vertex shader
    shader->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader->vertex_shader, 1, (const GLchar**)&vertex_source, NULL);
    glCompileShader(shader->vertex_shader);
    check_shader_errors(shader->vertex_shader, "VERTEX");
    
    // Compile fragment shader
    shader->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader->fragment_shader, 1, (const GLchar**)&fragment_source, NULL);
    glCompileShader(shader->fragment_shader);
    check_shader_errors(shader->fragment_shader, "FRAGMENT");
    
    // Link shaders into a program
    shader->program = glCreateProgram();
    glAttachShader(shader->program, shader->vertex_shader);
    glAttachShader(shader->program, shader->fragment_shader);
    glLinkProgram(shader->program);
    check_shader_errors(shader->program, "PROGRAM");
    
    // Free shader source code
    free(vertex_source);
    free(fragment_source);
    
    return 1;
}

void shader_use(Shader *shader) {
    glUseProgram(shader->program);
}

void shader_delete(Shader *shader) {
    glDeleteShader(shader->vertex_shader);
    glDeleteShader(shader->fragment_shader);
    glDeleteProgram(shader->program);
}

void shader_set_float(Shader *shader, const char *name, float value) {
    GLint location = glGetUniformLocation(shader->program, name);
    glUniform1f(location, value);
}

void shader_set_int(Shader *shader, const char *name, int value) {
    GLint location = glGetUniformLocation(shader->program, name);
    glUniform1i(location, value);
}

void shader_set_vec3(Shader *shader, const char *name, float x, float y, float z) {
    GLint location = glGetUniformLocation(shader->program, name);
    glUniform3f(location, x, y, z);
}

void shader_set_mat4(Shader *shader, const char *name, const float *matrix) {
    GLint location = glGetUniformLocation(shader->program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
}
    