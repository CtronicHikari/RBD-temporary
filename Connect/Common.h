#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OpenCV.h"
#include "MyClass.h"

#define MAX_NUM 8.988465674311579538647e+307

//TODO class Transform

class intrinsicPara {

public:
	intrinsicPara() :
        focalx(0.f), focaly(0.f), ppx(0.f), ppy(0.f), k1(0.f), k2(0.f), k3(0.f), width(0), height(0) {
	};

	intrinsicPara(const double focalx, const double focaly, const double ppx, const double ppy,
		const double k1, const double k2, const double k3, const int width, const int height) :
        focalx(focalx), focaly(focaly), ppx(ppx), ppy(ppy), k1(k1), k2(k2), k3(k3), width(width), height(height){
	};

	~intrinsicPara() {
	};

	//������x
    	double focalx;
    	double focaly;

	//����
	double ppx;
	double ppy;

	//��߂S��
	double k1;
	double k2;
	double k3;

    	//������
    	int width;
    	int height;
};

class extrinsicPara {

public:
    extrinsicPara() {

        r[0] = r[1] = r[2] = r[3] = r[4] = r[5] = r[6] = r[7] = r[8] = MAX_NUM;
        t[0] = t[1] = t[2] = MAX_NUM;
    };

    ~extrinsicPara() {
    };

    //��ܞ
    double r[9];

    //�K�M
    double t[3];
};

class pairIE {

public:
    pairIE() : i_id(0), e_id(0){

    };

    ~pairIE() {
    };

    //�ڲ�ID
    double i_id;

    //�ⲿID
    double e_id;

    //�ե�����ѥ�
    std::string path;
};

/**
 * @fn
 * @brief RDB����������`�ɤ����꥽�`���Υ᥿����xml�ե�����˱���
 * @param const std::vector<ResourceMeta> resourcemeta ���椹��vector
 * @param const std::string path ���椹��ǥ��쥯�ȥ�ѥ�
 */
extern bool xmlFileGenerator(const std::vector<sigma::ResourceMeta> resourcemetas, const std::string path);

/**
 * @fn
 * @brief RDB����������`�ɤ����꥽�`���Υ᥿����xml�ե�����˱��� ???
 * @std::vector<ResourceMeta> & resourcemetametas xml�ե�������i�ߣ�resourcemetametas��vector�˸�{
 * @param const std::string path ���椷�Ƥ���ǥ��쥯�ȥ�ѥ�
 */
extern void xmlFileParser(std::vector<sigma::ResourceMeta>& resourcemetas, const std::string path);

/**
 * @fn
 * @brief OpenMVG�ǳ�������xml�ե���������
 * @std::vector<pairIE>& pairie xml�ե�������i�ߣ�pairIE��vector�˸�{
 * @std::vector<intrinsicPara>& intrinsicpara xml�ե�������i�ߣ�intrinsicPara��vector�˸�{
 * @std::vector<extrinsicPara>& extrinsicpara xml�ե�������i�ߣ�extrinsicPara��vector�˸�{
 * @param const std::string path ���椷�Ƥ���ǥ��쥯�ȥ�ѥ�
 */
extern void parameterFileParser(std::vector<pairIE>& pairie, std::vector<intrinsicPara>& intrinsicpara, std::vector<extrinsicPara>& extrinsicpara, const std::string path);

/**
 * @fn
 * @brief �������ڲ��ѥ��`�����������Ф�����
 * @intrinsicPara _intrinsicpara intrinsicPara
 * @return ������
 */
extern std::string strGenerator(intrinsicPara& _intrinsicpara);

//strGenerator��csv��
extern std::string strGeneratorCSV(intrinsicPara &_intrinsicpara);

/**
 * @fn
 * @brief strGenerator�����ɤ��������Ф����
 * @intrinsicPara _intrinsicpara intrinsicPara
 * @std::string str ���ɤ���������
 */
extern void strParser(intrinsicPara& _intrinsicpara, std::string str);

//strParser��csv��
extern void strParserCSV(intrinsicPara& _intrinsicpara, std::string str);

/**
 * @fn
 * @brief ���Ф������glm��ʽ�Υ����`���˥��󤬷��ä�����
 * @double* m ����(3��3��)
 */
extern glm::quat convertM2Q(double* m);

/**
 * @fn
 * @brief ���Ф������glm��ʽ�Υ����`���˥��󤬷��ä�����
 * @glm::quat q��glm��ʽ�Υ����`���˥��� 
 * @double* m ����(3��3��)
 */
extern void convertQ2M(glm::quat q, double* m);


