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

	//enemy ���� �߰�
	/*
	Enemy* temp = new Enemy;
	temp->init(PointMake(100, 100));
	_vEnemy.push_back(temp);
	*/

	_boss = new Boss();
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
		//enemy���� �÷��̾� ��ǥ �Ѱ��ֱ�
		(*_viEnemy)->setPlayerAddressLink(_player);
		(*_viEnemy)->setUiAddressLink(_ui);
		//enemy���� ��,��,��,�� Ÿ������ �Ѱ��ֱ�
		(*_viEnemy)->setMap(_map);
		(*_viEnemy)->setTileInfo(
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) / TILESIZE - 1, int((*_viEnemy)->getPoint().x) / TILESIZE - 1),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) / TILESIZE - 1, int((*_viEnemy)->getPoint().x) / TILESIZE),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) / TILESIZE - 1, int((*_viEnemy)->getPoint().x) / TILESIZE + 1),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) / TILESIZE, int((*_viEnemy)->getPoint().x) / TILESIZE - 1),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) / TILESIZE, int((*_viEnemy)->getPoint().x) / TILESIZE),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) / TILESIZE, int((*_viEnemy)->getPoint().x) / TILESIZE + 1),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) / TILESIZE + 1, int((*_viEnemy)->getPoint().x) / TILESIZE - 1),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) / TILESIZE + 1, int((*_viEnemy)->getPoint().x) / TILESIZE),
		_map->getMapInfo(int((*_viEnemy)->getPoint().y) / TILESIZE + 1, int((*_viEnemy)->getPoint().x) / TILESIZE + 1));

		//������Ʈ
		(*_viEnemy)->update();
	}

	//���� üũ

	//��ȣ������ ���� address��ũ
	_boss->setPlayerAddressLink(_player);
	_boss->setUiAddressLink(_ui);
	_boss->setMapAddressLink(_map);

	//������Ʈ
	_boss->update();

	//enemy�� �׾����� üũ
	deadEnemyCheck();
}
void EnemyManager::render()
{

}

//�׸� ��	x��ǥ�� camera.x ��ŭ
//			y��ǥ�� camera.y ��ŭ �����ֱ�!!!!
void EnemyManager::render(POINT camera)
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		(*_viEnemy)->render(camera);
	}
	draw(camera);

	_boss->render(camera);
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