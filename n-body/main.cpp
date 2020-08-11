/**
 * @author Zhitao Lin
 * @email zhitaolin@outlook.com
 * @create date 2020-08-11 17:35:35
 * @modify date 2020-08-11 17:35:35
 * @desc n-body
 */

#include <CL/cl.hpp>
#include <utils.hpp>
#include <iostream>
#include <math.h>

using namespace std;

typedef struct {
    float x;
    float y;
    float z;
} unit;

typedef unit vel;
typedef unit loc;
typedef unit acc;

float get_r(loc *loc0, loc *loc1, float eps);
void nBodySerially(const int n, float *m, vel *vs, loc *ls, const float G, const float eps);

int main(int argc, char **argvs) {
    // cout << kernel_name << endl; // print Kernel Name
    // utils::CL cl(kernel_source, kernel_name);
    // cl.run();
    loc ls[] ={
        { 0, 0, 0 },
        { 2, 2, 2 },
    };
    vel vs[] ={
        { 0, 0, 0 },
        { 1, 0, 0 },
    };
    float m[] ={
        1, 3
    };
    for(int i=0; i<10; i++) {
        nBodySerially(2, m, vs, ls, 6.67, 0.000001);
        cout << ls[0].x << ", " << ls[0].y << ", " << ls[0].z << endl;
        cout << ls[1].x << ", " << ls[1].y << ", " << ls[1].z << endl;
    }
}

void nBodySerially(const int n, float *m, vel *vs, loc *ls, const float G, const float eps) {
    loc *new_ls = (loc *)malloc(sizeof(loc) * n);
    for (int i=0; i<n; i++) {
        acc a;
        float r, ac, dx, dy, dz;
        float dt = eps;
        for (int j=0; j<n; j++) {
            if (i != j) {
                dx = ls[j].x - ls[i].x;
                dy = ls[j].y - ls[i].y;
                dz = ls[j].z - ls[i].z;
                r = get_r(&ls[i], &ls[j], eps);
                ac = G * m[j] / pow(r, 3); // ac = Gm/r3
                a.x += ac * dx;
                a.y += ac * dy;
                a.z += ac * dz;
            }
        }
        new_ls[i].x = ls[i].x + vs[i].x * dt + 0.5*pow(dt, 2)*a.x;
        new_ls[i].y = ls[i].y + vs[i].y * dt + 0.5*pow(dt, 2)*a.y;
        new_ls[i].z = ls[i].z + vs[i].z * dt + 0.5*pow(dt, 2)*a.z;
        vs[i].x = dt*a.x;
        vs[i].y = dt*a.y;
        vs[i].z = dt*a.z;
    }
    for (int i=0; i<n; i++) {
        ls[i].x = new_ls[i].x;
        ls[i].y = new_ls[i].y;
        ls[i].z = new_ls[i].z;
    }
    free(new_ls);
}

float get_r(loc *loc0, loc *loc1, float eps) {
    return sqrt(pow(abs(loc0->x - loc1->x), 2)
        + pow(abs(loc0->y - loc1->y), 2)
        + pow(abs(loc0->z - loc1->z), 2)) + eps;
}
