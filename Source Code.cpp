/**
*
* Carnegie Mellon University.
* Department of Mechanical Engineering
* 24-780: Engineering Computation
* Professor: Soji Yamakawa
*
* Project Title: CNC Lathe.
* Team: Falcon
*
* Team Members: Vaibhav Jain
*               Apurva Modak
*               Archit Dhanani
*               Akshay Ramakrishnan
*               Yashdeep Padey
*               Deepak Kakad
*               Kedar Panchanadikar
*/
#include<stdio.h>
#include<ctype.h>
#include<string>
#include<sstream>
#include "fssimplewindow.h"
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include "ysglfontdata.h"
#include<time.h>
using namespace std;
char *YsFgets(char str[], int maxn, FILE *fp);
const double YS_PI = 3.1415927;
using namespace std;
class TextString
{
protected:
	char *str;
public:
	TextString();
	TextString(const TextString &incoming);
	~TextString();
	void CleanUp(void);
	TextString &operator=(const TextString &incoming);
	void CopyFrom(const TextString &incoming);
	void Add(char c);
	void Add(const char str[]);
	void BackSpace(void);
	void Set(const char incoming[]);
	const char *GetPointer(void) const;
	int Strlen(void) const;
	const char *Fgets(FILE *fp);
};
TextString::TextString()
{
	str = nullptr;
}
TextString::TextString(const TextString &incoming)
{
	str = nullptr;
	CopyFrom(incoming);
}
TextString::~TextString()
{
	CleanUp();
}
TextString &TextString::operator=(const TextString &incoming)
{
	CopyFrom(incoming);
	return *this;
}
void TextString::CopyFrom(const TextString &incoming)
{
	Set(incoming.GetPointer());
}
void TextString::CleanUp(void)
{
	if (nullptr != str)
	{
		delete[] str;
		str = nullptr;
	}
}
void TextString::Add(char c)
{
	if (nullptr == str)
	{
		const char newStr[2] = { c, 0 };
		Set(newStr);
	}
	else
	{
		// Allocate
		auto l = strlen(str);
		char *newStr = new char[l + 2];
		// Copy
		strcpy(newStr, str);
		newStr[l] = c;
		newStr[l + 1] = 0;
		// Set(newStr);
		// delete [] newStr;
		// CleanUp
		CleanUp();
		// Re-point
		str = newStr;
	}
}
void TextString::Add(const char str[])
{
	for (int i = 0; str[i] != 0; ++i)
	{
		Add(str[i]);
	}
}
void TextString::Set(const char incoming[])
{
	if (incoming != str)
	{
		auto l = strlen(incoming);
		char *newStr = new char[l + 1];
		strcpy(newStr, incoming);
		CleanUp();
		str = newStr;
	}
}
void TextString::BackSpace(void)
{
	if (nullptr != str)
	{
		auto l = strlen(str);
		// Quick version
		// if(0<l)
		// {
		//  str[l-1]=0;
		// }
		if (0<l)
		{
			str[l - 1] = 0;
			// Allocate
			char *newStr = new char[l];
			// Copy
			strcpy(newStr, str);
			// CleanUp
			CleanUp();
			// Re-point
			str = newStr;
		}
	}
}
const char *TextString::GetPointer(void) const
{
	if (nullptr != str)
	{
		return str;
	}
	else
	{
		return "";
	}
}
int TextString::Strlen(void) const
{
	return strlen(GetPointer());
}
const char *TextString::Fgets(FILE *fp)
{
	CleanUp();
	char str[16];
	for (;;)
	{
		if (nullptr != fgets(str, 15, fp))
		{
			for (int i = 0; str[i] != 0; ++i)
			{
				if (str[i] == '\n')
				{
					return GetPointer();
				}
				Add(str[i]);
			}
		}
		else
		{
			if (0<Strlen())
			{
				return GetPointer();
			}
			else
			{
				return nullptr;
			}
		}
	}
}
class Graphics
{
public:
	int ox = 50, oy = 550;
	//void passzero();
	void initialize();
	void drawStock(double lf, double df);
	void drawFeature(int n, int f, double xf, double yf, double df,
		double lf, double af);
};
void DrawLine(int x, int y, int ax, int ay)
{
	glBegin(GL_LINES);
	glVertex2i(x, y);
	glVertex2i(ax, ay);
	glEnd();
}
void Graphics::initialize()//draws ref lines and origin
{
	glBegin(GL_LINE_LOOP);
	glVertex2i(1, 0);
	glVertex2i(799, 0);
	glVertex2i(799, 598);
	glVertex2i(1, 598);
	glEnd();
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xff00);
	glColor3f(0.0, 0.0, 0.0);
	DrawLine(0, oy, 600, oy);
	DrawLine(ox, 100, ox, 600);
	glDisable(GL_LINE_STIPPLE);
	glFlush();
	// PRINTING O
	glColor3ub(0, 0, 0);
	glRasterPos2d(30, 575);
	YsGlDrawFontBitmap16x20("O");
	DrawLine(0, 100, 800, 100);
	DrawLine(600, 100, 600, 600);
	DrawLine(600, 225, 800, 225);
	DrawLine(600, 350, 800, 350);
	DrawLine(600, 475, 800, 475);
	//CheckBox Dimensions (Rightmost Box)
	DrawLine(750, 0, 750, 50);
	glLineWidth(3);
	DrawLine(760, 25, 770, 35);
	DrawLine(770, 35, 790, 15);
	//tabs
	glColor3ub(0, 0, 0);
	glRasterPos2d(645, 170);
	YsGlDrawFontBitmap16x20("FACING");
	glColor3ub(0, 0, 0);
	glRasterPos2d(635, 300);
	YsGlDrawFontBitmap16x20("TURNING");
	glColor3ub(0, 0, 0);
	glRasterPos2d(650, 400);
	YsGlDrawFontBitmap16x20("TAPER");
	glColor3ub(0, 0, 0);
	glRasterPos2d(635, 440);
	YsGlDrawFontBitmap16x20("TURNING");
	glColor3ub(0, 0, 0);
	glRasterPos2d(625, 525);
	YsGlDrawFontBitmap16x20("STOCK DIM.");
	DrawLine(0, 50, 800, 50);
	DrawLine(400, 0, 400, 100);
	DrawLine(600, 0, 600, 100);
	glColor3ub(0, 0, 0);
	glRasterPos2d(415, 35);
	YsGlDrawFontBitmap16x20("D:");
	glColor3ub(0, 0, 0);
	glRasterPos2d(615, 35);
	YsGlDrawFontBitmap16x20("L:");
	glColor3ub(0, 0, 0);
	glRasterPos2d(415, 85);
	YsGlDrawFontBitmap16x20("ANGLE:");
	glColor3ub(0, 0, 0);
	glRasterPos2d(615, 85);
	YsGlDrawFontBitmap16x20("OFFSET:");
	glColor3ub(0, 0, 0);
	glRasterPos2d(50, 85);
	YsGlDrawFontBitmap16x20("SELECT START POINT");
}
void Graphics::drawStock(double lf, double df)
{
	double D, L;
	D = df;
	L = lf;
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex2d(ox, oy - D / 2);
	glVertex2d(ox + L, oy - D / 2);
	glVertex2d(ox + L, oy);
	glVertex2d(ox, oy);
	glEnd();
}

void Graphics::drawFeature(int n, int f, double xf, double yf, double
	df, double lf, double af)
{
	double D1, L1, X1, Y1;
	X1 = xf;
	Y1 = 2 * yf;
	D1 = df;
	L1 = lf;
	switch (f)
	{
	case 1:                      //turning
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex2d(ox + X1 - L1, oy - Y1 / 2);
		glVertex2d(ox + X1, oy - Y1 / 2);
		glEnd();
		glBegin(GL_LINES);
		glVertex2d(ox + X1, oy - Y1 / 2);
		glVertex2d(ox + X1, oy - (D1) / 2);
		glEnd();
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		glVertex2d(ox + X1 - L1, oy - Y1 / 2);
		glVertex2d(ox + X1 - L1, oy - (D1) / 2);
		glEnd();
		glBegin(GL_LINES);
		glVertex2d(ox + X1 - L1, oy - (D1) / 2);
		glVertex2d(ox + X1, oy - (D1) / 2);
		glEnd();
		break;

	case 2:                      //facing
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_LOOP);
		glVertex2d(ox + X1 - L1, oy);
		glVertex2d(ox + X1 - L1, oy - Y1 / 2);
		glVertex2d(ox + X1, oy - Y1 / 2);
		glVertex2d(ox + X1, oy);
		glEnd();
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		glVertex2d(ox + X1 - L1, oy - Y1 / 2);
		glVertex2d(ox + X1 - L1, oy);
		glEnd();
		break;
	case 3:                      //taper turning
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex2d(ox + X1 - L1, oy - Y1 / 2);
		glVertex2d(ox + X1, oy - Y1 / 2);
		glEnd();
		glBegin(GL_LINES);
		glVertex2d(ox + X1, oy - Y1 / 2);
		glVertex2d(ox + X1, oy - (Y1 / 2) + L1*tan(YS_PI*af / 180));
		glEnd();
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		glVertex2d(ox + X1 - L1, oy - Y1 / 2);
		glVertex2d(ox + X1, oy - (Y1 / 2) + L1*tan(YS_PI*af / 180));
		glEnd();
		break;
	case 4:                      //parting
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex2d(ox + X1, oy - Y1 / 2);
		glVertex2d(ox + X1 - L1, oy - Y1 / 2);
		glEnd();
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		glVertex2d(ox + X1, oy - Y1 / 2);
		glVertex2d(ox + X1, oy - (D1) / 2);
		glEnd();
		glBegin(GL_LINES);
		glVertex2d(ox + X1, oy - (D1) / 2);
		glVertex2d(ox + X1 - L1, oy - (D1) / 2);
		glEnd();
		glBegin(GL_LINES);
		glVertex2d(ox + X1 - L1, oy - (D1) / 2);
		glVertex2d(ox + X1 - L1, oy - Y1 / 2);
		glEnd();
		break;
	}
}
void Highlight(int x, int y, int w, int h)
{
	//glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x + w, y);
	glVertex2i(x + w, y + h);
	glVertex2i(x, y + h);
	glEnd();
}
void DrawErrorBox(int x)
{
	glColor3ub(200, 200, 200);
	glBegin(GL_QUADS);
	glVertex2i(220, 220);
	glVertex2i(580, 220);
	glVertex2i(580, 380);
	glVertex2i(220, 380);
	glEnd();
	glLineWidth(1);
	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(220, 220);
	glVertex2i(580, 220);
	glVertex2i(580, 380);
	glVertex2i(220, 380);
	glEnd();
	glColor3ub(0, 0, 0);
	glRasterPos2d(240, 260);
	YsGlDrawFontBitmap12x16("Dimensions too large.");
	if (x == 0)
	{
		glColor3ub(0, 0, 0);
		glRasterPos2d(240, 290);
		YsGlDrawFontBitmap10x14("D should be between 0 and 350.");
	}
	else if (x == 1)
	{
		glColor3ub(0, 0, 0);
		glRasterPos2d(240, 290);
		YsGlDrawFontBitmap12x16("L should be between 0 & 600");
	}
	//YsGlDrawFontBitmap16x20("Dimensions too large.");
	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(500, 340);
	glVertex2i(560, 340);
	glVertex2i(560, 360);
	glVertex2i(500, 360);
	glEnd();
	glColor3ub(0, 0, 0);
	glRasterPos2d(522, 356);
	YsGlDrawFontBitmap10x14("OK");
	glLineWidth(3);
}
int check(int mx, int my, double x, double y)
{
	if (mx < 55 + x && mx > 45 + x && my > 545 - y && my < 555 - y)
		return 1;
	else
		return 0;
}
void Swap(double &a, double &b)
{
	double c;
	c = a;
	a = b;
	b = c;
}

class varray
{
protected:
	double *ptr;
	int length;
public:
	//default constructor
	varray()
	{
		ptr = nullptr;
		length = 0;
	}
	//Initialization of the array directly
	varray(double points[], int length)
	{
		this->length = length;
		ptr = new double[length * 2];
		for (int i = 0; i < length * 2; ++i)
		{
			ptr[i] = points[i];
		}
	}
	//To define the initial length of the array and to allocate the memory to array.
	varray(int initialCapacity)
	{
		length = initialCapacity;
		ptr = new double[length * 2];
	}
	//Methods to deallocate the memory
	void CleanUp()
	{
		if (ptr != nullptr)
		{
			delete[] ptr;
		}
		ptr = nullptr;
		length = 0;
	}
	~varray()
	{
		CleanUp();
	}
	varray &operator=(varray &a)
	{
		this->ptr = nullptr;
		length = a.getLength();
		ptr = new double[length * 2];
		double *x = a.GetPointer();
		for (int i = 0; i < length * 2; ++i)
		{
			ptr[i] = x[i];
		}
		return *this;
	}
	//Getters
	int getLength() { return length; }
	double* GetPointer() { return ptr; }
	double xcoordinate(int index) { return ptr[index * 2]; }
	double ycoordinate(int index) { return ptr[index * 2 + 1]; }
	//to add a new point in the array
	void add(double x, double y)
	{
		int newlength = length + 1;
		double *newptr = new double[newlength * 2];
		for (int i = 0; i < length * 2; ++i)
		{
			newptr[i] = ptr[i];
		}
		newptr[length * 2] = x;
		newptr[length * 2 + 1] = y;
		ptr = newptr;
		length = newlength;
	}
	// to print the array already stored
	void printPoints()
	{
		for (int i = 0; i < length; ++i)
		{
			printf("point %d: %lf %lf\n", i + 1, ptr[i * 2], ptr[i * 2 + 1]);
		}
	}
};

//Class Article is to defined the final object to be manufactured
class Article
{
private:
	varray Points;
	double s_diameter = 20, s_length = 50;
	double c_diameter, diameter;
	double feed_rate, spindle_speed;
public:
	Article()
	{
		double x[] = { 0.0, 0.0, 0.0, 18.0, 25.0, 18.0, 25.0, 10.0, 45.0,
			10.0, 45.0, 0.0 };
		varray s(x, 6);
		feed_rate = 1;
		spindle_speed = 180;
		Points = s;
		c_diameter = s_diameter;
		diameter = s_diameter;
	}
	Article(double p[], int x, double s_d, double s_l, double fr, double ss)
	{
		varray s(p, x);
		Points = s;
		feed_rate = fr;
		spindle_speed = ss;
		s_diameter = s_d;
		s_length = s_l;
		c_diameter = s_diameter;
		diameter = s_diameter;
	}
	Article &operator=(Article &a)
	{
		feed_rate = a.getFeed();
		spindle_speed = a.getSpindlespeed();
		s_diameter = a.gets_diameter();
		s_length = a.gets_length();
		c_diameter = s_diameter;
		diameter = s_diameter;
		Points = a.get_pointer();
		return *this;
	}
	//Setters and Getters
	void setdiameter(double x) { diameter = x; }
	void setFeed(double x) { feed_rate = x; }
	void sets_length(double x) { s_length = x; }
	void setc_diameter(double x) { c_diameter = x; }
	void setSpindlespeed(double x) { spindle_speed = x; }
	double getSpindlespeed() { return spindle_speed; }
	double getFeed() { return feed_rate; }
	double getc_diameter() { return c_diameter; }
	double gets_diameter() { return s_diameter; }
	double getdiameter() { return diameter; }
	double gets_length() { return s_length; }
	varray &get_pointer() { return Points; }
	void Input_data()
	{
		//Input from the user
		printf("Enter the Dimensions of the raw material>\ndiameter and length respectively\n");
		scanf("%lf %lf", &s_diameter, &s_length);
		while (true)
		{
			int flag;
			printf("Do you want to enter point>");
			scanf("%d", &flag);
			if (flag == 0)
			{
				break;
			}
			else
			{
				double x, y;
				printf("Enter the co-ordinates of the point>");
				scanf("%lf %lf", &x, &y);
				Points.add(x, y);
			}
		}
		c_diameter = s_diameter;
	}
	double retFacingDepth()
	{
		Points.printPoints();
		double min = s_length + 1;
		for (int i = 0; i < Points.getLength(); ++i)
		{
			if ((s_length - Points.xcoordinate(i)) < min)
			{
				min = s_length - Points.xcoordinate(i);
			}
		}
		printf("%lf\n", min);
		return min;
	}
	double retturningDepthmin()
	{
		double mind = diameter + 1;
		for (int i = 0; i < Points.getLength(); ++i)
		{
			if (mind > Points.ycoordinate(i) && Points.ycoordinate(i) > 0)
			{
				mind = Points.ycoordinate(i);
			}
		}
		return mind;
	}
	double retturningDepthmax()
	{
		double maxd = 0;
		for (int i = 0; i < Points.getLength(); ++i)
		{
			if (maxd < Points.ycoordinate(i) && Points.ycoordinate(i) < diameter)
			{
				maxd = Points.ycoordinate(i);
			}
		}
		return maxd;
	}
	double indexofheight(double y)
	{
		for (int i = 0; i < Points.getLength(); ++i)
		{
			if (Points.ycoordinate(i) == y)
			{
				return Points.xcoordinate(i);
			}
		}
		return -1;
	}
	void indexofTaperStart(int &index)
	{
		Points.printPoints();
		int state = 0;
		for (int i = index + 1; i < Points.getLength() - 1; i++)
		{
			cout << i << endl;
			if (Points.ycoordinate(i) != Points.ycoordinate(i + 1))
			{
				if (Points.xcoordinate(i) != Points.xcoordinate(i + 1))
				{
					cout << "done!" << endl;
					index = i;
					state = 1;
					break;
				}
			}
		}
		if (state == 0)
		{
			index = -2;
		}
	}
	void getPoints(double &x1, double &y1, double &x2, double &y2, int index)
	{
		x1 = Points.xcoordinate(index);
		y1 = Points.ycoordinate(index);
		x2 = Points.xcoordinate(index + 1);
		y2 = Points.ycoordinate(index + 1);
	}
};

class Gcode
{
protected:
	int N;
	int *G;
	int *M;
	int glength, mlength;
	ofstream myfile;
public:
	Gcode()
	{
		N = 0;
		G = new int[0];
		M = new int[0];
		glength = 0;
		mlength = 0;
		myfile.open("GMcode.tap");
		//myfile << "N10 G90 G80 G40 G54 G20 G17 G50 G94 G64" << endl;
		//(safety block)
		//myfile << "N20 G90 G21 G00 X500 Y500 S0 F0" << endl;   //(Absolute and metric programming)
		N = N + 30;
		//myfile << endl;
	}
	~Gcode()
	{
		myfile << "N" << N << " G00 M06 X500 Y500 S0 F0 T01" << endl;
		N = N + 10;
		myfile << "N" << N << " G00 M05 X500 Y500 S0 F0 T01" << endl;// (End Block);
		N = N + 10;
		myfile << "N" << N << " G00 M30 X500 Y500 S0 F0 T01" << endl;
		N = N + 10;
		myfile.close();
	}
	Gcode(int g[], int m[], int x1, int x2)
	{
		N = 0;
		glength = x1;
		mlength = x2;
		G = new int[glength];
		M = new int[mlength];
		for (int i = 0; i < glength; ++i)
		{
			G[i] = g[i];
		}
		for (int i = 0; i < mlength; ++i)
		{
			M[i] = m[i];
		}
		myfile.open("GMcode.tap");
		// myfile << "N" << N << " G90 G80 G40 G54 G20 G17 G50 G94 G64" <<endl;//(safety block)
		N = N + 10;
		// myfile << "N" << N << " G90 G21 X500 Y500 S0 F0" << endl;
		//(Absolute and metric programming)
		N = N + 10;
	}
	void addGcode(int x1)
	{
		int g = glength + 1;
		int *x = new int[g];
		for (int i = 0; i < glength; ++i)
		{
			x[i] = G[i];
		}
		x[glength] = x1;
		G = x;
		glength = g;
	}
	void addMcode(int x1)
	{
		int m = mlength + 1;
		int *x = new int[m];
		for (int i = 0; i < mlength; ++i)
		{
			x[i] = M[i];
		}
		x[mlength] = x1;
		M = x;
		mlength = m;
	}

	void GCodeLine(double G, double M, double X, double Y, double
		spindle_speed, double feed_rate, int T)
	{
		myfile << "N" << N << " G0" << G << " M0" << M << " X" << X << " Y"
			<< Y << " S" << spindle_speed << " F" << feed_rate << " T0" << T <<
			endl;
	}

	void cuttingloop(Article &obj, double depth_x, double depth_y)
	{
		GCodeLine(1, 8, obj.gets_length() - depth_x, obj.getc_diameter(),
			obj.getSpindlespeed(), obj.getFeed(), 2);
		N = N + 10;
		GCodeLine(1, 8, obj.gets_length() - depth_x, obj.gets_diameter(),
			obj.getSpindlespeed(), obj.getFeed(), 2);
		N = N + 10;
		GCodeLine(0, 8, obj.gets_length() + 5, obj.gets_diameter(),
			obj.getSpindlespeed(), 5, 2);
		N = N + 10;
		GCodeLine(0, 8, obj.gets_length() + 5, obj.getc_diameter() -
			depth_y, obj.getSpindlespeed(), 5, 2);
		N = N + 10;
		myfile << endl;
	}
	void cuttingloopt(Article &obj, double x3, double depth_x, double
		depth_y, double y)
	{
		GCodeLine(1, 8, x3 - depth_x, y, obj.getSpindlespeed(), obj.getFeed(), 2);
		N = N + 10;
		GCodeLine(1, 8, x3 - depth_x, y, obj.getSpindlespeed(), obj.getFeed(), 2);
		N = N + 10;
		GCodeLine(0, 8, x3, y, obj.getSpindlespeed(), 5, 2);
		N = N + 10;
		GCodeLine(0, 8, x3, y - depth_y, obj.getSpindlespeed(), 5, 2);
		N = N + 10;
		myfile << endl;
	}
	void cuttingloopfacing(double depth, double bottom, double
		c_diameter, double feed_rate, double spindle_speed)
	{
		GCodeLine(0, 8, depth, c_diameter, spindle_speed, 5, 1);
		N = N + 10;
		GCodeLine(1, 8, depth, bottom, spindle_speed, feed_rate, 1);
		N = N + 10;
		GCodeLine(1, 8, depth + 20, bottom, spindle_speed, feed_rate, 1);
		N = N + 10;
		GCodeLine(0, 8, depth + 20, c_diameter, spindle_speed, 5, 1);
		N = N + 10;
		myfile << endl;
	}
	void toolchange(int T)
	{
		myfile << "N" << N << " G00 M06 X500 Y500 S0 F0 T0" << T << endl;
		N = N + 10;
	}
	void BlockStartLines(double initial_x, double initial_y, double
		spindle_speed, int T)
	{
		//N130 G00 M08 X55 Y18 S180 F5
		GCodeLine(0, 8, initial_x, initial_y, spindle_speed, 5, T);
		//(initial position for the cut and coolant on command)
		//myfile << "N50 G00 M08 X" << initial_x << " Y" << initial_y << "  S" << spindle_speed << endl;
		N = N + 10;
		myfile << endl;
	}
	void Facingcut(Article &obj, double depth)
	{
		BlockStartLines(obj.gets_length() + 5, obj.getc_diameter() + 5,
			obj.getSpindlespeed(), 1);
		double step = depth / 2;
		double c_depth = depth;
		for (int i = 0; i <= step; ++i)
		{
			if (c_depth < 2)
			{
				cuttingloopfacing(obj.gets_length() - c_depth, 0,
					obj.getc_diameter() + 5, obj.getFeed(), obj.getSpindlespeed());
				obj.sets_length(obj.gets_length() - c_depth);
			}
			else
			{
				cuttingloopfacing(obj.gets_length() - 2, 0, obj.getc_diameter() +
					5, obj.getFeed(), obj.getSpindlespeed());
				c_depth = c_depth - 2;
				obj.sets_length(obj.gets_length() - 2);
			}
		}
	}

	void turningcut(Article &obj, double depth_x, double depth_y)
	{
		//BlockStartLines(obj.gets_length() + 5, obj.getc_diameter() - depth_y, obj.getSpindlespeed(), 2);
		double step = depth_y / 2;
		double c_depth = depth_y;
		for (int i = 0; i <= step; ++i)
		{
			if (c_depth < 0)
			{
				break;
			}
			if (c_depth < 2)
			{
				BlockStartLines(obj.gets_length() + 5, obj.getc_diameter(),
					obj.getSpindlespeed(), 2);
				cuttingloop(obj, depth_x, c_depth);
				obj.setc_diameter(obj.getc_diameter() - c_depth);
			}
			else
			{
				BlockStartLines(obj.gets_length() + 5, obj.getc_diameter(),
					obj.getSpindlespeed(), 2);
				cuttingloop(obj, depth_x, 2);
				c_depth = c_depth - 2;
				obj.setc_diameter(obj.getc_diameter() - 2);
			}
		}
	}
	void taperturningcut(Article &obj, double x1, double y1, double x2, double y2)
	{
		BlockStartLines(obj.gets_length() + 5, y2, obj.getSpindlespeed(), 2);
		GCodeLine(0, 8, x2, y2, obj.getSpindlespeed(), 5, 2);
		N = N + 10;
		GCodeLine(1, 8, x1, y1, obj.getSpindlespeed(), obj.getFeed(), 2);
		N = N + 10;
		GCodeLine(0, 8, x1, y1 + 2, obj.getSpindlespeed(), 5, 2);
		N = N + 10;
		GCodeLine(0, 8, obj.gets_length() + 5, y1 + 2, obj.getSpindlespeed(), 5, 2);
		N = N + 10;
		myfile << endl;
	}
};
class Operations
{
private:
	Gcode abc;
	Article obj;
public:
	Operations() {}
	Operations(double a[], int length, double s_diameter, double
		s_length, double feed_rate, double spindle_speed)
	{
		Article temp(a, length, s_diameter, s_length, feed_rate, spindle_speed);
		obj = temp;
	}
	void facing()
	{
		abc.toolchange(1);
		double depth = obj.retFacingDepth();
		if (depth > 0)
		{
			abc.Facingcut(obj, depth);
		}
	}
	void turning()
	{
		abc.toolchange(2);
		double mind = obj.retturningDepthmin();
		while (mind < obj.getdiameter())
		{
			double maxd = obj.retturningDepthmax();
			printf("maxd = %lf\n", maxd);
			printf("mind = %lf\n", mind);
			double depth_x = obj.gets_length() - obj.indexofheight(maxd);
			double depth_y = obj.getdiameter() - maxd;
			if (depth_x == 0)
			{
				break;
			}
			abc.turningcut(obj, depth_x, depth_y);
			printf("c_diameter = %lf\n", obj.getdiameter());
			//call the function for turning.
			obj.setdiameter(maxd);
		}
	}
	void grooving() {}
	void taperturning()
	{
		int index = -1;
		while (true)
		{
			obj.indexofTaperStart(index);
			printf("%d", index);
			cout << endl;
			if (index == -2)
			{
				break;
			}
			cout << "hello" << index << endl;
			double x1, x2, y1, y2, x3, y3;
			obj.getPoints(x1, y1, x2, y2, index);
			double delta_y = 1;// (y1 - y2) / 10;
			double delta_x = 1;// (x2 - x1) / 10;
							   /*if (delta_y == 0)
							   {
							   delta_y = (y1 - y2) / 2;
							   }
							   if (delta_x == 0)
							   {
							   delta_x = (x2 - x1) / 2;
							   }*/
			x3 = x1;
			y3 = y1;
			abc.toolchange(2);
			abc.BlockStartLines(x2, y1 - delta_y, obj.getSpindlespeed(), 2);
			y1 = y1 - delta_y;
			x1 = x1 + delta_x;
			while (y1 >= y2)
			{
				abc.cuttingloopt(obj, x2, x2 - x1, delta_y, y1);
				y1 = y1 - delta_y;
				x1 = x1 + delta_x;
			}
			abc.toolchange(2);
			abc.BlockStartLines(obj.gets_length() + 5, y3, obj.getSpindlespeed(), 2);
			abc.taperturningcut(obj, x3, y3, x2, y2);
		}
	}
};
class Chuck {
protected:
	double speed;
	double holeY;
	int state;
public:
	double getSpeed() const;
	void setSpeed(int Speed);
	void draw() const;
	void rotate(double t);
	double getHoleY() const;
	void setHoleY(double HoleY);
	int getState() const;
	void setState(int State);
	void drawHole() const;
};
class Job {
public:
	int width, height;
	int xPosition[600];
	int yPosition[600];
	int getWidth() const;
	void setWidth(int Width);
	int getHeight() const;
	void setHeight(int Height);
	void setSize(int Width, int Height);
	void draw();
	void updateTurn(int x, int y);
	void updateFace(int x, int y, int xval);
};
class Tool {
	/*Class Tool is the parent function for all the different tool types
	used in the program.
	*/
protected:
	/*This class has three menber variables.viz x, y and feed.
	x and y variables describe the tool tip position with reference to
	the top left corner of the  screen(0, 0).
	feed variable stores the Feed value of the cut.
	*/
	int x, y;
	double feed;
public:
	Tool();    //Default constructor
	Tool(int X, int Y, double Feed);    //Initializing Constructor
	int getX() const; // Getter Functions
	int getY() const;
	double getFeed() const;
	void setX(int X);   //Setter Functions
	void setY(int Y);
	void setFeed(double Feed);
	virtual void Draw(int X, int Y);  // Draws tool on the screen
	virtual void Move(int &X, int &Y, int finalX, int finalY);
	//Updates tool position
};
class TurnTool : public Tool {
	/*Tool used for Turning Operation*/
public:
	void Draw(int X, int Y);
	void Move(int &X, int &Y, int finalX, int finalY);
};
class FaceTool : public Tool {
	/*Tool used for Facing Operation*/
public:
	void Draw(int X, int Y) const;
	void Move(int &X, int &Y, int finalX, int finalY);
};
//class TaperTool : public Tool {
// /*Tool used for Taper Turning Operation*/
//protected:
// int theta;
//
//public:
// int getTheta() const;
// void setTheta(int Theta);
// void Draw(int X, int Y) const;
// void Move(int Feed);
//};
//
//class GrooveTool : public Tool {
// /*Tool used for Grooving Operation*/
//public:
// void Draw(int X, int Y) const;
// void Move(int Feed);
//};
//
//class ThreadTool : public Tool {
//protected:
// double pitch;
// /*Tool used for Threading Operation*/
//
//public:
// double getPitch() const;
// void setPitch(double Pitch);
// void Draw(int X, int Y) const;
// void Move(int Feed);
//};
//class ChamferTool : public Tool {
// /*Tool used for Chamfering Operation*/
//public:
// void Draw(int X, int Y) const;
// void Move(int Feed);
//};
//
//class FilletTool : public Tool {
// /*Tool used for Fillet Operation*/
//public:
// void Draw(int X, int Y) const;
// void Move(int Feed);
//};
//
//class DrillTool : public Tool {
// /*Tool used for Drilling Operation*/
//public:
// void Draw(int X, int Y) const;
// void Move(int Feed);
//};
char *YsFgets(char str[], int maxn, FILE *fp);
void animate(int width, int height) {
	int px = 0, py = 0;
	int nx = 0, ny = 0;
	const int length = 8;
	int prevInput[length];
	int nextInput[length];
	int delta[length];
	for (int i = 0; i < length; i++) {
		prevInput[i] = 0;
		nextInput[i] = 0;
		delta[i] = 0;
	}
	int counter = 0;
	bool first = true;

	FILE *fp = fopen("GMcode.tap", "r");

	Chuck chuck;
	chuck.setHoleY(20);
	chuck.setState(1);
	Job job;
	job.setSize(width, height);
	double t = 0;
	TurnTool tt;
	FaceTool ft;
	Tool tool;
	int terminate = 0;
	int xval = width;
	//FsOpenWindow(16, 16, 800, 600, 1);
	char str[256];
	while (NULL != YsFgets(str, 255, fp)) {
		if (str[0] != 0)
		{
			istringstream iss(str);
			string sub;
			counter = 0;
			for (int i = 0; i < length; i++) {
				iss >> sub;
				sub[0] = ' ';
				if (first == true) {
					if (counter == 3) {
						prevInput[counter] = stoi(sub);
					}
					else if (counter == 4) {
						prevInput[counter] = stoi(sub);
					}
					else {
						prevInput[counter] = stoi(sub);
					}
					counter++;
				}
				else {
					if (counter == 3) {
						nextInput[counter] = stoi(sub);
					}
					else if (counter == 4) {
						nextInput[counter] = stoi(sub);
					}
					else {
						nextInput[counter] = stoi(sub);
					}
					counter++;
				}
			}
			cout << endl;
			if (first == false) {
				for (int i = 0; i < length; i++) {
					if (i == 3 || i == 4 || i == 6 || i == 7) {
						delta[i] = nextInput[i] - prevInput[i];
						cout << prevInput[i] << " " << nextInput[i] << " " << delta[i] << endl;
					}
				}
				terminate = 0;
				while (terminate != 1) {
					FsPollDevice();
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					chuck.draw();
					chuck.drawHole();
					chuck.rotate(t);
					t += 0.01;
					if (prevInput[7] == 1)
					{
						if (nextInput[6] == 1) //&& prevInput[6] == 5)
						{
							if (prevInput[3] < xval)
							{
								xval = prevInput[3];
							}
							job.updateFace(prevInput[3], prevInput[4], xval);

						}
					}
					if (prevInput[7] == 2)
					{
						if (nextInput[6] == 1 && prevInput[6] == 5)
						{
							job.updateTurn(prevInput[3], prevInput[4]);
						}
					}
					job.draw();
					if (prevInput[7] == 2)
					{
						tt.Draw(prevInput[3], prevInput[4]);
						tt.Move(prevInput[3], prevInput[4], nextInput[3], nextInput[4]);
					}
					else if (prevInput[7] == 1)
					{
						ft.Draw(prevInput[3], prevInput[4]);
						ft.Move(prevInput[3], prevInput[4], nextInput[3], nextInput[4]);
					}

					if (prevInput[3] == nextInput[3] && prevInput[4] == nextInput[4])
						terminate = 1;
					FsSwapBuffers();
					FsSleep(5);
				}

				for (int i = 0; i < length; i++) {
					prevInput[i] = nextInput[i];
				}
			}
			first = false;
		}
		else
		{
		}
	}
}


char *YsFgets(char str[], int maxn, FILE *fp) {
	if (nullptr != fgets(str, maxn, fp)) {
		int i;
		for (i = 0; 0 != str[i]; ++i) {
		}
		for (i = i; 0 <= i && 0 == isprint(str[i]); --i) {
			str[i] = 0;
		}
		return str;
	}
	return nullptr;
}
double Chuck::getSpeed() const { return speed; }
void Chuck::setSpeed(int Speed) { speed = Speed; }
void Chuck::draw() const {
	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);
	glColor3d(0.80, 0, 0);
	glVertex2i(0, 10);
	glColor3d(0.80, 0, 0);
	glVertex2i(0 + 70, 10);
	glColor3d(0.329412, 0, 0);
	glVertex2i(0 + 70, 10 + 580);
	glColor3d(0.329412, 0, 0);
	glVertex2i(0, 10 + 580);
	glEnd();
	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);
	glColor3d(0.80, 0, 0);
	glVertex2i(70, 90);
	glColor3d(0.80, 0, 0);
	glVertex2i(70 + 30, 90);
	glColor3d(0.329412, 0, 0);
	glVertex2i(70 + 30, 510);
	glColor3d(0.329412, 0, 0);
	glVertex2i(70, 510);
	glEnd();
}
void Chuck::rotate(double t) {
	int N = 100;
	holeY = 280 * sin((2 * YS_PI * N / 60) * t) + 300;
	if (holeY > 578.466) {
		this->setState(0);
	}
	if (holeY < 21.5339) {
		this->setState(1);
	}
}
double Chuck::getHoleY() const { return holeY; }
void Chuck::setHoleY(double HoleY) { holeY = HoleY; }
int Chuck::getState() const { return state; }
void Chuck::setState(int State) { state = State; }
void Chuck::drawHole() const {
	if (state == 1) {
		glColor3d(0, 0, 0);
		glBegin(GL_POLYGON);
		int i;
		for (i = 0; i < 64; i++)
		{
			double angle = (double)i*YS_PI / 32.0;
			double x = 35 + cos(angle) * 10;
			double y = holeY + sin(angle) * 10;
			glVertex2d(x, y);
		}
		glEnd();
	}
}

int Job::getWidth() const { return width; }
void Job::setWidth(int Width) { width = Width; }
int Job::getHeight() const { return height; }
void Job::setHeight(int Height) { height = Height; }
void Job::setSize(int Width, int Height) {
	this->setWidth(Width);
	this->setHeight(Height);
	for (int i = 0; i < 600; i++) {
		xPosition[i] = i + 100;
		//cout << xPosition[i] << " ";
		if (i < width) {
			yPosition[i] = 300 - height / 2;
			//cout << yPosition[i] << endl;
		}
		else {
			yPosition[i] = 300;
			//cout << yPosition[i] << endl;
		}
	}
}
void Job::draw() {
	for (int i = 0; i < 599; i++) {
		glShadeModel(GL_SMOOTH);
		glBegin(GL_POLYGON);
		glColor3d(0.329412, 0.329412, 0.329412);
		glVertex2i(xPosition[i], yPosition[i]);
		glColor3d(0.329412, 0.329412, 0.329412);
		glVertex2i(xPosition[i + 1], yPosition[i]);
		glColor3d(0.80, 0.80, 0.80);
		glVertex2i(xPosition[i + 1], 300 + (300 - yPosition[i]));
		glColor3d(0.80, 0.80, 0.80);
		glVertex2i(xPosition[i], 300 + (300 - yPosition[i]));
		glEnd();
	}
}
void Job::updateTurn(int x, int y)
{
	yPosition[x] = y + 300;
	yPosition[x + 1] = y + 300;

}
void Job::updateFace(int x, int y, int xval)
{
	yPosition[x] = y + 300;
	yPosition[x + 1] = y + 300;
	xPosition[x] = 100 + xval;
	//yPosition[x + 2] = y + 300;
}
Tool::Tool() {};
Tool::Tool(int X, int Y, double Feed) {};
int Tool::getX() const { return x; }
int Tool::getY() const { return y; }
double Tool::getFeed() const { return feed; }
void Tool::setX(int X) { x = X; }
void Tool::setY(int Y) { y = Y; }
void Tool::setFeed(double Feed) { feed = Feed; }
void Tool::Draw(int X, int Y) {
	X = 100 + X;
	Y = 300 - Y;
	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);
	glColor3d(0.329412, 0.329412, 0);
	glVertex2d(X, Y);
	glColor3d(0.329412, 0.329412, 0);
	glVertex2d(X + 49.809734904587276614750520123694, Y -
		4.3577871373829086779032135418737);
	glColor3d(0.80, 0.80, 0);
	glVertex2d(X + 49.809734904587276614750520123694, Y -
		49.809734904587276614750520123694);
	glColor3d(0.80, 0.80, 0);
	glVertex2d(X + 4.3577871373829086779032135418737, Y -
		49.809734904587276614750520123694);
	glEnd();
	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);
	glColor3d(0.329412, 0.329412, 0);
	glVertex2d(X + 4.3577871373829086779032135418737, Y -
		49.809734904587276614750520123694);
	glColor3d(0.329412, 0.329412, 0);
	glVertex2d(X + 49.809734904587276614750520123694, Y -
		49.809734904587276614750520123694);
	glColor3d(0.80, 0.80, 0);
	glVertex2d(X + 49.809734904587276614750520123694, 0);
	glColor3d(0.80, 0.80, 0);
	glVertex2d(X + 4.3577871373829086779032135418737, 0);
	glEnd();
}
void Tool::Move(int &X, int &Y, int finalX, int finalY) {
	if (X < finalX && Y < finalY) {
		X += 1; Y += 1;
	}
	else if (X > finalX && Y < finalY) {
		X -= 1; Y += 1;
	}
	else if (X > finalX && Y > finalY) {
		X -= 1; Y -= 1;
	}
	else if (X < finalX && Y > finalY) {
		X += 1; Y -= 1;
	}
	else if (X < finalX && Y == finalY) {
		X += 1;
	}
	else if (X > finalX && Y == finalY) {
		X -= 1;
	}
	else if (X == finalX && Y < finalY) {
		Y += 1;
	}
	else if (X == finalX && Y > finalY) {
		Y -= 1;
	}
	x = X;
	y = Y;
}
void TurnTool::Draw(int X, int Y) {
	X = 100 + X;
	Y = 300 - Y;
	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);
	glColor3d(0.329412, 0.329412, 0);
	glVertex2i(X, Y);
	glColor3d(0.329412, 0.329412, 0);
	glVertex2d(X + 49.809734904587276614750520123694, Y -
		4.3577871373829086779032135418737);
	glColor3d(0.80, 0.80, 0);
	glVertex2d(X + 49.809734904587276614750520123694, Y -
		49.809734904587276614750520123694);
	glColor3d(0.80, 0.80, 0);
	glVertex2d(X + 4.3577871373829086779032135418737, Y -
		49.809734904587276614750520123694);
	glEnd();
	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);
	glColor3d(0.80, 0.80, 0);
	glVertex2d(X + 4.3577871373829086779032135418737, Y -
		49.809734904587276614750520123694);
	glColor3d(0.80, 0.80, 0);
	glVertex2d(X + 49.809734904587276614750520123694, Y -
		49.809734904587276614750520123694);
	glColor3d(0.329412, 0.329412, 0);
	glVertex2d(X + 49.809734904587276614750520123694, 0);
	glColor3d(0.329412, 0.329412, 0);
	glVertex2d(X + 4.3577871373829086779032135418737, 0);
	glEnd();
}
void TurnTool::Move(int &X, int &Y, int finalX, int finalY) {
	if (X < finalX && Y < finalY) {
		X += 1; Y += 1;
	}
	else if (X > finalX && Y < finalY) {
		X -= 1; Y += 1;
	}
	else if (X > finalX && Y > finalY) {
		X -= 1; Y -= 1;
	}
	else if (X < finalX && Y > finalY) {
		X += 1; Y -= 1;
	}
	else if (X < finalX && Y == finalY) {
		X += 1;
	}
	else if (X > finalX && Y == finalY) {
		X -= 1;
	}
	else if (X == finalX && Y < finalY) {
		Y += 1;
	}
	else if (X == finalX && Y > finalY) {
		Y -= 1;
	}
	x = X;
	y = Y;
}
void FaceTool::Draw(int X, int Y) const {
	X = 100 + X;
	Y = 300 - Y;
	glShadeModel(GL_SMOOTH);
	glBegin(GL_POLYGON);
	glColor3d(0.80, 0.80, 0);
	glVertex2d(X + 4.3577871373829086779032135418737, Y -
		49.809734904587276614750520123694);
	glColor3d(0.329412, 0.329412, 0);
	glVertex2d(X + 49.809734904587276614750520123694, Y -
		49.809734904587276614750520123694);
	glColor3d(0.80, 0.80, 0);
	glVertex2d(X + 49.809734904587276614750520123694, Y -
		4.3577871373829086779032135418737);
	glColor3d(0.329412, 0.329412, 0);
	glVertex2d(800, Y - 4.3577871373829086779032135418737);
	glColor3d(0.329412, 0.329412, 0);
	glVertex2d(800, Y - 49.809734904587276614750520123694);
	glEnd();
	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);
	glColor3d(0.329412, 0.329412, 0);
	glVertex2i(X, Y);
	glColor3d(0.80, 0.80, 0);
	glVertex2d(X + 49.809734904587276614750520123694, Y -
		4.3577871373829086779032135418737);
	glColor3d(0.80, 0.80, 0);
	glVertex2d(X + 49.809734904587276614750520123694, Y -
		49.809734904587276614750520123694);
	glColor3d(0.329412, 0.329412, 0);
	glVertex2d(X + 4.3577871373829086779032135418737, Y -
		49.809734904587276614750520123694);
	glEnd();


}
void FaceTool::Move(int &X, int &Y, int finalX, int finalY) {
	if (X < finalX && Y < finalY) {
		X += 1; Y += 1;
	}
	else if (X > finalX && Y < finalY) {
		X -= 1; Y += 1;
	}
	else if (X > finalX && Y > finalY) {
		X -= 1; Y -= 1;
	}
	else if (X < finalX && Y > finalY) {
		X += 1; Y -= 1;
	}
	else if (X < finalX && Y == finalY) {
		X += 1;
	}
	else if (X > finalX && Y == finalY) {
		X -= 1;
	}
	else if (X == finalX && Y < finalY) {
		Y += 1;
	}
	else if (X == finalX && Y > finalY) {
		Y -= 1;
	}
	x = X;
	y = Y;
}


int main(void) {
	int count = 0, TextState = 1, trialX = 0, trialY = 0, state = 0,
		stateT = 1, terminate = 0, tstate = 0, erbx = 0, n = 0, f[20], k = 3,
		l = 0, m = 1, o = 0;
	double xf[20], yf[20], df[20], lf[20], af[20], p[100], q[100],
		r[100], F[20], t[100], fx, fy, pf[100], x = 0, y = 0;
	for (int i = 0; i < 100; i++)
	{
		p[i] = 0;
		q[i] = 0;
		r[i] = 0;
		pf[i] = 0;
	}
	for (int i = 0; i < 20; i++)
	{
		f[i] = 0;
		xf[i] = 0;
		yf[i] = 0;
		df[i] = 0;
		lf[i] = 0;
		af[i] = 0;
		F[i] = 0;
	}
	Graphics g;
	int a = 0, b = 0, d = 0, e = 0;
	double f1 = 0, f2 = 0, f3 = 0;
	FsOpenWindow(200, 0, 800, 600, 1);
	char s[256], s1[256], s2[256], s3[256];
	TextString str, str1;
	str.Set("");
	str1.Set("");
	auto key = FsInkey();
	while (0 == terminate)
	{
		FsPollDevice();
		int key = FsInkey();
		if (key == FSKEY_ENTER && stateT >= 1 && erbx == 0)
		{
			strcpy(s, str.GetPointer());
			strcpy(s2, str1.GetPointer());
			//printf("You entered: %s\n", s);
			stateT++;
		}
		else if (key == FSKEY_BS)
		{
			str.BackSpace();
			str1.BackSpace();
		}
		auto c = FsInkeyChar();
		if (' ' <= c)
		{
			str.Add(c);
			str1.Add(c);
		}
		int lb, rb, mb, mx, my;
		auto evt = FsGetMouseEvent(lb, mb, rb, mx, my);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// STOCK AND CNC MACHINE BOX HIGHLIGHT
		if (count == 0 && TextState == 1 && stateT == 1)
		{
		reenter1:;
			glColor3ub(255, 0, 0);
			glBegin(GL_QUADS);
			Highlight(400, 0, 200, 50);
			glColor3ub(0, 0, 0);
			glRasterPos2d(450, 35);
			a = 0;
			if (0 != time(nullptr) % 2)
			{
				str.Add('_');
			}
			else
			{
				str.Add('|');
			}
			YsGlDrawFontBitmap16x20(str.GetPointer());
			str.BackSpace();
			tstate = 1;
		}
		if (count == 0 && TextState == 1 && stateT == 2)
		{
			f1 = atof(s);
			if ((f1 > 350 || f1 <= 0) && tstate == 1)
			{
				DrawErrorBox(0);
				erbx = 1;
				if (FSMOUSEEVENT_LBUTTONDOWN == evt && mx >= 500 && mx <= 560 &&
					my >= 340 && my <= 360 && stateT == 2)
				{
					stateT = 1;
					while (a < 10)
					{
						str.BackSpace();
						if (a < 11)
							a++;
					}
					glColor3ub(0, 0, 0);
					glRasterPos2d(450, 35);
					erbx = 0;
					goto reenter1;
				}
			}
			else
			{
				if (b < 1)
				{
					strcpy(s1, s);
					f1 = atof(s1);
					df[n] = f1;
					y = df[n] / 2;
					b++;
				}

				glColor3ub(0, 0, 0);
				glRasterPos2d(450, 35);
				YsGlDrawFontBitmap16x20(s1);
			reenter2:;
				glColor3ub(255, 0, 0);
				glBegin(GL_QUADS);
				Highlight(600, 0, 150, 50);
				if (a < 10)
				{
					str1.BackSpace();
					if (a < 11)
						a++;
				}
				glColor3ub(0, 0, 0);
				glRasterPos2d(650, 35);
				if (0 != time(nullptr) % 2)
				{
					str1.Add('_');
				}
				else
				{
					str1.Add('|');
				}
				YsGlDrawFontBitmap16x20(str1.GetPointer());
				str1.BackSpace();
				tstate = 2;
			}
		}
		if (count == 0 && TextState == 1 && stateT == 3) //reset
		{
			f2 = atof(s2);
			if (f2 > 600 || f2 <= 0)
			{
				DrawErrorBox(1);
				erbx = 1;
				if (FSMOUSEEVENT_LBUTTONDOWN == evt && mx >= 500 && mx <= 560 &&
					my >= 340 && my <= 360 && stateT == 3)
				{
					stateT = 2;
					while (a < 10)
					{
						str.BackSpace();
						if (a < 11)
							a++;
					}
					glColor3ub(0, 0, 0);
					glRasterPos2d(650, 35);
					printf("%d \n", stateT);
					a = 0;
					erbx = 0;
					goto reenter2;
				}
			}
			else
			{
				b = 0;
				if (b < 1)
				{
					strcpy(s3, s2);
					f1 = atof(s3);
					lf[n] = f2;
					x = lf[n];
					b++;
				}
				lf[n] = f2;
				x = lf[n];
				for (int c = 0; c<10; c++)
				{
					str1.BackSpace();
					str.BackSpace();
				}
				a = 0;
				b = 0;
				count = 0;
				stateT = 0;
				TextState = 0;
				f1 = 0;
				f2 = 0;
				p[0] = 0;
				p[1] = df[n] / 2;
				p[2] = lf[n];
				p[3] = df[n] / 2;
				p[4] = lf[n];
				p[5] = 0;
				r[0] = lf[n];
				r[1] = df[n] / 2;
				fx = lf[n];
				fy = df[n] / 2;
			}
		}
		if (TextState == 0)
		{
			glRasterPos2d(630, 557);
			YsGlDrawFontBitmap16x20("o");
			glRasterPos2d(631, 562);
			YsGlDrawFontBitmap16x24("|");
			glRasterPos2d(650, 560);
			YsGlDrawFontBitmap16x20(s1);
			glRasterPos2d(701, 560);
			YsGlDrawFontBitmap16x20("X");
			glRasterPos2d(720, 560);
			YsGlDrawFontBitmap16x20(s3);
		}
		if (TextState == 0 && state == 1)
		{
			for (int c = 0; c<10; c++)
			{
				str.BackSpace();
			}
		}
		//TURNING BOX HIGHLIGHT
		if (FSMOUSEEVENT_LBUTTONDOWN == evt && mx >= 600 && my >= 225 && my
			<= 350 && TextState == 0 && state == 0 && stateT == 0)
		{
			count = 1;
			state = 1;
			n++;
			f[n] = 1;
		}
		if (count == 1)
		{
			glColor3ub(0, 255, 0);
			glBegin(GL_QUADS);
		}
		else
		{
			glColor3ub(0, 0, 0);
			glBegin(GL_LINE_LOOP);
		}
		Highlight(600, 225, 200, 125);

		// FACING BOX HIGHLIGHT
		if (FSMOUSEEVENT_LBUTTONDOWN == evt && mx >= 600 && my <= 225 && my
			>= 100 && TextState == 0 && state == 0 && stateT == 0)
		{
			count = 2;
			stateT = 2;
			n++;
			f[n] = 2;
			xf[n] = fx;
			yf[n] = fy;
		}
		if (count == 2)
		{
			glColor3ub(0, 255, 0);
			glBegin(GL_QUADS);
		}
		else
		{
			glColor3ub(0, 0, 0);
			glBegin(GL_LINE_LOOP);
		}
		Highlight(600, 100, 200, 125);
		// TAPER TURNING BOX HIGHLIGHT
		if (FSMOUSEEVENT_LBUTTONDOWN == evt && mx >= 600 && my >= 350 && my
			<= 475 && TextState == 0 && state == 0 && stateT == 0)
		{
			count = 3;
			state = 1;
			n++;
			f[n] = 3;
		}
		if (count == 3)
		{
			glColor3ub(0, 255, 0);
			glBegin(GL_QUADS);
		}
		else
		{
			glColor3ub(0, 0, 0);
			glBegin(GL_LINE_LOOP);
		}
		Highlight(600, 350, 200, 125);
		// PARTING BOX HIGHLIGHT
		if (FSMOUSEEVENT_LBUTTONDOWN == evt && mx >= 600 && my >= 475 && my
			<= 600 && TextState == 0 && state == 0 && stateT == 0)
		{
			count = 4;
			state = 1;
			n++;
			f[n] = 4;
		}
		if (count == 4)
		{
			glColor3ub(0, 255, 0);
			glBegin(GL_QUADS);
		}
		else
		{
			glColor3ub(0, 0, 0);
			glBegin(GL_LINE_LOOP);
		}
		Highlight(600, 475, 200, 125);
		// TERMINATE BY CHECK BOX
		if (FSMOUSEEVENT_LBUTTONDOWN == evt && mx >= 750 && my >= 0 && my <=
			50 && TextState == 0 && state == 0 && stateT == 0)
		{
			glColor3ub(0, 255, 0);
			glBegin(GL_QUADS);
			Highlight(750, 0, 50, 50);
			terminate = 1;
		}
		// COORDINATE BOX COLOR RED
		if ((count != 0 || count != 2) && state == 1)
		{
			glColor3ub(255, 0, 0);
			glBegin(GL_QUADS);
			Highlight(0, 50, 400, 50);
			for (int i = 0; i < 100; i += 2)
			{
				if (check(mx, my, r[i], r[i + 1]) == 1 && FSMOUSEEVENT_LBUTTONDOWN == evt)
				{
					xf[n] = r[i];
					yf[n] = r[i + 1];
					//printf("%d %d %d %d\n", mx, my, x, y);
					stateT = 1;
					state = 2;
				}
			}
			/*else if (mx < 55 + sl && mx > 45 + sl && my > 545 - sd / 2 && my
			< 555 - sd / 2 && FSMOUSEEVENT_LBUTTONDOWN == evt && count == 2)
			{
			mx = 550 - sd / 2;
			my = 50 + sl;
			trialX = mx;
			trialY = my;
			printf("%d %d \n", trialX, trialY);
			stateT = 3;
			state = 3;
			}*/
			//printf("%d \n", state);
		}
		//COORD BOX GREY
		if (count == 2)
		{
			glColor3ub(169, 169, 169);
			glBegin(GL_QUADS);
			Highlight(0, 50, 400, 50);
		}
		//Text Input when Turning tab is selected
		if (count == 1 && stateT == 1)
		{
			glColor3ub(255, 0, 0);
			glBegin(GL_QUADS);
			Highlight(400, 0, 200, 50);
			glColor3ub(0, 0, 0);
			glRasterPos2d(450, 35);
			if (0 != time(nullptr) % 2)
			{
				str.Add('_');
			}
			else
			{
				str.Add('|');
			}
			YsGlDrawFontBitmap16x20(str.GetPointer());
			str.BackSpace();
		}

		if (count == 1 && stateT == 2)
		{
			f1 = atof(s);
			df[n] = f1;
			glColor3ub(255, 0, 0);
			glBegin(GL_QUADS);
			Highlight(600, 0, 150, 50);

			glColor3ub(0, 0, 0);
			glRasterPos2d(450, 35);
			YsGlDrawFontBitmap16x20(s);
			if (a<10)
			{
				str.BackSpace();
				if (a<11)
					a++;
			}
			glColor3ub(0, 0, 0);
			glRasterPos2d(650, 35);
			if (0 != time(nullptr) % 2)
			{
				str.Add('_');
			}
			else
			{
				str.Add('|');
			}
			YsGlDrawFontBitmap16x20(str.GetPointer());
			str.BackSpace();
		}
		if (count == 1 && stateT == 3) //reset
		{
			f2 = atof(s);
			lf[n] = f2;
			for (int c = 0; c<10; c++)
			{
				str.BackSpace();
			}
			a = 0;
			count = 0;
			stateT = 0;
			state = 0;
			p[2 * k] = xf[n] - lf[n];
			p[2 * k + 1] = yf[n];
			p[2 * k + 2] = xf[n] - lf[n];
			p[2 * k + 3] = df[n] / 2;
			p[2 * k + 4] = xf[n];
			p[2 * k + 5] = df[n] / 2;
			q[2 * l] = xf[n];
			q[2 * l + 1] = yf[n];
			r[2 * m] = xf[n] - lf[n];
			r[2 * m + 1] = yf[n];
			r[2 * m + 2] = xf[n];
			r[2 * m + 3] = df[n] / 2;
			t[2 * o] = xf[n];
			t[2 * o + 1] = yf[n];
			m += 2;
			n++;
			k += 3;
			l += 1;
			if (yf[n] == fy && xf[n] == fx)
			{
				fy = df[n] / 2;
			}
		}
		// //Text Input when FACING tab is selected
		if (count == 2 && stateT == 2)
		{
			glColor3ub(255, 0, 0);
			glBegin(GL_QUADS);
			Highlight(600, 0, 150, 50);

			glColor3ub(0, 0, 0);
			glRasterPos2d(650, 35);
			if (0 != time(nullptr) % 2)
			{
				str.Add('_');
			}
			else
			{
				str.Add('|');
			}
			YsGlDrawFontBitmap16x20(str.GetPointer());
			str.BackSpace();
		}
		if (count == 2 && stateT == 3) //reset
		{
			f2 = atof(s);
			lf[n] = f2;
			for (int c = 0; c<10; c++)
			{
				str.BackSpace();
			}
			a = 0;
			count = 0;
			stateT = 0;
			state = 0;
			p[2 * k] = xf[n] - lf[n];
			p[2 * k + 1] = yf[n];
			p[2 * k + 2] = xf[n] - lf[n];
			p[2 * k + 3] = 0;
			q[2 * l] = xf[n];
			q[2 * l + 1] = yf[n];
			q[2 * l + 2] = xf[n];
			q[2 * l + 3] = 0;
			k += 2;
			l += 2;
			r[2 * m] = xf[n] - lf[n];
			r[2 * m + 1] = yf[n];
			t[2 * o] = xf[n];
			t[2 * o + 1] = yf[n];
			m += 1;
			n++;
			fx = fx - lf[n];
		}
		//Text Input when Taper-Turning tab is selected
		if (count == 3 && stateT == 1)
		{
			glColor3ub(255, 0, 0);
			glBegin(GL_QUADS);
			Highlight(400, 50, 200, 50);
			glColor3ub(0, 0, 0);
			glRasterPos2d(508, 85);
			if (0 != time(nullptr) % 2)
			{
				str.Add('_');
			}
			else
			{
				str.Add('|');
			}
			YsGlDrawFontBitmap16x20(str.GetPointer());
			str.BackSpace();
		}

		if (count == 3 && stateT == 2)
		{
			glColor3ub(255, 0, 0);
			glBegin(GL_QUADS);
			Highlight(600, 0, 150, 50);
			f1 = atof(s);
			af[n] = f1;
			glColor3ub(0, 0, 0);
			glRasterPos2d(508, 85);
			YsGlDrawFontBitmap16x20(s);
			if (a<10)
			{
				str.BackSpace();
				if (a<11)
					a++;
			}
			glColor3ub(0, 0, 0);
			glRasterPos2d(650, 35);
			if (0 != time(nullptr) % 2)
			{
				str.Add('_');
			}
			else
			{
				str.Add('|');
			}
			YsGlDrawFontBitmap16x20(str.GetPointer());
			str.BackSpace();
		}
		if (count == 3 && stateT == 3) //reset
		{
			f2 = atof(s);
			lf[n] = f2;
			for (int c = 0; c<10; c++)
			{
				str.BackSpace();
			}
			a = 0;
			count = 0;
			stateT = 0;
			state = 0;
			p[2 * k] = xf[n] - lf[n];
			p[2 * k + 1] = yf[n];
			p[2 * k + 2] = xf[n];
			p[2 * k + 3] = yf[n] - lf[n] * tan(YS_PI*af[n] / 180);
			q[2 * l] = xf[n];
			q[2 * l + 1] = yf[n];
			k += 2;
			l += 1;
			r[2 * m] = xf[n] - lf[n];
			r[2 * m + 1] = yf[n];
			t[2 * o] = xf[n];
			t[2 * o + 1] = yf[n];
			m++;
			n++;
		}

		//TAKE INPUT OF G CODE AND M CODE AFTER HERE
		//DIA GREY
		if ((count == 2 || count == 3))
		{
			glColor3ub(169, 169, 169);
			glBegin(GL_QUADS);
			Highlight(400, 0, 200, 50);
		}
		//ANGLE BOX COLOR
		if (count != 3 && count != 0)
		{
			glColor3ub(169, 169, 169);
			glBegin(GL_QUADS);
			Highlight(400, 50, 200, 50);
		}
		// OFFSET BOX COLOR
		if (count != 4 && count != 0)
		{
			glColor3ub(169, 169, 169);
			glBegin(GL_QUADS);
			Highlight(600, 50, 200, 50);
		}
		// TOP LEFT BOX TEXT
		if (TextState == 1)
		{
			glColor3ub(0, 255, 0);
			glBegin(GL_QUADS);
			Highlight(0, 0, 400, 50);
			glColor3ub(0, 0, 0);
			glRasterPos2d(100, 35);
			YsGlDrawFontBitmap16x20("ENTER STOCK");
		}
		else if (TextState == 0)
		{
			glColor3ub(255, 165, 0);
			glBegin(GL_QUADS);
			Highlight(0, 0, 400, 50);
			glColor3ub(0, 0, 0);
			glRasterPos2d(100, 35);
			YsGlDrawFontBitmap16x20("CNC MACHINE");
		}
		for (int i = 0; i < 100; i += 2)
		{
			for (int j = 0; j < 100; j += 2)
			{
				if (p[i] == q[j] && p[i + 1] == q[j + 1])
				{
					p[i] = 0;
					p[i + 1] = 0;
				}
			}
		}
		for (int i = 0; i < 100; i += 2)
		{
			for (int j = 0; j < 100; j += 2)
			{
				if (r[i] == t[j] && r[i + 1] == t[j + 1])
				{
					r[i] = 0;
					r[i + 1] = 0;
				}
			}
		}
		g.drawStock(lf[0], df[0]);
		for (int i = 0; i < 20; i++)
		{
			g.drawFeature(i, f[i], xf[i], yf[i], df[i], lf[i], af[i]);
		}

		g.initialize();
		FsSwapBuffers();
		FsSleep(25);
	}
	int pts = 0;
	for (int i = 0; i < 100; i += 2)
	{
		if (p[i] == 0 && p[i + 1] == 0)
		{
		}
		else
		{
			pf[pts] = p[i];
			pf[pts + 1] = p[i + 1];
			pts += 2;
		}
	}
	/*
	for (int i = 0; i < pts; i += 2)
	{
	for (int j = i + 2; j < pts; j += 2)
	{
	if (pf[i] == pf[j])
	{
	Swap(pf[i + 2], pf[j]);
	Swap(pf[i + 3], pf[j + 1]);
	}
	if (pf[i + 1] == pf[j + 1])
	{
	Swap(pf[i + 2], pf[j]);
	Swap(pf[i + 3], pf[j + 1]);
	}
	}
	}
	*/
	for (int i = 0; i < pts; i += 2)
	{
		for (int j = i + 2; j < pts; j += 2)
		{
			if (pf[i] > pf[j])
			{
				swap(pf[i], pf[j]);
				swap(pf[i + 1], pf[j + 1]);
			}
		}
	}
	for (int i = 0; i < pts + 2; i += 2)
	{
		printf(" Swapped %lf , %lf \n", pf[i], pf[i + 1]);
	}
	for (int i = 0; i < pts; i += 2)
	{
		if (pf[i] == pf[i + 2])
		{
			if (pf[i + 1] < pf[i + 3])
			{
				Swap(pf[i + 1], pf[i + 3]);
			}
		}
	}
	/*
	for (int i = (pts/2); i < pts; i += 2)
	{
	for (int j = i + 2; j < pts; j += 2)
	{
	Swap(pf[i], pf[j]);
	Swap(pf[i + 1], pf[j + 1]);
	}
	}
	*/
	for (int i = 0; i < pts; i += 2)
	{
		printf("%lf\t %lf\n", pf[i], pf[i + 1]);
	}
	//FsCloseWindow();
	double arr[100];
	for (int i = 0; i < 100; i++)
	{
		arr[i] = 0;
	}
	printf(" %d \n", pts);
	arr[0] = 0;
	arr[1] = 0;
	for (int i = 0; i < pts; i++)
	{
		arr[i + 2] = pf[i];
	}
	double width = lf[0];
	double height = df[0];
	Operations A(arr, (pts + 2) / 2, height / 2, width, 1, 50);
	A.facing();
	A.turning();
	A.taperturning();
	animate(width, height);
	while (true)
	{
		FsPollDevice();
		auto flag = FsInkey();
		if (flag == FSKEY_ESC)
		{
			break;
		}
		if (flag == FSKEY_R)
		{
			animate(width, height);
		}
	}
	return 0;
}
