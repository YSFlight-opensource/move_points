#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <set>
#include "help_function.h"
#include "fssimplewindow.h"
#include "point.h"
#include "CameraObject.h"

using namespace std;

int main(int argc, char **argv)
{
	/*CameraObject camera;
	camera.z = 160.0;
	camera.y = 50.0;
	camera.x = 160.0;
	camera.h = 0.75;
	camera.b = 0.0;
	camera.p = -0.25;*/
	//srand(time(NULL));

	// Generate random NUM pts location
	points Points;
	Points.init_locations();
	Points.set_neighbors();
	double score = Points.config_score();
	double old_score = 0.0;

	/* You may comment the main algorithm here and uncomment the 
	   drawing part for visualization*/
	/*************************************************************
	* Main Algorithm
	**************************************************************/
	while (old_score < score) {
		for (int i = 0; i < NUM; i++) {
			vector<double> gradient = get_direction(Points, i);
			double eta = get_eta(Points, gradient, i);
			point curr_old = Points.get_point(i);
			point curr_old_to = Points.get_point(curr_old.get_to_idx());
			double o_dist = calc_distance(curr_old, curr_old_to);

			Points.set_point_pos(i, gradient, eta);
			point curr_new = Points.get_point(i);
			point curr_new_to = Points.get_point(curr_new.get_to_idx());
			double n_dist = calc_distance(curr_new, curr_new_to);
			set<int> from = curr_old.get_from_idx();
			for (set<int>::iterator iter = from.begin(); iter != from.end(); iter++) {
				point zi = Points.get_point(*iter);
				if (dist_decrease(curr_old, curr_new, zi) == true) continue;
				else {
					nearer_nei(Points, i, *iter);
				}
			}
			Points.set_max_r();
			score = Points.config_score();
			old_score = score;
		}
	}
	/*********************************************************
	* End of Main Algorithm
	*********************************************************/

	/*********************************************************
	* Drawing part
	*********************************************************/
	/*FsOpenWindow(0, 0, 800, 600, 1);
	for (;;)
	{
	FsPollDevice();
	int wid, hei;
	FsGetWindowSize(wid, hei);
	auto key = FsInkey();
	if (FSKEY_ESC == key)
	{
	break;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, wid, hei);

	for (int i = 0; i < NUM; i++) {
		vector<double> gradient = get_direction(Points, i);
		double eta = get_eta(Points, gradient, i);
		point curr_old = Points.get_point(i);
		point curr_old_to = Points.get_point(curr_old.get_to_idx());
		double o_dist = calc_distance(curr_old, curr_old_to);

		Points.set_point_pos(i, gradient, eta);
		point curr_new = Points.get_point(i);
		point curr_new_to = Points.get_point(curr_new.get_to_idx());
		double n_dist = calc_distance(curr_new, curr_new_to);
		set<int> from = curr_old.get_from_idx();
		for (set<int>::iterator iter = from.begin(); iter != from.end(); iter++) {
			point zi = Points.get_point(*iter);
			if (dist_decrease(curr_old, curr_new, zi) == true) continue;
			else {
				nearer_nei(Points, i, *iter);
			}
		}
		Points.set_max_r();
		score = Points.config_score();
		old_score = score;
	}

	// Set up 3D drawing
	camera.SetUpCameraProjection();
	camera.SetUpCameraTransformation();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);


	// 3D drawing from here
	glColor3ub(0, 0, 255);

	// Draw grid
	glBegin(GL_LINES);
	int x;
	for (x = 0; x <= 100; x += 20)
	{
	glVertex3i(x, 0, 0);
	glVertex3i(x, 0, 100);
	glVertex3i(0, 0, x);
	glVertex3i(100, 0, x);
	glVertex3i(0, x, 0);
	glVertex3i(100, x, 0);
	glVertex3i(0, x, 0);
	glVertex3i(0, x, 100);
	glVertex3i(0, 0, x);
	glVertex3i(0, 100, x);
	glVertex3i(x, 0, 0);
	glVertex3i(x, 100, 0);
	}
	glEnd();

	Points.draw();

	// Set up 2D drawing
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (float)wid - 1, (float)hei - 1, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	FsSwapBuffers();
	FsSleep(25);
	}*/
	/****************************************************
	* End of Drawing part
	****************************************************/

	return 0;
}




