#include "DxLib.h"

const int WIDTH = 1900, HEIGHT =720; // ウィンドウの幅と高さのピクセル数
const int FPS = 60;

enum HORSE{ RED,BLUE,BLACK,WHITE };
enum ABILITY{STAMINA_MAX,LAST_SPEED_UP,FARST_SPEED_UP,GAMBLER,ODDS_UP,SCARF}; // 特性
enum SCENE{START,SELECT,BED,RUN,RESULT,OVER};
enum HORSEMOVE{RED1,BLUE1,BLACK1,WHITE1,RED2,BLUE2,BLACK2,WHITE2,RED3,BLUE3,BLACK3,WHITE3};
const int IMG_HORSE_MAX = 4; // ウマの画像の枚数
const int img_horse_max = 12;
const int img_Human_max = 3;
int imgHorse[IMG_HORSE_MAX]; // ウマの画像
int imgHorse2[img_horse_max];
int imgHuman[img_Human_max];
int imgCourse; // 背景画像
int imgGoal; // ゴール画像
int timer; // タイマー
int StageDistance = 1800; // ステージの長さ
int Scene = START;
int money = 1000; // 最初の持ち金
int Bedmoney; // 掛けられた金額
int Winhorse; // 勝った馬
int Selecthorse; // 選んだ馬
int mouseX, mouseY; // マウスの座標
int dice = -1;
int dice2 = -1;
int clickstate;
int precClickstate;
int BGM, RaceBGM, moneySE, Resultjin,GAMEOVER;
bool pay;
bool horsestate;
bool clickcount;
bool clickcount2;
bool ability;

// ウマを表示する関数
void DrawHorse(int x, int y, int type)
{
	DrawExtendGraph(x, y, x+100, y+100, imgHorse[type], true);
}

// 文字列を表示する関数
void DrawText(int x, int y, int col, const char* txt, int val, int siz)
{
	SetFontSize(siz);
	DrawFormatString(x + 2, y + 2, 0xffffff, txt, val);
	DrawFormatString(x, y, col, txt, val);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	SetWindowText("競馬"); // ウィンドウのタイトル
	SetGraphMode(WIDTH, HEIGHT, 32); // ウィンドウの大きさとカラービット数の指定
	ChangeWindowMode(true); // ウィンドウモードで起動
	if (DxLib_Init() == -1)return -1; // ライブラリ初期化　エラーが起きたら終了
	SetBackgroundColor(255, 255, 255); // 背景色の指定
	SetDrawScreen(DX_SCREEN_BACK); // 描画面を裏画面にする

	// 画像を読み込む
	imgHorse[RED] = LoadGraph("image/赤馬１.png");
	imgHorse[BLUE] = LoadGraph("image/青馬１.png");
	imgHorse[BLACK] = LoadGraph("image/黒馬１.png");
	imgHorse[WHITE] = LoadGraph("image/白馬１.png");

	imgHorse2[RED1] = LoadGraph("image/赤馬１.png");
	imgHorse2[RED2] = LoadGraph("image/赤馬２.png");
	imgHorse2[RED3] = LoadGraph("image/赤馬3.png");
	imgHorse2[BLUE1] = LoadGraph("image/青馬１.png");
	imgHorse2[BLUE2] = LoadGraph("image/青馬２.png");
	imgHorse2[BLUE3] = LoadGraph("image/青馬3.png");
	imgHorse2[BLACK1] = LoadGraph("image/黒馬１.png");
	imgHorse2[BLACK2] = LoadGraph("image/黒馬２.png");
	imgHorse2[BLACK3] = LoadGraph("image/黒馬3.png");
	imgHorse2[WHITE1] = LoadGraph("image/白馬１.png");
	imgHorse2[WHITE2] = LoadGraph("image/白馬２.png");
	imgHorse2[WHITE3] = LoadGraph("image/白馬3.png");

	imgHuman[0] = LoadGraph("image/絶望人間1.png");
	imgHuman[1] = LoadGraph("image/絶望人間2.png");
	imgHuman[2] = LoadGraph("image/絶望人間3.png");
	imgCourse = LoadGraph("image/背景.png");
	imgGoal = LoadGraph("image/ゴール.png");

	// BGMを読み込む
	BGM = LoadSoundMem("music/BGM.wav");
	RaceBGM = LoadSoundMem("music/RaceBGM.wav");
	moneySE = LoadSoundMem("music/お金.wav");
	Resultjin = LoadSoundMem("music/Result.wav");
	GAMEOVER = LoadSoundMem("music/GAMEOVER.wav");

	// ウマのステータス
	float Speed[IMG_HORSE_MAX], Speedup[IMG_HORSE_MAX], Stamina[IMG_HORSE_MAX], Odds[IMG_HORSE_MAX], Position[IMG_HORSE_MAX], KeepStamina[IMG_HORSE_MAX], Ability[IMG_HORSE_MAX];
	for (int i = 0; i < IMG_HORSE_MAX; i++)
	{
		Speed[i] = GetRand(100) / 100.0 + 0.1;
		Speedup[i] = GetRand(100) / 100.0 + 0.1;
		Stamina[i] = GetRand(200) + 200;
		Odds[i] = GetRand(10) + 1;
		Position[i] = 0;
		KeepStamina[i] = Stamina[i];
		Ability[i] = GetRand(5);
	}

	while (1) // メインループ
	{
		ClearDrawScreen(); // 画面をクリアする
		timer++;
		GetMousePoint(&mouseX,&mouseY);
		precClickstate = clickstate;
		clickstate = GetMouseInput() & MOUSE_INPUT_LEFT;
		if (Scene == RUN)
		{
			DrawGraph(0, 0,imgCourse,true);
			DrawGraph(1800, 0, imgGoal, true);
			DrawLine(StageDistance * 0.5, 0, StageDistance * 0.5, 720, 0x000000);
			if (timer >= 0 && timer < 60)
			{
				DrawText(900, 300, 0xffffff, "3",0, 100);
			}
			else if (timer >= 60 && timer < 120)
			{
				DrawText(900, 300, 0xffffff, "2", 0, 100);
			}
			else if (timer >= 120 && timer < 180)
			{
				DrawText(900, 300, 0xffffff, "1", 0, 100);
			}
			else if (timer >= 180 && timer < 200)
			{
				DrawText(900, 300, 0xffffff, "GO", 0, 100);
			}
			if (timer <= 180)
			{
				for (int i = 0; i < IMG_HORSE_MAX; i++)
				{
					DrawHorse(Position[i], i * 200, i);
				}
			}
			if (timer > 180)
			{
				// ウマを動かす
				for (int i = 0; i < IMG_HORSE_MAX; i++)
				{
					DrawHorse(Position[i], i * 200, i);
					DrawBox(Position[Selecthorse], Selecthorse * 200, Position[Selecthorse] + 100, Selecthorse * 200 + 100, 0x000000, false);
					Position[i] += Speed[i];
					if (Ability[i] == FARST_SPEED_UP)
					{
						Stamina[i] -= 3;
					}
					else
					{
						Stamina[i]--;
					}
					if (Stamina[i] == 0 && Ability[i] != STAMINA_MAX)
					{
						Speed[i] /= 2;
						Stamina[i] = KeepStamina[i];
					}
					if (Position[i] >= StageDistance * 0.6 && Ability[i] == LAST_SPEED_UP)
					{
						Speed[i] += 1;
						Ability[i] = -1;
					}
					if (Position[i] <= StageDistance * 0.2 && Ability[i] == FARST_SPEED_UP)
					{
						Speed[i] += 1;
						Ability[i] = -1;
					}
					if (Position[i] >= StageDistance * 0.5 && Ability[i] == GAMBLER)
					{
						int rand = GetRand(1);
						if (rand == 1)
						{
							Speed[i] *= 3;
							Speedup[i] *= 3;
							Stamina[i] *= 3;
							Odds[i] *= 3;
							Ability[i] = -1;
						}
						else if(rand == 0)
						{
							Speed[i] = 0;
							Speedup[i] /= 2;
							Stamina[i] /= 2;
							Odds[i] = 0;
							Ability[i] = -1;
						}
					}
					if (Ability[i] == ODDS_UP)
					{
						if (timer % 120 == 0)
						{
							Odds[i] += GetRand(3) + 1;
						}
					}
					if (Ability[i] == SCARF && ability == false)
					{
						Speed[i] += 2;
						ability = true;
					}
				}
				if (timer % 120 == 0)
				{
					for (int i = 0; i < IMG_HORSE_MAX; i++)
					{
						if (Ability[i] != SCARF)
						{
							Speed[i] += Speedup[i];
						}
					}
				}
				if (timer % 15 <= 4 && timer)
				{
					imgHorse[RED] = imgHorse2[RED2];
					imgHorse[BLUE] = imgHorse2[BLUE2];
					imgHorse[BLACK] = imgHorse2[BLACK2];
					imgHorse[WHITE] = imgHorse2[WHITE2];
				}
				else if (timer % 15 >= 5 && timer % 15 <= 9)
				{
					imgHorse[RED] = imgHorse2[RED1];
					imgHorse[BLUE] = imgHorse2[BLUE1];
					imgHorse[BLACK] = imgHorse2[BLACK1];
					imgHorse[WHITE] = imgHorse2[WHITE1];
				}
				else if (timer % 15 >= 9)
				{
					imgHorse[RED] = imgHorse2[RED3];
					imgHorse[BLUE] = imgHorse2[BLUE3];
					imgHorse[BLACK] = imgHorse2[BLACK3];
					imgHorse[WHITE] = imgHorse2[WHITE3];
				}
			}
		}

		switch (Scene)
		{
		case START: // タイトル画面の処理
			DrawText(750, 160, 0x000000, "RUN FORSE", 0, 100);
			DrawText(800, 600, 0x000000, "ここをクリック", 0, 50);
			DrawBox(800, 600, 1150, 650, 0x000000,false);
			if (timer == 1)
			{
				PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);
			}
			if (clickstate != 0 && precClickstate == 0)
			{
				if (mouseX >= 800 && mouseX <= 1150 && mouseY >= 600 && mouseY <= 650)
				{
					Scene = SELECT;
				}
			}
			break;

		case SELECT: // セレクト画面の処理
			// ウマのステータスの設定
			if (horsestate == false)
			{
				for (int i = 0; i < IMG_HORSE_MAX; i++)
				{
					Speed[i] = GetRand(100) / 100.0 + 0.1;
					Speedup[i] = GetRand(100) / 100.0 + 0.1;
					Stamina[i] = GetRand(200) + 200;
					Odds[i] = GetRand(10) + 1;
					Position[i] = 0;
					KeepStamina[i] = Stamina[i];
					Ability[i] = GetRand(5);
				}
				horsestate = true;
			}
			for (int i = 0; i < IMG_HORSE_MAX; i++)
			{
				DrawHorse(Position[i], i * 200, i);
			}
			DrawText(200, 0, 0x000000, "SPEED", 0, 25);
			DrawText(400, 0, 0x000000, "UPSPEED", 0, 25);
			DrawText(600, 0, 0x000000, "STAMINA", 0, 25);
			DrawText(800, 0, 0x000000, "ODDS", 0, 25);
			DrawText(1000, 0, 0x000000, "ABILITY", 0, 25);
			DrawBox(1200, 310, 1800, 410, 0x000000, false);
			if (money < 10000)
			{
				DrawText(1200, 350, 0x000000, "100円払ってウマのステータスを見る(最大2つ)", 0, 25);
			}
			else if(money < 100000)
			{
				DrawText(1200, 350, 0x000000, "1000円払ってウマのステータスを見る(最大2つ)", 0, 25);
			}
			else
			{
				DrawText(1200, 350, 0x000000, "10000円払ってウマのステータスを見る(最大2つ)", 0, 25);
			}
			DrawText(1200, 680, 0x000000, "所持金:%d", money, 25);
			
			if (clickstate != 0 && precClickstate == 0)
			{
				// ステータス情報処理
				if (mouseX >= 1200 && mouseX <= 1800 && mouseY >= 310 && mouseY <= 410 && clickcount == false)
				{
					dice = GetRand(4);
					PlaySoundMem(moneySE, DX_PLAYTYPE_BACK);
					clickcount = true;
					if (money < 10000)
					{
						money -= 100;
					}
					else if (money < 100000)
					{
						money -= 1000;
					}
					else
					{
						money -= 10000;
					}
				}
				else if (mouseX >= 1200 && mouseX <= 1800 && mouseY >= 310 && mouseY <= 410 && clickcount2 == false && clickcount == true)
				{
					dice2 = GetRand(4);
					money -= 100;
					PlaySoundMem(moneySE, DX_PLAYTYPE_BACK);
					clickcount2 = true;
				}

				for (int i = 0; i < IMG_HORSE_MAX; i++)
				{
					if (mouseX >= Position[i] && mouseX <= Position[i] + 100 && mouseY >= i * 200 && mouseY <= i * 200 + 100)
					{
						Selecthorse = i;
						Scene = BED;
					}
				}
			}
			if (dice == 0 || dice2 == 0)
			{
				for (int i = 0; i < IMG_HORSE_MAX; i++)
				{
					DrawFormatString(200, 50 + (i * 200), 0x000000, "%f", Speed[i]);
				}
				if (dice == 0 && dice2 == 0)
				{
					dice2 = 1;
				}
			}
			if (dice == 1 || dice2 == 1)
			{
				for (int i = 0; i < IMG_HORSE_MAX; i++)
				{
					DrawFormatString(400, 50 + (i * 200), 0x000000, "%f", Speedup[i]);
				}if (dice == 1 && dice2 == 1)
				{
					dice2 = 2;
				}
			}
			if (dice == 2 || dice2 == 2)
			{
				for (int i = 0; i < IMG_HORSE_MAX; i++)
				{
					DrawFormatString(600, 50 + (i * 200), 0x000000, "%f", Stamina[i]);
				}if (dice == 2 && dice2 == 2)
				{
					dice2 = 3;
				}
			}
			if (dice == 3 || dice2 == 3)
			{
				for (int i = 0; i < IMG_HORSE_MAX; i++)
				{
					DrawFormatString(800, 50 + (i * 200), 0x000000, "%f", Odds[i]);
				}if (dice == 3 && dice2 == 3)
				{
					dice2 = 4;
				}
			}
			if (dice == 4 || dice2 == 4)
			{
				for (int i = 0; i < IMG_HORSE_MAX; i++)
				{
					if (Ability[i] == 0)
					{
						DrawFormatString(1000, 50 + (i * 200), 0x000000, "スタミナが減らない", 0, 25);
					}
					if (Ability[i] == 1)
					{
						DrawFormatString(1000, 50 + (i * 200), 0x000000, "ステージ後半でスピードアップ", 0, 25);
					}
					if (Ability[i] == 2)
					{
						DrawFormatString(1000, 50 + (i * 200), 0x000000, "最初だけスピードアップ　スタミナが減りやすくなる", 0, 25);
					}
					if (Ability[i] == 3)
					{
						DrawFormatString(1000, 50 + (i * 200), 0x000000, "ステージ半分を超えると全ステータス0.5倍or3倍", 0, 25);
					}
					if (Ability[i] == 4)
					{
						DrawFormatString(1000, 50 + (i * 200), 0x000000, "走っている間オッズが上がる", 0, 25);
					}
					if (Ability[i] == 5)
					{
						DrawFormatString(1000, 50 + (i * 200), 0x000000, "最初だけスピードアップ　upspeedの値が０になる", 0, 25);
					}
					
				}if (dice == 4 && dice2 == 4)
				{
					dice2 = 0;
				}
			}
			break;

		case BED:
			DrawText(800, 0, 0x000000, "賭けるお金:%d円", Bedmoney, 25);
			DrawText(800, 650, 0x000000, "持っているお金:%d円", money, 25);
			DrawText(600, 100, 0x000000, "+100円", 0, 50);
			DrawText(600, 300, 0x000000, "-100円", 0, 50);
			DrawText(800, 100, 0x000000, "+500円", 0, 50);
			DrawText(800, 300, 0x000000, "-500円", 0, 50);
			DrawText(1000, 100, 0x000000, "+1000円", 0, 50);
			DrawText(1000, 300, 0x000000, "-1000円", 0, 50);
			DrawText(750, 200, 0x000000, "オールイン", 0, 50);
			DrawText(750, 400, 0x000000, "キャンセル", 0, 50);
			DrawText(825, 500, 0x000000, "決定", 0, 50);
			DrawBox(600, 100, 760, 150, 0x000000, false);
			DrawBox(600, 300, 760, 350, 0x000000, false);
			DrawBox(800, 100, 960, 150, 0x000000, false);
			DrawBox(800, 300, 960, 350, 0x000000, false);
			DrawBox(1000, 100, 1180, 150, 0x000000, false);
			DrawBox(1000, 300, 1180, 350, 0x000000, false);
			DrawBox(800, 500, 960, 550, 0x000000, false);
			DrawBox(750, 200, 1010, 250, 0x000000, false);
			DrawBox(750, 400, 1010, 450, 0x000000, false);
			if (clickstate != 0 && precClickstate == 0)
			{
				if (mouseX >= 600 && mouseX <= 760 && mouseY >= 100 && mouseY <= 150 && money >= 100)
				{
					money -= 100;
					Bedmoney += 100;
					PlaySoundMem(moneySE, DX_PLAYTYPE_BACK);
				}
				if (mouseX >= 600 && mouseX <= 760 && mouseY >= 300 && mouseY <= 350 && Bedmoney >= 100)
				{
					money += 100;
					Bedmoney -= 100;
					PlaySoundMem(moneySE, DX_PLAYTYPE_BACK);
				}
				if (mouseX >= 800 && mouseX <= 960 && mouseY >= 100 && mouseY <= 150 && money >= 500)
				{
					money -= 500;
					Bedmoney += 500;
					PlaySoundMem(moneySE, DX_PLAYTYPE_BACK);
				}
				if (mouseX >= 800 && mouseX <= 960 && mouseY >= 300 && mouseY <= 350 && Bedmoney>= 500)
				{
					money += 500;
					Bedmoney -= 500;
					PlaySoundMem(moneySE,DX_PLAYTYPE_BACK);
				}
				if (mouseX >= 1000 && mouseX <= 1180 && mouseY >= 100 && mouseY <= 150 && money >=1000)
				{
					money -= 1000;
					Bedmoney += 1000;
					PlaySoundMem(moneySE, DX_PLAYTYPE_BACK);
				}
				if (mouseX >= 1000 && mouseX <= 1180 && mouseY >= 300 && mouseY <= 350 && Bedmoney >= 1000)
				{
					money += 1000;
					Bedmoney -= 1000;
					PlaySoundMem(moneySE, DX_PLAYTYPE_BACK);
				}
				if (mouseX >= 750 && mouseX <= 1010 && mouseY >= 200 && mouseY <= 250 && money != 0)
				{
					Bedmoney += money;
					money = 0;
					PlaySoundMem(moneySE, DX_PLAYTYPE_BACK);
				}
				if (mouseX >= 750 && mouseX <= 1010 && mouseY >= 400 && mouseY <= 450 && Bedmoney != 0)
				{
					money += Bedmoney;
					Bedmoney = 0;
					PlaySoundMem(moneySE, DX_PLAYTYPE_BACK);
				}
				if (mouseX >= 800 && mouseX <= 960 && mouseY >= 500 && mouseY <= 550)
				{
					timer = 0;
					StopSoundMem(BGM);
					PlaySoundMem(RaceBGM, DX_PLAYTYPE_LOOP);
					Scene = RUN;
				}
			}
			break;


		case RUN: // ゲーム画面の処理
			for (int i = 0; i < IMG_HORSE_MAX; i++)
			{
				if (Position[i] >= 1800)
				{
					Winhorse = i;
					Scene = RESULT;
					StopSoundMem(RaceBGM);
					PlaySoundMem(Resultjin,DX_PLAYTYPE_BACK);
					timer = 0;
				}
			}
			break;

		case RESULT:
			DrawText(700, 50, 0x000000, "勝ったのは……", 0, 100);
			if (timer >= 80)
			{
			DrawExtendGraph(900, 150, 1100, 350, imgHorse[Winhorse], true);
			}
			if (Winhorse == Selecthorse && pay == false)
			{
				money += Bedmoney * Odds[Selecthorse];
				pay = true;
			}
			if (timer >= 170)
			{
				DrawText(900, 400, 0x000000, "賭けたお金　%d円", Bedmoney, 25);
			}
			if (Winhorse == Selecthorse)
			{
				if (timer >= 210)
				{
					DrawText(925, 450, 0x000000, "オッズ　%d倍", Odds[Selecthorse], 25);
				}
				if (timer >= 210)
				{
					DrawText(900, 500, 0x000000, "増えたお金　%d円", Bedmoney* Odds[Selecthorse], 25);
				}
				if (timer >= 210)
				{
					DrawText(850, 550, 0x000000, "所持金　%d円", money, 50);
				}
				if (timer >= 240 && money == 0)
				{
					DrawText(925, 650, 0x000000, "……………", 0, 25);
					DrawBox(900, 650, 1075, 675, 0x000000, false);
				}
				else if (timer >= 240)
				{
					DrawText(925, 650, 0x000000, "次のレース", 0, 25);
					DrawBox(900, 650, 1075, 675, 0x000000, false);
				}

				if (clickstate != 0 && precClickstate == 0)
				{
					if (mouseX >= 900 && mouseX <= 1075 && mouseY >= 650 && mouseY <= 675 && money != 0)
					{
						horsestate = false;
						clickcount = false;
						clickcount2 = false;
						pay = false;
						dice = -1;
						dice2 = -1;
						Bedmoney = 0;
						PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);
						Scene = SELECT;
					}
					else if (mouseX >= 900 && mouseX <= 1075 && mouseY >= 650 && mouseY <= 675 && money == 0)
					{
						timer = 0;
						horsestate = false;
						clickcount = false;
						clickcount2 = false;
						pay = false;
						dice = -1;
						dice2 = -1;
						Bedmoney = 0;
						PlaySoundMem(GAMEOVER, DX_PLAYTYPE_BACK);
						Scene = OVER;
					}
				}
			}
			else
			{
				if (timer >= 210)
				{
					DrawText(850, 500, 0x000000, "所持金　%d円", money, 50);
				}
				if (timer >= 240 && money == 0)
				{
					DrawText(925, 650, 0x000000, "……………", 0, 25);
					DrawBox(900, 650, 1075, 675, 0x000000, false);
				}
				else if (timer >= 240)
				{
					DrawText(925, 650, 0x000000, "次のレース", 0, 25);
					DrawBox(900, 650, 1075, 675, 0x000000, false);
				}
				if (clickstate != 0 && precClickstate == 0)
				{
					if (mouseX >= 900 && mouseX <= 1075 && mouseY >= 650 && mouseY <= 675 && money != 0)
					{
						horsestate = false;
						clickcount = false;
						clickcount2 = false;
						pay = false;
						dice = -1;
						dice2 = -1;
						Bedmoney = 0;
						PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);
						Scene = SELECT;
					}
					else if (mouseX >= 900 && mouseX <= 1075 && mouseY >= 650 && mouseY <= 675 && money == 0)
					{
						timer = 0;
						horsestate = false;
						clickcount = false;
						clickcount2 = false;
						pay = false;
						dice = -1;
						dice2 = -1;
						Bedmoney = 0;
						PlaySoundMem(GAMEOVER, DX_PLAYTYPE_BACK);
						Scene = OVER;
					}
				}
			}
			break;

		case OVER:
			DrawText(700, 100, 0x000000, "GAME OVER", 0, 100);
			if (timer <= 50)
			{
				DrawExtendGraph(800, 300, 1000, 500, imgHuman[0], true);
			}
			else if(timer <= 100)
			{
				DrawExtendGraph(800, 300, 1000, 500, imgHuman[1], true);
			}
			else if (timer > 100)
			{
				DrawExtendGraph(800, 300, 1000, 500, imgHuman[2], true);
			}
			DrawText(800, 600, 0x000000, "コンテニュー", 0, 50);
			DrawBox(800, 600, 1110, 650, 0x000000, false);
			if (clickstate != 0 && precClickstate == 0 && mouseX >= 800 && mouseX <= 1110 && mouseY >= 600 && mouseY <= 650)
			{
				timer = 0;
				money = 1000;
				Scene = START;
			}
			break;
		}
		ScreenFlip(); // 裏画面の内容を表画面に反映させる
		WaitTimer(1000 / FPS); // 一定時間待つ
		if (ProcessMessage() == -1) break; // Windowsから情報を受け取りエラーが起きたら終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break; // ESCキーが押されたら終了
	}
	DxLib_End(); // DXライブラリ使用の終了処理
	return 0; // ソフトの終了
}
