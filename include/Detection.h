#ifndef __DETECTION__
#define __DETECTION__

#include "Point.h"
#include "Disc.h"
#include "Guard.h"
#include "Player.h"
#include "Grid.h"
#include "Terrain.h"


double position_Pa(int a, Point point_0, Point point_1);
double position_Pb(int b, Point point_0, Point point_1);
int p_in_segment(double p);
double ordinate_Pa(int a, Point point_0, Point point_1);
double abscissa_Pb(int b, Point point_0, Point point_1);
Point x_higher(const Point point_1, const Point point_2);
Point x_lower(const Point point_1, const Point point_2);
Point y_higher(const Point point_1, const Point point_2);
Point y_lower(const Point point_1, const Point point_2);
Point copy_point(const Point point);
Case point_to_case(double x, double y);
int wall_between_point_guard(Point center, const Guard *guard, const Grid *grid);
int player_in_zone(const Player *player, const Guard *guard);
int detect_missing_relics(const Guard *guard, Grid *grid);

#endif