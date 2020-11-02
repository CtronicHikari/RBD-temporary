#include "Connect.h"
#include<math.h>
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
const void DBConnect::addObject(sigma::Object *Object)
{
	char sql[200];
	int sql_len = sprintf(sql, "insert into server.object(OID,POID) values('%d','%d')", Object->ObjectID, Object->ParentNodeID);
	res = mysql_real_query(pcon, sql, sql_len);

	//link_Geo_and_Obj(Object->ObjectID, GeometryID);
}

const string DBConnect::addGeometry(sigma::Position *pos_est, sigma::Attitude *atti_est, sigma::Position *pos_obs, sigma::Attitude *atti_obs, sigma::Time *time)
{
	string ID = "";
	char sql[5000];
	int sql_len;
	if(pos_est == NULL || atti_est == NULL)
	{
		sigma::Position *pos__est = new sigma::Position(pos_obs->Posx, pos_obs->Posy, pos_obs->Posz);
		sigma::Attitude *atti__est = new sigma::Attitude(atti_obs->Attitudex, atti_obs->Attitudey, atti_obs->Attitudez, atti_obs->Attitudew);
		ID += to_string(pos__est->Posx) + "_" + to_string(pos__est->Posy) + "_" + to_string(pos__est->Posz) +"_";
		ID += to_string(atti__est->Attitudex) + "_" + to_string(atti__est->Attitudey) + "_" + to_string(atti__est->Attitudez) + "_" + to_string(atti__est->Attitudew) + "_";
		ID += to_string(time->s) + "." + to_string(time->ns);
		ID = encodeTool.getMD5(ID);
		sql_len = sprintf(sql, "insert into server.geometry(ID,Posx_est,Posy_est,Posz_est,Attix_est,Attiy_est,Attiz_est,Attiw_est,Posx_obs,Posy_obs,Posz_obs,Attix_obs,Attiy_obs,Attiz_obs,Attiw_obs,Time_s,Time_ns) values('%s','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%u','%u')", ID.c_str(), pos__est->Posx, pos__est->Posy, pos__est->Posz, atti__est->Attitudex, atti__est->Attitudey, atti__est->Attitudez, atti__est->Attitudew, pos_obs->Posx, pos_obs->Posy, pos_obs->Posz, atti_obs->Attitudex,atti_obs->Attitudey,atti_obs->Attitudez,atti_obs->Attitudew,time->s,time->ns);
		res = mysql_real_query(pcon,sql,sql_len);
	}
	else
	{
		ID += to_string(pos_est->Posx) + "_" + to_string(pos_est->Posy) + "_" + to_string(pos_est->Posz) +"_";
		ID += to_string(atti_est->Attitudex) + "_" + to_string(atti_est->Attitudey) + "_" + to_string(atti_est->Attitudez) + "_" + to_string(atti_est->Attitudew) + "_";
		ID += to_string(time->s) + "." + to_string(time->ns);
		ID = encodeTool.getMD5(ID);
		sql_len = sprintf(sql, "insert into server.geometry(ID,Posx_est,Posy_est,Posz_est,Attix_est,Attiy_est,Attiz_est,Attiw_est,Posx_obs,Posy_obs,Posz_obs,Attix_obs,Attiy_obs,Attiz_obs,Attiw_obs,Time_s,Time_ns) values('%s','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%.20f','%u','%u')", ID.c_str(), pos_est->Posx, pos_est->Posy, pos_est->Posz, atti_est->Attitudex, atti_est->Attitudey, atti_est->Attitudez, atti_est->Attitudew, pos_obs->Posx, pos_obs->Posy, pos_obs->Posz, atti_obs->Attitudex,atti_obs->Attitudey,atti_obs->Attitudez,atti_obs->Attitudew,time->s,time->ns);
		res = mysql_real_query(pcon,sql,sql_len);
	}	
	return ID;
}

const string DBConnect::addGeometryForObject(string ID, int style, sigma::Position *pos_offset, sigma::Attitude *atti_offset, sigma::Time *time)
{
	
}

const void DBConnect::addResource(string ResourceID, int ParentObjectID, int Type, int Format, string Parameters, string save_path)
{
	int sql_len;
	char sql[500];
	
	sql_len = sprintf(sql, "insert into server.resource(ID,OID,Type,Format,Path,Para) values('%s','%d','%d','%d','%s','%s')",ResourceID.c_str(),ParentObjectID,Type,Format,save_path.c_str(),Parameters.c_str());
	res = mysql_real_query(pcon, sql, sql_len);
}

string DBConnect::make_ResourceID(int ParentObjectID, sigma::Position *pos, sigma::Attitude *atti, sigma::Time *time)
{
	string ID = "";
	ID += to_string(ParentObjectID) + "_";
	ID += to_string(pos->Posx) + "_" + to_string(pos->Posy) + "_" + to_string(pos->Posz) +"_";
	ID += to_string(atti->Attitudex) + "_" + to_string(atti->Attitudey) + "_" + to_string(atti->Attitudez) + "_" + to_string(atti->Attitudew) + "_";
	ID += to_string(time->s) + "." + to_string(time->ns);
	ID = encodeTool.getMD5(ID);
	return ID;
}

const void DBConnect::link_Geo_and_Obj(int ObjectID, string GeometryID)
{
	char sql[100];
	int sql_len = sprintf(sql,"insert into server.link_geo_and_obj(OID,GID) values('%d','%s')", ObjectID, GeometryID.c_str());
	res = mysql_real_query(pcon, sql, sql_len);
}

const void DBConnect::link_Geo_and_Res(string ResourceID, string GeometryID)
{
	char sql[200];
	int sql_len = sprintf(sql,"insert into server.link_geo_and_res(RID,GID) values('%s','%s')", ResourceID.c_str(), GeometryID.c_str());
	res = mysql_real_query(pcon, sql, sql_len);
}

const std::vector<sigma::Geometry> DBConnect::loadGeometry(sigma::Vector4 *Origin, sigma::Vector4 *Range, int Mode)
{
	std::vector<sigma::Geometry> Geometries;
	sigma::Geometry src;
	string temp;
	int sql_len;
	char sql[1000];
	
	/*if(Mode == 1)  //Sphere: If no _est use _phys
	{
		if(Range->x > 0 && Range->y > 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,(POW(Posx_est-'%f',2)/POW('%f',2))+(POW(Posy_est-'%f',2)/POW('%f',2))+(POW(Posz_est-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),(POW(Posx_phys-'%f',2)/POW('%f',2))+(POW(Posy_phys-'%f',2)/POW('%f',2))+(POW(Posz_phys-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		
		else if(Range->x > 0 && Range->y > 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,(POW(Posx_est-'%f',2)/POW('%f',2))+(POW(Posy_est-'%f',2)/POW('%f',2))+(POW(Posz_est-'%f',2)/POW('%f',2))<1,(POW(Posx_phys-'%f',2)/POW('%f',2))+(POW(Posy_phys-'%f',2)/POW('%f',2))+(POW(Posz_phys-'%f',2)/POW('%f',2))<1);", Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->z, Range->z/2);
		}
		else if(Range->x > 0 && Range->y > 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,(POW(Posx_est-'%f',2)/POW('%f',2))+(POW(Posy_est-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),(POW(Posx_phys-'%f',2)/POW('%f',2))+(POW(Posy_phys-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x > 0 && Range->y > 0 && Range->z <= 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,(POW(Posx_est-'%f',2)/POW('%f',2))+(POW(Posy_est-'%f',2)/POW('%f',2))<1,(POW(Posx_phys-'%f',2)/POW('%f',2))+(POW(Posy_phys-'%f',2)/POW('%f',2))<1);", Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->x, Range->x/2, Origin->y, Range->y/2);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,(POW(Posx_est-'%f',2)/POW('%f',2))+(POW(Posz_est-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),(POW(Posx_phys-'%f',2)/POW('%f',2))+(POW(Posz_phys-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->x, Range->x/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->x, Range->x/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,(POW(Posx_est-'%f',2)/POW('%f',2))+(POW(Posz_est-'%f',2)/POW('%f',2))<1,(POW(Posx_phys-'%f',2)/POW('%f',2))+(POW(Posz_phys-'%f',2)/POW('%f',2))<1);", Origin->x, Range->x/2, Origin->z, Range->z/2, Origin->x, Range->x/2, Origin->z, Range->z/2);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,(POW(Posy_est-'%f',2)/POW('%f',2))+(POW(Posz_est-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),(POW(Posy_phys-'%f',2)/POW('%f',2))+(POW(Posz_phys-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,(POW(Posy_est-'%f',2)/POW('%f',2))+(POW(Posz_est-'%f',2)/POW('%f',2))<1,(POW(Posy_phys-'%f',2)/POW('%f',2))+(POW(Posz_phys-'%f',2)/POW('%f',2))<1);", Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->y, Range->y/2, Origin->z, Range->z/2);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,(POW(Posx_est-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),(POW(Posx_phys-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->x, Range->x/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->x, Range->x/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z <= 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,(POW(Posx_est-'%f',2)/POW('%f',2))<1,(POW(Posx_phys-'%f',2)/POW('%f',2))<1);", Origin->x, Range->x/2, Origin->x, Range->x/2);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,(POW(Posy_est-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),(POW(Posy_phys-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->y, Range->y/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->y, Range->y/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z <= 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,(POW(Posy_est-'%f',2)/POW('%f',2))<1,(POW(Posy_phys-'%f',2)/POW('%f',2))<1);", Origin->y, Range->y/2,Origin->y, Range->y/2);
		}
		else if(Range->x <= 0 && Range->y <= 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,(POW(Posz_est-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),(POW(Posz_phys-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x <= 0 && Range->y <= 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,(POW(Posz_est-'%f',2)/POW('%f',2))<1,(POW(Posz_phys-'%f',2)/POW('%f',2))<1);", Origin->z, Range->z/2, Origin->z, Range->z/2);
		}
		else if(Range->x <= 0 && Range->y <= 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else
		{
			sql_len = sprintf(sql, "select * from server.geometry;");
		}
	}
	else if(Mode == 2) //Cube: If no _est use _phys
	{
		if(Range->x > 0 && Range->y > 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,ABS(Posx_est-'%f')<='%f' AND ABS(Posy_est-'%f')<='%f' AND ABS(Posz_est-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),ABS(Posx_phys-'%f')<='%f' AND ABS(Posy_phys-'%f')<='%f' AND ABS(Posz_phys-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		
		else if(Range->x > 0 && Range->y > 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,ABS(Posx_est-'%f')<='%f' AND ABS(Posy_est-'%f')<='%f' AND ABS(Posz_est-'%f')<='%f',ABS(Posx_phys-'%f')<='%f' AND ABS(Posy_phys-'%f')<='%f' AND ABS(Posz_phys-'%f')<='%f');", Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->z, Range->z/2);
		}
		else if(Range->x > 0 && Range->y > 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,ABS(Posx_est-'%f')<='%f' AND ABS(Posy_est-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),ABS(Posx_phys-'%f')<='%f' AND ABS(Posy_phys-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x > 0 && Range->y > 0 && Range->z <= 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,ABS(Posx_est-'%f')<='%f' AND ABS(Posy_est-'%f')<='%f',ABS(Posx_phys-'%f')<='%f' AND ABS(Posy_phys-'%f')<='%f');", Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->x, Range->x/2, Origin->y, Range->y/2);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,ABS(Posx_est-'%f')<='%f' AND ABS(Posz_est-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),ABS(Posx_phys-'%f')<='%f' AND ABS(Posz_phys-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->x, Range->x/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->x, Range->x/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,ABS(Posx_est-'%f')<='%f' AND ABS(Posz_est-'%f')<='%f',ABS(Posx_phys-'%f')<='%f' AND ABS(Posz_phys-'%f')<='%f');", Origin->x, Range->x/2, Origin->z, Range->z/2, Origin->x, Range->x/2, Origin->z, Range->z/2);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,ABS(Posy_est-'%f')<='%f' AND ABS(Posz_est-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),ABS(Posy_phys-'%f')<='%f' AND ABS(Posz_phys-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,ABS(Posy_est-'%f')<='%f' AND ABS(Posz_est-'%f')<='%f',ABS(Posy_phys-'%f')<='%f' AND ABS(Posz_phys-'%f')<='%f');", Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->y, Range->y/2, Origin->z, Range->z/2);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,ABS(Posx_est-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),ABS(Posx_phys-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->x, Range->x/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->x, Range->x/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z <= 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,ABS(Posx_est-'%f')<='%f',ABS(Posx_phys-'%f')<='%f');", Origin->x, Range->x/2, Origin->x, Range->x/2);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,ABS(Posy_est-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),ABS(Posy_phys-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->y, Range->y/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->y, Range->y/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z <= 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,ABS(Posy_est-'%f')<='%f',ABS(Posy_phys-'%f')<='%f');", Origin->y, Range->y/2, Origin->y, Range->y/2);
		}
		else if(Range->x <= 0 && Range->y <= 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,ABS(Posz_est-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),ABS(Posz_phys-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x <= 0 && Range->y <= 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,ABS(Posz_est-'%f')<='%f',ABS(Posz_phys-'%f')<='%f');", Origin->z, Range->z/2, Origin->z, Range->z/2);
		}
		else if(Range->x <= 0 && Range->y <= 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Posx_est IS NOT NULL,IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'),IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u'));", Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else
		{
			sql_len = sprintf(sql, "select * from server.geometry;");
		}
	}*/
	if(Mode == 1)  //Sphere: Only use _est
	{
		if(Range->x > 0 && Range->y > 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(Posx_est-'%f',2)/POW('%f',2))+(POW(Posy_est-'%f',2)/POW('%f',2))+(POW(Posz_est-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		
		else if(Range->x > 0 && Range->y > 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(Posx_est-'%f',2)/POW('%f',2))+(POW(Posy_est-'%f',2)/POW('%f',2))+(POW(Posz_est-'%f',2)/POW('%f',2))<1;", Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->z, Range->z/2);
		}
		else if(Range->x > 0 && Range->y > 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(Posx_est-'%f',2)/POW('%f',2))+(POW(Posy_est-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->time.s, Origin->time.s, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x > 0 && Range->y > 0 && Range->z <= 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(Posx_est-'%f',2)/POW('%f',2))+(POW(Posy_est-'%f',2)/POW('%f',2))<1;", Origin->x, Range->x/2, Origin->y, Range->y/2);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(Posx_est-'%f',2)/POW('%f',2))+(POW(Posz_est-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->x, Range->x/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(Posx_est-'%f',2)/POW('%f',2))+(POW(Posz_est-'%f',2)/POW('%f',2))<1;", Origin->x, Range->x/2, Origin->z, Range->z/2);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(Posy_est-'%f',2)/POW('%f',2))+(POW(Posz_est-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(Posy_est-'%f',2)/POW('%f',2))+(POW(Posz_est-'%f',2)/POW('%f',2))<1;", Origin->y, Range->y/2, Origin->z, Range->z/2);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(Posx_est-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->x, Range->x/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z <= 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(Posx_est-'%f',2)/POW('%f',2))<1;", Origin->x, Range->x/2);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(Posy_est-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->y, Range->y/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z <= 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(Posy_est-'%f',2)/POW('%f',2))<1;", Origin->y, Range->y/2);
		}
		else if(Range->x <= 0 && Range->y <= 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(Posz_est-'%f',2)/POW('%f',2))<1 AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x <= 0 && Range->y <= 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where (POW(Posz_est-'%f',2)/POW('%f',2))<1;", Origin->z, Range->z/2);
		}
		else if(Range->x <= 0 && Range->y <= 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else
		{
			sql_len = sprintf(sql, "select * from server.geometry;");
		}
	}	
	else if(Mode == 2) //Cube: Only use _est
	{
		if(Range->x > 0 && Range->y > 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(Posx_est-'%f')<='%f' AND ABS(Posy_est-'%f')<='%f' AND ABS(Posz_est-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		
		else if(Range->x > 0 && Range->y > 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(Posx_est-'%f')<='%f' AND ABS(Posy_est-'%f')<='%f' AND ABS(Posz_est-'%f')<='%f';", Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->z, Range->z/2);
		}
		else if(Range->x > 0 && Range->y > 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(Posx_est-'%f')<='%f' AND ABS(Posy_est-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->x, Range->x/2, Origin->y, Range->y/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x > 0 && Range->y > 0 && Range->z <= 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(Posx_est-'%f')<='%f' AND ABS(Posy_est-'%f')<='%f');", Origin->x, Range->x/2, Origin->y, Range->y/2);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(Posx_est-'%f')<='%f' AND ABS(Posz_est-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->x, Range->x/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(Posx_est-'%f')<='%f' AND ABS(Posz_est-'%f')<='%f';", Origin->x, Range->x/2, Origin->z, Range->z/2);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(Posy_est-'%f')<='%f' AND ABS(Posz_est-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->y, Range->y/2, Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(Posy_est-'%f')<='%f' AND ABS(Posz_est-'%f')<='%f';", Origin->y, Range->y/2, Origin->z, Range->z/2);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(Posx_est-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->x, Range->x/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x > 0 && Range->y <= 0 && Range->z <= 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(Posx_est-'%f')<='%f';", Origin->x, Range->x/2);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(Posy_est-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->y, Range->y/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x <= 0 && Range->y > 0 && Range->z <= 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(Posy_est-'%f')<='%f';", Origin->y, Range->y/2);
		}
		else if(Range->x <= 0 && Range->y <= 0 && Range->z > 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(Posz_est-'%f')<='%f' AND IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->z, Range->z/2, Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else if(Range->x <= 0 && Range->y <= 0 && Range->z > 0 && Range->time.s <= 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where ABS(Posz_est-'%f')<='%f';", Origin->z, Range->z/2);
		}
		else if(Range->x <= 0 && Range->y <= 0 && Range->z <= 0 && Range->time.s > 0)
		{
			sql_len = sprintf(sql, "select * from server.geometry where IF(Time_s='%u',Time_ns>='%u',Time_s>='%u') AND IF(Time_s='%u',Time_ns<='%u',Time_s<='%u');", Origin->time.s, Origin->time.ns, Origin->time.s, Range->time.s, Range->time.ns, Range->time.s);
		}
		else
		{
			sql_len = sprintf(sql, "select * from server.geometry;");
		}
	}
	else
	{
		cout << "Mode Error" << endl;
		Geometries.clear();
		return Geometries;
	}
	
	res = mysql_real_query(pcon, sql, sql_len);

	result = mysql_store_result(pcon);

	while(sql_row = mysql_fetch_row(result))
	{
		src.ID = sql_row[0];
		src.pos_est.Posx = sql_row[1]==NULL ? 0.0f : strtod(sql_row[1],NULL);
		src.pos_est.Posy = sql_row[2]==NULL ? 0.0f : strtod(sql_row[2],NULL);
		src.pos_est.Posz = sql_row[3]==NULL ? 0.0f : strtod(sql_row[3],NULL);
		src.atti_est.Attitudex = sql_row[4]==NULL ? 0.0f : strtod(sql_row[4],NULL);
		src.atti_est.Attitudey = sql_row[5]==NULL ? 0.0f : strtod(sql_row[5],NULL);
		src.atti_est.Attitudez = sql_row[6]==NULL ? 0.0f : strtod(sql_row[6],NULL);
		src.atti_est.Attitudew = sql_row[7]==NULL ? 0.0f : strtod(sql_row[7],NULL);
		src.pos_obs.Posx = strtod(sql_row[8],NULL);
		src.pos_obs.Posy = strtod(sql_row[9],NULL);
		src.pos_obs.Posz = strtod(sql_row[10],NULL);
		src.atti_obs.Attitudex = strtod(sql_row[11],NULL);
		src.atti_obs.Attitudey = strtod(sql_row[12],NULL);
		src.atti_obs.Attitudez = strtod(sql_row[13],NULL);
		src.atti_obs.Attitudew = strtod(sql_row[14],NULL);
		src.time.s = encodeTool.string_to_unsigned(sql_row[15]);
		src.time.ns = encodeTool.string_to_unsigned(sql_row[16]);
		Geometries.push_back(src);
	}
	return Geometries;
}

const std::vector<sigma::Object> DBConnect::loadObject(sigma::Vector4 *Origin, sigma::Vector4 *Range, int Mode)
{
	std::vector<sigma::Object> Objects;
	std::vector<sigma::Geometry> Geometries;
	std::vector<int> OIDs;
	sigma::Object src;
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

sigma::ResourceMeta DBConnect::readResourceInformation(MYSQL_ROW src)
{
	sigma::ResourceMeta dst;
	int sql_len,i;
	char sql[300];
	string temp;
	string temp_GID;

	dst.ID = src[0];
	temp = src[1];  	dst.ParentObjectID = atoi(temp.c_str());
	temp = src[2];  	dst.Type = atoi(temp.c_str());
	temp = src[3];		dst.Format = atoi(temp.c_str());
	dst.Path = src[4];
	dst.Parameters = src[5];

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
		dst.transform.pos.Posx = sql_row[1]==NULL ? strtod(sql_row[8],NULL) : strtod(sql_row[1],NULL);
		dst.transform.pos.Posy = sql_row[2]==NULL ? strtod(sql_row[9],NULL) : strtod(sql_row[2],NULL);
		dst.transform.pos.Posz = sql_row[3]==NULL ? strtod(sql_row[10],NULL) : strtod(sql_row[3],NULL);
		dst.transform.atti.Attitudex = sql_row[4]==NULL ? strtod(sql_row[11],NULL) : strtod(sql_row[4],NULL);
		dst.transform.atti.Attitudey = sql_row[5]==NULL ? strtod(sql_row[12],NULL) : strtod(sql_row[5],NULL);
		dst.transform.atti.Attitudez = sql_row[6]==NULL ? strtod(sql_row[13],NULL) : strtod(sql_row[6],NULL);
		dst.transform.atti.Attitudew = sql_row[7]==NULL ? strtod(sql_row[14],NULL) : strtod(sql_row[7],NULL);
		dst.transform.time.s = encodeTool.string_to_unsigned(sql_row[15]);
		dst.transform.time.ns = encodeTool.string_to_unsigned(sql_row[16]);
	}

	return dst;
}

std::vector<sigma::ResourceMeta> DBConnect::loadResourceMeta(sigma::Vector4 *Origin, sigma::Vector4 *Range, int Mode, int Type)
{
	std::vector<sigma::ResourceMeta> ResourceMetas;
	std::vector<sigma::Geometry> Geometries;
	std::vector<string> RIDs;
	sigma::ResourceMeta dst;
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
			sql_len = sprintf(sql, "select * from server.resource where ID='%s' and Type='%d';", RIDs[i].c_str(), Type);
		else
			sql_len = sprintf(sql, "select * from server.resource where ID='%s';", RIDs[i].c_str());
		res = mysql_real_query(pcon, sql, sql_len);
		result = mysql_store_result(pcon);
		while(sql_row = mysql_fetch_row(result))
		{
			dst = readResourceInformation(sql_row);
			ResourceMetas.push_back(dst);
		}
	}
	
	return ResourceMetas;
}

std::vector<sigma::ResourceMeta> DBConnect::loadResourceMeta(int ObjectID, int Type)
{
	std::vector<sigma::ResourceMeta> ResourceMetas;
	sigma::ResourceMeta dst;
	string temp;
	char sql[100];
	int sql_len;

	if(Type != 0)
		sql_len = sprintf(sql, "select * from server.resource where OID='%d' and Type='%d';", ObjectID, Type);
	else
		sql_len = sprintf(sql, "select * from server.resource where OID='%d';", ObjectID);
	res = mysql_real_query(pcon, sql, sql_len);
	result = mysql_store_result(pcon);
	while(sql_row = mysql_fetch_row(result))
	{
		dst = readResourceInformation(sql_row);
		ResourceMetas.push_back(dst);
	}
	return ResourceMetas;
}

std::vector<sigma::ResourceMeta>* DBConnect::loadResourceMeta(std::vector<sigma::Object> Objects, int Type)
{
	std::vector<sigma::ResourceMeta> ResourceMetas;
	std::vector<sigma::ResourceMeta>* collection = new std::vector<sigma::ResourceMeta>[Objects.size()];
	int i;

	for(i=0;i<Objects.size();i++)
	{
		ResourceMetas = loadResourceMeta(Objects[i].ObjectID, Type);
		collection[i] = ResourceMetas;
	}

	return collection;
}

std::vector<sigma::ResourceMeta> DBConnect::loadResourceMeta(string GeometryID, int Type)
{
	std::vector<sigma::ResourceMeta> ResourceMetas;
	std::vector<string> RIDs;
	sigma::ResourceMeta dst;
	string temp;
	char sql[300];
	int sql_len,i;


	sql_len = sprintf(sql, "select RID from server.link_geo_and_res where GID='%s';", GeometryID.c_str());
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
			sql_len = sprintf(sql, "select * from server.resource where ID='%s' and Type='%d';", RIDs[i].c_str(), Type);
		else
			sql_len = sprintf(sql, "select * from server.resource where ID='%s';", RIDs[i].c_str());
		res = mysql_real_query(pcon, sql, sql_len);
		result = mysql_store_result(pcon);
		while(sql_row = mysql_fetch_row(result))
		{
			dst = readResourceInformation(sql_row);
			ResourceMetas.push_back(dst);
		}
	}
	return ResourceMetas;
}

std::vector<sigma::ResourceMeta> DBConnect::loadResourceMeta(sigma::Vector4 *Origin, sigma::Vector4 *Range, sigma::Target *target, double theta, int Mode, int Type)
{
	
	std::vector<sigma::ResourceMeta> ResourceMetas;
	std::vector<sigma::ResourceMeta> temp_ResourceMetas;
	std::vector<sigma::Geometry> Geometries;
	EncodeTool encodeTool;
	int i,j;

	double dir_x,dir_y,dir_z;
	double temp_x,temp_t,temp_z;
	double cos_temp_theta;
	double roll,pitch,yaw;
	
	Geometries = loadGeometry(Origin,Range,Mode);

	for(i = 0;i < Geometries.size();i++)
	{
		//double tmp = (Geometries[i].Posx_est==0) ? Geometries[i].Posx_phys : Geometries[i].Posx_est;
		dir_x = target->targetX - Geometries[i].pos_est.Posx;
		//tmp = (Geometries[i].Posy_est==0) ? Geometries[i].Posy_phys : Geometries[i].Posy_est;
		dir_y = target->targetY - Geometries[i].pos_est.Posy;
		//tmp = (Geometries[i].Posz_est==0) ? Geometries[i].Posz_phys : Geometries[i].Posz_est;
		dir_z = target->targetZ - Geometries[i].pos_est.Posz;
		temp_ResourceMetas = loadResourceMeta(Geometries[i].ID, Type);
		
		for(j=0;j<temp_ResourceMetas.size();j++)
		{
			double Rx = temp_ResourceMetas[j].transform.atti.Attitudex;
			double Ry = temp_ResourceMetas[j].transform.atti.Attitudey;
			double Rz = temp_ResourceMetas[j].transform.atti.Attitudez;
			double Rw = temp_ResourceMetas[j].transform.atti.Attitudew;
	
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
std::vector<sigma::ResourceMeta> DBConnect::loadResourceMeta(sigma::Vector4 *Origin, sigma::Vector4 *Range, sigma::Target *target, double alpha, double beta, int Mode,int Type)
{
	
	std::vector<sigma::ResourceMeta> ResourceMetas;
	std::vector<sigma::ResourceMeta> temp_ResourceMetas;
	std::vector<sigma::Geometry> Geometries;
	EncodeTool encodeTool;
	int i,j;

	double dir_x,dir_y,dir_z;
	double temp_x,temp_t,temp_z;
	double cos_temp_theta,cos_temp_theta_A,cos_temp_theta_B;
	double resultA,resultB;
	double roll,pitch,yaw;
	
	Geometries = loadGeometry(Origin,Range,Mode);

	for(i = 0;i < Geometries.size();i++)
	{
		dir_x = target->targetX - Geometries[i].pos_est.Posx;
		dir_y = target->targetY - Geometries[i].pos_est.Posy;

		dir_z = target->targetZ - Geometries[i].pos_est.Posz;
		temp_ResourceMetas = loadResourceMeta(Geometries[i].ID, Type);
		
		for(j=0;j<temp_ResourceMetas.size();j++)
		{
			Eigen::Vector3d temp_z(0,0,1),temp_y(0,1,0),temp_x(1,0,0),temp_dir_vector_y(0,0,0),temp_dir_vector_x(0,0,0),temp_dir_vector_z(0,0,0);
			Eigen::Vector3d temp_vector(0,0,0),init_vector(0,0,0);
			Eigen::Vector3d vector_weight_with_z(0,0,0),vector_weight_notwith_z(0,0,0);
			Eigen::Matrix3d rotMatrix,R;
			
			double Rx = temp_ResourceMetas[j].transform.atti.Attitudex;
			double Ry = temp_ResourceMetas[j].transform.atti.Attitudey;
			double Rz = temp_ResourceMetas[j].transform.atti.Attitudez;
			double Rw = temp_ResourceMetas[j].transform.atti.Attitudew;
	
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

			if(resultA <= sin(alpha) && resultB <= sin(beta) && resultA >=0 && resultB >= 0)
			{
				ResourceMetas.push_back(temp_ResourceMetas[j]);				
			}
		}
	}
	return ResourceMetas;
}
