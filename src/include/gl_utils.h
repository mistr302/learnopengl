#pragma once
#include "../glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

struct Shader {
  unsigned int vertex;
  unsigned int fragment;
};
struct Transform {
  vec3 position;
  float rotation;
};
struct GameObject {
  struct Transform t;
  unsigned int VAO;
  unsigned int vertex_count;
  unsigned int texture;
};

int load_shaders(const char *vertex_shader_path,
                 const char *fragment_shader_path, struct Shader *out);

void delete_shader(struct Shader *shader);

unsigned int create_shading_program(struct Shader *shader);

int load_texture(const char *tex_path);
// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window);

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
int get_time_loc();
void set_time_loc(int timeLoc);
int get_transform_loc();
void set_transform_loc(int transformLoc);
