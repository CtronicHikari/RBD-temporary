#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <iostream>
#include <MyClass.h>
#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OpenCV.h"

/*class Transform {
public:

    Transform() {

    };

    Transform(const double WorldPositionX, const double WorldPositionY, const double WorldPositionZ,
        const double Time, const double AttitudeX, const double AttitudeY, const double AttitudeZ, const double AttitudeW) :
        WorldPositionX(WorldPositionX), WorldPositionY(WorldPositionY), WorldPositionZ(WorldPositionZ),
        Time(Time), AttitudeX(AttitudeX), AttitudeY(AttitudeY), AttitudeZ(AttitudeZ), AttitudeW(AttitudeW){

    };

    ~Transform() {

    };

    double WorldPositionX;
    double WorldPositionY;
    double WorldPositionZ;
    double Time;
    double AttitudeX;
    double AttitudeY;
    double AttitudeZ;
    double AttitudeW;
};*/
/*class Transform {
public:
	double Posx;
	double Posy;
	double Posz;
	double Attix;
	double Attiy;
	double Attiz;
	double Attiw;
	unsigned int Time_s;
	unsigned int Time_ms;

	Transform() {
		Posx = 0.0f;
		Posy = 0.0f;
		Posz = 0.0f;
		Attix = 0.0f;
		Attiy = 0.0f;
		Attiz = 0.0f;
		Attiw = 0.0f;	
		Time_s = 0;
		Time_ms = 0;
	};

    	Transform(const double Posx,  const double Posy,  const double Posz,
                  const double Attix, const double Attiy, const double Attiz, const double Attiw,
	          const unsigned int Time_s, const unsigned int Time_ms) :
                  Posx(Posx), Posy(Posy), Posz(Posz),
                  Attix(Attix), Attiy(Attiy), Attiz(Attiz), Attiw(Attiw),
		  Time_s(Time_s), Time_ms(Time_ms){};

	~Transform() {};
};*/


/*class ResourceMeta {

public:

    ResourceMeta() {

    };

    ResourceMeta(const std::string &ID, const int ParentObjectID, const int Type,
        const std::string Ext, const Transform &transform, const std::string Path, const std::string Paraments) :
        ID(ID), ParentObjectID(ParentObjectID), Type(Type),
        Ext(Ext), transform(transform), Path(Path), Paraments(Paraments){

    };

    ~ResourceMeta() {

    }

	std::string ID;
    int ParentObjectID;
    int Type;
    std::string Ext;
    Transform transform;
    std::string Path;
    std::string Paraments;
};*/
/*class ResourceMeta {

public:
	string ID;
    	int ParentObjectID;
    	int Type;
   	string Ext;
   	sigma::Transform transform;
   	string Path;
    	string Parameters;

   	ResourceMeta() {
		ID = "";
		ParentObjectID = 0;
		Type = 0;
		Ext = "";
		transform = {};
		Path = "";
		Parameters = ""; 
	};

  	ResourceMeta(const string &ID, const int ParentObjectID, const int Type,
        	     const string Ext, const sigma::Transform &transform, const string Path, const string Parameters) :
       		     ID(ID), ParentObjectID(ParentObjectID), Type(Type),
       		     Ext(Ext), transform(transform), Path(Path), Parameters(Parameters){};

   	 ~ResourceMeta() {}
};*/

class intrinsicPara {

public:
	intrinsicPara() :
        focal(0.f), ppx(0.f), ppy(0.f), k1(0.f), k2(0.f), k3(0.f), width(0), height(0) {
	};

	intrinsicPara(const double focal, const double ppx, const double ppy,
		const double k1, const double k2, const double k3, const int width, const int height) :
		focal(focal), ppx(ppx), ppy(ppy), k1(k1), k2(k2), k3(k3), width(width), height(height){
	};

	~intrinsicPara() {
	};

	//�œ_����
	double focal;

	//��_
	double ppx;
	double ppy;

	//�c�݌W��
	double k1;
	double k2;
	double k3;

    	//�T�C�Y
    	int width;
    	int height;
};

class extrinsicPara {

public:
    extrinsicPara() {

        r[0] = r[1] = r[2] = r[3] = r[4] = r[5] = r[6] = r[7] = r[8] = 0.f;
        t[0] = t[1] = t[2] = 0.f;
    };

    ~extrinsicPara() {
    };

    //��]
    double r[9];

    //���i
    double t[3];
};

class pairIE {

public:
    pairIE() : i_id(0), e_id(0){

    };

    ~pairIE() {
    };

    //����ID
    double i_id;

    //�O��ID
    double e_id;

    //�t�@�C���p�X
    std::string path;
};

/**
 * @fn
 * @brief RDB����_�E�����[�h�������\�[�X�̃��^����xml�t�@�C���ɕۑ�
 * @param const std::vector<ResourceMeta> resourcemeta �ۑ�����vector
 * @param const std::string path �ۑ�����f�B���N�g���p�X
 */
extern bool xmlFileGenerator(const std::vector<sigma::ResourceMeta> resourcemetas, const std::string path);

/**
 * @fn
 * @brief RDB����_�E�����[�h�������\�[�X�̃��^����xml�t�@�C���ɕۑ�
 * @std::vector<ResourceMeta> & resourcemetametas xml�t�@�C����ǂ݁Cresourcemetametas��vector�Ɋi�[
 * @param const std::string path �ۑ����Ă���f�B���N�g���p�X
 */
extern void xmlFileParser(std::vector<sigma::ResourceMeta>& resourcemetas, const std::string path);

/**
 * @fn
 * @brief OpenMVG�ŏo�͂���xml�t�@�C�������
 * @std::vector<pairIE>& pairie xml�t�@�C����ǂ݁CpairIE��vector�Ɋi�[
 * @std::vector<intrinsicPara>& intrinsicpara xml�t�@�C����ǂ݁CintrinsicPara��vector�Ɋi�[
 * @std::vector<extrinsicPara>& extrinsicpara xml�t�@�C����ǂ݁CextrinsicPara��vector�Ɋi�[
 * @param const std::string path �ۑ����Ă���f�B���N�g���p�X
 */
extern void parameterFileParser(std::vector<pairIE>& pairie, std::vector<intrinsicPara>& intrinsicpara, std::vector<extrinsicPara>& extrinsicpara, const std::string path);

/**
 * @fn
 * @brief �J�����̓����p�����[�^���當������쐬
 * @intrinsicPara _intrinsicpara intrinsicPara
 * @return ������
 */
extern std::string strGenerator(intrinsicPara &_intrinsicpara);

/**
 * @fn
 * @brief strGenerator�ō쐬��������������
 * @intrinsicPara _intrinsicpara intrinsicPara
 * @std::string str �쐬����������
 */
extern void strParser(intrinsicPara& _intrinsicpara, std::string str);

/**
 * @fn
 * @brief �z��������glm�`���̃N�H�[�^�j�I�����Ԃ��ė���
 * @double* m �z��(3�s3��)
 */
extern glm::quat convertM2Q(double* m);

/**
 * @fn
 * @brief �z��������glm�`���̃N�H�[�^�j�I�����Ԃ��ė���
 * @glm::quat q�@glm�`���̃N�H�[�^�j�I�� 
 * @double* m �z��(3�s3��)
 */
extern void convertQ2M(glm::quat q, double* m);
