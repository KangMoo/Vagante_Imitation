#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Map.h"

UI::UI()
{
}


UI::~UI()
{
}

HRESULT UI::init()
{
	_active = false;
	for (int i = 0; i < 10; i++)
	{

	}
	for (int i = 0; i < 5; i++)
	{

	}
	tagItemInfo _item[5];
	tagCoinInfo _coin[255];

	//========================  A D D   I M G ========================
	addImg();

	//================================================================
	_active = false;
	_menuNum = 0;
	_menuRect = RectMake(20, WINSIZEY - 88, 228, 280);
	_lvlRect = RectMake(_menuRect.left + 18, _menuRect.top + 162, 192, 84);
	_currentMoney = 0;
	_count = 0;
	_income = 0;
	_lvlNum = 0;
	_bagNum = 0;
	_skilNum = 0;
	_statNum = 0;
	_rankNum = 0;
	_tmp = "";
	_move = false;
	_delay.coin = 0;
	_delay.menu = 0;
	_inputAlphaSource = 0;
	_save.position0 = 0;
	_save.position1 = 0;
	_lvlPoint = 1;
	_plusAlphaSource = 0;

	//================================================================

	//setItemToBag(NAME_HEAL);
	setItemToBag(NAME_SWORD);


	return S_OK;
}
void UI::release()
{

}
void UI::update()
{
	//=========================== T I M E R ===========================
	_count++;
	if (_delay.coin) _delay.coin--;
	if (_delay.menu) _delay.menu--;

	//=================== P L A Y E R   U P D A T E ===================
	//_currentMoney = _player->getMoney();
	//_currentHp = _player->getHP();


	//=================== M E N U   R E C T ===========================
	_lvlRect = RectMake(_menuRect.left + 18, _menuRect.top + 157, 192, 224);

	//================= F R A M E   C O N T R O L =====================
	repeatIndex("cursor_idle", 8);
	repeatIndex("cursor_move", 8);
	//================= I N C O M E   U P D A T E =====================
	if (!_delay.coin && _income > 0)
	{
		_income--;
		_currentMoney++;
	}
	//================== A L P H A   S O U R C E ======================
	if (!_delay.menu)
	{
		_inputAlphaSource += 10;
		if (_inputAlphaSource >= 255) _inputAlphaSource = 255;
	}
	else
	{
		_inputAlphaSource = 0;
	}
	if (_lvlPoint)
	{
		if (_count % 180 < 90 && _count % 6 == 0) 
		{ 
			_plusAlphaSource += 15; 
			if (_plusAlphaSource > 180) _plusAlphaSource = 180;
		}
		else if (_count % 180 >= 90 && _count % 6 == 0)
		{
			_plusAlphaSource -= 15;
			if (_plusAlphaSource < 0) _plusAlphaSource = 0;
		}
	}

	//======================== F U N C T I O N ========================
	rectMove();
	if (_active) keyControl();

}
void UI::render()
{
	draw();
	explanation();

}
void UI::draw()
{
	IMAGEMANAGER->findImage("equip_slot")->render(getMemDC(), _menuRect.left + 52, _menuRect.top);
	IMAGEMANAGER->findImage("hpBarBottom")->render(getMemDC(), _menuRect.left + 2, _menuRect.top + 44);
	if (!_active)
		IMAGEMANAGER->findImage("selectOff")->render(getMemDC(), _menuRect.left + 2, _menuRect.top + 70);
	else
		IMAGEMANAGER->findImage("selectOn")->render(getMemDC(), _menuRect.left + 2, _menuRect.top + 70);
	switch (_menuNum)
	{
	case 0:
		IMAGEMANAGER->findImage("bag")->render(getMemDC(), _menuRect.left, _menuRect.top + 88);
		for (_viBag = _vBag.begin(); _viBag != _vBag.end(); ++_viBag)
		{
			_viBag->img->render(getMemDC(), _menuRect.left + (_viBag->position % 6) * 36,
				_menuRect.top + 90 + (_viBag->position / 6) * 36);
		}

		if (!_move)IMAGEMANAGER->findImage("cursor_idle")->frameRender(getMemDC(),
			_menuRect.left - 13 + (_bagNum % 6) * 36, _menuRect.top + 113 + (_bagNum / 6) * 36);
		else
		{
			for ( _viBag = _vBag.begin(); _viBag != _vBag.end(); ++_viBag)
			{
				if (_viBag->position == _save.position0)
				{
					_viBag->img->alphaRender(getMemDC(), _menuRect.left + (_bagNum % 6) * 36, _menuRect.top + 133 + (_bagNum / 6) * 36, 150);
				}

			}
			IMAGEMANAGER->findImage("cursor_move")->frameRender(getMemDC(),
				_menuRect.left - 13 + (_bagNum % 6) * 36, _menuRect.top + 113 + (_bagNum / 6) * 36);
		}

		break;
	case 1:
		IMAGEMANAGER->findImage("skil")->render(getMemDC(), _menuRect.left, _menuRect.top + 88);
		if (!_move)IMAGEMANAGER->findImage("cursor_idle")->frameRender(getMemDC(),
			_menuRect.left - 13 + (_skilNum % 6) * 36, _menuRect.top + 113 + (_skilNum / 6) * 36);
		else IMAGEMANAGER->findImage("cursor_move")->frameRender(getMemDC(),
			_menuRect.left - 13 + (_skilNum % 6) * 36, _menuRect.top + 113 + (_skilNum / 6) * 36);
		break;
	case 2:
		IMAGEMANAGER->findImage("stat")->render(getMemDC(), _menuRect.left, _menuRect.top + 88);
		if (_statNum < 16) IMAGEMANAGER->findImage("cursor_idle")->frameRender(getMemDC(),
			_menuRect.left - 15 + (_statNum % 3) * 78, _menuRect.top + 96 + (_statNum / 3) * 18);
		break;
	case 3:
		IMAGEMANAGER->findImage("lvl")->render(getMemDC(), _menuRect.left, _menuRect.top + 88);
		IMAGEMANAGER->findImage("lvl_icon")->frameRender(getMemDC(), _lvlRect.left, _lvlRect.top - 28, _lvlNum - 1, 0);
		IMAGEMANAGER->findImage("lvl_icon")->frameRender(getMemDC(), _lvlRect.left, _lvlRect.top, _lvlNum, 0);
		IMAGEMANAGER->findImage("lvl_icon")->frameRender(getMemDC(), _lvlRect.left, _lvlRect.top + 28, _lvlNum + 1, 0);
		switch (_lvlNum)
		{
		case 0:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Sword");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 36, "Holy");

			break;
		case 1:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top - 20, "Sword");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Holy");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 36, "Defense");
			break;
		case 2:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top - 20, "Holy");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Defense");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 36, "Strength");
			break;
		case 3:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top - 20, "Defense");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Strength");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 36, "Dexterity");
			break;
		case 4:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top - 20, "Strength");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Dexterity");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 36, "Vitality");
			break;
		case 5:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top - 20, "Dexterity");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Vitality");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 36, "Intelligence");
			break;
		case 6:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top - 20, "Vitality");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Intelligence");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 36, "Luck");
			break;
		case 7:
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top - 20, "Intelligence");
			letterMacro(LETTER_WHITE, _lvlRect.left + 38, _lvlRect.top + 8, "Luck");
			break;
		}
		if (_lvlPoint)
		{
			char tmp[10];
			itoa(_lvlPoint, tmp, 10);
			IMAGEMANAGER->findImage("plus")->alphaFrameRender(getMemDC(),
				_menuRect.left + 10, _menuRect.bottom - 28, _plusAlphaSource);
			letterMacro(LETTER_WHITE, _menuRect.left + 40, _menuRect.bottom - 26, tmp,_plusAlphaSource);
			letterMacro(LETTER_WHITE, _menuRect.left + 50, _menuRect.bottom - 26, " point available", _plusAlphaSource);
		}

		break;
	}
	IMAGEMANAGER->findImage("player_icon")->render(getMemDC(), _menuRect.left + 4, _menuRect.top + 38);

	IMAGEMANAGER->findImage("money_background")->render(getMemDC(), WINSIZEX - 138, 10);
	IMAGEMANAGER->findImage("money_icon")->render(getMemDC(), WINSIZEX - 130, 4);

	coinNumberMacro(NUMBER_COIN_YELLOW, WINSIZEX - 104, 2, _currentMoney);
	if (_income > 0)
	{
		IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), WINSIZEX - 90, 34, 10, 1);
		coinNumberMacro(NUMBER_COIN_GREEN, WINSIZEX - 104, 34, _income);
	}

}

void UI::setItemToBag(ITEMNAME name)
{
	int count = 0;
	bool end = false;

	tagItem item;
	ZeroMemory(&item, sizeof(item));
	item.name = name;
	switch (item.name)
	{
	case NAME_SWORD:
		item.img = IMAGEMANAGER->findImage("sword");
		item.type = TYPE_GEAR;
		break;
	case NAME_HEAL:
		item.img = IMAGEMANAGER->findImage("heal");
		item.type = TYPE_POTION;
		break;
	}
	item.alphaSource = 255;
	item.equip = false;
	while (true)
	{
		if (_vBag.size() == 0)
		{
			item.position = 0;
			end = true;

		}
		else
		{
			for (_viBag = _vBag.begin(); _viBag != _vBag.end(); ++_viBag)
			{
				if (_viBag->position == count)
				{
					count++;
					end = false;
					break;
				}
				end = true;
			}

		}
		if (end)
		{
			item.position = count;
			break;
		}

	}
	_vBag.push_back(item);
}

void UI::explanation()
{
	switch (_menuNum)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		switch (_lvlNum)
		{
		case 0:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*8,
					"Sword");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Sword Dance");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Slide backwards or");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*4,
					"forwards while stabbing.");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Hitting an enemy while");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"sliding deals extra");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"damage.");
				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*8,
					"Sword");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Omnistab");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Hold down and attack");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*4,
					"while falling for a falling");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"attack. Hold up and");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"attack to do a weaker");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"overhead slash.");
				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Sword");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Power Attack");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Hold your attack button");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"to gather strength.");
				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Sword");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Quick Draw");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Hone your dexterity.");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Draw your sword faster");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"than the enemy can");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"react.");
				break;
			}
			break;
		case 1:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Holy");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Angelic Wings");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Take no damage from");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"falling.");
				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Holy");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Divine weapon");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Swinging your weapon");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"after damaging enemies");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"manifests holy light.");

				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Holy");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Holy Shield");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Stored Divine weapon");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Charges give bonus");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"defense against");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"elemental damage.");

				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Holy");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Second Chance");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Attacks cannot kill you");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"unless your HP is zero.");
				break;
			}
			break;
		case 2:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Defense");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Defensive Stance");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Hold down to block");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"physical attacks.");

				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Defense");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Aspect of Rock");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Become immune to");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"Knockbacks when taking");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"damage.");

				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Defense");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Bulwark");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Gain a recharging");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"defensive shield.");
				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Defense");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Reflex Stance");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Correctly timed blocks will");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"reflect attack damage.");
				break;
			case 4:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Defense");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 5 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Aspect of Steel");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Some attacks will be");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"automatically blocked.");
				break;
			}
			break;
		case 3:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Strength");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"+1 Total Strength");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Strength increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"damage, especially with");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"melee weapons.");

				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Strength");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"+2 Total Strength");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Strength increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"damage, especially with");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"melee weapons.");

				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Strength");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Heavy Blows (+3 Total ");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Strength)");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Gain a small knockback");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"bonus.");

				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Strength");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"+4 Total Strength");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Strength increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"damage, especially with");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"melee weapons.");

				break;
			case 4:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Strength");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 5 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Brute Force(+5 Total ");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Strength)");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"your maximum damage is");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"increased.");

				break;
			}
			break;
		case 4:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Dexterity");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"+1 Total Dexterity");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Dexterity increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"attack/cast speed and");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"damage, especially with");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"ranged weapons.");

				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Dexterity");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"+2 Total Dexterity");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Dexterity increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"attack/cast speed and");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"damage, especially with");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"ranged weapons.");

				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Dexterity");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Quick Step(+3 Total ");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Dexterity) ");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Move faster while");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"attacking.");

				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Dexterity");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"+4 Total Dexterity");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Dexterity increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"attack/cast speed and");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"damage, especially with");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"ranged weapons.");

				break;
			case 4:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Dexterity");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 5 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Finesse(+5 Total ");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Dexterity) ");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"your mimimum damage is");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"increased.");

				break;
			}
			break;
		case 5:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Vitality");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"+1 Total Vitality");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Vitality increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"maximum health");

				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Vitality");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"+2 Total Vitality");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Vitality increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"maximum health");

				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Vitality");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Thick Skin(+3 Total ");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Vitality) ");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Gain a permanent +1");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"defense bonus.");

				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Vitality");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"+4 Total Vitality");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Vitality increases your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"maximum health");

				break;
			case 4:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Vitality");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 5 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Berserker's Blood(+5 Total ");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Vitality) ");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Gain +1 damage for each");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"20% of health missing.");

				break;

			}
			break;
		case 6:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*8,
					"Intelligence");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"+1 Total Intelligence");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Intelligence increases ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*4,
					"your maximum spell");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"charge, as well as your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"overall speel casting");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"abilities.");

				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*8,
					"Intelligence");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"+2 Total Intelligence");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Intelligence increases ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*4,
					"your maximum spell");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"charge, as well as your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"overall speel casting");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"abilities.");

				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Intelligence");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Meditation(+3 Total");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Intelligence");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"your spell charge limit is ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"increased.");

				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*8,
					"Intelligence");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"+4 Total Intelligence");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Intelligence increases ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*4,
					"your maximum spell");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"charge, as well as your");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"overall speel casting");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"abilities.");

				break;
			case 4:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Intelligence");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 5 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Preparation(+5 Total");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Intelligence");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"your spell charge limit is ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"greatly increased.");

				break;
			}
			break;
		case 7:
			switch (_rankNum)
			{
			case 0:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Luck");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 1 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"+1 Total Luck");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Luck affects your critical ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"strike rate and evasion,");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"as well as chance based");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"effects.");

				break;
			case 1:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Luck");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 2 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"+2 Total Luck");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Luck affects your critical ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"strike rate and evasion,");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"as well as chance based");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"effects.");

				break;
			case 2:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Luck");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 3 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Fortune(+3 Total Luck)");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*3,
					"Chests have a small");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"chance to drop an extra");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"item.");

				break;
			case 3:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*7,
					"Luck");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Level 4 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"+4 Total Luck");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Luck affects your critical ");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"strike rate and evasion,");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE*2,
					"as well as chance based");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"effects.");

				break;
			case 4:
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*6,
					"Luck");
				letterMacro(LETTER_WHITE, _menuRect.left + 10, _menuRect.top - SPACE*5,
					"Level 5 (next)");
				letterMacro(LETTER_BLUE, _menuRect.left + 10, _menuRect.top - SPACE*4,
					"Lucky Resist(+5 Total");
				letterMacro(LETTER_BLUE, _menuRect.left + 14, _menuRect.top - SPACE*3,
					"Luck");
				letterMacro(LETTER_GRAY, _menuRect.left + 10, _menuRect.top - SPACE*2,
					"Gain a chance to resist");
				letterMacro(LETTER_GRAY, _menuRect.left + 14, _menuRect.top - SPACE,
					"non-physical damage.");

				break;
			}
			break;
		}
		break;
	}
}

void UI::rectMove()
{
	if (_active)
	{
		_menuRect.top -= 15;
		_menuRect.bottom -= 15;
		if (_menuRect.top < WINSIZEY - 280)
		{
			_menuRect.top = WINSIZEY - 280;
			_menuRect.bottom = WINSIZEY;
		}
	}
	else
	{
		_menuRect.top += 15;
		_menuRect.bottom += 15;
		if (_menuRect.top >WINSIZEY - 88)
		{
			_menuRect.top = WINSIZEY - 88;
			_menuRect.bottom = _menuRect.top + 280;
		}

	}

	_lvlRect = RectMake(_menuRect.left + 18, _menuRect.top + 157, 192, 84);

}

void UI::keyControl()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT) && _active)
	{
		switch (_menuNum)
		{
		case 0:
			if (_bagNum % 6 != 0)_bagNum--;
			if (_bagNum < 0) _bagNum = 0;
			break;
		case 1:
			if (_skilNum % 6 != 0) _skilNum--;
			if (_skilNum < 0) _skilNum = 0;
			break;
		case 2:
			if (_statNum % 3 != 0)_statNum--;
			if (_statNum < 0) _statNum = 0;
			break;
		case 3:
			_rankNum--;
			if (_rankNum < 0) _rankNum = 0;
			break;
		}

	}
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) && _active)
	{
		switch (_menuNum)
		{
		case 0:
			if (_bagNum % 6 != 5) _bagNum++;
			if (_bagNum > 29) _bagNum = 29;
			break;
		case 1:
			if (_skilNum % 6 != 5)_skilNum++;
			if (_skilNum > 29) _skilNum = 29;
			break;
		case 2:
			if (_statNum % 3 != 2) _statNum++;
			if (_statNum > 14) _statNum = 14;
			break;
		case 3:
			_rankNum++;
			if (_rankNum > 3 && _lvlNum <= 1) _rankNum = 3;
			else if (_rankNum > 4 && _lvlNum > 1) _rankNum = 4;
			break;
		}

	}

	if (KEYMANAGER->isOnceKeyDown(VK_UP) && _active)
	{
		switch (_menuNum)
		{
		case 0:
			if (_bagNum / 6 != 0) _bagNum -= 6;
			if (_bagNum < 0) _bagNum = 0;
			break;
		case 1:
			if (_skilNum / 6 != 0)_skilNum -= 6;
			if (_skilNum < 0) _skilNum = 0;
			break;
		case 2:
			if (_statNum / 3 != 0) _statNum -= 3;
			if (_statNum < 0) _statNum = 0;
			break;
		case 3:
			_lvlNum--;
			if (_lvlNum != 0) _rankNum = 0;
			if (_lvlNum < 0) _lvlNum = 0;
			break;
		}
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _active)
	{
		switch (_menuNum)
		{
		case 0:
			if (_bagNum / 6 != 4)_bagNum += 6;
			if (_bagNum > 29) _bagNum = 29;
			break;
		case 1:
			if (_skilNum / 6 != 4)_skilNum += 6;
			if (_skilNum > 29) _skilNum = 29;
			break;
		case 2:
			if (_statNum / 3 != 4) _statNum += 3;
			if (_statNum > 14) _statNum = 14;
			break;
		case 3:
			if (_lvlNum != 7)
			{
				_lvlNum++;
				_rankNum = 0;
			}
			if (_lvlNum > 7) _lvlNum = 7;
			break;
		}
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LSHIFT))
	{
		_menuNum++;
		if (_menuNum > 3) _menuNum = 0;
	}
	if (KEYMANAGER->isOnceKeyDown('A'))
	{
		_menuNum--;
		if (_menuNum < 0) _menuNum = 3;
	}
	if (KEYMANAGER->isOnceKeyDown('Z'))
	{
		switch (_menuNum)
		{
		case 0:
			if (!_move)
			{
				for (_viBag = _vBag.begin(); _viBag != _vBag.end(); ++_viBag)
				{
					if (_viBag->position == _bagNum)
					{
						_move = true;
						_save.position0= _viBag->position;
						break;
					}
				}
			}
			else
			{
				for (_viBag = _vBag.begin(); _viBag != _vBag.end(); ++_viBag)
				{
					if (_viBag->position == _bagNum)
					{
						_viBag->position = _save.position0;
						continue;
					}
					if (_viBag->position == _save.position0)
					{
						_viBag->position = _bagNum;
					}
				}
				_move = false;
			}
			break;
		case 1:

			break;
		case 2:

			break;
		case 3:

			break;
		default:
			break;
		}
	}
	//if (KEYMANAGER->isOnceKeyDown('T'))
	//{
	//	setItemToBag(NAME_SWORD);
	//}

	//if (KEYMANAGER->isOnceKeyDown('Y'))
	//{
	//	setCoin(10);
	//}
}

void UI::positionChange(int sour, int dest)
{

}

void UI::coinNumberMacro(NUMBERFONT font, float x, float y, int num)
{
	int a, b, c, d;
	a = num / 1000;
	b = (num % 1000) / 100;
	c = (num % 100) / 10;
	d = num % 10;
	switch (font)
	{
	case NUMBER_COIN_YELLOW:
		if (a != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x, y, a, 0);
		if (a != 0 || b != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 18, y, b, 0);
		if (a != 0 || b != 0 || c != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 36, y, c, 0);
		IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 54, y, d, 0);
		break;
	case NUMBER_COIN_GREEN:
		if (a != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x, y, a, 1);
		if (a != 0 || b != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 18, y, b, 1);
		if (a != 0 || b != 0 || c != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 36, y, c, 1);
		IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 54, y, d, 1);
		break;
	case NUMBER_COIN_RED:
		if (a != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x, y, a, 2);
		if (a != 0 || b != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 18, y, b, 2);
		if (a != 0 || b != 0 || c != 0) IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 36, y, c, 2);
		IMAGEMANAGER->findImage("coin_number")->frameRender(getMemDC(), x + 54, y, d, 2);
		break;
	}
}

void UI::letterMacro(LETTERFONT font, float x, float y, char *str)
{
	for (int i = 0; *(str + i) != NULL; i++)
	{
		int sour;
		int dest = 0;
		if (*(str + i) >= 65 && *(str + i) <= 90)
		{
			sour = *(str + i) - 65;
			dest = 0;
		}
		else if (*(str + i) >= 97 && *(str + i) <= 122)
		{
			sour = *(str + i) - 97;
			dest = 1;
		}
		else if (*(str + i) >= 33 && *(str + i) <= 57)
		{
			sour = *(str + i) - 33;
			dest = 2;
		}
		else if (*(str + i) == 32)
		{
			sour = 26;
		}
		IMAGEMANAGER->findImage("letter")->alphaFrameRender(getMemDC(), x + i * 8, y, sour, dest + font * 3,0);
	}

}
void UI::letterMacro(LETTERFONT font, float x, float y, char *str, int alpha)
{
	for (int i = 0; *(str + i) != NULL; i++)
	{
		int sour;
		int dest = 0;
		if (*(str + i) >= 65 && *(str + i) <= 90)
		{
			sour = *(str + i) - 65;
			dest = 0;
		}
		else if (*(str + i) >= 97 && *(str + i) <= 122)
		{
			sour = *(str + i) - 97;
			dest = 1;
		}
		else if (*(str + i) >= 33 && *(str + i) <= 57)
		{
			sour = *(str + i) - 33;
			dest = 2;
		}
		else if (*(str + i) == 32)
		{
			sour = 26;
		}
		IMAGEMANAGER->findImage("letter")->alphaFrameRender(getMemDC(), x + i * 8, y, sour, dest + font * 3, alpha);
	}

}


void UI::repeatIndex(string keyName, int delay)
{
	if (_count % delay == 0)
	{
		if (IMAGEMANAGER->findImage(keyName)->getFrameX() < IMAGEMANAGER->findImage(keyName)->getMaxFrameX())
			IMAGEMANAGER->findImage(keyName)->setFrameX(IMAGEMANAGER->findImage(keyName)->getFrameX() + 1);
		else IMAGEMANAGER->findImage(keyName)->setFrameX(0);
	}
}

void UI::setCoin(int coin)
{
	_income += coin;
	_delay.coin = 100;
}

void UI::addImg()
{
	//============================ U I ========================================
	IMAGEMANAGER->addImage("bag", "Img/ui/bag.bmp", 228, 192, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("stat", "Img/ui/stat.bmp", 228, 192, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("lvl", "Img/ui/lvl.bmp", 228, 192, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("selectOn", "Img/ui/selectOn.bmp", 224, 18, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("selectOff", "Img/ui/selectOff.bmp", 224, 18, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hpBarBottom", "Img/ui/hpBarBottom.bmp", 224, 26, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("money_icon", "Img/ui/money_icon.bmp", 32, 32, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("money_background", "Img/ui/money_background.bmp", 128, 32, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("equip_slot", "Img/ui/equip_slot.bmp", 176, 44, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("player_icon", "Img/ui/player_icon.bmp", 28, 28, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("skil", "Img/ui/skill.bmp", 228, 192, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("lvl_icon", "Img/ui/lvl_icon.bmp", 224, 28, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("cursor_idle", "Img/ui/cursor_idle.bmp", 144, 36, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("cursor_move", "Img/ui/cursor_move.bmp", 144, 36, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("plus", "Img/ui/plus.bmp", 48, 48, 3, 3, true, RGB(255, 0, 255));
	//========================= F O N T ========================================
	IMAGEMANAGER->addFrameImage("coin_number", "Img/ui/font/number.bmp", 416, 96, 13, 3, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("letter", "Img/ui/font/letter_font.bmp", 270, 180, 27, 15, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("inputImage", "Img/ui/font/inputImage.bmp", 640, 640, 10, 10, true, RGB(255, 0, 255));

	//========================= I T E M =========================================
	IMAGEMANAGER->addImage("sword", "Img/ui/item/sword.bmp", 40, 40, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("heal", "Img/ui/item/heal.bmp", 40, 40, true, RGB(255, 0, 255));
}



void UI::addItemOnMap(tagItemInfo item)
{

}

void UI::addCoinOnMap(POINT coinPoint)
{
	//해당 POINT에 동전 스폰
}

void UI::showNumber(int num, COLORREF color)
{

}
