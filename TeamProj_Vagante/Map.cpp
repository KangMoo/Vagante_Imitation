#include "stdafx.h"
#include "Map.h"
#include "EnemyManager.h"
#include "Player.h"
#include "UI.h"

Map::Map()
{
}


Map::~Map()
{
}
HRESULT Map::init()
{
	setTile();

	// �� �̹�����
	_BgImg = new image;
	_BgImg->init("Img\\etc\\BackGround.bmp", 1980, 1280, true, RGB(255, 0, 255));
	_mapImg = new image;
	_mapImg->init("Img\\etc\\VaganteMap.bmp", 1856, 1280, true, RGB(255, 0, 255));
	_trapImg = new image;
	_trapImg->init("Img\\map\\Trap.bmp", 32, 32, true, RGB(255, 0, 255));
	return S_OK;
}
void Map::release()
{

}
void Map::update()
{




	/*
	������ ���� ���

	tagItemInfo item;
	//�̹���
	item.bigImg = IMAGEMANAGER->findImage(); // ū �̹���(�κ��丮���� ������ �̹���)
	item.smallImg = IMAGEMANAGER->findImage(); //���� �̹���(�� ������ ������ �̹���)
	//�ɷ�ġ
	item.itemstat.hp = 0;
	item.itemstat.str = 1;
	item.itemstat.dex = 0;
	item.itemstat.vit = 0;
	item.itemstat.inl = 0;
	item.itemstat.lck = 0;
	item.itemstat.def = 0;
	item.itemstat.fir = 0;
	item.itemstat.ice = 0;
	item.itemstat.lgt = 0;
	item.itemstat.psn = 0;
	item.itemstat.mel = 0;
	item.itemstat.rng = 0;
	item.itemstat.crit = 0;
	item.itemstat.aspd = 0;
	item.itemstat.spd = 0;
	item.itemstate = ITEMSTATE_ONMAP;
	//������ ��ǥ
	item.point = ;
	//������ rect
	item.rc = ;
	//���� ������ ���� map�� �Ѱ��ֱ�!
	_ui->addItemOnMap(item);
	*/

}
void Map::render()
{

}

//�׸� ��	x��ǥ�� camera.x ��ŭ
//			y��ǥ�� camera.y ��ŭ �����ֱ�!!!!
void Map::render(POINT camera)
{
	draw(camera);
}
void Map::draw(POINT camera)
{
	//test~
//	IMAGEMANAGER->render("bg", getMemDC(), camera.x, camera.y);
	_BgImg->render(getMemDC(), camera.x, camera.y);
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 58; j++)
		{
			if (_mapInfo[i][j].type == MAPTILE_WALL || _mapInfo[i][j].type == MAPTILE_WALL2)
			{
				Rectangle(getMemDC(), _mapInfo[i][j].rc.left + camera.x,
					_mapInfo[i][j].rc.top + camera.y,
					_mapInfo[i][j].rc.right + camera.x,
					_mapInfo[i][j].rc.bottom + camera.y);
			}
			else if (_mapInfo[i][j].type == MAPTILE_LADDER)
			{
				Rectangle(getMemDC(), _mapInfo[i][j].rc.left + camera.x,
					_mapInfo[i][j].rc.top + camera.y,
					_mapInfo[i][j].rc.right + camera.x,
					_mapInfo[i][j].rc.bottom + camera.y);
			}
			else if (_mapInfo[i][j].type == MAPTILE_GROUND_CAN_GO_DOWN_1 || _mapInfo[i][j].type == MAPTILE_GROUND_CAN_GO_DOWN_2)
			{
				Rectangle(getMemDC(), _mapInfo[i][j].rc.left + camera.x,
					_mapInfo[i][j].rc.top + camera.y,
					_mapInfo[i][j].rc.right + camera.x,
					_mapInfo[i][j].rc.bottom + camera.y);
			}
			else if (_mapInfo[i][j].type == 6)
			{
				_trapImg->render(getMemDC(), _mapInfo[i][j].rc.left +camera.x, _mapInfo[i][j].rc.top + camera.y);
			}
		}
	}
	_mapImg->render(getMemDC(), camera.x, camera.y);
	//~test
}
void Map::setTile()
{

	/* 	0 : MAPTILE_NULL
		1 : MAPTILE_WALL,
		2 : MAPTILE_WALL2,
		3 : MAPTILE_LADDER,
		4 : MAPTILE_GROUND_CAN_GO_DOWN_1,
		5 : MAPTILE_GROUND_CAN_GO_DOWN_2
		6 : TRAP
		7 : DOOR
		9 : ITEM BOX
	*/

	//�� ����
	int temp[40][58] = 
	{
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 } ,
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },

		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,3,0,0,0, 0,0,1,1,1, 1,1,0,0,0, 1,1,1,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,1,1, 0,0,0, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,3,1,1,5, 5,5,1,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,1,1, 1,0,0, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,3,1,1,0, 0,0,0,1,0, 1,5,5,5,5, 5,5,1,0,1, 5,5,5,5,5, 5,5,5,1,0, 0,0,0,1,1, 0,0,0, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,3,1,1,1, 1,0,0,0,0, 1,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 0,0,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,3,0,0,1, 1,0,0,0,4, 0,0,0,0,0, 0,0,0,4,0, 1,0,0,0,0, 0,0,1,0,1, 1,0,0,0,0, 0,0,1, 1,1,1,1,1 },
											 
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,3,0,1,1, 1,1,1,0,0, 0,6,6,0,0, 6,0,6,0,1, 1,0,6,0,0, 0,7,1,1,1, 1,0,0,0,0, 0,0,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,3,0,1,1, 1,1,1,1,1, 1,1,1,4,4, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,0,0,0, 0,1,1,1,1, 1,1,1,5,5, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,0,4,4,4, 4,0,0,1,1, 1,1,1,3,0, 1,1,1,1,0, 0,0,0,0,0, 0,0,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1, 1,1,1,3,0, 0,1,1,1,0, 0,0,0,0,0, 0,0,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },

		{ 1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 1,1,0,0,0, 1,1,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,4,0, 0,0,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 0,0,0,0,0, 1,1,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,1,0,0,0, 0,0,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 1,1,0,0,0, 0,1,1,1,1, 1,1,0,0,4, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 6,0,1,1,1, 0,0,0,0,0, 0,0,0,1,1, 1,1,6,0,6, 6,1,1,1,1, 0,0,0,0,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,5,0,0, 0,5,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,4,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },

		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,0,3,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,0,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 5,5,3,1,1, 0,0,0,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,0,0,4,4, 0,0,1,1,1, 0,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,0,0,1,1, 0,0,0,1,1, 1,1,1,1,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,0,1,0, 0,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,3,0,0,0, 0,0,0,1,1, 1,0,0,0,0, 5,1,0,1,1, 0,0,1,5,0, 0,1,0,0,0, 0,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,3,0,1,1, 1,1,1,1,1, 1,0,0,1,6, 6,1,0,1,1, 0,0,1,0,0, 0,0,0,0,0, 0,1,1,1,1, 1,1,1, 1,1,1,1,1 },

		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0,0,0,1,1, 1,1,1,1,1, 1,0,0,1,1, 1,1,0,1,1, 0,0,1,5,0, 0,0,0,0,0, 0,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 3,0,0,1,1, 1,1,1,1,1, 1,1,0,0,0, 0,0,0,1,1, 0,0,1,0,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 3,0,0,1,1, 1,1,1,1,1, 1,1,1,0,0, 1,1,1,1,1, 1,1,1,4,4, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,0, 0,0,0,0,1, 1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 0,0,0,3,0, 0,0,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,0, 1,1,0,1,1, 0,0,0,1,1, 1,1,1,0,0, 0,0,0,0,0, 1,1,0,3,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
																							 
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,0, 0,0,0,1,0, 0,1,0,1,1, 0,0,1,0,0, 0,0,0,0,0, 0,0,0,3,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,0, 0,0,1,1,0, 0,0,0,1,1, 0,5,1,5,0, 1,1,1,0,0, 0,0,0,3,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,0, 0,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 1,1,1,0,0, 0,0,0,3,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,0, 0,1,0,0,0, 0,0,1,1,1, 5,0,0,0,5, 1,1,1,5,0, 0,0,0,0,0, 0,0,0,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,0, 0,1,6,1,6, 1,6,1,1,1, 0,0,0,0,0, 1,1,1,0,0, 0,0,0,0,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },

		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 } ,
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 },
		{ 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1, 1,1,1,1,1 }

	};

	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 58; j++)
		{
			_mapInfo[i][j].rc = RectMake(_mapInfo[i][j].point.x, _mapInfo[i][j].point.y, TILESIZE, TILESIZE);
		}
	}


	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 58; j++)
		{
			_mapInfo[i][j].type = temp[i][j];
			_mapInfo[i][j].point = PointMake(j*TILESIZE, i*TILESIZE);
			_mapInfo[i][j].rc = RectMake(_mapInfo[i][j].point.x, _mapInfo[i][j].point.y, TILESIZE, TILESIZE);
		}
	}

}