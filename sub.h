#ifndef DBLDEF1_H_fjds3l3kfj28adl9ja_INCLUDED
#define DBLDEF1_H_fjds3l3kfj28adl9ja_INCLUDED

#include "DxLib.h"
#include <math.h>
#include <vector>
#define PI 3.141592
#define MAIN_WINDOW_WIDTH (DEFAULT_SCREEN_SIZE_X - 200)
#define MAIN_WINDOW_HEIGHT DEFAULT_SCREEN_SIZE_Y

// 参照サイト：http://homepage2.nifty.com/natupaji/DxLib/dxlecture_main.html
// 素材サイト：http://www.presys.jp/


//////////////////////////////////////////////
// Pointクラス
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
// GameObjectクラス
//////////////////////////////////////////////

class GameObject
{
protected:
	int img;
	int x, y;
	int w, h;
	int distance;// 1フレームでの移動距離
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
// Bulletクラス
//////////////////////////////////////////////
class Bullet : public GameObject
{
private:
	int speed;
	int direction;//enumで-1と1にした方がいいのかもしれない
	//↑それか、角度をいれるようにすれば斜めにも打てるけど
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
// Shooter クラス
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
// Playerクラス
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
// Enemyクラス
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
// Bossクラス
//////////////////////////////////////////////

//Bossクラス使えなかった
//Enemyのvectorに入れるとEnemyのメソッドがよびだされてしまうので
//解決策見つかるまで放置

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
// Scoreクラス
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
// 衝突判定関数
//////////////////////////////////////////////
int checkCollision(GameObject, GameObject);
//////////////////////////////////////////////
// 入力されたキーの判定関数
//////////////////////////////////////////////
Point checkDirectionInput(void);


#endif