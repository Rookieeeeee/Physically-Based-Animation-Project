#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <unistd.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "particle.h"
#include <stdlib.h>
#include <ctime>

float const t_interval = 0.02;
int const N = 10;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    //如果按下ESC，把windowShouldClose设置为True，外面的循环会关闭应用
    if(key==GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);
    std::cout<<"ESC"<<mode;
}




int main(void)
{
    srand(time(0));


    Particle particle[N];
    for(int i = 0; i<N; ++i){
       particle[i] = initialize_particle(); 
    }
    
/*  Test case: two determined balls

    Particle particle[2];
    particle[0].m = 8;
    particle[0].r = 0.03*particle[0].m;
    particle[0].pos = {-0.5, 0.0, 0.0};//initial position
    particle[0].pre_pos = particle[0].pos;
    particle[0].f = {0.0, 0.0, 0.0};
    particle[0].pre_f = particle[0].f;
    particle[0].v = {1.0, 0.0, 0.0}; 

    particle[1].m = 8;
    particle[1].r = 0.03*particle[1].m;
    particle[1].pos = {0.5, 0.0, 0.0};//initial position
    particle[1].pre_pos = particle[1].pos;
    particle[1].f = {0.0, 0.0, 0.0};
    particle[1].pre_f = particle[1].f;
    particle[1].v = {-1.0, 0.5, 0.0}; 
*/
        //初始化GLFW库
    if(!glfwInit())
        return -1;
    //创建窗口以及上下文
    GLFWwindow* window = glfwCreateWindow(800, 800, "Task 4", NULL, NULL);
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

    for(int i = 0; i < N; ++i){
        verlet_update(particle[i]);

        if (particle[i].pos[0]-particle[i].r<-1.0){
            reflect_wall(particle[i], 0);
            continue;
        }
        if (particle[i].pos[0]+particle[i].r>1.0){
            reflect_wall(particle[i], 1);
            continue; 
        }
        if (particle[i].pos[1]-particle[i].r<-1.0){
            reflect_wall(particle[i], 2);
            continue; 
        }
        if (particle[i].pos[1]+particle[i].r>1.0){
            reflect_wall(particle[i], 3);
            continue;
        }

        for(int j = i+1; j< N; ++j){
            if(glm::length(particle[i].pos-particle[j].pos) < particle[i].r + particle[j].r){
                collision(particle[i], particle[j]);
            }
        }
    }   
    for(int i = 0; i < N; ++i){
        //std::cout<<particle[i].pos[0]<<","<< particle[i].pos[1]<<std::endl;
        draw_circle(particle[i].pos[0], particle[i].pos[1], particle[i].r);      
    }

        
        /******交换缓冲区，更新window上的内容******/
        glfwSwapBuffers(window);   
        usleep(t_interval*1000000);
    }
    glfwTerminate();
    return 0;
}
