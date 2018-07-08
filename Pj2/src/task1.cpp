#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <unistd.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include "Eigen/Dense"

float const ks = 0, kd = 0;
float const alpha = 1;
float const t_interval = 0.01;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    //如果按下ESC，把windowShouldClose设置为True，外面的循环会关闭应用
    if(key==GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);
        std::cout<<"ESC"<<mode;
}

int main(void)
{
    //std::cout<<glm::length(glm::vec3({4.0,4.0,0.0}))<<std::endl;

    Square square;

    square.vertex[0].pos = {-0.2, 0.9, 0.0};//initial position
    square.vertex[0].pre_pos = square.vertex[0].pos;
           
    square.vertex[1].pos = {0.4, 0.7, 0.0};//initial position
    square.vertex[1].pre_pos = square.vertex[1].pos;

    square.vertex[2].pos = {0.2, 0.1, 0.0};//initial position
    square.vertex[2].pre_pos = square.vertex[2].pos;

    square.vertex[3].pos = {-0.4, 0.3, 0.0};//initial position
    square.vertex[3].pre_pos = square.vertex[3].pos;

    for(int i=0; i<4; i++){
        square.vertex[i].m = 1;
        square.vertex[i].f = {0, -0.8, 0};
        square.vertex[i].vec = {0, 0, 0};
   
    }

    //初始化GLFW库
    if(!glfwInit())
        return -1;
    //创建窗口以及上下文
    GLFWwindow* window = glfwCreateWindow(800, 800, "Task 1", NULL, NULL);
    if(!window){
        //创建失败会返回NULL
        glfwTerminate();
    }
    //建立当前窗口的上下文
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, key_callback); //注册回调函数
    usleep(5000000);

    //循环，直到用户关闭窗口
    while(!glfwWindowShouldClose(window)){
        /*******轮询事件*******/
        glfwPollEvents();
       
        /*******渲染*******/
        //选择清空的颜色RGBA
        glClearColor(0.2, 0.3, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        square_integration_update(square); 
/*
        for(int i=0; i<4; i++){
            std::cout<<i<<':'<<square.vertex[i].pre_vec[0]<<','<<square.vertex[i].pre_vec[1]<<std::endl;

            std::cout<<square.vertex[i].pos[1]<<std::endl;
        }
*/
        draw_square(square.vertex);      
//        std::cout<<square.vertex[0].pos[0]<< square.vertex[0].pos[1]<<std::endl;
//       std::cout<<square.vertex[1].pos[0]<< square.vertex[1].pos[1]<<std::endl;
       
        /******交换缓冲区，更新window上的内容******/
        glfwSwapBuffers(window);   
        usleep(t_interval*1000000);
    }
    glfwTerminate();
    return 0;
}
