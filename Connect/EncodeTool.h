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

#define A 0x67452301
#define B 0xefcdab89
#define C 0x98badcfe
#define D 0x10325476

#define shift(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))    
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))


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
	unsigned string_to_unsigned(char *s);
	
	uint32_t* fill(string str);
	void mainLoop(uint32_t M[]);
	string changeHex(int a);
	string getMD5(string source);

	//Old Conversion method
	Eigen::Matrix3d Quaternion2RotationMatrix(const double x,const double y,const double z,const double w);
	//New Conversion method
	//Rotation vector
	Eigen::AngleAxisd rv_init(double radian, Eigen::Vector3d AxisOfRotation);
	Eigen::Matrix3d rv2Maxtrix(Eigen::AngleAxisd rv);
	Eigen::Vector3d rv2Euler(Eigen::AngleAxisd rv);
	Eigen::Quaterniond rv2Quaternion(Eigen::AngleAxisd rv);
	//Rotation matrix
	Eigen::Vector3d rm2Euler(Eigen::Matrix3d rm);
	Eigen::AngleAxisd rm2RotationVector(Eigen::Matrix3d rm);
	Eigen::Quaterniond rm2Quaternion(Eigen::Matrix3d rm);
	//Euler Angle
	Eigen::Matrix3d ea2Matrix(Eigen::Vector3d ea);
	Eigen::Quaterniond ea2Quaternion(Eigen::Vector3d ea);
	Eigen::AngleAxisd ea2RotationVector(Eigen::Vector3d ea);
	//Quaternion
	Eigen::AngleAxisd q2RotationVector(Eigen::Quaterniond q);
	Eigen::Matrix3d q2Matrix(Eigen::Quaterniond q);
	Eigen::Vector3d q2Euler(Eigen::Quaterniond q);
	//Blend Quaternions without order
	Eigen::Quaterniond Blend2Rotations(Eigen::Quaterniond quaternion1, Eigen::Quaterniond quaternion2);
	Eigen::Quaterniond lnQuaternion(Eigen::Quaterniond quaternion);
	Eigen::Quaterniond addQuaternion(Eigen::Quaterniond quaternion1, Eigen::Quaterniond quaternion2);
	Eigen::Quaterniond expQuaternion(Eigen::Quaterniond quaternion);

	Eigen::Vector3d init_vector,temp_vector,temp_dir_vector;
	Eigen::Vector3d x_axiz,y_axiz,z_axiz,temp;
	Eigen::Matrix3d R;
	Eigen::Quaterniond Q,T,N;

	//For MD5
	const uint32_t k[64] = {
	0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
	0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,0x698098d8,
	0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,
	0xa679438e,0x49b40821,0xf61e2562,0xc040b340,0x265e5a51,
	0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
	0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,
	0xfcefa3f8,0x676f02d9,0x8d2a4c8a,0xfffa3942,0x8771f681,
	0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,
	0xbebfbc70,0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
	0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,0xf4292244,
	0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,
	0xffeff47d,0x85845dd1,0x6fa87e4f,0xfe2ce6e0,0xa3014314,
	0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391 };

	const uint32_t s[64] = { 7,12,17,22,7,12,17,22,7,12,17,22,7,
	12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
	4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,
	15,21,6,10,15,21,6,10,15,21,6,10,15,21 };
	
	constexpr static int step = 64 / sizeof(uint32_t);	
	uint32_t strlength;
	uint32_t atemp;
	uint32_t btemp;
	uint32_t ctemp;
	uint32_t dtemp;
};

#endif 
