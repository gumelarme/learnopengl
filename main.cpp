#include <cstddef>
#include <glad/glad.h>
#include <iostream>
//
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void checkCompilation(unsigned int shaderId, GLenum e);
void checkProgramLinking(unsigned int programId);

bool isWireframe = false;
int main() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(800, 600, "Lets Learn OpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 600, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // rendering codes
  // Build and linkk shader program

  const char *vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main(){ \n"
      "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\0";

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  checkCompilation(vertexShader, GL_VERTEX_SHADER);

  const char *fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main(){ \n"
      "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
      "}\0";

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  checkCompilation(fragmentShader, GL_FRAGMENT_SHADER);

  // shader program, link both vertex and fragment shader
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  checkProgramLinking(shaderProgram);
  // clear unneeded resources
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // triangle vertices
  float vertices[] = {
      -0.5f, -0.5f, 0.0f, // left foot
      0.5f,  -0.5f, 0.0f, // right foot
      0.0f,  0.5f,  0.0f, // tip
  };

  // bind Vertex Array Object
  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  // copy vertices array in a buffert for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // linking vertex attribute pointer
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0); // unbind

  if (isWireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.5f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // glBindVertexArray(0); // unbind?

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void checkCompilation(unsigned int shaderId, GLenum shaderType) {
  int success;
  char infoLog[512];
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

  std::string shaderTypeName;
  switch (shaderType) {
  case GL_VERTEX_SHADER:
    shaderTypeName = "VERTEX";
    break;
  case GL_FRAGMENT_SHADER:
    shaderTypeName = "FRAGMENT";
    break;
  default:
    shaderTypeName = "UNKNOWN";
  }

  if (!success) {
    glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
    std::cout << "ERR:SHADER::" << shaderTypeName << "::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
}

void checkProgramLinking(unsigned int programId) {
  int success;
  char infoLog[512];
  glGetProgramiv(programId, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(programId, 512, NULL, infoLog);
    std::cout << "ERR:SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
}
