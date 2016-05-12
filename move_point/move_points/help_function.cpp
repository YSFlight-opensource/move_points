#include "help_function.h"
#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include "point.h"

double fRand(double fMin, double fMax) {
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

/* Determine the gradient, reference to section 1.1 in pdf */
vector<double> get_direction(points Points, int curr_idx) {
	vector<double> gradient(3);
	point curr = Points.get_point(curr_idx);
	point to = Points.get_point(curr.get_to_idx());
	set<int> from_idx = curr.get_from_idx();
	gradient[0] = curr.x() - to.x();
	gradient[1] = curr.y() - to.y();
	gradient[2] = curr.z() - to.z();
	for (set<int>::iterator iter = from_idx.begin(); iter != from_idx.end(); iter++) {
		point from = Points.get_point(*iter);
		gradient[0] += (curr.x() - from.x());
		gradient[1] += (curr.y() - from.y());
		gradient[2] += (curr.z() - from.z());
	}
	return gradient;
}

double get_eta(points Points, vector<double> gradient, int curr_idx) {
	double eta = 2.0 / (sqrt(pow(gradient[0], 2) + pow(gradient[1], 2) + pow(gradient[2], 2)));
	// Constraint 1: Keep own nearest neighbor, reference to section 1.2
	// find points within k+4
	point curr = Points.get_point(curr_idx);
	point to = Points.get_point(curr.get_to_idx());
	set<int> from = curr.get_from_idx();
	double k = calc_distance(curr, to);
	double pos_x = curr.x() + k + 4.0;
	double neg_x = curr.x() - k - 4.0;
	double pos_y = curr.y() + k + 4.0;
	double neg_y = curr.y() - k - 4.0;
	double pos_z = curr.z() + k + 4.0;
	double neg_z = curr.z() - k - 4.0;
	int hash_idx_pos_x = (floor(pos_x / 10.0)) < 10 ? (int)(floor(pos_x / 10.0)) : 9;
	int hash_idx_neg_x = (floor(neg_x / 10.0)) >= 0 ? (int)(floor(neg_x / 10.0)) : 0;
	int hash_idx_pos_y = (floor(pos_y / 10.0)) < 10 ? (int)(floor(pos_y / 10.0)) : 9;
	int hash_idx_neg_y = (floor(neg_y / 10.0)) >= 0 ? (int)(floor(neg_y / 10.0)) : 0;
	int hash_idx_pos_z = (floor(pos_z / 10.0)) < 10 ? (int)(floor(pos_z / 10.0)) : 9;
	int hash_idx_neg_z = (floor(neg_z / 10.0)) >= 0 ? (int)(floor(neg_z / 10.0)) : 0;

	vector<int> b = Points.query(hash_idx_neg_x, hash_idx_pos_x, hash_idx_neg_y, hash_idx_pos_y, hash_idx_neg_z, hash_idx_pos_z);

	for (int i = 0; i < b.size(); i++) {
		if (b[i] == curr_idx) continue;
		point bi = Points.get_point(b[i]);
		double to_norm_sq = pow(to.x(), 2) + pow(to.y(), 2) + pow(to.z(), 2);
		double bi_norm_sq = pow(bi.x(), 2) + pow(bi.y(), 2) + pow(bi.z(), 2);
		double bi_mult_curr = bi.x()*curr.x() + bi.y()*curr.y() + bi.z()*curr.z();
		double to_mult_curr = to.x()*curr.x() + to.y()*curr.y() + to.z()*curr.z();
		double to_mult_grad = to.x()*gradient[0] + to.y()*gradient[1] + to.z()*gradient[2];
		double bi_mult_grad = bi.x()*gradient[0] + bi.y()*gradient[1] + bi.z()*gradient[2];
		double eta_temp = (to_norm_sq - bi_norm_sq + 2 * bi_mult_curr - 2 * to_mult_curr) / (2 * (to_mult_grad - bi_mult_grad));
		if (eta_temp >= 0 && eta_temp < eta) eta = eta_temp;
	}

	// Constraint 2: Keep other's nearest neighbor, reference to section 1.3
	pos_x = curr.x() + Points.get_max_r() + 2.0;
	neg_x = curr.x() - Points.get_max_r() - 2.0;
	pos_y = curr.y() + Points.get_max_r() + 2.0;
	neg_y = curr.y() - Points.get_max_r() - 2.0;
	pos_z = curr.z() + Points.get_max_r() + 2.0;
	neg_z = curr.z() - Points.get_max_r() - 2.0;

	hash_idx_pos_x = (floor(pos_x / 10.0)) < 10 ? (int)(floor(pos_x / 10.0)) : 9;
	hash_idx_neg_x = (floor(neg_x / 10.0)) >= 0 ? (int)(floor(neg_x / 10.0)) : 0;
	hash_idx_pos_y = (floor(pos_y / 10.0)) < 10 ? (int)(floor(pos_y / 10.0)) : 9;
	hash_idx_neg_y = (floor(neg_y / 10.0)) >= 0 ? (int)(floor(neg_y / 10.0)) : 0;
	hash_idx_pos_z = (floor(pos_z / 10.0)) < 10 ? (int)(floor(pos_z / 10.0)) : 9;
	hash_idx_neg_z = (floor(neg_z / 10.0)) >= 0 ? (int)(floor(neg_z / 10.0)) : 0;

	vector<int> p = Points.query(hash_idx_neg_x, hash_idx_pos_x, hash_idx_neg_y, hash_idx_pos_y, hash_idx_neg_z, hash_idx_pos_z);

	for (int i = 0; i < p.size(); i++) {
		if (p[i] == curr_idx || from.find(p[i]) != from.end()) continue;
		point pi = Points.get_point(p[i]);
		point pi_to = Points.get_point(pi.get_to_idx());
		double pi_mult_curr = pi.x()*curr.x() + pi.y()*curr.y() + pi.z()*curr.z();
		double pi_to_mult_pi = pi_to.x()*pi.x() + pi_to.y()*pi.y() + pi_to.z()*pi.z();
		double pi_to_norm_sq = pow(pi_to.x(), 2) + pow(pi_to.y(), 2) + pow(pi_to.z(), 2);
		double curr_norm_sq = pow(curr.x(), 2) + pow(curr.y(), 2) + pow(curr.z(), 2);
		double grad_mult_curr = gradient[0] * curr.x() + gradient[1] * curr.y() + gradient[2] * curr.z();
		double grad_norm_sq = pow(gradient[0], 2) + pow(gradient[1], 2) + pow(gradient[2], 2);
		double pi_mult_grad = pi.x()*gradient[0] + pi.y()*gradient[1] + pi.z()*gradient[2];

		double a = grad_norm_sq;
		double b = 2 * (grad_mult_curr - pi_mult_grad);
		double c = -(2 * pi_mult_curr - 2 * pi_to_mult_pi + pi_to_norm_sq - curr_norm_sq);

		if (sqrt(pow(b, 2) - 4 * a*c) < 0) continue;
		double alpha = (-b + sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);
		double beta = (-b - sqrt(pow(b, 2) - 4 * a*c)) / (2 * a);
		double eta_temp;
		if (alpha < beta) eta_temp = alpha;
		else eta_temp = beta;

		if (eta_temp >= 0 && eta_temp < eta) eta = eta_temp;
	}

	// Constraint 3: Do not move the point out of boundary.
	double eta_temp = (100.0 - curr.x()) / gradient[0];
	if (eta_temp >= 0 && eta_temp < eta) eta = eta_temp;
	eta_temp = (-curr.x()) / gradient[0];
	if (eta_temp >= 0 && eta_temp < eta) eta = eta_temp;
	eta_temp = (100.0 - curr.y()) / gradient[1];
	if (eta_temp >= 0 && eta_temp < eta) eta = eta_temp;
	eta_temp = (-curr.y()) / gradient[1];
	if (eta_temp >= 0 && eta_temp < eta) eta = eta_temp;
	eta_temp = (100.0 - curr.z()) / gradient[2];
	if (eta_temp >= 0 && eta_temp < eta) eta = eta_temp;
	eta_temp = (-curr.z()) / gradient[2];
	if (eta_temp >= 0 && eta_temp < eta) eta = eta_temp;

	return (0.999999)*eta;
}

bool dist_decrease(point curr_old, point curr_new, point zi) {
	double dist_old = calc_distance(curr_old, zi);
	double dist_new = calc_distance(curr_new, zi);
	if (dist_new < dist_old) return true;
	else return false;
}

// Update from and to of points.
void nearer_nei(points& Points, int curr_idx, int from_idx) {
	point curr = Points.get_point(curr_idx);
	point zi = Points.get_point(from_idx);
	double r = calc_distance(curr, zi);
	double pos_x = zi.x() + r;
	double neg_x = zi.x() - r;
	double pos_y = zi.y() + r;
	double neg_y = zi.y() - r;
	double pos_z = zi.z() + r;
	double neg_z = zi.z() - r;

	int hash_idx_pos_x = (floor(pos_x / 10.0)) < 10 ? (int)(floor(pos_x / 10.0)) : 9;
	int hash_idx_neg_x = (floor(neg_x / 10.0)) >= 0 ? (int)(floor(neg_x / 10.0)) : 0;
	int hash_idx_pos_y = (floor(pos_y / 10.0)) < 10 ? (int)(floor(pos_y / 10.0)) : 9;
	int hash_idx_neg_y = (floor(neg_y / 10.0)) >= 0 ? (int)(floor(neg_y / 10.0)) : 0;
	int hash_idx_pos_z = (floor(pos_z / 10.0)) < 10 ? (int)(floor(pos_z / 10.0)) : 9;
	int hash_idx_neg_z = (floor(neg_z / 10.0)) >= 0 ? (int)(floor(neg_z / 10.0)) : 0;

	vector<int> t = Points.query(hash_idx_neg_x, hash_idx_pos_x, hash_idx_neg_y, hash_idx_pos_y, hash_idx_neg_z, hash_idx_pos_z);

	double min_dist = r;
	int new_curr_idx = curr_idx;
	for (int i = 0; i < t.size(); i++) {
		if (t[i] == from_idx) continue;
		point ti = Points.get_point(t[i]);
		double dist_ti = calc_distance(zi, ti);

		// update from and to of each point
		if (dist_ti < min_dist) {
			Points.set_point_to(from_idx, t[i]);
			Points.delete_point_from(new_curr_idx, from_idx);
			Points.add_point_from(t[i], from_idx);
			min_dist = dist_ti;
			new_curr_idx = t[i];
		}
	}
}

double calc_distance(point &p1, point &p2) {
	double dist = sqrt(pow(p1.x() - p2.x(), 2) +
		pow(p1.y() - p2.y(), 2) +
		pow(p1.z() - p2.z(), 2));
	return dist;
}
