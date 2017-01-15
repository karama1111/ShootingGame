#ifndef DBLDEF1_H_fjds3l3kfj28adl9ja_INCLUDED
#define DBLDEF1_H_fjds3l3kfj28adl9ja_INCLUDED

#include "DxLib.h"
#include <math.h>
#include <vector>
#define PI 3.141592
#define MAIN_WINDOW_WIDTH (DEFAULT_SCREEN_SIZE_X - 200)
#define MAIN_WINDOW_HEIGHT DEFAULT_SCREEN_SIZE_Y

// �Q�ƃT�C�g�Fhttp://homepage2.nifty.com/natupaji/DxLib/dxlecture_main.html
// �f�ރT�C�g�Fhttp://www.presys.jp/


//////////////////////////////////////////////
// Point�N���X
//////////////////////////////////////////////
class Point
{
public:
	int x;
	int y;
	Point( int ax, int ay );
	Point();
};

//////////////////////////////////////////////
// GameWindow class
//////////////////////////////////////////////
class GameWindow
{
public:
	int backgroundColorR;
	int backgroundColorG;
	int backgroundColorB;
	char* windowText;
	GameWindow();
};

//////////////////////////////////////////////
// GameObject�N���X
//////////////////////////////////////////////

class GameObject
{
protected:
	int img;
	int x, y;
	int w, h;
	int distance;// 1�t���[���ł̈ړ�����
	char* imgName;

public:
	void setImage(char *s) {img = LoadGraph( s );}
	int getX(){return x;}
	int getY(){return y;}
	int getW(){return w;}
	int getH(){return h;}
	void draw(){
		DrawGraph( x, y, img, TRUE );
	}
	GameObject(char *s);
};

//////////////////////////////////////////////
// Bullet�N���X
//////////////////////////////////////////////
class Bullet : public GameObject
{
private:
	int speed;
	int direction;//enum��-1��1�ɂ������������̂�������Ȃ�
	//�����ꂩ�A�p�x�������悤�ɂ���Ύ΂߂ɂ��łĂ邯��
	int dx;
	int dy;
	int bossMoveCount;
	int deg;
	double rad;
public:
	Bullet(char *imgName, int aSpeedName, int x, int y, int direction);
	bool move();
	bool bossMove();
	bool deleteFlag;

};

//////////////////////////////////////////////
// Shooter �N���X
//////////////////////////////////////////////

class Shooter : public GameObject
{
protected:
	bool existenceFlag;
public:
	Shooter(char *s);
	void shoot( char *imgName, int speed, int x, int y, int direction );
	void moveBullets( void );
	void drawBullets( void );
	void deleteBullet( int n );
	std::vector<Bullet> bullets;
	void emerges( void );
	void kill( void );
	bool exists( void );
};
//////////////////////////////////////////////
// Player�N���X
//////////////////////////////////////////////

class Player : public Shooter
{
private:
	int collisioned();
	
public:
	Player( char *s );
	void move( Point p );
	void shoot( void );
	bool isVisible;
};

//////////////////////////////////////////////
// Enemy�N���X
//////////////////////////////////////////////

class Enemy : public Shooter
{
private:
	int dx;
	int dy;
	int bossHP;
	bool isBoss( void );
public:
	Enemy( char *s );
	virtual bool move();
	void shoot( void );
	void emerges( void );
	void kill( void );
	bool isClear( void );
};

//////////////////////////////////////////////
// Boss�N���X
//////////////////////////////////////////////

//Boss�N���X�g���Ȃ�����
//Enemy��vector�ɓ�����Enemy�̃��\�b�h����т�����Ă��܂��̂�
//�����􌩂���܂ŕ��u

class Boss : public Enemy
{
private:
	int dy;
public:
	Boss( char *s );
	void emerges( void );
	bool move();
};


//////////////////////////////////////////////
// Score�N���X
//////////////////////////////////////////////

class Score
{
private:
	int score;
public:
	Score();
	void addScore();
	int getScore();
};



//////////////////////////////////////////////
// �Փ˔���֐�
//////////////////////////////////////////////
int checkCollision(GameObject, GameObject);
//////////////////////////////////////////////
// ���͂��ꂽ�L�[�̔���֐�
//////////////////////////////////////////////
Point checkDirectionInput(void);


#endif