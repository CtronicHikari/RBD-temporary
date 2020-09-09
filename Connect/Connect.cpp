#include "Connect.h"
#include<iostream>
#include<string>
#include<ctime>
#include<time.h>
#include<stdio.h>
#include"MyClass.h"
#include<math.h>


using namespace std;
//******************************
//Connect & Disconnect
//******************************
const void DBConnect::Connect()
{
	pcon = mysql_init(NULL);
	if(!mysql_real_connect(pcon, server, user, password, db, port, NULL, 0))
	{
		cout << "connection failed!" << endl;
	}
	else
		cout << "connection success!" << endl;
}

const void DBConnect::Disconnect()
{
	mysql_free_result(result);
	mysql_close(pcon);
	cout << "Disconnect from the database!" << endl;
}

//********************************
//Add Object & Geometry & Connect
//********************************
const void DBConnect::addObject(int ObjectID, int ParentObjectID, string GeometryID)
{
	char sql[200];
	int sql_len = sprintf(sql, "insert into server.object(OID,POID) values('%d','%d')", ObjectID, ParentObjectID);
	res = mysql_real_query(pcon, sql, sql_len);

	//sql_len = sprintf(sql,"insert into server.link_geo_and_obj(OID,GID) values('%d','%s')", ObjectID, GeometryID.c_str());
	//res = mysql_real_query(pcon, sql, sql_len);
}
//***************************
const string DBConnect::addGeometry(WorldPosition worldPosition, double Time)
{
	
	stringstream s;
	string x,y,z,t;
	int sql_len;
	
	s.clear();
	s << worldPosition.WorldPositionX;
	s >> x;

	s.clear();
	s << worldPosition.WorldPositionY;
	s >> y;

	s.clear();
	s << worldPosition.WorldPositionZ;
	s >> z;

	s.clear();
	s << Time;
	s >> t;
	
	string ID = x + "_" + y + "_" + z + "_" + t;

	char sql[5000];
	if(worldPosition.WorldPositionX == 0 || worldPosition.WorldPositionY == 0 || worldPosition.WorldPositionZ == 0)
	{
		ID = "1.79769e+308_" + t;
		sql_len = sprintf(sql, "insert into server.geometry(ID,WPX,WPY,WPZ,Time) values('%s','%.20f','%.20f','%.20f','%.20f')",ID.c_str(), DBL_MAX, DBL_MAX, DBL_MAX, Time);
		
	}
	
	else{
		sql_len = sprintf(sql, "insert into server.geometry(ID,WPX,WPY,WPZ,Time) values('%s','%.20f','%.20f','%.20f','%.20f')",ID.c_str(), worldPosition.WorldPositionX, worldPosition.WorldPositionY, worldPosition.WorldPositionZ, Time);
	}
	


	res = mysql_real_query(pcon,sql,sql_len);
	
	return ID;
}
//***************************
//Abolition
//***************************
const void DBConnect::addConnectOG(int ObjectID, string GeometryID)
{
	char sql[100];
	int sql_len = sprintf(sql,"insert into server.link_geo_and_obj(OID,GID) values('%d','%s')", ObjectID, GeometryID.c_str());
	res = mysql_real_query(pcon, sql, sql_len);
}
//***************************
//***************************
const void DBConnect::addConnectRG(string ResourceID, string GeometryID)
{
	char sql[3000];
	int sql_len = sprintf(sql,"insert into server.link_geo_and_res(RID,GID) values('%s','%s')", ResourceID.c_str(), GeometryID.c_str());
	res = mysql_real_query(pcon, sql, sql_len);
}
//***************************
const void DBConnect::add_link_geo_obj(int ObjectID, string GeometryID)
{
	char sql[200];
	int sql_len = sprintf(sql,"insert into server.link_geo_and_obj(OID,GID) values('%d','%s')", ObjectID, GeometryID.c_str());
	res = mysql_real_query(pcon, sql, sql_len);
}
//***************************
const void DBConnect::add_link_geo_obj(int ObjectID, WorldPosition worldPosition, double Time)
{
	string tempGeo = addGeometry(worldPosition, Time);
	add_link_geo_obj(ObjectID,tempGeo);
}
//***************************
const void DBConnect::add_link_geo_obj(int ObjectID, std::vector<ResourceMeta> ReferenceList, std::vector<float> Weights, double Time, long int NoVibrations)
{
	string checkStr = "";
	std::vector<string> tempStrs;
	string tempData = "";
	WorldPosition tempWorldPosition;
	int n = ReferenceList.size();
	int m = Weights.size();
	std::vector<float> normal_weight;
	float sum = 0;
	double tempx=0.0f,tempy=0.0f,tempz=0.0f;
	double offsetX=0.0f,offsetY=0.0f,offsetZ=0.0f;
	double posX=0.0f,posY=0.0f,posZ=0.0f;
	int sql_len;
	char sql[1000];

	for(int x=0;x<ReferenceList.size();x++)
	{
		checkStr += ReferenceList[x].ID;
	}
	com02 = checkStr;
	resCom = com01 + com02 + com03;
	string2char(resCom,shellcommand);
	count = linuxshell(shellcommand,Hashres);
	
	sql_len = sprintf(sql, "select count(*) from server.checkTable where ObjectID='%d' and checkStr='%s';", ObjectID, Hashres[0].c_str());
	res = mysql_real_query(pcon, sql, sql_len);
	result = mysql_store_result(pcon);
	sql_row = mysql_fetch_row(result);
	
	tempData = sql_row[0];
	int dataCount = -1;
	dataCount = atoi(tempData.c_str());
	if(dataCount == 0)
	{
		if(m!=n || m==0 || n==0)
		{
			cout << "Error Data" << endl;
			return;
		}

		for(int j=0;j<m;j++)
		{
			sum += Weights[j];
		}
		for(int k=0;k<m;k++)
		{
			float temp = Weights[k] / sum;
			normal_weight.push_back(temp);
		}
	
		for(int i=0;i<n;i++)
		{
			tempx += ReferenceList[i].transform.WorldPositionX * normal_weight[i];
			tempy += ReferenceList[i].transform.WorldPositionY * normal_weight[i];
			tempz += ReferenceList[i].transform.WorldPositionZ * normal_weight[i];	
		}
		tempWorldPosition.WorldPositionX = tempx;
		tempWorldPosition.WorldPositionY = tempy;
		tempWorldPosition.WorldPositionZ = tempz;

		offsetX = ReferenceList[0].transform.WorldPositionX - tempx;
		offsetY = ReferenceList[0].transform.WorldPositionY - tempy;
		offsetZ = ReferenceList[0].transform.WorldPositionZ - tempz;	

		if(Time == -1 && NoVibrations == -1)
		{
			std::time_t t = std::time(0);
			string tempGeo = addGeometry(tempWorldPosition, (double)t);
			add_link_geo_obj(ObjectID,tempGeo);

			sql_len = sprintf(sql,"insert into server.checkTable(ObjectID,checkStr,offsetX,offsetY,offsetZ) values('%d','%s','%.20f','%.20f','%.20f')", ObjectID, Hashres[0].c_str(),offsetX,offsetY,offsetZ);
			res = mysql_real_query(pcon, sql, sql_len);
		}

		else if(Time != -1 && NoVibrations == -1)
		{
			string tempGeo = addGeometry(tempWorldPosition, Time);
			add_link_geo_obj(ObjectID,tempGeo);

			sql_len = sprintf(sql,"insert into server.checkTable(ObjectID,checkStr,offsetX,offsetY,offsetZ) values('%d','%s','%.20f','%.20f','%.20f')", ObjectID, Hashres[0].c_str(),offsetX,offsetY,offsetZ);
			res = mysql_real_query(pcon, sql, sql_len);
		}

		else if(Time == -1 && NoVibrations != -1)
		{
			double tempFre = GetSenserFrequency();
			double tempTime = StartTime + tempFre * NoVibrations;
			string tempGeo = addGeometry(tempWorldPosition, tempTime);
			add_link_geo_obj(ObjectID,tempGeo);

			sql_len = sprintf(sql,"insert into server.checkTable(ObjectID,checkStr,offsetX,offsetY,offsetZ) values('%d','%s','%.20f','%.20f','%.20f')", ObjectID, Hashres[0].c_str(),offsetX,offsetY,offsetZ);
			res = mysql_real_query(pcon, sql, sql_len);
		}

		else 
		{
			cout << "input Data has Error, Time and NoVibrations cannot be non-zero at the same time " << endl;
		} 
	}
	else
	{
		cout << "these Datas are used, use offset" << endl;
		double tempX,tempY,tempZ;
		sql_len = sprintf(sql, "select * from server.checkTable where ObjectID='%d' and checkStr='%s';", ObjectID, Hashres[0].c_str());
		res = mysql_real_query(pcon, sql, sql_len);
		result = mysql_store_result(pcon);
		sql_row = mysql_fetch_row(result);
	
		tempX = strtod(sql_row[2],NULL);
		tempY = strtod(sql_row[3],NULL);
		tempZ = strtod(sql_row[4],NULL);

		posX = ReferenceList[0].transform.WorldPositionX - tempX;
		posY = ReferenceList[0].transform.WorldPositionY - tempY;
		posZ = ReferenceList[0].transform.WorldPositionZ - tempZ;

		tempWorldPosition.WorldPositionX = posX;
		tempWorldPosition.WorldPositionY = posY;
		tempWorldPosition.WorldPositionZ = posZ;

		if(Time == -1 && NoVibrations == -1)
		{
			std::time_t t = std::time(0);
			string tempGeo = addGeometry(tempWorldPosition, (double)t);
			add_link_geo_obj(ObjectID,tempGeo);
		}

		else if(Time != -1 && NoVibrations == -1)
		{
			string tempGeo = addGeometry(tempWorldPosition, Time);
			add_link_geo_obj(ObjectID,tempGeo);
		}

		else if(Time == -1 && NoVibrations != -1)
		{
			double tempFre = GetSenserFrequency();
			double tempTime = StartTime + tempFre * NoVibrations;
			string tempGeo = addGeometry(tempWorldPosition, tempTime);
			add_link_geo_obj(ObjectID,tempGeo);
		}

		else 
		{
			cout << "input Data has Error, Time and NoVibrations cannot be non-zero at the same time " << endl;
		} 
	}
}
//***************************
const void DBConnect::add_link_geo_obj(int ObjectID, std::vector<Geometry> ReferenceList, std::vector<float> Weights, double Time, long int NoVibrations)
{
	string checkStr = "";
	std::vector<string> tempStrs;
	string tempData = "";
	WorldPosition tempWorldPosition;
	int n = ReferenceList.size();
	int m = Weights.size();
	std::vector<float> normal_weight;
	float sum = 0;
	double tempx=0.0f,tempy=0.0f,tempz=0.0f;
	double offsetX=0.0f,offsetY=0.0f,offsetZ=0.0f;
	double posX=0.0f,posY=0.0f,posZ=0.0f;
	int sql_len;
	char sql[1000];

	for(int x=0;x<ReferenceList.size();x++)
	{
		checkStr += ReferenceList[x].ID;
	}
	com02 = checkStr;
	resCom = com01 + com02 + com03;
	string2char(resCom,shellcommand);
	count = linuxshell(shellcommand,Hashres);
	
	sql_len = sprintf(sql, "select count(*) from server.checkTable where ObjectID='%d' and checkStr='%s';", ObjectID, Hashres[0].c_str());
	res = mysql_real_query(pcon, sql, sql_len);
	result = mysql_store_result(pcon);
	sql_row = mysql_fetch_row(result);
	
	tempData = sql_row[0];
	int dataCount = -1;
	dataCount = atoi(tempData.c_str());
	if(dataCount == 0)
	{
		if(m!=n || m==0 || n==0)
		{
			cout << "Error Data" << endl;
			return;
		}

		for(int j=0;j<m;j++)
		{
			sum += Weights[j];
		}
		for(int k=0;k<m;k++)
		{
			float temp = Weights[k] / sum;
			normal_weight.push_back(temp);
		}
	
		for(int i=0;i<n;i++)
		{
			tempx += ReferenceList[i].WorldPositionX * normal_weight[i];
			tempy += ReferenceList[i].WorldPositionY * normal_weight[i];
			tempz += ReferenceList[i].WorldPositionZ * normal_weight[i];	
		}
		tempWorldPosition.WorldPositionX = tempx;
		tempWorldPosition.WorldPositionY = tempy;
		tempWorldPosition.WorldPositionZ = tempz;

		offsetX = ReferenceList[0].WorldPositionX - tempx;
		offsetY = ReferenceList[0].WorldPositionY - tempy;
		offsetZ = ReferenceList[0].WorldPositionZ - tempz;	

		if(Time == -1 && NoVibrations == -1)
		{
			std::time_t t = std::time(0);
			string tempGeo = addGeometry(tempWorldPosition, (double)t);
			add_link_geo_obj(ObjectID,tempGeo);

			sql_len = sprintf(sql,"insert into server.checkTable(ObjectID,checkStr,offsetX,offsetY,offsetZ) values('%d','%s','%.20f','%.20f','%.20f')", ObjectID, Hashres[0].c_str(),offsetX,offsetY,offsetZ);
			res = mysql_real_query(pcon, sql, sql_len);
		}

		else if(Time != -1 && NoVibrations == -1)
		{
			string tempGeo = addGeometry(tempWorldPosition, Time);
			add_link_geo_obj(ObjectID,tempGeo);

			sql_len = sprintf(sql,"insert into server.checkTable(ObjectID,checkStr,offsetX,offsetY,offsetZ) values('%d','%s','%.20f','%.20f','%.20f')", ObjectID, Hashres[0].c_str(),offsetX,offsetY,offsetZ);
			res = mysql_real_query(pcon, sql, sql_len);
		}

		else if(Time == -1 && NoVibrations != -1)
		{
			double tempFre = GetSenserFrequency();
			double tempTime = StartTime + tempFre * NoVibrations;
			string tempGeo = addGeometry(tempWorldPosition, tempTime);
			add_link_geo_obj(ObjectID,tempGeo);

			sql_len = sprintf(sql,"insert into server.checkTable(ObjectID,checkStr,offsetX,offsetY,offsetZ) values('%d','%s','%.20f','%.20f','%.20f')", ObjectID, Hashres[0].c_str(),offsetX,offsetY,offsetZ);
			res = mysql_real_query(pcon, sql, sql_len);
		}

		else 
		{
			cout << "input Data has Error, Time and NoVibrations cannot be non-zero at the same time " << endl;
		} 
	}
	else
	{
		cout << "these Datas are used, use offset" << endl;
		double tempX,tempY,tempZ;
		sql_len = sprintf(sql, "select * from server.checkTable where ObjectID='%d' and checkStr='%s';", ObjectID, Hashres[0].c_str());
		res = mysql_real_query(pcon, sql, sql_len);
		result = mysql_store_result(pcon);
		sql_row = mysql_fetch_row(result);
	
		tempX = strtod(sql_row[2],NULL);
		tempY = strtod(sql_row[3],NULL);
		tempZ = strtod(sql_row[4],NULL);

		posX = ReferenceList[0].WorldPositionX - tempX;
		posY = ReferenceList[0].WorldPositionY - tempY;
		posZ = ReferenceList[0].WorldPositionZ - tempZ;

		tempWorldPosition.WorldPositionX = posX;
		tempWorldPosition.WorldPositionY = posY;
		tempWorldPosition.WorldPositionZ = posZ;

		if(Time == -1 && NoVibrations == -1)
		{
			std::time_t t = std::time(0);
			string tempGeo = addGeometry(tempWorldPosition, (double)t);
			add_link_geo_obj(ObjectID,tempGeo);
		}

		else if(Time != -1 && NoVibrations == -1)
		{
			string tempGeo = addGeometry(tempWorldPosition, Time);
			add_link_geo_obj(ObjectID,tempGeo);
		}

		else if(Time == -1 && NoVibrations != -1)
		{
			double tempFre = GetSenserFrequency();
			double tempTime = StartTime + tempFre * NoVibrations;
			string tempGeo = addGeometry(tempWorldPosition, tempTime);
			add_link_geo_obj(ObjectID,tempGeo);
		}

		else 
		{
			cout << "input Data has Error, Time and NoVibrations cannot be non-zero at the same time " << endl;
		} 
	}
}
//***************************
double DBConnect::GetSenserFrequency()
{
	return 1/60;
}
//***************************
double DBConnect::SetStartTime(double NewTime)
{
	DBConnect::StartTime = NewTime;
	return DBConnect::StartTime;
}

//***************************
//Add Resouces Function
//***************************
const void DBConnect::addResource(string GeometryID, int ParentObjectID, int Type, Attitude attitude, string path_temp, string Parameters)
{
	int sql_len;
	char sql[2000];
	string temp;
	//std::vector<string> splittedGID = encodeTool.ParametersSplitter(GeometryID,'_');
	string path = "/home/guan/Resources/";
	string POID = to_string(ParentObjectID);
	sql_len = sprintf(sql, "select Time from server.geometry where ID='%s';", GeometryID.c_str());
	res = mysql_real_query(pcon, sql, sql_len);
	result = mysql_store_result(pcon);
	while(sql_row = mysql_fetch_row(result))
	{
		temp = sql_row[0];
	}

	std::vector<string> splittedExt = encodeTool.ParametersSplitter(path_temp,'.');
	std::vector<string> splittedName = encodeTool.ParametersSplitter(path_temp,'/');

	//string name = to_string(ParentObjectID) + "_" + to_string(Type) + "_" + temp;
	string name = splittedName[1];
	//string full = path + name + "." + splittedExt[1];
	//string full = path_temp;
	//string RID = name + "." + splittedExt[1];

	
	
	sql_len = sprintf(sql, "insert into server.Resources(ID,POID,Type,Ext,AX,AY,AZ,AW,Path,para) values('%s','%d','%d','%s','%.16f','%.16f','%.16f','%.16f','%s','%s')",name.c_str(),ParentObjectID,Type,splittedExt[1].c_str(),attitude.AttitudeX,attitude.AttitudeY,attitude.AttitudeZ,attitude.AttitudeW,path_temp.c_str(),Parameters.c_str());
	res = mysql_real_query(pcon, sql, sql_len);

	//AddLink
	sql_len = sprintf(sql,"insert into server.link_geo_and_res(RID,GID) values('%s','%s')", name.c_str(), GeometryID.c_str());
	res = mysql_real_query(pcon, sql, sql_len);
}
//***************************
//Geometry Search
//***************************
const std::vector<Geometry> DBConnect::loadGeometry(Vector4 Origin, Vector4 Range, int Mode)
{
	std::vector<Geometry> Geometries;
	Geometry src;
	string temp;
	int sql_len;
		
	char sql[1000];
	if(Mode == 3)  //两点式  !!!!
	{
		sql_len = sprintf(sql, "select * from server.geometry where WPX>='%f' and WPX<='%f' and WPY>='%f' and WPY<='%f' and WPZ>='%f' and WPZ<='%f' and Time>='%f' and Time<='%f';", Origin.x, Range.x, Origin.y, Range.y, Origin.z, Range.z, Origin.t, Range.t);
		
	}
	else if(Mode == 2)  //球形范围
	{
		if(Range.x > 0 && Range.y > 0 && Range.z > 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(WPX-'%f',2)/POW('%f',2))+(POW(WPY-'%f',2)/POW('%f',2))+(POW(WPZ-'%f',2)/POW('%f',2))<1 and Time>='%f' and Time<='%f';", Origin.x, Range.x/2, Origin.y, Range.y/2, Origin.z, Range.z/2, Origin.t, Range.t);
		}
		else if(Range.x > 0 && Range.y > 0 && Range.z > 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(WPX-'%f',2)/POW('%f',2))+(POW(WPY-'%f',2)/POW('%f',2))+(POW(WPZ-'%f',2)/POW('%f',2))<1;", Origin.x, Range.x/2, Origin.y, Range.y/2, Origin.z, Range.z/2);
		}
		else if(Range.x > 0 && Range.y > 0 && Range.z <= 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(WPX-'%f',2)/POW('%f',2))+(POW(WPY-'%f',2)/POW('%f',2))<1 and Time>='%f' and Time<='%f';", Origin.x, Range.x/2, Origin.y, Range.y/2, Origin.t, Range.t);
		}
		else if(Range.x > 0 && Range.y > 0 && Range.z <= 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(WPX-'%f',2)/POW('%f',2))+(POW(WPY-'%f',2)/POW('%f',2))<1;", Origin.x, Range.x/2, Origin.y, Range.y/2);
		}
		else if(Range.x > 0 && Range.y <= 0 && Range.z > 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(WPX-'%f',2)/POW('%f',2))+(POW(WPZ-'%f',2)/POW('%f',2))<1 and Time>='%f' and Time<='%f';", Origin.x, Range.x/2, Origin.z, Range.z/2, Origin.t, Range.t);
		}
		else if(Range.x > 0 && Range.y <= 0 && Range.z > 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(WPX-'%f',2)/POW('%f',2))+(POW(WPZ-'%f',2)/POW('%f',2))<1;", Origin.x, Range.x/2, Origin.z, Range.z/2);
		}
		else if(Range.x <= 0 && Range.y > 0 && Range.z > 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(WPY-'%f',2)/POW('%f',2))+(POW(WPZ-'%f',2)/POW('%f',2))<1 and Time>='%f' and Time<='%f';", Origin.y, Range.y/2, Origin.z, Range.z/2, Origin.t, Range.t);
		}
		else if(Range.x <= 0 && Range.y > 0 && Range.z > 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(WPY-'%f',2)/POW('%f',2))+(POW(WPZ-'%f',2)/POW('%f',2))<1;", Origin.y, Range.y/2, Origin.z, Range.z/2);
		}
		else if(Range.x > 0 && Range.y <= 0 && Range.z <= 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(WPX-'%f',2)/POW('%f',2))<1 and Time>='%f' and Time<='%f';", Origin.x, Range.x/2, Origin.t, Range.t);
		}
		else if(Range.x > 0 && Range.y <= 0 && Range.z <= 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(WPX-'%f',2)/POW('%f',2))<1;", Origin.x, Range.x/2);
		}
		else if(Range.x <= 0 && Range.y > 0 && Range.z <= 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(WPY-'%f',2)/POW('%f',2))<1 and Time>='%f' and Time<='%f';", Origin.y, Range.y/2, Origin.t, Range.t);
		}
		else if(Range.x <= 0 && Range.y > 0 && Range.z <= 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(WPY-'%f',2)/POW('%f',2))<1;", Origin.y, Range.y/2);
		}
		else if(Range.x <= 0 && Range.y <= 0 && Range.z > 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(WPZ-'%f',2)/POW('%f',2))<1 and Time>='%f' and Time<='%f';", Origin.z, Range.z/2, Origin.t, Range.t);
		}
		else if(Range.x <= 0 && Range.y <= 0 && Range.z > 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(WPZ-'%f',2)/POW('%f',2))<1;", Origin.z, Range.z/2);
		}
		else
		{
			sql_len = sprintf(sql, "select * from server.geometry;");
		}
	}
	else if(Mode == 1)  //正方形底面
	{
		if(Range.x > 0 && Range.y > 0 && Range.z > 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f' and ABS(WPY-'%f')<='%f' and WPZ>='%f' and WPZ-'%f'<='%f' and Time>='%f' and Time<='%f';", Origin.x, Range.x/2, Origin.y, Range.y/2, Origin.z, Origin.z, Range.z, Origin.t, Range.t);
		}
		else if(Range.x > 0 && Range.y > 0 && Range.z > 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f' and ABS(WPY-'%f')<='%f' and WPZ>='%f' and WPZ-'%f'<='%f';",Origin.x, Range.x/2, Origin.y, Range.y/2, Origin.z, Origin.z, Range.z);
		}
		else if(Range.x > 0 && Range.y > 0 && Range.z <= 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f' and ABS(WPY-'%f')<='%f' and Time>='%f' and Time<='%f';", Origin.x, Range.x/2, Origin.y, Range.y/2, Origin.t, Range.t);
		}
		else if(Range.x > 0 && Range.y > 0 && Range.z <= 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f' and ABS(WPY-'%f')<='%f';", Origin.x, Range.x/2, Origin.y, Range.y/2);
		}
		else if(Range.x > 0 && Range.y <= 0 && Range.z > 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f' and WPZ>='%f' and WPZ-'%f'<='%f' and Time>='%f' and Time<='%f';", Origin.x, Range.x/2, Origin.z, Origin.z, Range.z, Origin.t, Range.t);
		}
		else if(Range.x > 0 && Range.y <= 0 && Range.z > 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f' and WPZ>='%f' and WPZ-'%f'<='%f';", Origin.x, Range.x/2, Origin.z, Origin.z, Range.z);
		}
		else if(Range.x <= 0 && Range.y > 0 && Range.z > 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPY-'%f')<='%f' and WPZ>='%f' and WPZ-'%f'<='%f' and Time>='%f' and Time<='%f';", Origin.y, Range.y/2, Origin.z, Origin.z, Range.z, Origin.t, Range.t);
		}
		else if(Range.x <= 0 && Range.y > 0 && Range.z > 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPY-'%f')<='%f' and WPZ>='%f' and WPZ-'%f'<='%f';", Origin.y, Range.y/2, Origin.z, Origin.z, Range.z);
		}
		else if(Range.x > 0 && Range.y <= 0 && Range.z <= 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f' and Time>='%f' and Time<='%f';", Origin.x, Range.x/2, Origin.t, Range.t);
		}
		else if(Range.x > 0 && Range.y <= 0 && Range.z <= 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f';", Origin.x, Range.x/2);
		}
		else if(Range.x <= 0 && Range.y > 0 && Range.z <= 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPY-'%f')<='%f' and Time>='%f' and Time<='%f';", Origin.y, Range.y/2, Origin.t, Range.t);
		}
		else if(Range.x <= 0 && Range.y > 0 && Range.z <= 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPY-'%f')<='%f';", Origin.y, Range.y/2);
		}
		else if(Range.x <= 0 && Range.y <= 0 && Range.z > 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where WPZ>='%f' and WPZ-'%f'<='%f' and Time>='%f' and Time<='%f';", Origin.z, Origin.z, Range.z, Origin.t, Range.t);
		}
		else if(Range.x <= 0 && Range.y <= 0 && Range.z > 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where WPZ>='%f' and WPZ-'%f'<='%f';", Origin.z, Origin.z, Range.z);
		}
		else
		{
			sql_len = sprintf(sql, "select * from server.geometry;");
		}
	}

	else if(Mode == 4) //正方形中点
	{
		
		if(Range.x > 0 && Range.y > 0 && Range.z > 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f' and ABS(WPY-'%f')<='%f' and ABS(WPZ-'%f')<='%f' and Time>='%f' and Time<='%f';", Origin.x, Range.x/2, Origin.y, Range.y/2, Origin.z, Range.z/2, Origin.t, Range.t);
		}
		else if(Range.x > 0 && Range.y > 0 && Range.z > 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f' and ABS(WPY-'%f')<='%f' and ABS(WPZ-'%f')<='%f';",Origin.x, Range.x/2, Origin.y, Range.y/2, Origin.z, Range.z/2);
		}
		else if(Range.x > 0 && Range.y > 0 && Range.z <= 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f' and ABS(WPY-'%f')<='%f' and Time>='%f' and Time<='%f';", Origin.x, Range.x/2, Origin.y, Range.y/2, Origin.t, Range.t);
		}
		else if(Range.x > 0 && Range.y > 0 && Range.z <= 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f' and ABS(WPY-'%f')<='%f';", Origin.x, Range.x/2, Origin.y, Range.y/2);
		}
		else if(Range.x > 0 && Range.y <= 0 && Range.z > 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f' and ABS(WPZ-'%f')<='%f' and Time>='%f' and Time<='%f';", Origin.x, Range.x/2, Origin.z, Range.z/2, Origin.t, Range.t);
		}
		else if(Range.x > 0 && Range.y <= 0 && Range.z > 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f' and ABS(WPZ-'%f')<='%f';", Origin.x, Range.x/2, Origin.z, Range.z/2);
		}
		else if(Range.x <= 0 && Range.y > 0 && Range.z > 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPY-'%f')<='%f' and ABS(WPZ-'%f')<='%f' and Time>='%f' and Time<='%f';", Origin.y, Range.y/2, Origin.z, Range.z/2, Origin.t, Range.t);
		}
		else if(Range.x <= 0 && Range.y > 0 && Range.z > 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPY-'%f')<='%f' and ABS(WPZ-'%f')<='%f';", Origin.y, Range.y/2, Origin.z, Range.z/2);
		}
		else if(Range.x > 0 && Range.y <= 0 && Range.z <= 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f' and Time>='%f' and Time<='%f';", Origin.x, Range.x/2, Origin.t, Range.t);
		}
		else if(Range.x > 0 && Range.y <= 0 && Range.z <= 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPX-'%f')<='%f';", Origin.x, Range.x/2);
		}
		else if(Range.x <= 0 && Range.y > 0 && Range.z <= 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPY-'%f')<='%f' and Time>='%f' and Time<='%f';", Origin.y, Range.y/2, Origin.t, Range.t);
		}
		else if(Range.x <= 0 && Range.y > 0 && Range.z <= 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPY-'%f')<='%f';", Origin.y, Range.y/2);
		}
		else if(Range.x <= 0 && Range.y <= 0 && Range.z > 0 && Range.t > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPZ-'%f')<='%f' and Time>='%f' and Time<='%f';", Origin.z, Range.z/2, Origin.t, Range.t);
		}
		else if(Range.x <= 0 && Range.y <= 0 && Range.z > 0 && Range.t <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(WPZ-'%f')<='%f';", Origin.z, Range.z/2);
		}
		else
		{
			sql_len = sprintf(sql, "select * from server.geometry;");
		}
	}
	
	res = mysql_real_query(pcon, sql, sql_len);

	result = mysql_store_result(pcon);

	while(sql_row = mysql_fetch_row(result))
	{
		src.ID = sql_row[0];
		src.WorldPositionX = strtod(sql_row[1],NULL);

		src.WorldPositionY = strtod(sql_row[2],NULL);

		src.WorldPositionZ = strtod(sql_row[3],NULL);

		src.Time =  strtod(sql_row[4],NULL);
		Geometries.push_back(src);
	}
	
	return Geometries;
}
//***************************

//***************************
//Object Search
//***************************
const std::vector<Object> DBConnect::loadObject(Vector4 Origin, Vector4 Range, int Mode)
{
	std::vector<Object> Objects;
	std::vector<Geometry> Geometries;
	std::vector<int> OIDs;
	Object src;
	string temp;
	int sql_len;
	char sql[300];
	int i;

	Geometries = loadGeometry(Origin,Range,Mode);
	for(i=0;i<Geometries.size();i++)
	{
		sql_len = sprintf(sql, "select OID from server.link_geo_and_obj where GID='%s';", Geometries[i].ID.c_str());
		res = mysql_real_query(pcon, sql, sql_len);
		result = mysql_store_result(pcon);
		while(sql_row = mysql_fetch_row(result))
		{
			temp = sql_row[0];
			OIDs.push_back(atoi(temp.c_str()));
		}
	}
	for(i=0;i<OIDs.size();i++)
	{
		sql_len = sprintf(sql, "select * from server.object where OID='%d';", OIDs[i]);
		res = mysql_real_query(pcon, sql, sql_len);
		result = mysql_store_result(pcon);
		while(sql_row = mysql_fetch_row(result))
		{
			temp = sql_row[0];
			src.ObjectID = atoi(temp.c_str());
			temp = sql_row[1];
			src.ParentNodeID = atoi(temp.c_str());
			Objects.push_back(src);
		}
	}
	return Objects;
}
//********************************

//********************************
//Read Resource's Information Support Function
//********************************
ResourceMeta DBConnect::readResourceInformation_new(MYSQL_ROW src)
{
	ResourceMeta dst;
	string temp;
	int sql_len,i;
	char sql[300];
	string temp_GID;

	dst.ID = src[0];
	temp = src[1];  	dst.ParentObjectID = atoi(temp.c_str());
	temp = src[2];  	dst.Type = atoi(temp.c_str());
	dst.Ext = src[3];
	temp = src[4];		dst.transform.AttitudeX = atof(temp.c_str());
	temp = src[5];		dst.transform.AttitudeY = atof(temp.c_str());
	temp = src[6];		dst.transform.AttitudeZ = atof(temp.c_str());
	temp = src[7];		dst.transform.AttitudeW = atof(temp.c_str());
	dst.Path = src[8];
	dst.Parameters = src[9];


	sql_len = sprintf(sql, "select GID from server.link_geo_and_res where RID='%s';", dst.ID.c_str());
	res = mysql_real_query(pcon, sql, sql_len);
	result = mysql_store_result(pcon);
	while(sql_row = mysql_fetch_row(result))
	{
		temp = sql_row[0];
		temp_GID = temp;
	}
	sql_len = sprintf(sql, "select * from server.geometry where ID='%s';", temp_GID.c_str());
	res = mysql_real_query(pcon, sql, sql_len);
	result = mysql_store_result(pcon);
	while(sql_row = mysql_fetch_row(result))
	{
		temp = sql_row[1];
		dst.transform.WorldPositionX = atof(temp.c_str());
		temp = sql_row[2];
		dst.transform.WorldPositionY = atof(temp.c_str());
		temp = sql_row[3];
		dst.transform.WorldPositionZ = atof(temp.c_str());
		temp = sql_row[4];
		dst.transform.Time = atof(temp.c_str());
	}

	return dst;
}
//********************************
std::vector<ResourceMeta> DBConnect::resourceFilter(std::vector<ResourceMeta> src,std::vector<int> count)
{
	std::vector<ResourceMeta> dst;
	ResourceMeta temp;

	for(int i=0;i<count.size();i++)
	{
		temp.ID = src[count[i]].ID;
		temp.ParentObjectID = src[count[i]].ParentObjectID;
		temp.Type = src[count[i]].Type;
		temp.Ext = src[count[i]].Ext;
		temp.transform.AttitudeX = src[count[i]].transform.AttitudeX;
		temp.transform.AttitudeY = src[count[i]].transform.AttitudeY;
		temp.transform.AttitudeZ = src[count[i]].transform.AttitudeZ;
		temp.transform.AttitudeW = src[count[i]].transform.AttitudeW;
		temp.Path = src[count[i]].Path;
		temp.Parameters = src[count[i]].Parameters;
		dst.push_back(temp);
	}
	return dst;
}
//********************************
//Load Resource's Functions (Expandable) (Now : 2)
//********************************
std::vector<ImageResource> DBConnect::loadImageResource(std::vector<ResourceMeta> src)
{
	std::vector<ImageResource> dst;
	ImageResource temp;

	for(int i=0;i<src.size();i++)
	{
		if(src[i].Type == 1)
		{		
			temp.ID = src[i].ID;
			temp.ParentObjectID = src[i].ParentObjectID;
			temp.Type = src[i].Type;                
			temp.Ext = src[i].Ext;
			temp.transform.AttitudeX = src[i].transform.AttitudeX;
			temp.transform.AttitudeY = src[i].transform.AttitudeY;
			temp.transform.AttitudeZ = src[i].transform.AttitudeZ;
			temp.transform.AttitudeW = src[i].transform.AttitudeW;
			temp.Path = src[i].Path;
			temp.Parameters = src[i].Parameters;
			temp.loadResourceFile(temp.Path);
			dst.push_back(temp);
		}
	}
	return dst;	
} 
//********************************
std::vector<PointCloudResource> DBConnect::loadPointCloudResource(std::vector<ResourceMeta> src)
{
	std::vector<PointCloudResource> dst;
	PointCloudResource temp;

	for(int i=0;i<src.size();i++)
	{
		if(src[i].Type == 2)
		{
			temp.ID = src[i].ID;
			temp.ParentObjectID = src[i].ParentObjectID;
			temp.Type = src[i].Type;
			temp.Ext = src[i].Ext;
			temp.transform.AttitudeX = src[i].transform.AttitudeX;
			temp.transform.AttitudeY = src[i].transform.AttitudeY;
			temp.transform.AttitudeZ = src[i].transform.AttitudeZ;
			temp.transform.AttitudeW = src[i].transform.AttitudeW;
			temp.Path = src[i].Path;
			temp.Parameters = src[i].Parameters;
			temp.loadResourceFile(temp.Path);
			dst.push_back(temp);
		}
	}
	return dst;	
} 
//********************************
//Resouce Search Function (4.0)
//********************************
std::vector<ResourceMeta> DBConnect::loadResourceMeta(Vector4 Origin, Vector4 Range, int Mode, int Type)
{
	std::vector<ResourceMeta> ResourceMetas;
	std::vector<Geometry> Geometries;
	std::vector<string> RIDs;
	ResourceMeta dst;
	string temp;
	char sql[300];
	int sql_len,i;


	Geometries = loadGeometry(Origin,Range,Mode);
	
	for(i=0;i<Geometries.size();i++)
	{
		sql_len = sprintf(sql, "select RID from server.link_geo_and_res where GID='%s';", Geometries[i].ID.c_str());
		res = mysql_real_query(pcon, sql, sql_len);
		result = mysql_store_result(pcon);
		while(sql_row = mysql_fetch_row(result))
		{
			temp = sql_row[0];
			RIDs.push_back(temp);
		}
	}
	
	for(i=0;i<RIDs.size();i++)
	{
		if(Type != 0)
			sql_len = sprintf(sql, "select * from server.Resources where ID='%s' and Type='%d';", RIDs[i].c_str(), Type);
		else
			sql_len = sprintf(sql, "select * from server.Resources where ID='%s';", RIDs[i].c_str());
		res = mysql_real_query(pcon, sql, sql_len);
		result = mysql_store_result(pcon);
		while(sql_row = mysql_fetch_row(result))
		{
			dst = readResourceInformation_new(sql_row);
			ResourceMetas.push_back(dst);
		}
	}
	
	return ResourceMetas;
}
//********************************
std::vector<ResourceMeta> DBConnect::loadResourceMeta(double x, double y, double z, int Type)
{
	std::vector<ResourceMeta> ResourceMetas;
	std::vector<Geometry> Geometries;
	std::vector<string> RIDs;
	ResourceMeta dst;
	Geometry src;
	string temp;
	char sql[300];
	int sql_len,i;

	sql_len = sprintf(sql, "select * from server.geometry where WPX='%f' and WPY='%f' and WPZ='%f';", x, y, z);
	res = mysql_real_query(pcon, sql, sql_len);
	result = mysql_store_result(pcon);
	while(sql_row = mysql_fetch_row(result))
	{
		src.ID = sql_row[0];
		temp = sql_row[1];
		src.WorldPositionX = atof(temp.c_str());
		temp = sql_row[2];
		src.WorldPositionY = atof(temp.c_str());
		temp = sql_row[3];
		src.WorldPositionZ = atof(temp.c_str());
		temp = sql_row[4];
		src.Time = atof(temp.c_str());
		Geometries.push_back(src);
	}

	for(i=0;i<Geometries.size();i++)
	{
		sql_len = sprintf(sql, "select RID from server.link_geo_and_res where GID='%s';", Geometries[i].ID.c_str());
		res = mysql_real_query(pcon, sql, sql_len);
		result = mysql_store_result(pcon);
		while(sql_row = mysql_fetch_row(result))
		{
			temp = sql_row[0];
			RIDs.push_back(temp);
		}
	}
	for(i=0;i<RIDs.size();i++)
	{
		if(Type != 0)
			sql_len = sprintf(sql, "select * from server.Resources where ID='%s' and Type='%d';", RIDs[i].c_str(), Type);
		else
			sql_len = sprintf(sql, "select * from server.Resources where ID='%s';", RIDs[i].c_str());
		res = mysql_real_query(pcon, sql, sql_len);
		result = mysql_store_result(pcon);
		while(sql_row = mysql_fetch_row(result))
		{
			dst = readResourceInformation_new(sql_row);
			ResourceMetas.push_back(dst);
		}
	}
	return ResourceMetas;
}
//********************************
std::vector<ResourceMeta> DBConnect::loadResourceMeta(string ID, int Type)
{
	std::vector<ResourceMeta> ResourceMetas;
	std::vector<string> RIDs;
	ResourceMeta dst;
	string temp;
	char sql[300];
	int sql_len,i;


	sql_len = sprintf(sql, "select RID from server.link_geo_and_res where GID='%s';", ID.c_str());
	res = mysql_real_query(pcon, sql, sql_len);
	result = mysql_store_result(pcon);
	while(sql_row = mysql_fetch_row(result))
	{
		temp = sql_row[0];
		RIDs.push_back(temp);
	}
	
	for(i=0;i<RIDs.size();i++)
	{
		if(Type != 0)
			sql_len = sprintf(sql, "select * from server.Resources where ID='%s' and Type='%d';", RIDs[i].c_str(), Type);
		else
			sql_len = sprintf(sql, "select * from server.Resources where ID='%s';", RIDs[i].c_str());
		res = mysql_real_query(pcon, sql, sql_len);
		result = mysql_store_result(pcon);
		while(sql_row = mysql_fetch_row(result))
		{
			dst = readResourceInformation_new(sql_row);
			ResourceMetas.push_back(dst);
		}
	}
	return ResourceMetas;
}
//********************************
std::vector<ResourceMeta> DBConnect::loadResourceMeta(int ObjectID, int Type)
{
	std::vector<ResourceMeta> ResourceMetas;
	ResourceMeta dst;
	string temp;
	char sql[100];
	int sql_len;

	if(Type != 0)
		sql_len = sprintf(sql, "select * from server.Resources where POID='%d' and Type='%d';", ObjectID, Type);
	else
		sql_len = sprintf(sql, "select * from server.Resources where POID='%d';", ObjectID);
	res = mysql_real_query(pcon, sql, sql_len);
	result = mysql_store_result(pcon);
	while(sql_row = mysql_fetch_row(result))
	{
		dst = readResourceInformation_new(sql_row);
		ResourceMetas.push_back(dst);
	}
	return ResourceMetas;
}
//********************************
std::vector<ResourceMeta>* DBConnect::loadResourceMeta(std::vector<Object> Objects, int Type)
{
	std::vector<ResourceMeta> ResourceMetas;
	std::vector<ResourceMeta>* collection = new std::vector<ResourceMeta>[Objects.size()];
	int i;

	for(i=0;i<Objects.size();i++)
	{
		ResourceMetas = loadResourceMeta(Objects[i].ObjectID, Type);
		collection[i] = ResourceMetas;
	}

	return collection;
}
//********************************
std::vector<ResourceMeta> DBConnect::loadResourceMeta(Vector4 Origin, Frustum frustum, int Type)
{
	std::vector<ResourceMeta> ResourceMetas;
	std::vector<ResourceMeta> temp_ResourceMetas;
	std::vector<Geometry> Geometries;
	float tempDis,tempL,tempR;
	Vector4 tempRange,tempOrigin;

	float nearPos_x, nearPos_y, nearPos_z;
	float farPos_x, farPos_y, farPos_z;
	float mid_x,mid_y,mid_z;
	float temp_x, temp_y, temp_z;
	float cos_temp_theta;
	float cos_temp_alpha;

	float k;
	int i,j;
	char sql[100];
	int sql_len;

	nearPos_z = (frustum.dir_z * frustum.nearDis) / sqrt(pow(frustum.dir_x,2) + pow(frustum.dir_y,2) + pow(frustum.dir_z,2)) + Origin.z;
	nearPos_x = frustum.dir_x * (nearPos_z - Origin.z) / frustum.dir_z + Origin.x;
	nearPos_y = frustum.dir_y * (nearPos_z - Origin.z) / frustum.dir_z + Origin.y;

	farPos_z = (frustum.dir_z * frustum.farDis) / sqrt(pow(frustum.dir_x,2) + pow(frustum.dir_y,2) + pow(frustum.dir_z,2)) + Origin.z;
	farPos_x = frustum.dir_x * (farPos_z - Origin.z) / frustum.dir_z + Origin.x;
	farPos_y = frustum.dir_y * (farPos_z - Origin.z) / frustum.dir_z + Origin.y;

	mid_x = (nearPos_x + farPos_x)/2;
	mid_y = (nearPos_y + farPos_y)/2;
	mid_z = (nearPos_z + farPos_z)/2;

	tempDis = sqrt(pow(farPos_x-mid_x,2) + pow(farPos_y-mid_y,2) + pow(farPos_z-mid_z,2));
	tempL = frustum.farDis * tan(frustum.theta);
	tempR = sqrt(pow(tempDis,2) + pow(tempL,2));
	tempRange.setVector4(tempR,tempR,tempR,frustum.t);
	tempOrigin.setVector4(mid_x,mid_y,mid_z,Origin.t);
	Geometries = loadGeometry(tempOrigin,tempRange,2);
	
	for(i = 0;i < Geometries.size();i++)
	{
		k = (frustum.dir_z * (Origin.z - Geometries[i].WorldPositionZ) + frustum.dir_y * (Origin.y - Geometries[i].WorldPositionY) + frustum.dir_x * (Origin.x - Geometries[i].WorldPositionX)) / (pow(frustum.dir_x,2) + pow(frustum.dir_y,2) + pow(frustum.dir_z,2));
		temp_x = k * frustum.dir_x + Geometries[i].WorldPositionX;
		temp_y = k * frustum.dir_y + Geometries[i].WorldPositionY;
		temp_z = k * frustum.dir_z + Geometries[i].WorldPositionZ;

		cos_temp_theta = (frustum.dir_x * (Geometries[i].WorldPositionX - Origin.x) + frustum.dir_y * (Geometries[i].WorldPositionY - Origin.y) + frustum.dir_z * (Geometries[i].WorldPositionZ - Origin.z)) / (sqrt(pow(frustum.dir_x,2) + pow(frustum.dir_y,2) + pow(frustum.dir_z,2)) * sqrt(pow(Geometries[i].WorldPositionX - Origin.x,2) + pow(Geometries[i].WorldPositionY - Origin.y,2) + pow(Geometries[i].WorldPositionZ - Origin.z,2)));
		
		if(temp_x>nearPos_x && temp_y>nearPos_y && temp_z>nearPos_z && temp_x<farPos_x && temp_y<farPos_y && temp_z<farPos_z)
		{
			if(cos(frustum.theta)<=cos_temp_theta)
			{
				temp_ResourceMetas = loadResourceMeta(Geometries[i].WorldPositionX, Geometries[i].WorldPositionY, Geometries[i].WorldPositionZ, Type);
				for(j=0;j<temp_ResourceMetas.size();j++)
				{	
					encodeTool.R = encodeTool.Quaternion2RotationMatrix(temp_ResourceMetas[j].transform.AttitudeX,temp_ResourceMetas[j].transform.AttitudeY,temp_ResourceMetas[j].transform.AttitudeZ,temp_ResourceMetas[j].transform.AttitudeW);
					encodeTool.init_vector << 0,1,0;
					encodeTool.temp_vector = encodeTool.R * encodeTool.init_vector;
					encodeTool.temp_dir_vector << frustum.dir_x,frustum.dir_y,frustum.dir_z;
					cos_temp_alpha = encodeTool.temp_dir_vector.normalized().dot(encodeTool.temp_vector.normalized());
			
					if(cos_temp_alpha<cos(frustum.alpha))
					{
						ResourceMetas.push_back(temp_ResourceMetas[j]);				
					}	
				} 
			}
		}
	}
	return ResourceMetas;
}
//********************************
//
//
std::vector<ResourceMeta> DBConnect::loadResourceMeta(Vector4 Origin, Vector4 Range, Target target, double theta, int Type)
{
	
	std::vector<ResourceMeta> ResourceMetas;
	std::vector<ResourceMeta> temp_ResourceMetas;
	std::vector<Geometry> Geometries;
	EncodeTool encodeTool;
	int i,j;

	double dir_x,dir_y,dir_z;
	double temp_x,temp_t,temp_z;
	double cos_temp_theta;
	double roll,pitch,yaw;
	
	Geometries = loadGeometry(Origin,Range,4);

	for(i = 0;i < Geometries.size();i++)
	{
		dir_x = target.targetX - Geometries[i].WorldPositionX;
		dir_y = target.targetY - Geometries[i].WorldPositionY;
		dir_z = target.targetZ - Geometries[i].WorldPositionZ;
		temp_ResourceMetas = loadResourceMeta(Geometries[i].ID, Type);
		
		for(j=0;j<temp_ResourceMetas.size();j++)
		{
			double Rx = temp_ResourceMetas[j].transform.AttitudeX;
			double Ry = temp_ResourceMetas[j].transform.AttitudeY;
			double Rz = temp_ResourceMetas[j].transform.AttitudeZ;
			double Rw = temp_ResourceMetas[j].transform.AttitudeW;
	
			encodeTool.R = encodeTool.Quaternion2RotationMatrix(Rx,Ry,Rz,Rw);
			encodeTool.init_vector << 0,0,1;
			encodeTool.temp_vector = encodeTool.R * encodeTool.init_vector;
			encodeTool.temp_dir_vector << dir_x,dir_y,dir_z;
			cos_temp_theta = encodeTool.temp_dir_vector.normalized().dot(encodeTool.temp_vector.normalized());
			if(cos_temp_theta > cos(theta))
			{
				ResourceMetas.push_back(temp_ResourceMetas[j]);				
			}
		}

		
	}

	return ResourceMetas;
}
//********************************
std::vector<ResourceMeta> DBConnect::loadResourceMeta(Vector4 Origin, Vector4 Range, Target target, double alpha, double beta, int Type)
{
	
	std::vector<ResourceMeta> ResourceMetas;
	std::vector<ResourceMeta> temp_ResourceMetas;
	std::vector<Geometry> Geometries;
	EncodeTool encodeTool;
	int i,j;

	double dir_x,dir_y,dir_z;
	double temp_x,temp_t,temp_z;
	double cos_temp_theta,cos_temp_theta_A,cos_temp_theta_B;
	double resultA,resultB;
	double roll,pitch,yaw;
	
	Geometries = loadGeometry(Origin,Range,4);

	for(i = 0;i < Geometries.size();i++)
	{
		dir_x = target.targetX - Geometries[i].WorldPositionX;
		dir_y = target.targetY - Geometries[i].WorldPositionY;
		dir_z = target.targetZ - Geometries[i].WorldPositionZ;
		temp_ResourceMetas = loadResourceMeta(Geometries[i].ID, Type);
		
		for(j=0;j<temp_ResourceMetas.size();j++)
		{
			Eigen::Vector3d temp_z(0,0,1),temp_y(0,1,0),temp_x(1,0,0),temp_dir_vector_y(0,0,0),temp_dir_vector_x(0,0,0),temp_dir_vector_z(0,0,0);
			Eigen::Vector3d temp_vector(0,0,0),init_vector(0,0,0);
			Eigen::Vector3d vector_weight_with_z(0,0,0),vector_weight_notwith_z(0,0,0);
			Eigen::Matrix3d rotMatrix,R;
			
			double Rx = temp_ResourceMetas[j].transform.AttitudeX;
			double Ry = temp_ResourceMetas[j].transform.AttitudeY;
			double Rz = temp_ResourceMetas[j].transform.AttitudeZ;
			double Rw = temp_ResourceMetas[j].transform.AttitudeW;
	
			R = encodeTool.Quaternion2RotationMatrix(Rx,Ry,Rz,Rw);
			init_vector << 0,0,1;
			temp_vector = (R * init_vector).normalized();  //姿势向量
			temp_dir_vector_z << dir_x,dir_y,dir_z;  //摄像机Z轴
			temp_dir_vector_z = temp_dir_vector_z.normalized();

			rotMatrix = Eigen::Quaterniond::FromTwoVectors(temp_z.normalized(),temp_dir_vector_z.normalized());
			temp_dir_vector_y = (rotMatrix * temp_y).normalized(); //摄像机y轴
			temp_dir_vector_x = (rotMatrix * temp_x).normalized(); //摄像机x轴

			cos_temp_theta = temp_dir_vector_z.dot(temp_vector);
			vector_weight_with_z = temp_vector * cos_temp_theta;
			vector_weight_notwith_z = temp_vector - vector_weight_with_z;

			cos_temp_theta_A = vector_weight_notwith_z.dot(temp_dir_vector_x);
			cos_temp_theta_B = vector_weight_notwith_z.dot(temp_dir_vector_y);

			resultA = (vector_weight_notwith_z * cos_temp_theta_A).norm();
			resultB = (vector_weight_notwith_z * cos_temp_theta_B).norm();
			
			//判断
			if(resultA <= sin(alpha) && resultB <= sin(beta) && resultA >=0 && resultB >= 0)
			{
				ResourceMetas.push_back(temp_ResourceMetas[j]);				
			}
		}

		
	}

	return ResourceMetas;
}

//********************************













