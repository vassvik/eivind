#include <stdio.h>  // printf, fprintf, stderr, sprintf, FILE, fopen, fclose, fread
#include <stdlib.h> // malloc, free
#include <math.h>   // sqrt

#ifndef __APPLE__
#include <glad/glad.h>
#include <glad/glad.c>
#else
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

#include <GLFW/glfw3.h>

#include "utils.hpp"
#include "math.hpp"


void calculate_frame_timings(GLFWwindow *window);
void error_callback(int error, const char* description);

char *read_entire_file(const char *filename);
int compile_shader(const char * file_path, GLuint shader_ID);
GLuint load_shaders(const char * vertex_file_path, const char * fragment_file_path);

int main() {
    //
    glfwSetErrorCallback(error_callback);

    //
    if (!glfwInit()) {
        return 1;
    }

    //
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //
    int resx = 1280, resy = 720;
    GLFWwindow *window = glfwCreateWindow(resx, resy, "Title", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 2;
    }

    //
    glfwMakeContextCurrent(window); 
    glfwSwapInterval(0);

    //
#ifndef __APPLE__
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Could not load Opengl functions");
        return 3;
    }
#endif

    //
	GLuint program = load_shaders("vertex_shader.vs", "fragment_shader.fs");
    if (program == 0) {
        printf("Could not load shaders. Exiting\n");
        glfwTerminate();
        return 4;
    }

    // create vao
    GLuint vao;
    glGenVertexArrays(1, &vao);

    Vec3f p = Vec3f(0.0, 0.0, 1.0);
    float theta = 170*PI/180.0;
    float phi = 0.0;
    float fov = 45.0;
    
    Vec3f r, f, u;

    //
    double mx_prev, my_prev;
    glfwGetCursorPos(window, &mx_prev, &my_prev);

    double t1 = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    //
    glClearColor(3.0/255, 72/255.0, 133/255.0, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        //
        calculate_frame_timings(window);

        double t2 = glfwGetTime();
        double dt = t2 - t1;
        t1 = t2;

        //
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GLFW_TRUE);

	    double deltaMoveForward = glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S);
	    double deltaMoveRight   = glfwGetKey(window, GLFW_KEY_D) - glfwGetKey(window, GLFW_KEY_A);
	    double deltaMoveUp      = glfwGetKey(window, GLFW_KEY_E) - glfwGetKey(window, GLFW_KEY_Q);

	    double mx, my;
	    glfwGetCursorPos(window, &mx, &my);
	    double dmx = mx - mx_prev, dmy = my - my_prev;
	    mx_prev = mx;
	    my_prev = my;

	    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {			
		    phi = phi - 0.1*dmx*(PI/180.0);
		    theta = theta + 0.1*dmy*(PI/180.0);
		    if (theta > PI) theta = PI;
		    if (theta < 0) theta = 0.0;
	    }

        float sinp = sin(phi),   cosp = cos(phi);
	    float sint = sin(theta), cost = cos(theta);
	    Vec3f f = Vec3f(cosp*sint, sinp*sint, cost);                   // forward vector, normalized, spherical coordinates
	    Vec3f r = Vec3f(sinp, -cosp, 0.0f);                            // right vector, relative to forward
	    Vec3f u = Vec3f(-cosp*cost, -sinp*cost, sint);                 // "up" vector, u = r x f

	    float speed = 1.0;
	    p = p + dt*speed*(deltaMoveForward*f + deltaMoveRight*r + deltaMoveUp*u);

        Mat4f Model;
        Mat4f View = view(r, u, f, p);
        Mat4f Projection = perspective(fov, resx/float(resy), 0.1, 1000.0);

        Mat4f MVP = Projection*View*Model;



        //
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(program);
        glUniform3f(glGetUniformLocation(program, "r"), r.x, r.y, r.z);
        glUniform3f(glGetUniformLocation(program, "u"), u.x, u.y, u.z);
        glUniform3f(glGetUniformLocation(program, "f"), f.x, f.y, f.z);
        glUniform3f(glGetUniformLocation(program, "cpos"), p.x, p.y, p.z);
        glUniform1f(glGetUniformLocation(program, "near"), 0.1);
        glUniform1f(glGetUniformLocation(program, "far"), 1000.0);
        glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, &MVP.M[0][0]); 

        glBindVertexArray(vao);

        glUniform1i(glGetUniformLocation(program, "draw_mode"), 0);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 2);
        glUniform1i(glGetUniformLocation(program, "draw_mode"), 1);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 32, 1);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 2);

        //
        glfwSwapBuffers(window);
    }

    //
    glfwTerminate();
    
    return 0;
}

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s (%d)\n", description, error);
}