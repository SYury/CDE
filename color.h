#pragma once

class Color{
	double r, g, b;
	public:
	Color(double _r, double _g, double _b):r(_r),g(_g),b(_b){}
	Color(int _r, int _g, int _b):r(_r/255.0),g(_g/255.0),b(_b/255.0){}
	double getR()const{return r;}
	double getG()const{return g;}
	double getB()const{return b;}
};

extern const Color greenColor;
extern const Color grayColor;
extern const Color yellowColor;
extern const Color lightBlueColor;
extern const Color whiteColor;
extern const Color redColor;
extern const Color blueColor;
extern const Color magentaColor;
extern const Color crimsonColor;
extern const Color ultraLightYellowColor;
