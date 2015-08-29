#include <typeinfo>

#include "c_game.h"
#include "c_asteroid.h"
#include "c_bullet.h"

sf::Clock c_Game::_timer;
int c_Game::_mapCounter;

c_Game::c_Game(){

}

void c_Game::Launch()
{
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
				//Activate the player shield
			}
		}

		sf::Event winEvent;
		while (_win.pollEvent(winEvent))
		{
			if (winEvent.type == sf::Event::Closed)
			{
				_win.close();
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				c_Asteroid* ast = new c_Asteroid(_win, 1, mousePos, 0.5, 0.5);
				InsertObject(*ast);
			}
		}

		_win.clear();
		UpdateScene();
		DrawScene();
		_win.display();
	}

}

bool c_Game::DoBoundingBoxesIntersect(c_LineSegment a, c_LineSegment b)
{
	return a.GetBoundingBox().intersects(b.GetBoundingBox());
}

double c_Game::CrossProduct(sf::Vector2i pointA, sf::Vector2i pointB)
{
	return (pointA.x * pointB.y) - (pointB.x * pointA.y);
}

bool c_Game::IsPointOnLine(c_LineSegment line, sf::Vector2i point)
{
	c_LineSegment lineTmp(sf::Vector2i(0, 0), sf::Vector2i(line._second.x - line._first.x, line._second.y - line._first.y));
	sf::Vector2i pointTmp;
	pointTmp.x = point.x - line._first.x;
	pointTmp.y = point.y - line._first.y;
	double r = CrossProduct(lineTmp._second, pointTmp);
	return std::abs(r) < 10;
}

bool c_Game::IsPointRightOfLine(c_LineSegment line, sf::Vector2i point)
{
	c_LineSegment lineTmp(sf::Vector2i(0, 0), sf::Vector2i(line._second.x - line._first.x, line._second.y - line._first.y));

	sf::Vector2i pointTmp;
	pointTmp.x = point.x - line._first.x;
	pointTmp.y = point.y - line._first.y;

	double r = CrossProduct(lineTmp._second, pointTmp);
	return CrossProduct(lineTmp._second, pointTmp) < 0;
}

bool c_Game::LineTouchesOrCrossesLine(c_LineSegment a, c_LineSegment b)
{
	return IsPointOnLine(a, b._first) || IsPointOnLine(a, b._second) || (IsPointRightOfLine(a, b._first) ^ IsPointRightOfLine(a, b._second));

}

bool c_Game::DoLinesIntersect(c_LineSegment a, c_LineSegment b)
{
	return DoBoundingBoxesIntersect(a, b) && LineTouchesOrCrossesLine(a, b) && LineTouchesOrCrossesLine(b, a);

}

bool c_Game::Collision(sf::ConvexShape shapeA, sf::ConvexShape shapeB)
{
	bool result = false;
	std::vector<c_LineSegment> linesA;
	std::vector<c_LineSegment> linesB;

	//ShapeA
	for (int i = 0; i < shapeA.getPointCount(); i++)
	{
		if (i == shapeA.getPointCount() - 1)
		{
			sf::Vector2i firstPoint(shapeA.getTransform().transformPoint(shapeA.getPoint(i)));
			sf::Vector2i secondPoint(shapeA.getTransform().transformPoint(shapeA.getPoint(0)));
			c_LineSegment ls(firstPoint, secondPoint);
			linesA.push_back(ls);
		}
		else
		{
			sf::Vector2i firstPoint(shapeA.getTransform().transformPoint(shapeA.getPoint(i)));
			sf::Vector2i secondPoint(shapeA.getTransform().transformPoint(shapeA.getPoint(i + 1)));
			c_LineSegment ls(firstPoint, secondPoint);
			linesA.push_back(ls);
		}
	}

	//ShapeB
	for (int i = 0; i < shapeB.getPointCount(); i++)
	{
		if (i == shapeB.getPointCount() - 1)
		{
			sf::Vector2i firstPoint(shapeB.getTransform().transformPoint(shapeB.getPoint(i)));
			sf::Vector2i secondPoint(shapeB.getTransform().transformPoint(shapeB.getPoint(0)));
			c_LineSegment ls(firstPoint, secondPoint);
			linesB.push_back(ls);
		}
		else
		{
			sf::Vector2i firstPoint(shapeB.getTransform().transformPoint(shapeB.getPoint(i)));
			sf::Vector2i secondPoint(shapeB.getTransform().transformPoint(shapeB.getPoint(i + 1)));
			c_LineSegment ls(firstPoint, secondPoint);
			linesB.push_back(ls);
		}
	}

	for (auto& itA : linesA)
	{
		for (auto& itB : linesB)
		{
			if (DoLinesIntersect(itA, itB))
			{
				result = true;
				break;
			}

		}
	}
	return result;
}

void c_Game::RemoveObject(int index)
{
	delete _gameWorldObjects.at(index);
	_gameWorldObjects.erase(index);
}

void c_Game::InsertObject(c_GameWorldObject& newObject)
{
	_gameWorldObjects.insert(std::pair<int, c_GameWorldObject*>(_mapCounter, &newObject));
	_mapCounter++;
}

float c_Game::GetTickCount()
{
	return _timer.getElapsedTime().asMilliseconds();
}

void c_Game::UpdateScene()
{
	//if (_gameOver)
	//	return;

	_collision = false;
	_indexes.clear();

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

							if (asteroid.GetSize() < 3)
							{
								c_Asteroid *ast1 = new c_Asteroid(_win, asteroid.GetSize() + 1, asteroid.GetPos(), asteroid.GetVX(), asteroid.GetVY());
								c_Asteroid *ast2 = new c_Asteroid(_win, asteroid.GetSize() + 1, asteroid.GetPos(), -asteroid.GetVX(), -asteroid.GetVY());

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
			if (it.second->IsColliding(*_player))
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

	if (_gameOver)
	{
		sf::Font font;
		font.loadFromFile("arial.ttf");
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

