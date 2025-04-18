#include "camera.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Camera movement directions
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
#define YAW         -90.0f
#define PITCH       0.0f
#define SPEED       2.5f
#define SENSITIVITY 0.1f
#define ZOOM        45.0f

void camera_init(Camera *camera) {
    camera->position = (Vec3){0.0f, 5.0f, 30.0f}; // Start 10 units back
    camera->world_up = (Vec3){0.0f, 1.0f, 0.0f};
    camera->yaw = YAW;
    camera->pitch = PITCH;
    camera->movement_speed = SPEED;
    camera->mouse_sensitivity = SENSITIVITY;
    camera->zoom = ZOOM;
    
    // Initialize front, right, up vectors
    camera_update_vectors(camera);
}

void camera_update_vectors(Camera *camera) {
    // Calculate the new front vector
    Vec3 front;
    front.x = cos(camera->yaw * M_PI / 180.0f) * cos(camera->pitch * M_PI / 180.0f);
    front.y = sin(camera->pitch * M_PI / 180.0f);
    front.z = sin(camera->yaw * M_PI / 180.0f) * cos(camera->pitch * M_PI / 180.0f);
    
    // Normalize the front vector
    float length = sqrtf(front.x * front.x + front.y * front.y + front.z * front.z);
    camera->front.x = front.x / length;
    camera->front.y = front.y / length;
    camera->front.z = front.z / length;
    
    // Recalculate the right and up vectors
    // Normalize the right vector
    camera->right.x = camera->front.y * camera->world_up.z - camera->front.z * camera->world_up.y;
    camera->right.y = camera->front.z * camera->world_up.x - camera->front.x * camera->world_up.z;
    camera->right.z = camera->front.x * camera->world_up.y - camera->front.y * camera->world_up.x;
    
    length = sqrtf(camera->right.x * camera->right.x + camera->right.y * camera->right.y + camera->right.z * camera->right.z);
    camera->right.x /= length;
    camera->right.y /= length;
    camera->right.z /= length;
    
    // Recalculate the up vector
    camera->up.x = camera->right.y * camera->front.z - camera->right.z * camera->front.y;
    camera->up.y = camera->right.z * camera->front.x - camera->right.x * camera->front.z;
    camera->up.z = camera->right.x * camera->front.y - camera->right.y * camera->front.x;
    
    length = sqrtf(camera->up.x * camera->up.x + camera->up.y * camera->up.y + camera->up.z * camera->up.z);
    camera->up.x /= length;
    camera->up.y /= length;
    camera->up.z /= length;
}

void camera_get_view_matrix(Camera *camera, float *view_matrix) {
    // Calculate the view matrix (a simplified version of the LookAt matrix)
    Vec3 target;
    target.x = camera->position.x + camera->front.x;
    target.y = camera->position.y + camera->front.y;
    target.z = camera->position.z + camera->front.z;
    
    Vec3 f;
    f.x = target.x - camera->position.x;
    f.y = target.y - camera->position.y;
    f.z = target.z - camera->position.z;
    
    float length = sqrtf(f.x * f.x + f.y * f.y + f.z * f.z);
    f.x /= length;
    f.y /= length;
    f.z /= length;
    
    Vec3 s;
    s.x = f.y * camera->world_up.z - f.z * camera->world_up.y;
    s.y = f.z * camera->world_up.x - f.x * camera->world_up.z;
    s.z = f.x * camera->world_up.y - f.y * camera->world_up.x;
    
    length = sqrtf(s.x * s.x + s.y * s.y + s.z * s.z);
    s.x /= length;
    s.y /= length;
    s.z /= length;
    
    Vec3 u;
    u.x = s.y * f.z - s.z * f.y;
    u.y = s.z * f.x - s.x * f.z;
    u.z = s.x * f.y - s.y * f.x;
    
    view_matrix[0] = s.x;
    view_matrix[1] = u.x;
    view_matrix[2] = -f.x;
    view_matrix[3] = 0.0f;
    
    view_matrix[4] = s.y;
    view_matrix[5] = u.y;
    view_matrix[6] = -f.y;
    view_matrix[7] = 0.0f;
    
    view_matrix[8] = s.z;
    view_matrix[9] = u.z;
    view_matrix[10] = -f.z;
    view_matrix[11] = 0.0f;
    
    view_matrix[12] = -(s.x * camera->position.x + s.y * camera->position.y + s.z * camera->position.z);
    view_matrix[13] = -(u.x * camera->position.x + u.y * camera->position.y + u.z * camera->position.z);
    view_matrix[14] = (f.x * camera->position.x + f.y * camera->position.y + f.z * camera->position.z);
    view_matrix[15] = 1.0f;
}

void camera_get_projection_matrix(Camera *camera, float *projection_matrix, float aspect_ratio) {
    // Calculate the projection matrix (perspective projection)
    float tan_half_fov = tanf((camera->zoom * 0.5f) * M_PI / 180.0f);
    float near_plane = 0.1f;
    float far_plane = 1000.0f;
    
    memset(projection_matrix, 0, 16 * sizeof(float));
    
    projection_matrix[0] = 1.0f / (aspect_ratio * tan_half_fov);
    projection_matrix[5] = 1.0f / tan_half_fov;
    projection_matrix[10] = -(far_plane + near_plane) / (far_plane - near_plane);
    projection_matrix[11] = -1.0f;
    projection_matrix[14] = -(2.0f * far_plane * near_plane) / (far_plane - near_plane);
}

void camera_process_keyboard(Camera *camera, int direction, float delta_time) {
    float velocity = camera->movement_speed * delta_time;
    
    if (direction == FORWARD) {
        camera->position.x += camera->front.x * velocity;
        camera->position.y += camera->front.y * velocity;
        camera->position.z += camera->front.z * velocity;
    }
    if (direction == BACKWARD) {
        camera->position.x -= camera->front.x * velocity;
        camera->position.y -= camera->front.y * velocity;
        camera->position.z -= camera->front.z * velocity;
    }
    if (direction == LEFT) {
        camera->position.x -= camera->right.x * velocity;
        camera->position.y -= camera->right.y * velocity;
        camera->position.z -= camera->right.z * velocity;
    }
    if (direction == RIGHT) {
        camera->position.x += camera->right.x * velocity;
        camera->position.y += camera->right.y * velocity;
        camera->position.z += camera->right.z * velocity;
    }
    if (direction == UP) {
        camera->position.x += camera->up.x * velocity;
        camera->position.y += camera->up.y * velocity;
        camera->position.z += camera->up.z * velocity;
    }
    if (direction == DOWN) {
        camera->position.x -= camera->up.x * velocity;
        camera->position.y -= camera->up.y * velocity;
        camera->position.z -= camera->up.z * velocity;
    }
}

void camera_process_mouse_movement(Camera *camera, float xoffset, float yoffset, int constrain_pitch) {
    xoffset *= camera->mouse_sensitivity;
    yoffset *= camera->mouse_sensitivity;
    
    camera->yaw += xoffset;
    camera->pitch += yoffset;
    
    // Constrain pitch to avoid gimbal lock
    if (constrain_pitch) {
        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;
    }
    
    // Update front, right and up vectors
    camera_update_vectors(camera);
}

void camera_process_mouse_scroll(Camera *camera, float yoffset) {
    // Adjust zoom level (FOV)
    camera->zoom -= yoffset;
    
    // Constrain zoom level
    if (camera->zoom < 1.0f)
        camera->zoom = 1.0f;
    if (camera->zoom > 45.0f)
        camera->zoom = 45.0f;
}