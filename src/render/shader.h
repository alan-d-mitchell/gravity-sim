#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

typedef struct {
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;
} Shader;

// Load and compile shader from file
int shader_load_from_file(Shader *shader, const char *vertex_path, const char *fragment_path);

// Use this shader program
void shader_use(Shader *shader);

// Delete shader program and free resources
void shader_delete(Shader *shader);

// Utility functions to set uniform values
void shader_set_float(Shader *shader, const char *name, float value);
void shader_set_int(Shader *shader, const char *name, int value);
void shader_set_vec3(Shader *shader, const char *name, float x, float y, float z);
void shader_set_mat4(Shader *shader, const char *name, const float *matrix);

#endif /* SHADER_H */