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

double EncodeTool::ArcLengthOfMeridian(double phi)
{
	
	double alpha, beta, gamma, delta, epsilon, n;
	double result;
	
	n = (sm_a - sm_b) / (sm_a + sm_b);
	alpha = ((sm_a + sm_b) / 2.0) * (1.0 + (pow(n, 2.0) / 4.0) + (pow(n, 4.0) / 64.0));
	beta = (-3.0 * n / 2.0) + (9.0 * pow(n, 3.0) / 16.0) + (-3.0 * pow(n, 5.0) / 32.0);
	gamma = (15.0 * pow(n, 2.0) / 16.0) + (-15.0 * pow(n, 4.0) / 32.0);
	delta = (-35.0 * pow(n, 3.0) / 48.0) + (105.0 * pow(n, 5.0) / 256.0);
	epsilon = (315.0 * pow(n, 4.0) / 512.0);
	result = alpha * (phi + (beta * sin(2.0 * phi)) + (gamma * sin(4.0 * phi)) + (delta * sin(6.0 * phi)) + (epsilon * sin(8.0 * phi)));
	return result;
	
}

double EncodeTool::UTMCentralLatitude(char zone)
{
	
	if(zone-'A'>=3 && zone-'A'<=8)  //C~H
	{
		return DegToRad((double)((int)(zone - 'A' + 1) * 8 - 100));
	}
	else if(zone-'A'>=10 && zone-'A'<=14)  //J~N
	{
		return DegToRad((double)(((int)(zone - 'A' + 1) - 1) * 8 - 100));
	}
	else //P~X
	{
		return DegToRad((double)(((int)(zone - 'A' + 1) - 2) * 8 - 100));
	}
}

double EncodeTool::UTMCentralMeridian(int zone)
{
	double tmp = -183.0 + (zone * 6.0);
	if(tmp < 0) tmp -= 6;
	return DegToRad(tmp); 
}




double EncodeTool::FootpointLatitude(double y)
{
	double y_, alpha_, beta_, gamma_, delta_, epsilon_, n;
	double result;

	n = (sm_a - sm_b) / (sm_a + sm_b);
	alpha_ = ((sm_a + sm_b) / 2.0) * (1 + (pow(n, 2.0) / 4) + (pow(n, 4.0) / 64));
	y_ = y / alpha_;
	beta_ = (3.0 * n / 2.0) + (-27.0 * pow(n, 3.0) / 32.0) + (269.0 * pow(n, 5.0) / 512.0);
	gamma_ = (21.0 * pow(n, 2.0) / 16.0) + (-55.0 * pow(n, 4.0) / 32.0);
	delta_ = (151.0 * pow(n, 3.0) / 96.0) + (-417.0 * pow(n, 5.0) / 128.0);
	epsilon_ = (1097.0 * pow(n, 4.0) / 512.0);
	result = y_ + (beta_ * sin(2.0 * y_)) + (gamma_ * sin(4.0 * y_)) + (delta_ * sin(6.0 * y_)) + (epsilon_ * sin(8.0 * y_));
	return result;
}

void EncodeTool::MapLatLonToXY(double phi, double lambda, double lambda0, sigma::UTMCorr &xy)
{
	double N, nu2, ep2, t, t2, l;
	double l3coef, l4coef, l5coef, l6coef, l7coef, l8coef;
	double tmp;

	ep2 = (pow(sm_a, 2.0) - pow(sm_b, 2.0)) / pow(sm_b, 2.0);
	nu2 = ep2 * pow(cos(phi), 2.0);
	N = pow(sm_a, 2.0) / (sm_b * sqrt(1 + nu2));
	t = tan(phi);
	t2 = t * t;
	tmp = (t2 * t2 * t2) - pow(t, 6.0);
	l = lambda - lambda0;

	l3coef = 1.0 - t2 + nu2;
	l4coef = 5.0 - t2 + 9 * nu2 + 4.0 * (nu2 * nu2);
	l5coef = 5.0 - 18.0 * t2 + (t2 * t2) + 14.0 * nu2 - 58.0 * t2 * nu2;
	l6coef = 61.0 - 58.0 * t2 + (t2 * t2) + 270.0 * nu2 - 330.0 * t2 * nu2;
	l7coef = 61.0 - 479.0 * t2 + 179.0 * (t2 * t2) - (t2 * t2 * t2);
	l8coef = 1385.0 - 3111.0 * t2 + 543.0 * (t2 * t2) - (t2 * t2 * t2);

	xy.x = N * cos(phi) * l + (N / 6.0 * pow(cos(phi), 3.0) * l3coef * pow(l, 3.0))
		+ (N / 120.0 * pow(cos(phi), 5.0) * l5coef * pow(l, 5.0))
		+ (N / 5040.0 * pow(cos(phi), 7.0) * l7coef * pow(l, 7.0));
	xy.y = ArcLengthOfMeridian(phi)
		+ (t / 2.0 * N * pow(cos(phi), 2.0) * pow(l, 2.0))
		+ (t / 24.0 * N * pow(cos(phi), 4.0) * l4coef * pow(l, 4.0))
		+ (t / 720.0 * N * pow(cos(phi), 6.0) * l6coef * pow(l, 6.0))
		+ (t / 40320.0 * N * pow(cos(phi), 8.0) * l8coef * pow(l, 8.0));
}

void EncodeTool::MapXYToLatLon(double x, double y, double lambda0, sigma::WGS84Corr &philambda)
{
	double phif, Nf, Nfpow, nuf2, ep2, tf, tf2, tf4, cf;
	double x1frac, x2frac, x3frac, x4frac, x5frac, x6frac, x7frac, x8frac;
	double x2poly, x3poly, x4poly, x5poly, x6poly, x7poly, x8poly;

	phif = FootpointLatitude(y);
	ep2 = (pow(sm_a, 2.0) - pow(sm_b, 2.0)) / pow(sm_b, 2.0);
	cf = cos(phif);
	nuf2 = ep2 * pow(cf, 2.0);
	Nf = pow(sm_a, 2.0) / (sm_b * sqrt(1 + nuf2));
	Nfpow = Nf;
	tf = tan(phif);
	tf2 = tf * tf;
	tf4 = tf2 * tf2;

	x1frac = 1.0 / (Nfpow * cf);
	Nfpow *= Nf;
	x2frac =tf / (2.0 * Nfpow);
	Nfpow *= Nf;
	x3frac = 1.0 / (6.0 * Nfpow * cf);
	Nfpow *= Nf;
	x4frac = tf / (24.0 * Nfpow);
	Nfpow *= Nf;
	x5frac = 1.0 / (120.0 * Nfpow * cf);
	Nfpow *= Nf;
	x6frac = tf / (720.0 * Nfpow);
	Nfpow *= Nf;
	x7frac = 1.0 / (5040.0 * Nfpow * cf);
	Nfpow *= Nf;
	x8frac = tf / (40320.0 * Nfpow);

	x2poly = -1.0 - nuf2;
	x3poly = -1.0 - 2 * tf2 - nuf2;
	x4poly = 5.0 + 3.0 * tf2 + 6.0 * nuf2 - 6.0 * tf2 * nuf2 - 3.0 * (nuf2 * nuf2) - 9.0 * tf2 * (nuf2 * nuf2);
	x5poly = 5.0 + 28.0 * tf2 + 24.0 * tf4 + 6.0 * nuf2 + 8.0 * tf2 * nuf2;
	x6poly = -61.0 - 90.0 * tf2 - 45.0 * tf4 - 107.0 * nuf2 + 162.0 * tf2 * nuf2;
	x7poly = -61.0 - 662.0 * tf2 - 1320.0 * tf4 - 720.0 * (tf4 * tf2);
	x8poly = 1385.0 + 3633.0 * tf2 + 4095.0 * tf4 + 1575 * (tf4 * tf2);

	philambda.lat = phif + x2frac * x2poly * (x * x) + x4frac * x4poly * pow(x, 4.0) + x6frac * x6poly * pow(x, 6.0) + x8frac * x8poly * pow(x, 8.0);
	philambda.log = lambda0 + x1frac * x + x3frac * x3poly * pow(x, 3.0) + x5frac * x5poly * pow(x, 5.0) + x7frac * x7poly * pow(x, 7.0);
}

void EncodeTool::LatLonToUTMXY(double lat, double lon, int zone, sigma::UTMCorr &xy)
{
	MapLatLonToXY(lat, lon, UTMCentralMeridian(zone), xy);
	xy.x = xy.x * UTMScaleFactor + 500000.0;
	xy.y = xy.y * UTMScaleFactor;
	if(xy.y < 0.0)
		xy.y += 10000000.0;
}	

void EncodeTool::UTMXYToLatLon(double x, double y, int zone, bool southhemi, sigma::WGS84Corr &latlon)
{
	double cmeridian;
	x -= 500000.0;
	x /= UTMScaleFactor;

	if(southhemi)
		y -= 10000000.0;
	y /= UTMScaleFactor;

	cmeridian = UTMCentralMeridian(zone);
	MapXYToLatLon(x, y, cmeridian, latlon);
}

char EncodeTool::GetZoneChar(double lat)
{
	int temp = RadToDeg(lat);
	int step = (int)temp / 8;
	char start1 = 'N';
	char start2 = 'M';
	char res; 
	if(step == 0)
	{
		if(temp >= 0)
			res = start1;
		else
			res = start2;
	}
	else
	{
		if(step > 0)
		{
			res = start1 + 1 + step;
		}
		else
		{
			if(step >= -3)
				res = start2 + step;
			else
				res = start2 + step - 1;
		}
	}
	return res;
}

void EncodeTool::LatLonToSIGMA(double lat, double lon, double ele, sigma::Time *time, sigma::sigmaCorr &corr)
{
	double latitude = RadToDeg(lat);
	double longitude = RadToDeg(lon);
	corr.number = GetZoneNum(lon);
	corr.symbol = GetZoneChar(lat);
	string zone = to_string(corr.number) + corr.symbol;
	
	sigma::UTMCorr tmp, center_temp;
	double temp_lat, temp_lon;
	LatLonToUTMXY(lat, lon, corr.number, tmp);
	temp_lon = UTMCentralMeridian(corr.number);
	temp_lat = UTMCentralLatitude(corr.symbol);
	LatLonToUTMXY(temp_lat, temp_lon, corr.number, center_temp);
	printf("Pos    lat lon:(%f,%f)\n",RadToDeg(lat),RadToDeg(lon));
	printf("Center lat lon:(%f,%f)\n",RadToDeg(temp_lat),RadToDeg(temp_lon));
	cout << "------------------------" << endl;
	
	corr.position.Posx = tmp.x - center_temp.x;
	corr.position.Posy = tmp.y - center_temp.y;
	printf("Pos    Corr:(%f,%f)\n",tmp.x, tmp.y);
	printf("Center Corr:(%f,%f)\n",center_temp.x, center_temp.y);
	corr.position.Posz = ele - Elevation[zone];
	corr.time.s = time->s;
	corr.time.ns = time->ns;
}

void EncodeTool::SIGMAToLatLon(sigma::sigmaCorr corr, sigma::WGS84Corr &latlon)
{
	sigma::UTMCorr center_temp;
	double temp_lat, temp_lon;
	temp_lon = UTMCentralMeridian(corr.number);
	temp_lat = UTMCentralLatitude(corr.symbol);
	LatLonToUTMXY(temp_lat, temp_lon, corr.number, center_temp);

	corr.position.Posx += center_temp.x;
	corr.position.Posy += center_temp.y;
	UTMXYToLatLon(corr.position.Posx, corr.position.Posy, corr.number, (corr.symbol<='M'), latlon);
}

string EncodeTool::Geohash_w_bin(double w, double left, double right, int step, int max_step)
{
	if(step > max_step)
	{
		return "";
	}
	double mid = (left + right) * 1.0 / 2;

	if(w >= left && w <= mid)
		return Geohash_w_bin(w, left, mid, step+1, max_step) + "0";
	if(w > mid && w <= right)
		return Geohash_w_bin(w, mid, right, step+1, max_step) + "1";
}

string EncodeTool::Geohash_merge(string Lon, string Lat)
{
	string s;
	for(int i=0;i<Lon.size();i++)
	{
		s += Lon[i];
		s += Lat[i];
	}
	return s;
}

string EncodeTool::Geohash_base32(string s)
{
	string temp;
	string ans;
	for(int i=0;i<s.size();i++)
	{
		temp += s[i];
		if((i + 1) % 5 == 0)
		{
			ans += base32[temp];
			temp = "";
		}
	}
	return ans;
}

string EncodeTool::Geohash_space(sigma::sigmaCorr corr, int precision)
{
	sigma::WGS84Corr temp;
	SIGMAToLatLon(corr,temp);

	string s_w="", s_j="";
	string s="", ss="";

	double LatDeg = RadToDeg(temp.lat);
	double LonDeg = RadToDeg(temp.log);

	double left = (int)(LonDeg/6)*6;
	double lower = (int)(LatDeg/8)*8;

	s_w = Geohash_w_bin(LatDeg,lower,lower+8.0,1,precision/2*5);
	s_j = Geohash_w_bin(LonDeg,left,left+6.0,1,precision/2*5);
	//s_w = Geohash_w_bin(LatDeg,-90,90,1,precision/2*5);
	//s_j = Geohash_w_bin(LonDeg,-180,180,1,precision/2*5);

	reverse(s_w.begin(), s_w.end());
	reverse(s_j.begin(), s_j.end());

	s = Geohash_merge(s_j, s_w);
	s = Geohash_base32(s);

	return s;
}
















