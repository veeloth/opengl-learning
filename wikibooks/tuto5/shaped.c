#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>

#define PI 3.141593
#include "include/utils.c"
#include "include/vlinal.c"
#include "include/shapes.c"

const unsigned int triangles = 6 * 2; //number of triangles rendered
const unsigned int vertices_index = 0;
const unsigned int colors_index = 1;

unsigned int shader_program;
unsigned int vao;
GLFWwindow* window;
unsigned int uniform_transform;

//secondary
int init_res()
  {
  glEnable(GL_DEPTH_TEST);

  //models
  float vertices[] = { cube(-0.5, -0.5, -0.5, 1) };

  float vertex_colors[] =
    {
    1, 1, 0, 
    1, 1, 0, 
    1, 0, 1, 
    1, 0, 1, 

    1, 1, 0, 
    1, 1, 0, 
    1, 0, 1, 
    1, 0, 1, 

    1, 0, 0, 
    1, 0, 0, 
    0, 1, 0, 
    0, 1, 0, 

    1, 0, 0, 
    1, 0, 0, 
    0, 1, 0, 
    0, 1, 0, 

    0, 0, 0, 
    0, 0, 0, 
    1, 1, 1, 
    1, 1, 1, 

    0, 0, 0, 
    0, 0, 0, 
    1, 1, 1, 
    1, 1, 1, 

    };

  unsigned short triangle_indices[] = { cubeindices(0) };
  unsigned int triangles_ebo;
  unsigned int vertices_vbo;
  unsigned int colors_vbo;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &triangles_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangles_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_indices), triangle_indices, GL_STATIC_DRAW);

  glGenBuffers(1, &vertices_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(vertices_index, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(vertices_index);

  glGenBuffers(1, &colors_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_colors), vertex_colors, GL_STATIC_DRAW);

  glVertexAttribPointer(colors_index, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(colors_index);

  //unbind to prevent accidental modification
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //shaders
  GLuint vertex_shader;
  GLuint fragment_shader;
  if (!(vertex_shader = create_shader("shader/vertex.glsl", GL_VERTEX_SHADER)))
    return 1;
  if (!(fragment_shader = create_shader("shader/fragment.glsl", GL_FRAGMENT_SHADER)))
    return 1;

  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  if (!link_shader_program(shader_program))
    return 1;

  uniform_transform = glGetUniformLocation(shader_program, "transform");
  return 0;
  }

void space_compute()
  {
  struct mat4 transform = iden4;
  struct fa16 t16;
  transform = mul4(transform, pers4());
  transform = mul4(transform, trans4(0, 0, -3));
  transform = mul4(transform, rot4(0.15 * PI, 1, 2));
  transform = mul4(transform, rot4(0.05 * PI * glfwGetTime(), 0, 2));
  //transform = mul4(transform, rot4(0.20 * PI * glfwGetTime(), 0, 1));
  t16 = mat4_fa16(transform);
  glUniformMatrix4fv(uniform_transform, 1, GL_FALSE, t16.f);
  glBindVertexArray(vao);
  }


//main flow
void init()
  {
  if (!glfwInit())
    exit(1);

  GLFWmonitor* fullscreen_monitor = NULL;//windowed
  GLFWwindow* share = NULL;
  if ( !(window = glfwCreateWindow(512, 512, "cube", fullscreen_monitor, share)))
    {
    glfwTerminate();
    exit(1);
    }

  glfwMakeContextCurrent(window);

  if (!init_res())
    exit(1);
  }

void draw()
  {
    {
    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }


  glUseProgram(shader_program);
  glDrawElements(GL_TRIANGLES, triangles * 3, GL_UNSIGNED_SHORT, NULL);
  }

void update()
  {
  space_compute();
  glfwSwapBuffers(window);
  glfwPollEvents();
  }

int main(int argc, char* argv[argc+1])
  {
  init();

  while (!glfwWindowShouldClose(window))
    {
    update();
    draw();
    }
  return 0;
  }

