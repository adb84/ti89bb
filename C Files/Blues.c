//Blues Brothers for ti89
//Adrien BELLY
//V1.00 2019

#define  HEIGHT 100
#define  WIDTH  160
#define  LARG 128 // map width in tiles (16x16 pixels)
#define  HAUT 86  // map height in tiles (16x16 pixels)
#define  DEBUG 0 // 1: debug mode (slows down) 0: normal mode

#include "C:\TIGCC\demo\MyMap.h"
#include "tigcclib.h"
#include "extgraph.h"
#include "tilemap.h"

	//pointers to uncompressed sprites :
	unsigned char*  sprites =NULL; // 255 tiles (16x16 pixels)
	unsigned long*  p_array32[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	unsigned short* p_array16[] = {NULL, NULL, NULL, NULL, NULL, NULL};

	short intro=0; //intro timer
  short x_bg=64,y_bg=1072; //initial background position
  short x_pos=88, y_pos=32; //initial player position
  short x_bg_target=64,y_bg_target=1072;//initial background position (target)
  short left_and_right_pressed = 0;// keep last direction in memory in order to reverse it when both right and left keys are pressed
  short tst = 0;
  short j=0, seq=0,seq2=0, seq3=0,seq5=0,seq6=0,seq8=0,seq9=0;
  short k=0;//for enemies sprites
  short antirebond  = 0;
  short antirebond2 = 0;
  short antirebond3 = 0;//prevent launch right after holding a box
  short antirebond4 = 0;//for debug
  const short enum_seq[6]={0,1,3,0,1,2};//normal tile sequence for mode 1 (walking)
  const short enum_seq_box[6]={13,15,13,15,13,15};//special tile sequence for mode 1 (walking while holding a box)
  const short enum_seq2[4]={8,8,9,9};//tile sequence for mode 2 (climbing)
  const short enum_seq3[6]={16,10,10,8,4,2};//offset sequence for mode 3 (jumping) and mode 4 (falling)
  const short enum_seq6[4]={5,5,6,6};//tile sequence for mode 6 (crawling)
  const short enum_seq8[39]={19,19,19,30,30,30,31,31,31,32,32,32,27,27,27,28,28,28,29,29,29,27,27,27,28,28,28,29,29,29,27,27,27,28,28,28,29,29,29};//tile sequence for mode 8 (bumped)
  const short enum_seq9[80]={34,34,34,34,35,35,35,35,36,36,36,36,37,37,37,37,38,38,38,38,34,34,34,34,35,35,35,35,36,36,36,36,37,37,37,37,38,38,38,38,34,34,34,34,35,35,35,35,36,36,36,36,37,37,37,37,38,38,38,38,34,34,34,34,35,35,35,35,36,36,36,36,37,37,37,37,38,38,38,38};//tile sequence for mode 9 (dance)
  short stop = 0;//stop =1 to quit
  short mode = 0;
   /* Modes :
    0: stopped
    1: walking
    2: climbing
    3: jumping
    4: falling
    5: pass through floor
    6: crawling
    7: teleport
    8: bumped
    9: dance
   10: swimm
   11: fly with balloon
   12: gameover
   13: reset level
   14: intro
  */
 	short n=0, m=0;
	short pas = 8;
	short slowdown = 5;
	short display_slowdown = 0;
  short disque_u = 0; // disk counter (unit)
  short disque_d = 0; // disk counter (decade)
  short vie = 3;//3
  short biglife = 2;//2
  short reset_level = 0;//timer until level is reset
  short item = 0; //0:found nothing  1:found guitar
  short blink_item = 0;// 1:guitar is blinking when player hit flag witout item found
  short immortal=0;//0:normal 1-100:immortal counter (in images)
  short x_elevator[5]       ={37*16, 94*16, 100*16, 94*16, 100*16};
  short y_elevator[5]       ={52*16, 44*16, 53*16 , 27*16, 33*16 };
  short y_elevator_bottom[5]={52*16, 44*16, 54*16 , 28*16, 34*16 };//elevators bottom positions
  short y_elevator_top[5]   ={36*16, 34*16, 43*16 , 23*16, 28*16 };//elevators top positions
  short s_elevator[5]       ={4    , 4    , 4     , 4    , 4     };//elevators modes 0:static  1:to the right  2:to the left  3:down  4: up
  short v_elevator[5]       ={2    , 2     ,2     , 2    , 2     };//elevators speeds (pixels/image)
  short elevator_delay[5]   ={0    , 0     ,0     , 0    , 0     };//delay between up and down movements
  short hold_object = 0; //0:hold nothing  1:hold a box  2:hold an umbrella
  short x_box=-1;
  short x_box_target = -1;
  short y_box=-1;
	short box_speed =12;
	#define nb_enemy 22 //total number of enemy (next arrays)
	short g_enemy[]        ={    1,     1,     1 ,     2 ,     2,     3,     3,     3,     3,     3,     4,       4,     5,     5,     5,     5,     5,     5,     6,     6,     7,     7};//Group 1:Grandma 2:Fish 3:Cop 4:Woman 5:Junky 6:Farmer 7:Bird
  short x_enemy[]        ={47*16, 73*16, 65*16 , 46*16 , 68*16, 29*16, 89*16, 23*16, 97*16, 83*16,  8*16,101*16+8, 22*16, 62*16, 52*16,  5*16, 70*16, 68*16, 56*16, 37*16, 23*16, 59*16};
  short y_enemy[]        ={69*16, 69*16, 61*16 , 34*16 , 34*16, 69*16, 69*16, 61*16, 32*16, 21*16, 39*16,   49*16, 81*16, 81*16, 61*16, 30*16, 49*16, 46*16, 21*16, 21*16,  5*16,  3*16};
  short x_enemy_left[]   ={47*16, 73*16, 65*16 , 46*16 , 68*16, 29*16, 89*16, 23*16, 97*16, 83*16,  8*16,101*16+8, 22*16, 62*16, 52*16,  5*16, 70*16, 68*16, 56*16, 37*16, 23*16, 59*16};//left limit
  short x_enemy_right[]  ={68*16, 84*16, 84*16 , 58*16 , 80*16, 43*16,101*16, 47*16, 98*16, 92*16, 15*16,  102*16, 38*16, 78*16, 60*16, 18*16, 93*16, 81*16, 70*16, 50*16, 39*16, 74*16};//right limit
  short m_enemy[]        ={1    , 1    , 1     , 1     , 1    , 1    , 1    , 1    , 1    , 1    , 1    , 1      , 1    , 1    , 1    , 1    , 1    , 1    , 1    , 1    , 1    , 1    };//mode 0:dead  1:to the right  2:to the left  3:dying 4: shooting to the right 5: shooting to the left
  short seq_enemy[]      ={0    , 0    , 0     , 0     , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0      , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    };//enemy sequence
  short r_enemy[]        ={0    , 0    , 0     , 0     , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0      , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    };//reversed ?
  short shooting[]       ={0    , 0    , 0     , 0     , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0      , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    };//shooting sequence
  short x_bullet[]       ={0    , 0    , 0     , 0     , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0      , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    };//bullet x position 
  short y_bullet[]       ={0    , 0    , 0     , 0     , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0      , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    };//bullet y position
  short dying_seq[]      ={0    , 0    , 0     , 0     , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0      , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    };//dying seq  
  short dying_tile[] ={0,0,2,29,15,18,9};//dying tile (Grandma(tile0) Fish(not dying)Cop(tile 2) Woman(tile29) Junky(tile) Farmer (tile) Bird (tile)
  const short enum_seq_enemy[] ={0,0,1,1};//Grandma
  const short enum_seq_enemy3[] ={2,3,4,5,4,3};//Cop walking
  const short enum_seq_enemy4[] ={24,25,26,27,26,25};//Woman walking
  const short enum_seq_enemy5[] ={15,14,16,32,16,14};//Junky walking
  const short enum_seq_enemy6[] ={18,17,19,20,19,17};//Farmer walking
  const short enum_seq_enemy7[] ={9,11,10,11};//Bird flying
  const short x_enum_dying_seq[]={6,12,18,24,30,36,42,48,54,60,66,72,78,84};
  const short y_enum_dying_seq[]={-10,-19,-26,-30,-32,-30,-26,-19,-10,0,10,19,26,30};

  short curtain = 0; //0: disabled  1-6: positions
  short m_curtain = 0; //mode 0:close 1:open
 
  short x_teleport[]  ={19*16,36*16,39*16,89*16,75*16,70*16,75*16,53*16,58*16,29*16};
  short y_teleport[]  ={81*16,69*16,61*16,81*16,61*16,81*16,69*16,81*16,69*16,81*16};
  short xbg_teleport[]={11*16,30*16,34*16,83*16,70*16,65*16,68*16,46*16,51*16,22*16};
  short ybg_teleport[]={79*16,66*16,58*16,78*16,58*16,78*16,67*16,78*16,67*16,78*16};
  short enum_p_teleport[]  ={1,0,3,2,5,4,7,6,9,8};
  short p_teleport=0;
                      //   ?=dance  ,guitar,+1 life,?=-20  ,?=+50  ,balloon1,balloon2,balloon3,balloon4,umbrella1,umbrella2,umbrella3,flag1 ,flag2
  short x_special_pos[] = { 100*16-8,    94*16,  47*16,  63*16,  24*16,   25*16,   24*16,   23*16,   32*16,    17*16,    12*16,    10*16,   116*16,   116*16,   125*16};//x position
  short y_special_pos[] = { 70*16-16,    81*16,  40*16,  33*16,  48*16,   54*16,   54*16,   54*16,   34*16,    18*16,    13*16,     7*16,    68*16,    69*16,    69*16};//y position
  short c_special_pos[] = {        1,        2,      3,      4,      5,       6,       6,       6,       6,        7,        7,        7,        8,        8,        8};//switch case value (0: disabled)
  
  const short g_animated_tile_pos[]       = {  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  3,  2,  3,  2,  3,  2,  3,  4,  4,  5,  6,  7,  7,  7,  7,  7,  6,  6,  6,  6,  6,  7,  7,  7,  7};//group (1:camera/2:right axe/3:left axe/4:flag/5:guitar/6:Right moving walkway/7:Left moving walkway)
  const short x_animated_tile_pos[]       = { 25, 94, 57, 87, 72, 45, 60, 29, 85, 86, 89, 86, 89, 84, 83, 80, 84,116,125, 94, 85, 84, 85, 86, 87, 88, 81, 82, 83, 84, 85, 85, 86, 87, 88};//x position
  const short y_animated_tile_pos[]       = { 64, 55, 55, 26, 26, 26, 26, 28, 64, 36, 39, 42, 45, 36, 39, 42, 45, 69, 70, 81, 36, 39, 39, 39, 39, 39, 42, 42, 42, 42, 42, 45, 45, 45, 45};//y position
  short s_animated_tile_pos[]       = {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};//current position in animation sequence
  const short seq_animated_tile_group1[]  = {184,184,184,185,185,185,186,186,186,185,185,185};
  const short seq_animated_tile_group2[]  = {253,252,251};
  const short seq_animated_tile_group3[]  = {240,241,242};
  const short seq_animated_tile_group4[]  = {158,174};
  const short seq_animated_tile_group5[]  = {159,165};
  const short seq_animated_tile_group6[]  = {248,249,250};
  const short seq_animated_tile_group7[]  = {239,238,237};
  
  short x_swimm_speed = 0;
  short y_swimm_speed = 0;
  short swimm_mode = 0 ; //0:static 1:to the right 2: to the surface
  short x_bubble = 0;
  short y_bubble = 0;
  
  short showtiles = 0; //for debug
	static const unsigned char prop[256]= //blocks properties 0=air 1=solid 2=step
	{
	//000,001,002,003,004,005,006,007,008,009,010,011,012,013,014,015,016,017,018,019,020,021,022,023,024,025,026,027,028,029,030,031,032,033,034,035,036,037,038,039,040,041,042,043,044,045,046,047,048,049,050,051,052,053,054,055,056,057,058,059,060,061,062,063,064,065,066,067,068,069,070,071,072,073,074,075,076,077,078,079,080,081,082,083,084,085,086,087,088,089,090,091,092,093,094,095,096,097,098,099,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
	  000,000,000,000,000,000,000,001,001,001,001,000,000,001,001,001,001,001,000,002,000,000,000,000,000,000,000,000,002,002,000,000,000,000,002,002,002,001,000,000,000,000,000,000,000,000,000,000,000,000,002,002,002,000,000,002,000,002,002,002,002,002,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,002,002,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,001,001,001,000,000,001,000,001,000,000,001,001,002,000,000,000,000,000,000,000,000,000,000,000,000,000,001,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,001,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,001,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,001,000,000,000,002,002,000,001,001,001,001,001,001,000,000,000,000,000,001,001,001,001,001,001,000,000
	};

  short reversed = 0;
	char info1[15]= "";
	char info2[15]= "";
	char info3[15]= "";
	char info4[15]= "";

	unsigned short cpt = 0;
	unsigned	char nb=0;
	unsigned	char nb2=0;
	unsigned	char nb3=0;
	short nb4=0;

	short rebondok=0;
	short animate_umbrella=0;
	short gameover=0;
	short levelcomplete=0;

  void *vecran;
  Plane bg_plane;


static void reset_variables (void) {

  //place new items:
  static const unsigned short a_val[] = {70,70,70,70,62,47,50,50, 70, 70, 70, 15, 15, 15,31, 7, 7, 8, 8,40,40,40,50,45,45,48, 70,33,41, 18, 19, 13, 14,  7,  8, 34, 35, 54, 55, 54, 55, 54, 55};
  static const unsigned short b_val[] = {22,23,24,48,89,47,64,65,114,115,116,101,102,103,82,54,55,18,19,22,23,24,98,97,98,24,100,63,47, 17, 17, 12, 12, 10, 10, 32, 32, 23, 23, 24, 24, 25, 25};
  static const unsigned short c_val[] = {19,19,19,19,19,19,19,19, 19, 19, 19, 19, 19, 19,19,19,19,19,19,19,19,19,19,19,19,71, 71,71,18,153,154,153,154,153,154,155,156,155,156,155,156,155,156};
  for (n=0;n<43;n++) {
  	bg_map[a_val[n]][b_val[n]]=c_val[n];
  }
  bg_plane.force_update=1;
  x_bg=64,y_bg=1072; //initial background position
  x_pos=88, y_pos=32; //initial player position
  x_bg_target=64,y_bg_target=1072;//initial background position (target)
  left_and_right_pressed = 0;// keep last direction in memory in order to reverse it when both right and left keys are pressed
  tst = 0;
  j=0, seq=0,seq2=0, seq3=0,seq5=0,seq6=0,seq8=0,seq9=0;
  k=0;//for enemies sprites
  antirebond  = 0;
  antirebond2 = 0;
  antirebond3 = 0;//prevent launch right after holding a box
  antirebond4 = 0;//for debug
  mode = 0;
  stop = 0;
 	n=0, m=0;
	pas = 8; 
	reset_level = 0;
	display_slowdown = 0;
  immortal=0;//0:normal 1-100:immortal counter (in images)
  hold_object = 0; //0:hold nothing  1:hold a box  2:hold an umbrella
  x_box=-1;
  x_box_target = -1;
  y_box=-1;
	box_speed =12;
	blink_item = 0;

  static const short init_x_enemy[] = {47*16, 73*16, 65*16 , 46*16 , 68*16, 29*16, 89*16, 23*16, 97*16, 83*16,  8*16,101*16+8, 22*16, 62*16, 52*16,  5*16, 70*16, 68*16, 56*16, 37*16, 23*16, 59*16};
  static const short init_y_enemy[] = {69*16, 69*16, 61*16 , 34*16 , 34*16, 69*16, 69*16, 61*16, 32*16, 21*16, 39*16,   49*16, 81*16, 81*16, 61*16, 30*16, 49*16, 46*16, 21*16, 21*16,  5*16,  3*16};
  static const short init_m_enemy[] = {1    , 1    , 1     , 1     , 1    , 1    , 1    , 1    , 1    , 1    , 1    , 1      , 1    , 1    , 1    , 1    , 1    , 1    , 1    , 1    , 1    , 1    };

  for (n=0;n<nb_enemy;n++) {
		  x_enemy[n] = init_x_enemy[n];
		  y_enemy[n] = init_y_enemy[n];
		  m_enemy[n] = init_m_enemy[n];
		  seq_enemy[n]=0;
		  r_enemy[n]=0;
		  shooting[n]=0;
		  x_bullet[n]=0;
		  y_bullet[n]=0;
		  dying_seq[n]=0;
  }
  
	static const short init_c_special_pos[] = {        1,        2,      3,      4,      5,       6,       6,       6,       6,        7,        7,        7,        8,        8};//switch case value
  for (n=0;n<14;n++)
  		c_special_pos[n]=init_c_special_pos[n];
  		
  curtain = 0; //0: disabled  1-6: positions
  m_curtain = 0; //mode 0:close 1:open
  p_teleport=0;
 
  x_swimm_speed = 0;
  y_swimm_speed = 0;
  swimm_mode = 0 ; //0:static 1:to the right 2: to the surface
  x_bubble = 0;
  y_bubble = 0;
  
  showtiles = 0; //for debug
  
  reversed = 0;
	nb=0;
	nb2=0;
	nb3=0;
	nb4=0;

	rebondok=0;
	animate_umbrella=0;
	gameover=0;
	levelcomplete=0;
}

static unsigned short test_bg_map (short x, short y) {
	return bg_map[(y_bg/16)+((y_pos+y)/16)][(x_bg/16)+((x_pos+x)/16)];
}

static void add_one_disk (void) {
				    disque_u++;
						if (disque_u>9) {
						disque_u=0;
						disque_d++;
						}
						if (disque_d>9) {
						disque_d=0;
						vie++;
						}
}

static void test_disque (unsigned short x, unsigned short y) {
	unsigned char * bg_map_ptr = &bg_map[(y_bg/16)+((y_pos+y)/16)][(x_bg/16)+((x_pos+x)/16)];
				nb3=*bg_map_ptr;
				if (nb3==208) {//disk (dark background)
						*bg_map_ptr=20;
				    add_one_disk();
				    bg_plane.force_update=1;
				}
				else if (nb3==188) {//disk (light background)
						*bg_map_ptr=0;
						add_one_disk();
				    bg_plane.force_update=1;
				}
				else if (nb3==129) {//broken disk
						*bg_map_ptr=0;
				    disque_d--;
				    if (disque_d<0) {
				    		disque_d=0;
				    		disque_u=0;
				    }
				    bg_plane.force_update=1;
				}
}

static unsigned short test_elevator(short i) {
  	if ((y_bg+y_pos+32==y_elevator[i])&&(x_bg+x_pos+16>=x_elevator[i])&&(x_bg+x_pos<=x_elevator[i]+32-12)) {//test elevator
						if ((s_elevator[i]==3)&& (elevator_delay[i]==0))
								y_pos+=v_elevator[i];
						if ((s_elevator[i]==4)&& (elevator_delay[i]==0))
								y_pos-=v_elevator[i];
						return 1;
		}
		else {
			return 0;
		}
}

short une_image(void) {
  BEGIN_KEYTEST
  if _keytest_optimized(RR_ESC)
    	stop=1;//quit game
  
 if (DEBUG) {
				  if(_keytest_optimized(RR_8) && y_bg>16) //bg up
				      y_bg-=16;
				  if(_keytest_optimized(RR_2) && y_bg<HAUT*16-HEIGHT) //bg down
				      y_bg+=16;
				  if(_keytest_optimized(RR_4) && (x_bg>16)) //bg left
				      x_bg-=16;
				  if(_keytest_optimized(RR_6) && x_bg<LARG*16-WIDTH) //bg right
				      x_bg+=16;
				      
					if (_keytest_optimized(RR_5) && (antirebond2 == 0)){
						antirebond2 = 1;
									
								for (n=0;n<nb_enemy;n++) //kill all enemies
										m_enemy[n]=0;
								
								if (showtiles==0)
										showtiles=1;
								else
										showtiles=0;
									}
				 if (!(_keytest_optimized(RR_DIVIDE)) && !(_keytest_optimized(RR_MULTIPLY)) && !(_keytest_optimized(RR_5))) {
				 	antirebond2=0;
				 }
				 if(_keytest_optimized(RR_DIVIDE)&& (antirebond2==0)) {
				  	k--;
				  	antirebond2=1;
				 }
				 if(_keytest_optimized(RR_MULTIPLY)&& (antirebond2==0)) {
				  	k++;
				  	antirebond2=1;
				 }
	}
	
  if (!_keytest_optimized(RR_PLUS) && !_keytest_optimized(RR_MINUS))
 		antirebond4=0;
  if (_keytest_optimized(RR_PLUS) && (antirebond4 == 0)) {
 		slowdown++;
 		display_slowdown=15;
 		if (slowdown>9)
 			slowdown=10;
 		antirebond4=1;
  }
 if ((_keytest_optimized(RR_MINUS) && (antirebond4 == 0))) {
 		slowdown--;
 		display_slowdown=15;
 		if (slowdown<2)
 			slowdown=1;
 		antirebond4=1;
 }

  short n=0;
  short a=0;
  
switch (mode) {

case 0: // stopped
  		if((_keytest_optimized(RR_RIGHT))&& !(_keytest_optimized(RR_DOWN))) {
     		seq=0;//init sequence
	   		mode =1;//walking
	   		reversed = 0;
     	}
  		if((_keytest_optimized(RR_LEFT))&& !(_keytest_optimized(RR_DOWN))) {
     		seq=0;//init sequence
	   		mode =1;//walking
	   		reversed = 1;
     	}
     	if (!(_keytest_optimized(RR_UP))) 
     			antirebond=0;
     	if(_keytest_optimized(RR_UP)&& antirebond==0) {
     		antirebond=1;
     		seq3=0;//init sequence
	   		mode =3;//vertical jump
     	}
     	
			if (reversed==0)
				a=8;
			else
				a=22;
				
			nb=test_bg_map(a,32);
			//if ((nb==251) || (nb==252)|| (nb==253)|| (nb==248)|| (nb==249)|| (nb==250)) {//Moving walkway test
			if ((nb>=248) && (nb<=253)) {//Moving walkway test
					x_pos+=4;
					if (prop[test_bg_map(a,32)]==0) {
						j=11;
						mode=4;
					} 
			}
			//if ((nb==240) || (nb==241) || (nb==242) || (nb==237) || (nb==238) || (nb==239)) {//Moving walkway test
			if ((nb>=237) && (nb<=242)) {//Moving walkway test
					x_pos-=4;
					if (prop[test_bg_map(a,32)]==0) {
						j=11;
						mode=4;
					} 
			}

				nb=test_bg_map(8+(14*reversed),32);//Floor test
				nb2=test_bg_map(22-(14*reversed),16);//Box test

				if (_keytest_optimized(RR_SHIFT) && (hold_object ==1) && antirebond3==0) { //launch box ?
						antirebond3=1;
						hold_object=0;
						y_box+=8;
						if (reversed == 0)
							x_box_target=x_box+box_speed*10;
						else
							x_box_target=x_box-box_speed*10;
				}	
				
				if ((_keytest_optimized(RR_SHIFT))&&(nb2 == 19) && (hold_object==0) && (antirebond3==0)) {//box test
								n=19;//search for tile behind a box
								m=0;//offset
								while (n==19) {
											n=bg_map[(y_bg/16)+(y_pos/16)+1][(x_bg/16)+(((x_pos+22-(14*reversed))/16))+m];
											m++;
								}
							  bg_map[(y_bg/16)+(y_pos/16)+1][(x_bg/16)+((x_pos+22-(14*reversed))/16)]=n;
							  bg_plane.force_update=1;
							  hold_object=1;
							  antirebond3=1;
				}

				if (hold_object==0)//hold nothing
						j=1;
				else if (hold_object==1){//hold box
						j=14;
						x_box=x_bg+x_pos+8;
						y_box=y_bg+y_pos-12;
				}
				else if (hold_object==2)
						j=14;
				if (_keytest_optimized(RR_SHIFT) && (hold_object ==2)) {
						animate_umbrella=32;
						hold_object=0;
				}

				if (!_keytest_optimized(RR_SHIFT))
						antirebond3=0;

				test_disque(16,8);
				test_disque(16,24);

				if (!_keytest_optimized(RR_DOWN)) {
					rebondok=1;
				}
	    	if((_keytest_optimized(RR_DOWN))&&(rebondok==1)&&(prop[nb] == 2)) {
	    	j=7;
	    	seq5=0;//init sequence
	   		mode=5;//pass through the floor
 	    	}
	    	if((_keytest_optimized(RR_DOWN))&&(prop[nb] != 2)&&hold_object==0) {
				seq6=0;
	    	j=4;
	   		mode =6;//crawling
 	    	}
				//ladder test
 	    	if ((_keytest_optimized(RR_DOWN))&&((nb==135)||(nb==234)||(nb==235))) {
					j=8;
					seq2=0;
					mode = 2;
				}
				
				for (n=0;n<5;n++) {
						test_elevator(n);
				}
				for (n=0;n<10;n++) {
						if ((_keytest_optimized(RR_SHIFT)&&(_keytest_optimized(RR_UP)&&(((x_bg+x_pos)>=x_teleport[n]-16)&&((x_bg+x_pos)<=x_teleport[n]+8)&&((y_bg+y_pos)>=y_teleport[n])&&((y_bg+y_pos)<=y_teleport[n]+16))))) {
							mode=7;
							p_teleport=enum_p_teleport[n];
						}
				}
  break;
  
case 1: //walking

	if (_keytest_optimized(RR_RIGHT) && !(_keytest_optimized(RR_LEFT))) {
			left_and_right_pressed=1;
	}
	if (!(_keytest_optimized(RR_RIGHT)) && _keytest_optimized(RR_LEFT)) {
			left_and_right_pressed=2;
	}

			if ((_keytest_optimized(RR_RIGHT)&& !(_keytest_optimized(RR_LEFT)))||(((_keytest_optimized(RR_RIGHT)) && (_keytest_optimized(RR_LEFT))) && (left_and_right_pressed == 2))) {
					reversed = 0;//walking to the right
					
					if (hold_object==0)
							j=enum_seq[seq];
					else if (hold_object==1) {
							j=enum_seq_box[seq];
							x_box=x_bg+x_pos+16 ;
							y_box=y_bg+y_pos-12;
					}
					else if (hold_object==2)
							j=enum_seq_box[seq];
							
					nb  =test_bg_map(32,0);  //Wall test (up)
					nb2 =test_bg_map(32,16); //Wall test (down)
		     	if ((prop[nb]!=1)&& (prop[nb2]!=1)) {
		     		x_pos+=pas;
		     	}
//
				for (n=0;n<5;n++) {
					if(test_elevator(n)==0) {
							nb=test_bg_map(9,32);//Fall test
							if (!((prop[nb]==1) || (prop[nb]==2))){
									seq3=0;
									mode=4;
									//j=11;
							}
					}
				}
					test_disque(22,8);
					test_disque(22,24);

					seq++;
					if (seq==6) seq=0;
			}
			if ((_keytest_optimized(RR_LEFT)&& !(_keytest_optimized(RR_RIGHT)))||((_keytest_optimized(RR_RIGHT) && _keytest_optimized(RR_LEFT)) && (left_and_right_pressed==1))) {
					reversed = 1; //walking to the left
					
					if (hold_object==0)
							j=enum_seq[seq];
					else if (hold_object==1) {
							j=enum_seq_box[seq];
							x_box=x_bg+x_pos;
							y_box=y_bg+y_pos-12;
					}
					else if (hold_object==2)
							j=enum_seq_box[seq];
					
					nb  =test_bg_map(0,0);  // //Wall test (up)
					nb2 =test_bg_map(0,16); // //Wall test (down)
		     	if ((prop[nb]!=1)&& (prop[nb2]!=1)) {
		     	 		x_pos-=pas;
		     	}
//
				for (n=0;n<5;n++) {
					if(test_elevator(n)==0) {
							nb=test_bg_map(23,32);//Fall test
							if (!((prop[nb]==1) || (prop[nb]==2))){
									seq3=0;
									mode=4;
									//j=11;
							}
					}
				}

				
					test_disque(10,8);
					test_disque(10,24);
					
				
					seq++;
					if (seq==6) seq=0;
			}
			
				if (_keytest_optimized(RR_SHIFT) && (hold_object ==1) && antirebond3==0) {//launch box ?
						hold_object=0;
						y_box+=8;
						if (reversed == 0)
							x_box_target=x_box+box_speed*10;
						else
							x_box_target=x_box-box_speed*10;
				}	
				if((_keytest_optimized(RR_DOWN))&&(prop[nb] != 2)&&hold_object==0) {
				seq6=0;
	    	j=4;
	   		mode =6;//crawling
 	    	}	
 	    	
 	    	
  if (!(_keytest_optimized(RR_LEFT))&&(!(_keytest_optimized(RR_RIGHT)))){
//
		for (n=0;n<5;n++) {
				test_elevator(n);
		}
  	mode=0;
  }
  if(_keytest_optimized(RR_UP)) {
		     		seq3=0;//init sequence
			   		mode =3;//vertical jump
	}
	if (_keytest_optimized(RR_SHIFT) && (hold_object ==2)) {
			animate_umbrella=32;
			hold_object=0;
	}
  
  break;

case 2: //climbing

  if(_keytest_optimized(RR_UP)) {
  	
  		nb=test_bg_map(16,0);//end of ladder test (up)
			nb2=test_bg_map(16,28);//end of ladder test
		  if (prop[nb]!=1 && ((nb2==135)||(nb2==234)||(nb2==235))) {
		
		  		y_pos-=pas/2;
					seq2++;
					if (seq2==4)
						seq2=0;
					j=enum_seq2[seq2];
			}
			if (!((nb2==135)||(nb2==234))) {
				antirebond=1;
				j=1;
				mode=0;
			}
	}
  if(_keytest_optimized(RR_DOWN)) {
  	
   		nb=test_bg_map(16,32+(y_bg%16));//Floor test
   		nb2=test_bg_map(16,16);//type of ladder test (under water or not)
		  if (prop[nb]!=1) {

		  	  y_pos+=pas/2;
					seq2++;
					if (seq2==4)
						seq2=0;
					j=enum_seq2[seq2];
			}
			if ((prop[nb]==1)&&(nb2!=234)) {
					j=1;
					mode=0;
			}
			if (prop[nb]==0) {
				seq3=0;
				mode=4;
				j=11;
			}
	}
	nb2=test_bg_map(16,16);//ladder under water or not ?
	  if (!(_keytest_optimized(RR_LEFT))&& (reversed==1))
  		antirebond=0;
	  if (!(_keytest_optimized(RR_RIGHT))&& (reversed==0))
  		antirebond=0;
  		
	  if((_keytest_optimized(RR_LEFT))&& (antirebond==0)) {
	  	if (nb2==135) {
			  	j=1;
			  	reversed=1;
			  	mode=0;
	  	}
	  	else {
			  	nb=test_bg_map(0,16);
			  	if ((nb==226)||(nb==138))
			  			mode=10;
	  	}
	  }
	  if((_keytest_optimized(RR_RIGHT))&& (antirebond==0)) {
	  	if (nb2==135) {
			  	j=1;
			  	reversed=0;
			  	mode=0;
	  	}
	  	else {
	  			nb=test_bg_map(32,16);
			  	if ((nb==226)||(nb==138))
			  			mode=10;
	  	}
	  }
	  if (hold_object==1) {
				x_box=x_bg+x_pos+8;
				y_box=y_bg+y_pos-12;
		}
  break;
  
case 3: // Jump

  	    if ((seq3>0) && (hold_object==1)){
	    	    mode=4;
	    	    seq3=5;
	    	    break;
    	  }
      	if (seq3>4) {
	    	    mode=4;
	    	    seq3=5;
	    	    break;
    	  }

	    	if(_keytest_optimized(RR_RIGHT)) {//Jump to the right
			    	nb=test_bg_map(32,0);//Wall test
		     				if (((prop[nb]!=1) && (nb !=135))) {
		     						x_pos+=pas;
		     						reversed=0;
		     				}
		     				
		     				if (nb==135) {//If ladder
		     						x_pos-=x_pos%16;
		     						x_pos+=(16+8);
		     						if (x_bg%16==8) {x_pos-=8;}//correction hit during scroll
		     						j=8;
		     						reversed=0;
		     						mode=2;
		     						break;
		     				}
		     				
		     		nb=test_bg_map(16,-16);//Ceiling test (low)
		     				if (((prop[nb]==1) && (nb !=135)) && (seq3<1)) {
										mode=8;//Bumped
	    	    				break;
		     				}
		     		nb=test_bg_map(16,0);//Ceiling test (normal case)
		     				if (((prop[nb]==1) && (nb !=135))) {
										mode=4;//Fall mode to avoid Ceiling
										seq3--;
	    	    				break;
		     				}
		     				
		     				if(!_keytest_optimized(RR_LEFT)) {left_and_right_pressed=0;}
       	}

				if(_keytest_optimized(RR_LEFT)) {//Jump to the left
						nb=test_bg_map(0,0);//Wall test
		     				if ((prop[nb]!=1) && (nb !=135)) {
		     						x_pos-=pas;
		     						reversed=1;
		     				}
		       			if (nb==135) {//If ladder
		     						x_pos-=x_pos%16;
		     						x_pos-=8;
		     						if (x_bg%16==8) {x_pos-=8;}//correction hit during scroll
		     						j=8;
		     						reversed=1;
		     						mode=2;
		     						y_pos-=enum_seq3[seq3];
		     						break;
		     				}
		     		nb=test_bg_map(16,-16);//Ceiling test (low)
		     				if (((prop[nb]==1) && (nb !=135)) && (seq3<1)) {
										mode=8;//Bumped
	    	    				break;
		     				}
		     		nb=test_bg_map(16,0);//Ceiling test (normal case)
		     				if (((prop[nb]==1) && (nb !=135))) {
										mode=4;//Fall mode to avoid Ceiling
										seq3--;
	    	    				break;
		     				}
		     				if(!_keytest_optimized(RR_RIGHT)) {left_and_right_pressed=0;}
       	}

				if(_keytest_optimized(RR_LEFT)&&_keytest_optimized(RR_RIGHT) && left_and_right_pressed==0) {
						reversed = 1;
						left_and_right_pressed= 1;
				}    	  
				if (!(_keytest_optimized(RR_LEFT))&&!(_keytest_optimized(RR_RIGHT))) {//Vertical jump
						nb=test_bg_map(16,0);//Ladder test
						if (nb==135) {//If ladder
     						x_pos-=x_pos%16;
     						x_pos+=8;
     						j=8;
     						reversed=1;
     						mode=2;
     						y_pos-=enum_seq3[seq3];
					    	test_disque(16,8);
					    	test_disque(24,24);
					    	test_disque(8,24);
     						break;
     				}
     				nb=test_bg_map(16,-16);//Ceiling test (low)
		     				if (((prop[nb]==1) && (nb !=135)) && (seq3<1)) {
										mode=8;//Bumped
	    	    				break;
		     				}
		     		nb=test_bg_map(16,0);//Ceiling test (normal case)
		     				if (((prop[nb]==1) && (nb !=135))) {
										mode=4;//Fall mode to avoid Ceiling
										seq3--;
	    	    				break;
		     				}
				}
				
	    	y_pos-=enum_seq3[seq3];
	    	test_disque(16,8);
	    	test_disque(24,24);
	    	test_disque(8,24);
	    	
	    	if (hold_object==1) {
			    	x_box=x_bg+x_pos+8 ;
						y_box=y_bg+y_pos-12;
						j=15;
	   		}
	    	else
	    			j=7;
	    	
	    	if (_keytest_optimized(RR_SHIFT) && (hold_object ==1) && antirebond3==0) {//launch box ?
						hold_object=0;
						y_box+=8;
						if (reversed == 0)
							x_box_target=x_box+box_speed*10;
						else
							x_box_target=x_box-box_speed*10;
				}	
				seq3++;
  break;
  
case 4: // Falling

     	if (hold_object==0)
     			j=11;
     	if (hold_object==1)
     			j=15;
     	if (hold_object==2)
     			j=39;
		  if (_keytest_optimized(RR_RIGHT) && !(_keytest_optimized(RR_LEFT))) {
				left_and_right_pressed=1;
			}
			if (!(_keytest_optimized(RR_RIGHT)) && _keytest_optimized(RR_LEFT)) {
				left_and_right_pressed=2;
			}
			if ((_keytest_optimized(RR_RIGHT)&& !(_keytest_optimized(RR_LEFT)))||(((_keytest_optimized(RR_RIGHT)) && (_keytest_optimized(RR_LEFT))) && (left_and_right_pressed == 2))) {
	    	nb=test_bg_map(32,0);//Wall test
     				if ((prop[nb]!=1) && (nb != 135)) {
     						x_pos+=pas;
     						reversed=0;
     				}
     				if (nb==135) {//If ladder
     						x_pos-=x_pos%16;
     						x_pos+=(16+8);
     						if (x_bg%16==8) {x_pos-=8;}//correction hit during scroll
     						j=8;
     						seq2=0;
     						reversed=0;
     						antirebond=1;
     						mode=2;
     						break;
     				}
       	}
			if ((_keytest_optimized(RR_LEFT)&& !(_keytest_optimized(RR_RIGHT)))||((_keytest_optimized(RR_RIGHT) && _keytest_optimized(RR_LEFT)) && (left_and_right_pressed==1))) {
				nb=test_bg_map(0,0);//Wall test
     				if ((prop[nb]!=1)&& (nb !=135)) {
     						x_pos-=pas;
     						reversed=1;
     				}
       			if (nb==135) {//If ladder
     						x_pos-=x_pos%16;
     						x_pos-=8;
     						if (x_bg%16==8) {x_pos-=8;}//correction hit during scroll
     						j=8;
     						seq2=0;
     						reversed=1;
     						antirebond=1;
     						mode=2;
     						break;
     				}
       	}

				nb=test_bg_map(9+(14*reversed),32);//Floor test
				short n = 0;
				for (n=0;n<5;n++) {
				  	if ((y_bg+y_pos+32>=y_elevator[n]-32)&&(y_bg+y_pos+32<=y_elevator[n]+32)&&(x_bg+x_pos+16>=x_elevator[n])&&(x_bg+x_pos<=x_elevator[n]+32-12)) {//Elevator test
								y_pos=y_elevator[n]-y_bg-32;
								test_elevator(n);
								mode=0;
					    	j=1;
						}
				}
				test_disque(16,8);
	    	test_disque(24,24);
	    	test_disque(8,24);
				
				if ((prop[nb]==1) || (prop[nb]==2)){//if floor is solid
	    	   mode=0;
	    	   j=1;
	    	   y_pos-=y_pos%16;
    	  }
    	  
    	  nb=test_bg_map(16,16);// water test
    	  if ((nb==226) || (nb==138)) { //if water
    	    x_swimm_speed = 0;
  				y_swimm_speed = 2;
  				swimm_mode=0;
  				j=40;
    	 		mode = 10;
    	  }
    	  
			    	if ((seq3==-1)&&(mode==4)&&(hold_object!=2))
			    			y_pos+=pas;
			    	if (hold_object==2)
			    			y_pos+=1;
			    	if ((seq3>-1)&&(mode==4)&&(hold_object!=2)) {
							  y_pos+=enum_seq3[seq3];
							  seq3--;
						}
						if (hold_object==1) {//hold box
								x_box=x_bg+x_pos+8 ;
								y_box=y_bg+y_pos-12;
						}
						if (_keytest_optimized(RR_SHIFT) && (hold_object ==2)) {
								animate_umbrella=32;
								hold_object=0;
						}
						if (_keytest_optimized(RR_SHIFT) && (hold_object ==1) && antirebond3==0) {//launch box ?
								hold_object=0;
								y_box+=8;
								if (reversed == 0)
									x_box_target=x_box+box_speed*10;
								else
									x_box_target=x_box-box_speed*10;
						}
  break;
  
case 5: // pass through the floor
  
  y_pos+=pas;
  seq5++;
  if (seq5>1) {
  	seq5=0;
  	nb=test_bg_map(0,32);//Test
  	if ((prop[nb] == 1)||(prop[nb] == 2)) {
  	j=1;
  	mode=0;
  	rebondok=0;
  	}
  	if (prop[nb] == 0) {
  	j=7;
  	seq3=0;
  	mode=4;
  	}
  }
  break;
  
case 6: // Crawl

  if (_keytest_optimized(RR_DOWN)) {
			if (_keytest_optimized(RR_RIGHT) && !(_keytest_optimized(RR_LEFT))) {
					left_and_right_pressed=1;
			}
			if (!(_keytest_optimized(RR_RIGHT)) && _keytest_optimized(RR_LEFT)) {
					left_and_right_pressed=2;
			}
			if ((_keytest_optimized(RR_RIGHT)&& !(_keytest_optimized(RR_LEFT)))||(((_keytest_optimized(RR_RIGHT)) && (_keytest_optimized(RR_LEFT))) && (left_and_right_pressed == 2))) {
					reversed = 0;
					j=enum_seq6[seq6];
					nb=test_bg_map(32,16);//Wall test
		     	if (prop[nb]!=1) {
		     		x_pos+=pas;
		     	}
		    for (n=0;n<5;n++) {
		     	if (test_elevator(n)==0) {
							nb=test_bg_map(9,32);//Fall test
							if (!((prop[nb]==1) || (prop[nb]==2))){
									seq3=0;
									mode=4;
									j=11;
							}
					}
				}
					seq6++;
					if (seq6==4) seq6=0;
			}
			if ((_keytest_optimized(RR_LEFT)&& !(_keytest_optimized(RR_RIGHT)))||((_keytest_optimized(RR_RIGHT) && _keytest_optimized(RR_LEFT)) && (left_and_right_pressed==1))) {
					j=enum_seq6[seq6];
					reversed = 1;
					nb=test_bg_map(0,16);//Wall test
		     	if (prop[nb]!=1) {
		     		x_pos-=pas;
		     	}
		    for (n=0;n<5;n++) {
		     	if (test_elevator(n)==0) {
							nb=test_bg_map(23,32);//Fall test
							if (!((prop[nb]==1) || (prop[nb]==2))){
									seq3=0;
									mode=4;
									j=11;
							}
					}
				}
					seq6++;
					if (seq6==4) seq6=0;
			}
			if (!(_keytest_optimized(RR_LEFT)&& !(_keytest_optimized(RR_RIGHT)))) {
					for (n=0;n<5;n++) {
							test_elevator(n);
					}
			}
		//
		
			}
		else {
			for (n=0;n<5;n++) {
					test_elevator(n);
			}
			j=1;
			mode=0;
		}

  break;
  
case 7://Teleport

	for (n=0;n<nb_enemy;n++) //avoid openning door on ennemy
			x_enemy[n]=x_enemy_right[n];


  if (m_curtain==0) {
		  curtain++;
		  if (curtain>=6) {
		  	m_curtain=1;
		  	x_bg=xbg_teleport[p_teleport];
		  	y_bg=ybg_teleport[p_teleport];
		  	x_bg_target=xbg_teleport[p_teleport];
		  	y_bg_target=ybg_teleport[p_teleport];
		  	x_pos=x_teleport[p_teleport]-x_bg-8;
		  	y_pos=y_teleport[p_teleport]-y_bg;
			}
	}
	else {
		  curtain--;
		  if (curtain<=0) {
		  	m_curtain=0;
		  	mode=0;
			}
	}
  break;
  
case 8: //Bumped

  j=enum_seq8[seq8];
  seq8++;
  if (seq8>38) {
  	seq8=0;
  	mode=1;
  }
  break;
  
case 9: //Dance

  j=enum_seq9[seq9];
  seq9++;
  if (seq9>79) {
  	seq9=0;
  	mode=1;
  }
  
  if (levelcomplete >=150)
  	stop=1;
  
  break;

case 10: //swimm

  /*
  short x_swimm_speed = 0;
  short y_swimm_speed = 0;
  short x_bubble = 0;
  short y_bubble = 0;
  16/18 swimm
  40/12 dont move
  40/41 up/down
  */
  if (swimm_mode == 0) {
		  j=40;
		  y_swimm_speed=2;
		  nb=test_bg_map(16,48);//Pre-floor test (for sprite n°12/40 change)
			if (!((nb==226) || (nb==138)))
		  	j=12;
		  nb=test_bg_map(16,32);//Floor test
			if (!((nb==226) || (nb==138))) {
				y_swimm_speed=0;//no move
			}
			nb=test_bg_map(16,-16);//Surface test
			//nb=bg_map[(y_bg/16)+((y_pos)/16)+1][(x_bg/16)+((x_pos+16)/16)];
			if (((nb==226) || (nb==138))&& _keytest_optimized(RR_UP)){
					swimm_mode =2;
					y_swimm_speed=0;//no move
			}
	}
	if (swimm_mode == 2) {
			if (_keytest_optimized(RR_UP)) {
					j=40;
					y_swimm_speed=-2;
					if (cpt%15<7)
					j=41;
			}
			if (_keytest_optimized(RR_DOWN)) {
					j=40;
					y_swimm_speed=+2;
					if (cpt%15<7)
					j=41;
			}
			if ((!_keytest_optimized(RR_UP)) && (!_keytest_optimized(RR_DOWN))) {
					j=40;
					swimm_mode=0;
			}
			nb=test_bg_map(16,-16);//Surface test
			//nb=bg_map[(y_bg/16)+((y_pos)/16)+1][(x_bg/16)+((x_pos+16)/16)];
			if (!((nb==226) || (nb==138) || _keytest_optimized(RR_DOWN)))
					y_swimm_speed=0;//no move
			nb=test_bg_map(16,48);//Pre-floor test (for sprite n°12/40 change)
			if (!((nb==226) || (nb==138))) {
		  	j=12;
		  	swimm_mode=0;
		  }
	}

  if (_keytest_optimized(RR_RIGHT) && !(_keytest_optimized(RR_LEFT))) {
			left_and_right_pressed=1;
	}
	if (!(_keytest_optimized(RR_RIGHT)) && _keytest_optimized(RR_LEFT)) {
			left_and_right_pressed=2;
	}

	if ((_keytest_optimized(RR_RIGHT)&& !(_keytest_optimized(RR_LEFT)))||(((_keytest_optimized(RR_RIGHT)) && (_keytest_optimized(RR_LEFT))) && (left_and_right_pressed == 2))) {
					reversed = 0;//Swimming to the right
					if (cpt%15<7)
							j=16;
					else
							j=18;
					nb  =test_bg_map(32,0);  //Wall test (up)
					nb2 =test_bg_map(32,16); //Wall test (down)
		     	if ((prop[nb]!=1)&& (prop[nb2]!=1)) {
		     		x_pos+=4;
		     	}
     			if ((nb2==234)||(nb2==235)) {//If ladder
     						x_pos-=x_pos%16;
     						x_pos+=(16+8);
     						if (x_bg%16==8) {x_pos-=8;}//correction hit during scroll
     						j=8;
     						seq2=0;
     						reversed=0;
     						antirebond=1;
     						mode=2;
     						break;
     			}
	}
	if ((_keytest_optimized(RR_LEFT)&& !(_keytest_optimized(RR_RIGHT)))||((_keytest_optimized(RR_RIGHT) && _keytest_optimized(RR_LEFT)) && (left_and_right_pressed==1))) {
					reversed = 1; //Swimming to the left
					if (cpt%15<7)
							j=16;
					else
							j=18;
					nb  =test_bg_map(0,0);  // //Wall test (up)
					nb2 =test_bg_map(0,16); // //Wall test (down)
		     	if ((prop[nb]!=1)&& (prop[nb2]!=1)) {
		     	 		x_pos-=4;
		     	}
		     	if ((nb2==234)||(nb2==235)) {//If ladder
     						x_pos-=x_pos%16;
     						x_pos-=8;
     						if (x_bg%16==8) {x_pos-=8;}//correction hit during scroll
     						j=8;
     						seq2=0;
     						reversed=1;
     						antirebond=1;
     						mode=2;
     						break;
     			}
	}

	if (y_bg_target == y_bg)//move while not scrolling
			y_pos+=y_swimm_speed;

  break;

case 11: //fly with balloon
  	
  j=39;
  y_pos--;
  if (_keytest_optimized(RR_RIGHT) && !(_keytest_optimized(RR_LEFT))) {
			left_and_right_pressed=1;
	}
	if (!(_keytest_optimized(RR_RIGHT)) && _keytest_optimized(RR_LEFT)) {
			left_and_right_pressed=2;
	}

	if ((_keytest_optimized(RR_RIGHT)&& !(_keytest_optimized(RR_LEFT)))||(((_keytest_optimized(RR_RIGHT)) && (_keytest_optimized(RR_LEFT))) && (left_and_right_pressed == 2))) {
					reversed = 0;//Flying to the right
					nb  =test_bg_map(32,0);  //Wall test (up)
					nb2 =test_bg_map(32,16); //Wall test (down)
		     	if ((prop[nb]!=1)&& (prop[nb2]!=1)) {
		     		x_pos+=4;
		     	}
					test_disque(22,8);
					test_disque(22,24);
	}
	if ((_keytest_optimized(RR_LEFT)&& !(_keytest_optimized(RR_RIGHT)))||((_keytest_optimized(RR_RIGHT) && _keytest_optimized(RR_LEFT)) && (left_and_right_pressed==1))) {
					reversed = 1; //Flying to the left
					nb  =test_bg_map(0,0);  // //Wall test (up)
					nb2 =test_bg_map(0,16); // //Wall test (down)
		     	if ((prop[nb]!=1)&& (prop[nb2]!=1)) {
		     	 		x_pos-=4;
		     	}
					test_disque(10,8);
					test_disque(10,24);
	}
  test_disque(16,8);
	test_disque(16,24);
  nb=test_bg_map(16,-16-8);//Ceiling test
	if ((prop[nb]==1) || ((y_bg+ y_pos-32)==0) || (_keytest_optimized(RR_SHIFT))) {
			mode=4;//Falling
	}
	
  break;
  
case 12://gameover

  if (gameover == 0)
  	  stop=1;
	break;
	
case 13://reset_level

  if (reset_level == 0) {
				  curtain++;
				  if (curtain>=6) {
			  		biglife--;
						vie = 3;
						reset_variables();
					}
  }
  
  break;
  
case 14://intro
	if ((_keytest_optimized(RR_SHIFT)) || (_keytest(RR_ESC)))
			intro=0;
	if ((intro == 0) )
	  	mode=0;
	break;
  
  default:
  //unknown mode
  }
  END_KEYTEST
  
    //////////// Animate background tiles
  
  if ((cpt%10)==0) {//Once every 10 images

    	for (n=0;n<35;n++) { // For each animated tile

				if ((x_animated_tile_pos[n]*16>x_bg) && (x_animated_tile_pos[n]*16<x_bg+160+32) && (y_animated_tile_pos[n]*16>y_bg-16) && (y_animated_tile_pos[n]*16<y_bg+100)) { //if tile is visible
						unsigned char value = 0;
						switch (g_animated_tile_pos[n]) {
						  case 1://Cameras
						  s_animated_tile_pos[n]++;
						  if (s_animated_tile_pos[n]==12) {//max seq is 12 for Camera
						  	s_animated_tile_pos[n]=0;
						  }
							value=seq_animated_tile_group1[(s_animated_tile_pos[n])];
						  break;
						  
						  case 2://Right Axes
						  s_animated_tile_pos[n]++;
						  if (s_animated_tile_pos[n]==3) {//max seq is 3 for Axes
						  	s_animated_tile_pos[n]=0;
						  }
							value=seq_animated_tile_group2[(s_animated_tile_pos[n])];
						  break;
	
							case 3://Left Axes
						  s_animated_tile_pos[n]++;
						  if (s_animated_tile_pos[n]==3) {//max seq is 3 for Axes
						  	s_animated_tile_pos[n]=0;
						  }
							value=seq_animated_tile_group3[(s_animated_tile_pos[n])];
						  break;
						  
						  case 4://Flags
						  s_animated_tile_pos[n]++;
						  if (s_animated_tile_pos[n]==2) {//max seq is 2 for Flags
						  	s_animated_tile_pos[n]=0;
						  }
							value=seq_animated_tile_group4[(s_animated_tile_pos[n])];
						  break;
						  
						  case 5://guitar
						  s_animated_tile_pos[n]++;
						  if (s_animated_tile_pos[n]==2) {//max seq is 2 for guitar
						  	s_animated_tile_pos[n]=0;
						  }
						  if (item!=1) {
							value=seq_animated_tile_group5[(s_animated_tile_pos[n])];
						  }
						  break;
						  
						  case 6://Right moving walkways
						  s_animated_tile_pos[n]++;
						  if (s_animated_tile_pos[n]==3) {//max seq is 3 for moving walkways
						  	s_animated_tile_pos[n]=0;
						  }
							value=seq_animated_tile_group6[(s_animated_tile_pos[n])];
						  break;

						  case 7://Right moving walkways
						  s_animated_tile_pos[n]++;
						  if (s_animated_tile_pos[n]==3) {//max seq is 3 for moving walkways
						  	s_animated_tile_pos[n]=0;
						  }
							value=seq_animated_tile_group7[(s_animated_tile_pos[n])];
						  break;
						  
						default:
						}
						bg_map[y_animated_tile_pos[n]][x_animated_tile_pos[n]] = value;
				bg_plane.force_update=1;
				}
			}
  }

   /////Test special x_pos/y_pos
  for (n=0;n<15;n++) {
		  if ((x_bg+x_pos+9>=x_special_pos[n]) && (x_bg+x_pos+9<=x_special_pos[n]+16) && (y_bg+y_pos>=y_special_pos[n]) && (y_bg+y_pos<=y_special_pos[n]+15)) {
				  switch (c_special_pos[n]) {
						  case 1://?=dance
							if (mode !=4) {
							  bg_map[(y_special_pos[n]+16)/16][(x_special_pos[n]+8)/16]=20;
					    	bg_plane.force_update=1;
							  mode=9;
							  c_special_pos[n]=0;
							  }
						  break;
						  case 2://guitar found
						  if (mode!=6) {
								  bg_map[(y_special_pos[n])/16][(x_special_pos[n]+8)/16]=96;
								  bg_plane.force_update=1;
								  item=1;
								  c_special_pos[n]=0;
						  }
						  break;
						  case 3://+1 up
						  bg_map[(y_special_pos[n]+16)/16][(x_special_pos[n]+8)/16]=20;
						  bg_plane.force_update=1;
						  vie++;
						  c_special_pos[n]=0;
						  break;
						  case 4://?=-20points
						  bg_map[(y_special_pos[n])/16][(x_special_pos[n]+8)/16]=20;
						  bg_plane.force_update=1;
						  c_special_pos[n]=0;
						  disque_d-=2;
				    	if (disque_d<0) {
				    		disque_d=0;
				    		disque_u=0;
				    	}
						  break;
						  case 5://?=+50
						  bg_map[(y_special_pos[n])/16][(x_special_pos[n]+8)/16]=20;
						  bg_plane.force_update=1;
						  for (m=0;m<50;m++) {
						  		add_one_disk();
						  }
						  c_special_pos[n]=0;
						  break;
						  case 6://ballons
						  if (mode != 11) {
								  bg_map[(y_special_pos[n])/16][(x_special_pos[n]+8)/16]=20;
								  bg_map[(y_special_pos[n]+16)/16][(x_special_pos[n]+8)/16]=20;
								  bg_plane.force_update=1;
								  mode=11;
								  c_special_pos[n]=0;
						  }
						  break;
						  case 7://umbrella
						  if (hold_object == 0) {
								  bg_map[(y_special_pos[n])/16][(x_special_pos[n]+8)/16]=0;
								  bg_map[(y_special_pos[n]+16)/16][(x_special_pos[n]+8)/16]=0;
								  bg_plane.force_update=1;
								  hold_object = 2;
								  c_special_pos[n]=0;
						  }
						  break;
						  case 8://Flag (LEVEL FINISHED)
							if (item==1) {
								mode=9;//dance
								levelcomplete++;
							}
							else {
								blink_item=15;//show missing guitar
							}
						  break;
						  default:
				  }
		  }
  }
  
	  //////////////////
	  // safety check //
	  //////////////////
	 
	  if ((x_bg+x_pos)<24)
	  	x_pos=24;
	  if ((x_bg+x_pos)>((LARG*16)-32))
	  	x_pos=((LARG*16)-32-x_bg);

  	
  	/////////////////
    /// Scrolling ///
    /////////////////
    
  	//X scrolling: (3 tiles)
		if ((x_bg<((LARG-(WIDTH/16)-3-1)*16))&&(x_pos>16*((WIDTH/16)-4+2))) {
			x_bg_target = ((x_bg/16)+3)*16;	
		}
		if((x_bg>(3*16))&&(x_pos<16*4)) {
			x_bg_target = ((x_bg/16)-3)*16;
		}
		if (x_bg_target > x_bg) {
			x_bg+=pas;
			x_pos-=pas;
		}
		if (x_bg_target < x_bg) {
			x_bg-=pas;
			x_pos+=pas;
		}
		
  	//Y scrolling:
  	if (mode==4) { //case: falling
  			if ((y_bg<(16*(HAUT)))&&(y_pos>(48))) {
					y_bg_target = ((y_bg/16)+2)*16;
				}
		}
		else {
				if ((y_bg<(16*(HAUT)))&&(y_pos>(72))) {
					y_bg_target = ((y_bg/16)+1)*16;
				}
		}
		
		if (mode==11) {//case: fly with balloons
				if((y_bg>16)&&(y_pos<8)) {
					y_bg_target = ((y_bg/16)-4)*16;
				}
		}
		else {
				if((y_bg>16)&&(y_pos<8)) {
					y_bg_target = ((y_bg/16)-2)*16;
				}
		}
		if (y_bg_target > y_bg) {
			y_bg+=pas;
			y_pos-=pas;
		}
		if (y_bg_target < y_bg) {
			y_bg-=pas;
			y_pos+=pas;
		}
		
		///////////////////////////
    /// Objects computing   ///
    ///////////////////////////
    
	// Calculating elevators positions:

	for (n=0;n<5;n++) {
		if ((s_elevator[n]==3)&& (elevator_delay[n]==0))
				y_elevator[n]+=v_elevator[n];
		if ((s_elevator[n]==4)&& (elevator_delay[n]==0))
				y_elevator[n]-=v_elevator[n];
		if (y_elevator[n]>=y_elevator_bottom[n]) {
			elevator_delay[n]++;
			if (elevator_delay[n]>10) {
				s_elevator[n]=4;
				elevator_delay[n]=0;
			}
		}
		if (y_elevator[n]<=y_elevator_top[n]) {
			elevator_delay[n]++;
			if (elevator_delay[n]>10) {
				s_elevator[n]=3;
				elevator_delay[n]=0;
			}
		}
	}

	// Calculating enemies positions:
	
	for (n=0;n<nb_enemy;n++) {
		if (((immortal ==0) && (m_enemy[n]!=0) && (m_enemy[n]!=3) && (x_bg+x_pos >= x_enemy[n]-16) && (x_bg+x_pos <= x_enemy[n]+16) && (y_bg+y_pos >= y_enemy[n]-16) && (y_bg+y_pos <= y_enemy[n]+16) && (g_enemy[n]!=2)) \
		      //bullet test (to the right)
     	|| ((immortal==0) && (m_enemy[n]!=0) && (m_enemy[n]!=3) && (m_enemy[n]==4) && (mode!=6) && (y_bg+y_pos >= y_enemy[n]-16) && (y_bg+y_pos <= y_enemy[n]) && (x_bg+x_pos >= x_enemy[n]-16+x_bullet[n]) && (x_bg+x_pos <= x_enemy[n]+16+x_bullet[n])) \
		      //bullet test (to the left)
    	|| ((immortal==0) && (m_enemy[n]!=0) && (m_enemy[n]!=3) && (m_enemy[n]==5) && (mode!=6) && (y_bg+y_pos >= y_enemy[n]-16) && (y_bg+y_pos <= y_enemy[n]) && (x_bg+x_pos >= x_enemy[n]-16-x_bullet[n]) && (x_bg+x_pos <= x_enemy[n]-x_bullet[n])) \
    	
    	|| ((immortal==0) && (y_bullet[n]!=0) && (y_bg+y_pos >= y_bullet[n]-16) && (y_bg+y_pos <= y_bullet[n]+16) && (x_bg+x_pos >= x_bullet[n]-16) && (x_bg+x_pos <= x_bullet[n]+16)))  {

				if (((immortal ==0) && (m_enemy[n]!=0) && (m_enemy[n]!=3) && (x_bg+x_pos >= x_enemy[n]-16) && (x_bg+x_pos <= x_enemy[n]+16) && (y_bg+y_pos >= y_enemy[n]-16) && (y_bg+y_pos <= y_enemy[n]+16) && (g_enemy[n]!=2) && (hold_object==1))) {//kill enemy
					m_enemy[n]=3;
					dying_seq[n]=0;
				}
				hold_object=0;
				x_box_target =-1;
				x_box=-1;
				y_box=-1;
				immortal=100;
				y_bullet[n]=0;
				if (!((mode==3)||(mode==4)))//avoid bumped while jumping or falling bug
						mode=8;
				vie--;
				if (vie<1) {
					if (biglife<=0) {
							gameover=100;
							mode=12;
					}
					else {
							reset_level=15;
							mode =13;
					}
				}
		}

		 if ((m_enemy[n]!=0) && (m_enemy[n]!=3) && ((g_enemy[n]==3)||(g_enemy[n]==4)) && (x_bg+x_pos>=x_enemy[n]-48) && (x_bg+x_pos<=x_enemy[n]+0) && (shooting[n]==0)) {
		 		m_enemy[n]=5;//shooting to the left
		 		shooting[n]=35;
		 }
		 
		 if ((m_enemy[n]!=0) && (m_enemy[n]!=3) && ((g_enemy[n]==3)||(g_enemy[n]==4)) && (x_bg+x_pos>=x_enemy[n]-0) && (x_bg+x_pos<=x_enemy[n]+48) && (shooting[n]==0)) {
		 		m_enemy[n]=4;//shooting to the right
		 		shooting[n]=35;
		 }		 
		 		 
		 if ((g_enemy[n]==7) && (m_enemy[n]!=0) && (m_enemy[n]!=3) && y_bullet[n]==0 && (x_bg+x_pos>=x_enemy[n]-16) && (x_bg+x_pos<=x_enemy[n]+16) && (y_bg+y_pos>=y_enemy[n]+16)) {
		 	x_bullet[n]=x_enemy[n]+16;
		 	y_bullet[n]=y_enemy[n]+16;
		 }
		 
		 if (y_bullet[n] !=0) {
		 	y_bullet[n]+=3;
		 		if (y_bullet[n]>=180)
		 				y_bullet[n]=0;
		 }
		 
		 shooting[n]--;
		 if ((shooting[n]==1) && (m_enemy[n]!=0) && ((m_enemy[n]!=3)||(g_enemy[n]==4))) {
			 	if (m_enemy[n]==4){
			 		m_enemy[n]=1;
			 		r_enemy[n]=0;
			 		x_bullet[n]=0;
			 	}
			 	else {
			 	m_enemy[n]=2;
			 	r_enemy[n]=1;
			 	x_bullet[n]=0;
			 	}
		 }
		 if (shooting[n]<=0)
		 	shooting[n]=0;
		
		if (shooting[n]==21)
			x_bullet[n]=16;
			
		if (((m_enemy[n]==4)||(m_enemy[n]==5)) && (shooting[n]>=2) && (shooting[n]<=20)) {
				x_bullet[n]+=3;
		}
		
		
		
		if ((m_enemy[n]==1) || (m_enemy[n]==2) || ((m_enemy[n]!=0) && g_enemy[n]==7)) {
				if (m_enemy[n]==1)
						x_enemy[n]+=2;
				if (m_enemy[n]==2)
						x_enemy[n]-=2;

				if (x_enemy[n]>=x_enemy_right[n]) {
						m_enemy[n]=2;
						r_enemy[n]=1;
				}
				if (x_enemy[n]<=x_enemy_left[n]) {
						m_enemy[n]=1;
						r_enemy[n]=0;
				}
				seq_enemy[n]++;
				if ((seq_enemy[n]>=4) && ((g_enemy[n]==1)||(g_enemy[n]==7)))
					seq_enemy[n]=0;
				if ((seq_enemy[n]>=6) && ((g_enemy[n]==3)||(g_enemy[n]==4)||(g_enemy[n]==5)||(g_enemy[n]==6)))
					seq_enemy[n]=0;
		}
	}

	// Calculating box position:
	
	if (x_box_target != -1) {//box launched
				if (x_box_target > x_box)//move to target
					x_box +=box_speed;
				else
					if (x_box_target < x_box)//move to target
					x_box -=box_speed;
				if (x_box_target == x_box) {//if target reached without hitting any enemy
					x_box_target =-1;
					x_box=-1;
					y_box=-1;
				}
				for (n=0;n<nb_enemy;n++) {
						if ((m_enemy[n]!=0) && (m_enemy[n]!=3) && (x_box >= x_enemy[n]-16) && (x_box <= x_enemy[n]+16) && (y_box >= y_enemy[n]-16) && (y_box <= y_enemy[n]+16)) {// if box hit enemy
							m_enemy[n]=3;
							dying_seq[n]=0;
						}
				}
	}

	// Calculating immortality:
	
	immortal--;
	if (immortal<=0)
			immortal=0;
	
		// Calculating blinking item:
	
	blink_item--;
	if (blink_item<=0)
			blink_item=0;
	
	// Calculating reset level timer:
	
	reset_level--;
	if (reset_level<=0)
			reset_level=0;

	// Calculating intro timer:
	
	intro--;
	if (intro<=0)
			intro=0;
	
		// Calculating display_slowdown timer:
	
	display_slowdown--;
	if (display_slowdown<=0)
			display_slowdown=0;
	
	// Calculating Umbrella animation:
	
	animate_umbrella--;
	if (animate_umbrella<=0)
		animate_umbrella=0;
		
	// Calculating Gameover animation:
	
	gameover--;
	if (gameover<=0)
		gameover=0;

	// Calculating Frame counter:
	
	cpt++;


		/////////////////
		///  Display  ///
		/////////////////
	
	
    DrawPlane(x_bg,y_bg,&bg_plane,vecran,TM_GRPLC,TM_G16B); // Draw background
        
  	for (n=0;n<5;n++) { // Draw elevators
				if ((x_elevator[n]>x_bg) && (x_elevator[n] <x_bg+160+32) && (y_elevator[n]>y_bg-16) && (y_elevator[n]<y_bg+100)) {
				    GraySprite16_SMASK_R(x_elevator[n]-x_bg,y_elevator[n]-y_bg,16,p_array16[1]+22*16,p_array16[2]+22*16,p_array16[0]+22*16,vecran,vecran+LCD_SIZE);
					  GraySprite16_SMASK_R(x_elevator[n]-x_bg+16,y_elevator[n]-y_bg,16,p_array16[1]+23*16,p_array16[2]+23*16,p_array16[0]+23*16,vecran,vecran+LCD_SIZE);
				}
		}

  	for (n=0;n<nb_enemy;n++) { // Draw enemies
				if ((m_enemy[n]!=0)&&(m_enemy[n]!=3)&&(x_enemy[n]>x_bg) && (x_enemy[n] <x_bg+160+32) && (y_enemy[n]>y_bg-32) && (y_enemy[n]<y_bg+100)) {//draw moving enemy
					short xcoord_=x_enemy[n]-x_bg;
					short ycoord_=y_enemy[n]-y_bg;
					switch (g_enemy[n]) {

						case 1: //Draw Grandma
						if (r_enemy[n]==0) {
					    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+(enum_seq_enemy[seq_enemy[n]])*32,p_array32[8]+(enum_seq_enemy[seq_enemy[n]])*32,p_array32[6]+(enum_seq_enemy[seq_enemy[n]])*32,vecran,vecran+LCD_SIZE);
						}
						else if (r_enemy[n]==1) {
					    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+(enum_seq_enemy[seq_enemy[n]])*32,p_array32[11]+(enum_seq_enemy[seq_enemy[n]])*32,p_array32[9]+(enum_seq_enemy[seq_enemy[n]])*32,vecran,vecran+LCD_SIZE);
						}
						break;
						case 2: //Draw Fish
						if (r_enemy[n]==0) {
					    GraySprite16_SMASK_R(xcoord_,ycoord_,16,p_array16[1]+24*16,p_array16[2]+24*16,p_array16[0]+24*16,vecran,vecran+LCD_SIZE);
						}
						else if (r_enemy[n]==1) {
					    GraySprite16_SMASK_R(xcoord_,ycoord_,16,p_array16[4]+24*16,p_array16[5]+24*16,p_array16[3]+24*16,vecran,vecran+LCD_SIZE);
						}
						break;
						case 3: //Draw Cop
						if ((m_enemy[n]==1) || (m_enemy[n]==2)){ //Cop is walking
								if (r_enemy[n]==0) {
							    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+(enum_seq_enemy3[seq_enemy[n]])*32,p_array32[8]+(enum_seq_enemy3[seq_enemy[n]])*32,p_array32[6]+(enum_seq_enemy3[seq_enemy[n]])*32,vecran,vecran+LCD_SIZE);
								}
								else if (r_enemy[n]==1) {
							    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+(enum_seq_enemy3[seq_enemy[n]])*32,p_array32[11]+(enum_seq_enemy3[seq_enemy[n]])*32,p_array32[9]+(enum_seq_enemy3[seq_enemy[n]])*32,vecran,vecran+LCD_SIZE);
								}
						}
						else if ((m_enemy[n]==4)){ //cop is shooting to the right
								if      (shooting[n]>=30)
							    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+6*32,p_array32[8]+6*32,p_array32[6]+6*32,vecran,vecran+LCD_SIZE);
							  else if (shooting[n]>=25)
							    		GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+7*32,p_array32[8]+7*32,p_array32[6]+7*32,vecran,vecran+LCD_SIZE);
							  else if (shooting[n]>=15)
									    		GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+8*32,p_array32[8]+8*32,p_array32[6]+8*32,vecran,vecran+LCD_SIZE);
								else if (shooting[n]>=10)
									    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+7*32,p_array32[8]+7*32,p_array32[6]+7*32,vecran,vecran+LCD_SIZE);
								else if (shooting[n]>=2)
									    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+6*32,p_array32[8]+6*32,p_array32[6]+6*32,vecran,vecran+LCD_SIZE);
							//Draw bullet
							if ((shooting[n]<=20) && ((x_enemy[n]+x_bullet[n])>x_bg) && ((x_enemy[n]+x_bullet[n]) <x_bg+160+32))
								GraySprite16_SMASK_R(xcoord_+x_bullet[n],ycoord_+8,16,p_array16[4]+21*16,p_array16[5]+21*16,p_array16[3]+21*16,vecran,vecran+LCD_SIZE);
						}
						else if ((m_enemy[n]==5)){ //cop is shooting to the left
								if      (shooting[n]>=30)
							    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+6*32,p_array32[11]+6*32,p_array32[9]+6*32,vecran,vecran+LCD_SIZE);
							  else if (shooting[n]>=25)
							    		GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+7*32,p_array32[11]+7*32,p_array32[9]+7*32,vecran,vecran+LCD_SIZE);
							  else if (shooting[n]>=15)
									    		GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+8*32,p_array32[11]+8*32,p_array32[9]+8*32,vecran,vecran+LCD_SIZE);
								else if (shooting[n]>=10)
									    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+7*32,p_array32[11]+7*32,p_array32[9]+7*32,vecran,vecran+LCD_SIZE);
								else if (shooting[n]>=2)
									    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+6*32,p_array32[11]+6*32,p_array32[9]+6*32,vecran,vecran+LCD_SIZE);
							//Draw bullet
							if ((shooting[n]<=20) && ((x_enemy[n]-x_bullet[n]+16)>x_bg) && ((x_enemy[n]-x_bullet[n]+16) <x_bg+160+32))
								GraySprite16_SMASK_R(xcoord_-x_bullet[n]+16,ycoord_+8,16,p_array16[4]+22*16,p_array16[5]+22*16,p_array16[3]+22*16,vecran,vecran+LCD_SIZE);
						}
						break;	
						case 4: //Draw Woman
						if ((m_enemy[n]==1) || (m_enemy[n]==2)){ //Woman is walking
								if (r_enemy[n]==0) {
							    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+(enum_seq_enemy4[seq_enemy[n]])*32,p_array32[8]+(enum_seq_enemy4[seq_enemy[n]])*32,p_array32[6]+(enum_seq_enemy4[seq_enemy[n]])*32,vecran,vecran+LCD_SIZE);
								}
								else if (r_enemy[n]==1) {
							    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+(enum_seq_enemy4[seq_enemy[n]])*32,p_array32[11]+(enum_seq_enemy4[seq_enemy[n]])*32,p_array32[9]+(enum_seq_enemy4[seq_enemy[n]])*32,vecran,vecran+LCD_SIZE);
								}
						}
						else if ((m_enemy[n]==4)){ //Woman is shooting to the right
								if      (shooting[n]>=30)
							    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+28*32,p_array32[8]+28*32,p_array32[6]+28*32,vecran,vecran+LCD_SIZE);
							  else if (shooting[n]>=25)
							    		GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+29*32,p_array32[8]+29*32,p_array32[6]+29*32,vecran,vecran+LCD_SIZE);
							  else if (shooting[n]>=15)
									    		GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+30*32,p_array32[8]+30*32,p_array32[6]+30*32,vecran,vecran+LCD_SIZE);
								else if (shooting[n]>=10)
									    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+31*32,p_array32[8]+31*32,p_array32[6]+31*32,vecran,vecran+LCD_SIZE);
								else if (shooting[n]>=2)
									    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+31*32,p_array32[8]+31*32,p_array32[6]+31*32,vecran,vecran+LCD_SIZE);
							//Draw plate
							if ((shooting[n]<=20) && ((x_enemy[n]+x_bullet[n])>x_bg) && ((x_enemy[n]+x_bullet[n]) <x_bg+160+32))
								GraySprite16_SMASK_R(xcoord_+x_bullet[n],ycoord_+8,16,p_array16[1]+21*16,p_array16[2]+21*16,p_array16[0]+21*16,vecran,vecran+LCD_SIZE);
						}
						else if ((m_enemy[n]==5)){ //Woman is shooting to the left
								if      (shooting[n]>=30)
							    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+28*32,p_array32[11]+28*32,p_array32[9]+28*32,vecran,vecran+LCD_SIZE);
							  else if (shooting[n]>=25)
							    		GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+29*32,p_array32[11]+29*32,p_array32[9]+29*32,vecran,vecran+LCD_SIZE);
							  else if (shooting[n]>=15)
									    		GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+30*32,p_array32[11]+30*32,p_array32[9]+30*32,vecran,vecran+LCD_SIZE);
								else if (shooting[n]>=10)
									    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+31*32,p_array32[11]+31*32,p_array32[9]+31*32,vecran,vecran+LCD_SIZE);
								else if (shooting[n]>=2)
									    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+31*32,p_array32[11]+31*32,p_array32[9]+31*32,vecran,vecran+LCD_SIZE);
							//Draw plate
							if ((shooting[n]<=20) && ((x_enemy[n]-x_bullet[n]+16)>x_bg) && ((x_enemy[n]-x_bullet[n]+16) <x_bg+160+32))
								GraySprite16_SMASK_R(xcoord_-x_bullet[n]+16,ycoord_+8,16,p_array16[1]+21*16,p_array16[2]+21*16,p_array16[0]+21*16,vecran,vecran+LCD_SIZE);
						}
						break;
						case 5://Draw Junky
						if ((m_enemy[n]==1) || (m_enemy[n]==2)){ //Junky is walking
								if (r_enemy[n]==0) {
							    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+(enum_seq_enemy5[seq_enemy[n]])*32,p_array32[8]+(enum_seq_enemy5[seq_enemy[n]])*32,p_array32[6]+(enum_seq_enemy5[seq_enemy[n]])*32,vecran,vecran+LCD_SIZE);
								}
								else if (r_enemy[n]==1) {
							    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+(enum_seq_enemy5[seq_enemy[n]])*32,p_array32[11]+(enum_seq_enemy5[seq_enemy[n]])*32,p_array32[9]+(enum_seq_enemy5[seq_enemy[n]])*32,vecran,vecran+LCD_SIZE);
								}
						}
						break;
						case 6://Draw Farmer
						if ((m_enemy[n]==1) || (m_enemy[n]==2)){ //Farmer is walking
								if (r_enemy[n]==0) {
							    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+(enum_seq_enemy6[seq_enemy[n]])*32,p_array32[8]+(enum_seq_enemy6[seq_enemy[n]])*32,p_array32[6]+(enum_seq_enemy6[seq_enemy[n]])*32,vecran,vecran+LCD_SIZE);
								}
								else if (r_enemy[n]==1) {
							    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+(enum_seq_enemy6[seq_enemy[n]])*32,p_array32[11]+(enum_seq_enemy6[seq_enemy[n]])*32,p_array32[9]+(enum_seq_enemy6[seq_enemy[n]])*32,vecran,vecran+LCD_SIZE);
								}
						}
						break;
						case 7://Draw Bird
						if (m_enemy[n]!=0){ //Bird is flying
								if (r_enemy[n]==0) {
							    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[7]+(enum_seq_enemy7[seq_enemy[n]])*32,p_array32[8]+(enum_seq_enemy7[seq_enemy[n]])*32,p_array32[6]+(enum_seq_enemy7[seq_enemy[n]])*32,vecran,vecran+LCD_SIZE);
								}
								else if (r_enemy[n]==1) {
							    GraySprite32_SMASK_R(xcoord_,ycoord_,32,p_array32[10]+(enum_seq_enemy7[seq_enemy[n]])*32,p_array32[11]+(enum_seq_enemy7[seq_enemy[n]])*32,p_array32[9]+(enum_seq_enemy7[seq_enemy[n]])*32,vecran,vecran+LCD_SIZE);
								}
						}
						//Draw Egg
						if ((y_bullet[n]!=0) && ((x_bullet[n])>x_bg) && ((x_bullet[n])<x_bg+160+32) && ((y_bullet[n]+16)>y_bg) && (y_bullet[n] <y_bg+100))
								GraySprite16_SMASK_R(x_bullet[n]-x_bg,y_bullet[n]-y_bg,16,p_array16[1]+25*16,p_array16[2]+25*16,p_array16[0]+25*16,vecran,vecran+LCD_SIZE);
						break;	
						default:
					}
				}
				if (m_enemy[n]==3) {//draw dying enemy
						if  (((x_enemy[n]+x_enum_dying_seq[dying_seq[n]])>x_bg) && ((x_enemy[n]+x_enum_dying_seq[dying_seq[n]])<x_bg+160+32) && ((y_enemy[n]+y_enum_dying_seq[dying_seq[n]])>y_bg-32) && ((y_enemy[n]+y_enum_dying_seq[dying_seq[n]])<y_bg+100)) {
									if ((dying_seq[n] == 0) || (dying_seq[n] == 1) || (dying_seq[n] == 2) || (dying_seq[n] == 6) || (dying_seq[n] == 7) || (dying_seq[n] == 8) || (dying_seq[n] == 9) || (dying_seq[n] == 10) || (dying_seq[n] == 11))
										GraySprite32_SMASK_R(x_enemy[n]+x_enum_dying_seq[dying_seq[n]]-x_bg,y_enemy[n]+y_enum_dying_seq[dying_seq[n]]-y_bg,32,p_array32[7]+(dying_tile[(g_enemy[n])-1])*32,p_array32[8]+(dying_tile[(g_enemy[n])-1])*32,p_array32[6]+(dying_tile[(g_enemy[n])-1])*32,vecran,vecran+LCD_SIZE);
									else
										GraySprite32_SMASK_R(x_enemy[n]+x_enum_dying_seq[dying_seq[n]]-x_bg,y_enemy[n]+y_enum_dying_seq[dying_seq[n]]-y_bg,32,p_array32[10]+(dying_tile[(g_enemy[n])-1])*32,p_array32[11]+(dying_tile[(g_enemy[n])-1])*32,p_array32[9]+(dying_tile[(g_enemy[n])-1])*32,vecran,vecran+LCD_SIZE);
						}
						dying_seq[n]++;
						if (dying_seq[n]>=14) {
								dying_seq[n]=0;
								m_enemy[n]=0;
						}
				}
		}

		if ((x_box!=-1) && (y_box !=-1)) {// Draw box
			if ((x_box>x_bg) && (x_box <x_bg+160+16) && (y_box>y_bg-16) && (y_box<y_bg+100))
				GraySprite16_SMASK_R(x_box-x_bg,y_box-y_bg,16,p_array16[1]+10*16,p_array16[2]+10*16,p_array16[0]+10*16,vecran,vecran+LCD_SIZE);	
		}

short x_offset=0;//sprite offset

if ((immortal%4==0)||(immortal%4==1)||(mode==8)) {// Draw character
		if (reversed){// Character (looking to the left)
				if (j==31) {//add tile32 n°42 on the top of tile32 n°31
				GraySprite32_SMASK_R(x_pos,y_pos-32,32,p_array32[4]+42*32,p_array32[5]+42*32,p_array32[3]+42*32,vecran,vecran+LCD_SIZE);
				}
				else if (j==32) {//add tile32 n°43 on the top of tile32 n°32
				GraySprite32_SMASK_R(x_pos,y_pos-32,32,p_array32[4]+43*32,p_array32[5]+43*32,p_array32[3]+43*32,vecran,vecran+LCD_SIZE);
				}
				else if (j==19) {//add tile16 n°00 on the top (left) of tile32 n°19
				GraySprite16_SMASK_R(x_pos,y_pos-16,16,p_array16[4]+0*16,p_array16[5]+0*16,p_array16[3]+0*16,vecran,vecran+LCD_SIZE);
				            //add tile16 n°01 on the top (right) of tile32 n°19
				GraySprite16_SMASK_R(x_pos+16,y_pos-16,16,p_array16[4]+1*16,p_array16[5]+1*16,p_array16[3]+1*16,vecran,vecran+LCD_SIZE);
				}
				else if (j==30) {//add tile16 n°02 on the top (left) of tile32 n°30
				GraySprite16_SMASK_R(x_pos,y_pos-16,16,p_array16[4]+2*16,p_array16[5]+2*16,p_array16[3]+2*16,vecran,vecran+LCD_SIZE);
				            //add tile16 n°03 on the top (right) of tile32 n°30
				GraySprite16_SMASK_R(x_pos+16,y_pos-16,16,p_array16[4]+3*16,p_array16[5]+3*16,p_array16[3]+3*16,vecran,vecran+LCD_SIZE);
				}
				else if (j==12) {//add tile16 n°04 on the right of tile32 n°12[was 45]
				GraySprite16_SMASK_R(x_pos+32,y_pos+4+8,16,p_array16[4]+4*16,p_array16[5]+4*16,p_array16[3]+4*16,vecran,vecran+LCD_SIZE);
				}
				else if (j==16) {//add tile16 n°05 on the right of tile32 n°16[was 46]
				GraySprite16_SMASK_R(x_pos+32,y_pos+16-1,16,p_array16[4]+5*16,p_array16[5]+5*16,p_array16[3]+5*16,vecran,vecran+LCD_SIZE);
				}
				else if (j==18) {//add tile16 n°06 on the right of tile32 n°18[was 47]
				GraySprite16_SMASK_R(x_pos+32,y_pos+10,16,p_array16[4]+6*16,p_array16[5]+6*16,p_array16[3]+6*16,vecran,vecran+LCD_SIZE);
				            //add tile16 n°07 on the left of tile32 n°18[was 47]
				GraySprite16_SMASK_R(x_pos+32,y_pos+10+16,16,p_array16[4]+7*16,p_array16[5]+7*16,p_array16[3]+7*16,vecran,vecran+LCD_SIZE);
				}
				else if (j==40) {//add tile16 n°08 on the left of tile32 n°40[was 48]
				GraySprite16_SMASK_R(x_pos+32,y_pos+16,16,p_array16[4]+8*16,p_array16[5]+8*16,p_array16[3]+8*16,vecran,vecran+LCD_SIZE);
				}
				else if ((j==39)&&(mode==11)) {//add tile16 n°11 on the left of tile32 n°39
				GraySprite16_SMASK_R(x_pos+8,y_pos-16-8,16,p_array16[4]+11*16,p_array16[5]+11*16,p_array16[3]+11*16,vecran,vecran+LCD_SIZE);
				                          //add tile16 n°12 on the left of tile32 n°39
				GraySprite16_SMASK_R(x_pos+8,y_pos-8,16,p_array16[4]+12*16,p_array16[5]+12*16,p_array16[3]+12*16,vecran,vecran+LCD_SIZE);
				}
				GraySprite32_SMASK_R(x_pos+x_offset,y_pos,32,p_array32[4]+j*32,p_array32[5]+j*32,p_array32[3]+j*32,vecran,vecran+LCD_SIZE);//Draw character
		}
		else {// Character (looking to the right)
				if (j==31) {//add tile 42 on the top
				GraySprite32_SMASK_R(x_pos,y_pos-32,32,p_array32[1]+42*32,p_array32[2]+42*32,p_array32[0]+42*32,vecran,vecran+LCD_SIZE);
				}
				else if (j==32) {//add tile 43 on the top
				GraySprite32_SMASK_R(x_pos,y_pos-32,32,p_array32[1]+43*32,p_array32[2]+43*32,p_array32[0]+43*32,vecran,vecran+LCD_SIZE);
				}
				else if (j==19) {//add tile16 n°00 on the top (left) of tile32 n°19
				GraySprite16_SMASK_R(x_pos,y_pos-16,16,p_array16[1]+0*16,p_array16[2]+0*16,p_array16[0]+0*16,vecran,vecran+LCD_SIZE);
				            //add tile16 n°01 on the top (right) of tile32 n°19
				GraySprite16_SMASK_R(x_pos+16,y_pos-16,16,p_array16[1]+1*16,p_array16[2]+1*16,p_array16[0]+1*16,vecran,vecran+LCD_SIZE);
				}
				else if (j==30) {//add tile16 n°02 on the top (left) of tile32 n°30
				GraySprite16_SMASK_R(x_pos,y_pos-16,16,p_array16[1]+2*16,p_array16[2]+2*16,p_array16[0]+2*16,vecran,vecran+LCD_SIZE);
				            //add tile16 n°03 on the top (right) of tile32 n°30
				GraySprite16_SMASK_R(x_pos+16,y_pos-16,16,p_array16[1]+3*16,p_array16[2]+3*16,p_array16[0]+3*16,vecran,vecran+LCD_SIZE);
				}
				else if (j==12) {//add tile16 n°04 on the left of tile32 n°12[was 45]
				GraySprite16_SMASK_R(x_pos-16,y_pos+4+8,16,p_array16[1]+4*16,p_array16[2]+4*16,p_array16[0]+4*16,vecran,vecran+LCD_SIZE);
				}
				else if (j==16) {//add tile16 n°05 on the left of tile32 n°16[was 46]
				GraySprite16_SMASK_R(x_pos-16,y_pos+16-1,16,p_array16[1]+5*16,p_array16[2]+5*16,p_array16[0]+5*16,vecran,vecran+LCD_SIZE);
				}
				else if (j==18) {//add tile16 n°06 on the left of tile32 n°18[was 47]
				GraySprite16_SMASK_R(x_pos-16,y_pos+10,16,p_array16[1]+6*16,p_array16[2]+6*16,p_array16[0]+6*16,vecran,vecran+LCD_SIZE);
				            //add tile16 n°07 on the left of tile32 n°18[was 47]
				GraySprite16_SMASK_R(x_pos-16,y_pos+10+16,16,p_array16[1]+7*16,p_array16[2]+7*16,p_array16[0]+7*16,vecran,vecran+LCD_SIZE);
				}
				else if (j==40) {//add tile16 n°08 on the left of tile32 n°40[was 48]
				GraySprite16_SMASK_R(x_pos-16,y_pos+16,16,p_array16[1]+8*16,p_array16[2]+8*16,p_array16[0]+8*16,vecran,vecran+LCD_SIZE);
				}
				else if ((j==39)&&(mode==11)) {//add tile16 n°11 on the left of tile32 n°39 (balloon)
				GraySprite16_SMASK_R(x_pos+8,y_pos-16-8,16,p_array16[1]+11*16,p_array16[2]+11*16,p_array16[0]+11*16,vecran,vecran+LCD_SIZE);
				                          //add tile16 n°12 on the left of tile32 n°39 (balloon)
				GraySprite16_SMASK_R(x_pos+8,y_pos-8,16,p_array16[1]+12*16,p_array16[2]+12*16,p_array16[0]+12*16,vecran,vecran+LCD_SIZE);
				}
				else if (j==41)
					x_offset=-6;
				GraySprite32_SMASK_R(x_pos+x_offset,y_pos,32,p_array32[1]+j*32,p_array32[2]+j*32,p_array32[0]+j*32,vecran,vecran+LCD_SIZE);//Draw character
		}
				if (hold_object==2) {//Draw umbrella
				GraySprite32_SMASK_R(x_pos-8,y_pos-25,32,p_array32[1]+44*32,p_array32[2]+44*32,p_array32[0]+44*32,vecran,vecran+LCD_SIZE);//Draw tile 44
				//add tile16 n°9 on the left of tile32 n°44:
				GraySprite16_SMASK_R(x_pos+32-8,y_pos-16-8,16,p_array16[1]+9*16,p_array16[2]+9*16,p_array16[0]+9*16,vecran,vecran+LCD_SIZE);
		}
}

if (animate_umbrella>0) {//Animate umbrella
				GraySprite32_SMASK_R(x_pos-8,y_pos-25-(64-animate_umbrella*2),32,p_array32[1]+44*32,p_array32[2]+44*32,p_array32[0]+44*32,vecran,vecran+LCD_SIZE);//Draw tile 44
				//add tile16 n°9 on the left of tile32 n°44:
				GraySprite16_SMASK_R(x_pos+32-8,y_pos-16-8-(64-animate_umbrella*2),16,p_array16[1]+9*16,p_array16[2]+9*16,p_array16[0]+9*16,vecran,vecran+LCD_SIZE);
}
if (gameover>0) {//Draw gameover
		for (n=0;n<5;n++)
				GraySprite32_SMASK_R(6+32+n*32,32,32,p_array32[1]+(47+n)*32,p_array32[2]+(47+n)*32,p_array32[0]+(47+n)*32,vecran,vecran+LCD_SIZE);
}

if (levelcomplete>0) {//Draw level end
				    GrayDrawStr2B(35,5, "Congratulation!!!",A_REPLACE,vecran,vecran+LCD_SIZE);
				    GrayDrawStr2B(35,15,"Level complete",A_REPLACE,vecran,vecran+LCD_SIZE);
}

	int l,c;
	if (curtain != 0) { //Draw curtain
		 	for (l=0;l<7;l++) { 
		 			for (c=9;c>(9-curtain);c--) { 
					GraySprite16_SMASK_R(c*16+32,l*16,16,p_array16[4]+10*16,p_array16[5]+10*16,p_array16[3]+10*16,vecran,vecran+LCD_SIZE);
					}
					for (c=0;c<curtain;c++) { 
					GraySprite16_SMASK_R(c*16+32,l*16,16,p_array16[4]+10*16,p_array16[5]+10*16,p_array16[3]+10*16,vecran,vecran+LCD_SIZE);
					}
			}
	}

//Draw life bar
for (n=2;n<8;n++) { 
GraySprite16_SMASK_R(16*n,90,16,p_array16[4]+25*16,p_array16[5]+25*16,p_array16[3]+25*16,vecran,vecran+LCD_SIZE);
}

GraySprite16_SMASK_R(16*2+8,90,16,p_array16[4]+23*16,p_array16[5]+23*16,p_array16[3]+23*16,vecran,vecran+LCD_SIZE);//head

for (n=0;n<5;n++) { 
if (vie >n)
	GraySprite16_SMASK_R(((16*(7+n))+8)-(2*n),90,16,p_array16[4]+26*16,p_array16[5]+26*16,p_array16[3]+26*16,vecran,vecran+LCD_SIZE);
else
	GraySprite16_SMASK_R(((16*(7+n))+8)-(2*n),90,16,p_array16[4]+25*16,p_array16[5]+25*16,p_array16[3]+25*16,vecran,vecran+LCD_SIZE);
}
for (n=0;n<slowdown;n++) {//repeat "slowdown" times to slow the game down
		sprintf(info1,"0%hi",biglife);
		sprintf(info2,"%hi%hi",disque_d,disque_u);
		GrayDrawStr2B(16*3+8,92,info1,A_NORMAL ,vecran,vecran+LCD_SIZE);//Draw Continue number
		GrayDrawStr2B(16*7-4,92,info2,A_NORMAL ,vecran,vecran+LCD_SIZE);//Draw disc number
		if (display_slowdown >0) {
			sprintf(info1,"Slow%hi",slowdown);
			GrayDrawStr2B(16*3+8+14,92,info1,A_NORMAL ,vecran,vecran+LCD_SIZE);//Draw Slowdown value
		}
}

if ((item==1) || ((blink_item>0) && (cpt%3==0)))
		GraySprite16_SMASK_R(16*5,90,16,p_array16[4]+27*16,p_array16[5]+27*16,p_array16[3]+27*16,vecran,vecran+LCD_SIZE);//Draw guitar item

		if(DEBUG){
			int i;
			for (i=0;i<slowdown;i++) {
										if (levelcomplete ==0)
										sprintf(info1,"xB=%hi yB=%hi",x_bullet[20],y_bullet[20]);
										if (levelcomplete ==0)
										sprintf(info2,"     ");
										sprintf(info3,"     ");
										sprintf(info4,"     ");

						if (showtiles) {
						GraySprite16_SMASK_R(48,32,16,p_array16[1]+k*16,p_array16[2]+k*16,p_array16[0]+k*16,vecran,vecran+LCD_SIZE); //print masked tile 16 n°k at debug position
						GraySprite16_SMASK_R(48,64,16,p_array16[4]+k*16,p_array16[5]+k*16,p_array16[3]+k*16,vecran,vecran+LCD_SIZE); //print masked tile 16 n°k at debug position
						GraySprite32_SMASK_R(64,32,32,p_array32[1]+k*32,p_array32[2]+k*32,p_array32[0]+k*32,vecran,vecran+LCD_SIZE);
						GraySprite32_SMASK_R(64,64,32,p_array32[4]+k*32,p_array32[5]+k*32,p_array32[3]+k*32,vecran,vecran+LCD_SIZE);
						GraySprite32_SMASK_R(96,32,32,p_array32[7]+k*32,p_array32[8]+k*32,p_array32[6]+k*32,vecran,vecran+LCD_SIZE);
						GraySprite32_SMASK_R(96,64,32,p_array32[10]+k*32,p_array32[11]+k*32,p_array32[9]+k*32,vecran,vecran+LCD_SIZE);
						}
				    GrayDrawStr2B(35,5, info1,A_REPLACE,vecran,vecran+LCD_SIZE);//A_REPLACE
				    GrayDrawStr2B(35,15,info2,A_NORMAL,vecran,vecran+LCD_SIZE);
				    GrayDrawStr2B(35,25,info3,A_NORMAL ,vecran,vecran+LCD_SIZE);
				    GrayDrawStr2B(35,35,info4,A_NORMAL ,vecran,vecran+LCD_SIZE);
			}
		}
		
		if (intro>0) {
				for (n=0;n<5;n++) {
						GraySprite32_SMASK_R(32*(n+1),32*2+5,32,p_array32[1]+20*32,p_array32[2]+20*32,p_array32[0]+20*32,vecran,vecran+LCD_SIZE);
						GraySprite32_SMASK_R(32*(n+1),32*0,32,p_array32[1]+(20+n)*32,p_array32[2]+(20+n)*32,p_array32[0]+(20+n)*32,vecran,vecran+LCD_SIZE);
						GraySprite32_SMASK_R(32*(n+1),32*2,32,p_array32[4]+(22+n)*32,p_array32[5]+(22+n)*32,p_array32[3]+(22+n)*32,vecran,vecran+LCD_SIZE);
				}
				GraySprite32_SMASK_R(32*1,32*1,32,p_array32[1]+20*32,p_array32[2]+20*32,p_array32[0]+20*32,vecran,vecran+LCD_SIZE);
				GraySprite32_SMASK_R(32*2,32*1,32,p_array32[1]+25*32,p_array32[2]+25*32,p_array32[0]+25*32,vecran,vecran+LCD_SIZE);
				GraySprite32_SMASK_R(32*3,32*1,32,p_array32[1]+26*32,p_array32[2]+26*32,p_array32[0]+26*32,vecran,vecran+LCD_SIZE);
				GraySprite32_SMASK_R(32*4,32*1,32,p_array32[4]+20*32,p_array32[5]+20*32,p_array32[3]+20*32,vecran,vecran+LCD_SIZE);
				GraySprite32_SMASK_R(32*5,32*1,32,p_array32[4]+21*32,p_array32[5]+21*32,p_array32[3]+21*32,vecran,vecran+LCD_SIZE);
		}

    FastCopyScreen_R(vecran+64,GrayGetPlane(LIGHT_PLANE));//From virtual to real screen with 32 pixels horizontal shift (light plane)
    FastCopyScreen_R(vecran+LCD_SIZE+64,GrayGetPlane(DARK_PLANE));//From virtual to real screen with 32 pixels horizontal shift (dark plane)

return 0;
}

static unsigned char* GetPointerToFile(const char* fname, HANDLE* h) { //returns pointer to start of external variable which name is given by fname
																    SYM_ENTRY*     symptr;
																    unsigned char* src;
																
																    if ((symptr = DerefSym(SymFind(SYMSTR(fname))))) {
																        *h = symptr->handle;
																        if (!(src = (unsigned char*)HLock(*h))) return NULL;
																            src+=2;
																            return src;
																        }
																    else {
																       return NULL;
																    }
}

static unsigned char* GetDecompressedData(const char* fname) {// decompresses an external variable and returns an allocated buffer
																    HANDLE         h;
																    unsigned char* src = GetPointerToFile(fname,&h);
																    unsigned char* dest;
																
																    if (!src) return NULL; // file not found
																
																    // check if data is really ExePacked
																    if (!ttunpack_valid(src)) {
																        HeapUnlock(h);  // data NOT valid - unlock locked variable again!!
																        return NULL;
																    }
																
																    // allocate buffer for decompressed data
																    if (!(dest = malloc(ttunpack_size(src)))) {
																        HeapUnlock(h);  // out of memory - unlock locked variable again!!
																        return NULL;
																    }
																
																    // decompress the data
																    if (ttunpack_decompress(src,dest) != TTUNPACK_OKAY) {
																        free(dest);
																        dest = NULL;
																    }
																
																    HeapUnlock(h);
																    return dest;
}


void _main(void)
{
  INT_HANDLER ai1,ai5;
  void *bloc=malloc(BIG_VSCREEN_SIZE*2+LCD_SIZE*2); // 2 big_vscreens and 1 buffer by plane
  int i;
 	cpt=0;
  LCD_BUFFER backbuffer;
  LCD_save(backbuffer);
  														HANDLE         h;
  														static const char nomfich[8]="level01";
															unsigned char* src = GetPointerToFile(nomfich,&h);
															
															if (!src) {
															    ST_helpMsg("level01 variable not found [exit]");
															            ngetchx();
															    return;
															}
															else {
															   if (ttarchive_valid(src)) {//decomp interlaced 16x
															       unsigned short i;
															       //unsigned short nr_entries = ttarchive_entries(src); // get number of entries

															           unsigned char* pointer_to_entry = ttarchive_data(src,0);
															           if (ttunpack_valid(pointer_to_entry)) {
																	           	if ((sprites = malloc(ttunpack_size(pointer_to_entry)))) {
																			                   if (ttunpack_decompress(pointer_to_entry,sprites) == TTUNPACK_OKAY) {
																			                        // entry successfully decompressed.
																			                   }
																			        }
																			        else {
																			                   // out of memory!!
																			              ST_helpMsg("out of memory");
																	            			ngetchx();
																			        }
																			    }
																			    
															           	for (i=0;i<12;i++) {//decomp masked 32x
															           		
																	           	unsigned char* pointer_to_entry = ttarchive_data(src,i+1);
																			           	 if (ttunpack_valid(pointer_to_entry)) {
																						           		if ((p_array32[i] = malloc(ttunpack_size(pointer_to_entry)))) {
																								                   if (ttunpack_decompress(pointer_to_entry,(char*)p_array32[i]) == TTUNPACK_OKAY) {
																								                        // entry successfully decompressed.
																								                   }
																								          }
																								          else {
																								                   // out of memory!!
																								                   ST_helpMsg("out of memory");
																						            			ngetchx();
																								          }
																						       }
															           	}
															           	for (i=0;i<6;i++) {//decomp masked 16x
															           		
																	           	unsigned char* pointer_to_entry = ttarchive_data(src,i+13);
																			           	 if (ttunpack_valid(pointer_to_entry)) {
																						           		if ((p_array16[i] = malloc(ttunpack_size(pointer_to_entry)))) {
																								                   if (ttunpack_decompress(pointer_to_entry,(char*)p_array16[i]) == TTUNPACK_OKAY) {
																								                        // entry successfully decompressed.
																								                   }
																								          }
																								          else {
																								                   // out of memory!!
																								                   ST_helpMsg("out of memory");
																						            			ngetchx();
																								          }
																						       }
															           	}

															   }
															   HeapUnlock(h); // unlock variable again
															}

  if(!bloc)
    return;

// Inits
  vecran=bloc;

  bg_plane.matrix=bg_map;
  bg_plane.width=LARG;
  bg_plane.sprites=sprites;
  bg_plane.big_vscreen=bloc+LCD_SIZE*2;
  bg_plane.force_update=1;

  ai1=GetIntVec(AUTO_INT_1);
  ai5=GetIntVec(AUTO_INT_5);

  SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
  SetIntVec(AUTO_INT_5,DUMMY_HANDLER);
  
	GrayOn();
  mode = 0;
  stop=0;
  bg_map[81][94]=159;//place new guitar
  disque_u = 0; // disk counter (unit)
  disque_d = 0; // disk counter (decade)
  vie = 3;//3
  biglife = 2;//2
  item = 0; //0:found nothing  1:found guitar
  slowdown = 5;
  cpt = 0;
	reset_variables();
	intro=50;
	mode=14;//intro
	
		while ((stop==0) && (une_image()==0)) {
		}

	GrayOff();

  SetIntVec(AUTO_INT_1,ai1);
  SetIntVec(AUTO_INT_5,ai5);

  free(bloc);
  GKeyFlush();
  //ST_helpMsg(EXTGRAPH_VERSION_PWDSTR);
  //free memory if allocated:
  if (sprites)
 			free(sprites);
	
	for (i=11;i>=0;i--) {
		if (p_array32[i])
				free(p_array32[i]);
	}
	
	for (i=5;i>=0;i--) {
		if (p_array16[i])
				free(p_array16[i]);
	}
	LCD_restore(backbuffer);//exe file on calc should not be > 42000 to properly restore LCD
}
