#include <typeinfo>

#include "c_game.h"
#include "c_asteroid.h"
#include "c_bullet.h"
#include "SFML/Network.hpp"

sf::Clock c_Game::_timer;
int c_Game::_mapCounter;

c_Game::c_Game(){

}

void c_Game::Launch()
{
	_updateThread = new std::thread(&c_Game::Update, this);
}


void c_Game::RemoveObject(int index)
{
	if (typeid(*(_gameWorldObjects.at(index))) == typeid(c_Asteroid))
	{
		_asteroidsCount--;
		std::cout << _asteroidsCount << "\n";
	}
	delete _gameWorldObjects.at(index);
	_gameWorldObjects.erase(index);
}

void c_Game::InsertObject(c_GameWorldObject& newObject)
{
	_gameWorldObjects.insert(std::pair<int, c_GameWorldObject*>(_mapCounter, &newObject));
	_mapCounter++;
	if (typeid(newObject) == typeid(c_Asteroid))
	{
		_asteroidsCount++;
	}
}

float c_Game::GetTickCount()
{
	return _timer.getElapsedTime().asMilliseconds();
}

void c_Game::UpdateScene()
{
	_collision = false;
	_indexes.clear();

	if (_asteroidsCount == 0)
	{
		_actualWave++;
		for (int i = 0; i < _actualWave; i++)
		{
			std::srand(GetTickCount());
			sf::Vector2i pos;
			pos.x = std::rand() % _win.getSize().x - _player->GetPosition().x;
			pos.y = std::rand() % _win.getSize().y - _player->GetPosition().y;
			c_Asteroid* asteroid = new c_Asteroid(_win, 1, pos);

			InsertObject(*asteroid);
		}
	}

	for (auto& it : _gameWorldObjects)
	{
		//Collision between bullets and asteroids
		if (typeid(*(it.second)) == typeid(c_Bullet))
		{
			for (auto it2 : _gameWorldObjects)
			{
				if (it.second->GetShape().getGlobalBounds().intersects(it2.second->GetShape().getGlobalBounds()))
				{
					if (typeid(*(it2.second)) == typeid(c_Asteroid))
					{
						if (it.second->IsColliding(*it2.second))
						{
							c_Asteroid &asteroid = *dynamic_cast<c_Asteroid *>(it2.second);
							MarkForDelete(it.first);
							MarkForDelete(it2.first);
							_player->AddPoints(asteroid.GetSize() * 10);
							
							if (asteroid.GetSize() < 3)
							{
								c_Asteroid *ast1 = new c_Asteroid(_win, asteroid.GetSize() + 1, asteroid.GetPos());
								c_Asteroid *ast2 = new c_Asteroid(_win, asteroid.GetSize() + 1, asteroid.GetPos());

								InsertObject(*ast1);
								InsertObject(*ast2);
							}
							_collision = true;
						}
					}
				}
			}
		}

		//Collision between the _player and asteroids
		if (typeid(*(it.second)) != typeid(c_Player) && it.second->GetShape().getGlobalBounds().intersects(_player->GetShape().getGlobalBounds()) && typeid(*(it.second)) != typeid(c_Bullet))
		{
			if (it.second->IsColliding(*_player) && !_player->IsInvicible())
			{
				_player->MakeInactive(1000);
				if (!_player->GetRemainingLives())
				{
					_gameOver = true;
				}
			}
		}
		if (it.second->Remove())
		{
			MarkForDelete(it.first);
		}

		it.second->Update();

		if (_collision)
		{
			break;
		}
	}

	//Remove no longer needed objects
	for (auto& it : _indexes)
	{
		RemoveObject(it);
	}
}

void c_Game::DrawScene()
{
	for (auto& it : _gameWorldObjects)
	{
		if (typeid(*(it.second)) == typeid(c_Player) && _gameOver)
		{

		}
		else
		{
			it.second->Draw();
		}
	}
	//Draw the wave number
	sf::Font font;
	font.loadFromFile("eve.ttf");
	sf::Text waveText;
	waveText.setFont(font);
	waveText.setCharacterSize(30);
	waveText.setString("Wave " + std::to_string(_actualWave));
	waveText.setPosition(_win.getSize().x - waveText.getLocalBounds().width - 30, _win.getSize().y - waveText.getLocalBounds().height - 30);
	_win.draw(waveText);

	if (_gameOver)
	{
		sf::Font font;
		font.loadFromFile("eve.ttf");
		sf::Text gameOverText;
		gameOverText.setFont(font);
		gameOverText.setString("Game Over !");
		gameOverText.setCharacterSize(60);
		gameOverText.setPosition(_win.getSize().x / 2, _win.getSize().y / 2);
		gameOverText.setOrigin(gameOverText.getLocalBounds().width / 2.0f, gameOverText.getLocalBounds().height / 2.0f);
		_win.draw(gameOverText);
	}
}

void c_Game::MarkForDelete(int index)
{
	if (std::find(_indexes.begin(), _indexes.end(), index) != _indexes.end())
	{
	}
	else
	{
		_indexes.push_back(index);
	}
}

void c_Game::Update()
{
	sf::TcpSocket t1;
	t1.connect("127.0.0.1", 5555);

	_win.create(sf::VideoMode(1536, 986), "Asteroids");
	_win.setFramerateLimit(60);
	_player = new c_Player(_win);
	InsertObject(*_player);
	_timer.restart();

	while (_win.isOpen())
	{


		sf::Vector2i mousePos = sf::Mouse::getPosition(_win);

		if (!_gameOver)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				_player->Rotate(-1);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				_player->Rotate(1);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				_player->Accelerate();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				c_Bullet* bullet = _player->Fire();

				if (bullet)
				{
					InsertObject(*bullet);
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				_player->ActivateShield();
			}
		}

		sf::Event winEvent;
		while (_win.pollEvent(winEvent))
		{
			if (winEvent.type == sf::Event::Closed)
			{
				sf::Packet packet;
				packet << "quit";
				t1.send(packet);
				_win.close();
			}
		}

		_win.clear();
		UpdateScene();
		DrawScene();
		_win.display();
	}
}

void c_Game::JoinThread()
{
	if (_updateThread->joinable())
	{
		_updateThread->join();
	}
}
