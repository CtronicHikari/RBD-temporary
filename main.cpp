#include <iostream>
#include "Common.h"
#include "Connect.h"
#include "EncodeTool.h"
#include "MyClass.h"

int main() {
	
	std::string path = "data";

	//任意のフォルダを作成
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

	//任意の外部パラメータ.xmlファイルを呼び出す
	std::vector<intrinsicPara> intrinsicpara;
	std::vector<extrinsicPara> extrinsicpara;
	std::vector<pairIE> pairie;
	parameterFileParser(pairie, intrinsicpara, extrinsicpara, "sfm-data.xml");

	//適当なリソースを準備
	std::vector<ResourceMeta> resourcemetas(pairie.size());

	EncodeTool encodeTool;  //support class
	DBConnect dbConnect;  //RDBctrl Instantiate
	dbConnect.Connect();  //RDB connect
	/*
	//cout << __DBL_MAX__ << endl;
	for (int i = 0; i < pairie.size(); ++i) {
	//for (int i = 0; i < 1; ++i) {

		//ロードしたリソース（画像）をmeta.xmlと同じ位置にコピー
		const boost::filesystem::path src(pairie[i].path); // コピー元

		std::string img_path(path + "/" + src.filename().string()); // コピー先

		cv::Mat img =  cv::imread(pairie[i].path);
		cv::imwrite(img_path, img);

		glm::quat quaternion = convertM2Q(extrinsicpara[pairie[i].e_id].r);

		//double m[9];
		//convertQ2M(quaternion, m);
		fv::TransformA t(extrinsicpara[pairie[i].e_id].t[0], extrinsicpara[pairie[i].e_id].t[1], extrinsicpara[pairie[i].e_id].t[2],
			(double)i + 0.4,
			(double)quaternion.x, (double)quaternion.y, (double)quaternion.z, (double)quaternion.w);

		

		const std::string str = std::to_string(i);

		//拡張子
		boost::filesystem::path extension = src.extension();

		//カメラパラメータ
		//std::string para = strGenerator(intrinsicpara[pairie[i].i_id]);
		std::string para = strGeneratorCSV(intrinsicpara[pairie[i].i_id]);

		ResourceMeta r(str, i, 1, extension.generic_string(), t, img_path, para);

		//cv::Mat img(intrinsicpara[pairie[i].i_id].height, intrinsicpara[pairie[i].i_id].width, CV_8UC3, cv::Scalar(i * 2, 0, 0));
		//cv::imwrite(r.Path, img);
		resourcemetas[i] = r;

		////////////////////////////////////
		
		
		////Add
		string GeometryID;
		WorldPosition worldposition;		
		Attitude attitude;

		////Prepare WorldPosition & Attitude information
		//encodeTool.Q = encodeTool.euler2Quaternion(45.0, 45.0, 45.0);
		//attitude.setAttitude(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
		//worldposition.setWorldPostion(extrinsicpara[pairie[i].e_id].t[0], extrinsicpara[pairie[i].e_id].t[1], extrinsicpara[pairie[i].e_id].t[2]);
		attitude.setAttitude(t.AttitudeX, t.AttitudeY, t.AttitudeZ, t.AttitudeW);
		worldposition.setWorldPostion(t.WorldPositionX, t.WorldPositionY, t.WorldPositionZ);

		////Load Image
		//ImageData_new imgData;
		//imgData.ImageData = imread(pairie[i].path);
		//Data_new* edata = &imgData;

		////addGeomtry() ->  addObject()  ->  addResource()
		GeometryID = dbConnect.addGeometry(worldposition,t.Time);
		dbConnect.addResource(GeometryID,resourcemetas[i].ParentObjectID,1, attitude, resourcemetas[i].Path, resourcemetas[i].Parameters);

		
		/////////////////////////////////////////////////////////////////
	}
	*/
	
	Vector4 Origin,Range;
	Origin.setVector4(0.0f,0.0f,0.0f,0);
	Range.setVector4(5.0f,5.0f,5.0f,123);
	Target target;
	target.setTarget(1.6f,1.5f,0.0f);
	std::vector<ResourceMeta> TestResourceMetaResults;
	TestResourceMetaResults = dbConnect.loadResourceMeta(Origin, Range, target, M_PI/180.0f*10.0f, M_PI/180.0f*30.0f);
	cout << TestResourceMetaResults.size() << endl;

	dbConnect.Disconnect(); //RDB Disconnect
	//書き出し
	xmlFileGenerator(TestResourceMetaResults, path);
	//xmlFileGenerator(resourcemetas2, path);

	//std::vector<ResourceMeta> resourcemetas2;

	//読み込み
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
	/*
	DBConnect dbConnect;

	dbConnect.Connect();

	std::vector<ResourceMeta> metas;
	Vector4 Origin,Range;
	Origin.setVector4(0.0f,0.0f,0.0f,0);
	Range.setVector4(5.0f,5.0f,5.0f,123);
	Target target;
	target.setTarget(1.6f,1.5f,0.0f);
	metas = dbConnect.loadResourceMeta(Origin, Range, target, M_PI/180.0f*60.0f);
	cout << metas.size() << endl;
	metas = dbConnect.loadResourceMeta(Origin, Range, target, M_PI/180.0f*10.0f,M_PI/180.0f*20.0f);
	cout << metas.size() << endl;

	//std::vector<float> weights(21,1);
	
	//dbConnect.add_link_geo_obj(10,metas,weights);
	
	dbConnect.Disconnect();
	*/

	return 0;
}
