#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Forward declarations for static functions
static void setup_sphere_mesh(void);
static void render_sphere(Renderer *renderer, Vec3 position, float radius, Vec3 color);

// Callback function wrappers (to access renderer from callbacks)
static Renderer *current_renderer = NULL;

// Initialize the renderer
int renderer_init(Renderer *renderer, SimConfig *config) {
    renderer->window_width = config->window_width;
    renderer->window_height = config->window_height;
    
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return 0;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); // Anti-aliasing
    
    // Create window
    renderer->window = glfwCreateWindow(renderer->window_width, renderer->window_height, 
                                        config->window_title, NULL, NULL);
    if (!renderer->window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return 0;
    }
    
    glfwMakeContextCurrent(renderer->window);
    
    // Set callback functions
    current_renderer = renderer;
    glfwSetFramebufferSizeCallback(renderer->window, framebuffer_size_callback);
    glfwSetCursorPosCallback(renderer->window, mouse_callback);
    glfwSetScrollCallback(renderer->window, scroll_callback);
    glfwSetKeyCallback(renderer->window, key_callback);
    
    // Capture mouse
    glfwSetInputMode(renderer->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return 0;
    }
    
    // Configure OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE); // Enable anti-aliasing
    
    // Initialize camera
    camera_init(&renderer->camera);
    
    // Load shaders
    if (!shader_load_from_file(&renderer->shader, config->vertex_shader_path, config->fragment_shader_path)) {
        fprintf(stderr, "Failed to load shaders\n");
        return 0;
    }
    
    // Setup VAO and VBO for rendering
    glGenVertexArrays(1, &renderer->vao);
    glGenBuffers(1, &renderer->vbo);
    
    // Setup sphere mesh for rendering particles
    setup_sphere_mesh();
    
    // Initialize time
    renderer->last_frame_time = glfwGetTime();
    renderer->delta_time = 0.0f;
    
    // Initialize mouse position
    renderer->last_mouse_x = renderer->window_width / 2.0f;
    renderer->last_mouse_y = renderer->window_height / 2.0f;
    renderer->first_mouse = 1;
    
    // Initialize simulation state
    renderer->paused = 0;
    renderer->step_mode = 0;
    renderer->single_step = 0;
    
    // Clear key state
    for (int i = 0; i < 1024; i++) {
        renderer->keys[i] = 0;
    }
    
    return 1;
}

void renderer_main_loop(Renderer *renderer, Particle *particles, int particle_count, SimConfig *config) {
    while (!glfwWindowShouldClose(renderer->window)) {
        // Update delta time
        renderer_update_time(renderer);
        
        // Process input
        renderer_process_input(renderer);
        
        // Update physics if not paused
        if (!renderer->paused || renderer->single_step) {
            update_particle_system(particles, particle_count, config->time_step, config->integration_method);
            renderer->single_step = 0;
        }
        
        // Render frame
        renderer_render_frame(renderer, particles, particle_count);
        
        // Swap buffers and poll events
        glfwSwapBuffers(renderer->window);
        glfwPollEvents();
    }
}

void renderer_render_frame(Renderer *renderer, Particle *particles, int particle_count) {
    // Clear the screen
    glClearColor(0.2f, 0.0f, 0.2f, 1.0f); // Dark blue background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use our shader program
    shader_use(&renderer->shader);
    
    // Set up view and projection matrices
    float view_matrix[16];
    float projection_matrix[16];
    
    camera_get_view_matrix(&renderer->camera, view_matrix);
    camera_get_projection_matrix(&renderer->camera, projection_matrix, 
                               (float)renderer->window_width / (float)renderer->window_height);
    
    // Pass matrices to shader
    shader_set_mat4(&renderer->shader, "view", view_matrix);
    shader_set_mat4(&renderer->shader, "projection", projection_matrix);
    
    // Render each particle as a sphere
    for (int i = 0; i < particle_count; i++) {
        Particle *p = &particles[i];
        render_sphere(renderer, p->position, p->radius, p->color);
    }
}

void renderer_process_input(Renderer *renderer) {
    // Check if the window should close (ESC key)
    if (renderer->keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(renderer->window, 1);
    
    // Camera movement
    if (renderer->keys[GLFW_KEY_W])
        camera_process_keyboard(&renderer->camera, 0, renderer->delta_time); // Forward
    if (renderer->keys[GLFW_KEY_S])
        camera_process_keyboard(&renderer->camera, 1, renderer->delta_time); // Backward
    if (renderer->keys[GLFW_KEY_A])
        camera_process_keyboard(&renderer->camera, 2, renderer->delta_time); // Left
    if (renderer->keys[GLFW_KEY_D])
        camera_process_keyboard(&renderer->camera, 3, renderer->delta_time); // Right
    if (renderer->keys[GLFW_KEY_SPACE])
        camera_process_keyboard(&renderer->camera, 4, renderer->delta_time); // Up
    if (renderer->keys[GLFW_KEY_LEFT_CONTROL])
        camera_process_keyboard(&renderer->camera, 5, renderer->delta_time); // Down
}

void renderer_update_time(Renderer *renderer) {
    double current_time = glfwGetTime();
    renderer->delta_time = current_time - renderer->last_frame_time;
    renderer->last_frame_time = current_time;
}

void renderer_cleanup(Renderer *renderer) {
    // Delete VAO and VBO
    glDeleteVertexArrays(1, &renderer->vao);
    glDeleteBuffers(1, &renderer->vbo);
    
    // Delete shader program
    shader_delete(&renderer->shader);
    
    // Terminate GLFW
    glfwTerminate();
}

// GLFW callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    if (current_renderer) {
        current_renderer->window_width = width;
        current_renderer->window_height = height;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!current_renderer) return;
    
    if (current_renderer->first_mouse) {
        current_renderer->last_mouse_x = xpos;
        current_renderer->last_mouse_y = ypos;
        current_renderer->first_mouse = 0;
    }
    
    float xoffset = xpos - current_renderer->last_mouse_x;
    float yoffset = current_renderer->last_mouse_y - ypos; // Reversed: y ranges bottom to top
    
    current_renderer->last_mouse_x = xpos;
    current_renderer->last_mouse_y = ypos;
    
    camera_process_mouse_movement(&current_renderer->camera, xoffset, yoffset, 1);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (current_renderer)
        camera_process_mouse_scroll(&current_renderer->camera, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (!current_renderer) return;
    
    // Update key state
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            current_renderer->keys[key] = 1;
        else if (action == GLFW_RELEASE)
            current_renderer->keys[key] = 0;
    }
    
    // Special key handling for single press actions
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        current_renderer->paused = !current_renderer->paused;
    
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && current_renderer->paused)
        current_renderer->single_step = 1;
}

// Sphere rendering for particles
// We'll use a simplified version with icosphere generation
#define SPHERE_STACKS 16
#define SPHERE_SECTORS 32

static GLfloat *sphere_vertices = NULL;
static GLuint sphere_vertex_count = 0;

static void setup_sphere_mesh(void) {
    // Calculate number of vertices needed
    sphere_vertex_count = (SPHERE_STACKS + 1) * (SPHERE_SECTORS + 1) * 6; // 6 floats per vertex (position + normal)
    
    // Allocate memory for vertices
    sphere_vertices = (GLfloat*)malloc(sphere_vertex_count * sizeof(GLfloat));
    if (!sphere_vertices) {
        fprintf(stderr, "Failed to allocate memory for sphere mesh\n");
        return;
    }
    
    // Generate unit sphere vertices
    int vertex_index = 0;
    for (int i = 0; i <= SPHERE_STACKS; i++) {
        float stack_angle = M_PI / 2 - i * M_PI / SPHERE_STACKS; // From +90 to -90 degrees
        float xy = cosf(stack_angle);
        float z = sinf(stack_angle);
        
        for (int j = 0; j <= SPHERE_SECTORS; j++) {
            float sector_angle = j * 2 * M_PI / SPHERE_SECTORS; // From 0 to 360 degrees
            
            // Vertex position (x, y, z)
            float x = xy * cosf(sector_angle);
            float y = xy * sinf(sector_angle);
            
            // Position
            sphere_vertices[vertex_index++] = x;
            sphere_vertices[vertex_index++] = y;
            sphere_vertices[vertex_index++] = z;
            
            // Normal (same as position for a unit sphere)
            sphere_vertices[vertex_index++] = x;
            sphere_vertices[vertex_index++] = y;
            sphere_vertices[vertex_index++] = z;
        }
    }
}

static void render_sphere(Renderer *renderer, Vec3 position, float radius, Vec3 color) {
    shader_use(&renderer->shader);
    
    // Create model matrix (translation and scaling)
    float model_matrix[16] = {0};
    
    // Scale
    model_matrix[0] = radius;
    model_matrix[5] = radius;
    model_matrix[10] = radius;
    
    // Translation
    model_matrix[12] = position.x;
    model_matrix[13] = position.y;
    model_matrix[14] = position.z;
    model_matrix[15] = 1.0f;
    
    // Set uniforms
    shader_set_mat4(&renderer->shader, "model", model_matrix);
    shader_set_vec3(&renderer->shader, "objectColor", color.x, color.y, color.z);
    
    // Bind VAO
    glBindVertexArray(renderer->vao);
    
    // Update VBO with sphere data
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sphere_vertex_count * sizeof(GLfloat), sphere_vertices, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Draw sphere
    glDrawArrays(GL_TRIANGLE_STRIP, 0, sphere_vertex_count / 6);
    
    // Unbind
    glBindVertexArray(0);
}