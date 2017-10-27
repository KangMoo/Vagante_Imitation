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
	_player->init(PointMake(TILESIZE*(36 + 5), TILESIZE*(4 + 5)));
	_map = new Map;
	_map->init();
	_ui = new UI;
	_ui->init();

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
	_ui->render();
}

void GameScene::addImage()
{
	//이미지 추가는 여기서!!
	IMAGEMANAGER->addImage("bg", "Img\\etc\\temp_bg.bmp", TILESIZE * 58, TILESIZE * 40, true, RGB(255, 0, 255));

	//맨이터 - 김주섭
	IMAGEMANAGER->addImage("maneater_Dead",				 "image/enemy/manEater/ManEater_dead.bmp", 32, 32, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("maneataer_Attack",		 "image/enemy/manEater/ManEater_attack.bmp", 64, 32, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("maneater_Attack_After", "image/enemy/manEater/ManEater_after_attack.bmp", 224, 32, 7, 1, true, RGB(255, 0, 255));

	//고블린 - 김주섭
	IMAGEMANAGER->addFrameImage("goblin_Idle",		"image/enemy/goblin/Goblin_idle.bmp", 32, 68,1,2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("doblin_Hit",		"image/enemy/goblin/Goblin_hit.bmp", 32, 68, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("goblin_Dead",		"image/enemy/goblin/Goblin_dead.bmp", 32, 68, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("goblin_Attack",	"image/enemy/goblin/Goblin_attack.bmp,", 256, 68, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("goblin_Jump",		"image/enemy/goblin/Goblin_jump.bmp", 128, 68, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("goblin_Move",		"image/enemy/goblin/Goblin_move.bmp,",256, 68, 8, 2, true, RGB(255, 0, 255));

	//박쥐 - 김주섭
	IMAGEMANAGER->addImage("bat_Idle",		"image/enemy/bat/Bat_idle.bmp", 32, 32, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bat_Hit",		"image/enemy/bat/Bat_hit.bmp", 32, 32, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("bat_Move", "image/enemy/bat/Bat_moving.bmp", 128, 32,4,1, true, RGB(255, 0, 255));

	//슬라임 - 김주섭
	IMAGEMANAGER->addFrameImage("slime_Idle",		"image/enemy/slime/Slime_idle.bmp", 128, 64, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("slime_Move",		"image/enemy/slime/Slime_move.bmp", 192, 64, 6, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("slime_Dash_Cast",	"image/enemy/slime/Slime_dashCast.bmp", 128, 64, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("slime_Dash",		"image/enemy/slime/Slime_dash.bmp", 192, 64, 6, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("slime_Jump",		"image/enemy/slime/Slime_jump.bmp", 96, 64, 3, 2, true, RGB(255, 0, 255));

	//=======================================================================================================================================================
	
	IMAGEMANAGER->addFrameImage("폭발", "Img\\etc\\explosion_1.bmp", 448, 64, 7, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("test", "Img\\etc\\tes.bmp", 40, 224, 1, 7, true, RGB(255, 0, 255));
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