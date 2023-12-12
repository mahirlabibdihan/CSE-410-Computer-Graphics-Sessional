
#ifdef __linux__
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

#include "CollisionManager.hpp"
#include "Ball.hpp"
#include "Wall.hpp"
#include "GLM.hpp"
#include "Vec3.hpp"
#include "DGraphics.hpp"