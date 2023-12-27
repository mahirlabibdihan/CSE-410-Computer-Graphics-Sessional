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
        if (!ball->isManualControl())
        {
            if (!collision_manager->check())
            {
                double min_dist = collision_manager->getMinDistance();
                time = min(time, min_dist / (ball->look.magnitude() * 0.1 / 10));
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
    camera->set();
    drawCheckerboard();

    ball->draw();
    wall->draw();
    // camera->drawReference();
}

void DGraphics::init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    ball->init();
    wall->init(4);
}

void DGraphics::keyboard(unsigned char key)
{
    camera->keyPress(key);
    switch (key)
    {
    case 'i':
        if (ball->isManualControl())
        {
            if (ball->isBackward())
                ball->switchDirection();
            else
                ball->moveForward(collision_manager);
        }
        break;

    case 'k':
        if (ball->isManualControl())
        {
            if (!ball->isBackward())
                ball->switchDirection();
            else
                ball->moveBackward(collision_manager);
        }
        break;

    case 'j':
        ball->rotateCounterClock();
        collision_manager->setNearestWall();
        if (!ball->isManualControl())
        {
            collision_manager->init();
            glutTimerFunc(0, update, ++current_id);
        }
        break;

    case 'l':
        ball->rotateClock();
        collision_manager->setNearestWall();
        if (!ball->isManualControl())
        {
            collision_manager->init();
            glutTimerFunc(0, update, ++current_id);
        }
        break;
    case ' ':
        animate = !animate;
        ball->switchControl();
        if (ball->isManualControl())
        {
            collision_manager->init();
        }
        else
        {
            if (ball->isBackward())
                ball->switchDirection();
            collision_manager->setNearestWall();
            glutTimerFunc(0, update, ++current_id);
        }
        break;

    case '+':
        ball->init();
        if (!ball->isManualControl())
        {
            collision_manager->init();
            glutTimerFunc(0, update, ++current_id);
        }
        wall->increase();
        break;

    case '-':
        ball->init();
        if (!ball->isManualControl())
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
    camera->specialKeyPress(key);
}

void DGraphics::mouseScroll(int dir)
{
    camera->mouseScroll(dir);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutTimerFunc(10, update, 10);
    DGraphics::initialize(600, 600, "Rolling Ball");
    glutMainLoop();
    return 0;
}