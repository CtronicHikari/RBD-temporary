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
	string geoID = "";
	int sql_len;
	char sql[500];
	string targetGID = "";
	sigma::Geometry src;

	if(style==1)
	{
		sql_len = sprintf(sql, "select GID from server.link_geo_and_obj where OID='%s';", ID.c_str());
	}
	else if(style==2)
	{
		sql_len = sprintf(sql, "select GID from server.link_geo_and_res where RID='%s';", ID.c_str());	
	}
	else
	{
		cout << "Get a wrong style" << endl;
		return "";
	}
	res = mysql_real_query(pcon, sql, sql_len);
	result = mysql_store_result(pcon);
	while(sql_row = mysql_fetch_row(result))
	{
		targetGID = sql_row[0];
	}
	sql_len = sprintf(sql, "select * from server.geometry where ID='%s';", targetGID.c_str());
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
	}

	src.pos_est.Posx += pos_offset->Posx;
	src.pos_est.Posy += pos_offset->Posy;
	src.pos_est.Posz += pos_offset->Posz;
	Eigen::Quaterniond temp(atti_offset->Attitudex, atti_offset->Attitudey, atti_offset->Attitudez, atti_offset->Attitudew);
	Eigen::Quaterniond origin(src.atti_est.Attitudex, src.atti_est.Attitudey, src.atti_est.Attitudez, src.atti_est.Attitudew);
	Eigen::Quaterniond newQuaternion = encodeTool.Blend2Rotations(temp, origin);
	src.atti_est.Attitudex = newQuaternion.x();
	src.atti_est.Attitudey = newQuaternion.y();
	src.atti_est.Attitudez = newQuaternion.z();
	src.atti_est.Attitudew = newQuaternion.w();
	sigma::Position *pos = new sigma::Position(src.pos_est.Posx, src.pos_est.Posy, src.pos_est.Posz);
	sigma::Attitude *atti = new sigma::Attitude(src.atti_est.Attitudex, src.atti_est.Attitudey, src.atti_est.Attitudez, src.atti_est.Attitudew);	
	geoID = addGeometry(pos, atti, pos, atti, time);		

	return geoID;	
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
	//cout << RIDs.size() << endl;
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

std::vector<sigma::ResourceMeta> DBConnect::loadResourceMeta(sigma::Vector4 *Origin, sigma::Vector4 *Range, sigma::BoudingBox *box, int Mode, int Type)
{
	std::vector<sigma::ResourceMeta> ResourceMetas;
	std::vector<sigma::ResourceMeta> temp_ResourceMetas;
	std::vector<sigma::Geometry> Geometries;
	std::vector<string> paras;
	EncodeTool encodeTool;

	double x0, y0, z0;  //camera origin
	double x_, y_, z_;  //camera corr z-axis in app corr (x_, y_, z_)
	double x_a, y_a, z_a; //camera corr x-axis in app corr (x_alpha, y_alpha, z_alpha)
	double x_b, y_b, z_b; //camera corr y-axis in app corr (x_beta, y_beta, z_beta)
	double theta; //Vertical angle of camera
	double eta;   //Horizontal angle of view
	//double imageSensor_x, imageSensor_y;  //camera image sensor size (Discard)
	double resolution_w, resolution_h;
	double focusDisx, focusDisy; //camera Focus Distance
	//imageSensor_x = 30.2f; imageSensor_y = 16.7f;  //APS-H dummy (mm)

	double A1, B1, C1;  double A2, B2, C2; double A3, B3, C3; double A4, B4, C4;
	sigma::Vector3 P1, P2, P3, P4, P5, P6, P7, P8;
	double dis1, dis2, dis3, dis4, dis5, dis6, dis7, dis8;
	//The eight points of the bouding box
	P1.setVector3(box->minPoint.x, box->maxPoint.y, box->minPoint.z);  P2.setVector3(box->minPoint.x, box->maxPoint.y, box->maxPoint.z);
	P3.setVector3(box->maxPoint.x, box->maxPoint.y, box->minPoint.z);  P4.setVector3(box->maxPoint.x, box->maxPoint.y, box->maxPoint.z);
	P5.setVector3(box->minPoint.x, box->minPoint.y, box->minPoint.z);  P6.setVector3(box->minPoint.x, box->minPoint.y, box->maxPoint.z);
	P7.setVector3(box->maxPoint.x, box->minPoint.y, box->minPoint.z);  P8.setVector3(box->maxPoint.x, box->minPoint.y, box->maxPoint.z);
	bool inner, inner_up, inner_down, inner_left, inner_right;

	Geometries = loadGeometry(Origin,Range,Mode);
	//cout << Geometries.size() << endl;
	for(int i = 0;i < Geometries.size();i++)
	{
		x0 = Geometries[i].pos_est.Posx; y0 = Geometries[i].pos_est.Posy; z0 = Geometries[i].pos_est.Posz;
		encodeTool.R = encodeTool.Quaternion2RotationMatrix(Geometries[i].atti_est.Attitudex,Geometries[i].atti_est.Attitudey,Geometries[i].atti_est.Attitudez,Geometries[i].atti_est.Attitudew);
		encodeTool.init_vector << 0,0,1; encodeTool.init_vectorx << 1,0,0; encodeTool.init_vectory << 0,1,0;
		encodeTool.init_vector = encodeTool.R * encodeTool.init_vector; 
		encodeTool.init_vectorx = encodeTool.R * encodeTool.init_vectorx;
		encodeTool.init_vectory = encodeTool.R * encodeTool.init_vectory;
		x_ = encodeTool.init_vector.x();   y_ = encodeTool.init_vector.y();   z_ = encodeTool.init_vector.z();
		x_a = encodeTool.init_vectorx.x(); y_a = encodeTool.init_vectorx.y(); z_a = encodeTool.init_vectorx.z();
		x_b = encodeTool.init_vectory.x(); y_b = encodeTool.init_vectory.y(); z_b = encodeTool.init_vectory.z();
		temp_ResourceMetas = loadResourceMeta(Geometries[i].ID, Type);
		for(int j=0;j<temp_ResourceMetas.size();j++)
		{
			paras = encodeTool.splitter(temp_ResourceMetas[j].Parameters, ',');
			focusDisx = encodeTool.str2double(paras[0].c_str());
			focusDisy = encodeTool.str2double(paras[1].c_str());
			resolution_w = encodeTool.str2double(paras[7].c_str());
			resolution_h = encodeTool.str2double(paras[8].c_str());
			theta = encodeTool.DegToRad(atan(resolution_h / (2 * focusDisx)));
			eta =   encodeTool.DegToRad(atan(resolution_w / (2 * focusDisy)));
			
			//Equation of 4 faces
			//Up A1*(x-x0)+B1*(y-y0)+C1*(z-z0) = 0
			A1 = x_ - (x_ + x_b * sqrt(x_ * x_ + y_ * y_ + z_ * z_) * tan(theta)) / (1 + tan(theta) * tan(theta));
			B1 = y_ - (y_ + y_b * sqrt(x_ * x_ + y_ * y_ + z_ * z_) * tan(theta)) / (1 + tan(theta) * tan(theta));
			C1 = z_ - (z_ + z_b * sqrt(x_ * x_ + y_ * y_ + z_ * z_) * tan(theta)) / (1 + tan(theta) * tan(theta));
			//Down A2*(x-x0)+B2*(y-y0)+C2*(z-z0) = 0
			A2 = x_ - (x_ - x_b * sqrt(x_ * x_ + y_ * y_ + z_ * z_) * tan(theta)) / (1 + tan(theta) * tan(theta));
			B2 = y_ - (y_ - y_b * sqrt(x_ * x_ + y_ * y_ + z_ * z_) * tan(theta)) / (1 + tan(theta) * tan(theta));
			C2 = z_ - (z_ - z_b * sqrt(x_ * x_ + y_ * y_ + z_ * z_) * tan(theta)) / (1 + tan(theta) * tan(theta));
			//Left A3*(x-x0)+B3*(y-y0)+C3*(z-z0) = 0
			A3 = x_ - (x_ + x_a * sqrt(x_ * x_ + y_ * y_ + z_ * z_) * tan(eta)) / (1 + tan(eta) * tan(eta));
			B3 = y_ - (y_ + y_a * sqrt(x_ * x_ + y_ * y_ + z_ * z_) * tan(eta)) / (1 + tan(eta) * tan(eta));
			C3 = z_ - (z_ + z_a * sqrt(x_ * x_ + y_ * y_ + z_ * z_) * tan(eta)) / (1 + tan(eta) * tan(eta));
			//Right A4*(x-x0)+B4*(y-y0)+C4*(z-z0) = 0
			A4 = x_ - (x_ - x_a * sqrt(x_ * x_ + y_ * y_ + z_ * z_) * tan(eta)) / (1 + tan(eta) * tan(eta));
			B4 = y_ - (y_ - y_a * sqrt(x_ * x_ + y_ * y_ + z_ * z_) * tan(eta)) / (1 + tan(eta) * tan(eta));
			C4 = z_ - (z_ - z_a * sqrt(x_ * x_ + y_ * y_ + z_ * z_) * tan(eta)) / (1 + tan(eta) * tan(eta));

			//Distance from the eight points of the collision box to the face
			//Up
			dis1 = A1 * (P1.x - x0) + B1 * (P1.y - y0) + C1 * (P1.z - z0);  dis2 = A1 * (P2.x - x0) + B1 * (P2.y - y0) + C1 * (P2.z - z0);
			dis3 = A1 * (P3.x - x0) + B1 * (P3.y - y0) + C1 * (P3.z - z0);  dis4 = A1 * (P4.x - x0) + B1 * (P4.y - y0) + C1 * (P4.z - z0);
			dis5 = A1 * (P5.x - x0) + B1 * (P5.y - y0) + C1 * (P5.z - z0);  dis6 = A1 * (P6.x - x0) + B1 * (P6.y - y0) + C1 * (P6.z - z0);
			dis7 = A1 * (P7.x - x0) + B1 * (P7.y - y0) + C1 * (P7.z - z0);  dis8 = A1 * (P8.x - x0) + B1 * (P8.y - y0) + C1 * (P8.z - z0);
			if(dis1<=0 && dis2<=0 && dis3<=0 && dis4<=0 && dis5<=0 && dis6<=0 && dis7<=0 && dis8<=0) inner_up = false;
			else inner_up = true;
			//Down
			dis1 = A2 * (P1.x - x0) + B2 * (P1.y - y0) + C2 * (P1.z - z0);  dis2 = A2 * (P2.x - x0) + B2 * (P2.y - y0) + C2 * (P2.z - z0);
			dis3 = A2 * (P3.x - x0) + B2 * (P3.y - y0) + C2 * (P3.z - z0);  dis4 = A2 * (P4.x - x0) + B2 * (P4.y - y0) + C2 * (P4.z - z0);
			dis5 = A2 * (P5.x - x0) + B2 * (P5.y - y0) + C2 * (P5.z - z0);  dis6 = A2 * (P6.x - x0) + B2 * (P6.y - y0) + C2 * (P6.z - z0);
			dis7 = A2 * (P7.x - x0) + B2 * (P7.y - y0) + C2 * (P7.z - z0);  dis8 = A2 * (P8.x - x0) + B2 * (P8.y - y0) + C2 * (P8.z - z0);
			if(dis1<=0 && dis2<=0 && dis3<=0 && dis4<=0 && dis5<=0 && dis6<=0 && dis7<=0 && dis8<=0) inner_up = false;
			else inner_up = true;
			//Left
			dis1 = A3 * (P1.x - x0) + B3 * (P1.y - y0) + C3 * (P1.z - z0);  dis2 = A3 * (P2.x - x0) + B3 * (P2.y - y0) + C3 * (P2.z - z0);
			dis3 = A3 * (P3.x - x0) + B3 * (P3.y - y0) + C3 * (P3.z - z0);  dis4 = A3 * (P4.x - x0) + B3 * (P4.y - y0) + C3 * (P4.z - z0);
			dis5 = A3 * (P5.x - x0) + B3 * (P5.y - y0) + C3 * (P5.z - z0);  dis6 = A3 * (P6.x - x0) + B3 * (P6.y - y0) + C3 * (P6.z - z0);
			dis7 = A3 * (P7.x - x0) + B3 * (P7.y - y0) + C3 * (P7.z - z0);  dis8 = A3 * (P8.x - x0) + B3 * (P8.y - y0) + C3 * (P8.z - z0);
			if(dis1<=0 && dis2<=0 && dis3<=0 && dis4<=0 && dis5<=0 && dis6<=0 && dis7<=0 && dis8<=0) inner_up = false;
			else inner_up = true;
			//Right
			dis1 = A4 * (P1.x - x0) + B4 * (P1.y - y0) + C4 * (P1.z - z0);  dis2 = A4 * (P2.x - x0) + B4 * (P2.y - y0) + C4 * (P2.z - z0);
			dis3 = A4 * (P3.x - x0) + B4 * (P3.y - y0) + C4 * (P3.z - z0);  dis4 = A4 * (P4.x - x0) + B4 * (P4.y - y0) + C4 * (P4.z - z0);
			dis5 = A4 * (P5.x - x0) + B4 * (P5.y - y0) + C4 * (P5.z - z0);  dis6 = A4 * (P6.x - x0) + B4 * (P6.y - y0) + C4 * (P6.z - z0);
			dis7 = A4 * (P7.x - x0) + B4 * (P7.y - y0) + C4 * (P7.z - z0);  dis8 = A4 * (P8.x - x0) + B4 * (P8.y - y0) + C4 * (P8.z - z0);
			if(dis1<=0 && dis2<=0 && dis3<=0 && dis4<=0 && dis5<=0 && dis6<=0 && dis7<=0 && dis8<=0) inner_up = false;
			else inner_up = true;

			inner = (inner_up && inner_down) || (inner_left && inner_right);
			//inner = (inner_up && inner_down) && (inner_left && inner_right);

			if(inner)
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
	double focusDisx,focusDisy,resolution_w,resolution_h,theta, eta;
	vector<string> paras;
	
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

			paras = encodeTool.splitter(temp_ResourceMetas[j].Parameters, ',');
			focusDisx = encodeTool.str2double(paras[0].c_str());
			focusDisy = encodeTool.str2double(paras[1].c_str());
			resolution_w = encodeTool.str2double(paras[7].c_str());
			resolution_h = encodeTool.str2double(paras[8].c_str());
			theta = encodeTool.DegToRad(atan(resolution_h / (2 * focusDisx)));
			eta =   encodeTool.DegToRad(atan(resolution_w / (2 * focusDisy)));
			
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

			if(resultA <= sin(eta) && resultB <= sin(theta) && resultA >=0 && resultB >= 0)
			{
				ResourceMetas.push_back(temp_ResourceMetas[j]);				
			}
		}
	}
	return ResourceMetas;
}



