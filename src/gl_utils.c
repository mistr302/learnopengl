#include "include/gl_utils.h"
#include "include/stb_image.h"
#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>

int load_shaders(const char *vertex_shader_path,
                 const char *fragment_shader_path, struct Shader *out) {
  if (out == NULL) {
    printf("out is null");
    return -1;
  }
  char *fragmentShaderSource = read_file(fragment_shader_path, NULL);
  if (!fragmentShaderSource) {
    printf("couldnt read fragment shader\n");
    return -1;
  }
  char *vertexShaderSource = read_file(vertex_shader_path, NULL);
  if (!vertexShaderSource) {
    printf("couldnt read vertex shader\n");
    return -1;
  }

  const char *vertexShaderSrc = vertexShaderSource;
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
  free(vertexShaderSource);

  glCompileShader(vertexShader);
  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("Vertex shader compilation failed:\n%s\n", infoLog);
    return -1;
  }
  // fragment shader
  const char *fragmentShaderSrc = fragmentShaderSource;
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
  free(fragmentShaderSource);

  glCompileShader(fragmentShader);
  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("Fragment shader compilation failed:\n%s\n", infoLog);
    return -1;
  }
  *out = (struct Shader){.vertex = vertexShader, .fragment = fragmentShader};
  return 0;
}
void delete_shader(struct Shader *shader) {
  glDeleteShader(shader->vertex);
  glDeleteShader(shader->fragment);
}
unsigned int create_shading_program(struct Shader *shader) {
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, shader->vertex);
  glAttachShader(shaderProgram, shader->fragment);
  glLinkProgram(shaderProgram);
  int success;
  char infoLog[512];
  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("Fragment shader compilation failed:\n%s\n", infoLog);
  }
  return shaderProgram;
}
int load_texture(const char *tex_path) {
  // load texture
  int width, height, nrChannels;
  unsigned char *data = stbi_load(tex_path, &width, &height, &nrChannels, 0);
  if (data == NULL) {
    printf("failed to load wall.jpg");
    return -1;
  }
  // gen texture object
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
  return texture;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}
