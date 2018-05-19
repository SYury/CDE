#include "ui.h"
#include "map.h"
#include "player.h"

Map * Map::_instance;
GameWindow * GameWindow::_instance;

int main(int argc, char ** argv){
	freopen("log.txt", "w", stderr);
	(new Map)->init();
	(new GameWindow)->init();
	for(int i = 0; i < MAX_PLAYERS; i++)player[i].id = i;
	system("mkdir saves");
	GameWindow * window = (new GameWindow)->Instance();
	window->setWindowW(1024);
	window->setWindowH(780);
	window->setMapW(30);
	window->setMapH(30);
	window->setCurrentMode(main_menu_mode);
	player[0].human = true;
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(window->getWindowW(), window->getWindowH());
    glutCreateWindow("Carthago Delenda Est"); 
    glutDisplayFunc(drawField);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKey);
	glutSpecialFunc(handleSpecialKey);
    glutMainLoop();
}
