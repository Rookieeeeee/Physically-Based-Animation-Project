extern float const t_interval;
typedef struct{
    glm::vec3 pos;  //position
    glm::vec3 pre_pos;  //previous position
    glm::vec3 f;    //force
    glm::vec3 f_sup, f_g;
    float m;    //mass
    float r;    //radius
    bool duration;
}Particle;

Particle initialize_particle();

Particle initialize_still_particle();

void draw_line(glm::vec3 p1, glm::vec3 p2);

void draw_circle(double const x, double const y, double const r);

void verlet_update(Particle & particle);

void gravitation(Particle & p1, Particle & p2);

void bead_on_edge(Particle & particle, glm::vec3 uni_edge_v);

void bead_on_circle(Particle & particle, glm::vec3 p, float r);

void collision(Particle & p1, Particle & p2);

void reflect_wall(Particle & particle, int mode); //mode = 0 left, = 1 right, = 3 bottom, = 4 top

void string_sup(Particle & particle, glm::vec3 p);
