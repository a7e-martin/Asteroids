#ifndef DEF_C_GAME
#define DEF_C_GAME
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "c_gameWorldObject.h"
#include "c_player.h"
#include <vector>
#include <map>
#include <thread>

class c_Game{
public:
	c_Game();
	void Launch();
	void InsertObject(c_GameWorldObject& newObject);
	void RemoveObject(int index);
	static float GetTickCount();
	void Update();
	void UpdateScene();
	void DrawScene();
	void MarkForDelete(int index); //This one ensures we do not try to delete the same object twice
	void JoinThread();
private:
	std::thread* _updateThread;
	sf::RenderWindow _win;
	std::map<int, c_GameWorldObject*> _gameWorldObjects;
	std::vector<int> _indexes;
	c_Player* _player;
	static int _maxBullets;
	int _asteroidsCount;
	int _actualWave;
	static int _mapCounter;
	static sf::Clock _timer;
	bool _collision;
	bool _gameOver;
};

#endif

