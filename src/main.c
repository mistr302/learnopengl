#define CGLM_SHARED
#define STB_IMAGE_IMPLEMENTATION
#include "glad/include/glad/glad.h"
#include "include/gl_utils.h"
#include "include/stb_image.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// settings
void render_gameobject(struct GameObject *go);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int main() {
  // glfw: initialize and configure
  // ------------------------------
  if (!glfwInit()) {
    printf("failed to initialize GLFW\n");
    exit(EXIT_FAILURE);
  };
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  // TODO: handle path to be available from anywhere, not just source dir
  // glfw window creation
  // --------------------
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mandelbrot", NULL, NULL);
  if (window == NULL) {
    printf("Couldnt create glfw window");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to load gl functions");
    return -1;
  }

  // build and compile our shader program
  // ------------------------------------
  // vertex shader
  struct Shader shader;
  int err;
  err = load_shaders("./src/shaders/vertex.glsl", "./src/shaders/fragment.glsl",
                     &shader);
  if (err) {
    printf("Failed to load shaders");
    exit(EXIT_FAILURE);
  }
  // link shaders
  int shaderProgram = create_shading_program(&shader);
  delete_shader(&shader);

  set_time_loc(glGetUniformLocation(shaderProgram, "u_time"));
  set_transform_loc(glGetUniformLocation(shaderProgram, "u_transform"));

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  // float vertices[] = {-1.0f, -1.0f, 3.0f, -1.0f, -1.0f, 3.0f};
  // float vertices[] = {0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f};
  float vertices[] = {
      // positions                      // colors                texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
  };
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  unsigned int texture = load_texture("./src/assets/wall.jpg");
  if (texture == -1) {
    printf("failed to load texture");
    exit(EXIT_FAILURE);
  }
  // note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the vertex attribute's bound vertex buffer object so afterwards we can
  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally
  // modify this VAO, but this rarely happens. Modifying other VAOs requires a
  // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
  // VBOs) when it's not directly necessary.
  glBindVertexArray(0);
  // uncomment this call to draw in wireframe polygons.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  struct GameObject go = {.VAO = VAO,
                          .vertex_count = 3,
                          .t = {.position = {0, 0, 0}, .rotation = 0.f},
                          .texture = texture};
  struct GameObject go2 = {.VAO = VAO,
                           .vertex_count = 3,
                           .t = {.position = {1, 1, 0}, .rotation = 0.f},
                           .texture = texture};
  struct GameObject go3 = {.VAO = VAO,
                           .vertex_count = 3,
                           .t = {.position = {-1, -1, 0}, .rotation = 0.f},
                           .texture = texture};
  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float timeValue = glfwGetTime();

    // float scale = sin(timeValue);
    // glm_scale(mat, (vec3){scale, scale, 1.});

    glUseProgram(shaderProgram);
    glUniform1f(get_time_loc(), timeValue);

    // activate the texture unit first before binding texture
    // seeing as we only have a single VAO there's no need to bind it
    // every time, but we'll do so to keep things a bit more organized
    //     glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0); // no need to unbind it every time

    render_gameobject(&go);
    render_gameobject(&go2);
    render_gameobject(&go3);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}
void render_gameobject(struct GameObject *go) {
  if (go == NULL) {
    // Handle
    exit(EXIT_FAILURE);
  }
  glBindVertexArray(go->VAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, go->texture);

  mat4 mat = GLM_MAT4_IDENTITY_INIT;

  glm_rotate(mat, glm_rad(go->t.rotation), (vec3){0, 0, 1.0f});
  glm_translate(mat, go->t.position);
  glUniformMatrix4fv(get_transform_loc(), 1, GL_FALSE, (float *)mat);
  glDrawArrays(GL_TRIANGLES, 0, go->vertex_count);
}
