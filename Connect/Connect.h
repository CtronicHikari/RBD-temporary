#pragma once
#ifndef CONNECT_H
#define CONNECT_H

#include<mysql/mysql.h>
#include<string>
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <typeinfo>
#include"MyClass.h"
#include"EncodeTool.h"

#include<ctime>
#include<cstdlib>

#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<stdio.h> 
#include<stdlib.h> 
#include<fcntl.h> 
#include<limits.h> 
#include<iostream>
#include<vector>
#include<cstring>
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
	double StartTime = 0;

	vector<string> Hashres;
	string com01 = "echo -n '";
	string com02 = "";
    	string com03 = "'|md5sum|cut -d ' ' -f1";
	string resCom;
	int count;
	char shellcommand[200];

	EncodeTool encodeTool;
	
	//Connect&Disconnect  //OK
	const void Connect();
	const void Disconnect();
	//Add Object & Geometry & Connect  //OK
	const void addObject(int ObjectID, int ParentObejctID, string GeometryID);
	const string addGeometry(WorldPosition worldPostion, double Time);
	const void addConnectOG(int ObjectID, string GeometryID); //Abolition
	const void addConnectRG(string ResourceID, string GeometryID);
	const void addResource(string GeometryID, int ParentObjectID, int Type, Attitude attitude, string path_temp, string Parameters);
	//New Connect OG
	
	const void add_link_geo_obj(int ObjectID, string GeometryID);
	const void add_link_geo_obj(int ObjectID, WorldPosition worldPosition, double Time);
	const void add_link_geo_obj(int ObjectID, std::vector<ResourceMeta> ReferenceList, std::vector<float> Weights, double Time = -1, long int NoVibrations = -1);
	const void add_link_geo_obj(int ObjectID, std::vector<Geometry> ReferenceList, std::vector<float> Weights, double Time = -1, long int NoVibrations = -1);
	double GetSenserFrequency();
	double SetStartTime(double NewTime);
	
	//Geometry Search Functions  //OK
	const std::vector<Geometry> loadGeometry(Vector4 Origin, Vector4 Range, int Mode); 
	//object Search Functions  //OK
	const std::vector<Object> loadObject(Vector4 Origin, Vector4 Range, int Mode);
	//Read Resource's Information Support Function
	ResourceMeta readResourceInformation_new(MYSQL_ROW src);  //Wait
	std::vector<ResourceMeta> resourceFilter(std::vector<ResourceMeta> src,std::vector<int> count);  //Wait
	//Resouce Search Function
	std::vector<ResourceMeta> loadResourceMeta(Vector4 Origin, Vector4 Range, int Mode, int Type=0);
	std::vector<ResourceMeta> loadResourceMeta(int ObjectID, int Type=0);
	std::vector<ResourceMeta>* loadResourceMeta(std::vector<Object> Objects, int Type=0);
	std::vector<ResourceMeta> loadResourceMeta(double x, double y, double z, int Type=0);
	std::vector<ResourceMeta> loadResourceMeta(string ID , int Type=0);
	//Load Resource's Functions (Expandable)
	std::vector<ImageResource> loadImageResource(std::vector<ResourceMeta> src); //Wait
	std::vector<PointCloudResource> loadPointCloudResource(std::vector<ResourceMeta> src);  //Wait
	//Frustum Search
	std::vector<ResourceMeta> loadResourceMeta(Vector4 Origin, Frustum frustum, int Type=0);
	std::vector<ResourceMeta> loadResourceMeta(Vector4 Origin, Vector4 Range, Target target, double theta, int Type=0);
	std::vector<ResourceMeta> loadResourceMeta(Vector4 Origin, Vector4 Range, Target target, double alpha, double beta, int Type=0);

	void *string2char(string theString,char re[])
	{
		strcpy(re,theString.c_str());
	}


	//execute shell command
	//执行一个shell命令，输出结果逐行存储在resvec中，并返回行数
	int32_t linuxshell(const char *cmd, vector<string> &resvec) {
   		resvec.clear();
  	  	FILE *pp = popen(cmd, "r"); //建立管道,读取模式,”w“为写入模式
  	  	if (!pp) {
   	     		return -1;
   	 	}
   	 	char tmp[1024]; //设置一个合适的长度，以存储每一行输出
   		while (fgets(tmp, sizeof(tmp), pp) != NULL) {
   	     		if (tmp[strlen(tmp) - 1] == '\n') {
    				tmp[strlen(tmp) - 1] = '\0'; //去除换行符
    	    		}
			resvec.push_back(tmp);
    		}
   	 	pclose(pp); //关闭管道
    		return resvec.size();
	}

	
};
	
	


#endif

