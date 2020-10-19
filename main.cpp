#include <iostream>
#include "Common.h"
#include "Connect.h"
#include "EncodeTool.h"
#include "MyClass.h"
using namespace std;

int main() {
	DBConnect dbConnect;  
	dbConnect.Connect(); 

	//sigma::Object* obj = new sigma::Object(1,0);



	
	//std::vector<sigma::Geometry> Geometries;
	//std::vector<sigma::Object> Objects;
	//sigma::Vector4 *Origin = new sigma::Vector4(0.0f,0.0f,0.0f,0,0);
	//sigma::Vector4 *Range = new sigma::Vector4(10.0f,10.0f,10.0f,2,2);

	//dbConnect.addObject(obj, GeometryID);
	//GeometryID = dbConnect.addGeometry(NULL,NULL,pos_phys2,atti_phys2,time);
	//GeometryID = dbConnect.addGeometry(pos_est1,atti_est1,pos_phys1,atti_phys1,time);
	//ResourceID = dbConnect.addResource(1,1,"dummy-Paraments","dummy-GeometryID","/load_path_A/load_path_A/abc.jpg","/save_path_A/save_path_B");
	//dbConnect.link_Geo_and_Obj(2,GeometryID);
	//dbConnect.link_Geo_and_Res(ResourceID,GeometryID);
	//Geometries = dbConnect.loadGeometry(Origin,Range,3);
	//cout << Geometries.size() << endl;
	//Objects = dbConnect.loadObject(Origin,Range,1);
	//cout << Objects.size() << endl;
	/*
	for(int i=0;i<Geometries.size();i++)
	{
		cout << Geometries[i].ID << " ";
		cout << Geometries[i].Posx_est << " ";
		cout << Geometries[i].Posy_est << " ";
		cout << Geometries[i].Posz_est << " ";
		cout << Geometries[i].Attix_est << " ";
		cout << Geometries[i].Attiy_est << " ";
		cout << Geometries[i].Attiz_est << " ";
		cout << Geometries[i].Attiw_est << " ";
		cout << Geometries[i].Posx_phys << " ";
		cout << Geometries[i].Posy_phys << " ";
		cout << Geometries[i].Posz_phys << " ";
		cout << Geometries[i].Attix_phys << " ";
		cout << Geometries[i].Attiy_phys << " ";
		cout << Geometries[i].Attiz_phys << " ";
		cout << Geometries[i].Attiw_phys << " ";
		cout << Geometries[i].Time_s << " ";
		cout << Geometries[i].Time_ms << endl;
		cout << endl;
	}
	*/

	//-----------------------------------------------------------------------------------------------------------------------------------------
/*	
	std::string path = "data/";

	const boost::filesystem::path path2(path);

	if (boost::filesystem::exists(path2)) {
		std::cout << "already exists " << path2 << std::endl;
	}
	else {
		boost::system::error_code error;
		const bool result = boost::filesystem::create_directory(path2, error);
		if (!result || error) {
			std::cout << "failed to make directory" << std::endl;
		}
	}

	std::vector<intrinsicPara> intrinsicpara;
	std::vector<extrinsicPara> extrinsicpara;
	std::vector<pairIE> pairie;
	parameterFileParser(pairie, intrinsicpara, extrinsicpara, "sfm-data.xml");

	std::vector<sigma::ResourceMeta> resourcemetas(pairie.size());

	for (int i = 0; i < pairie.size(); ++i) {
	//for (int i = 0; i < 1; ++i) {

		const boost::filesystem::path src(pairie[i].path);  //sfm-data.xml:root_path+filename.png

		//std::string img_path(path + src.filename().string());  //save path  data/filename.png

		glm::quat quaternion = convertM2Q(extrinsicpara[pairie[i].e_id].r);

		sigma::Transform t(extrinsicpara[pairie[i].e_id].t[0], extrinsicpara[pairie[i].e_id].t[1], extrinsicpara[pairie[i].e_id].t[2],
			(double)quaternion.x, (double)quaternion.y, (double)quaternion.z, (double)quaternion.w,
			(unsigned int)i, (unsigned int)i+1);

		const std::string str = std::to_string(i);

		boost::filesystem::path extension = src.extension();
		vector<string> ext = dbConnect.encodeTool.ParametersSplitter(extension.generic_string(),'.');

		std::string para = strGeneratorCSV(intrinsicpara[pairie[i].i_id]);

		sigma::ResourceMeta r(str, i, 1, ext[1], t, path, para);

		//cv::Mat img(intrinsicpara[pairie[i].i_id].height, intrinsicpara[pairie[i].i_id].width, CV_8UC3, cv::Scalar(i * 2, 0, 0));
		//cv::imwrite(r.Path, img);
		resourcemetas[i] = r;

		////////////////////////////////////
		sigma::Position *pos_phys = new sigma::Position(9.99f,9.99f,9.99f);
		sigma::Position *pos_est = new sigma::Position(t.Posx,t.Posy,t.Posz);
		sigma::Attitude *atti_phys = new sigma::Attitude(9.99f,9.99f,9.99f,9.99f);
		sigma::Attitude *atti_est = new sigma::Attitude(t.Attix,t.Attiy,t.Attiz,t.Attiw);
		sigma::Time *time = new sigma::Time(t.Time_s,t.Time_ms);
		string GeometryID = dbConnect.addGeometry(pos_est,atti_est,pos_phys,atti_phys,time);
		vector<string> filename = dbConnect.encodeTool.ParametersSplitter(pairie[i].path,'/');
		string ResourceID = dbConnect.addResource(r.ParentObjectID, r.Type, r.Parameters, GeometryID, filename[filename.size()-1], path);
		dbConnect.link_Geo_and_Res(ResourceID, GeometryID);
		cv::Mat img =  cv::imread(pairie[i].path);
		cv::imwrite(path + ResourceID, img);

		////Prepare WorldPosition & Attitude information
		//encodeTool.Q = encodeTool.euler2Quaternion(45.0, 45.0, 45.0);
		//attitude.setAttitude(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
		//worldposition.setWorldPostion(extrinsicpara[pairie[i].e_id].t[0], extrinsicpara[pairie[i].e_id].t[1], extrinsicpara[pairie[i].e_id].t[2]);
		//attitude.setAttitude(t.AttitudeX, t.AttitudeY, t.AttitudeZ, t.AttitudeW);
		//worldposition.setWorldPostion(t.WorldPositionX, t.WorldPositionY, t.WorldPositionZ);

		////addGeomtry() ->  addObject()  ->  addResource()
		//GeometryID = dbConnect.addGeometry(worldposition,t.Time);
		//dbConnect.addResource(GeometryID,resourcemetas[i].ParentObjectID,1, attitude, resourcemetas[i].Path, resourcemetas[i].Parameters);

		
		/////////////////////////////////////////////////////////////////
	}*/

	/*
	Vector4 Origin,Range;
	Origin.setVector4(0.0f,0.0f,0.0f,0);
	Range.setVector4(5.0f,5.0f,5.0f,123);
	Target target;
	target.setTarget(1.6f,1.5f,0.0f);
	std::vector<ResourceMeta> TestResourceMetaResults;
	TestResourceMetaResults = dbConnect.loadResourceMeta(Origin, Range, target, M_PI/180.0f*10.0f, M_PI/180.0f*30.0f);
	cout << TestResourceMetaResults.size() << endl;

	dbConnect.Disconnect(); //RDB Disconnect
	//èëÇ´èoÇµ
	xmlFileGenerator(TestResourceMetaResults, path);
	*/
	//xmlFileGenerator(resourcemetas2, path);

	//std::vector<ResourceMeta> resourcemetas2;

	//ì«Ç›çûÇ›
	//xmlFileParser(resourcemetas2, path);
	/*
	std::cout << "----Load xml----" << std::endl;
	for (auto v : resourcemetas2) {
		std::cout << "ID:" << v.ID << std::endl;
		std::cout << "ParentObjectID:" << v.ParentObjectID << std::endl;
		std::cout << "Type:" << v.Type << std::endl;
		std::cout << "Path:" << v.Path << std::endl;
		std::cout << "Paraments:" << v.Parameters << std::endl;
		std::cout << "transform:" << std::endl;
		std::cout << "  WorldPositionX:" << v.transform.WorldPositionX << std::endl;
		std::cout << "  WorldPositionY:" << v.transform.WorldPositionY << std::endl;
		std::cout << "  WorldPositionZ:" << v.transform.WorldPositionZ << std::endl;
		std::cout << "  Time:" << v.transform.Time << std::endl;
		std::cout << "  AttitudeX:" << v.transform.AttitudeX << std::endl;
		std::cout << "  AttitudeY:" << v.transform.AttitudeY << std::endl;
		std::cout << "  AttitudeZ:" << v.transform.AttitudeZ << std::endl;
		std::cout << "  AttitudeW:" << v.transform.AttitudeW << std::endl;
	}*/
	


	//-----------------------------------------------------------------------------------------------------------------------------------------
	

	dbConnect.Disconnect();
	return 0;
}
