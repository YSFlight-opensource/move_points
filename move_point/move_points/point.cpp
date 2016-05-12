#include "point.h"
#include <math.h>
#include <iostream>
#include "fssimplewindow.h"
#include "help_function.h"

// member functions in point class
void point::set_locations(double x, double y, double z) {
	locations[0] = x;
	locations[1] = y;
	locations[2] = z;
}
void point::set_to(int idx) {
	to_idx = idx;
}
void point::add_from(int idx) {
	from_idx.insert(idx);
}
void point::delete_from(int idx) {
	from_idx.erase(idx);
}
double point::x(void) {
	return locations[0];
}
double point::y(void) {
	return locations[1];
}
double point::z(void) {
	return locations[2];
}
int point::get_to_idx(void) {
	return to_idx;
}
set<int> point::get_from_idx(void) {
	return from_idx;
}


// member functions in points class
points::points(void) {
	Points_hash = new vector<set<int> >(1000);
	max_r = 0.0;
}
points::~points(void) {
	Points_hash->clear();
	delete [] Points_hash;
}
void points::init_locations(void) {
	for (int i = 0; i < NUM; i++) {
		double x = fRand(0.0, 100.0);
		double y = fRand(0.0, 100.0);
		double z = fRand(0.0, 100.0);
		Points[i].set_locations(x, y, z);
		int hash_idx = (int)(floor(x / 10.0) + floor(y / 10.0) * 10 + floor(z / 10.0) * 100);
		(*Points_hash)[hash_idx].insert(i);
	}
}
int points::nearest_nei(int curr_idx) {
	double min_dist = 9999.0;
	int nearest_nei_idx;
	for (int i = 0; i < NUM; i++) {
		if (i == curr_idx) continue;
		double dist = calc_distance(Points[i], Points[curr_idx]);
		if (dist < min_dist) {
			min_dist = dist;
			nearest_nei_idx = i;
		}
	}
	if (max_r < min_dist) max_r = min_dist;
	return nearest_nei_idx;
}
void points::set_neighbors(void) {
	for (int i = 0; i < NUM; i++) {
		int nearest_nei_idx = nearest_nei(i);
		Points[i].set_to(nearest_nei_idx);
		Points[nearest_nei_idx].add_from(i);
	}
}
double points::config_score(void) {
	double score = 0.0;
	for (int i = 0; i < NUM; i++) {
		int to = Points[i].get_to_idx();
		double dist = calc_distance(Points[i], Points[to]);
		score += dist;
	}
	cout << "score: " << score << endl;
	return score;
}
void points::display(void) {
	for (int i = 0; i < NUM; i++) {
		cout << i << ": " << Points[i].x() << "  " << Points[i].y() << "  " << Points[i].z() << "  " << endl;
	}
}
/*void points::draw(void) {
	glColor3d(1.0, 0.0, 0.0);
	glPointSize(4);
	glBegin(GL_POINTS);
	for (int i = 0; i < NUM; i++) {
		glVertex3d(Points[i].x(), Points[i].y(), Points[i].z());
	}
	glEnd();
}*/
point points::get_point(int idx) {
	return Points[idx];
}
vector<int> points::query(int x_start, int x_end, int y_start, int y_end, int z_start, int z_end) {
	vector<int> query_idx;
	for (int i = z_start; i <= z_end; i++) {
		for (int j = y_start; j <= y_end; j++) {
			for (int k = x_start; k <= x_end; k++) {
				int idx = i * 100 + j * 10 + k;
				set<int> points_set = (*Points_hash)[idx];
				for (set<int>::iterator iter = points_set.begin(); iter != points_set.end(); iter++) {
					query_idx.push_back(*iter);
				}
			}
		}
	}
	return query_idx;
}
double points::get_max_r(void) {
	return max_r;
}
void points::set_point_pos(int idx, vector<double> gradient, double eta) {
	if (eta < 0.0001) return;
	point curr = get_point(idx);
	double x = curr.x() + gradient[0] * eta;
	double y = curr.y() + gradient[1] * eta;
	double z = curr.z() + gradient[2] * eta;

	Points[idx].set_locations(x, y, z);

	// Update Point_hash
	int hash_idx_old = (int)(floor(curr.x() / 10.0) + floor(curr.y() / 10.0) * 10 + floor(curr.z() / 10.0) * 100);
	int hash_idx_new = (int)(floor(x / 10.0) + floor(y / 10.0) * 10 + floor(z / 10.0) * 100);
	if (hash_idx_new != hash_idx_old) {
		(*Points_hash)[hash_idx_old].erase(idx);
		(*Points_hash)[hash_idx_new].insert(idx);
	}
}
void points::set_point_to(int curr, int to) {
	Points[curr].set_to(to);
}
void points::delete_point_from(int curr, int from) {
	Points[curr].delete_from(from);
}
void points::add_point_from(int curr, int from) {
	Points[curr].add_from(from);
}
void points::set_max_r(void) {
	for (int i = 0; i < NUM; i++) {
		int to = Points[i].get_to_idx();
		double dist = calc_distance(Points[i], Points[to]);
		if (max_r < dist) max_r = dist;
	}
}
void points::checker(void) {
	for (int i = 0; i < NUM; i++) {
		int to = Points[i].get_to_idx();
		int to_save = nearest_nei(i);
		if (to != to_save) cout << i << ": " << " Wrong to " << to << " Correct: " << to_save << endl;
		set<int> from_idx = Points[i].get_from_idx();
		for (set<int>::iterator iter = from_idx.begin(); iter != from_idx.end(); iter++) {
			point from = get_point(*iter);
			int from_save = from.get_to_idx();
			int from_correct = nearest_nei(*iter);
			point wrong = get_point(from_save);
			point right = get_point(from_correct);
			double dist_wrong = sqrt(pow(from.x() - wrong.x(), 2) + pow(from.y() - wrong.y(), 2) + pow(from.z() - wrong.z(), 2));
			double dist_right = sqrt(pow(from.x() - right.x(), 2) + pow(from.y() - right.y(), 2) + pow(from.z() - right.z(), 2));
			if (from_save != from_correct) cout << "iter: " << *iter << " Wrong from " << from_save << ' ' << dist_wrong << " Correct: " << from_correct << ' ' << dist_right << endl;
			if (from_save != i) cout << "not correspond " << from_save << ' ' << i << endl;
		}
	}
}