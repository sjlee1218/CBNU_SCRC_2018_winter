#include <iostream>
#include "CatmullRom.h"
#include "Bezier.h"
#include "BSpline.h"
#include <fstream>
#include <string.h>

void demo_bezier();
void demo_bspline();
void demo_catmullrom();

using namespace std;

int main(int argc,char** argv) {
	demo_bezier();
	demo_bspline();
	demo_catmullrom();

    ifstream in("/Users/leeseungjoon/Desktop/GitHub_SJLEE/CBNU_SCRC_2018_winter/DeadReckonging/spline_temp/temp_only_lat_lon.txt");
    ofstream out("/Users/leeseungjoon/Desktop/GitHub_SJLEE/CBNU_SCRC_2018_winter/DeadReckonging/spline_temp/just_lat_lon_tab.txt");

    if (!in.is_open()){
        cout<<"input error!"<<endl;
        return 0;
    }

    if (!out.is_open()){
        cout<<"input error!"<<endl;
        return 0;
    }

    int i=0;

    char inputString[30];
    while(!in.eof()){

        in.getline(inputString, 30);
        char *ptr = strtok(inputString, "\t");
        float lon=(float)atoi(ptr)/1000;
        ptr = strtok(NULL, "\t");
        float lat=(float)atoi(ptr)/1000;

        if ((i++)%4!=0)
            continue;

        out<<lon<<"\t"<<lat<<endl;

    }
    in.close();
    out.close();

}

void demo_bezier() {
	Curve* curve = new Bezier();
	curve->set_steps(10); // generate 10 interpolate points between the last 4 way points

	//curve->add_way_point(Vector(1, 1, 0));
	//curve->add_way_point(Vector(2, 3, 0));
	//curve->add_way_point(Vector(3, 2, 0));
	//curve->add_way_point(Vector(4, 6, 0));
    ifstream in("/Users/leeseungjoon/Desktop/GitHub_SJLEE/CBNU_SCRC_2018_winter/DeadReckonging/spline_temp/temp_only_lat_lon.txt");
    ofstream out("/Users/leeseungjoon/Desktop/GitHub_SJLEE/CBNU_SCRC_2018_winter/DeadReckonging/spline_temp/by_bezier.txt");

    if (!in.is_open()){
        cout<<"input error!"<<endl;
        return;
    }

    if (!out.is_open()){
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
        //out<<lon<<","<<lat<<endl;
    }
    in.close();

    std::cout << "nodes: " << curve->node_count() << std::endl;
    std::cout << "total length: " << curve->total_length() << std::endl;

    for (int i = 0; i < curve->node_count(); ++i) {
        if (i%4!=0)
            continue;
        out <<curve->node(i).toString()<<endl;
    }
    out.close();

    delete curve;
}

void demo_bspline() {
	Curve* curve = new BSpline();
	curve->set_steps(10); // generate 10 interpolate points between the last 4 way points

	//curve->add_way_point(Vector(1, 1, 0));
	//curve->add_way_point(Vector(2, 3, 0));
	//curve->add_way_point(Vector(3, 2, 0));
	//curve->add_way_point(Vector(4, 6, 0));

    ifstream in("/Users/leeseungjoon/Desktop/GitHub_SJLEE/CBNU_SCRC_2018_winter/DeadReckonging/spline_temp/temp_only_lat_lon.txt");
    ofstream out("/Users/leeseungjoon/Desktop/GitHub_SJLEE/CBNU_SCRC_2018_winter/DeadReckonging/spline_temp/by_BSpline.txt");

    if (!in.is_open()){
        cout<<"input error!"<<endl;
        return;
    }

    if (!out.is_open()){
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
        //out<<lon<<","<<lat<<endl;
    }
    in.close();

    std::cout << "nodes: " << curve->node_count() << std::endl;
    std::cout << "total length: " << curve->total_length() << std::endl;

    for (int i = 0; i < curve->node_count(); ++i) {
        if (i%4!=0)
            continue;
        out <<curve->node(i).toString()<<endl;
    }
    out.close();

	delete curve;
}

void demo_catmullrom() {
	Curve* curve = new CatmullRom();
	curve->set_steps(10); // generate 10 interpolate points between the last 4 way points

	ifstream in("/Users/leeseungjoon/Desktop/GitHub_SJLEE/CBNU_SCRC_2018_winter/DeadReckonging/spline_temp/temp_only_lat_lon.txt");
	ofstream out("/Users/leeseungjoon/Desktop/GitHub_SJLEE/CBNU_SCRC_2018_winter/DeadReckonging/spline_temp/by_catmull.txt");

	if (!in.is_open()){
	    cout<<"input error!"<<endl;
	    return;
	}

    if (!out.is_open()){
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
        //out<<lon<<","<<lat<<endl;
    }
	in.close();

	std::cout << "nodes: " << curve->node_count() << std::endl;
	std::cout << "total length: " << curve->total_length() << std::endl;

	for (int i = 0; i < curve->node_count(); ++i) {
        if (i%4!=0)
            continue;
		out <<curve->node(i).toString()<<endl;
	}
	out.close();

	delete curve;
}