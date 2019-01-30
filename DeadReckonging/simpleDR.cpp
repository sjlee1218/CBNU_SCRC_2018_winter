#include <string.h>
#include <iostream>
#include <fstream>
#include <math.h>

#define degree (M_PI/180.0)
using namespace std;

void DR(double lon_lat[2], double GPS_lon, double GPS_lat,double GPS_gSpeed, double GPS_head, double time){

    //gSpeed == 565 cm/s

    double moving_distance = GPS_gSpeed * time; // cm

    double moved_lat_dist = moving_distance*sin(GPS_head*degree);
    double moved_lon_dist = moving_distance*cos(GPS_head*degree);

    double moved_lat_degree = moved_lat_dist/8888400;
    double moved_lon_degree = moved_lon_dist/11100000;

    lon_lat[0] = GPS_lon+moved_lon_degree; //0 for longitude
    lon_lat[1] = GPS_lat + moved_lat_degree;
}


int main() {
    double lon_lat[2];

    ifstream in("/Users/leeseungjoon/Desktop/CBNU_2018_winer/fuckthat/ThinkSimple.txt");

    char first1[200];
    char first2[300];
    char second1[200];
    char second2[300];

    double time;

    double lat;
    double lon;
    double gSpeed, headMot;

    double time2;
    double lat2;
    double lon2;
    double gSpeed2, headMot2;

    if (!in.is_open())
        return 0;


    in.getline(first1, 200);
    in.getline(first2, 300);

    time = atof(first1);

    //127 이 경도 latitude
    char* tok2 = strtok(first2, "\t"); // lat
    char* tok3 = strtok(NULL, "\t"); // lon
    char* tok4 = strtok(NULL, "\t"); // gSpeed (cm/s)
    char* tok5 = strtok(NULL, "\t"); // headMot

    lat = atof(tok2);
    lon=atof(tok3);
    gSpeed=atof(tok4);
    headMot=atof(tok5);

    long double sum=0;
    long itr=0;

    double temp;
    while(!in.eof()){

        in.getline(second1, 200);
        in.getline(second2, 300);

        time2=atof(second1);

        //127 이 경도 latitude
        char* tok2 = strtok(second2, "\t"); // lat
        char* tok3 = strtok(NULL, "\t"); // lon
        char* tok4 = strtok(NULL, "\t"); // gSpeed (cm/s)
        char* tok5 = strtok(NULL, "\t"); // headMot

        lat2 = atof(tok2);
        lon2=atof(tok3);
        gSpeed2=atof(tok4);
        headMot2=atof(tok5);

        DR(lon_lat, lon,lat,gSpeed, headMot,abs(time2-time));

        double lon_diff_centi = (lon_lat[0] - lon2) * 11100000; // 예측값과 실제값의 longitude가 이만큼 차이 남
        double lat_diff_centi = (lon_lat[1] - lat2) * 8888400;
        // 예측값과 실제값의 차를 sum

        //if (sqrt(pow(lon_diff_centi, 2) + pow(lat_diff_centi, 2))>10)
            cout<<"시간 차: "<< abs(time2-time)<<" 거리 차: " << sqrt(pow(lon_diff_centi, 2) + pow(lat_diff_centi, 2))<<" gSpeed: "<<gSpeed<<" itr:"<<itr<<endl;


        temp = time;

        lat = lat2;
        lon= lon2;
        gSpeed =gSpeed2;
        headMot=headMot2;
        time =time2;
        itr++;

        if (abs(time2-temp) >10)
            continue;

        sum += sqrt(pow(lon_diff_centi, 2) + pow(lat_diff_centi, 2));

    }

    long double mean = sum /itr;

    cout << "mean error: "<< mean<<endl;
    return 0;
}
