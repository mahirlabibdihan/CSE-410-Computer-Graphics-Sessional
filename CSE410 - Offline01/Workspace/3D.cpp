
#ifdef __linux__
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <stdio.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define PI 3.14159265358979324
#define xyz(r, a, t) r *cos(a) * cos(t), r *cos(a) * sin(t), r *sin(a)
#define color(R, G, B) .3 + R / (1.3 * r), .3 + G / (1.3 * r), .3 + B / (1.3 * r)
GLint WindowHeight = 600;
GLint WindowWidth = 600;
int angle = 0;

int dir_angle = 30;
double ball_rot = 0, ball_rot_x = 0, ball_rot_y = 0;
double ball_x = 0, ball_y = 0;
double ROT_ANG = PI/180;
struct PT
{
	PT() {}
	double x,y,z;

	PT(double x, double y, double z) : x(x), y(y), z(z) {}
    PT(const PT &p) : x(p.x), y(p.y), z(p.z) {}

	// arithemtic operations
	PT operator +(PT b)  {return PT(x+b.x,y+b.y, z+b.z);}
    PT operator -(PT b)  {return PT(x-b.x,y-b.y, z-b.z);}
	PT operator *(double b)  {return PT(x*b,y*b, z*b);}
	PT operator /(double b)  {return PT(x/b,y/b, z/b);}
	PT operator *(PT b)  {return PT(y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x);}
};

void drawCheckerboard() {
    int size = 100;
    for (int i = -size; i < size; i+=2) {
        for (int j = -size; j < size; j+=2) {
            if ((i + j)/2 % 2 == 0) {
                glColor3f(1.0, 1.0, 1.0);  // White
            } else {
                glColor3f(0.0, 0.0, 0.0);  // Black
            }

            glBegin(GL_QUADS);
            glVertex2f(i, j);
            glVertex2f(i + 2, j);
            glVertex2f(i + 2, j + 2);
            glVertex2f(i, j + 2);
            glEnd();
        }
    }
}


void drawSquare(double a)
{
    glBegin(GL_QUADS);
    {
        // Anticlock
        glColor4f(1, 0, 0, 1);
        glVertex2f(a, a);
        glColor4f(0, 1, 0, 1);
        glVertex2f(a, -a);
        glColor4f(0, 0, 1, 1);
        glVertex2f(-a, -a);
        glColor4f(1, 0, 1, 1);
        glVertex2f(-a, a);
    }
    glEnd();
    glBegin(GL_LINES);
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(a, a);
        glVertex2f(a, -a);
        glVertex2f(-a, -a);
        glVertex2f(-a, a);
    }
    glEnd();
}

void drawAxis()
{
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(5, 0, 0);
        glVertex3f(-5, 0, 0);
        glVertex3f(0.0f,5, 0);
        glVertex3f(0.0f, -5, 0);
        glVertex3f(0.0f, 0, 5);
        glVertex3f(0.0f, 0, -5);
    }
    glEnd();
}
void drawCube() {
    // Front face
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);  // Red
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glEnd();

    // Back face
    glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 0.0);  // Green
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glEnd();

    // Connect front and back faces with lines
    glColor3f(0.0, 0.0, 0.0);  // Black
    glBegin(GL_LINES);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(-1.0, -1.0, -1.0);

    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);

    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0);

    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glEnd();
}


int stacks = 5, sectors = 3;

int r = 300;

void circle(int x, int y, int r)
{
	GLfloat t, dt = 2 * PI / 100;
	glBegin(GL_LINE_STRIP);
	glLineWidth(3.0);
	// Draw circle
	for (t = 0; t <= 2 * PI + dt; t += dt)
	{
		glVertex2f(x + r * cos(t), y + r * sin(t));		// Formula of circle
	}
	glEnd();
}


void drawSphere(double radius,int slices,int stacks)
{
	struct PT points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(PI/2));
		r=radius*cos(((double)i/(double)stacks)*(PI/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*PI);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*PI);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(0.5+(double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        // glColor3f(1,1,1);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void sphere()
{
	glPushMatrix();
	glRotatef(angle, 1, 0 , 0);
	glRotatef(angle, 0, 1, 0);
	glRotatef(angle, 0, 0, 1);
	GLdouble t, dt = 2 * PI / stacks;
	for (t = 0; t <= 2 * PI ; t += dt)
	{
		GLfloat x = r * cos(t), y = r * sin(t);
		glPushMatrix();
		glRotatef(atan(y / x) * 180 / PI, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		glColor3f(0.0, 0.0, 0.0);
		circle(0, 0, r);
		glPopMatrix();
	}

	dt = 2 * PI / sectors;
	for (t = 0; t <= 2 * PI ; t += dt)
	{
		glPushMatrix();
		glTranslatef(0, sin(t)*r, 0);
		glRotatef(90, 1, 0, 0);
		glColor3f(0.0, 0.0, 0.0);
		circle(0, 0, sqrt(pow(r, 2.0) - pow(sin(t)*r, 2.0)));
		glPopMatrix();
	}


	glColor3f(1.0, 1.0, 0.0);

	GLfloat a, da = 2 * PI / sectors;
	for (a = 0; a <= 2 * PI ; a += da)
	{
		GLdouble t, dt = 2 * PI / stacks;
		for (t = 0; t <= 2 * PI ; t += dt)
		{
			GLfloat vertex[4][3] = {
				{ xyz(r, a, t) },
				{ xyz(r, a, t + dt) },
				{ xyz(r, a + da, t + dt) },
				{ xyz(r, a + da, t) }
			};

			glBegin(GL_POLYGON);
			// glColor3f(color(vertex[0][0],vertex[0][1],vertex[0][2]));
			for (int i = 0; i < 4; i++)
			{	
				glColor3f(color(vertex[i][0],vertex[i][1],vertex[i][2]));
				glVertex3fv(vertex[i]);
			}
			glEnd();
		}
	}
	glPopMatrix();
}

void drawSphereQuad(double radius,int slices,int stacks, double R, double G, double B)
{
	struct PT points[100][100];
	int i,j;
	double h,r;
	//generate points
    GLfloat a, da = 2 * PI / stacks;
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/stacks)*(PI/2));
		r=radius*cos(((double)i/stacks)*(PI/2));
		for(j=0;j<6;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*PI);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*PI);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        // glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        glColor3f(R,G,B);
		for(j=0;j<5;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                // //lower hemisphere
                // glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				// glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				// glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				// glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


// position of camera
PT pos(8, 8, 10);
// up, right and look direction
PT up(0,0,1);
PT right(0,1,0);
PT look(-8,-8,-10);

void wall()
{
    glBegin(GL_QUADS);
    {
        glColor4f(1, 0, 0, 1);
        glVertex3f(+6, 0, 0.0);
        glVertex3f(+6, 0, 0.5);
        glVertex3f(-6, 0, 0.5);
        glVertex3f(-6, 0, 0.0);
    }
    glEnd();
}

void ball()
{
    // glPushMatrix();
    // glTranslatef(0 , 0, 0.5);
    glPushMatrix();
    drawSphereQuad(0.5,41,40,1,0,0);
    glRotatef(45,0,0,1);
    drawSphereQuad(0.5,41,40,0,1,0);
    glRotatef(45,0,0,1);
    drawSphereQuad(0.5,41,40,1,0,0);
    glRotatef(45,0,0,1);
    drawSphereQuad(0.5,41,40,0,1,0);
    glRotatef(45,0,0,1);
    drawSphereQuad(0.5,41,40,1,0,0);
    glRotatef(45,0,0,1);
    drawSphereQuad(0.5,41,40,0,1,0);
    glRotatef(45,0,0,1);
    drawSphereQuad(0.5,41,40,1,0,0);
    glRotatef(45,0,0,1);
    drawSphereQuad(0.5,41,40,0,1,0);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180,1,0,0);
    glPushMatrix();
    drawSphereQuad(0.5,41,40,1,0,0);
    glRotatef(45,0,0,1);
    drawSphereQuad(0.5,41,40,0,1,0);
    glRotatef(45,0,0,1);
    drawSphereQuad(0.5,41,40,1,0,0);
    glRotatef(45,0,0,1);
    drawSphereQuad(0.5,41,40,0,1,0);
    glRotatef(45,0,0,1);
    drawSphereQuad(0.5,41,40,1,0,0);
    glRotatef(45,0,0,1);
    drawSphereQuad(0.5,41,40,0,1,0);
    glRotatef(45,0,0,1);
    drawSphereQuad(0.5,41,40,1,0,0);
    glRotatef(45,0,0,1);
    drawSphereQuad(0.5,41,40,0,1,0);
    glPopMatrix();
    glPopMatrix();
    // glPopMatrix();
}

double ball_r = 0;
void drawArrow() {
    // Draw arrow body
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(1.0f, 0.2f);
    glVertex2f(1.0f, -0.2f);
    glEnd();

    // Draw arrowhead
    glBegin(GL_TRIANGLES);
    glVertex2f(1.0f, 0.1f);
    glVertex2f(1.2f, 0.0f);
    glVertex2f(1.0f, -0.1f);
    glEnd();
}

const int numSegments = 50;
void drawCylinder(float radius, float height) {
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= numSegments; ++i) {
        float theta = 2.0 * PI * static_cast<float>(i) / numSegments;
        float x = radius * cos(theta);
        float y = radius * sin(theta);

        glVertex3f(x, y, 0.0);
        glVertex3f(x, y, height);
    }
    glEnd();
}

void drawCone(float radius, float height) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0, 0.0, height); // Apex
    for (int i = 0; i <= numSegments; ++i) {
        float theta = 2.0 * PI * static_cast<float>(i) / numSegments;
        float x = radius * cos(theta);
        float y = radius * sin(theta);

        glVertex3f(x, y, 0.0);
    }
    glEnd();
}

void display()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Ensures that each time when we enter the projection mode, the matrix will be reset to identity matrix
	gluLookAt(pos.x, pos.y, pos.z, 
			pos.x + look.x, pos.y + look.y, pos.z + look.z,
			up.x, up.y, up.z);
    // gluLookAt(
    //     -8, -8, 10,  // EYE: Location of camera
    //     0, 0, 0, // AT: direction where the camera is aiming at. 
    //     0, 0, 1 // UP: upward orientation of the camera rougly. typically coincided with y axis
    // );
    

    // Clear all pixels
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    drawCheckerboard();
    double x[] = {0.5, 0.5, -0.5, -0.5};
    double y[] = {0.5, -0.5, -0.5, 0.5};
    // for(int i=0;i<4;i++)
    // {
    //     glPushMatrix();
    //     // glTranslatef(x[i],y[i],0.0f);
    //     glRotatef(90*i, 1, 0, 0);
    //     glTranslatef(0, 0, 2);
        
    //     drawSquare(2);
    //     // drawAxis();
    //     // drawCube();
    //     glPopMatrix();
    // }
    drawAxis();
    // for(int i=0;i<4;i++)
    // {
    //     glPushMatrix();
    //     // glTranslatef(x[i],y[i],0.0f);
    //     glRotatef(90*i, 0, 1, 0);
    //     glTranslatef(0, 0, 2);
        
    //     drawSquare(2);
    //     // drawAxis();
    //     // drawCube();
    //     glPopMatrix();
    // }   

    double axis = dir_angle - 90;
    glPushMatrix();
    glTranslatef(ball_x, ball_y, 0);
    glPushMatrix();
    glTranslatef(0, 0, 0.5);
    glRotatef(ball_rot, ball_rot_x, ball_rot_y, 0);
    ball();
    glPopMatrix();
    glPopMatrix();

    for(int i=0;i<4;i++)
    {
        glPushMatrix();
        glRotatef(90*i,0,0,1);
        glTranslatef(0,6,0);
        wall();
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(ball_x, ball_y, 0);
    glRotatef(dir_angle, 0, 0, 1);
    glPushMatrix();
    glTranslatef(0, 0, 0.5);
    glRotatef(90, 0, 1, 0);

    glColor3f(0.0, 0.0, 1.0); // Red color for the cylinder
    drawCylinder(0.05, 2.0);

    glColor3f(0.0, 0.0, 1.0); // Blue color for the cone
    glTranslatef(0.0, 0.0, 2.0); // Move the cone up to connect with the cylinder
    drawCone(0.1, 1.0);
    glPopMatrix();
    glPopMatrix();
    glutSwapBuffers();
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h); // resizing the window

    // Setup viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    WindowWidth = w;
    WindowHeight = h;
    // L      R     B    T      N       F
    // glOrtho(-WindowWidth / 2, WindowWidth / 2, -WindowHeight / 2, WindowHeight / 2, -WindowWidth / 2, WindowWidth / 2);
    gluPerspective(
        60, // fov
        1.0*w/h, // aspect
        // 1,
        1, 
        200);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void update(int time)
{
    angle = (angle + 1) % 360;
    // printf("%d\n", dir_angle);
    glutPostRedisplay();
    glutTimerFunc(time, update, time);
}

void rotate3D(PT &vec,PT &axis,double ang)
{
	// vec = vec*cos(ang)+(vec*axis)*sin(ang);
	vec = vec*cos(ang)+(axis*vec)*sin(ang);
}
bool last_i = true;
void key(unsigned char key, int, int)
{
    // if (key == 27) glutLeaveMainLoop();
	if (key == 'a') if (stacks > 0)stacks-=1;
	if (key == 'd') stacks+=1;
	if (key == 'w') sectors+=1;
	if (key == 's') if (sectors > 0)sectors-=1;
    if (key == 27)
        glutLeaveMainLoop();
    switch(key)
    {
        case '1':
			// rotate LEFT 
			rotate3D(right,up,ROT_ANG);
			rotate3D(look,up,ROT_ANG);
			break;
		case '2':
			// rotate right
			rotate3D(right,up,-ROT_ANG);
			rotate3D(look,up,-ROT_ANG);
			break;
		case '3':
			// rotate UP
			rotate3D(up,right,ROT_ANG);
			rotate3D(look,right,ROT_ANG);
			break;
		case '4':
			// rotate DOWN
			rotate3D(up,right,-ROT_ANG);
			rotate3D(look,right,-ROT_ANG);
			break;
		case '5':
			// tilt CLKWISE
			rotate3D(right,look,ROT_ANG);
			rotate3D(up,look,ROT_ANG);
			break;
		case '6':
			// tilt COUNTER CLKWISE
			rotate3D(right,look,-ROT_ANG);
			rotate3D(up,look,-ROT_ANG);
			break;
        
        case 'i':
            ball_r += 0.1;
            if(!last_i) 
            {
                dir_angle = (180 + dir_angle) % 360;
                last_i = true;
            }
            ball_rot = ball_rot + 5.0;
            ball_rot_x = -cos((dir_angle-90)*PI/180);
            ball_rot_y = -sin((dir_angle-90)*PI/180);
            ball_x += 0.1 * cos(dir_angle * PI/180);
            ball_y += 0.1 * sin(dir_angle * PI/180);
            if(ball_x>5.5)
            {
                printf("x right\n");
                dir_angle = dir_angle - 180;
                dir_angle = 360 - dir_angle;
                
            }
            else if(ball_x<-5.5)
            {
                printf("x left\n");
                dir_angle = dir_angle - 180;
                dir_angle = 360 - dir_angle;
            }
            else if(ball_y>5.5)
            {
                printf("y right\n");
                dir_angle = 360 - dir_angle;
            }
            else if(ball_y <-5.5)
            {
                printf("y left\n");
                dir_angle = 360 - dir_angle;
                // dir_angle = 180 - dir_angle;
            }

            ball_x = MIN(MAX(ball_x,-5.5),5.5);
            ball_y = MIN(MAX(ball_y,-5.5),5.5);
			break;

        case 'k':
            ball_r += 0.1;
            if(last_i) 
            {
                dir_angle = (180 + dir_angle) % 360;
                last_i = false;
            }
            ball_rot = ball_rot - 5.0;
            ball_rot_x = cos((dir_angle-90)*PI/180);
            ball_rot_y = sin((dir_angle-90)*PI/180);
			ball_x += 0.1 * cos(dir_angle * PI/180);
            ball_y += 0.1 * sin(dir_angle * PI/180);

            if(ball_x>5.5)
            {
                printf("x right\n");
                dir_angle = dir_angle - 180;
                dir_angle = 360 - dir_angle;
                
            }
            else if(ball_x<-5.5)
            {
                printf("x left\n");
                dir_angle = dir_angle - 180;
                dir_angle = 360 - dir_angle;
            }
            else if(ball_y>5.5)
            {
                printf("y right\n");
                dir_angle = 360 - dir_angle;
            }
            else if(ball_y <-5.5)
            {
                printf("y left\n");
                dir_angle = 360 - dir_angle;
                // dir_angle = 180 - dir_angle;
            }

            ball_x = MIN(MAX(ball_x,-5.5),5.5);
            ball_y = MIN(MAX(ball_y,-5.5),5.5);
			break;

        case 'j':
            dir_angle = (dir_angle + 1)%360;
            break;
        
        case 'l':
            dir_angle = (360+dir_angle - 1)%360;
            break;
    }
}

void specialkey(int key, int x, int y)
{
    switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			// cameraHeight -= 3.0;
			pos.x += 0.1;
			break;
		case GLUT_KEY_UP:		// up arrow key
			// cameraHeight += 3.0;
			pos.x -= 0.1;
			break;

		case GLUT_KEY_RIGHT:
			// cameraAngle += 0.03;
			pos.y += 0.1;
			break;
		case GLUT_KEY_LEFT:
			// cameraAngle -= 0.03;
			pos.y -= 0.1;
			break;

		case GLUT_KEY_PAGE_UP:
			pos.z += 0.1;
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos.z -= 0.1;
			break;


		default:
			break;
	}

    glutPostRedisplay();
}

void mouseDrag(int mx, int my)
{
    // iMouseX = mx;
    // iMouseY = iScreenHeight - my;
    // iMouseDrag(iMouseX, iMouseY);
    glutSwapBuffers();
}

void mouseMove(int mx, int my)
{
    // iMouseDirection(mx, iScreenHeight - my);
    // iMouseX = mx;
    // iMouseY = iScreenHeight - my;
    // iMouseMove(iMouseX, iMouseY);
    

    glutSwapBuffers();
}

void mouseClick(int button, int state, int x, int y)
{
    // iMouseX = x;
    // iMouseY = iScreenHeight - y;
    // iMouseClick(button, state, iMouseX, iMouseY);
    if(button==3)
    {
        pos.x -= 0.1;
        pos.y -= 0.1;
        pos.z -= 0.1;
    }
    else if(button==4)
    {
        pos.x += 0.1;
        pos.y += 0.1;
        pos.z += 0.1;
    }

    glutPostRedisplay();
    // glutSwapBuffers();
}

void mouseScroll(int button, int dir, int x, int y)
{
    // iMouseScroll(dir);
    
    // glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv); // Initialize GLUT // Without this : Other glut functions can't be called .
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(500, 0);                  // Position the window's initial top-left corner
    glutInitWindowSize(WindowWidth, WindowHeight); // Set the window's initial width & height // Default value : 300,300
    glutCreateWindow("First Program");             // Create a window with the given tit le	// Without this : ERROR:  glutMainLoop called with no windows created.
    glutDisplayFunc(display);                      // Register display callback handler for window re-paint  // Without this: ERROR:  No display callback registered for window 1
    glutReshapeFunc(resize);

    // Input handlers
    glutKeyboardFunc(key);
    glutSpecialFunc(specialkey); // special keys
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseDrag);
    glutPassiveMotionFunc(mouseMove);
    glutMouseWheelFunc(mouseScroll);

    glutTimerFunc(10, update, 10);
    // glMatrixMode(GL_MODELVIEW);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        60, // fov
        // 1920/1080.0, // aspect
        1,
        1, 
        200);
    
    // gluPerspective(80,	1,	1,	1000.0);

    // glOrtho(-WindowWidth / 2, WindowWidth / 2, -WindowHeight / 2, WindowHeight / 2, -WindowWidth / 2, WindowWidth / 2);

    // GLfloat mat_specular[] = {1.0, 1.0, 1.0};
	// GLfloat mat_shininess[] = {50.0};
	// GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
	// glClearColor(0, 0, 0, 0);
	// glShadeModel(GL_SMOOTH);
	// glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	// glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	// glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// glEnable(GL_COLOR_MATERIAL);
	// glEnable(GL_LIGHTING);
	// glEnable(GL_LIGHT0);
	// glEnable(GL_DEPTH_TEST);
	// glEnable(GL_LINE_SMOOTH);
    
    glEnable(GL_DEPTH_TEST); // je object kache setake rakhe
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(3.0);
    glEnable(GL_LINE_SMOOTH);
    init();
    // glutFullScreen();
    // glutSetCursor(GLUT_CURSOR_NONE);
    glutMainLoop(); // Enter the event-processing loop
    return 0;
}