#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <unistd.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "particle.h"

float const t_interval = 0.05;
float const g_acc = 0.8;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    //如果按下ESC，把windowShouldClose设置为True，外面的循环会关闭应用
    if(key==GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);
        std::cout<<"ESC"<<mode;
}



int main(void)
{
    
    Particle particle;
    particle.r = 0.03;
    particle.pos = {0.0, 0.9, 0.0};//initial position
    particle.pre_pos = particle.pos;//previous position
    particle.m = 1.0;
    particle.f = {0.0, -particle.m * g_acc, 0.0};

    //初始化GLFW库
    if(!glfwInit())
        return -1;
    //创建窗口以及上下文
    GLFWwindow* window = glfwCreateWindow(800, 800, "Task 2", NULL, NULL);
    if(!window){
        //创建失败会返回NULL
        glfwTerminate();
    }
    //建立当前窗口的上下文
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, key_callback); //注册回调函数
    //循环，直到用户关闭窗口
    while(!glfwWindowShouldClose(window)){
        /*******轮询事件*******/
        glfwPollEvents();
       
        /*******渲染*******/
        //选择清空的颜色RGBA
        glClearColor(0.2, 0.3, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        


        verlet_update(particle); 

        if(particle.pos[1] - particle.r<= -1.0 ){ 
            reflect_wall(particle, 2);
        }

        //std::cout<<particle.pos[1]<<std::endl;

       //std::cout<<particle.pos[1]<<", "<< (particle.pos[1]+1)*g_acc+0.5*particle.v[1]*particle.v[1] <<std::endl;

        draw_circle(particle.pos[0], particle.pos[1], particle.r);      

        /******交换缓冲区，更新window上的内容******/
        glfwSwapBuffers(window);   
        usleep(t_interval*1000000);
    }
    glfwTerminate();
    return 0;
}
