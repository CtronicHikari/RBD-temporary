#pragma once
#ifndef ENCODETOOL_H
#define ENCODETOOL_H


#include<iostream>
#include<string.h>
#include<stdio.h>
#include<math.h>
#include <sstream>
#include<ctime>

#include <Eigen/Eigen>
#include <stdlib.h>
#include <Eigen/Geometry>
#include <Eigen/Core>
#include <vector>

#include <algorithm>
#include "MyClass.h"

using namespace std;
using namespace Eigen;

class EncodeTool {
public:
	
	string string_replace(string &strPath, const string &str1, const string &str);
	string floatTranStr(double flo);
	string doubleToString(double dou);
	double convertStringToDouble(string &s);
	double random(double start, double end);
	double stol(const char *str);
	double str2double(const char *src);
	string randomStringByTime();
	std::vector<string> ParametersSplitter(string Parameters, char delimeter);
	std::vector<string> splitter(string target, char delimeter);
	template <class T>
	void convertFromString(T &value, const std::string &s);	

	Eigen::Quaterniond euler2Quaternion(const double roll, const double pitch, const double yaw);
	Eigen::Vector3d Quaterniond2Euler(const double x,const double y,const double z,const double w);

	Eigen::Matrix3d Quaternion2RotationMatrix(const double x,const double y,const double z,const double w);
	Eigen::Quaterniond rotationMatrix2Quaterniond(Eigen::Matrix3d R);
	Eigen::Matrix3d euler2RotationMatrix(const double roll, const double pitch, const double yaw);
	Eigen::Vector3d RotationMatrix2euler(Eigen::Matrix3d R);
	void toEulerAngle(double x,double y,double z,double w, double& roll, double& pitch, double& yaw);

	Eigen::Vector3d init_vector,temp_vector,temp_dir_vector;
	Eigen::Vector3d x_axiz,y_axiz,z_axiz,temp;
	Eigen::Matrix3d R;
	Eigen::Quaterniond Q;
	Eigen::Quaterniond T;

};

	/*cout<<"E to Q"<<endl;
	tool.euler2Quaternion(0,0,0);
	cout<<"Q to E"<<endl;
	tool.Quaterniond2Euler(0,0,0,1);
	cout<<"Q to R"<<endl;
	tool.Quaternion2RotationMatrix(0,0,0,1);
	cout<<"R to Q"<<endl;
  	tool.x_axiz << 1,0,0;
  	tool.y_axiz << 0,1,0;
  	tool.z_axiz << 0,0,1;
  	tool.R << tool.x_axiz,tool.y_axiz,tool.z_axiz;
  	tool.rotationMatrix2Quaterniond(tool.R);
	cout<<"E to R"<<endl;
	tool.euler2RotationMatrix(0,0,0);
	cout<<"R to E"<<endl;
	tool.RotationMatrix2euler(tool.R);
	cout << "All transform is done!" << endl;*/

#endif 
