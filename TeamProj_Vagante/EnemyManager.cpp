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
	temp->init(PointMake(100, 100));
	_vEnemy.push_back(temp);
	*/

	return S_OK;
}
void EnemyManager::release()
{

}
void EnemyManager::update()
{

	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		//enemy에게 플레이어 좌표 넘겨주기
		(*_viEnemy)->setPlayerPoint(_player->getPoint());
		//enemy에게 상,하,좌,우 타일정보 넘겨주기
		(*_viEnemy)->setTileInfo(
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) % TILESIZE - 1, int((*_viEnemy)->getPoint().x) % TILESIZE - 1),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) % TILESIZE - 1, int((*_viEnemy)->getPoint().x) % TILESIZE),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) % TILESIZE - 1, int((*_viEnemy)->getPoint().x) % TILESIZE + 1),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) % TILESIZE, int((*_viEnemy)->getPoint().x) % TILESIZE - 1),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) % TILESIZE, int((*_viEnemy)->getPoint().x) % TILESIZE),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) % TILESIZE, int((*_viEnemy)->getPoint().x) % TILESIZE + 1),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) % TILESIZE + 1, int((*_viEnemy)->getPoint().x) % TILESIZE - 1),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) % TILESIZE + 1, int((*_viEnemy)->getPoint().x) % TILESIZE),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) % TILESIZE + 1, int((*_viEnemy)->getPoint().x) % TILESIZE + 1));

		//업데이트
		(*_viEnemy)->update();
	}

	//enemy가 죽었는지 체크
	deadEnemyCheck();
}
void EnemyManager::render()
{

}

//그릴 때	x좌표에 (WINSIZEX/2 - camera.x) 만큼
//			y좌표에 (WINSIZEY/2 - camera.y) 만큼 더해주기!!!!
void EnemyManager::render(POINT camera)
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		(*_viEnemy)->render(camera);
	}
	draw(camera);
}
void EnemyManager::draw(POINT camera)
{

}
void EnemyManager::deadEnemyCheck()
{
	for (int i = _vEnemy.size() - 1; i >= 0; i--)
	{
		if (_vEnemy[i]->getHP() <= 0)
		{
			for (int i = 0; i < (*_viEnemy)->getMoney(); i++)
			{
				_ui->addCoinOnMap(PointMake(RND->getFromIntTo((*_viEnemy)->getRect().left, (*_viEnemy)->getRect().right), ((*_viEnemy)->getRect().top, (*_viEnemy)->getRect().bottom)));
			}
			SAFE_DELETE(_vEnemy[i]);
			_vEnemy.erase(_vEnemy.begin() + i);
		}
	}
}