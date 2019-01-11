
/*
     GNSS 의 느린 Hz를 보정해줄 DR 구현 스레드
 */

#include <stdio.h>
#include <iostream>
#include "env_setting.h"
#include <sys/time.h>
#include <unistd.h>
#include <queue>
#include <fstream>

// Read Coordinates Map Data
//void Coordinates_Map_Data();

#define DOT_STEERING_MODEL
//#define ACKERMANN_STEERING_BICYCLE_MODEL // Not working
//#define KINEMATIC_BICYCLE_MODEL
//#define ONLY_DGPS

//#define YAW_ANGLE_WRITE


void* Dead_Reckoning(void *DR_error_flag_)
{
    // 스티어링 각도의 변화량을 계산하기 위한 값.
    std::queue<double> steering_angle;
    steering_angle.push(0.0);

    // check running time
    double elapsedTime = 0;

#ifdef YAW_ANGLE_WRITE
    std::ofstream fin;
    
#ifdef __APPLE__
    fin.open("../../YAW_ANGLE/yaw_angle_save.txt");
#else
    fin.open("/home/chp/darknet_ros_ws/src/darknet_ros/ioniq_control/YAW_ANGLE/yaw_angle_save.txt");
#endif
    
#endif
    while(1)
    {
        if(gnss_initial_flag == 1)
        {
            // check running time
            struct timeval t1, t2;
//            double elapsedTime;
            
            // start timer
            gettimeofday(&t1, NULL);
            
//            usleep(100);
            
#if defined(DOT_STEERING_MODEL) || defined(ACKERMANN_STEERING_BICYCLE_MODEL)// || defined(BICYCLE_MODEL)
            // Dead reckoning
            // 시간 기준은 GNSS thread의 elapsedTime으로 잡음. 해당 갱신이 chassis CAN의 갱신보다 빠르면 약간의 오차가 발생함. 나중에 mutex로 잡으면 될 듯.
            
            // Dead Reckoning으로 yaw_angle 구하기
            //  rad/s?? degree/s?? * ms
            //  우선 deg/s로 계산 --> 환산하면 1/1000 deg / ms
            // 1deg/s = 1deg/1000ms
            
            //  얘 단위 수정해야함. **********************************************
            if(vehicle_yaw_rate_error_correct == 1)
                dr_yaw_rate_integral_val = -vehicle_yaw_rate / 1000 * elapsedTime;
            
            // steering angle 기반 dr yaw angle (재대로 계산)
//            steering_angle.push(vehicle_steering_angle * 0.06);//0.09);
//            double steering_angle_tmp = steering_angle.front();
////            std::cout << "steering_angle_tmp = " << steering_angle_tmp << std::endl;
//            steering_angle.pop();
//            steering_angle_tmp += -steering_angle.front();
//            dr_yaw_rate_integral_val = steering_angle_tmp;// / 1000 * elapsedTime;
            
            
            // 기존 yaw_angle에 적분된 yaw_rate (=yaw_angle) 더하기
//            gnss_yaw_angle += dr_yaw_rate_integral_val;                 // gnss_yaw_angle += 적분 vehicle_yaw_rate(chassis CAN);
//            dr_yaw_angle = gnss_yaw_angle;// + dr_yaw_rate_integral_val;
            
            // steering angle기반 dr yaw angle (야매)
//            dr_yaw_angle = gnss_yaw_angle - vehicle_steering_angle * 0.09;
            
            
            // 앞 - 뒤 (부호)
            
            // 스티어앵글 변화량
//            double steering_rate = vehicle_steering_angle * 0.09;
            
            // 이전 스티어앵글 각 - 지금 스티어앵글 값 에다 시간 계산해서 요레이트 뽑기?
            
            // yaw rate 누적에 대한 결과를 따로 빼서 계산. 얘가맞음!
            yaw_rate_accum_tmp += dr_yaw_rate_integral_val;
            if(vehicle_yaw_rate_error_correct == 1)
                gnss_yaw_angle = gnss_yaw_angle + dr_yaw_rate_integral_val;
            dr_yaw_angle = gnss_yaw_angle;
            
//            std::cout << "dr_yaw_angle = " << dr_yaw_angle << "\t\tyar_rate_accum_tmp = " << yaw_rate_accum_tmp << "\t\tdr_yaw_rate_integral_val = "<< dr_yaw_rate_integral_val << "\t\tvehicle_yaw_rate = "<< vehicle_yaw_rate << std::endl;
            
//            printf("dr_yaw_angle = %f\n", dr_yaw_angle);
#endif

            
#ifdef DOT_STEERING_MODEL // Coord 기준 : GNSS 설치 위치
            // x축 변화량 구하기 (elapsedTime을 이용해 적분 효과)
            // current_vehicle_speed가 kph이므로 m/ms로 바꾸려면 3600을 나눠주면 됨.
//            delta_dr_x = elapsedTime * (double)current_vehicle_speed / 3600. * cos(3.1415926 / 180. * dr_yaw_angle); // (m/ms)
            delta_dr_x = (double)current_vehicle_speed / 3600. * cos(3.1415926 / 180. * dr_yaw_angle); // (m/ms)

//            delta_dr_x = elapsedTime * (double)current_vehicle_speed / 3600. * cos(3.1415926 / 180. * (gnss_yaw_angle - vehicle_steering_angle * 0.09));
            
//            printf("current_vehicle_speed = %d\n", current_vehicle_speed);
//            printf("yaw_rate = %f\n", vehicle_yaw_rate);
            
            //            printf("elapsedTime * current_vehicle_speed / 3600 = %f\n", elapsedTime * (double)current_vehicle_speed / 3600.);
            //            printf("cos(3.1415926 / 180 * dr_yaw_angle) = %f\n", cos(3.1415926 / 180. * dr_yaw_angle));
            
            // 기존 x좌표에 x축 변화량 더하기
            gnss_x += (delta_dr_x * elapsedTime);
            dr_x = gnss_x;// + delta_dr_x;
            
            //            dr_hard_x += delta_dr_x;
            //            dr_x = dr_hard_x;
            
            
            // y축 변화량 구하기 (elapsedTime을 이용해 적분 효과)
            // current_vehicle_speed가 kph이므로 m/ms로 바꾸려면 3600을 나눠주면 됨.
            delta_dr_y = (double)current_vehicle_speed / 3600. * sin(3.1415926 / 180. * dr_yaw_angle);//cos(3.1415926 / 180. * (90. - dr_yaw_angle));
//            delta_dr_y = elapsedTime * (double)current_vehicle_speed / 3600. * sin(3.1415926 / 180. * (gnss_yaw_angle - vehicle_steering_angle * 0.09));
            //cos(3.1415926 / 180. * (90. - (gnss_yaw_angle - vehicle_steering_angle * 0.0819)));
            
            // 기존 y좌표에 y축 변화량 더하기
            gnss_y += (delta_dr_y * elapsedTime);
            dr_y = gnss_y;// + delta_dr_y;
            
            //            dr_hard_y += delta_dr_y;
            //            dr_y = dr_hard_y;
            
            
            //최종 출력 : dr_yaw_angle, dr_x, dr_y
           //std::cout << std::fixed;
           //std::cout.precision(7);
           //std::cout<< dr_x << "\t\t" << dr_y << "\t\t" << dr_yaw_angle << "\t\t" << "DR" << std::endl;
#endif
            
#ifdef ACKERMANN_STEERING_BICYCLE_MODEL
            dr_yaw_angle = dr_yaw_angle < 0.01 ? 0.01 : dr_yaw_angle;
            double r = 2.7 / (cos((90-dr_yaw_angle) * 3.1415926 / 180));
            
            double delta_theta_s = (double)current_vehicle_speed * elapsedTime / r * 180 / 3.1415926; // rad/s^2;

            double a1 = cos(-delta_theta_s * 3.1415926 / 180);
            double a2 = -sin(-delta_theta_s * 3.1415926 / 180);
            double a3 = sin(-delta_theta_s * 3.1415926 / 180);
            double a4 = cos(-delta_theta_s * 3.1415926 / 180);
            
            delta_dr_x = (a1 * dr_x + a2 * dr_y) - dr_x;
            delta_dr_y = (a3 * dr_x + a4 * dr_y) - dr_y;
            
            // 기존 x좌표에 x축 변화량 더하기
            gnss_x += (delta_dr_x * elapsedTime);
            dr_x = gnss_x;// + delta_dr_x;
            
            gnss_y += (delta_dr_y * elapsedTime);
            dr_y = gnss_y;// + delta_dr_y;

             //최종 출력 : dr_yaw_angle, dr_x, dr_y
           // std::cout << std::fixed;
           // std::cout.precision(7);
           // std::cout<< delta_theta_s << "\t\t" << delta_dr_x << "\t\t" << delta_dr_y << "\t\t" << dr_x << "\t\t" << dr_y << "\t\t" << dr_yaw_angle << "\t\t" << "DR" << std::endl;
#endif


#ifdef ONLY_DGPS
            
            dr_x = gnss_x;
            dr_y = gnss_y;
            dr_yaw_angle = gnss_yaw_angle;
#endif

#ifdef KINEMATIC_BICYCLE_MODEL // No IMS, Coord 기준 : 무게중심 ?
            double KBM_l_r = 1.57;//1.35; // m
            double KBM_l_f = 1.13;//1.35; // m
            
            steer_angle = steer_angle > 30 ? 30 : steer_angle;
            steer_angle = steer_angle < -30 ? -30 : steer_angle;
            
            double KBM_beta = atan(KBM_l_r / (KBM_l_r + KBM_l_f) * tan(3.1415926 / 180. * steer_angle * 30 / 500)) * 180. / 3.1415926;
            
            double KBM_psi_delta = (double)current_vehicle_speed / 3600 / KBM_l_r * sin(3.1415926 / 180. * KBM_beta);// * 180. / 3.141592;
            
            double KBM_psi = KBM_psi_delta * elapsedTime;    // KBM_psi_delta 부호, KBM_psi_delta를 deg/s로 가정 -> 1deg/s = 1deg/1,000ms
//            yaw_rate_accum_tmp += KBM_psi;
            
            // Method 1
            if(vehicle_yaw_rate_error_correct == 1)
                gnss_yaw_angle = gnss_yaw_angle + KBM_psi;
            dr_yaw_angle = gnss_yaw_angle;
            
            delta_dr_x = (double)current_vehicle_speed / 3600 * cos(3.1415926 / 180. * (dr_yaw_angle + KBM_beta)); // m/ms
            // 기존 x좌표에 x축 변화량 더하기
            gnss_x += (delta_dr_x * elapsedTime);
            dr_x = gnss_x;// + delta_dr_x;
            
            delta_dr_y = (double)current_vehicle_speed / 3600 * sin(3.1415926 / 180. * (dr_yaw_angle + KBM_beta)); // m/ms
            // 기존 x좌표에 x축 변화량 더하기
            gnss_y += (delta_dr_y * elapsedTime);
            dr_y = gnss_y;// + delta_dr_x;
            
//            // Method 2
//            delta_dr_x = (double)current_vehicle_speed / 3600 * cos(3.1415926 / 180. * (KBM_psi + KBM_beta)); // m/ms
//            // 기존 x좌표에 x축 변화량 더하기
//            gnss_x += (delta_dr_x * elapsedTime);
//            dr_x = gnss_x;// + delta_dr_x;
//
//            delta_dr_y = (double)current_vehicle_speed / 3600 * sin(3.1415926 / 180. * (KBM_psi + KBM_beta)); // m/ms
//            // 기존 x좌표에 x축 변화량 더하기
//            gnss_y += (delta_dr_y * elapsedTime);
//            dr_y = gnss_y;// + delta_dr_x;
#endif
            
#ifdef OpenCV_View_MAP
            //            pthread_mutex_lock(&mutex); // 잠금을 생성한다.
            // opencv viewer에 그릴 좌표 설정
            opencv_x = dr_x * opencv_viewer_zoom;
            opencv_y = dr_y * opencv_viewer_zoom;
            //            printf("elapsedTime = %f \t\t delta_dr_x = %f \t\t opencv_x = %f \t\t opencv_y = %f\n", elapsedTime, delta_dr_x, opencv_x, opencv_y);
            //            pthread_mutex_unlock(&mutex); // 잠금을 생성한다.
            
            // cv::circle을 이용한 좌표 그리기
            cv::Point dot(dr_x*opencv_viewer_zoom, dr_y*opencv_viewer_zoom);
            cv::circle(img, dot, 1, CvScalar(0,255,255));
            
            // img data에 접근하여 픽셀 변경을 통한 좌표 그리기
            //            img.data[(int)(dr_y*4) * img.cols * 3 + (int)(dr_x*4) * 3 + 0] = 0;
            //            img.data[(int)(dr_y*4) * img.cols * 3 + (int)(dr_x*4) * 3 + 1] = 255;
            //            img.data[(int)(dr_y*4) * img.cols * 3 + (int)(dr_x*4) * 3 + 2] = 255;
            
            //            usleep(1);
            //            count_dr++;
            //            printf("count_dr = %d\n", count_dr);
            // imshow는 main()에서. thread에서는 imshow 애러뜸.
#endif
            
#ifdef YAW_ANGLE_WRITE
            fin << std::fixed;
            fin.precision(8);
            fin << "GNSS heading" << "\t\t" << "dr_yaw_angle" << "\t\t" << "yaw_rate_accum_tmp" << std::endl;
    if(vehicle_yaw_rate_error_correct == 1)
    {
        fin << std::fixed;
        fin.precision(8);
        fin << GNSS_heading << "\t\t" << dr_yaw_angle << "\t\t" << yaw_rate_accum_tmp << std::endl;

        //std::cout << GNSS_heading << "\t\t" << dr_yaw_angle << "\t\t" << yaw_rate_accum_tmp << std::endl;
    }
    usleep(100000);
#endif

            // finish timer
            gettimeofday(&t2, NULL);
            
            // compute and print the elapsed time in millisec
            elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
            elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
//            std::cout << std::fixed;
//            std::cout.precision(4);
//            std::cout << elapsedTime << " ms.\n";
        }
    }

#ifdef YAW_ANGLE_WRITE
    fin.close();
#endif
}
