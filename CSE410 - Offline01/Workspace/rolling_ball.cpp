#include "Headers.hpp"

Camera *camera = new Camera();
Ball *ball = new Ball(0.5, 8, 40);
Wall *wall = new Wall();
CollisionManager *collision_manager = new CollisionManager(ball, wall);

bool animate = false;
int current_id = 0;
void update(int id)
{
    double time = 10;
    if (id == current_id)
    {
        if (animate)
        {
            if (!collision_manager->check())
            {
                double min_dist = collision_manager->getMinDistance(collision_manager->nearest_wall, false);
                time = min(time, min_dist / (det2D(ball->look) * 0.1 / 10));
                double d = min(min_dist, 0.1);
                ball->pos = ball->pos + (ball->look * d);
                ball->rotate(-10 * d / 0.1);
            }
        }
        glutPostRedisplay();
        glutTimerFunc(time, update, id);
    }
}

void DGraphics::display()
{
    camera->setCamera();
    drawCheckerboard();
    wall->draw();
    ball->draw();
}

void DGraphics::init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    ball->init();
    wall->init(4);
}

void DGraphics::keyboard(unsigned char key)
{
    switch (key)
    {
    case '1':
        camera->rotateLeft();
        break;
    case '2':
        camera->moveRight();
        break;
    case '3':
        camera->rotateUp();
        break;
    case '4':
        camera->rotateDown();
        break;
    case '5':
        camera->tiltClock();
        break;
    case '6':
        camera->tiltCounterClock();
        break;
    case 'w':
        camera->moveUpRef();
        break;
    case 's':
        camera->moveDownRef();
        break;

    case 'i':
        if (!animate)
        {
            printf("Forward\n");
            ball->moveForward(collision_manager);
        }
        break;

    case 'k':
        if (!animate)
        {
            ball->moveBackward(collision_manager);
        }
        break;

    case 'j':
        ball->rotateCounterClock();
        if (animate)
        {
            collision_manager->init();
            glutTimerFunc(0, update, ++current_id);
        }
        break;

    case 'l':
        ball->rotateClock();
        if (animate)
        {
            collision_manager->init();
            glutTimerFunc(0, update, ++current_id);
        }
        break;
    case ' ':
        animate = !animate;
        if (animate)
        {
            glutTimerFunc(0, update, ++current_id);
        }
        else
        {
            collision_manager->init();
        }
        break;

    case '+':
        ball->init();
        if (animate)
        {
            collision_manager->init();
            glutTimerFunc(0, update, ++current_id);
        }
        wall->increase();
        break;

    case '-':
        ball->init();
        if (animate)
        {
            collision_manager->init();
            glutTimerFunc(0, update, ++current_id);
        }
        wall->decrease();
        break;
    case 27:
        glutLeaveMainLoop();
        break;
    default:
        break;
    }
}

void DGraphics::specialKeyboard(int key)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        camera->moveForward();
        break;
    case GLUT_KEY_DOWN:
        camera->moveBackward();
        break;

    case GLUT_KEY_RIGHT:
        camera->moveRight();
        break;

    case GLUT_KEY_LEFT:
        camera->moveLeft();
        break;

    case GLUT_KEY_PAGE_UP:
        camera->moveUp();
        break;
    case GLUT_KEY_PAGE_DOWN:
        camera->moveDown();
        break;

    default:
        break;
    }
}

void DGraphics::mouseScroll(int dir)
{
    if (dir == 1)
    {
        camera->moveForward();
    }
    else if (dir == -1)
    {
        camera->moveBackward();
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutTimerFunc(10, update, 10);
    DGraphics::initialize(600, 600, "Rolling Ball");
    glutMainLoop();
    return 0;
}