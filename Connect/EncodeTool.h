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
#include <unordered_map>
#include <map>

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
	/*For FrameWork Coordinates*/
	//Ellipsoid model constants (actual values here are for WGS84)
	const double sm_a = 6378137.0;  //Ellipsoid model major axis
	const double sm_b = 6356752.314; //Ellipsoid model minor axis
	const double sm_EccSquared = 6.69437999013e-03;
	const double UTMScaleFactor = 0.9996;
	//Elevation of zone's central latitude and longitude
	unordered_map<string,double> Elevation = {{"54R",0.0},{"53R",0.0},{"52R",0.0},{"54S",0.0},{"53S",0.0},{"52S",112.0},{"54T",0.0},{"53T",464.0},{"52T",367.0}};
	
	//Converts degrees to radians & Convers radians to degrees
	inline double DegToRad(double deg) { return (deg / 180.0 * M_PI); }
	inline double RadToDeg(double rad) { return (rad / M_PI * 180.0); }
	//Get zone number by longitude,in radians
	inline int GetZoneNum(double lon) { return (int)RadToDeg(lon)/6+31; }
	//Get zone char by latitude,int radians
	char GetZoneChar(double lat);
	//Computes the ellipsoidal distance from the equator to a point at a given latitude.
	//Inputs:phi - Latitude of the point, in radians.
	//Returns:The ellipsoidal distance of the point from the equator, in meters
	double ArcLengthOfMeridian(double phi);
	//Determines the central meridian for the given UTM zone
	double UTMCentralMeridian(int zone); 
	double UTMCentralLatitude(char zone);
	//Computes the footpoint latitude for use in converting transverse Mercator coordinates to ellipsoidal coordinates
	//Inputs:y - The UTM northing coordinate, in meters
	//Returns:The footpoint lattitude, in radians
	double FootpointLatitude(double y);
	//Converts a latitude/longitude pair to x and y coordinates in the Transverse Mercator projection
	//Inputs: phi - Latitude of the point, int radians.  lambda - Longitude of the point, in radians.  lambda0 - Longitude of the central meridian to be used, in radians.
	//Outputs:xy - A 2-elementa array containing the x and y coordiantes of the point
	void MapLatLonToXY(double phi, double lambda, double lambda0, sigma::UTMCorr &xy);
	//Convers x and y coordinates in the Transvers Mercator projection to a latitude/longitude pair.
	//Inputs:x - the easting of the point, in meters.  y - The northing of the point, in meters.
	//Outputs: philambda - A 2-element containing the latitude and longitude in radians
	void MapXYToLatLon(double x, double y, double lambda0, sigma::WGS84Corr &philambda);
	//Converts a latitude/longitude pair to x and y coordinates in the Universal Transverse Mercator projection
	//Inputs:lat - Latitude of the point, in radians.  lon - Longitude of the point, in radians. zone - UTM zone to be used for calculating values for x and y
	//Outputs: xy - A 2-element arry where the UTM x and y values will be stored.
	void LatLonToUTMXY(double lat, double lon, int zone, sigma::UTMCorr &xy);
	//Convers x and y coordinates in the Universal Transverse Mercator projection to a latitude/longitude pair
	//Inputs:x - The easting of the point, in meters.  y - The northing of the point, in meters. zone - The UTM zone in which the point lies.  
	//southhemi - True if the point is in the southern hemisohere. flase otherwise
	//Outputs:latlon - A 2-elemet array containing the latitude and longitude of the point, in radians.
	void UTMXYToLatLon(double x, double y, int zone, bool southhemi, sigma::WGS84Corr &latlon);
	//Converts a latitude/longitude pair to sigma coordinates
	//Inputs:lat - Latitude of the point, in radians.  lon - Longitude of the point, in radians.  ele - GPS's elevation $GPGGA 9th data
	//Outputs:corr - simga Coordinates(Only No.zone and xy)
	void LatLonToSIGMA(double lat, double lon, double ele, sigma::Time *time, sigma::sigmaCorr &corr);
	void SIGMAToLatLon(sigma::sigmaCorr corr, sigma::WGS84Corr &latlon);
	//Framwork corrinate Index
	//Binarization of latitude and longitude
	//Inputs:w - The number to be converted to binary. left/right:Calculating the range of geohash step/max_step:Number of iterations
	//step - Always 1. max_step - Determine the length of the geohash, the length of the geohash:max_step/5
	string Geohash_w_bin(double w, double left, double right, int step, int max_step);
	//Passes in longitude and latitude and returns the combined binary code.
	string Geohash_merge(string Lon, string Lat);
	//Binary encoding base 32
	string Geohash_base32(string s);
	//Converts latitude and longitude to base32 Returns base32 encoding , in radians. precision Must be an even number.
	string Geohash_space(sigma::sigmaCorr corr, int precision=12);


	//Eigen variables
	Eigen::Vector3d init_vector,temp_vector,temp_dir_vector,init_vectorx, init_vectory;
	Eigen::Vector3d x_axiz,y_axiz,z_axiz,temp;
	Eigen::Matrix3d R;
	Eigen::Quaterniond Q,T,N;

	//For Base32
	map<string,string> base32 = {{"00000","0"},{"00001","1"},{"00010","2"},{"00011","3"},{"00100","4"},{"00101","5"},{"00110","6"},{"00111","7"},
				{"01000","8"},{"01001","9"},{"01010","b"},{"01011","c"},{"01100","d"},{"01101","e"},{"01110","f"},{"01111","g"},
				{"10000","h"},{"10001","j"},{"10010","k"},{"10011","m"},{"10100","n"},{"10101","p"},{"10110","q"},{"10111","r"},
				{"11000","s"},{"11001","t"},{"11010","u"},{"11011","v"},{"11100","w"},{"11101","x"},{"11110","y"},{"11111","z"}};
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
