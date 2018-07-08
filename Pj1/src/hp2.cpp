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

float const t_interval = 0.03;
float const g_acc = 2.0;
int const N = 5;//Number of simulating objects. *Remember to change!

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    //如果按下ESC，把windowShouldClose设置为True，外面的循环会关闭应用
    if(key==GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);
    std::cout<<"ESC"<<mode;
}




int main(void)
{
    srand(time(0));

/*
    Particle particle[N];
    for(int i = 0; i<N; ++i){
       particle[i] = initialize_particle(); 
    }
*/    

    Particle particle_a[3];
    particle_a[0].m = 5;
    particle_a[0].r = 0.02*particle_a[0].m;
    particle_a[0].pos = {-0.7, 0.3, 0.0};//initial position
    particle_a[0].pre_pos = particle_a[0].pos;
    particle_a[0].f_g = {0.0, -particle_a[0].m * g_acc, 0.0};

    particle_a[1].m = 5;
    particle_a[1].r = 0.02*particle_a[1].m;
    particle_a[1].pos = {-0.2, 0.0, 0.0};//initial position
    particle_a[1].pre_pos = particle_a[1].pos;
    particle_a[1].f_g = {0.0, -particle_a[1].m * g_acc, 0.0};

    particle_a[2].m = 5;
    particle_a[2].r = 0.02*particle_a[2].m;
    particle_a[2].pos = {0.0, 0.0, 0.0};//initial position
    particle_a[2].pre_pos = particle_a[2].pos;
    particle_a[2].f_g = {0.0, -particle_a[2].m * g_acc, 0.0};

    particle_a[3].m = 5;
    particle_a[3].r = 0.02*particle_a[3].m;
    particle_a[3].pos = {0.2, 0.0, 0.0};//initial position
    particle_a[3].pre_pos = particle_a[3].pos;
    particle_a[3].f_g = {0.0, -particle_a[3].m * g_acc, 0.0};

    particle_a[4].m = 5;
    particle_a[4].r = 0.02*particle_a[4].m;
    particle_a[4].pos = {0.4, 0.0, 0.0};//initial position
    particle_a[4].pre_pos = particle_a[4].pos;
    particle_a[4].f_g = {0.0, -particle_a[4].m * g_acc, 0.0};


    glm::vec3 sup_point[N]; 
    for(int i = 0; i<N; i++){
        sup_point[i] = {i*0.2-0.4, 0.3, 0.0}; 
    }

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
        string_sup(particle_a[i], sup_point[i]);
        particle_a[i].f = particle_a[i].f_sup + particle_a[i].f_g;
        verlet_update(particle_a[i]);

        bead_on_circle(particle_a[i], sup_point[i], 0.5);

        if (particle_a[i].pos[0]-particle_a[i].r<-1.0){
            reflect_wall(particle_a[i], 0);
            continue;
        }
        if (particle_a[i].pos[0]+particle_a[i].r>1.0){
            reflect_wall(particle_a[i], 1);
            continue; 
        }
        if (particle_a[i].pos[1]-particle_a[i].r<-1.0){
            reflect_wall(particle_a[i], 2);
            continue; 
        }
        if (particle_a[i].pos[1]+particle_a[i].r>1.0){
            reflect_wall(particle_a[i], 3);
            continue;
        }

        for(int j = i+1; j< N; ++j){
            if(glm::length(particle_a[i].pos-particle_a[j].pos) < particle_a[i].r + particle_a[j].r){
                collision(particle_a[i], particle_a[j]);
            }
        }
        draw_circle(particle_a[i].pos[0], particle_a[i].pos[1], particle_a[i].r);      
	draw_line(particle_a[i].pos, sup_point[i]);
    }   

        
        /******交换缓冲区，更新window上的内容******/
        glfwSwapBuffers(window);   
        usleep(t_interval*1000000);
    }
    glfwTerminate();
    return 0;
}
