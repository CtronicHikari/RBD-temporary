#include "EncodeTool.h"

string EncodeTool::string_replace(string & strPath, const string & str1, const string & str)
{
	string::size_type pos = 0;
	string::size_type a = str1.size();
	string::size_type b = str.size();
	while ((pos = strPath.find(str1, pos)) != string::npos) {
		strPath.replace(pos, a, str);
		pos += b;
	}
	return strPath;
}

string EncodeTool::floatTranStr(double flo)
{
	stringstream ss;
	ss << flo;
	return ss.str();
}

double EncodeTool::random(double start,double end)
{
	return start + (end - start) * rand() / (RAND_MAX + 1.0);
}

string EncodeTool::randomStringByTime()
{
	int t;
	string temp;
	srand(time(0));
	t = rand()%9999999999+1;
	temp = to_string(t);
	return temp;
}

string EncodeTool::doubleToString(double dou)
{
	string buffer;
	char r[256];
	string s;
	sprintf(r,"%.3f",dou);
	s = r;
	return s;
}

template <class T>
void convertFromString(T &value, const std::string &s)
{
	std::stringstream ss(s);
	ss >> value;
}

double EncodeTool::stol(const char *str)
{
	double sum=0,fac=10;
   	int negflag=0;
    	if(*str)
   	{
       		if(*str=='-')
        	{
            		negflag=1;
            		str++;
        	}
        	while(*str!='.' && *str)
        	{
         		sum=sum*10+int(*str)-48;
            		str++;
        	}
        	str++;
        	while(*str)
        	{
            		sum=sum+(int(*str)-48)/fac;
            		fac=fac*10;
            		str++;
        	}
        	if(negflag==1)
            		sum=0-sum;
        	printf("%18.15lf",sum);
        	return sum;
    	}
    	else
    	{
        	printf("warning:字符串为空");
        	return 0;
    	}
}

double EncodeTool::str2double(const char *src) 
{ 
    double ret = 0, sign = 1;
    char *p = (char *)src; 
    //处理符号位 
    if (*p == '+')
    {
        sign = 1;
        p++;
    }
    else if (*p == '-')
    {
        sign = -1;
        p++;
    }
    while (*p && (*p != '.'))//处理整数部分 
    {
        ret *= 10;
        ret += (*p) - '0';//将字符转为数字并累计 
        p++;//移动指针 
    }
    if (*p == '.')//存在小数的情况 
    {
        double step = 0.1; //用来标识位数 
        p++; //略过小数点    
        while (*p)//处理小数部分，如果没有E就直接到结束
        {
            double step2 = 1;
            if (*p == 'E'||*p == 'e')//比如说323.443E-03；
            {
                p++;//跳过E或者e;           
                if (*p == '+')
                {
                    p++;//跳过+
                    int temp = atoi(p);
                    while (temp--)
                    {
                        step2 *= 10;
                    }               
                }
                else if (*p == '-')
                {
                    p++;//跳过-
                    int temp = atoi(p);
                    while (temp--)
                    {
                        step2 /= 10;
                    }               
                }
                ret *= step2;
                break;
            }
            ret += step*((*p) - '0');
            step /= 10;
            p++;
        }
    }
    return ret*sign; //不要忘记符号位 
}


double EncodeTool::convertStringToDouble(string &s)
{
	double val;
	std::stringstream ss;
	ss << s;
	ss >> val;
	return val;
}

std::vector<string> EncodeTool::ParametersSplitter(string Parameters, char delimeter)
{
	std::stringstream ss(Parameters);
    	std::string item;
   	std::vector<std::string> splittedStrings;
   	while (std::getline(ss, item, delimeter))
    	{
       		splittedStrings.push_back(item);
    	}
    	return splittedStrings;
}

std::vector<string> EncodeTool::splitter(string target, char delimeter)
{
	std::vector<string> temp;
	int n = target.size();
	for(int i=0;i<n;++i)
	{
		if(target[i] == delimeter)
		{
			target[i] = ' ';
		}
	}
	istringstream out(target);
	string str;
	while(out >> str)
	{
		temp.push_back(str);
	}
	return temp;
}

Eigen::Matrix3d EncodeTool::Quaternion2RotationMatrix(const double x,const double y,const double z,const double w)
{
	Eigen::Quaterniond q;
   	q.x() = x;
   	q.y() = y;
   	q.z() = z;
   	q.w() = w;
  	Eigen::Matrix3d R = q.normalized().toRotationMatrix();
  	return R;
}


uint32_t* EncodeTool::fill(string str)
{
	uint32_t groupCount = ((str.length() + 8) / 64) + 1;      
	strlength = groupCount * step;
	uint32_t *strByte = new uint32_t[strlength];
	for (uint32_t i = 0; i < strlength; i++)
		strByte[i] = 0;
	for (uint32_t i = 0; i <str.length(); i++)
	{
		strByte[i >> 2] |= (str[i]) << ((i % 4) * 8);
	}
	strByte[str.length() >> 2] |= 0x80 << (((str.length() % 4)) * 8);
	size_t length = str.length() * 8;
	memmove(&strByte[strlength - 2], &length, sizeof(size_t));
	return strByte;
}

void EncodeTool::mainLoop(uint32_t M[])
{

	uint32_t f, g;
 	uint32_t a = atemp;
	uint32_t b = btemp;
	uint32_t c = ctemp;
	uint32_t d = dtemp;
	for (uint32_t i = 0; i < 64; i++)
	{
		if (i<16) {
			f = F(b, c, d);
			g = i;
		}
		else if (i<32)
		{
			f = G(b, c, d);
			g = (5 * i + 1) % 16;
		}
		else if (i<48) {
			f = H(b, c, d);
			g = (3 * i + 5) % 16;
		}
		else {
			f = I(b, c, d);
			g = (7 * i) % 16;
		}
		uint32_t tmp = d;
		d = c;
		c = b;
		b = b + shift((a + f + k[i] + M[g]), s[i]);
		a = tmp;
	}
	atemp = a + atemp;
	btemp = b + btemp;
	ctemp = c + ctemp;
	dtemp = d + dtemp;
}

string EncodeTool::changeHex(int a)
{
	const char str16[] = "0123456789abcdef";
	int b;
	string str1;
	string str = "";
	for (int i = 0; i<4; i++)
	{
		str1 = "";
		b = ((a >> i * 8) % (1 << 8)) & 0xff;
		for (int j = 0; j < 2; j++)
		{
			str1.insert(0, 1, str16[b % 16]);
			b = b / 16;
		}
		str += str1;
	}
	return str;
}

string EncodeTool::getMD5(string source)
{
	atemp = A;
	btemp = B;
	ctemp = C;
	dtemp = D;
	uint32_t *strByte = fill(source);
	for (uint32_t i = 0; i<strlength / step; i++)
	{
		uint32_t num[step];
		for (uint32_t j = 0; j<step; j++)
			num[j] = strByte[i * step + j];
 
		mainLoop(num);
	}
	return changeHex(atemp).append(changeHex(btemp)).append(changeHex(ctemp)).append(changeHex(dtemp));
}

unsigned EncodeTool::string_to_unsigned(char *s)
{
	unsigned result = 0;
	for (int i = strlen(s) - 1; i >= 0; --i){
		unsigned temp = 0;
		int k = strlen(s) - i - 1;
		if (isdigit(s[i])){
			temp = s[i] - '0';
			while (k--){
				temp *= 10;
			}
			result += temp;
		}
		else{
			break;
		}
	}
	return result;
}

Eigen::AngleAxisd EncodeTool::rv_init(double radian, Eigen::Vector3d AxisOfRotation)
{
	Eigen::AngleAxisd rv(radian, AxisOfRotation);
	return rv;	
}
Eigen::Matrix3d EncodeTool::rv2Maxtrix(Eigen::AngleAxisd rv)
{
	Eigen::Matrix3d rotation_matrix = Eigen::Matrix3d::Identity();
	rotation_matrix = rv.matrix();
	return rotation_matrix;
}
Eigen::Vector3d EncodeTool::rv2Euler(Eigen::AngleAxisd rv)
{
	Eigen::Vector3d eulerAngle = rv.matrix().eulerAngles(2,1,0);
	return eulerAngle;
}

Eigen::Quaterniond EncodeTool::rv2Quaternion(Eigen::AngleAxisd rv)
{
	Eigen::Quaterniond quaternion(rv);
}

Eigen::Vector3d EncodeTool::rm2Euler(Eigen::Matrix3d rm)
{
	Eigen::Vector3d euler = rm.eulerAngles(2,1,0);
	return euler;
}
Eigen::AngleAxisd EncodeTool::rm2RotationVector(Eigen::Matrix3d rm)
{
	Eigen::AngleAxisd rv;
	rv.fromRotationMatrix(rm);
	return rv;
}
Eigen::Quaterniond EncodeTool::rm2Quaternion(Eigen::Matrix3d rm)
{
	Eigen::Quaterniond q(rm);
	return q;
}

Eigen::Matrix3d EncodeTool::ea2Matrix(Eigen::Vector3d ea)
{
	Eigen::Matrix3d rm;
	rm = Eigen::AngleAxisd(ea[0], Eigen::Vector3d::UnitZ()) * 
             Eigen::AngleAxisd(ea[1], Eigen::Vector3d::UnitY()) * 
             Eigen::AngleAxisd(ea[2], Eigen::Vector3d::UnitX());
	return rm;
}
Eigen::Quaterniond EncodeTool::ea2Quaternion(Eigen::Vector3d ea)
{
	Eigen::Quaterniond q;
	q = Eigen::AngleAxisd(ea[0], Eigen::Vector3d::UnitZ()) * 
            Eigen::AngleAxisd(ea[1], Eigen::Vector3d::UnitY()) * 
            Eigen::AngleAxisd(ea[2], Eigen::Vector3d::UnitX());
	return q;
}
Eigen::AngleAxisd EncodeTool::ea2RotationVector(Eigen::Vector3d ea)
{
	Eigen::AngleAxisd rv;
	rv = Eigen::AngleAxisd(ea[0], Eigen::Vector3d::UnitZ()) * 
             Eigen::AngleAxisd(ea[1], Eigen::Vector3d::UnitY()) * 
             Eigen::AngleAxisd(ea[2], Eigen::Vector3d::UnitX());
	return rv;
}

Eigen::AngleAxisd EncodeTool::q2RotationVector(Eigen::Quaterniond q)
{
	Eigen::AngleAxisd rv(q);
	return rv;
}
Eigen::Matrix3d EncodeTool::q2Matrix(Eigen::Quaterniond q)
{
	Eigen::Matrix3d matrix;
	matrix = q.matrix();
	return matrix;
}
Eigen::Vector3d EncodeTool::q2Euler(Eigen::Quaterniond q)
{
	Eigen::Vector3d euler = q.matrix().eulerAngles(2,1,0);
	return euler;
}

Eigen::Quaterniond EncodeTool::Blend2Rotations(Eigen::Quaterniond quaternion1, Eigen::Quaterniond quaternion2)
{
	Eigen::Quaterniond logQ1 = lnQuaternion(quaternion1);
	Eigen::Quaterniond logQ2 = lnQuaternion(quaternion2);
	Eigen::Quaterniond add = addQuaternion(logQ1,logQ2);
	Eigen::Quaterniond blend = expQuaternion(add);

	return blend;
}

Eigen::Quaterniond EncodeTool::lnQuaternion(Eigen::Quaterniond quaternion)
{
	//ln(p) = ln(|p|) + Sgn(u)*arg(p)  u:vector
	double modulusOfP = sqrt(quaternion.x()*quaternion.x() + quaternion.y()*quaternion.y() + quaternion.z()*quaternion.z() + quaternion.w()*quaternion.w());
	double w = log(modulusOfP); //ln(|p|)

	double modulusOfU = sqrt(quaternion.x()*quaternion.x() + quaternion.y()*quaternion.y() + quaternion.z()*quaternion.z());
	double SgnUx = quaternion.x()/modulusOfU;  //Sgn(u)
	double SgnUy = quaternion.y()/modulusOfU;
	double SgnUz = quaternion.z()/modulusOfU;

	double argP = acos(quaternion.w()/modulusOfP);  //arg(p)

	Eigen::Quaterniond lnQ(argP*SgnUx, argP*SgnUy, argP*SgnUz, w);
	return lnQ;
}

Eigen::Quaterniond EncodeTool::addQuaternion(Eigen::Quaterniond quaternion1, Eigen::Quaterniond quaternion2)
{

	double x = quaternion1.x() + quaternion2.x();
	double y = quaternion1.y() + quaternion2.y();
	double z = quaternion1.z() + quaternion2.z();
	double w = quaternion1.w() + quaternion2.w();

	Eigen::Quaterniond addQ(x,y,z,w);
	return addQ;
}

Eigen::Quaterniond EncodeTool::expQuaternion(Eigen::Quaterniond quaternion)
{
	//exp(p) = exp(w) + (cos(|u|)+Sgn(u)sin(|u|))
	double expW = exp(quaternion.w());

	double modulusU = sqrt(quaternion.x()*quaternion.x() + quaternion.y()*quaternion.y() + quaternion.z()*quaternion.z());
	double cosU = cos(modulusU);
	double sinU = sin(modulusU);

	double w = expW * cosU;
	double x = quaternion.x()/modulusU*expW*sinU;
	double y = quaternion.y()/modulusU*expW*sinU;
	double z = quaternion.z()/modulusU*expW*sinU;

	Eigen::Quaterniond expQ(x,y,z,w);
	return expQ;
}



















