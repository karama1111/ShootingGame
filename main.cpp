#include "DxLib.h"
#include "math.h"
#include "sub.h"
#include "stdlib.h"

#define PI 3.141592
#define MAIN_WINDOW_WIDTH (DEFAULT_SCREEN_SIZE_X - 200)
#define MAIN_WINDOW_HEIGHT DEFAULT_SCREEN_SIZE_Y


//////////////////////////////////////////////
// �O���[�o���ϐ�
//////////////////////////////////////////////
	int stringColor = GetColor(0, 0, 0);

//////////////////////////////////////////////
// �֐��v���g�^�C�v�錾
//////////////////////////////////////////////
	int GameStart();

//////////////////////////////////////////////
// ���C���֐�
//////////////////////////////////////////////
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
			 LPSTR lpCmdLine, int nCmdShow )
{
	// ����
	// �E�C���h�E�T�C�Y��DEFAULT_SCREEN_SIZE_X �� DEFAULT_SCREEN_SIZE_Y


	GameWindow();
 	// �c�w���C�u����������
	if( DxLib_Init() == -1 ) return -1;
	
	// �`���𗠉�ʂɂ���
	SetDrawScreen( DX_SCREEN_BACK ); 

		char KeyBuf[ 256 ] ;
		
		while(1){
			GetHitKeyStateAll( KeyBuf ) ;
			if(KeyBuf[KEY_INPUT_SPACE] != 0){
				GameStart();
			}
			// Windows�V�X�e�����炭�������������(�������[�v�̍ەK�{)
			if( ProcessMessage() == -1 ) break;
			//ESC�L�[�������ꂽ�烋�[�v���甲����(�������[�v�̍ەK�{)
			if( CheckHitKey( KEY_INPUT_ESCAPE ) == 1) break;

			//�X�^�[�g��ʂ̑���
			ClearDrawScreen();
			//DrawExtendGraph( 0, 0, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, bgImg, true );
			DrawString(100, 100, "������@�F", stringColor);
			DrawString(100, 150, "�@�@���E�̖��L�[�ňړ�", stringColor);
			DrawString(100, 200, "�@�@�X�y�[�X�L�[�Œe�𔭎˂��܂��I", stringColor);
			DrawString(100, 300, "�X�y�[�X�L�[�ŊJ�n���܂�(^ ^)", stringColor);
			ScreenFlip();
		}
	// �c�w���C�u�����g�p�̏I��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}

int GameStart(){
	Player player("player.png");
	const int enemiesSize = 10;
	std::vector<Enemy> enemies(enemiesSize, "dora062.png");
	//Enemy *enemy = new Enemy("dora062.png");
	bool playerLoses = 0;
	bool enemyLoses = 0;
	Score score;//�����Ȃ��R���X�g���N�^�́Ascore()�Ƃ��ł͂����Ȃ�
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
		// ��ʂɏ�����Ă�����̂����S������
		ClearDrawScreen();
		DrawExtendGraph( 0, 0, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, bgImg, true );
		// �v���C���[�̒e�𓮂���
		player.moveBullets();
		// �v���C���[�ƓG�̏Փ˔���
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
		// �G�ƒe(�v���C���[)�̏Փ˔���
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
						enemies[ei].kill();//erase( enemies.begin() + ei )����Ƃ����;
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
		// �G�̒e�ƃv���C���[�̏Փ˔���
		for( int i = 0; i < enemiesSize; i++ )
		{
			for( int j = 0; j < enemies[i].bullets.size(); j++ ){
				if( checkCollision( player, enemies[i].bullets[j]) )
				{
					//�����Ńv���C���[����
					player.isVisible = false;
				}
			}
		}

		// �v���C���[���e������
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
		// �G���e������
		for(int i = 0; i < enemiesSize; i++ ){
			if( enemies[i].exists() == false ){continue;}
			if( enemyShootFlag % enemyShootFlagInterval == 0 )
			{
				enemies[i].shoot();
			}
		}
		enemyShootFlag++;

		// �G�̏o��

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
				//boss�o��
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

		// �v���C���[�̈ړ��ƕ\��
		if( player.isVisible  == true ){
			player.move( checkDirectionInput() );
			player.draw();
		}
		player.drawBullets();
		// �G�̈ړ��ƕ\��
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

		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();

		if(player.isVisible == false){
			while(1){
				//�Q�[���I�[�o�[�ƕ\������i�G���^�[�L�[�������Ă��������j
				DrawString(MAIN_WINDOW_WIDTH+5, 30, "�Q�[���I�[�o�[�ł�(>_<)", stringColor);
				DrawString(MAIN_WINDOW_WIDTH+5, 80, "Enter�L�[��", stringColor);
				DrawString(MAIN_WINDOW_WIDTH+5, 100, "�X�^�[�g��ʂɖ߂�܂�!", stringColor);
				ScreenFlip();
				//�G���^�[�L�[�Ȃ�return 0;
				if( CheckHitKey( KEY_INPUT_RETURN ) == 1) return 0;
			}
		}
		if(clearFlag){
			while(1){
				DrawString(MAIN_WINDOW_WIDTH+5, 30, "���߂łƂ��������܂��I", stringColor);
				DrawString(MAIN_WINDOW_WIDTH+5, 55, "�Q�[���N���A�ł��I�I", stringColor);
				DrawString(MAIN_WINDOW_WIDTH+5, 80, "Enter�L�[��", stringColor);
				DrawString(MAIN_WINDOW_WIDTH+5, 100, "�X�^�[�g��ʂɖ߂�܂�!", stringColor);
				ScreenFlip();
				//�G���^�[�L�[�Ȃ�return 0;
				if( CheckHitKey( KEY_INPUT_RETURN ) == 1) return 0;

			}
		}

		// 20�~���b�҂�
		WaitTimer( 20 );

		// Windows�V�X�e�����炭�������������(�������[�v�̍ەK�{)
		if( ProcessMessage() == -1 ) break;

		//ESC�L�[�������ꂽ�烋�[�v���甲����(�������[�v�̍ەK�{)
		if( CheckHitKey( KEY_INPUT_ESCAPE ) == 1) break;
		
	}
	
	return 0;
}

