#pragma once
#ifndef MYCLASS_H
#define MYCLASS_H

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include <pcl/common/common_headers.h>
#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
//#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>
#include <pcl/io/io.h>

using namespace std;
using namespace cv;

#define PICTURE 0
#define POINTCLOUD 1
#define TXT 2
#define FV_MIN 0.0000001

//////////////////////////////////////////////////////////////////////////
//********************4.0(Discard things before 4.0)********************//
//////////////////////////////////////////////////////////////////////////

//Object
class Object{
public:
	int ObjectID;
	int ParentNodeID;

	Object()
	{
		ObjectID = 0;
		ParentNodeID = 0;
	}
};

//Geometry
class Geometry{
public:
	string ID;
	double WorldPositionX;
	double WorldPositionY;
	double WorldPositionZ;
	double Time;
	
	Geometry()
	{
		ID = "";
		WorldPositionX = 0;
		WorldPositionY = 0;
		WorldPositionZ = 0;
		Time = 0;
	}
};

namespace fv{
class TransformA {
public:

    TransformA() {

    };

    TransformA(const double WorldPositionX, const double WorldPositionY, const double WorldPositionZ,
        const double Time, const double AttitudeX, const double AttitudeY, const double AttitudeZ, const double AttitudeW) :
        WorldPositionX(WorldPositionX), WorldPositionY(WorldPositionY), WorldPositionZ(WorldPositionZ),
        Time(Time), AttitudeX(AttitudeX), AttitudeY(AttitudeY), AttitudeZ(AttitudeZ), AttitudeW(AttitudeW){

    };

    ~TransformA() {

    };

    double WorldPositionX;
    double WorldPositionY;
    double WorldPositionZ;
    double Time;
    double AttitudeX;
    double AttitudeY;
    double AttitudeZ;
    double AttitudeW;
};
}

class ResourceMeta {

public:

   	ResourceMeta() {

    	};

  	  ResourceMeta(const std::string &ID, const int ParentObjectID, const int Type,
        	const std::string Ext, const fv::TransformA &transform, const std::string Path, const std::string Parameters) :
       		ID(ID), ParentObjectID(ParentObjectID), Type(Type),
       		Ext(Ext), transform(transform), Path(Path), Parameters(Parameters){

  	  };

   	 ~ResourceMeta() {

  	  }

	virtual void loadResourceFile(string full) {}

	std::string ID;
    	int ParentObjectID;
    	int Type;
   	std::string Ext;
   	fv::TransformA transform;
   	std::string Path;
    	std::string Parameters;
};



//Origin Radius Range
class Vector4
{
public:
	double x;
	double y;
	double z;
	double t;

	Vector4()
	{
		x = 0.0; y = 0.0; z = 0.0; t = 0;
	}
	Vector4(double coordinateX,double coordinateY,double coordinateZ,double time)
	{
		x = coordinateX; y = coordinateY; z = coordinateZ; t = time;
	}
	void setVector4(double coordinateX,double coordinateY,double coordinateZ,double time)
	{
		x = coordinateX; y = coordinateY; z = coordinateZ; t = time;
	}
};

//Frustum
class Frustum
{
public:
	float dir_x, dir_y, dir_z;
	float nearDis, farDis;
	float theta, alpha;
	double t;

	Frustum()
	{
		dir_x = 0.0; dir_y = 0.0; dir_z = 0.0; nearDis = 0.0; farDis = 0.0; theta = 0.0; alpha = 0.0; t = 0.0;
	}
	Frustum(float dirX, float dirY, float dirZ, float near, float far, float angle1, float angle2, double time)
	{
		dir_x = dirX; dir_y = dirY; dir_z = dirZ; nearDis = near; farDis = far; theta = angle1; alpha = angle2; t = time;
	}
	void setFrustum(float dirX, float dirY, float dirZ, float near, float far, float angle1, float angle2,double time)
	{
		dir_x = dirX; dir_y = dirY; dir_z = dirZ; nearDis = near; farDis = far; theta = angle1; alpha = angle2; t = time;
	}
	
};

class WorldPosition
{
public:
	double WorldPositionX, WorldPositionY, WorldPositionZ;
	WorldPosition()
	{
		WorldPositionX = 0.0f; WorldPositionY = 0.0f; WorldPositionZ = 0.0f;
	}
	WorldPosition(double x,double y,double z)
	{
		WorldPositionX = x; WorldPositionY = y; WorldPositionZ = z;
	}
	void setWorldPostion(double x,double y,double z)
	{
		WorldPositionX = x; WorldPositionY = y; WorldPositionZ = z;
	}
};

class Attitude
{
public:
	double AttitudeX, AttitudeY, AttitudeZ, AttitudeW;
	Attitude()
	{
		AttitudeX = 0.0f; AttitudeY = 0.0f; AttitudeZ = 0.0f; AttitudeW = 0.0f;
	}
	Attitude(double x, double y, double z, double w)
	{
		AttitudeX = x; AttitudeY = y; AttitudeZ = z; AttitudeW = w;
	}
	void setAttitude(double x, double y, double z, double w)
	{
		AttitudeX = x; AttitudeY = y; AttitudeZ = z; AttitudeW = w;
	}
};

class Target
{
public:
	double targetX, targetY, targetZ;
	Target()
	{
		targetX = 0.0f; targetY = 0.0f; targetZ = 0.0f; 
	}	
	Target(double x, double y, double z)
	{
		targetX = x; targetY = y; targetZ = z; 
	}
	void setTarget(double x, double y, double z)
	{
		targetX = x; targetY = y; targetZ = z; 
	}		
};

//Load




//Expandable
//Image
class ImageResource:public ResourceMeta
{
public:
	Mat ImageData;
	void loadResourceFile(string full)
	{
		this->ImageData = imread(full);
	}


};

//PointCloud
class PointCloudResource:public ResourceMeta
{
public:
	pcl::PointCloud<pcl::PointXYZ> PointCloud;
	void loadResourceFile(string full)
	{
		pcl::io::loadPCDFile<pcl::PointXYZ>(full, this->PointCloud);
	}
};

//SAVE
class Data_new
{
public:
	virtual void saveResourceFile(string full){}
};

//Expandable
//Image
class ImageData_new:public Data_new
{
public:
	Mat ImageData;
	void saveResourceFile(string full)
	{
		cv::imwrite(full,this->ImageData);
	}
};

//PointCloud
class PointCloudData_new:public Data_new
{
public:
	pcl::PointCloud<pcl::PointXYZ> PointCloud;
	void saveResourceFile(string full)
	{
		pcl::io::savePCDFile(full, this->PointCloud);
	}
};
















#endif
