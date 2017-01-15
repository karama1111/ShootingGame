#include "DxLib.h"
#include "math.h"
#include "sub.h"
#include "stdlib.h"

#define PI 3.141592
#define MAIN_WINDOW_WIDTH (DEFAULT_SCREEN_SIZE_X - 200)
#define MAIN_WINDOW_HEIGHT DEFAULT_SCREEN_SIZE_Y


//////////////////////////////////////////////
// グローバル変数
//////////////////////////////////////////////
	int stringColor = GetColor(0, 0, 0);

//////////////////////////////////////////////
// 関数プロトタイプ宣言
//////////////////////////////////////////////
	int GameStart();

//////////////////////////////////////////////
// メイン関数
//////////////////////////////////////////////
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
			 LPSTR lpCmdLine, int nCmdShow )
{
	// メモ
	// ウインドウサイズはDEFAULT_SCREEN_SIZE_X と DEFAULT_SCREEN_SIZE_Y


	GameWindow();
 	// ＤＸライブラリ初期化
	if( DxLib_Init() == -1 ) return -1;
	
	// 描画先を裏画面にする
	SetDrawScreen( DX_SCREEN_BACK ); 

		char KeyBuf[ 256 ] ;
		
		while(1){
			GetHitKeyStateAll( KeyBuf ) ;
			if(KeyBuf[KEY_INPUT_SPACE] != 0){
				GameStart();
			}
			// Windowsシステムからくる情報を処理する(無限ループの際必須)
			if( ProcessMessage() == -1 ) break;
			//ESCキーが押されたらループから抜ける(無限ループの際必須)
			if( CheckHitKey( KEY_INPUT_ESCAPE ) == 1) break;

			//スタート画面の代わり
			ClearDrawScreen();
			//DrawExtendGraph( 0, 0, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, bgImg, true );
			DrawString(100, 100, "操作方法：", stringColor);
			DrawString(100, 150, "　　左右の矢印キーで移動", stringColor);
			DrawString(100, 200, "　　スペースキーで弾を発射します！", stringColor);
			DrawString(100, 300, "スペースキーで開始します(^ ^)", stringColor);
			ScreenFlip();
		}
	// ＤＸライブラリ使用の終了
	DxLib_End();

	// ソフトの終了
	return 0;
}

int GameStart(){
	Player player("player.png");
	const int enemiesSize = 10;
	std::vector<Enemy> enemies(enemiesSize, "dora062.png");
	//Enemy *enemy = new Enemy("dora062.png");
	bool playerLoses = 0;
	bool enemyLoses = 0;
	Score score;//引数なしコンストラクタは、score()とよんではいけない
	int bgImg = LoadGraph("backGround1.jpg");
	int bulletFlagInterval = 5;
	int bulletFlag = bulletFlagInterval;
	int enemyShootFlagInterval = 20;
	int enemyShootFlag = enemyShootFlagInterval;
	int enemyEmergingFlagInterval = 100;
	int enemyEmergingFlag = enemyEmergingFlagInterval;
	int enemyCounter = 0;
	int enemyNum = 5;
	bool clearFlag = 0;

	while(true){
		// 画面に書かれているものを一回全部消す
		ClearDrawScreen();
		DrawExtendGraph( 0, 0, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, bgImg, true );
		// プレイヤーの弾を動かす
		player.moveBullets();
		// プレイヤーと敵の衝突判定
		for( int i = 0; i < enemiesSize; i++){
			if( checkCollision( player, enemies[i] ) )
			{
				playerLoses = 1;
				player.isVisible = false;
			}
			else
			{
				playerLoses = 0;
			}
		}
		// 敵と弾(プレイヤー)の衝突判定
		for( int bi = 0; bi < player.bullets.size(); bi++ )
		{
			for( int ei = 0; ei < enemiesSize; ei++ ){
				if( enemies[ei].exists()  )
				{
					if( checkCollision( player.bullets[bi], enemies[ei]) )
					{
						enemyLoses = 1;
						score.addScore();
						//DrawString(MAIN_WINDOW_WIDTH, 100, "delete!", stringColor);
						player.bullets[bi].deleteFlag = false;
						player.bullets[bi].deleteFlag = true;
						enemies[ei].kill();//erase( enemies.begin() + ei )するとずれる;
						if(enemies[ei].isClear()){
							clearFlag = 1;
						}
					}
				}
				if(player.bullets[bi].deleteFlag) 
				{
					//score += 100;
				}
			}
		}
		// 敵の弾とプレイヤーの衝突判定
		for( int i = 0; i < enemiesSize; i++ )
		{
			for( int j = 0; j < enemies[i].bullets.size(); j++ ){
				if( checkCollision( player, enemies[i].bullets[j]) )
				{
					//ここでプレイヤー消す
					player.isVisible = false;
				}
			}
		}

		// プレイヤーが弾を撃つ
		if( CheckHitKey( KEY_INPUT_SPACE ) ){
			if(bulletFlag % bulletFlagInterval == 0){
				player.shoot();
				bulletFlag++;
			}else{
				bulletFlag++;
			}
		}else{
			bulletFlag = bulletFlagInterval;
		}
		// 敵が弾を撃つ
		for(int i = 0; i < enemiesSize; i++ ){
			if( enemies[i].exists() == false ){continue;}
			if( enemyShootFlag % enemyShootFlagInterval == 0 )
			{
				enemies[i].shoot();
			}
		}
		enemyShootFlag++;

		// 敵の出現

		if( (enemyEmergingFlag %= enemyEmergingFlagInterval) == 0)
		{
			if(enemyCounter<enemyNum)
			{
				for(int i=0; i<enemiesSize; i++)
				{
					if(enemies[i].exists() == false)
					{
 						enemies[i].emerges();
						enemyCounter++;
						goto enemyEmergingFinished;
					}
				}
			}
			else if (enemyCounter == enemyNum)
			{
				//boss出す
				for(int i=0; i<enemiesSize; i++)
				{
					if(enemies[i].exists() == false)
						{
							enemies[i] = Boss("boss.png");
 							enemies[i].emerges();
							enemyCounter++;
							goto enemyEmergingFinished;
						}
				}
			}
		}

		enemyEmergingFinished:
		enemyEmergingFlag++;

		// プレイヤーの移動と表示
		if( player.isVisible  == true ){
			player.move( checkDirectionInput() );
			player.draw();
		}
		player.drawBullets();
		// 敵の移動と表示
		for( int i = 0; i < enemiesSize; i++ )
		{
			if(enemies[i].move())
			{
				enemies[i].draw();
			}else{
				enemies[i].kill();//erase(enemies.begin() + i);
			}
			enemies[i].moveBullets();
			enemies[i].drawBullets();

		}
		


		DrawFormatString(MAIN_WINDOW_WIDTH+5, 0, stringColor, "score : %d", score.getScore());

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();

		if(player.isVisible == false){
			while(1){
				//ゲームオーバーと表示する（エンターキーを押してください）
				DrawString(MAIN_WINDOW_WIDTH+5, 30, "ゲームオーバーです(>_<)", stringColor);
				DrawString(MAIN_WINDOW_WIDTH+5, 80, "Enterキーで", stringColor);
				DrawString(MAIN_WINDOW_WIDTH+5, 100, "スタート画面に戻ります!", stringColor);
				ScreenFlip();
				//エンターキーならreturn 0;
				if( CheckHitKey( KEY_INPUT_RETURN ) == 1) return 0;
			}
		}
		if(clearFlag){
			while(1){
				DrawString(MAIN_WINDOW_WIDTH+5, 30, "おめでとうございます！", stringColor);
				DrawString(MAIN_WINDOW_WIDTH+5, 55, "ゲームクリアです！！", stringColor);
				DrawString(MAIN_WINDOW_WIDTH+5, 80, "Enterキーで", stringColor);
				DrawString(MAIN_WINDOW_WIDTH+5, 100, "スタート画面に戻ります!", stringColor);
				ScreenFlip();
				//エンターキーならreturn 0;
				if( CheckHitKey( KEY_INPUT_RETURN ) == 1) return 0;

			}
		}

		// 20ミリ秒待つ
		WaitTimer( 20 );

		// Windowsシステムからくる情報を処理する(無限ループの際必須)
		if( ProcessMessage() == -1 ) break;

		//ESCキーが押されたらループから抜ける(無限ループの際必須)
		if( CheckHitKey( KEY_INPUT_ESCAPE ) == 1) break;
		
	}
	
	return 0;
}

