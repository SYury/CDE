#pragma once

#include "other.h"

enum mode_id{
	main_menu_mode,
	map_view_mode,
	select_mode,
	unit_move_mode,
	unit_attack_mode,
	resize_mode,
	new_game_mode,
	fail_mode,
	win_mode,
	hire_mode,
	hire_dir_mode,
	tile_info_mode
};

class GameWindow{
	int windowW, windowH;
	int mapW, mapH;
	mode_id currentMode;
	static GameWindow * _instance;
	public:
	void init(){_instance = nullptr;}
	static GameWindow * Instance(){
		if(_instance == nullptr)_instance = new GameWindow;
		return _instance;
	}
	mode_id getCurrentMode()const{return currentMode;}
	int getWindowW()const{return windowW;}
	int getWindowH()const{return windowH;}
	int getMapW()const{return mapW;}
	int getMapH()const{return mapH;}
	void setCurrentMode(mode_id newMode){currentMode = newMode;}
	void setWindowW(int val){windowW = val;}
	void setWindowH(int val){windowH = val;}
	void setMapW(int val){mapW = val;}
	void setMapH(int val){mapH = val;}
};

extern int playerId;
extern int turnId;

extern void reshape(int w, int h);
extern void drawField();
extern void handleKey(unsigned char key, int x, int y);
extern void handleSpecialKey(int key, int x, int y);
