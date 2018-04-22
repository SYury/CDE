#include "ui.h"
#include "color.h"
#include "other.h"
#include "map.h"
#include "ai.h"

const int mainMenuOptions = 4;
const int resizeOptions = 2;
const int newGameOptions = 1;
const std::string cursorString = "<<<<<<<";
const Color cursorColor = lightBlueColor;
const Color menuColor = whiteColor;

int turnId;
int menuCursor;
int xPos, yPos;
int cornerX, cornerY;
int selectX, selectY;
int dX, dY;
int playerId;
std::string userInput;

std::vector<char> toSplit;
int toPush;

const int xDelta = 9, yDelta = 15;

void reshape(int w, int h){
	GLdouble size;
	GLdouble aspect;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	size = (GLdouble)((w >= h) ? w : h) / 2.0;
	if (w <= h) {
		aspect = (GLdouble)h/(GLdouble)w;
		glOrtho(-size, size, -size*aspect, size*aspect, -100000.0, 100000.0);
	}
	else {
		aspect = (GLdouble)w/(GLdouble)h;
		glOrtho(-size*aspect, size*aspect, -size, size, -100000.0, 100000.0);
	}
	glScaled(aspect, aspect, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

inline void setColor(const Color & col){
	glColor4f(col.getR(), col.getG(), col.getB(), 1.0f);
}

inline void printChar(const char & c){
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
}

inline void printChar(const char & c, const Color & col){
	setColor(col);
	glRasterPos2f(xPos, yPos);
	printChar(c);
}

inline void printString(const std::string & s, const Color & col){
	setColor(col);
	glRasterPos2f(xPos, yPos);
	for(char c : s)printChar(c);
}

void drawField(){
	GameWindow * window = (new GameWindow)->Instance();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	xPos = -(window->getWindowW() - 20)/2;
	yPos = (window->getWindowH() - 20)/2;
	Map* map = (new Map)->Instance();
	if(window->getCurrentMode() == fail_mode){
		printString("The thread of history is severed.", whiteColor);
		yPos -= 2 * yDelta;
		printString("Press ESC to continue.", whiteColor);
		glutSwapBuffers();
		return;
	}
	if(window->getCurrentMode() == win_mode){
		printString("You won this campaign!", whiteColor);
		yPos -= 2 * yDelta;
		printString("Press ESC to continue.", whiteColor);
		glutSwapBuffers();
		return;
	}
	if(window->getCurrentMode() == main_menu_mode){
		int _xPos = xPos;
		xPos = -130;
		printString("Carthago Delenda Est", yellowColor);
		xPos = _xPos;
		yPos -= 3 * yDelta;
		printString("New Campaign", menuColor);
		if(menuCursor == 0){
			xPos += xDelta * (1 + strlen("New Campaign"));
			printString(cursorString, cursorColor);
			xPos -= xDelta * (1 + strlen("New Campaign"));
		}
		yPos -= yDelta;
		printString("Load Campaign", menuColor);
		if(menuCursor == 1){
			xPos += xDelta * (1 + strlen("Load Campaign"));
			printString(cursorString, cursorColor);
			xPos -= xDelta * (1 + strlen("Load Campaign"));
		}
		yPos -= yDelta;
		printString("Resize Window", menuColor);
		if(menuCursor == 2){
			xPos += xDelta * (1 + strlen("Resize Window"));
			printString(cursorString, cursorColor);
			xPos -= xDelta * (1 + strlen("Resize Window"));
		}
		yPos -= yDelta;
		printString("Exit", menuColor);
		if(menuCursor == 3){
			xPos += xDelta * (1 + strlen("Exit"));
			printString(cursorString, cursorColor);
			xPos -= xDelta * (1 + strlen("Exit"));
		}
		yPos -= yDelta;
		glutSwapBuffers();
		return;
	}
	if(window->getCurrentMode() == resize_mode){
		printString("Big", menuColor);
		if(menuCursor == 0){
			xPos += xDelta * (1 + strlen("Big"));
			printString(cursorString, cursorColor);
			xPos -= xDelta * (1 + strlen("Big"));
		}
		yPos -= yDelta;
		printString("Normal", menuColor);
		if(menuCursor == 1){
			xPos += xDelta * (1 + strlen("Normal"));
			printString(cursorString, cursorColor);
			xPos -= xDelta * (1 + strlen("Normal"));
		}
		yPos -= yDelta;
		glutSwapBuffers();
		return;
	}
	if(window->getCurrentMode() == new_game_mode){
		printString("Choose your destiny...", menuColor);
		yPos -= 2 * yDelta;
		printString("Hannibal", menuColor);
		if(menuCursor == 0){
			xPos += xDelta * (1 + strlen("Hannibal"));
			printString(cursorString, cursorColor);
			xPos -= xDelta * (1 + strlen("Hannibal"));
		}
		yPos -= yDelta;
		glutSwapBuffers();
		return;
	}
	if(window->getCurrentMode() == map_view_mode){
		int w = std::min(window->getMapW(), map->W - cornerY);
		int h = std::min(window->getMapH(), map->H - cornerX);
		int defaultX = xPos;
		printString("View mode", whiteColor);
		yPos -= yDelta;
		printString("This is your ", whiteColor);
		xPos += xDelta * strlen("This is your ");
		printString("color", playerColorById(playerId));
		xPos -= xDelta * strlen("This is your ");
		yPos -= 2*yDelta;
		for(int i = cornerX; i < cornerX + h; i++){
			for(int j = cornerY; j < cornerY + w; j++){
				if(map->units[i][j] != nullptr){
					printChar('A', playerColorById(map->units[i][j]->getFraction()));
					xPos += xDelta;
					continue;
				}
				if(map->bases[i][j] != nullptr){
					printChar(map->bases[i][j]->getIcon(), playerColorById(map->bases[i][j]->getOwner()));
					xPos += xDelta;
					continue;
				}
				if(map->villages[i][j] != nullptr){
					printChar(map->villages[i][j]->getIcon(), playerColorById(map->villages[i][j]->getOwner()));
					xPos += xDelta;
					continue;
				}
				if(map->towns[i][j] != nullptr){
					printChar(map->towns[i][j]->getIcon(), playerColorById(map->towns[i][j]->getOwner()));
					xPos += xDelta;
					continue;
				}
				printChar(map->tiles[i][j].getIcon(), map->tiles[i][j].getColor());
				xPos += xDelta;
			}
			xPos = defaultX;
			yPos -= yDelta;
		}
		yPos -= yDelta;
		printString("Turn: " + std::to_string(turnId), whiteColor);
		yPos -= yDelta;
		printString("You have " + std::to_string(player[0].gold) + " gold and " + std::to_string(player[0].food) + " food left", whiteColor);
		yPos -= yDelta;
		glutSwapBuffers();
		return;
	}
	if(window->getCurrentMode() == select_mode || window->getCurrentMode() == unit_move_mode || window->getCurrentMode() == unit_attack_mode || window->getCurrentMode() == hire_dir_mode || window->getCurrentMode() == merge_mode || window->getCurrentMode() == split_dir_mode){
		int w = std::min(window->getMapW(), map->W - cornerY);
		int h = std::min(window->getMapH(), map->H - cornerX);
		int defaultX = xPos;
		if(window->getCurrentMode() == select_mode)printString("Select mode", whiteColor);
		if(window->getCurrentMode() == unit_move_mode)printString("Move mode", whiteColor);
		if(window->getCurrentMode() == unit_attack_mode)printString("Attack mode", whiteColor);
		if(window->getCurrentMode() == hire_dir_mode)printString("Choose the direction for unit to spawn", whiteColor);
		if(window->getCurrentMode() == merge_mode)printString("Choose the direction to merge with", whiteColor);
		if(window->getCurrentMode() == split_dir_mode)printString("Choose the direction for unit to split", whiteColor);
		yPos -= yDelta;
		printString("This is your ", whiteColor);
		xPos += xDelta * strlen("This is your ");
		printString("color", playerColorById(playerId));
		xPos -= xDelta * strlen("This is your ");
		yPos -= 2*yDelta;
		for(int i = cornerX; i < cornerX + h; i++){
			for(int j = cornerY; j < cornerY + w; j++){
				bool overrideCol = window->getCurrentMode() != hire_dir_mode && selectX == i && selectY == j;
				bool ov2 = window->getCurrentMode() == unit_attack_mode && selectX + dX == i && selectY + dY == j;
				if(map->units[i][j] != nullptr){
					printChar('A', ov2 ? ultraLightYellowColor : (overrideCol ? magentaColor : playerColorById(map->units[i][j]->getFraction())));
					xPos += xDelta;
					continue;
				}
				if(map->bases[i][j] != nullptr){
					printChar(map->bases[i][j]->getIcon(), ov2 ? ultraLightYellowColor : (overrideCol ? magentaColor : playerColorById(map->bases[i][j]->getOwner())));
					xPos += xDelta;
					continue;
				}
				if(map->villages[i][j] != nullptr){
					printChar(map->villages[i][j]->getIcon(), ov2 ? ultraLightYellowColor : (overrideCol ? magentaColor : playerColorById(map->villages[i][j]->getOwner())));
					xPos += xDelta;
					continue;
				}
				if(map->towns[i][j] != nullptr){
					printChar(map->towns[i][j]->getIcon(), ov2 ? ultraLightYellowColor : (overrideCol ? magentaColor : playerColorById(map->towns[i][j]->getOwner())));
					xPos += xDelta;
					continue;
				}
				printChar(map->tiles[i][j].getIcon(), ov2 ? ultraLightYellowColor : (overrideCol ? magentaColor : map->tiles[i][j].getColor()));
				xPos += xDelta;
			}
			xPos = defaultX;
			yPos -= yDelta;
		}
		yPos -= yDelta;
		printString("Turn: " + std::to_string(turnId), whiteColor);
		yPos -= yDelta;
		printString("You have " + std::to_string(player[0].gold) + " gold and " + std::to_string(player[0].food) + " food left", whiteColor);
		yPos -= yDelta;
		glutSwapBuffers();
		return;
	}
	if(window->getCurrentMode() == hire_mode){
		printString("Hire mode", whiteColor);
		yPos -= yDelta;
		if(dX == -1 && dY == 0)printString("Direction: UP", whiteColor);
		if(dX == 1 && dY == 0)printString("Direction: DOWN", whiteColor);
		if(dX == 0 && dY == -1)printString("Direction: LEFT", whiteColor);
		if(dX == 0 && dY == 1)printString("Direction: RIGHT", whiteColor);
		yPos -= 2 * yDelta;
		Unit * tmp = player[0].factory->getInfantry(0);
		printString("a| " + tmp->getName(), whiteColor);
		yPos -= yDelta;
		delete tmp;
		tmp = player[0].factory->getCavalry(0);
		printString("b| " + tmp->getName(), whiteColor);
		yPos -= yDelta;
		delete tmp;
		tmp = player[0].factory->getRanged(0);
		printString("c| " + tmp->getName(), whiteColor);
		yPos -= yDelta;
		delete tmp;
		tmp = player[0].factory->getHeavy(0);
		printString("d| " + tmp->getName(), whiteColor);
		yPos -= yDelta;
		delete tmp;
		glutSwapBuffers();
		return;
	}
	if(window->getCurrentMode() == split_mode){
		printString("Choose squads or units to split to the new army", whiteColor);
		yPos -= yDelta;
		if(dX == -1 && dY == 0)printString("Direction: UP", whiteColor);
		if(dX == 1 && dY == 0)printString("Direction: DOWN", whiteColor);
		if(dX == 0 && dY == -1)printString("Direction: LEFT", whiteColor);
		if(dX == 0 && dY == 1)printString("Direction: RIGHT", whiteColor);
		yPos -= 2 * yDelta;
		Army * what = map->units[selectX][selectY];
		int id = 1;
		auto outSquad = [&yPos, &id, &toSplit](Squad * s){
			std::queue<Squad*> q;
			std::queue<int> parent;
			std::queue<bool> removed;
			q.push(s);
			parent.push(0);
			removed.push(false);
			while(!q.empty()){
				Squad * curr = q.front();
				int p = parent.front();
				bool rm = removed.front();
				q.pop();
				parent.pop();
				removed.pop();
				auto col = (!rm && (toSplit.empty() || !toSplit[id])) ? whiteColor : lightBlueColor;
				if(curr->isUnit()){
					UnitInSquad * u = (UnitInSquad*)curr;
					printString(std::to_string(id) + "| Unit " + u->getName() + " in " + (p == 0 ? "initial army" : ("squad " + std::to_string(p))), col);
					yPos -= yDelta;
					++id;
				}
				else{
					printString(std::to_string(id) + "| Squad in " + (p == 0 ? "initial army" : ("squad " + std::to_string(p))), col);
					yPos -= yDelta;
					for(auto it = curr->begin(); it != curr->end(); it++){
						q.push(*it);
						parent.push(id);
						removed.push(!(!rm && (toSplit.empty() || !toSplit[id])));
					}
					++id;
				}
			}
		};
		for(auto it = what->begin(); it != what->end(); it++)
				outSquad(*it);
		if(toSplit.empty()){
			toSplit.resize(id);
			toSplit.assign(id, false);
		}
		yPos -= yDelta;
		printString("Enter squad number:", whiteColor);
		yPos -= yDelta;
		printString(userInput, lightBlueColor);
		yPos -= yDelta;
		glutSwapBuffers();
	}
	if(window->getCurrentMode() == squad_manage_mode){
		printString("Choose squad:", whiteColor);
		yPos -= 2 * yDelta;
		Army * what = map->units[selectX][selectY];
		int id = 1;
		auto outSquad = [&yPos, &id, &toSplit](Squad * s){
			std::queue<Squad*> q;
			std::queue<int> parent;
			std::queue<bool> removed;
			q.push(s);
			parent.push(0);
			removed.push(false);
			while(!q.empty()){
				Squad * curr = q.front();
				int p = parent.front();
				bool rm = removed.front();
				q.pop();
				parent.pop();
				removed.pop();
				auto col = (!rm && (toSplit.empty() || !toSplit[id])) ? whiteColor : lightBlueColor;
				if(curr->isUnit()){
					UnitInSquad * u = (UnitInSquad*)curr;
					printString(std::to_string(id) + "| Unit " + u->getName() + " in " + (p == 0 ? "initial army" : ("squad " + std::to_string(p))), col);
					yPos -= yDelta;
					++id;
				}
				else{
					printString(std::to_string(id) + "| Squad in " + (p == 0 ? "initial army" : ("squad " + std::to_string(p))), col);
					yPos -= yDelta;
					for(auto it = curr->begin(); it != curr->end(); it++){
						q.push(*it);
						parent.push(id);
						removed.push(!(!rm && (toSplit.empty() || !toSplit[id])));
					}
					++id;
				}
			}
		};
		for(auto it = what->begin(); it != what->end(); it++)
				outSquad(*it);
		yPos -= yDelta;
		if(toPush != -1){
			printString("Choosing squad to make " + std::to_string(toPush) + " a child of", whiteColor);
			yPos -= yDelta;
		}
		printString("Enter squad number:", whiteColor);
		yPos -= yDelta;
		printString(userInput, lightBlueColor);
		yPos -= yDelta;
		glutSwapBuffers();
	}
	if(window->getCurrentMode() == upgrade_mode){
		printString("Enhance army", whiteColor);
		yPos -= 2 * yDelta;
		printString("a| Enhance power for 100 gold", whiteColor);
		yPos -= yDelta;
		printString("b| Enhance defence for 100 gold", whiteColor);
		yPos -= yDelta;
		glutSwapBuffers();
		return;
	}
	if(window->getCurrentMode() == tile_info_mode){
		printString("Info mode", whiteColor);
		yPos -= 2*yDelta;
		printString("Tile position: " + std::to_string(selectX) + "X " + std::to_string(selectY) + "Y", whiteColor);
		yPos -= 2*yDelta;
		if(map->units[selectX][selectY] != nullptr){
			printString("This tile has an army:", whiteColor);
			yPos -= yDelta;
			printString("Owner's color:", whiteColor);
			yPos -= yDelta;
			Army * what = map->units[selectX][selectY];
			printString("XXXXXXXXXXXX", playerColorById(what->getFraction()));
			yPos -= yDelta;
			int id = 1;
			auto outSquad = [&yPos, &id](Squad * s){
				std::queue<Squad*> q;
				std::queue<int> parent;
				q.push(s);
				parent.push(0);
				while(!q.empty()){
					Squad * curr = q.front();
					int p = parent.front();
					q.pop();
					parent.pop();
					if(curr->isUnit()){
						UnitInSquad * u = (UnitInSquad*)curr;
						printString("Unit " + u->getName() + " in " + (p == 0 ? "initial army" : ("squad " + std::to_string(p))), whiteColor);
						yPos -= yDelta;
						printString("This unit has " + std::to_string(u->getHealth()) + " health points and " + std::to_string(u->getMP()) + " movement points left",lightBlueColor); 
						yPos -= yDelta;
					}
					else{
						printString("Squad " + std::to_string(id) + " in " + (p == 0 ? "initial army" : ("squad " + std::to_string(p))), whiteColor);
						yPos -= yDelta;
						for(auto it = curr->begin(); it != curr->end(); it++){
							q.push(*it);
							parent.push(id);
						}
						++id;
					}
				}
			};
			for(auto it = what->begin(); it != what->end(); it++)
				outSquad(*it);
			yPos -= 2 * yDelta;
		}
		if(map->bases[selectX][selectY] != nullptr){
			printString("This tile has a base.", whiteColor);
			yPos -= yDelta;
			if(map->bases[selectX][selectY]->getOwner() == -1){
				printString("UNOWNED", grayColor);
				yPos -= yDelta;
			}
			else{
				printString("Owner's color:", whiteColor);
				yPos -= yDelta;
				printString("XXXXXXXXXXXX", playerColorById(map->bases[selectX][selectY]->getOwner()));
				yPos -= yDelta;
			}
			printString("Health left: " + std::to_string(map->bases[selectX][selectY]->getHp()), whiteColor);
			yPos -= 2 * yDelta;
		}
		if(map->villages[selectX][selectY] != nullptr){
			printString("This tile has a village.", whiteColor);
			yPos -= yDelta;
			if(map->villages[selectX][selectY]->getOwner() == -1){
				printString("UNOWNED", grayColor);
				yPos -= yDelta;
			}
			else{
				printString("Owner's color:", whiteColor);
				yPos -= yDelta;
				printString("XXXXXXXXXXXX", playerColorById(map->villages[selectX][selectY]->getOwner()));
				yPos -= yDelta;
			}
			yPos -= yDelta;
		}
		if(map->towns[selectX][selectY] != nullptr){
			printString("This tile has a town.", whiteColor);
			yPos -= yDelta;
			if(map->towns[selectX][selectY]->getOwner() == -1){
				printString("UNOWNED", grayColor);
				yPos -= yDelta;
			}
			else{
				printString("Owner's color:", whiteColor);
				yPos -= yDelta;
				printString("XXXXXXXXXXXX", playerColorById(map->towns[selectX][selectY]->getOwner()));
				yPos -= yDelta;
			}
			yPos -= yDelta;
		}
		glutSwapBuffers();
		return;
	}
}

void handleKey(unsigned char key, int x, int y){
	Map * map = (new Map)->Instance();
	GameWindow * window = (new GameWindow)->Instance();
	if(window->getCurrentMode() == main_menu_mode){
		if(key == 13){
			if(menuCursor == 0){
				menuCursor = 0;
				window->setCurrentMode(new_game_mode);
				glutPostRedisplay();
			}
			if(menuCursor == 2){
				menuCursor = 0;
				window->setCurrentMode(resize_mode);
				glutPostRedisplay();
			}
			if(menuCursor == 3){
				exit(0);
			}
		}
		return;
	}
	if(window->getCurrentMode() == resize_mode){
		if(key == 13){
			if(menuCursor == 0){
				window->setWindowW(1500);
				window->setWindowH(840);
				window->setMapW(40);
				window->setMapH(40);
				glutReshapeWindow(window->getWindowW(), window->getWindowH());
				glutPostRedisplay();
			}
			if(menuCursor == 1){
				window->setWindowW(1024);
				window->setWindowH(780);
				window->setMapW(30);
				window->setMapH(30);
				glutReshapeWindow(window->getWindowW(), window->getWindowH());
				glutPostRedisplay();
			}
		}
		if(key == 27){
			menuCursor = 2;
			window->setCurrentMode(main_menu_mode);
			glutPostRedisplay();
		}
		return;
	}
	if(window->getCurrentMode() == new_game_mode){
		if(key == 27){
			menuCursor = 0;
			window->setCurrentMode(main_menu_mode);
			glutPostRedisplay();
		}
		if(key == 13){
			if(menuCursor == 0){
				turnId = 0;
				FILE * fl = fopen("maps/hannibal_1", "r");
				loadMap(fl);
				fclose(fl);
				cornerX = cornerY = 0;
				window->setCurrentMode(map_view_mode);
			}
			glutPostRedisplay();
		}
		return;
	}
	if(window->getCurrentMode() == map_view_mode){
		if(key == 's'){
			window->setCurrentMode(select_mode);
			selectX = cornerX;
			selectY = cornerY;
			glutPostRedisplay();
		}
		if(key == ' '){
			++turnId;
			aiTurn();
			for(int i = 0; i < map->H; i++)
				for(int j = 0; j < map->W; j++)
					if(map->units[i][j] != nullptr)map->units[i][j]->restoreMP();
			glutPostRedisplay();
		}
		return;
	}
	if(window->getCurrentMode() == select_mode){
		if(key == 27){
			window->setCurrentMode(map_view_mode);
			glutPostRedisplay();
		}
		if(key == 'm'){
			if(map->units[selectX][selectY] != nullptr && map->units[selectX][selectY]->getFraction() == 0)window->setCurrentMode(unit_move_mode);
			glutPostRedisplay();
		}
		if(key == 'a'){
			if(map->units[selectX][selectY] != nullptr && map->units[selectX][selectY]->getFraction() == 0){dX = dY = 0; window->setCurrentMode(unit_attack_mode);}
			glutPostRedisplay();
		}
		if(key == 'h'){
			if(map->bases[selectX][selectY] != nullptr && map->bases[selectX][selectY]->getOwner() == 0){dX = dY = 0; window->setCurrentMode(hire_dir_mode);}
			glutPostRedisplay();
		}
		if(key == 'i'){
			window->setCurrentMode(tile_info_mode);
			glutPostRedisplay();
		}
		if(key == 'g'){
			if(map->units[selectX][selectY] != nullptr && map->units[selectX][selectY]->getFraction() == 0){window->setCurrentMode(merge_mode);}
			glutPostRedisplay();
		}
		if(key == 'e'){
			if(map->units[selectX][selectY] != nullptr && map->units[selectX][selectY]->getFraction() == 0 && canUpgrade(selectX, selectY)){window->setCurrentMode(upgrade_mode);}
			glutPostRedisplay();
		}
		if(key == 'p'){
			if(map->units[selectX][selectY] != nullptr && map->units[selectX][selectY]->getFraction() == 0){dX = dY = 0; window->setCurrentMode(split_dir_mode);}
			glutPostRedisplay();
		}
		if(key == 'q'){
			if(map->units[selectX][selectY] != nullptr && map->units[selectX][selectY]->getFraction() == 0){dX = dY = 0; toPush = -1; window->setCurrentMode(squad_manage_mode);}
			glutPostRedisplay();
		}
		if(key == ' '){
			++turnId;
			aiTurn();
			for(int i = 0; i < map->H; i++)
				for(int j = 0; j < map->W; j++){
					if(map->units[i][j] != nullptr)map->units[i][j]->restoreMP();
					if(map->towns[i][j] != nullptr)map->towns[i][j]->makeIncome();
				}
			glutPostRedisplay();
		}
		return;
	}
	if(window->getCurrentMode() == unit_move_mode){
		if(key == 27){
			window->setCurrentMode(select_mode);
			glutPostRedisplay();
		}
		return;
	}
	if(window->getCurrentMode() == unit_attack_mode){
		if(key == 27){
			window->setCurrentMode(select_mode);
			glutPostRedisplay();
		}
		if(key == 13){
			if(map->units[selectX + dX][selectY + dY] == nullptr && map->bases[selectX + dX][selectY + dY] == nullptr)return;
			if(map->units[selectX + dX][selectY + dY] != nullptr)attackArmy(map->units[selectX][selectY], map->units[selectX + dX][selectY + dY]);
			else attackBase(map->units[selectX][selectY], map->bases[selectX + dX][selectY + dY]);
			window->setCurrentMode(select_mode);
			glutPostRedisplay();
		}
		return;
	}
	if(window->getCurrentMode() == hire_mode){
		if(key == 27){
			window->setCurrentMode(hire_dir_mode);
			dX = dY = 0;
			glutPostRedisplay();
		}
		if(key == 'a'){
			map->bases[selectX][selectY]->hireUnit(player[0].factory->getInfantry(0), dX, dY);
			window->setCurrentMode(map_view_mode);
			glutPostRedisplay();
		}
		if(key == 'b'){
			map->bases[selectX][selectY]->hireUnit(player[0].factory->getCavalry(0), dX, dY);
			window->setCurrentMode(map_view_mode);
			glutPostRedisplay();
		}
		if(key == 'c'){
			map->bases[selectX][selectY]->hireUnit(player[0].factory->getRanged(0), dX, dY);
			window->setCurrentMode(map_view_mode);
			glutPostRedisplay();
		}
		if(key == 'd'){
			map->bases[selectX][selectY]->hireUnit(player[0].factory->getHeavy(0), dX, dY);
			window->setCurrentMode(map_view_mode);
			glutPostRedisplay();
		}
		return;
	}
	if(window->getCurrentMode() == split_mode){
		if(key == 27){
			window->setCurrentMode(split_dir_mode);
			dX = dY = 0;
			toSplit.clear();
			userInput = "";
			glutPostRedisplay();
		}
		if(key >= '0' && key <= '9'){
			userInput += key;
			glutPostRedisplay();
		}
		if(key == 13){
			int id = atoi(userInput.c_str());
			if(id < (int)toSplit.size())
				toSplit[id] = true;
			userInput = "";
			glutPostRedisplay();
		}
		if(key == 'p'){
			int id = 1;
			Army * splitted = new Army;
			Army * init = map->units[selectX][selectY];
			std::vector<Squad*> was;
			was.push_back(init);
			auto splitSquad = [&was, &id, &splitted, &toSplit, &init](Squad * s){
				std::queue<Squad*> q;
				std::queue<int> parent;
				std::queue<bool> removed;
				q.push(s);
				parent.push(0);
				removed.push(false);
				while(!q.empty()){
					Squad * curr = q.front();
					int p = parent.front();
					bool rm = removed.front();
					was.push_back(curr);
					q.pop();
					parent.pop();
					removed.pop();
					if(toSplit[id] && !rm){
						was[p]->removeSquad(curr);
						splitted->addSquad(curr);
						rm = true;
					}
					++id;
					if(!curr->isUnit()){
						for(auto it = curr->begin(); it != curr->end(); it++){
							q.push(*it);
							parent.push(id - 1);
							removed.push(rm);
						}
					}
				}
			};
			std::vector<Squad*> check;
			for(auto it = map->units[selectX][selectY]->begin(); it != map->units[selectX][selectY]->end(); it++)
				check.push_back(*it);
			for(auto s : check)
				splitSquad(s);
			check.clear();
			if(!splitted->empty()){
				Army * what = map->units[selectX + dX][selectY + dY];
				if(what == nullptr)
					map->units[selectX + dX][selectY + dY] = splitted;
				else
					what->addSquad(splitted);
			}
			if(!cleanSquad(init, nullptr))
				map->units[selectX][selectY] = nullptr;
			window->setCurrentMode(select_mode);
			glutPostRedisplay();
		}
		if(key == 8){
			if(!userInput.empty())userInput.pop_back();
			glutPostRedisplay();
		}
		return;
	}
	if(window->getCurrentMode() == squad_manage_mode){
		if(key == 27){
			window->setCurrentMode(select_mode);
			userInput = "";
			glutPostRedisplay();
		}
		if(key >= '0' && key <= '9'){
			userInput += key;
			glutPostRedisplay();
		}
		if(key == '-'){
			int must_remove = atoi(userInput.c_str());
			userInput = "";
			int id = 1;
			Army * init = map->units[selectX][selectY];
			std::vector<Squad*> was;
			was.push_back(init);
			auto upSquad = [&was, &id, &must_remove, &init](Squad * s)->bool{
				std::queue<Squad*> q;
				std::queue<int> parent;
				q.push(s);
				parent.push(0);
				while(!q.empty()){
					Squad * curr = q.front();
					int p = parent.front();
					was.push_back(curr);
					q.pop();
					parent.pop();
					if(id == must_remove){
						was[p]->removeSquad(curr);
						init->addSquad(curr);
						return true;
					}
					++id;
					if(!curr->isUnit()){
						for(auto it = curr->begin(); it != curr->end(); it++){
							q.push(*it);
							parent.push(id - 1);
						}
					}
				}
				return false;
			};
			std::vector<Squad*> check;
			for(auto it = map->units[selectX][selectY]->begin(); it != map->units[selectX][selectY]->end(); it++)
				check.push_back(*it);
			for(auto s : check)
				if(upSquad(s))
					break;
			check.clear();
			if(!cleanSquad(init, nullptr))
				map->units[selectX][selectY] = nullptr;
			glutPostRedisplay();
		}
		if(key == '+'){
			int num = atoi(userInput.c_str());
			userInput = "";
			if(toPush == -1){
				toPush = num;
				glutPostRedisplay();
				return;
			}
			int id = 1;
			Army * init = map->units[selectX][selectY];
			Squad * toCut = nullptr, * toP = nullptr, * wP = nullptr;
			std::vector<Squad*> was;
			was.push_back(init);
			auto getSquad = [&was, &id, &toPush, &num, &toCut, &toP, &wP, &init](Squad * s)->bool{
				std::queue<Squad*> q;
				std::queue<int> parent;
				std::queue<bool> sub;
				q.push(s);
				parent.push(0);
				sub.push(false);
				while(!q.empty()){
					Squad * curr = q.front();
					int p = parent.front();
					bool is = sub.front();
					was.push_back(curr);
					q.pop();
					parent.pop();
					sub.pop();
					if(id == toPush){
						is = true;
						toCut = curr;
						wP = was[p];
					}
					if(id == num){
						if(is)
							return false;
						toP = curr;
						if(toP->isUnit()){
							Squad * pp = was[p];
							pp->removeSquad(toP);
							Squad * ns = new Squad(toP);
							pp->addSquad(ns);
							toP = ns;
						}
					}
					++id;
					if(!curr->isUnit()){
						for(auto it = curr->begin(); it != curr->end(); it++){
							q.push(*it);
							parent.push(id - 1);
							sub.push(is);
						}
					}
				}
				return true;
			};
			std::vector<Squad*> check;
			for(auto it = map->units[selectX][selectY]->begin(); it != map->units[selectX][selectY]->end(); it++)
				check.push_back(*it);
			bool can = true;
			for(auto s : check)
				if(!getSquad(s)){
					can = false;
					break;
				}
			if(can){
				wP->removeSquad(toCut);
				toP->addSquad(toCut);
			}
			if(!cleanSquad(init, nullptr))
				map->units[selectX][selectY] = nullptr;
			toPush = -1;
			glutPostRedisplay();
		}
		if(key == 8){
			if(!userInput.empty())userInput.pop_back();
			glutPostRedisplay();
		}
		return;
	}
	if(window->getCurrentMode() == upgrade_mode){
		if(key == 27){
			window->setCurrentMode(select_mode);
			glutPostRedisplay();
		}
		if(key == 'a'){
			map->units[selectX][selectY]->promote<UISStrong>();
			window->setCurrentMode(select_mode);
			glutPostRedisplay();
		}
		if(key == 'b'){
			map->units[selectX][selectY]->promote<UISArmored>();
			window->setCurrentMode(select_mode);
			glutPostRedisplay();
		}
		return;
	}
	if(window->getCurrentMode() == fail_mode || window->getCurrentMode() == win_mode){
		if(key == 27){
			window->setCurrentMode(main_menu_mode);
			glutPostRedisplay();
		}
		return;
	}
	if(window->getCurrentMode() == hire_dir_mode || window->getCurrentMode() == tile_info_mode || window->getCurrentMode() == merge_mode || window->getCurrentMode() == split_dir_mode){
		if(key == 27){
			window->setCurrentMode(select_mode);
			glutPostRedisplay();
		}
		return;
	}
}

void handleSpecialKey(int key, int x, int y){
	Map * map = (new Map)->Instance();
	GameWindow * window = (new GameWindow)->Instance();
	if(window->getCurrentMode() == main_menu_mode){
		if(key == GLUT_KEY_UP){
			if(menuCursor > 0)--menuCursor;
			else menuCursor = mainMenuOptions - 1;
		}
		if(key == GLUT_KEY_DOWN){
			if(menuCursor + 1 == mainMenuOptions)menuCursor = 0;
			else ++menuCursor;
		}
		glutPostRedisplay();
		return;
	}
	if(window->getCurrentMode() == resize_mode){
		if(key == GLUT_KEY_UP){
			if(menuCursor > 0)--menuCursor;
			else menuCursor = resizeOptions - 1;
		}
		if(key == GLUT_KEY_DOWN){
			if(menuCursor + 1 == resizeOptions)menuCursor = 0;
			else ++menuCursor;
		}
		glutPostRedisplay();
		return;
	}
	if(window->getCurrentMode() == new_game_mode){
		if(key == GLUT_KEY_UP){
			if(menuCursor > 0)--menuCursor;
			else menuCursor = newGameOptions - 1;
		}
		if(key == GLUT_KEY_DOWN){
			if(menuCursor + 1 == newGameOptions)menuCursor = 0;
			else ++menuCursor;
		}
		glutPostRedisplay();
		return;
	}
	if(window->getCurrentMode() == map_view_mode){
		if(key == GLUT_KEY_UP && cornerX > 0)cornerX--;
		if(key == GLUT_KEY_LEFT && cornerY > 0)cornerY--;
		if(key == GLUT_KEY_DOWN && cornerX + window->getMapH() < map->H)cornerX++;
		if(key == GLUT_KEY_RIGHT && cornerY + window->getMapW() < map->W)cornerY++;
		glutPostRedisplay();
		return;
	}
	if(window->getCurrentMode() == select_mode){
		if(key == GLUT_KEY_UP && selectX > cornerX)selectX--;
		if(key == GLUT_KEY_LEFT && selectY > cornerY)selectY--;
		if(key == GLUT_KEY_DOWN && selectX < cornerX + std::min(map->H, window->getMapH()) - 1)selectX++;
		if(key == GLUT_KEY_RIGHT && selectY < cornerY + std::min(map->W, window->getMapW()) - 1)selectY++;
		glutPostRedisplay();
		return;
	}
	if(window->getCurrentMode() == unit_move_mode){
		int dx = 0, dy = 0;
		if(key == GLUT_KEY_UP){dx = -1; dy = 0;}
		if(key == GLUT_KEY_DOWN){dx = 1; dy = 0;}
		if(key == GLUT_KEY_LEFT){dx = 0; dy = -1;}
		if(key == GLUT_KEY_RIGHT){dx = 0; dy = 1;}
		if(dx == 0 && dy == 0)return;
		if(!map->inMap(selectX + dx, selectY + dy))return;
		if(map->units[selectX + dx][selectY + dy] != nullptr || map->bases[selectX + dx][selectY + dy] != nullptr)return;
		moveArmy(map->units[selectX][selectY], dx, dy);
		window->setCurrentMode(select_mode);
		glutPostRedisplay();
		return;
	}
	if(window->getCurrentMode() == unit_attack_mode){
		if(key == GLUT_KEY_UP){dX--; if(abs(dX) + abs(dY) > map->units[selectX][selectY]->getAR() || !map->inMap(selectX + dX, selectY + dY))dX++;}
		if(key == GLUT_KEY_DOWN){dX++; if(abs(dX) + abs(dY) > map->units[selectX][selectY]->getAR() || !map->inMap(selectX + dX, selectY + dY))dX--;}
		if(key == GLUT_KEY_LEFT){dY--; if(abs(dX) + abs(dY) > map->units[selectX][selectY]->getAR() || !map->inMap(selectX + dX, selectY + dY))dY++;}
		if(key == GLUT_KEY_RIGHT){dY++; if(abs(dX) + abs(dY) > map->units[selectX][selectY]->getAR() || !map->inMap(selectX + dX, selectY + dY))dY--;}
		glutPostRedisplay();
		return;
	}
	if(window->getCurrentMode() == hire_dir_mode){
		if(key == GLUT_KEY_UP && map->inMap(selectX - 1, selectY) && (map->units[selectX - 1][selectY] == nullptr || map->units[selectX - 1][selectY]->getFraction() == 0) && map->bases[selectX - 1][selectY] == nullptr){dX = -1; dY = 0; window->setCurrentMode(hire_mode);}
		if(key == GLUT_KEY_DOWN && map->inMap(selectX + 1, selectY) && (map->units[selectX + 1][selectY] == nullptr || map->units[selectX + 1][selectY]->getFraction() == 0) && map->bases[selectX + 1][selectY] == nullptr){dX = 1; dY = 0; window->setCurrentMode(hire_mode);}
		if(key == GLUT_KEY_LEFT && map->inMap(selectX, selectY - 1) && (map->units[selectX][selectY - 1] == nullptr || map->units[selectX][selectY - 1]->getFraction() == 0) && map->bases[selectX][selectY - 1] == nullptr){dX = 0; dY = -1; window->setCurrentMode(hire_mode);}
		if(key == GLUT_KEY_RIGHT && map->inMap(selectX, selectY + 1) && (map->units[selectX][selectY + 1] == nullptr || map->units[selectX][selectY + 1]->getFraction() == 0) && map->bases[selectX][selectY + 1] == nullptr){dX = 0; dY = 1; window->setCurrentMode(hire_mode);}
		glutPostRedisplay();
		return;
	}
	if(window->getCurrentMode() == split_dir_mode){
		userInput = "";
		if(key == GLUT_KEY_UP && map->inMap(selectX - 1, selectY) && (map->units[selectX - 1][selectY] == nullptr || map->units[selectX - 1][selectY]->getFraction() == 0) && map->bases[selectX - 1][selectY] == nullptr){dX = -1; dY = 0; toSplit.clear(); window->setCurrentMode(split_mode);}
		if(key == GLUT_KEY_DOWN && map->inMap(selectX + 1, selectY) && (map->units[selectX + 1][selectY] == nullptr || map->units[selectX + 1][selectY]->getFraction() == 0) && map->bases[selectX + 1][selectY] == nullptr){dX = 1; dY = 0; toSplit.clear(); window->setCurrentMode(split_mode);}
		if(key == GLUT_KEY_LEFT && map->inMap(selectX, selectY - 1) && (map->units[selectX][selectY - 1] == nullptr || map->units[selectX][selectY - 1]->getFraction() == 0) && map->bases[selectX][selectY - 1] == nullptr){dX = 0; dY = -1; toSplit.clear(); window->setCurrentMode(split_mode);}
		if(key == GLUT_KEY_RIGHT && map->inMap(selectX, selectY + 1) && (map->units[selectX][selectY + 1] == nullptr || map->units[selectX][selectY + 1]->getFraction() == 0) && map->bases[selectX][selectY + 1] == nullptr){dX = 0; dY = 1; toSplit.clear(); window->setCurrentMode(split_mode);}
		glutPostRedisplay();
		return;
	}
	if(window->getCurrentMode() == merge_mode){
		if(key == GLUT_KEY_UP && map->inMap(selectX - 1, selectY) && map->units[selectX - 1][selectY] != nullptr && map->units[selectX - 1][selectY]->getFraction() == map->units[selectX][selectY]->getFraction()){
			map->units[selectX - 1][selectY]->addSquad(dynamic_cast<Squad*>(map->units[selectX][selectY]));
			map->units[selectX][selectY] = nullptr;
			window->setCurrentMode(select_mode);
		}
		if(key == GLUT_KEY_DOWN && map->inMap(selectX + 1, selectY) && map->units[selectX + 1][selectY] != nullptr && map->units[selectX + 1][selectY]->getFraction() == map->units[selectX][selectY]->getFraction()){
			map->units[selectX + 1][selectY]->addSquad(dynamic_cast<Squad*>(map->units[selectX][selectY]));
			map->units[selectX][selectY] = nullptr;
			window->setCurrentMode(select_mode);
		}
		if(key == GLUT_KEY_LEFT && map->inMap(selectX, selectY - 1) && map->units[selectX][selectY - 1] != nullptr && map->units[selectX][selectY - 1]->getFraction() == map->units[selectX][selectY]->getFraction()){
			map->units[selectX][selectY - 1]->addSquad(dynamic_cast<Squad*>(map->units[selectX][selectY]));
			map->units[selectX][selectY] = nullptr;
			window->setCurrentMode(select_mode);
		}
		if(key == GLUT_KEY_RIGHT && map->inMap(selectX, selectY + 1) && map->units[selectX][selectY + 1] != nullptr && map->units[selectX][selectY + 1]->getFraction() == map->units[selectX][selectY]->getFraction()){
			map->units[selectX][selectY + 1]->addSquad(dynamic_cast<Squad*>(map->units[selectX][selectY]));
			map->units[selectX][selectY] = nullptr;
			window->setCurrentMode(select_mode);
		}
		glutPostRedisplay();
		return;
	}
}
