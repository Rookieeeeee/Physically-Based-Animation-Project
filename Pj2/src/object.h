extern float const alpha;
extern float const t_interval;
extern float const ks;
extern float const kd;

#include "Eigen/Dense"
typedef struct{
    glm::vec3 pos;  //position
    glm::vec3 pre_pos;  //previous position
    glm::vec3 vec;
    glm::vec3 f;    //force
    glm::vec3 f_sup, f_g;
    float r=0.01;
    float m;    //mass
    bool duration;
}Particle;

typedef struct{
    glm::vec3 pos;  //position
    glm::vec3 pre_pos;  //previous position
    glm::vec3 f;    //force
    glm::vec3 f_sup, f_g;
    Particle vertex[4];
    float m;    //mass
    float len;
    bool duration;
}Square;

void draw_square(Particle vertex[]);

void draw_a_square(Eigen::Vector2f vertex[]);

void verlet_update(Particle & particle);

void square_integration_update(Square & square);

void euler_spring_update(Square & square);

void euler_update(Particle & particle);

void reflect_wall(Particle & particle, int mode); //mode = 0 left, = 1 right, = 3 bottom, = 4 top

