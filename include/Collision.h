#ifndef __COLLISION__
#define __COLLISION__

#include "Point.h"
#include "Disc.h"
#include "Direction.h"
#include "Grid.h"
#include "Terrain.h"
#include "Guard.h"

void determine_front_cases(Disc agent, Direction dir, int dist, Case *front, Case *front_side1, Case *front_side2);
int risk_collide_wall(Disc agent, Direction dir, int dist, const Grid *grid);

void resize_mini_coordinate(double *x);
void resize_real_coordinate(double *x);
int travel_limit_in_y(Disc agent, Direction dir, int dist, const Grid *grid, double *ymax);
int travel_limit_in_x(Disc agent, Direction dir, int dist, const Grid *grid, double *xmax);
int travel_limit(Disc agent, Direction dir, int dist, const Grid *grid, double *limit);

int collide_wall(Disc *agent, Direction dir, double limit);
int collide_guard(Guard *guard, int index, const Guard *tab, int size);

#endif