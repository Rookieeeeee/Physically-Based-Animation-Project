#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <unistd.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include <stdlib.h>
#include <ctime>
#include "Eigen/Dense"



void draw_square(Particle vertex[]){
    glBegin(GL_POLYGON);

    glColor3d(1.0f, 1.0f, 1.0f);
    for(int i = 0; i < 4; ++i){
        glVertex2d(vertex[i].pos[0], vertex[i].pos[1]); 

//        std::cout<<vertex[i].pos[0]<<", "<<vertex[i].pos[1]<<std::endl;
    }


    glEnd();
}

void draw_a_square(Eigen::Vector2f vertex[]){
    glBegin(GL_LINE_STRIP);

    glColor3d(1.0f, 0.0f, 1.0f);
    for(int i = 0; i < 4; ++i){
        glVertex2d(vertex[i][0], vertex[i][1]+0.3); 
    }
    glEnd();
}


void verlet_update(Particle & particle){
    glm::vec3 tmp = particle.pos;
    particle.pos = 2.0f*particle.pos - particle.pre_pos + t_interval * t_interval * particle.f / particle.m;
    particle.pre_pos = tmp;
    //std::cout<<particle.pre_pos[0]<<", "<<particle.pos[0]<<std::endl;

}
    
void square_integration_update(Square & square){
    Eigen::Matrix2f Apq, D, P, S, R; 
    Apq.setZero();
    Eigen::Vector2f p, q, goal[4], x0i[4], xi[4], x0cm, xcm, tmp;
    float m_sum = 0;
    Particle vertex_after[4]; 


    m_sum = 0;
    x0cm = {0, 0};
    for(int i = 0; i<4; i++){
        x0i[i] = {square.vertex[i].pos[0], square.vertex[i].pos[1]};
        x0cm += x0i[i]*square.vertex[i].m;
        m_sum += square.vertex[i].m;

    }
    x0cm = x0cm/m_sum;

    xcm = {0, 0};
    for(int i = 0; i<4; i++){
        vertex_after[i]= square.vertex[i]; 
        verlet_update(vertex_after[i]);

        if(vertex_after[i].pos[1] < -1.0 ){ 
            reflect_wall(vertex_after[i], 2);
        }

        xi[i] = {vertex_after[i].pos[0], vertex_after[i].pos[1]};

//        std::cout<<"xi "<<i<<':'<<xi[i][0]<<','<<xi[i][1]<<std::endl;
        
        xcm += xi[i]*square.vertex[i].m;
    }
    xcm = xcm/m_sum;
    for(int i = 0; i<4; i++){
        q = x0i[i] - x0cm;
        p = xi[i] - xcm;

        Apq += square.vertex[i].m * p * q.transpose();
    }

    Eigen::SelfAdjointEigenSolver<Eigen::Matrix2f> eigensolver(Apq.transpose() * Apq);
    D<< eigensolver.eigenvalues()[0], 0,
        0, eigensolver.eigenvalues()[1];
    P = eigensolver.eigenvectors();
    S = P * D.cwiseSqrt() * P.inverse();
    R = Apq * S.inverse();
//    std::cout<<"det R = "<<R.determinant()<<std::endl;

//   std::cout<<"x0cm = "<<x0cm[0]<<','<<x0cm[1]<<std::endl;
//    std::cout<<"xcm = "<<xcm[0]<<','<<xcm[1]<<std::endl;


    for(int i = 0; i<4; i++){
        goal[i] = R * (x0i[i] - x0cm) + xcm;
        tmp = goal[i] - xi[i];
        square.vertex[i].vec = square.vertex[i].vec + alpha * glm::vec3({tmp[0], tmp[1], 0})/t_interval + t_interval * square.vertex[i].f / square.vertex[i].m;    
        square.vertex[i].pre_pos = square.vertex[i].pos;
        square.vertex[i].pos = square.vertex[i].pos + t_interval * square.vertex[i].vec;
        if(square.vertex[i].pos[1] < -1.0 ){ 
            reflect_wall(square.vertex[i], 2);
        }


    }    
    //draw_a_square(xi);
}

void euler_spring_update(Square & square){
    glm::vec3 d, F, tmp_f[4];
    float l0, l;
    for(int i=0; i<4; i++){
        tmp_f[i] = square.vertex[i].f;
    }

    for(int i=0; i<4; i++){
        for(int j = i+1; j<4; j++){
            if ((i == 0 && j == 2)||(i == 1 && j == 3))l0 = square.len * sqrt(2);
            else l0 = square.len;
            l = float(glm::length(square.vertex[j].pos-square.vertex[i].pos));
            d = (square.vertex[j].pos-square.vertex[i].pos)/l;
            std::cout<<"i, j: "<<i<<","<<j<<std::endl;
            std::cout<<d[0]<<' '<<d[1]<<std::endl;
            std::cout<<"l0: "<<l0<<' '<<"l: "<<l<<std::endl;
            F = kd * (glm::dot(square.vertex[j].vec, d) - glm::dot(square.vertex[i].vec, d)) * d + ks * (l - l0)*d;
            square.vertex[i].f += F;
            square.vertex[j].f -= F;  
            std::cout<<"F: "<<F[0]<<','<<F[1]<<std::endl;

        }    
    }

    for(int i=0; i<4; i++){
        euler_update(square.vertex[i]);
        square.vertex[i].f = tmp_f[i];
        if(square.vertex[i].pos[1] < -1.0 ){ 
            reflect_wall(square.vertex[i], 2);
        }
    }
}

void euler_update(Particle & particle){
    particle.pos += t_interval * particle.vec;
    particle.vec += t_interval * particle.f/particle.m;     
}

void reflect_wall(Particle & particle, int mode){ //mode = 0 left, = 1 right, = 2 bottom, = 3 top
    glm::vec3 tmp = particle.pos;
    switch(mode){
        case 0:
            // flipping the pre position against current position, translating pre and c position to adjust current pos against the edge,
            
            particle.pos[0] =  -1.0 + particle.r; 
            particle.pre_pos[0] = 2*tmp[0]-particle.pre_pos[0]+particle.pos[0] - tmp[0];

            break;

        case 1:
            particle.pos[0] =  1.0 - particle.r; 
            particle.pre_pos[0] = 2*tmp[0]-particle.pre_pos[0]+particle.pos[0] - tmp[0];

            break;

        case 2:
            //std::cout<<particle.pre_pos[1]<<","<<particle.pos[1]<<","<<particle.pre_vec[1]<<std::endl;

            particle.pos[1] =  -1.0 + particle.r; 
            particle.pre_pos[1] = 2*tmp[1]-particle.pre_pos[1]+particle.pos[1] - tmp[1];
            particle.vec[1] = -particle.vec[1];
            //std::cout<<particle.pre_pos[1]<<","<<particle.pos[1]<<","<<particle.pre_vec[1]<<std::endl;
            break;
             
        case 3:
            particle.pos[1] =  1.0 - particle.r; 
            particle.pre_pos[1] = 2*tmp[1]-particle.pre_pos[1]+particle.pos[1] - tmp[1];
 
            break;
 
    }
    
}
