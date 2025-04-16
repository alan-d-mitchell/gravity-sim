#ifndef CAMERA_H
#define CAMERA_H

#include "../utils/vector.h"

typedef struct {
    Vec3 position;       // Camera position
    Vec3 front;          // Direction the camera is looking
    Vec3 up;             // Camera up vector
    Vec3 right;          // Camera right vector
    Vec3 world_up;       // World up vector (usually 0,1,0)
    
    float yaw;           // Yaw angle (rotation around y-axis)
    float pitch;         // Pitch angle (rotation around x-axis)
    
    float movement_speed; // Movement speed
    float mouse_sensitivity; // Mouse sensitivity
    float zoom;          // Zoom level (field of view)
} Camera;

// Initialize camera with default values
void camera_init(Camera *camera);

// Get the view matrix for this camera
void camera_get_view_matrix(Camera *camera, float *view_matrix);

// Get the projection matrix
void camera_get_projection_matrix(Camera *camera, float *projection_matrix, float aspect_ratio);

// Process keyboard input for camera movement
void camera_process_keyboard(Camera *camera, int direction, float delta_time);

// Process mouse movement for camera rotation
void camera_process_mouse_movement(Camera *camera, float xoffset, float yoffset, int constrain_pitch);

// Process mouse scroll for camera zoom
void camera_process_mouse_scroll(Camera *camera, float yoffset);

// Update camera vectors (recalculate front, right, up vectors)
void camera_update_vectors(Camera *camera);

#endif /* CAMERA_H */