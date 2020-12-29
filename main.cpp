#include <iostream>
#include <stdio.h>
#include <time.h>
#include "Common.h"
#include "Connect.h"
#include "EncodeTool.h"
#include "MyClass.h"
using namespace std;

int main() {
	clock_t start,end;
	DBConnect dbConnect;  
	dbConnect.Connect(); 

	

	//------Coordinate conversion test--------
	
	//sigma::sigmaCorr tmp;
	//dbConnect.encodeTool.LatLonToSIGMA(dbConnect.encodeTool.DegToRad(32), dbConnect.encodeTool.DegToRad(143.9999999999999), 7.0, new sigma::Time(1606005570,500), tmp);
	//cout << "Zone:" << tmp.number << tmp.symbol << endl;
	//printf("X:%f    Y:%f    Z:%f\n",tmp.position.Posx,tmp.position.Posy,tmp.position.Posz);	
	//printf("second:%u  tiny second:%u\n",tmp.time.s,tmp.time.ns);
	
	//----------------------------------------
	//cout << dbConnect.encodeTool.Geohash_space(tmp, 4) << endl;
	//-----------------------------------------------------------------------------------------------------------------------------------------
	//Resource Registration
	/*
	start = clock();
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
	end = clock();
	cout << double(end - start) / CLOCKS_PER_SEC << "s" << endl;
	/*
	//for (int i = 0; i < pairie.size(); ++i) {
	for (int i = 0; i < 1; ++i) {

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

		sigma::ResourceMeta r(str, i, 1, 1, t, path, para);

		resourcemetas[i] = r;

		////////////////////////////////////
		sigma::Position *pos_phys = new sigma::Position(9.99f,9.99f,9.99f);
		sigma::Position *pos_est = new sigma::Position(t.pos.Posx,t.pos.Posy,t.pos.Posz);
		sigma::Attitude *atti_phys = new sigma::Attitude(9.99f,9.99f,9.99f,9.99f);
		sigma::Attitude *atti_est = new sigma::Attitude(t.atti.Attitudex,t.atti.Attitudey,t.atti.Attitudez,t.atti.Attitudew);
		sigma::Time *time = new sigma::Time(t.time.s,t.time.ns);
		//string GeometryID = dbConnect.addGeometry(pos_est,atti_est,pos_phys,atti_phys,time);
		//string ResourceID = dbConnect.make_ResourceID(r.ParentObjectID, pos_est, atti_est, time); 
		//dbConnect.addResource(ResourceID, r.ParentObjectID, r.Type, r.Type, r.Parameters, path);
		//dbConnect.link_Geo_and_Res(ResourceID, GeometryID);
		cv::Mat img =  cv::imread(pairie[i].path);
		string filename = path + ResourceID + ".png";
		cv::imwrite(filename, img);
		/////////////////////////////////////////////////////////////////
	}
	//End Resource Registration
	*/

	//Posture Search
	
	sigma::Vector4 *Origin = new sigma::Vector4(0.0f,0.0f,0.0f,0, 0);
	sigma::Vector4 *Range = new sigma::Vector4(-5.0f,-5.0f,-5.0f,1200, 0);
	sigma::Target *target = new sigma::Target(0.23f,-0.025f,0.85f);
	std::vector<sigma::ResourceMeta> TestResourceMetaResults;
	TestResourceMetaResults = dbConnect.loadResourceMeta(Origin, Range, target, 0.0f, 0.0f, 2);
	cout << TestResourceMetaResults.size() << endl;
	xmlFileGenerator(TestResourceMetaResults, "data/Posture");
	for(int i=0;i<TestResourceMetaResults.size();i++)
	{
		string tmp = "./data/" + TestResourceMetaResults[i].ID + ".png";
		string tmp1 = "./data/Posture/" + TestResourceMetaResults[i].ID + ".png";
		cv::Mat img =  cv::imread(tmp);
		cv::imwrite(tmp1, img);
	}
	
	//End Posture Search
	
	//Bouding Box Search
	/*
	//sigma::Vector4 *Origin = new sigma::Vector4(0.0f,-0.025f,0.85f,0, 0);
	sigma::Vector4 *Origin = new sigma::Vector4(0.0f,0.0f,0.0f,0, 0);
	sigma::Vector4 *Range = new sigma::Vector4(-1.0f,-1.0f,-1.0f,12000, 0);
	//sigma::BoudingBox *box = new sigma::BoudingBox(3.0f, 2.1f, 1.1f, 1.0f, 0.8f, -1.0f);
	sigma::BoudingBox *box = new sigma::BoudingBox(0.38f, -0.25f, 1.0f, 0.08f, 0.2f, 0.7f);
	std::vector<sigma::ResourceMeta> TestResourceMetaResults;
	TestResourceMetaResults = dbConnect.loadResourceMeta(Origin, Range, box, 2);
	//TestResourceMetaResults = dbConnect.loadResourceMeta(Origin, Range, 2);
	cout << TestResourceMetaResults.size() << endl;
	xmlFileGenerator(TestResourceMetaResults, "data/BoudingBox");
	for(int i=0;i<TestResourceMetaResults.size();i++)
	//for(int i=0;i<1;i++)
	{
		string tmp = "./data/" + TestResourceMetaResults[i].ID + ".png";
		string tmp1 = "./data/BoudingBox/" + TestResourceMetaResults[i].ID + ".png";
		cv::Mat img =  cv::imread(tmp);
		cv::imwrite(tmp1, img);
	}
	*/
	
	//End Posture Search


	//xml Test
	
	//xmlFileGenerator(TestResourceMetaResults, path);
	
	//xmlFileGenerator(resourcemetas2, path);

	//std::vector<ResourceMeta> resourcemetas2;
	//撉傒崬傒
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
	//End xml Test
	
	//-----------------------------------------------------------------------------------------------------------------------------------------
	

	dbConnect.Disconnect();
	return 0;
}
