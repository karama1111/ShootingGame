#include "DxLib.h"
#include "math.h"
#include "sub.h"
#include "stdlib.h"
#include "time.h"
#include "iostream"
#define PI 3.141592
#define MAIN_WINDOW_WIDTH (DEFAULT_SCREEN_SIZE_X - 200)
#define MAIN_WINDOW_HEIGHT DEFAULT_SCREEN_SIZE_Y

// 参照サイト：http://homepage2.nifty.com/natupaji/DxLib/dxlecture_main.html
// 素材サイト：http://www.presys.jp/

//////////////////////////////////////////////
// Pointクラス
//////////////////////////////////////////////
Point::Point( int ax, int ay )
{
		x = ax;
		y = ay;
};

Point::Point()
{
		x = 0;
		y = 0;
};

//////////////////////////////////////////////
// GameWindow
//////////////////////////////////////////////
GameWindow::GameWindow()
{
	backgroundColorR = 255;
	backgroundColorG = 255;
	backgroundColorB = 255;
	windowText = "シューティングゲーム";

	SetBackgroundColor( backgroundColorR, backgroundColorG, backgroundColorB );
	//ウインドウ表示
	ChangeWindowMode( TRUE ) ;
	SetMainWindowText("シューティングゲーム");
};


//////////////////////////////////////////////
// GameObjectクラス
//////////////////////////////////////////////

GameObject::GameObject( char *s )
{
	imgName = s;
	setImage( s );

	GetGraphSize( img, &w, &h );

	x = 100;
	y = 100;

	distance = 10;
}

//////////////////////////////////////////////
// Bulletクラス
//////////////////////////////////////////////

Bullet::Bullet(char *imgName, int aSpeed, int ax, int ay, int aDirection ) : GameObject(imgName)
{
	speed = aSpeed;
	direction = aDirection;
	deleteFlag = false;
	x = ax - w/2;
	if(direction < 0){
		y = ay - h;
	}else{
		y = ay;
	}
	deg = 20;
	rad = deg * PI / 180;
	dx = distance * cos(rad);
	dy = distance * sin(rad);
	int bossMoveCount = 0;
}

bool Bullet::move( void )
{
	y += ( speed * direction );

	//bulletが画面外に出たらdelete
	if( direction > 0 )
	{
		if( y > DEFAULT_SCREEN_SIZE_Y )
		{
			return false;
		}
	}
	else
	{
		if( y < 0 )
		{
			return false;
		}
	}

	return true;
}

bool Bullet::bossMove( void ){

	bossMoveCount++;
	//if(bossMoveCount %2 == 0){
		y += ( speed * direction );
		if((x <= 0) && (dx < 0))
		{
			dx *= -1;
		}
		if((x >= (MAIN_WINDOW_WIDTH-w)) && (dx > 0))
		{
			dx *= -1;
		}
		x+=dx;
	//}

		if( direction > 0 )
	{
		if( y > DEFAULT_SCREEN_SIZE_Y )
		{
			return false;
		}
	}
	else
	{
		if( y < 0 )
		{
			return false;
		}
	}

	return true;

}

//////////////////////////////////////////////
// Shooterクラス
//////////////////////////////////////////////
Shooter::Shooter( char *s) : GameObject( s ), existenceFlag( false )
{

}

void Shooter::shoot( char *imgName, int speed, int x, int y, int direction )
{
		bullets.push_back( Bullet(imgName, speed, x, y, direction) );
}

void Shooter::moveBullets( void )
{
	if(imgName == "boss.png"){
		for( int i = 0; i < bullets.size(); i++){
			if( bullets[i].bossMove() == false)
			{
				deleteBullet( i );
			}
		}

	}else{
		for( int i = 0; i < bullets.size(); i++){
			if( bullets[i].move() == false)
			{
				deleteBullet( i );
			}
		}
	}
}


void Shooter::drawBullets( void )
{
	for( int i = 0; i < bullets.size(); i++){
		bullets[i].draw();
	}
}

void Shooter::deleteBullet( int n )
{
	bullets.erase( bullets.begin() + n );
}

bool Shooter::exists( void ){
	return existenceFlag;
}

void Shooter::emerges( void ){
	existenceFlag = true;
	srand((unsigned int)time(NULL));
 	x = rand() % (MAIN_WINDOW_WIDTH - w);
	y = -h;

}

void Shooter::kill(){
	existenceFlag = false;
}
//////////////////////////////////////////////
// Playerクラス
//////////////////////////////////////////////

Player::Player( char *s ) : Shooter( s ), isVisible( true )
{
	x = ( MAIN_WINDOW_WIDTH - w ) / 2;
	y = MAIN_WINDOW_HEIGHT - h - 20;
	
}

void Player::move( Point p )
{
	x += distance * p.x;
	
	if( x < 0 ){
		x = 0;
	}else if( x > ( MAIN_WINDOW_WIDTH - w ) ){
		x = MAIN_WINDOW_WIDTH - w;
	}

	y += distance * p.y;
	if( y < 0 ){
		y = 0;
	}else if(y > (DEFAULT_SCREEN_SIZE_Y - h) ){
		y = DEFAULT_SCREEN_SIZE_Y - h;
	}
}

void Player::shoot( void )
{
	if( isVisible == true )
	{
		Shooter::shoot("bullet.png", 10, x+ w/2, y, -1);
	}
}

//////////////////////////////////////////////
// Enemyクラス
//////////////////////////////////////////////
Enemy::Enemy(char *s) : Shooter(s)
{
	int deg = 40;
	double rad = deg * PI / 180;
	x = 300;
	y = -h; 
	dx = distance * cos(rad);
	dy = distance * sin(rad);
	bossHP = 240;
}

void Enemy::emerges( void ){
	Shooter::emerges();
	srand((unsigned int)time(NULL));
	if(rand() % 2 == 0){
		dx *= -1;
	}
	if(imgName == "boss.png"){
		x = 160;
	}
}
void Enemy::kill(){
	if(imgName != "boss.png"){
		Shooter::kill();
	}else{
		bossHP -= 1;
		if(bossHP <= 0){
			Shooter::kill();

		}
	}
}
bool Enemy::move(){
	if( existenceFlag == false )
	{
		return false;
	}
	if((x <= 0) && (dx < 0))
	{
		dx *= -1;
	}
	if((x >= (MAIN_WINDOW_WIDTH-w)) && (dx > 0))
	{
		dx *= -1;
	}

	if((y >= (MAIN_WINDOW_HEIGHT)) && (dy > 0))
	{
	//	dy *= -1;
		return false;
	}
	
	if(imgName != "boss.png"){
		y += dy/3;
		x += dx;
	}else{
		if(y<10){
			y += dy/3;
		}
		x += dx/5;
	}
	return true;
}

bool Enemy::isBoss( void )
{
	if(imgName == "boss.png"){
		return true;
	}else{
		return false;
	}
}

void Enemy::shoot( void )
{
	if(imgName == "boss.png"){
		//srand((unsigned int)time(NULL));
 		//int bulletx = rand() % (MAIN_WINDOW_WIDTH - w);
		//std::cout << bulletx;
		//Shooter::shoot("bullet.png", 5, bulletx, y + h, 1);
		Shooter::shoot("bullet.png", 5, x+ w/2, y + h, 1);
	}else{
		Shooter::shoot("bullet.png", 5, x+ w/2, y + h, 1);
	}
}

bool Enemy::isClear( void )
{
	if(bossHP <= 0){
		return true;
	}else{
		return false;
	}
}


//////////////////////////////////////////////
// Bossクラス
//////////////////////////////////////////////
Boss::Boss(char *s) : Enemy(s)
{
	int deg = 40;//後で変更
	double rad = deg * PI / 180;
	x = 300;
	y = -h; 
	dy = 1;
}

void Boss::emerges( void ){
	Shooter::emerges();
}

bool Boss::move(){
	y += dy/3;
	return true;
}

//////////////////////////////////////////////
// Scoreクラス
//////////////////////////////////////////////
Score::Score()
{
	score = 0;
}
void Score::addScore()
{
	score += 100;
}

int Score::getScore()
{
	return score;
}

//////////////////////////////////////////////
// 衝突判定関数
//////////////////////////////////////////////
int checkCollision(GameObject a, GameObject b){
	int ax = a.getX();
	int ay = a.getY();
	int aw = a.getW();
	int ah = a.getH();
	int bx = b.getX();
	int by = b.getY();
	int bw = b.getW();
	int bh = b.getH();

	//w と　h が違うと変な感じになる(wを円の直径みたいに使ってるから)
	if( (( ax + aw/2 ) - ( bx + bw/2 )) * (( ax + aw/2 ) - ( bx + bw/2 ))
			+(( ay + ah/2 ) - ( by + bh/2 )) * (( ay + ah/2 ) - ( by + bh/2 ))
			< ( (aw + bw)/2 )  * ( (aw + bw)/2 ) )
		{
			return 1;
		}else{
			return 0;
		}
}

//////////////////////////////////////////////
// 入力されたキーの判定関数
//////////////////////////////////////////////
Point checkDirectionInput()
{
	int x = 0, y = 0;
	if( CheckHitKey( KEY_INPUT_LEFT ) == 1 )
		{ x -= 1; }
	if( CheckHitKey( KEY_INPUT_RIGHT ) == 1 ) 
		{ x += 1; }
	if( CheckHitKey( KEY_INPUT_UP ) == 1 ) 
		{ y -= 1;}
	if( CheckHitKey( KEY_INPUT_DOWN ) == 1 )
		{ y += 1;}
	Point p( x, y );
	return p;
}

