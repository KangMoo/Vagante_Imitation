#include "stdafx.h"
#include "mainGame.h"

mainGame::mainGame()
{
}


mainGame::~mainGame()
{

}


HRESULT mainGame::init(void)
{
	gameNode::init(true);

	SCENEMANAGER->addScene("게임씬", new GameScene);
	SCENEMANAGER->changeScene("게임씬");
	return S_OK;
}

void mainGame::release(void)
{
	
}


void mainGame::update(void)	
{
	gameNode::update();

		
	SCENEMANAGER->update();
	
	
}


void mainGame::render(void)	
{
	//흰색 도화지 한 장 필요
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//==============================================================

	SCENEMANAGER->render();
	
	TIMEMANAGER->render(getMemDC());
	
	//========================================================================
	//백버퍼에 있는걸 HDC로 그려주는 역할
	this->getBackBuffer()->render(getHDC(), 0, 0);

}