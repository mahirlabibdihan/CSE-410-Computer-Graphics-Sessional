#include <vector>
using namespace std;
vector<PT> vertex;
int n = 4;
void drawWall()
{
    glBegin(GL_QUADS);
    {
        glColor4f(1, 0, 0, 0.7);
        for (int i = 0; i < n; i++)
        {
            glVertex3f(vertex[i].x, vertex[i].y, 0.0);
            glVertex3f(vertex[(i + 1) % n].x, vertex[(i + 1) % n].y, 0.0);
            glVertex3f(vertex[(i + 1) % n].x, vertex[(i + 1) % n].y, 0.8);
            glVertex3f(vertex[i].x, vertex[i].y, 0.8);
        }
    }
    glEnd();

    {

        glColor4f(0, 0, 0, 1);
        for (int i = 0; i < n; i++)
        {
            glBegin(GL_LINE_STRIP);
            glVertex3f(vertex[i].x, vertex[i].y, 0.0);
            glVertex3f(vertex[(i + 1) % n].x, vertex[(i + 1) % n].y, 0.0);
            glVertex3f(vertex[(i + 1) % n].x, vertex[(i + 1) % n].y, 0.8);
            glVertex3f(vertex[i].x, vertex[i].y, 0.8);
            glEnd();
        }
    }
}
