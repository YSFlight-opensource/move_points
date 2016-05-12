#ifndef _HELPF
#define _HELPF
#include <vector>
#include <set>
#include "point.h"

// Additional helping functions
double fRand(double fMin, double fMax);
vector<double> get_direction(points Points, int curr_idx);
double get_eta(points Points, vector<double> gradient, int curr_idx);
bool dist_decrease(point curr_old, point curr_new, point zi);
void nearer_nei(points& Points, int curr, int from);
double calc_distance(point &p1, point &p2);
#endif