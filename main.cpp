#include <glad/glad.h>
#include <glad/glad.c>
#include <GLFW/glfw3.h>

#include <stdio.h>


void error_callback(int error, const char* description) {
    fprintf(stderr, "Error %d: %s\n", error, description);
}

int main() {
	glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        return 1;
    }

    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    int resx = 1280, resy = 720;
    GLFWwindow *window = glfwCreateWindow(resx, resy, "test", NULL, NULL);
    if (window == NULL) {
        return 2;
    }
     
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
#ifndef __APPLE
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Could not load Opengl functions");
        return 3;
    }
    printf("asd\n"); fflush(stdout);
#endif

    printf("here\n"); fflush(stdout);
    GLuint vao;
    glCreateVertexArrays(1, &vao);
    printf("here\n"); fflush(stdout);

    while (!glfwWindowShouldClose(window)) {
    	glfwPollEvents();

    	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GLFW_TRUE);

    	glfwSwapBuffers(window);
    }
 }