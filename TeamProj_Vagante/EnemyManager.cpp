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
	temp->init();
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	_vEnemy.push_back(temp);
	*/


	POINT pt = PointMake(TILESIZE * 14, TILESIZE * 18 - 1);
	Enemy* temp = new worm;
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 9999, 9999);
	_vEnemy.push_back(temp);

	temp = new bat;
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 0, 0);
	_vEnemy.push_back(temp);

	_boss = new Boss();
	//��ȣ������ ���� address��ũ
	_boss->setPlayerAddressLink(_player);
	_boss->setUiAddressLink(_ui);
	_boss->setMapAddressLink(_map);
	_boss->init(PointMake(TILESIZE * 35, TILESIZE * 6));

	temp = new slime;
	pt = PointMake(TILESIZE * 35, TILESIZE * 6);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	//_vEnemy.push_back(temp);

	pt = PointMake(TILESIZE * 20 , TILESIZE * 5 );
	temp = new manEater;
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);

	temp = new goblin;
	pt = PointMake(TILESIZE * 35, TILESIZE * 6);
	temp->setMap(_map);
	temp->setPlayerAddressLink(_player);
	temp->setUiAddressLink(_ui);
	temp->init(pt, 50, 50);
	_vEnemy.push_back(temp);

	return S_OK;
}

void EnemyManager::release()
{

}

void EnemyManager::update()
{

	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		//������Ʈ
		(*_viEnemy)->update();
	}

	//���� üũ

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
	_boss->render(camera);

	draw(camera);

}
void EnemyManager::draw(POINT camera)
{

}
void EnemyManager::deadEnemyCheck()
{
	for (int i = _vEnemy.size() - 1; i >= 0; i--)
	{
		if (_vEnemy[i]->getHP() <= 0 && !_vEnemy[i]->getdead())
		{
			//for (int j = 0;j < (*_viEnemy)->getMoney(); j++)
			//{
			//	_ui->addCoinOnMap(PointMake(RND->getFromIntTo((*_viEnemy)->getRect().left, (*_viEnemy)->getRect().right), ((*_viEnemy)->getRect().top, (*_viEnemy)->getRect().bottom)));
			//}
			//SAFE_DELETE(_vEnemy[i]);
			//_vEnemy.erase(_vEnemy.begin() + i);
			_vEnemy[i]->setState(ENEMYSTATE_DEAD);
		}
		else if (_vEnemy[i]->getdead())
		{
			SAFE_DELETE(_vEnemy[i]);
			_vEnemy.erase(_vEnemy.begin() + i);
		}
	}
}