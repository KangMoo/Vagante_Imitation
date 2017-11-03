#include "stdafx.h"
#include "EnemyManager.h"
#include "Player.h"
#include "Map.h"
#include "UI.h"

EnemyManager::EnemyManager()
{
}


EnemyManager::~EnemyManager()
{
}

HRESULT EnemyManager::init()
{

	//enemy 정보 추가
	/*
	Enemy* temp = new Enemy;
	temp->init();
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	_vEnemy.push_back(temp);
	*/

	//추가
	for (int i = 0; i < 3; i++)
	{
		Enemy* temp = new Enemy();
		temp->init(PointMake(TILESIZE * (38 + i), TILESIZE * 10), 150, 1000);
		temp->setMap(_map);
		temp->setPlayerAddressLink(_player);
		temp->setUiAddressLink(_ui);
		_vEnemy.push_back(temp);
	}

	_boss = new Boss();
	//상호참조를 위한 address링크
	_boss->setPlayerAddressLink(_player);
	_boss->setUiAddressLink(_ui);
	_boss->setMapAddressLink(_map);
	_boss->init(PointMake(TILESIZE*35,TILESIZE*6));

	return S_OK;
}

void EnemyManager::release()
{

}

void EnemyManager::update()
{

	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		//업데이트
		(*_viEnemy)->update();
	}

	//보스 체크

	//업데이트
	_boss->update();

	//enemy가 죽었는지 체크
	deadEnemyCheck();
}
void EnemyManager::render()
{

}

//그릴 때	x좌표에 camera.x 만큼
//			y좌표에 camera.y 만큼 더해주기!!!!
void EnemyManager::render(POINT camera)
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		(*_viEnemy)->render(camera);
	}

	_boss->render(camera);

	draw(camera);
}
void EnemyManager::draw(POINT camera)
{

}
void EnemyManager::deadEnemyCheck()
{
	//for (int i = _vEnemy.size() - 1; i > 0; i--)
	//{
	//	if (_vEnemy[i]->getHP() <= 0)
	//	{
	//		for (int i = 0; i < (*_viEnemy)->getMoney(); i++)
	//		{
	//			_ui->addCoinOnMap(PointMake(RND->getFromIntTo((*_viEnemy)->getRect().left, (*_viEnemy)->getRect().right), ((*_viEnemy)->getRect().top, (*_viEnemy)->getRect().bottom)));
	//		}
	//		SAFE_DELETE(_vEnemy[i]);
	//		_vEnemy.erase(_vEnemy.begin() + i);
	//	}
	//}
}