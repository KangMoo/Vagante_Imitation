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
	_player = new Player;
	_map = new Map;
	_ui = new UI;

	//상호참조 연결
	_map->setEnemyManagerAddressLink(_em);
	_map->setPlayerAddressLink(_player);
	_map->setUiAddressLink(_ui);
	_player->setEnemyManagerAddressLink(_em);
	_player->setMapAddressLink(_map);
	_player->setUiAddressLink(_ui);
	_em->setPlayerAddressLink(_player);
	_em->setMapAddressLink(_map);
	_em->setUiAddressLink(_ui);
	_ui->setPlayerAddressLink(_player);
	_ui->setMapAddressLink(_map);
	_ui->setEnemyManagerAddressLink(_em);

	_ui->init();
	_map->init();
	_player->init(PointMake(TILESIZE*(36 + 5), TILESIZE*(4 + 5)));
	_em->init();

	_UsingStatusWindow = false;
	_camera = PointMake(WINSIZEX, WINSIZEY);

	return S_OK;
}
void GameScene::release()
{

}
void GameScene::update()
{
	_camera = _player->getPoint();
	_em->update();
	_player->update();
	_ui->update();
	keyInput();

	//카메라 위치 재조정
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
	_map->render(PointMake(WINSIZEX / 2 - _camera.x, WINSIZEY / 2 - _camera.y));
	_em->render(PointMake(WINSIZEX / 2 - _camera.x, WINSIZEY / 2 - _camera.y));
	_player->render(PointMake(WINSIZEX / 2 - _camera.x, WINSIZEY / 2 - _camera.y));
	_ui->render(PointMake(WINSIZEX / 2 - _camera.x, WINSIZEY / 2 - _camera.y));
	TIMEMANAGER->render(getMemDC());
}

void GameScene::addImage()
{
	//이미지 추가는 여기서!!
	IMAGEMANAGER->addImage("bg", "Img/etc/temp_bg.bmp", TILESIZE * 58, TILESIZE * 40, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("폭발", "Img/etc/explosion_1.bmp", 448, 64, 7, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("test", "Img/etc/tes.bmp", 40, 224, 1, 7, true, RGB(255, 0, 255));


	IMAGEMANAGER->addFrameImage("boss_dragon_cry", "Img/enemy/dragon_cry.bmp", 304, 164, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("boss_dragon_dead", "Img/enemy/dragon_dead.bmp", 39 * 2, 30 * 2, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("boss_dragon_stamp", "Img/enemy/dragon_drive.bmp", 152 * 2, 80 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("boss_dragon_fire", "Img/enemy/dragon_fire.bmp", 175 * 2, 76 * 2, 5, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("fireball", "Img/enemy/dragon_fireball.bmp", 640, 160, 16, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("boss_dragon_fly", "Img/enemy/dragon_fly.bmp", 186 * 2, 76 * 2, 6, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("boss_dragon_sleep", "Img/enemy/dragon_sleep.bmp", 37 * 2, 30 * 2, 1, 2, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("wormMoveUp", "Img\\enemy\\crawler_move_up.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveDown", "Img\\enemy\\crawler_move_down.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveLeft", "Img\\enemy\\crawler_move_left.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveRight", "Img\\enemy\\crawler_move_right.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("bathit", "Img\\enemy\\bat_hit.bmp", 32, 32, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("batsleep", "Img\\enemy\\bat_idle.bmp", 32, 32, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("batflying", "Img\\enemy\\bat_moving.bmp", 128, 32, 4, 1, true, RGB(255, 0, 255));

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