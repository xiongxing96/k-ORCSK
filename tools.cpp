#include "tools.h"

#define NAN_D -999999

using namespace std;

int Round(double r)  
{  
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);  
}  

double parseFloat(const std::string& input)
{
    const char *p = input.c_str();
    if (!*p || *p == '?')
        return NAN_D;
    int s = 1;
    while (*p == ' ') 
		p++;

    if (*p == '-') 
	{
		s = -1; 
		p++;
	}

    double acc = 0;
    while (*p >= '0' && *p <= '9')
        acc = acc * 10 + *p++ - '0';

    if (*p == '.') 
	{
		double k = 0.1;
	    p++;
	    while (*p >= '0' && *p <= '9') 
		{
			acc += (*p++ - '0') * k;
			k *= 0.1;
		}
	}
    
	if (*p) 
	{
		std::cout << "Invalid numeric format from parseFloat" << std::endl;	
		return NAN_D;
	}

	return s * acc;
}

int parseInt(const std::string& input)
{
    const char *p = input.c_str();
    if (!*p || *p == '?')
        return NAN_D;
    int s = 1;
    while (*p == ' ') 
		p++;

    if (*p == '-') 
	{
		s = -1; 
		p++;
	}

    double acc = 0;
    while (*p >= '0' && *p <= '9')
        acc = acc * 10 + *p++ - '0';

	if (*p) 
	{
		std::cout << "Invalid numeric format from parseInt" << std::endl;	
		return NAN_D;
	}

	return s * acc;
}

std::vector<std::string> split(const std::string &s, const std::string &seperator)
{
	std::vector<std::string> result;
	typedef std::string::size_type string_size;
	string_size i = 0;
		    
	while(i != s.size())
	{
		int flag = 0;
	    while(i != s.size() && flag == 0)
		{
			flag = 1;
		    for(string_size x = 0; x < seperator.size(); ++x)
			{
				if(s[i] == seperator[x])
				{
					++i;
					flag = 0;
					break;
				}
			}
	    }
					
		flag = 0;
		string_size j = i;
		while(j != s.size() && flag == 0)
		{
			for(string_size x = 0; x < seperator.size(); ++x)
			{
				if(s[j] == seperator[x])
				{
					flag = 1;
					break;
				}
			}
			if(flag == 0) 
				++j;
		}
		 
		if(i != j)
		{
			result.push_back(s.substr(i, j-i));
			i  = j;
		}
	}

	return result;
}

int getdir (std::string dir, std::vector<std::string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) 
	{
		std::cout << "Error(" << errno << ") opening " << dir << std::endl;
	        return errno;
	}

    while ((dirp = readdir(dp)) != NULL) 
	{
	        files.push_back(std::string(dirp->d_name));
	}
    closedir(dp);
    return 0;
}

double nodeRoadDistance(double nx, double ny, double rx1, double ry1, double rx2, double ry2)
{
	return (fabs((ry2 - ry1) * nx +(rx1 - rx2) * ny + ((rx2 * ry1) -(rx1 * ry2)))) / (sqrt(pow(ry2 - ry1, 2) + pow(rx1 - rx2, 2)));
}

void GetFootOfPerpendicular(
	double nx, double ny,
	double rx1, double ry1,
	double rx2, double ry2,
	double& x, double& y)
{
	double dx = rx1 - rx2;
	double dy = ry1 - ry2;
	if(std::abs(dx) < 0.00000001 && std::abs(dy) < 0.00000001 )
	{
		x = rx1;
		y = ry1;
	}

	double u = (nx - rx1) * (rx1 - rx2) + (ny - ry1)*(ry1 - ry2);
	u = u/((dx*dx)+(dy*dy));

	x = rx1 + u*dx;
	y = ry1 + u*dy;
}
		
bool nodeOnRoad(double nx, double ny, double rx1, double ry1, double rx2, double ry2)
{
	if(nx < rx1 && nx < rx2)
		return false;
	if(nx > rx1 && nx > rx2)
		return false;
	if(ny < ry1 && ny < ry2)
		return false;
	if(ny > ry1 && ny > ry2)
		return false;
	return true;
}

int findCost(std::vector<int> &vCostT, std::vector<int> &vCostC, int t)
{
	std::vector<int>::iterator low, high;
	low = std::lower_bound(vCostT.begin(), vCostT.end(), t);
	if(*low > t)
	{
		high = low;
		low -= 1;
		int t1, t2;
		int c1, c2;
		t1 = *low;
		t2 = *high;
		c1 = vCostC[low - vCostT.begin()];
		c2 = vCostC[high - vCostT.begin()];
		return (c2-c1)/(t2-t1)*(t-t1) + c1;
	}
	else
	{
		return vCostC[low - vCostT.begin()];
	}

}

int hasIntersection(int x11, int y11, int x12, int y12, int x21, int y21, int x22, int y22, int& x, int& y)
{
	std::pair<int, int> p11 = std::make_pair(x11, y11);
	std::pair<int, int> p12 = std::make_pair(x12, y12);
	std::pair<int, int> p21 = std::make_pair(x21, y21);
	std::pair<int, int> p22 = std::make_pair(x22, y22);

	double d1 = direction(p21, p22, p11);
	double d2 = direction(p21, p22, p12);
	double d3 = direction(p11, p12, p21);
	double d4 = direction(p11, p12, p22);
	
	if(d1>0 && d2<0 && d3<0 && d4>0 )
	{
		double k1=(double)(y11-y12)/(x11-x12);
		double k2=(double)(y21-y22)/(x21-x22);
		x = (k1*x11 - k2*x21 +y21 - y11) / (k1-k2);
		y = y11 + (x-x11)*k1;
		if(x < 0 || y < 0)
		{
/*			std::cout << y11 << "\t" << y12 << "\t" << y11-y12 << "\t" << x11 << "\t" << x12 << "\t" << x11-x12 << "\t" << (double)(y11-y12)/(x11-x12) << "\t" << k1 << std::endl;
			std::cout << y21 << "\t" << y22 << "\t" << y21-y22 << "\t" << x21 << "\t" << x22 << "\t" << x21-x22 << "\t" << (double)(y21-y22)/(x21-x22) << "\t" << k2 << std::endl;
			std::cout << "k1:" << k1 << "\tk2:" << k2 << "\tx:" << x << "\ty:" << y << std::endl;
*/			return 0;
		}
		if(onSegment(p11, p12, std::make_pair(x,y)) && onSegment(p21, p22, std::make_pair(x,y)))
		{
			return 1;
		}
		else
			return 0;
	}
	else if(d1<0 && d2>0 && d3>0 && d4<0)
	{
		double k1=(double)(y11-y12)/(x11-x12);
		double k2=(double)(y21-y22)/(x21-x22);
		x = (k1*x11 - k2*x21 +y21 - y11) / (k1-k2);
		y = y11 + (x-x11)*k1;
		if(x < 0 || y < 0)
		{
/*			std::cout << y11 << "\t" << y12 << "\t" << y11-y12 << "\t" << x11 << "\t" << x12 << "\t" << x11-x12 << "\t" << (double)(y11-y12)/(x11-x12) << "\t" << k1 << std::endl;
			std::cout << y21 << "\t" << y22 << "\t" << y21-y22 << "\t" << x21 << "\t" << x22 << "\t" << x21-x22 << "\t" << (double)(y21-y22)/(x21-x22) << "\t" << k2 << std::endl;
			std::cout << "k1:" << k1 << "\tk2:" << k2 << "\tx:" << x << "\ty:" << y << std::endl;
*/			return 0;
		}
		if(onSegment(p11, p12, std::make_pair(x,y)) && onSegment(p21, p22, std::make_pair(x,y)))
		{
//			std::cout << "k1:" << k1 << "\tk2:" << k2 << "\tx:" << x << "\ty:" << y << std::endl;
			return 2;
		}
		else
			return 0;
	}
	else
		return 0;
}

double direction(std::pair<int, int> pi, std::pair<int, int> pj, std::pair<int, int> pk)
{
	return (pk.first-pi.first) * (pj.second-pi.second) - (pj.first-pi.first) * (pk.second-pi.second);
}

bool onSegment(std::pair<int, int> pi, std::pair<int, int> pj, std::pair<int, int> pk)
{
	if(min(pi.first, pj.first) < pk.first < max(pi.first, pj.first) && min(pi.second, pj.second) <= pk.second <= max(pi.second, pj.second))
		return true;
	else
		return false;

}

double min(double x, double y)
{
	if(x < y)
		return x;
	else 
		return y;
}

double max(double x, double y)
{
	if(x > y)
		return x;
	else
		return y;
}

//Return the intersection segment of two line segments
//Else return false
bool lineCoincide(std::pair<int, int> p1, std::pair<int, int> p2, std::pair<int, int> p3, std::pair<int, int> p4, std::pair<int, int>& pr1, std::pair<int, int>& pr2)
{
	double d1 = direction(p3,p4,p1);
	if(d1 != 0)
		return false;

	double d2 = direction(p3,p4,p2);
	if(d2 != 0)
		return false;

	int px1, px2, py1, py2;
	px1 = max(p1.first, p3.first);
	px2 = min(p2.first, p4.first);

	if(px1 == p1.first)
		py1 = p1.second;
	else
		py1 = p3.second;

	if(px2 == p2.first)
		py2 = p2.second;
	else
		py2 = p4.second;

	pr1 = std::make_pair(px1, py1);
	pr2 = std::make_pair(px2, py2);

	return true;
}

double lineGradient(int x1, int y1, int x2, int y2)
{
	return (double)(y2-y1)/(x2-x1);
}

int hasIntersectionDouble(double x11, double y11, double x12, double y12, double x21, double y21, double x22, double y22, double& x, double& y)
{
	std::pair<double, double> p11 = std::make_pair(x11, y11);
	std::pair<double, double> p12 = std::make_pair(x12, y12);
	std::pair<double, double> p21 = std::make_pair(x21, y21);
	std::pair<double, double> p22 = std::make_pair(x22, y22);

	double d1 = direction(p21, p22, p11);
	double d2 = direction(p21, p22, p12);
	double d3 = direction(p11, p12, p21);
	double d4 = direction(p11, p12, p22);
	
	if(d1>0 && d2<0 && d3<0 && d4>0 )
	{
		double k1=(double)(y11-y12)/(x11-x12);
		double k2=(double)(y21-y22)/(x21-x22);
		x = (k1*x11 - k2*x21 +y21 - y11) / (k1-k2);
		y = y11 + (x-x11)*k1;
		if(x < 0 || y < 0)
		{
			return 0;
		}
		if(onSegment(p11, p12, std::make_pair(x,y)) && onSegment(p21, p22, std::make_pair(x,y)))
		{
			return 1;
		}
		else
			return 0;
	}
	else if(d1<0 && d2>0 && d3>0 && d4<0)
	{
		double k1=(double)(y11-y12)/(x11-x12);
		double k2=(double)(y21-y22)/(x21-x22);
		x = (k1*x11 - k2*x21 +y21 - y11) / (k1-k2);
		y = y11 + (x-x11)*k1;
		if(x < 0 || y < 0)
		{
			return 0;
		}
		if(onSegment(p11, p12, std::make_pair(x,y)) && onSegment(p21, p22, std::make_pair(x,y)))
		{
			return 2;
		}
		else
			return 0;
	}
	else
		return 0;
}

double direction(std::pair<double,double> pi, std::pair<double, double> pj, std::pair<double, double> pk)
{
	return (pk.first-pi.first) * (pj.second-pi.second) - (pj.first-pi.first) * (pk.second-pi.second);
}

bool findFile(std::string filename)
{
	std::vector<std::string> vFiles;
	std::vector<std::string>::iterator ivFiles;
	getdir("./", vFiles);

	for(ivFiles = vFiles.begin(); ivFiles != vFiles.end(); ivFiles++) 
		if(*ivFiles == filename)  
			return true;

	return false;
}

void strToVec(std::vector<int> &key, std::string &ss)
{
			char str[100];// note the size of str,maybe not save(using method strcpy()) and lead to buffer overflow
			strcpy(str,ss.c_str());
			const char * split = ",";
    		char * p;
    		p = strtok (str,split);
    		while(p!=NULL) {
					key.push_back(atoi(p));   
        		p = strtok(NULL,split);
        }
} 