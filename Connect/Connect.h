#pragma once
#ifndef CONNECT_H
#define CONNECT_H

#include<mysql/mysql.h>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<math.h>
#include"MyClass.h"
#include"EncodeTool.h"

//#include<ctime>
//#include<cstdlib>
//#include<unistd.h> 
//#include<sys/types.h> 
//#include<sys/wait.h> 
//#include<stdio.h> 
//#include<stdlib.h> 
//#include<fcntl.h> 
//#include<limits.h> 
//#include<iostream>
//#include <typeinfo>
//#include<cstring>
using namespace std;

class DBConnect{
public:
	const char * db = "server"; //DataBase name
	const char * server = "127.0.0.1"; //server ip
	const char * user = "root"; //user
	const char * password = "root"; //password
	const unsigned int port = 3306; //port

	int res;
	MYSQL * pcon;
	MYSQL_RES * result = NULL;
	MYSQL_ROW sql_row;
	MYSQL_FIELD *fd;
	EncodeTool encodeTool;

	//Connect&Disconnect
	const void Connect();
	const void Disconnect();
	//Add Object & Geometry & Resource
	const void addObject(sigma::Object *Object);
	const string addGeometry(sigma::Position *pos_est, sigma::Attitude *atti_est, sigma::Position *pos_obs, sigma::Attitude *atti_obs, sigma::Time *time);
	const void addResource(string ResourceID, int ParentObjectID, int Type, int Format, string Parameters, string save_path="./data");
	//Link Object/Resrouce & Geometry
	const void link_Geo_and_Obj(int ObjectID, string GeometryID);
	const void link_Geo_and_Res(string ResourceID, string GeometryID);

 	const string addGeometryForObject(string ID, int style, sigma::Position *pos_offset, sigma::Attitude *atti_offset, sigma::Time *time); //TODO  //style:1.By Object   2.By Resource
	string make_ResourceID(int ParentObjectID, sigma::Position *pos, sigma::Attitude *atti, sigma::Time *time);

	//Search func
	const std::vector<sigma::Geometry> loadGeometry(sigma::Vector4 *Origin, sigma::Vector4 *Range, int Mode);
	const std::vector<sigma::Object> loadObject(sigma::Vector4 *Origin, sigma::Vector4 *Range, int Mode);
	sigma::ResourceMeta readResourceInformation(MYSQL_ROW src);
	std::vector<sigma::ResourceMeta> loadResourceMeta(sigma::Vector4 *Origin, sigma::Vector4 *Range, int Mode, int Type=0);
	std::vector<sigma::ResourceMeta> loadResourceMeta(int ObjectID, int Type=0);
	std::vector<sigma::ResourceMeta>* loadResourceMeta(std::vector<sigma::Object> Objects, int Type=0);
	//
	std::vector<sigma::ResourceMeta> loadResourceMeta(string GeometryID , int Type=0);
	std::vector<sigma::ResourceMeta> loadResourceMeta(sigma::Vector4 *Origin, sigma::Vector4 *Range, sigma::Target *target, double theta, int Mode, int Type=0);
	std::vector<sigma::ResourceMeta> loadResourceMeta(sigma::Vector4 *Origin, sigma::Vector4 *Range, sigma::Target *target, double alpha, double beta, int Mode, int Type=0);
	//
	std::vector<sigma::ResourceMeta> loadResourceMeta(sigma::Vector4 *Origin, sigma::Vector4 *Range, sigma::BoudingBox *box, int Mode, int Type=0);
};
#endif

