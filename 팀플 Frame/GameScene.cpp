#include "stdafx.h"
#include "GameScene.h"


GameScene::GameScene()
{
}


GameScene::~GameScene()
{

}
HRESULT GameScene::init()
{

	addImage();
	_em = new EnemyManager;
	_em->init();
	_player = new Player;
	_player->init(PointMake(1920,480));
	_map = new Map;
	_map->init();
	_ui = new UI;
	_ui->init();

	_map->setEnemyManagerAddressLink(_em);
	_map->setPlayerAddressLink(_player);
	_player->setEnemyManagerAddressLink(_em);
	_player->setMapAddressLink(_map);
	_em->setPlayerAddressLink(_player);
	_em->setMapAddressLink(_map);
	_ui->setPlayerAddressLink(_player);
	_UsingStatusWindow = false;
	_camera = PointMake(WINSIZEX, WINSIZEY);
	
	return S_OK;
}
void GameScene::release()
{

}
void GameScene::update() 
{
	_em->update();
	_player->update();
	_ui->update();
	keyInput();
	_camera = _player->getPoint();

	//ī�޶� ��ġ ������
	if (_camera.x < WINSIZEX / 2)
	{
		_camera.x = WINSIZEX / 2;
	}
	else if (_camera.x > IMAGEMANAGER->findImage("bg")->getWidth() - WINSIZEX / 2)
	{
		_camera.x = IMAGEMANAGER->findImage("bg")->getWidth() - WINSIZEX / 2;
	}
	if (_camera.y < WINSIZEY / 2)
	{
		_camera.y = WINSIZEY / 2;
	}
	else if (_camera.y > IMAGEMANAGER->findImage("bg")->getHeight() - WINSIZEY / 2)
	{
		_camera.y = IMAGEMANAGER->findImage("bg")->getHeight() - WINSIZEY / 2;
	}
	
}
void GameScene::render() 
{
	_map->render(_camera);
	_em->render(_camera);
	_player->render(_camera);
	_ui->render();
}

void GameScene::addImage()
{
	//�̹��� �߰��� ���⼭!!
	IMAGEMANAGER->addImage("bg","Img\\etc\\bg.bmp",2784,1920,true,RGB(255,0,255));
}
void GameScene::keyInput()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		if (_UsingStatusWindow)
		{
			_ui->setActive(false);
			_player->setCtrl(true);
			_UsingStatusWindow = false;
		}
		else
		{
			_ui->setActive(true);
			_player->setCtrl(false);
			_UsingStatusWindow = true;
		}
	}
}