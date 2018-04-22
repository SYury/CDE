#pragma once

#include<bits/stdc++.h>
#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

#define die(...) fprintf(stderr, __VA_ARGS__),fflush(stderr),exit(-1)
#define debug(...) fprintf(stderr, __VA_ARGS__),fflush(stderr)

inline int dist(int x, int y, int nx, int ny){return abs(x - nx) + abs(y - ny);}
