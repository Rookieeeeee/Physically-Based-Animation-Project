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


Particle initialize_particle(){

    float const v_param = 0.03;
    Particle particle;
    particle.m = 5 * float(rand())/RAND_MAX;
    particle.r = 0.03*particle.m;
    particle.pos = {2*float(rand())/RAND_MAX -1, 2*float(rand())/RAND_MAX -1, 0.0};//initial position
    glm::vec3 v = {v_param*float(rand())/RAND_MAX, v_param*float(rand())/RAND_MAX, 0.0};
    particle.pre_pos = particle.pos - v;//calculating previous position based on randomized initial position and randomized velocity
    particle.f = {0.0, 0.0, 0.0};
    return particle;
} 

Particle initialize_still_particle(){

    float const v_param = 0.03;
    Particle particle;
    particle.m = 5 * float(rand())/RAND_MAX;
    particle.r = 0.03*particle.m+ 0.05;
    particle.pos = {2*float(rand())/RAND_MAX -1, 2*float(rand())/RAND_MAX -1, 0.0};//initial position
    particle.pre_pos = particle.pos;
    particle.f = {0.0, 0.0, 0.0};
    return particle;
}

void draw_line(glm::vec3 p1, glm::vec3 p2){
    glBegin(GL_LINES);
    glColor3d(1.0f, 0.0f, 1.0f);
    glVertex2d(p1[0], p1[1]); 
    glVertex2d(p2[0], p2[1]); 

    glEnd(); 

}

void draw_circle(double const x, double const y, double const r){
    int const N = 24;
    glBegin(GL_POLYGON);

    glColor3d(1.0f, 1.0f, 1.0f);
    for(int i = 0; i < N; ++i){
        glVertex2d(x+cos(2*M_PI/N*i)*r, y+sin(2*M_PI/N*i)*r); 
    }
    glEnd();
}

void verlet_update(Particle & particle){
    glm::vec3 tmp = particle.pos;
    particle.pos = 2.0f*particle.pos - particle.pre_pos + t_interval * t_interval * particle.f / particle.m;
    particle.pre_pos = tmp;
    //std::cout<<particle.pre_pos[0]<<", "<<particle.pos[0]<<std::endl;

}
    
void gravitation(Particle & p1, Particle & p2){
    float const G_const = 0.002;
    p1.f += G_const* p1.m*p2.m*(p2.pos-p1.pos)/float(pow(glm::length(p2.pos - p1.pos), 3));
    p2.f += G_const* p1.m*p2.m*(p1.pos-p2.pos)/float(pow(glm::length(p2.pos - p1.pos), 3));

}


void bead_on_edge(Particle & particle, glm::vec3 uni_edge_v){
    glm::vec3 v = particle.pos - particle.pre_pos;
    glm::vec3 v_para = glm::dot(v, uni_edge_v)*uni_edge_v;
    particle.pos = particle.pre_pos + v_para;
     
}


void bead_on_circle(Particle & particle, glm::vec3 p, float r){
    glm::vec3 vec = p - particle.pos;    
    particle.pos += (1-r/glm::length(vec)) * vec;
}

void collision(Particle & p1, Particle & p2){
    glm::vec3 v1 = p1.pos - p1.pre_pos, v2 = p2.pos - p2.pre_pos;
    glm::vec3 uni_v = (p2.pos - p1.pos)/glm::length(p2.pos - p1.pos);

    glm::vec3 v1_para = glm::dot(v1, uni_v)*uni_v;
    glm::vec3 v2_para = glm::dot(v2, uni_v)*uni_v;

    glm::vec3 v1_perpen = v1 - v1_para;
    glm::vec3 v2_perpen = v2 - v2_para;

    v1 = v1_perpen + (v1_para*(p1.m - p2.m)+ 2* p2.m * v2_para)/(p1.m + p2.m);
    v2 = v2_perpen + (v2_para*(p2.m - p1.m)+ 2* p1.m * v1_para)/(p1.m + p2.m);

       
    p1.pos = p1.pos + p2.m / (p1.m + p2.m) *(p1.r + p2.r - glm::length(p1.pos - p2.pos)) * (-uni_v);
    p2.pos = p2.pos + p1.m / (p1.m + p2.m) *(p1.r + p2.r - glm::length(p1.pos - p2.pos)) * uni_v;
    
    p1.pre_pos = p1.pos - v1; 
    p2.pre_pos = p2.pos - v2; 


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
            particle.pos[1] =  -1.0 + particle.r; 
            particle.pre_pos[1] = 2*tmp[1]-particle.pre_pos[1]+particle.pos[1] - tmp[1];
            //std::cout<<particle.pre_pos[1]<<","<<particle.pos[1]<<std::endl;
            break;
            
        case 3:
            particle.pos[1] =  1.0 - particle.r; 
            particle.pre_pos[1] = 2*tmp[1]-particle.pre_pos[1]+particle.pos[1] - tmp[1];
 
           break;
 
    }
    
}

void string_sup(Particle & particle, glm::vec3 p){
    glm::vec3 uni_v = (particle.pos - p)/glm::length(particle.pos - p);
    particle.f_sup = - glm::dot(particle.f_g, uni_v) *uni_v;
         
}
