#pragma once
#ifndef MYCLASS_H
#define MYCLASS_H

#include <iostream>
#include <string>
#include <vector>
//#include <opencv2/opencv.hpp>
//#include <pcl/common/common_headers.h>
//#include <pcl/common/common_headers.h>
//#include <pcl/features/normal_3d.h>
//#include <pcl/io/pcd_io.h>
//#include <pcl/console/parse.h>
//#include <pcl/io/io.h>

using namespace std;

#define PICTURE 0
#define POINTCLOUD 1
#define TXT 2
#define FV_MIN 0.0000001

//////////////////////////////////////////////////////////////////////////
//********************4.0(Discard things before 4.0)********************//
//////////////////////////////////////////////////////////////////////////
namespace sigma{
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
	
	Object(int _ObjectID, int _ParentNodeID)
	{
		ObjectID = _ObjectID;
		ParentNodeID = _ParentNodeID;
	}
};

class Position
{
public:
	double Posx, Posy, Posz;
	Position()
	{
		Posx = 0.0f; Posy = 0.0f; Posz = 0.0f;
	}
	Position(double x,double y,double z)
	{
		Posx = x; Posy = y; Posz = z;
	}
	void setPosition(double x,double y,double z)
	{
		Posx = x; Posy = y; Posz = z;
	}
};

class Attitude
{
public:
	double Attitudex, Attitudey, Attitudez, Attitudew;
	Attitude()
	{
		Attitudex = 0.0f; Attitudey = 0.0f; Attitudez = 0.0f; Attitudew = 0.0f;
	}
	Attitude(double x, double y, double z, double w)
	{
		Attitudex = x; Attitudey = y; Attitudez = z; Attitudew = w;
	}
	void setAttitude(double x, double y, double z, double w)
	{
		Attitudex = x; Attitudey = y; Attitudez = z; Attitudew = w;
	}
};

class Time
{
public:
	unsigned int s;
	unsigned int ns;
	Time()
	{
		s = 0;  ns = 0;
	}
	Time(unsigned int _s, unsigned int _ns)
	{
		s = _s;  ns = _ns;
	}
	void setTime(unsigned int _s, unsigned int _ns)
	{
		s = _s;  ns = _ns;
	}
};

//Geometry
class Geometry{
public:
	string ID;
	Position pos_est;
	Attitude atti_est;
	Position pos_obs;
	Attitude atti_obs;
	Time time;
	
	Geometry()
	{
		ID = "";
		pos_est.setPosition(0.0,0.0,0.0);
		atti_est.setAttitude(0.0,0.0,0.0,0.0);
		pos_obs.setPosition(0.0,0.0,0.0);
		atti_obs.setAttitude(0.0,0.0,0.0,0.0);
		time.setTime(0,0);
	}
};

class Transform {
public:
	Position pos;
	Attitude atti;
	Time time;

	Transform() {
		pos.setPosition(0.0,0.0,0.0);
		atti.setAttitude(0.0,0.0,0.0,0.0);
		time.setTime(0,0);
	};

    	Transform(const double Posx,  const double Posy,  const double Posz,
                  const double Attix, const double Attiy, const double Attiz, const double Attiw,
	          const unsigned int Time_s, const unsigned int Time_ns)
	{
		pos.Posx = Posx; pos.Posy = Posy; pos.Posz = Posz;
		atti.Attitudex = Attix; atti.Attitudey = Attiy; atti.Attitudez = Attiz; atti.Attitudew = Attiw;
		time.s = Time_s; time.ns = Time_ns;
	}

	~Transform() {};
};

class ResourceMeta {

public:
	string ID;
    	int ParentObjectID;
    	int Type;
   	int Format;
   	Transform transform;
   	string Path;
    	string Parameters;

   	ResourceMeta() {
		ID = "";
		ParentObjectID = 0;
		Type = 0;
		Format = 0;
		transform = {};
		Path = "";
		Parameters = ""; 
	};

  	ResourceMeta(const string &ID, const int ParentObjectID, const int Type,
        	     const int Format, const sigma::Transform &transform, const string Path, const string Parameters) :
       		     ID(ID), ParentObjectID(ParentObjectID), Type(Type),
       		     Format(Format), transform(transform), Path(Path), Parameters(Parameters){};

   	 ~ResourceMeta() {}

	//virtual void loadResourceFile(string full) {}
};

//Origin Radius Range
class Vector4
{
public:
	double x;
	double y;
	double z;
	Time time;

	Vector4()
	{
		x = 0.0; y = 0.0; z = 0.0; time.s = 0; time.ns =0;
	}
	Vector4(double coordinateX,double coordinateY,double coordinateZ,unsigned int _s,unsigned int _ns)
	{
		x = coordinateX; y = coordinateY; z = coordinateZ; time.s = _s; time.ns = _ns;
	}
	void setVector4(double coordinateX,double coordinateY,double coordinateZ,unsigned int _s,unsigned int _ns)
	{
		x = coordinateX; y = coordinateY; z = coordinateZ; time.s = _s; time.ns = _ns;
	}
};

class Vector3
{
public:
	double x;
	double y;
	double z;

	Vector3()
	{
		x = 0.0; y = 0.0; z = 0.0;
	}
	Vector3(double coordinateX,double coordinateY,double coordinateZ)
	{
		x = coordinateX; y = coordinateY; z = coordinateZ;
	}
	void setVector3(double coordinateX,double coordinateY,double coordinateZ)
	{
		x = coordinateX; y = coordinateY; z = coordinateZ;
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

class BoudingBox
{
public:
	Vector3 maxPoint, minPoint;
	BoudingBox()
	{
		maxPoint.x = 0.0f; maxPoint.y = 0.0f; maxPoint.z = 0.0f;
		minPoint.x = 0.0f; minPoint.y = 0.0f; minPoint.z = 0.0f;
	}
	BoudingBox(double maxx, double maxy, double maxz, double minx, double miny, double minz)
	{
		maxPoint.x = maxx; maxPoint.y = maxy; maxPoint.z = maxz;
		minPoint.x = minx; minPoint.y = miny; minPoint.z = minz;
	}
	void setBoudingBox(double maxx, double maxy, double maxz, double minx, double miny, double minz)
	{
		maxPoint.x = maxx; maxPoint.y = maxy; maxPoint.z = maxz;
		minPoint.x = minx; minPoint.y = miny; minPoint.z = minz;
	}

};
typedef struct tagUTMCoor
	{
		double x;
		double y;
	}UTMCorr;
typedef struct tagWGS84Coor
	{
		double lat;
		double log;
	}WGS84Corr;

typedef struct tagFrameWorkCoor
	{
		int number;
		char symbol;
		Position position;
		Time time;
	}sigmaCorr;
}

	
//Load




//Expandable
//Image
/*class ImageResource:public ResourceMeta
{
public:
	Mat ImageData;
	void loadResourceFile(string full)
	{
		this->ImageData = imread(full);
	}


};*/

//PointCloud
/*class PointCloudResource:public ResourceMeta
{
public:
	pcl::PointCloud<pcl::PointXYZ> PointCloud;
	void loadResourceFile(string full)
	{
		pcl::io::loadPCDFile<pcl::PointXYZ>(full, this->PointCloud);
	}
};*/

//SAVE
/*class Data_new
{
public:
	virtual void saveResourceFile(string full){}
};*/

//Expandable
//Image
/*class ImageData_new:public Data_new
{
public:
	Mat ImageData;
	void saveResourceFile(string full)
	{
		cv::imwrite(full,this->ImageData);
	}
};*/

//PointCloud
/*class PointCloudData_new:public Data_new
{
public:
	pcl::PointCloud<pcl::PointXYZ> PointCloud;
	void saveResourceFile(string full)
	{
		pcl::io::savePCDFile(full, this->PointCloud);
	}
};*/

#endif
