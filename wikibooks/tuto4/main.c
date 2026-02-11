#include <math.h>
#include <stddef.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct attributes
  {
  GLfloat coord3d[3];
  GLfloat color[3];
  };
GLFWwindow* window;
GLuint program;
GLuint vbo_triangle;
GLuint vbo_triangle_colors;
GLint attribute_coord3d;
GLint attribute_v_color;
GLint uniform_m_transform;


//utility n stuff
char* load_shader(const char* name)
  {
  FILE* fp;
  long size = 0;
  char* shader;

  //read for size
  if(!(fp = fopen(name, "rb")))
    return "";

  fseek(fp, 0, SEEK_END);
  size = ftell(fp)+1;
  fclose(fp);

  //read actual file
  fp = fopen(name, "r");
  shader = memset(malloc(size), '\0', size);
  if (!fread(shader, 1, size-1, fp))
    return "";

  fclose(fp);

  return shader;
  }


//gl related wrappers / code organization
void print_log(GLuint object)
  {
  GLint log_length = 0;
  if (glIsShader(object))
    {
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    }
  else if (glIsProgram(object))
    {
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    }
  else
    {
    fprintf(stderr, "printlog: not a shader or a program\n");
    return;
    }

  char* log = malloc(log_length);

  if (glIsShader(object))
    glGetShaderInfoLog(object, log_length, NULL, log);
  else if (glIsProgram(object))
      glGetProgramInfoLog(object, log_length, NULL, log);

  fprintf(stderr, "%s\n", log);
  free(log);
  }


GLuint create_shader(const char* filename, GLenum type)
  {
  void* source_mem = load_shader(filename);
  const GLchar* source = source_mem;

  if (!source)
    {
    fprintf(stderr, "error opening %s\n", filename);
    return 0;
    }

  GLuint res = glCreateShader(type);
  glShaderSource(res, 1, &source, NULL);
  free(source_mem);

  glCompileShader(res);
  GLint compile_ok = GL_FALSE;
  glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
  if (compile_ok == GL_FALSE)
    {
    fprintf(stderr, "an error ocurred while compiling %s: \n", filename);
    print_log(res);
    glDeleteShader(res);
    return 0;
    }

  return res;
  }


int init_resources()
  {
  struct attributes triangle_attributes[3] =
    {
      { .coord3d = { 0.0, 0.0, 0.0 }, .color = {1.0, 0.5, 0.0, } },
      { .coord3d = {-0.6,-0.6, 0.0 }, .color = {1.0, 1.0, 0.0, } },
      { .coord3d = { 0.6,-0.6, 0.0 }, .color = {1.0, 0.0, 0.0, } },
    };
  GLint compile_ok;
  GLint link_ok = GL_FALSE;
  GLuint vs;
  GLuint fs;
  const char* attribute_name;
  const char* uniform_name;

  glGenBuffers(1, &vbo_triangle);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

  if (!(vs = create_shader("vertex.glsl", GL_VERTEX_SHADER)))
    return 0;
  if (!(fs = create_shader("fragment.glsl", GL_FRAGMENT_SHADER)))
    return 0;

  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok)
    {
    printf("couldn't link program\n");
    print_log(program);
    return 1;
    }

  attribute_name = "coord3d";
  attribute_coord3d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord3d == -1)
    {
    printf("couldn't bind coords\n");
    return 1;
    }

  attribute_name = "v_color";
  attribute_v_color = glGetAttribLocation(program, attribute_name);
  if (attribute_v_color == -1)
    {
    printf("couldn't bind color\n");
    return 1;
    }

  uniform_name = "m_transform";
  uniform_m_transform = glGetUniformLocation(program, uniform_name);
  if (uniform_m_transform == -1)
    {
    printf("adada\n");
    return 1;
    }

  return 0;
  }


void free_resources()
  {
  glDeleteProgram(program);
  glDeleteBuffers(1, &vbo_triangle);
  }

void space_compute()
  {
  float angle = glfwGetTime()*3;
  float trans = sinf(angle);
  printf("time %g angle %g trnas %g\n", angle, cosf(angle), trans);
  float translate[16] =
    {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
    };
  float matrix[16] =
    {
    cosf(angle), sinf(angle), 0, 0,
    sinf(angle), -1*cosf(angle), 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
    };
  glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, matrix);
  }


//main flow declarations
void init()
  {
  GLenum glew_status;
  if (!glfwInit())
    {
    printf("couldn't initialize glfw\n");
    exit(1);
    }

  if (!(window = glfwCreateWindow(512, 512, "Hello World", NULL, NULL)))
    {
    glfwTerminate();
    exit(1);
    }

  glfwMakeContextCurrent(window);
  printf("%s\n", glGetString(GL_VERSION));//print version
  
  //if ((glew_status = glewInit()))
    //{
    //printf("glew init error: %s\n", glewGetErrorString(glew_status));
    //exit(1);
    //}

  if (init_resources())
    {
    printf("couldn't initialize resources\n");
    exit(1);
    }
  }


void terminate()
  {
  free_resources();
  glfwTerminate();
  }


void draw()
  {
  glUseProgram(program);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.f, 0.f, 0.f, 1.0f);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
  glEnableVertexAttribArray(attribute_coord3d);
  glEnableVertexAttribArray(attribute_v_color);
  glVertexAttribPointer
      (
      attribute_coord3d,
      3, //number of attributes per vertex: (x, y) in this case so two
      GL_FLOAT,
      GL_FALSE,
      sizeof(struct attributes),
      0
      );
  glVertexAttribPointer
      (
      attribute_v_color,
      3, //number of attributes per vertex: (r, g, b) in this case so two
      GL_FLOAT,
      GL_FALSE,
      sizeof(struct attributes),
      (GLvoid*) offsetof(struct attributes, color)
      );

  glDrawArrays(GL_TRIANGLES, 0, 3);
  glfwSwapBuffers(window);

  glDisableVertexAttribArray(attribute_coord3d);
  glDisableVertexAttribArray(attribute_v_color);
  }


void update()
  {
  glfwPollEvents();
  space_compute();
  }



//
int main(void)
  {
  init();

  while (!glfwWindowShouldClose(window))
    {
    draw();
    update();
    }

  terminate();
  return 0;
  }
