#ifndef _POINTH
#define _POINTH
#include <vector>
#include <set>

#define NUM 1000
using namespace std;

class point {
private:
	double locations[3];
	int to_idx;   // the nearest neighbor of this point
	set<int> from_idx;  // this point is the nearest neighbor of points in the set

public:
	void set_locations(double x, double y, double z);
	void set_locations(vector<double> gradient, double eta);
	void set_to(int idx);
	void add_from(int idx);
	void delete_from(int idx);
	double x(void);
	double y(void);
	double z(void);
	int get_to_idx(void);
	set<int> get_from_idx(void);
};

class points {
private:
	point Points[NUM];
	vector<set<int> > *Points_hash;
	double max_r;

public:
	points(void);
	~points(void);
	void init_locations(void);
	int nearest_nei(int curr_idx);
	void set_neighbors(void);
	double config_score(void);
	void display(void);
	// void draw(void);
	point get_point(int idx);
	vector<int> query(int x_start, int x_end, int y_start, int y_end, int z_start, int z_end);
	double get_max_r(void);
	void set_point_pos(int idx, vector<double> gradient, double eta);
	void set_point_to(int curr, int to);
	void delete_point_from(int curr, int from);
	void add_point_from(int curr, int from);
	void set_max_r(void);
	void checker(void);
};

#endif