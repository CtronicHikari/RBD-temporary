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

Eigen::Quaterniond EncodeTool::euler2Quaternion(const double roll, const double pitch, const double yaw)
{
	Eigen::AngleAxisd rollAngle(roll, Eigen::Vector3d::UnitZ());
    	Eigen::AngleAxisd yawAngle(yaw, Eigen::Vector3d::UnitY());
    	Eigen::AngleAxisd pitchAngle(pitch, Eigen::Vector3d::UnitX());
 
    	Eigen::Quaterniond q = rollAngle * yawAngle * pitchAngle;
   	return q;
}

Eigen::Vector3d EncodeTool::Quaterniond2Euler(const double x,const double y,const double z,const double w)
{
	Eigen::Quaterniond q;
    	q.x() = x;
   	q.y() = y;
   	q.z() = z;
   	q.w() = w;
   	Eigen::Vector3d euler = q.toRotationMatrix().eulerAngles(2, 1, 0);
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

Eigen::Quaterniond EncodeTool::rotationMatrix2Quaterniond(Eigen::Matrix3d R)
{
	Eigen::Quaterniond q = Eigen::Quaterniond(R);
  	q.normalize();
  	return q;
}

Eigen::Matrix3d EncodeTool::euler2RotationMatrix(const double roll, const double pitch, const double yaw)
{
	Eigen::AngleAxisd rollAngle(roll, Eigen::Vector3d::UnitZ());
    	Eigen::AngleAxisd yawAngle(yaw, Eigen::Vector3d::UnitY());
   	Eigen::AngleAxisd pitchAngle(pitch, Eigen::Vector3d::UnitX());
   	Eigen::Quaterniond q = rollAngle * yawAngle * pitchAngle;
   	Eigen::Matrix3d R = q.matrix();
   	return R;

}

Eigen::Vector3d EncodeTool::RotationMatrix2euler(Eigen::Matrix3d R)
{
	 Eigen::Matrix3d m;
    	m = R;
    	Eigen::Vector3d euler = m.eulerAngles(0, 1, 2);
    	return euler;
}

void EncodeTool::toEulerAngle(double x,double y,double z,double w, double& roll, double& pitch, double& yaw)
{
// roll (x-axis rotation)
    double sinr_cosp = +2.0 * (w * x + y * z);
    double cosr_cosp = +1.0 - 2.0 * (x * x + y * y);
    roll = atan2(sinr_cosp, cosr_cosp);

// pitch (y-axis rotation)
    double sinp = +2.0 * (w * y - z * x);
    if (fabs(sinp) >= 1)
        pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        pitch = asin(sinp);

// yaw (z-axis rotation)
    double siny_cosp = +2.0 * (w * z + x * y);
    double cosy_cosp = +1.0 - 2.0 * (y * y + z * z);
    yaw = atan2(siny_cosp, cosy_cosp);
//    return yaw;
}

