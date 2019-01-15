#include <iostream>
#include "CatmullRom.h"
#include <fstream>
#include <string.h>

//void demo_bezier();
//void demo_bspline();
void demo_catmullrom();

using namespace std;

int main(int argc,char** argv) {
	//demo_bezier();
	//demo_bspline();
	demo_catmullrom();
}
/*
void demo_bezier() {
	Curve* curve = new Bezier();
	curve->set_steps(100); // generate 100 interpolate points between the last 4 way points
	curve->add_way_point(Vector(1, 1, 0));
	curve->add_way_point(Vector(2, 3, 0));
	curve->add_way_point(Vector(3, 2, 0));
	curve->add_way_point(Vector(4, 6, 0));
	std::cout << "nodes: " << curve->node_count() << std::endl;
	std::cout << "total length: " << curve->total_length() << std::endl;
	for (int i = 0; i < curve->node_count(); ++i) {
		std::cout << "node #" << i << ": " << curve->node(i).toString() << " (length so far: " << curve->length_from_starting_point(i) << ")" << std::endl;
	}
	delete curve;
}

void demo_bspline() {
	Curve* curve = new BSpline();
	curve->set_steps(100); // generate 100 interpolate points between the last 4 way points
	curve->add_way_point(Vector(1, 1, 0));
	curve->add_way_point(Vector(2, 3, 0));
	curve->add_way_point(Vector(3, 2, 0));
	curve->add_way_point(Vector(4, 6, 0));
	std::cout << "nodes: " << curve->node_count() << std::endl;
	std::cout << "total length: " << curve->total_length() << std::endl;
	for (int i = 0; i < curve->node_count(); ++i) {
		std::cout << "node #" << i << ": " << curve->node(i).toString() << " (length so far: " << curve->length_from_starting_point(i) << ")" << std::endl;
	}
	delete curve;
}
*/
void demo_catmullrom() {
	Curve* curve = new CatmullRom();
	curve->set_steps(1); // generate 100 interpolate points between the last 4 way points
	/*
	curve->add_way_point(Vector(1, 1, 0));
	curve->add_way_point(Vector(2, 3, 0));
	curve->add_way_point(Vector(3, 2, 0));
	curve->add_way_point(Vector(4, 6, 0));
	 */
	ifstream in("/Users/leeseungjoon/Desktop/GitHub_SJLEE/CBNU_SCRC_2018_winter/DeadReckonging/spline_temp/temp_only_lat_lon.txt");
	if (!in.is_open()){
	    cout<<"input error!"<<endl;
	    return;
	}
	char inputString[30];
	while(!in.eof()){
	    in.getline(inputString, 30);
        char *ptr = strtok(inputString, "\t");
        float lon=(float)atoi(ptr)/1000;
        ptr = strtok(NULL, "\t");
        float lat=(float)atoi(ptr)/1000;
        curve->add_way_point(Vector(lon, lat, 0));
        cout<< "original node: "<<lon<<'\t'<<lat<<endl;
    }
	std::cout << "nodes: " << curve->node_count() << std::endl;
	std::cout << "total length: " << curve->total_length() << std::endl;
	for (int i = 0; i < curve->node_count(); ++i) {
		std::cout << "node #" << i << ": " << curve->node(i).toString() << " (length so far: " << curve->length_from_starting_point(i) << ")" << std::endl;
	}
	delete curve;
}