#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <time.h>
#include <string.h>
#include <SDL_image.h>
#include <SDL_events.h>
#include <SDL_video.h>
#include <SDL_Rect.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>


#define WINDOW_WIDTH 1020
#define WINDOW_HEIGHT 1020

SDL_Window *window;
SDL_Renderer* renderer = NULL;

typedef struct {
	SDL_Rect rect;
	SDL_Texture* textures[4];
	int state; //state = 0 for open , 1 for closed
	int stateV; //stateV = 0 for open, 1 for closed
	clock_t right;
	clock_t left;
	clock_t up;
	clock_t down;
	clock_t duration;
	int dx;
	int dy;	
	int speed;
	SDL_Surface* pacop ;
	SDL_Surface* pacclo;
	SDL_Surface* pacopv ;
	SDL_Surface* pacclov;
}PACMAN;

typedef struct{
	SDL_Rect rect;
	SDL_Texture* texture[2];
	clock_t lastTime;
	clock_t scared;
	clock_t duration;
	int dx;
	int dy;
	int speed;
	int direction;
	int out;
	SDL_Surface* surface;
	int id;
	int state; //1 for normal, 0 for white
}GHOST;


typedef struct{
	SDL_Rect rect;
	SDL_Texture* texture;
	int touched;
	int dx;
	int dy;
	int ok;
}COIN;


enum DIR {
    DOWN = 1,
    UP = 0,
    LEFT = 2,
    RIGHT = 3,
};

enum STATE{
	PLAY = 1,
	QUIT = 0,
};

void SDL_ExitWithError(char *message);
bool out_of_bounds();
void drawmove();
void drawstat();
void check_events(SDL_Rect rectangle, int pos);
void draw_rectangles();
bool exceeds_up(SDL_Rect spriteRect, SDL_Rect rectangle);
bool exceeds_down(SDL_Rect spriteRect, SDL_Rect rectangle);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
bool exceeds_left(SDL_Rect spriteRect, SDL_Rect rectangle);
bool exceeds_right(SDL_Rect spriteRect, SDL_Rect rectangle);
bool find_neighbour(SDL_Rect spriteRect, int pos);
void flip_left();
void flip_right();
void flip_up();
void flip_down();
void game_init();
void pac_init();
void init_rects();
void ghost_init(GHOST* ghost);
void move_ghost(GHOST* ghost);
void move_up(GHOST* ghost, SDL_Rect rect, int pos);
void move_down(GHOST* ghost, SDL_Rect rect, int pos);
void move_left(GHOST* ghost, SDL_Rect rect, int pos);
void move_right(GHOST* ghost, SDL_Rect rect, int pos);
void load_coin_textures();
void display_coins();
void check_contact();
void display_score();
void free_sounds();
void play();
void quit();
int menu();
void sounds_init();
int difficulty();


	
SDL_Rect mapRect;
SDL_Rect rect_move;
SDL_Rect rect[50];

//Creation of our texture
SDL_Surface* monster = NULL;
SDL_Surface* pacmap = NULL;
SDL_Surface* overSurface;
SDL_Surface* textSurface;
SDL_Surface* lifeSurface;
SDL_Surface* creasurf;
SDL_Surface* pacdent; 
SDL_Surface* thump;

SDL_Texture* pactex;
SDL_Texture* monstexture = NULL;
SDL_Texture* map_texture = NULL;
SDL_Texture* textTexture = NULL;
SDL_Texture* lifeTexture = NULL;
SDL_Texture* creator;
SDL_Texture* thump_tex;


SDL_Rect textRect;
SDL_Rect livesRect;
SDL_Rect paclives;
SDL_Rect monstrect;
SDL_Rect dentrect;
SDL_Rect thumpRect;



TTF_Font* font;
TTF_Font* font1;
SDL_Color textColor = {255,255,255,0};
SDL_Color textColor1 = {10, 150, 255, 0};
SDL_Color red = {250,0,0,0};
SDL_Color black = {0,0,0,0};
SDL_Rect overRect;

int rectIndex=0;
bool game_state;
char pts[20];
char life[20];

PACMAN pac_sprite;

GHOST ghost[4];


int dx, dy;

int score;
int lives;
int count =0; //Number of coins still on map
int pause=0; //Pause state of the game


COIN coin_move;
COIN coin[146];
COIN super_coin[4];
COIN mid_coin[3];

SDL_Keycode actu_key = SDLK_RIGHT;

Mix_Music* bgmusic[5];
Mix_Music* menuMusic;
Mix_Chunk* eat_ghost;
Mix_Chunk* gameover;
Mix_Chunk* gameover1;
Mix_Chunk* eat_supercoin;
Mix_Chunk* eat_supercoin1;
Mix_Chunk* killed;
Mix_Chunk* killed1;
Mix_Chunk* coin500;
Mix_Chunk* eating;
Mix_Chunk* laughter;
Mix_Chunk* winner;
Mix_Chunk* winner1;
Mix_Chunk* coin_500_1;
Mix_Chunk* eaten;
Mix_Chunk* select;
Mix_Chunk* chosen;
Mix_Chunk* okay;
Mix_Chunk* oh_no;

int main_state = PLAY;
int music;
int level;
int midcoin;
int supcoin;
	
int main(int argc, char* argv[]){
	
	srand(time(NULL));
	
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT,2,2048);
	
	ghost[0].id = 1; //Bluey
	ghost[1].id = 2; //Orangy
	ghost[2].id = 3; //Pinky
	ghost[3].id = 4; //Redy
	
	game_init(); //Creation of window and renderer
	
	pac_init(); //Initialise pacman sprite
	
	load_coin_textures(); //Initialise textures of the coins in game
	
	/*Initialisation of mixer, soundtracts and sound effects*/
	sounds_init();
	
	//Initialise ghosts sprites
	for (int i=0; i<4; i++){
		ghost_init(&ghost[i]); 
	}
	
	monster = IMG_Load("images\\pacmonster.jpg");
	monstexture = SDL_CreateTextureFromSurface(renderer, monster);
	
	if(SDL_QueryTexture(monstexture, NULL, NULL, &monstrect.w, &monstrect.h)!=0)
		SDL_ExitWithError("Couldn't load monster");
	
	monstrect.w = 800;
	monstrect.h = 750;
	monstrect.x = (WINDOW_WIDTH - monstrect.w)/2 +450;
	monstrect.y = (WINDOW_HEIGHT - monstrect.h)/2;
	SDL_FreeSurface(monster);
	
	while(main_state){
		
		printf("\nok\n");
		if(menu()){
			level = difficulty();
			play();
		}
		
		else{
			free_sounds();
			Mix_CloseAudio();
			main_state=QUIT;
			quit();
		}
		printf("\ndacco dac\n");
		SDL_RenderClear(renderer);
	}
	
	return EXIT_SUCCESS;
}

void SDL_ExitWithError(char *message){
	SDL_Log("Error %s . %s", SDL_GetError(), message);
	SDL_Quit();
	exit(EXIT_FAILURE);
}


void random_shuffle(int *nums, int length) {
    for (int i = 0; i < length; i++) {
        int shuffle_index = i + (rand() % (length - i));

        int temp = nums[i];
        nums[i] = nums[shuffle_index];
        nums[shuffle_index] = temp;
    }

}

bool out_of_bounds(SDL_Rect spriteRect, SDL_Rect rectangle){
	
	return exceeds_left(spriteRect, rectangle) || exceeds_right(spriteRect, rectangle) || exceeds_up(spriteRect, rectangle) || exceeds_down(spriteRect, rectangle);
}


bool exceeds_left(SDL_Rect spriteRect, SDL_Rect rectangle){
	
	return spriteRect.x < rectangle.x;
}

bool exceeds_right(SDL_Rect spriteRect, SDL_Rect rectangle){
	
	return spriteRect.x + spriteRect.w  > rectangle.x + rectangle.w;
}

bool exceeds_up(SDL_Rect spriteRect, SDL_Rect rectangle){
		
	return spriteRect.y < rectangle.y;
}

bool exceeds_down(SDL_Rect spriteRect, SDL_Rect rectangle){

	return spriteRect.y + spriteRect.h > rectangle.y + rectangle.h;
}


void check_events(SDL_Rect rectangle, int pos){
	SDL_Event event;
	
	if(!pause){
		float dt = clock() - pac_sprite.duration;
		pac_sprite.duration = clock();
		pac_sprite.dx = dt*pac_sprite.speed; pac_sprite.dy = dt*pac_sprite.speed;
	}
	
	
	while(SDL_PollEvent(&event)){
		
		if(event.type != SDL_MOUSEMOTION  && !(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)){
			actu_key = event.key.keysym.sym;
		}
		
		if (event.type == SDL_QUIT){
			game_state = SDL_FALSE;
		}
				
		if (event.type == SDL_KEYDOWN)	{
			if(event.key.keysym.sym == SDLK_m){
				game_state = SDL_FALSE;
			}	
			if(event.key.keysym.sym == SDLK_LEFT){
				flip_left();
				pac_sprite.rect.x -= pac_sprite.dx;
							
				if (!out_of_bounds(pac_sprite.rect, rect[16]) && pac_sprite.rect.x  < rect[16].x +15 && pac_sprite.rect.y < rect[16].y+rect[16].y){ //For Pacman to pass through middle path
					pac_sprite.rect.x = mapRect.x + mapRect.w - pac_sprite.rect.w -20; pac_sprite.rect.y = 454;
				}
							
				if (exceeds_left(pac_sprite.rect, rectangle) &&  !(find_neighbour(pac_sprite.rect, pos))){
					pac_sprite.rect.x += pac_sprite.dx;
				}
			}
				
		
			if(event.key.keysym.sym == SDLK_RIGHT){
				flip_right();
				pac_sprite.rect.x += pac_sprite.dx;
				if (!out_of_bounds(pac_sprite.rect, rect[17]) && pac_sprite.rect.x + pac_sprite.rect.w >= rect[17].x + rect[17].w -15 && pac_sprite.rect.y < rect[17].y+rect[17].h){ //For Pacman to pass through middle path
					pac_sprite.rect.x = 20 + pac_sprite.rect.w; pac_sprite.rect.y = 454;
				}
							
				if (exceeds_right(pac_sprite.rect,rectangle) &&  !(find_neighbour(pac_sprite.rect,pos))){
					pac_sprite.rect.x -= pac_sprite.dx;
				}
			}	
				
			if (event.key.keysym.sym == SDLK_UP){
				flip_up();
				pac_sprite.rect.y -= pac_sprite.dy;
				if (exceeds_up(pac_sprite.rect, rectangle) &&  !(find_neighbour(pac_sprite.rect,pos))){
					pac_sprite.rect.y += pac_sprite.dy;
				}
			}
				
			if(event.key.keysym.sym == SDLK_DOWN){
				flip_down();
				pac_sprite.rect.y += pac_sprite.dy;
				if (exceeds_down(pac_sprite.rect,rectangle) &&  !(find_neighbour(pac_sprite.rect,pos))){
					pac_sprite.rect.y -= pac_sprite.dy;
				}				
			}
			
			if (event.key.keysym.sym == SDLK_n){
//				Mix_FreeMusic(bgmusic[music]);
				music++;
				if(music>4){
					printf("greater");
					music=0;
				}
				Mix_PlayMusic(bgmusic[music], -1);
			}
			
			if (event.key.keysym.sym == SDLK_p){
				if(!pause){
					pause=1;
					for(int i=0;i<4;i++){
					ghost[i].dx=0; ghost[i].dy=0;
				}
					pac_sprite.dx=0; pac_sprite.dy=0;
				}
				else{
					pause=0;					
				}
				
				
			}
		}
	}
}

bool find_neighbour(SDL_Rect spriteRect, int pos){ //pos is the index of the rectangle we're tryin to avoid
	for (int i = 0; i<=47; i++){
		if (i == pos){
			continue;
		}
		else if (!(out_of_bounds(spriteRect , rect[i]))){
			return true;
		}
	}
	return false;
}

void init_rects(){
	//Set the positions of the different rectangles
	rect[0].x = 69;	rect[0].y = 20;	rect[0].w = 65;	rect[0].h = 292;	

	rect[1].x = 233;	rect[1].y = 20;	rect[1].w = 61;	rect[1].h = 884;	
	
	rect[2].x = 725;	rect[2].y = 20;	rect[2].w = 61;	rect[2].h = 884;	
	
	rect[3].x = 69;	rect[3].y = 20;	rect[3].w = 425;	rect[3].h = 64;	
	
	rect[4].x = 529;	rect[4].y = 20;	rect[4].w = 421;	rect[4].h = 60;	
	
	rect[5].x = 69;	rect[5].y = 156;	rect[5].w = 881;	rect[5].h = 60;	
	
	rect[6].x = 69;	rect[6].y = 940;	rect[6].w = 885;	rect[6].h = 60;	
	
	rect[7].x = 69;	rect[7].y = 840;	rect[7].w = 225;	rect[7].h = 64;	
	
	rect[8].x = 69;	rect[8].y = 644;	rect[8].w = 425;	rect[8].h = 64;	
	
	rect[9].x = 529;	rect[9].y = 644;	rect[9].w = 421;	rect[9].h = 64;	
	
	rect[10].x = 233;	rect[10].y = 740;	rect[10].w = 553;	rect[10].h = 64;	
	
	rect[11].x = 329;	rect[11].y = 840;	rect[11].w = 165;	rect[11].h = 64;	
	
	rect[12].x = 529;	rect[12].y = 840;	rect[12].w = 161;	rect[12].h = 64;	
	
	rect[13].x = 725;	rect[13].y = 840;	rect[13].w = 229;	rect[13].h = 64;	
	
	rect[14].x = 825;	rect[14].y = 744;	rect[14].w = 125;	rect[14].h = 60;	
	
	rect[15].x = 333;	rect[15].y = 548;	rect[15].w = 357;	rect[15].h = 60;	
	
	rect[16].x = 49;	rect[16].y = 448;	rect[16].w = 345;	rect[16].h = 60;	
	
	rect[17].x = 625;	rect[17].y = 448;	rect[17].w = 345;	rect[17].h = 60;	
	
	rect[18].x = 333;	rect[18].y = 352;	rect[18].w = 357;	rect[18].h = 60;	
	
	rect[19].x = 333;	rect[19].y = 252;	rect[19].w = 161;	rect[19].h = 60;	
	
	rect[20].x = 529;	rect[20].y = 252;	rect[20].w = 161;	rect[20].h = 64;	
	
	rect[21].x = 725;	rect[21].y = 248;	rect[21].w = 225;	rect[21].h = 68;	
	
	rect[22].x = 69;	rect[22].y = 248;	rect[22].w = 225;	rect[22].h = 64;	
	
	rect[23].x = 429;	rect[23].y = 20;	rect[23].w = 65;	rect[23].h = 196;	
	
	rect[24].x = 529;	rect[24].y = 20;	rect[24].w = 61;	rect[24].h = 196;	
		
	rect[26].x = 889;	rect[26].y = 20;	rect[26].w = 61;	rect[26].h = 296;	
	
	rect[27].x = 625;	rect[27].y = 156;	rect[27].w = 65;	rect[27].h = 160;	
	
	rect[28].x = 333;	rect[28].y = 156;	rect[28].w = 61;	rect[28].h = 156;	
	
	rect[29].x = 429;	rect[29].y = 252;	rect[29].w = 65;	rect[29].h = 160;	
	
	rect[30].x = 529;	rect[30].y = 252;	rect[30].w = 61;	rect[30].h = 160;	
	
	rect[31].x = 477;	rect[31].y = 352;	rect[31].w = 70;	rect[31].h = 175;	
	
	rect[32].x = 333;	rect[32].y = 352;	rect[32].w = 61;	rect[32].h = 356;	
	
	rect[33].x = 625;	rect[33].y = 352;	rect[33].w = 65;	rect[33].h = 356;	
	
	rect[34].x = 529;	rect[34].y = 644;	rect[34].w = 61;	rect[34].h = 160;	
	
	rect[35].x = 889;	rect[35].y = 644;	rect[35].w = 61;	rect[35].h = 160;	
	
	rect[36].x = 429;	rect[36].y = 644;	rect[36].w = 65;	rect[36].h = 160;	
	
	rect[37].x = 69;	rect[37].y = 644;	rect[37].w = 64;	rect[37].h = 160;	
	
	rect[38].x = 133;	rect[38].y = 744;	rect[38].w = 61;	rect[38].h = 160;	
	
	rect[39].x = 69;	rect[39].y = 840;	rect[39].w = 65;	rect[39].h = 160;	
	
	rect[40].x = 429;	rect[40].y = 840;	rect[40].w = 65;	rect[40].h = 160;	
	
	rect[41].x = 529;	rect[41].y = 840;	rect[41].w = 61;	rect[41].h = 160;	
	
	rect[42].x = 889;	rect[42].y = 840;	rect[42].w = 65;	rect[42].h = 160;	
	
	rect[43].x = 825;	rect[43].y = 744;	rect[43].w = 64;	rect[43].h = 160;	
	
	rect[44].x = 329;	rect[44].y = 740;	rect[44].w = 61;	rect[44].h = 164;	
	
	rect[45].x = 625;	rect[45].y = 740;	rect[45].w = 65;	rect[45].h = 164;	
	
	rect[46].x = 413;	rect[46].y = 428;	rect[46].w = 197;	rect[46].h = 100;	
	
	rect[47].x = 69;	rect[47].y = 744;	rect[47].w = 125;	rect[47].h = 60;	
}
	
void draw_rectangles(){
	//Draw the rectangles
	
	for (int i = 0; i < 50; i++){
		
		if(SDL_HasIntersection(&rect[i], &pac_sprite.rect)) {
			if( i==31){
				continue;
			}
			
			SDL_SetRenderDrawColor(renderer, 10, 10,255,SDL_ALPHA_OPAQUE);
			SDL_RenderDrawRect(renderer, &rect[i]);
		}
		else{
			SDL_SetRenderDrawColor(renderer, 10, 10,100,SDL_ALPHA_OPAQUE);
			SDL_RenderDrawRect(renderer, &rect[i]);
		}
	}
	
	SDL_SetRenderDrawColor(renderer, 0, 0,0,SDL_ALPHA_OPAQUE);
		
}

void flip_left(){
			
	SDL_Point center = {
		.x = pac_sprite.rect.x,
		.y = pac_sprite.rect.y
	};
	
	SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
	if(abs(clock() - pac_sprite.left) >100 ){
		if(pac_sprite.state){
			pac_sprite.state = 0;
		}
		else {
			pac_sprite.state = 1;
		}
		pac_sprite.left = clock();
	}
	
	SDL_RenderCopyEx(renderer, pac_sprite.textures[pac_sprite.state],NULL, &pac_sprite.rect,0,&center,flip);
}

void flip_right(){	
		
	if(abs(clock() - pac_sprite.right) >100 ){
		if(pac_sprite.state){
			pac_sprite.state = 0;
		}
		else {
			pac_sprite.state = 1;
		}
		pac_sprite.right = clock();
	}
	
	SDL_RenderCopy(renderer, pac_sprite.textures[pac_sprite.state], NULL, &pac_sprite.rect);	
}
//
void flip_up(){
		
	if(abs(clock() - pac_sprite.up) >100 ){
		if(pac_sprite.stateV  == 2){
			pac_sprite.stateV = 3;
		}
		else {
			pac_sprite.stateV = 2;
		}
		pac_sprite.up = clock();
	}
	
	SDL_RenderCopy(renderer, pac_sprite.textures[pac_sprite.stateV], NULL, &pac_sprite.rect);
}

void flip_down(){
			
	SDL_Point center = {
		.x = pac_sprite.rect.x,
		.y = pac_sprite.rect.y
	};
	
	if(abs(clock() - pac_sprite.down) > 100 ){
		if(pac_sprite.stateV  == 2){
			pac_sprite.stateV = 3;
		}
		else {
			pac_sprite.stateV = 2;
		}
		pac_sprite.down = clock();
	}
	
	SDL_RendererFlip flip = SDL_FLIP_VERTICAL;
	
	SDL_RenderCopyEx(renderer, pac_sprite.textures[pac_sprite.stateV], NULL, &pac_sprite.rect,0,&center,flip);
}

void game_init(){
	printf("Initializing SDL...\n");
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1){  // or if(SDL_Init(SDL_EVERYTHING) != 0)
		SDL_ExitWithError("SDL Initialisation Failed.\n");
	}
	
		
	//Window Creation
	window = SDL_CreateWindow("PAC-BOY", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	
	if (window == NULL){
		SDL_ExitWithError("Window Creation Failed.\n");
	}
	
	//Creation of renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	
	if (renderer == NULL){
		SDL_ExitWithError("Render Creation Failed.\n");
	}
	
	if (TTF_Init() < 0){
		SDL_ExitWithError("Couldn't initialize SDL_TTF\n");
	}
	
	sprintf(pts, "By M-S-G-A");
	font = TTF_OpenFont("fonts\\vga850.fon", 1000);
	creasurf = TTF_RenderText_Solid(font,pts,black);
	creator = SDL_CreateTextureFromSurface(renderer,creasurf);
	SDL_QueryTexture(creator,NULL,NULL,&rect_move.w,&rect_move.h);
	SDL_FreeSurface(creasurf);
	rect_move.x = 920; rect_move.y=1000;
	
	sprintf(pts, "Score: %d",score);
	font = TTF_OpenFont("fonts\\Rumbling.ttf",64);
	textSurface = TTF_RenderText_Solid(font, pts, textColor);
	
	textRect.x=1000;
	textRect.y= 340;
	textRect.w= textSurface->w;
	textRect.h= textSurface->h;
		
	sprintf(life, "x %d",lives);
	lifeSurface = TTF_RenderText_Solid(font, life, textColor);
	
	livesRect.x = 1080;
	livesRect.y = 440;//120
	livesRect.w = lifeSurface->w;
	livesRect.h = lifeSurface->h;
	
	paclives.x=1000;
	paclives.y= 455;//135
	paclives.w=50;
	paclives.h=50;
	
	
	
}

void pac_init(){
	
	pacmap = IMG_Load("images/pacmmap1.jpg");
	
	map_texture = SDL_CreateTextureFromSurface(renderer, pacmap);
		
	
	if(SDL_QueryTexture(map_texture, NULL, NULL, &mapRect.w, &mapRect.h ) != 0){
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_ExitWithError("Couldn't load  sprite texture");
	}
	
	SDL_FreeSurface(pacmap);
	
	mapRect.x = (WINDOW_WIDTH - mapRect.w)/2;
	mapRect.y = (WINDOW_HEIGHT - mapRect.h)/2;
	
	printf("\nMap at x = %d\t y = %d\n", mapRect.x, mapRect.y);
		
	pac_sprite.pacop = IMG_Load("images/pacop11.png");
	pac_sprite.pacclo = IMG_Load("images/pacclo11.png");
	
	pac_sprite.pacopv = IMG_Load("images/pacop111.png");
	pac_sprite.pacclov = IMG_Load("images/pacclo111.png");
	
	pac_sprite.textures[0] = SDL_CreateTextureFromSurface(renderer, pac_sprite.pacop);
	pac_sprite.textures[1] = SDL_CreateTextureFromSurface(renderer, pac_sprite.pacclo);
	
	pac_sprite.textures[2] = SDL_CreateTextureFromSurface(renderer, pac_sprite.pacopv);
	pac_sprite.textures[3] = SDL_CreateTextureFromSurface(renderer, pac_sprite.pacclov);
	
	
	if(SDL_QueryTexture(pac_sprite.textures[0] , NULL, NULL, &pac_sprite.rect.w, &pac_sprite.rect.h ) != 0){
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_ExitWithError("Couldn't load  sprite texture");
	}
	
	SDL_FreeSurface(pac_sprite.pacop);
	SDL_FreeSurface(pac_sprite.pacclo);

	SDL_FreeSurface(pac_sprite.pacopv);
	SDL_FreeSurface(pac_sprite.pacclov);
	
	pac_sprite.rect.w = 40;
	pac_sprite.rect.h = 40;
	pac_sprite.rect.x = mapRect.x + 50;
	pac_sprite.rect.y = 454;
	
	pac_sprite.right = clock();
	pac_sprite.left = clock();
	pac_sprite.up = clock();
	pac_sprite.down = clock();
	pac_sprite.speed = 3;
	
	pacdent = IMG_Load("images/pacmonster.png");
	pactex = SDL_CreateTextureFromSurface(renderer,pacdent);
	SDL_FreeSurface(pacdent);
	SDL_QueryTexture(pactex,NULL,NULL,&dentrect.w,&dentrect.h);
	dentrect.x=1000; dentrect.y=10;
	
	thump = IMG_Load("images//pacman-thumbsup.jpg");
	thump_tex = SDL_CreateTextureFromSurface(renderer, thump);
	SDL_FreeSurface(thump);
	SDL_QueryTexture(thump_tex,NULL,NULL,&thumpRect.w,&thumpRect.h);
	
	
}

void ghost_init(GHOST* ghost){
	
	ghost->surface = NULL;
	
	if (ghost->id == 1){
		ghost->surface = IMG_Load("images/bluegho.png"); 
		
		printf("\nWidth: %d Height:%d", ghost->rect.w, ghost->rect.h);
	}
		
	else if(ghost->id == 2){
		ghost->surface = IMG_Load("images/orangho.png");
	}
		
	else if(ghost->id == 3){
		ghost->surface = IMG_Load("images/pinkgho.png");
	}
	
	else{
		ghost->surface = IMG_Load("images/redgho.png");
	}
	
	ghost->texture[1] =  SDL_CreateTextureFromSurface(renderer, ghost->surface);
	
	if(SDL_QueryTexture(ghost->texture[1] , NULL, NULL, &ghost->rect.w, &ghost->rect.h ) != 0){
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_ExitWithError("Couldn't load  ghost texture");
	}
		
	ghost->rect.x = (WINDOW_WIDTH - ghost->rect.w)/2;
	ghost->rect.y = (WINDOW_HEIGHT - ghost->rect.h-60)/2;
	ghost->rect.w = 50;
	ghost->rect.h = 50;
	
	ghost->lastTime = clock();
	ghost->direction = UP;
	ghost->out =0;
	ghost->state=1;
	ghost->scared = 2000000000;
	
	ghost->surface = IMG_Load("images/white.png");
	ghost->texture[0] = SDL_CreateTextureFromSurface(renderer, ghost->surface);
	
	SDL_FreeSurface(ghost->surface);
	
	
}

void move_ghost(GHOST* ghost){
	
	for (int i = 0; i<=47; i++){
		if (!(out_of_bounds(ghost->rect, rect[i]))){
			if(!pause){
				float dt = clock() - ghost->duration;
				ghost->dx = dt*ghost->speed;
				ghost->dy = dt*ghost->speed;
				ghost->duration = clock();
			}
			
			switch(ghost->direction){
				case UP:
					move_up(ghost,rect[i], i);
					break;
								
				case DOWN:
					move_down(ghost,rect[i], i);
					break;
					
				case LEFT:
					move_left(ghost,rect[i], i);
					break;
								
				case RIGHT:
					move_right(ghost,rect[i], i);
					break;
								
				default:
					break;
			}
			
			if(!ghost->out){		
				if(ghost->rect.y+ghost->rect.h > rect[46].y -20){
					ghost->direction = UP;
				}
				else{
					ghost->out=1;
					ghost->direction = rand()%2+2;				
					ghost->lastTime = clock();
					
				}
			}	
			
			SDL_RenderCopy(renderer, ghost->texture[ghost->state],NULL, &ghost->rect);
		}
	}
}

void move_up(GHOST* ghost, SDL_Rect rect, int pos){
	ghost->rect.y -= ghost->dy;
	if (exceeds_up(ghost->rect, rect) && !(find_neighbour(ghost->rect, pos))){
		ghost->rect.y += ghost->dy;
		
		if(abs(clock() - ghost->lastTime > 200 )){	
			ghost->direction = rand()%2+2;
			ghost->lastTime = clock();
		}
			
	}
	if(abs(clock() - ghost->lastTime > 400 ) && ghost->out){	
		ghost->direction = rand()%2+2;
		ghost->lastTime = clock();
	}	
}

void move_down(GHOST* ghost, SDL_Rect rect, int pos){
	ghost->rect.y += ghost->dy;
	if (exceeds_down(ghost->rect, rect) && !(find_neighbour(ghost->rect, pos))){
		ghost->rect.y -= ghost->dy;
		
		if(abs(clock() - ghost->lastTime > 200 )){	
			ghost->direction = rand()%2+2;
			ghost->lastTime = clock();	
		}		
	}
	if(abs(clock() - ghost->lastTime > 400 )){	
		ghost->direction = rand()%2+2;
		ghost->lastTime = clock();	
	}
}

void move_right(GHOST* ghost, SDL_Rect rect, int pos){
	ghost->rect.x += ghost->dx;
	
	if (pos == 17 && exceeds_right(ghost->rect, rect)){ //For Ghost to pass through middle path
		ghost->rect.x = 10 + ghost->rect.w; ghost->rect.y = 454;
		return;
	}
	if (exceeds_right(ghost->rect, rect) && !(find_neighbour(ghost->rect, pos))){
		ghost->rect.x -= ghost->dx;
		
		if(abs(clock() - ghost->lastTime > 200 )){	
			ghost->direction = rand()%2;
			ghost->lastTime = clock();	
		}	
		
	}
	if(abs(clock() - ghost->lastTime >= 400 )){	
			ghost->direction = rand()%2;
			ghost->lastTime = clock();	
	}
}

void move_left(GHOST* ghost, SDL_Rect rect, int pos){
	ghost->rect.x -= ghost->dx;
	if (pos == 16 && exceeds_left(ghost->rect,rect)){ //For Ghost to pass through middle path
		ghost->rect.x = mapRect.x + mapRect.w - ghost->rect.w - 10; ghost->rect.y = 454;
		return;
	}		
			
	if (exceeds_left(ghost->rect, rect) && !(find_neighbour(ghost->rect, pos))){
		ghost->rect.x += ghost->dx; 
		
		if(abs(clock() - ghost->lastTime > 200 )){	
			ghost->direction = rand()%2;
			ghost->lastTime = clock();	
		}
	}
	if(abs(clock() - ghost->lastTime >= 400 )){	
		ghost->direction = rand()%2;
		ghost->lastTime = clock();	
	}		
}

void load_coin_textures(){
	
	SDL_Surface* surf = IMG_Load("images/5francss.png");
	SDL_Surface* ngombe = IMG_Load("images/10000frs.jpg");
	SDL_Surface* coin_500 = IMG_Load("images/500frs.png");
	for (int i = 0; i<146; i++){
		coin[i].texture = SDL_CreateTextureFromSurface(renderer, surf);
		
		if(SDL_QueryTexture(coin[i].texture , NULL, NULL, &coin[i].rect.w, &coin[i].rect.h ) != 0){
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_ExitWithError("Couldn't load  coin texture");
		}
		coin[i].rect.w = 16; coin[i].rect.h = 16; 
	}
	
	coin_move.texture = SDL_CreateTextureFromSurface(renderer, surf);
	if(SDL_QueryTexture(coin_move.texture , NULL, NULL, &coin_move.rect.w, &coin_move.rect.h ) != 0){
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_ExitWithError("Couldn't load  coin texture");
		}
	coin_move.rect.w = 16; coin_move.rect.h = 16; 
	
	for (int i=0; i<4;i++){
		super_coin[i].texture = SDL_CreateTextureFromSurface(renderer,ngombe);
		if(SDL_QueryTexture(super_coin[i].texture , NULL, NULL, &super_coin[i].rect.w, &super_coin[i].rect.h ) != 0){
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_ExitWithError("Couldn't load  Ngombe texture");
		}
		super_coin[i].rect.w=40; super_coin[i].rect.h=20;
	}
	
	for(int i=0;i<3;i++){
		mid_coin[i].texture = SDL_CreateTextureFromSurface(renderer,coin_500);
		if(SDL_QueryTexture(mid_coin[i].texture , NULL, NULL, &mid_coin[i].rect.w, &mid_coin[i].rect.h ) != 0){
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_ExitWithError("Couldn't load  Fapsan texture");
		}		
		mid_coin[i].rect.w=30; mid_coin[i].rect.h=30;
	}
	
	SDL_FreeSurface(surf);
	SDL_FreeSurface(ngombe);
	SDL_FreeSurface(coin_500);
}


void init_coins(){
	coin[0].rect.x = 98;	coin[0].rect.y = 94;	coin[0].touched=0;

	coin[1].rect.x = 98;	coin[1].rect.y = 222;	coin[1].touched=0;
	
	coin[2].rect.x = 258;	coin[2].rect.y = 94;	coin[2].touched=0;
	
	coin[3].rect.x = 258;	coin[3].rect.y = 222;	coin[3].touched=0;
	
	coin[4].rect.x = 258;	coin[4].rect.y = 350;	coin[4].touched=0;
	
	coin[5].rect.x = 258;	coin[5].rect.y = 414;	coin[5].touched=0;
	
	coin[6].rect.x = 258;	coin[6].rect.y = 542;	coin[6].touched=0;
	
	coin[7].rect.x = 258;	coin[7].rect.y = 606;	coin[7].touched=0;
	
	coin[8].rect.x = 258;	coin[8].rect.y = 734;	coin[8].touched=0;
	
	coin[9].rect.x = 258;	coin[9].rect.y = 798;	coin[9].touched=0;
	
	coin[10].rect.x = 750;	coin[10].rect.y = 94;	coin[10].touched=0;
	
	coin[11].rect.x = 750;	coin[11].rect.y = 222;	coin[11].touched=0;
	
	coin[12].rect.x = 750;	coin[12].rect.y = 350;	coin[12].touched=0;
	
	coin[13].rect.x = 750;	coin[13].rect.y = 414;	coin[13].touched=0;
	
	coin[14].rect.x = 750;	coin[14].rect.y = 542;	coin[14].touched=0;
	
	coin[15].rect.x = 750;	coin[15].rect.y = 606;	coin[15].touched=0;
	
	coin[16].rect.x = 750;	coin[16].rect.y = 734;	coin[16].touched=0;
	
	coin[17].rect.x = 750;	coin[17].rect.y = 798;	coin[17].touched=0;
	
	coin[18].rect.x = 99;	coin[18].rect.y = 48;	coin[18].touched=0;
	
	coin[19].rect.x = 163;	coin[19].rect.y = 48;	coin[19].touched=0;
	
	coin[20].rect.x = 227;	coin[20].rect.y = 48;	coin[20].touched=0;
	
	coin[21].rect.x = 291;	coin[21].rect.y = 48;	coin[21].touched=0;
	
	coin[22].rect.x = 355;	coin[22].rect.y = 48;	coin[22].touched=0;
	
	coin[23].rect.x = 419;	coin[23].rect.y = 48;	coin[23].touched=0;
	
	coin[24].rect.x = 559;	coin[24].rect.y = 44;	coin[24].touched=0;
	
	coin[25].rect.x = 623;	coin[25].rect.y = 44;	coin[25].touched=0;
	
	coin[26].rect.x = 687;	coin[26].rect.y = 44;	coin[26].touched=0;
	
	coin[27].rect.x = 751;	coin[27].rect.y = 44;	coin[27].touched=0;
	
	coin[28].rect.x = 815;	coin[28].rect.y = 44;	coin[28].touched=0;
	
	coin[29].rect.x = 879;	coin[29].rect.y = 44;	coin[29].touched=0;
	
	coin[30].rect.x = 99;	coin[30].rect.y = 180;	coin[30].touched=0;
	
	coin[31].rect.x = 163;	coin[31].rect.y = 180;	coin[31].touched=0;
	
	coin[32].rect.x = 227;	coin[32].rect.y = 180;	coin[32].touched=0;
	
	coin[33].rect.x = 291;	coin[33].rect.y = 180;	coin[33].touched=0;
	
	coin[34].rect.x = 355;	coin[34].rect.y = 180;	coin[34].touched=0;
	
	coin[35].rect.x = 419;	coin[35].rect.y = 180;	coin[35].touched=0;
	
	coin[36].rect.x = 483;	coin[36].rect.y = 180;	coin[36].touched=0;
	
	coin[37].rect.x = 547;	coin[37].rect.y = 180;	coin[37].touched=0;
	
	coin[38].rect.x = 611;	coin[38].rect.y = 180;	coin[38].touched=0;
	
	coin[39].rect.x = 675;	coin[39].rect.y = 180;	coin[39].touched=0;
	
	coin[40].rect.x = 739;	coin[40].rect.y = 180;	coin[40].touched=0;
	
	coin[41].rect.x = 803;	coin[41].rect.y = 180;	coin[41].touched=0;
	
	coin[42].rect.x = 867;	coin[42].rect.y = 180;	coin[42].touched=0;
	
	coin[43].rect.x = 931;	coin[43].rect.y = 180;	coin[43].touched=0;
	
	coin[44].rect.x = 99;	coin[44].rect.y = 964;	coin[44].touched=0;
	
	coin[45].rect.x = 163;	coin[45].rect.y = 964;	coin[45].touched=0;
	
	coin[46].rect.x = 227;	coin[46].rect.y = 964;	coin[46].touched=0;
	
	coin[47].rect.x = 291;	coin[47].rect.y = 964;	coin[47].touched=0;
	
	coin[48].rect.x = 355;	coin[48].rect.y = 964;	coin[48].touched=0;
	
	coin[49].rect.x = 419;	coin[49].rect.y = 964;	coin[49].touched=0;
	
	coin[50].rect.x = 483;	coin[50].rect.y = 964;	coin[50].touched=0;
	
	coin[51].rect.x = 547;	coin[51].rect.y = 964;	coin[51].touched=0;
	
	coin[52].rect.x = 611;	coin[52].rect.y = 964;	coin[52].touched=0;
	
	coin[53].rect.x = 675;	coin[53].rect.y = 964;	coin[53].touched=0;
	
	coin[54].rect.x = 739;	coin[54].rect.y = 964;	coin[54].touched=0;
	
	coin[55].rect.x = 803;	coin[55].rect.y = 964;	coin[55].touched=0;
	
	coin[56].rect.x = 867;	coin[56].rect.y = 964;	coin[56].touched=0;
	
	coin[57].rect.x = 931;	coin[57].rect.y = 964;	coin[57].touched=0;
	
	coin[58].rect.x = 99;	coin[58].rect.y = 868;	coin[58].touched=0;
	
	coin[59].rect.x = 163;	coin[59].rect.y = 868;	coin[59].touched=0;
	
	coin[60].rect.x = 227;	coin[60].rect.y = 868;	coin[60].touched=0;
	
	coin[61].rect.x = 99;	coin[61].rect.y = 672;	coin[61].touched=0;
	
	coin[62].rect.x = 163;	coin[62].rect.y = 672;	coin[62].touched=0;
	
	coin[63].rect.x = 227;	coin[63].rect.y = 672;	coin[63].touched=0;
	
	coin[64].rect.x = 291;	coin[64].rect.y = 672;	coin[64].touched=0;
	
	coin[65].rect.x = 355;	coin[65].rect.y = 672;	coin[65].touched=0;
	
	coin[66].rect.x = 419;	coin[66].rect.y = 672;	coin[66].touched=0;
	
	coin[67].rect.x = 559;	coin[67].rect.y = 672;	coin[67].touched=0;
	
	coin[68].rect.x = 623;	coin[68].rect.y = 672;	coin[68].touched=0;
	
	coin[69].rect.x = 687;	coin[69].rect.y = 672;	coin[69].touched=0;
	
	coin[70].rect.x = 751;	coin[70].rect.y = 672;	coin[70].touched=0;
	
	coin[71].rect.x = 815;	coin[71].rect.y = 672;	coin[71].touched=0;
	
	coin[72].rect.x = 879;	coin[72].rect.y = 672;	coin[72].touched=0;
	
	coin[73].rect.x = 263;	coin[73].rect.y = 768;	coin[73].touched=0;
	
	coin[74].rect.x = 327;	coin[74].rect.y = 768;	coin[74].touched=0;
	
	coin[75].rect.x = 391;	coin[75].rect.y = 768;	coin[75].touched=0;
	
	coin[76].rect.x = 455;	coin[76].rect.y = 768;	coin[76].touched=0;
	
	coin[77].rect.x = 519;	coin[77].rect.y = 768;	coin[77].touched=0;
	
	coin[78].rect.x = 583;	coin[78].rect.y = 768;	coin[78].touched=0;
	
	coin[79].rect.x = 647;	coin[79].rect.y = 768;	coin[79].touched=0;
	
	coin[80].rect.x = 711;	coin[80].rect.y = 768;	coin[80].touched=0;
	
	coin[81].rect.x = 359;	coin[81].rect.y = 868;	coin[81].touched=0;
	
	coin[82].rect.x = 423;	coin[82].rect.y = 868;	coin[82].touched=0;
	
	coin[83].rect.x = 559;	coin[83].rect.y = 868;	coin[83].touched=0;
	
	coin[84].rect.x = 623;	coin[84].rect.y = 868;	coin[84].touched=0;
	
	coin[85].rect.x = 755;	coin[85].rect.y = 868;	coin[85].touched=0;
	
	coin[86].rect.x = 819;	coin[86].rect.y = 868;	coin[86].touched=0;
	
	coin[87].rect.x = 883;	coin[87].rect.y = 868;	coin[87].touched=0;
	
	coin[88].rect.x = 855;	coin[88].rect.y = 768;	coin[88].touched=0;
	
	coin[89].rect.x = 919;	coin[89].rect.y = 768;	coin[89].touched=0;
	
	coin[90].rect.x = 363;	coin[90].rect.y = 572;	coin[90].touched=0;
	
	coin[91].rect.x = 427;	coin[91].rect.y = 572;	coin[91].touched=0;
	
	coin[92].rect.x = 491;	coin[92].rect.y = 572;	coin[92].touched=0;
	
	coin[93].rect.x = 555;	coin[93].rect.y = 572;	coin[93].touched=0;
	
	coin[94].rect.x = 619;	coin[94].rect.y = 572;	coin[94].touched=0;
	
	coin[95].rect.x = 79;	coin[95].rect.y = 472;	coin[95].touched=0;
	
	coin[96].rect.x = 143;	coin[96].rect.y = 472;	coin[96].touched=0;
	
	coin[97].rect.x = 207;	coin[97].rect.y = 472;	coin[97].touched=0;
	
	coin[98].rect.x = 271;	coin[98].rect.y = 472;	coin[98].touched=0;
	
	coin[99].rect.x = 335;	coin[99].rect.y = 472;	coin[99].touched=0;
	
	coin[100].rect.x = 655;	coin[100].rect.y = 472;	coin[100].touched=0;
	
	coin[101].rect.x = 719;	coin[101].rect.y = 472;	coin[101].touched=0;
	
	coin[102].rect.x = 783;	coin[102].rect.y = 472;	coin[102].touched=0;
	
	coin[103].rect.x = 847;	coin[103].rect.y = 472;	coin[103].touched=0;
	
	coin[104].rect.x = 911;	coin[104].rect.y = 472;	coin[104].touched=0;
	
	coin[105].rect.x = 363;	coin[105].rect.y = 376;	coin[105].touched=0;
	
	coin[106].rect.x = 427;	coin[106].rect.y = 376;	coin[106].touched=0;
	
	coin[107].rect.x = 491;	coin[107].rect.y = 376;	coin[107].touched=0;
	
	coin[108].rect.x = 555;	coin[108].rect.y = 376;	coin[108].touched=0;
	
	coin[109].rect.x = 619;	coin[109].rect.y = 376;	coin[109].touched=0;
	
	coin[110].rect.x = 363;	coin[110].rect.y = 276;	coin[110].touched=0;
	
	coin[111].rect.x = 427;	coin[111].rect.y = 276;	coin[111].touched=0;
	
	coin[112].rect.x = 559;	coin[112].rect.y = 280;	coin[112].touched=0;
	
	coin[113].rect.x = 623;	coin[113].rect.y = 280;	coin[113].touched=0;
	
	coin[114].rect.x = 755;	coin[114].rect.y = 280;	coin[114].touched=0;
	
	coin[115].rect.x = 819;	coin[115].rect.y = 280;	coin[115].touched=0;
	
	coin[116].rect.x = 883;	coin[116].rect.y = 280;	coin[116].touched=0;
	
	coin[117].rect.x = 99;	coin[117].rect.y = 276;	coin[117].touched=0;
	
	coin[118].rect.x = 163;	coin[118].rect.y = 276;	coin[118].touched=0;
	
	coin[119].rect.x = 227;	coin[119].rect.y = 276;	coin[119].touched=0;
	
	coin[120].rect.x = 458;	coin[120].rect.y = 94;	coin[120].touched=0;
	
	coin[121].rect.x = 554;	coin[121].rect.y = 94;	coin[121].touched=0;
	
	coin[122].rect.x = 914;	coin[122].rect.y = 94;	coin[122].touched=0;
	
	coin[123].rect.x = 914;	coin[123].rect.y = 222;	coin[123].touched=0;
	
	coin[124].rect.x = 654;	coin[124].rect.y = 230;	coin[124].touched=0;
	
	coin[125].rect.x = 358;	coin[125].rect.y = 230;	coin[125].touched=0;
	
	coin[126].rect.x = 458;	coin[126].rect.y = 326;	coin[126].touched=0;
	
	coin[127].rect.x = 554;	coin[127].rect.y = 326;	coin[127].touched=0;
	
	coin[128].rect.x = 358;	coin[128].rect.y = 426;	coin[128].touched=0;
	
	coin[129].rect.x = 358;	coin[129].rect.y = 618;	coin[129].touched=0;
	
	coin[130].rect.x = 654;	coin[130].rect.y = 426;	coin[130].touched=0;
	
	coin[131].rect.x = 654;	coin[131].rect.y = 618;	coin[131].touched=0;
	
	coin[132].rect.x = 554;	coin[132].rect.y = 718;	coin[132].touched=0;
	
	coin[133].rect.x = 914;	coin[133].rect.y = 718;	coin[133].touched=0;
	
	coin[134].rect.x = 458;	coin[134].rect.y = 718;	coin[134].touched=0;
	
	coin[135].rect.x = 97;	coin[135].rect.y = 718;	coin[135].touched=0;
	
	coin[136].rect.x = 158;	coin[136].rect.y = 818;	coin[136].touched=0;
	
	coin[137].rect.x = 98;	coin[137].rect.y = 914;	coin[137].touched=0;
	
	coin[138].rect.x = 458;	coin[138].rect.y = 914;	coin[138].touched=0;
	
	coin[139].rect.x = 554;	coin[139].rect.y = 914;	coin[139].touched=0;
	
	coin[140].rect.x = 918;	coin[140].rect.y = 914;	coin[140].touched=0;
	
	coin[141].rect.x = 853;	coin[141].rect.y = 818;	coin[141].touched=0;
	
	coin[142].rect.x = 354;	coin[142].rect.y = 814;	coin[142].touched=0;
	
	coin[143].rect.x = 654;	coin[143].rect.y = 814;	coin[143].touched=0;
	
	coin[144].rect.x = 99;	coin[144].rect.y = 768;	coin[144].touched=0;
	
	coin[145].rect.x = 163;	coin[145].rect.y = 768;	coin[145].touched=0;
		

	for (int i=0; i<4; i++){
		int r = rand()%146;
		super_coin[i].rect.x = coin[r].rect.x+coin[r].rect.w-super_coin[i].rect.w; super_coin[i].rect.y = coin[r].rect.y;
		super_coin[i].touched=0;
	}

	for(int i=0; i<3 ; i++){
		int r = rand()%100;
		mid_coin[i].rect.x = coin[r].rect.x+coin[r].rect.w- mid_coin[i].rect.w; mid_coin[i].rect.y = coin[r].rect.y;
		mid_coin[i].touched = 0;
	}
}




void display_coins(){

	for (int i=0; i<=145;i++){
		if(SDL_HasIntersection(&coin[i].rect,&pac_sprite.rect)){
			coin[i].touched = 1;
			coin[i].rect.x = 0; coin[i].rect.y = 0;
			score+=5;
			Mix_PlayChannel(-1,eating,0);
			count+=1;
		}
		if (!coin[i].touched && !(SDL_HasIntersection(&coin[i].rect, &mid_coin[0].rect) || SDL_HasIntersection(&coin[i].rect, &mid_coin[1].rect) || SDL_HasIntersection(&coin[i].rect, &mid_coin[2].rect))){
			SDL_RenderCopy(renderer, coin[i].texture, NULL, &coin[i].rect);
		} 
	}
	
	for (int i=0; i<4; i++){
		if(SDL_HasIntersection(&super_coin[i].rect,&pac_sprite.rect) && ghost[i].state){
			Mix_PlayChannel(-1, eat_supercoin,0);
			Mix_PlayChannel(-1, eat_supercoin1, 0);
			super_coin[i].touched = 1;
			score+=10000;
			for (int j=0; j<4; j++){
				ghost[j].state = 0; //Ghost enter in scared state
				ghost[j].scared = clock();
			}
			super_coin[i].rect.x=0; super_coin[i].rect.y=0;
			
		}
		
		if (!super_coin[i].touched){
			SDL_RenderCopy(renderer, super_coin[i].texture, NULL, &super_coin[i].rect);
		}
	}
	
	for(int i=0;i<3;i++){
		if(SDL_HasIntersection(&mid_coin[i].rect,&pac_sprite.rect)){
			mid_coin[i].touched = 1;
			mid_coin[i].rect.x = 0; mid_coin[i].rect.y = 0;
			score+= 500;
			lives += 1;
			Mix_PlayChannel(-1,coin500,0);
			Mix_PlayChannel(-1, coin_500_1, 0);
		}
		if (!mid_coin[i].touched){
			SDL_RenderCopy(renderer, mid_coin[i].texture, NULL, &mid_coin[i].rect);
		}
	}
}

void check_contact(){
	if(clock() - ghost[1].scared > 5000){
		for(int i=0; i<4; i++){
			ghost[i].state=1;
		}	
	}
	
	for (int i=0; i<4; i++){
		if (SDL_HasIntersection(&ghost[i].rect, &pac_sprite.rect)){
			if(!ghost[i].state){
				ghost[i].rect.x = (WINDOW_WIDTH - ghost[i].rect.w)/2;
				ghost[i].rect.y = (WINDOW_HEIGHT - ghost[i].rect.h-60)/2;
				ghost[i].out=0;
				ghost[i].direction = UP;
				ghost[i].state =1;
				score += 200;
				Mix_PlayChannel(-1, killed, 0);
				Mix_PlayChannel(-1, eaten, 0);
			}
			
			else{
				lives-=1;
				Mix_PlayChannel(-1, killed, 0);
				Mix_PlayChannel(-1,killed1,0);
			
				SDL_Delay(500);
				for(int i=0; i<4; i++){
					ghost[i].rect.x = (WINDOW_WIDTH - ghost[i].rect.w)/2;
					ghost[i].rect.y = (WINDOW_HEIGHT - ghost[i].rect.h-60)/2;
					ghost[i].out=0;
					ghost[i].direction = UP;
					ghost[i].state =1;
				}
				pac_sprite.rect.x = mapRect.x + 50;
				pac_sprite.rect.y = 454;
				
			}
		}
	}
}

void display_score(){
	sprintf(pts, "Score: %d",score);
	textSurface = TTF_RenderText_Solid(font, pts, textColor);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if(textTexture == NULL){
		SDL_ExitWithError("error creating score texture");
	}
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	SDL_FreeSurface(textSurface);
	
	SDL_RenderCopy(renderer,pactex,NULL, &dentrect);
	
	SDL_RenderCopy(renderer, pac_sprite.textures[0], NULL, &paclives);
	sprintf(life, "X %d",lives);
	lifeSurface = TTF_RenderText_Solid(font, life, textColor);
	lifeTexture = SDL_CreateTextureFromSurface(renderer, lifeSurface);
	SDL_FreeSurface(lifeSurface);
	
	if(lifeTexture == NULL){
		SDL_ExitWithError("error creating lives texture");
	}
	SDL_RenderCopy(renderer, lifeTexture, NULL, &livesRect);	
	
	if (lives==0){
		game_state=QUIT;
		Mix_FreeMusic(bgmusic[music]);
		overSurface = TTF_RenderText_Solid(font, "GAME OVER!!", textColor);
		SDL_Texture* overTexture = SDL_CreateTextureFromSurface(renderer, overSurface);
		
		overRect.x = (WINDOW_WIDTH - ghost[0].rect.w)/2 - 110;
		overRect.y = (WINDOW_HEIGHT - ghost[0].rect.h-60)/2;
		overRect.w = overSurface->w; 
		overRect.h = overSurface->h;
		SDL_RenderCopy(renderer, overTexture, NULL, &overRect);
		SDL_FreeSurface(overSurface);
			
		SDL_RenderPresent(renderer);
		Mix_PlayChannel(-1, gameover, 0);
		Mix_PlayChannel(-1, gameover1, 0);
		SDL_Delay(3000);
		
	}	
	
	if(count==146){
		Mix_FreeMusic(bgmusic[music]);
		overSurface = TTF_RenderText_Solid(font, "YOU WIN!!", textColor);
		SDL_Texture* overTexture = SDL_CreateTextureFromSurface(renderer, overSurface);
		overRect.x = (WINDOW_WIDTH - ghost[0].rect.w)/2 - 100;
		overRect.y = (WINDOW_HEIGHT - ghost[0].rect.h-60)/2;
		overRect.w = overSurface->w; 
		overRect.h = overSurface->h;
		SDL_FreeSurface(overSurface);
		
		SDL_RenderCopy(renderer, overTexture, NULL, &overRect);
		SDL_RenderPresent(renderer);
		Mix_PlayChannel(-1, winner, 0);
		Mix_PlayChannel(-1, winner1,0);
		SDL_Delay(3000);
		game_state=QUIT;
	}	
}

int menu(){
		
		
	menuMusic = Mix_LoadMUS("songs\\city_hunter_sara_mp3_69657.mp3");
	select = Mix_LoadWAV("sfx\\common_00060.wav");
	chosen = Mix_LoadWAV("sfx\\common_00061.wav");
	okay = Mix_LoadWAV("sfx\\capcom-vs-snk-2-terry-okay.mp3");
	oh_no = Mix_LoadWAV("sfx\\f_09_voice_683308452.ogg");
	Mix_PlayMusic(menuMusic,-1);
	
	SDL_SetRenderDrawColor(renderer, 255,255,255,0);
	SDL_RenderClear(renderer);
	
	SDL_RenderCopy(renderer,monstexture,NULL,&monstrect);
	
	font1 = TTF_OpenFont("C:\\Users\\Time_for_Business\\Documents\\Python 1st Game\\Rush_turbo.ttf",124);
	
	char title[50];
	sprintf(title, "\n WELCOME TO PAC-MAN \n");
	
	
	SDL_Surface* titleSurface = TTF_RenderText_Solid(font1,title,textColor1);
	SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer,titleSurface);
	SDL_Rect titleRect = {120,10,titleSurface->w, titleSurface->h};
	SDL_RenderCopy(renderer, titleTexture,NULL, &titleRect);
		
	sprintf(title, "1. Play");
	SDL_Surface* play_surface = TTF_RenderText_Solid(font1, title, textColor1);
	SDL_Texture* play_texture = SDL_CreateTextureFromSurface(renderer,play_surface);
	SDL_Rect playRect = {220, 10+titleSurface->h+20, play_surface->w, play_surface->h};
	SDL_RenderCopy(renderer,play_texture,NULL,&playRect);
	
	SDL_SetRenderDrawColor(renderer, 10, 150, 255, 255);
	SDL_RenderDrawLine(renderer, titleRect.x, titleRect.y+titleRect.h-40, titleRect.x+titleRect.w, titleRect.y+titleRect.h-40);

	
	sprintf(title, "2. Quit");
	SDL_Surface* quit_surface = TTF_RenderText_Solid(font1, title, red);
	SDL_Texture* quit_texture = SDL_CreateTextureFromSurface(renderer,quit_surface);
	SDL_Rect quitRect = {220, playRect.y+playRect.h+70, quit_surface->w, quit_surface->h};
	SDL_RenderCopy(renderer,quit_texture,NULL,&quitRect);
	
	SDL_RenderCopy(renderer,creator,NULL,&rect_move);
	
	
	SDL_RenderPresent(renderer);
	SDL_FreeSurface(titleSurface);
	SDL_FreeSurface(play_surface);
	SDL_FreeSurface(quit_surface);
	
	SDL_Event event;
	
	while(1){
		
		while (SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				return QUIT;
			}
			if (event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym){
					
					case SDLK_q:
						SDL_SetRenderDrawColor(renderer, 250, 0, 0, 255);
						SDL_RenderDrawRect(renderer, &quitRect);
						Mix_FreeMusic(menuMusic);
						SDL_RenderPresent(renderer);
						Mix_PlayChannel(-1,chosen,0);
						Mix_PlayChannel(-1, oh_no,0);
						SDL_Delay(1000);
						return QUIT;
					
					case SDLK_1:
						SDL_SetRenderDrawColor(renderer, 10, 150, 255, 255);
						SDL_RenderDrawRect(renderer, &playRect);
						SDL_RenderDrawLine(renderer, titleRect.x, titleRect.y+titleRect.h-40, titleRect.x+titleRect.w, titleRect.y+titleRect.h-40);
						SDL_RenderPresent(renderer);
						Mix_PlayChannel(-1,chosen,0);
						SDL_Delay(500);
						return PLAY;
						break;
					
					case SDLK_2:
						SDL_SetRenderDrawColor(renderer, 250, 0, 0, 255);
						SDL_RenderDrawRect(renderer, &quitRect);
						SDL_RenderDrawLine(renderer, titleRect.x, titleRect.y+titleRect.h-40, titleRect.x+titleRect.w, titleRect.y+titleRect.h-40);
						SDL_RenderPresent(renderer);
						Mix_PlayChannel(-1,chosen,0);
						Mix_FreeMusic(menuMusic);
						Mix_PlayChannel(-1, oh_no,0);
						SDL_Delay(1000);
						return QUIT;
						break;
				
					case SDLK_r:
						SDL_SetRenderDrawColor(renderer, 255,255,255,0);
						SDL_RenderClear(renderer);
						SDL_SetRenderDrawColor(renderer, 10, 150, 255, 255);
						SDL_RenderCopy(renderer,monstexture,NULL,&monstrect);
						SDL_RenderCopy(renderer, titleTexture,NULL, &titleRect);
						SDL_RenderCopy(renderer,play_texture,NULL,&playRect);
						SDL_RenderDrawLine(renderer, titleRect.x, titleRect.y+titleRect.h-40, titleRect.x+titleRect.w, titleRect.y+titleRect.h-40);
						SDL_RenderCopy(renderer,quit_texture,NULL,&quitRect);
						SDL_RenderCopy(renderer,creator,NULL,&rect_move);
						SDL_RenderPresent(renderer);
				}
			}
			
		}
	}
}

void free_sounds(){
	Mix_FreeChunk(eat_ghost);
	Mix_FreeChunk(gameover);
	Mix_FreeChunk(gameover1);
	Mix_FreeChunk(eat_supercoin);
	Mix_FreeChunk(eat_supercoin1);
	Mix_FreeChunk(killed);
	Mix_FreeChunk (killed1);
	Mix_FreeChunk (coin500);
	Mix_FreeChunk(eating);
	Mix_FreeChunk (laughter);
	Mix_FreeChunk (winner);
	Mix_FreeChunk (winner1);
	Mix_FreeChunk (coin_500_1);
	Mix_FreeChunk (eaten);
	Mix_FreeChunk(chosen);
	Mix_FreeChunk(okay);
	Mix_FreeChunk(oh_no);
}

void play(){
	SDL_RenderClear(renderer);

	music = rand()%4;
	printf("music %d", music);
	game_state=PLAY;
	lives=3;
	score=0;
	count=0;
	
	switch(level){
		case QUIT:
			game_state=QUIT;
			break;
			
		case 1:
			for(int i=0;i<4;i++){
				ghost[i].speed=1;
			}
			lives=3;
			break;
			
		case 2:
			for(int i=0;i<4;i++){
				ghost[i].speed=2;
			}
			lives=2;
			break;
			
		case 3:
			for(int i=0;i<4;i++){
				ghost[i].speed=3;
			}
			lives=1;
			break;
	}
	
	/*Place pacman sprite on map*/
	pac_sprite.rect.x = mapRect.x + 50;
	pac_sprite.rect.y = 454;
	
	//Initialise ghosts sprites
	for (int i=0; i<4; i++){
		ghost[i].rect.x = (WINDOW_WIDTH - ghost[i].rect.w)/2;
		ghost[i].rect.y = (WINDOW_HEIGHT - ghost[i].rect.h-60)/2;
		ghost[i].out=0;
		ghost[i].direction = UP;
		ghost[i].state =1;
	}

	/*Display coins and rectangles on the map*/
	init_rects(); 
	init_coins();

//	/*Initialisation soundtracts and sound effects*/
	sounds_init();
			
	printf("\nPacman at x = %d\t y = %d\n", pac_sprite.rect.x, pac_sprite.rect.y);

	Mix_PlayMusic(bgmusic[music], -1);
		
	while(game_state){
		
		SDL_RenderClear(renderer);
		
		SDL_RenderCopy(renderer, map_texture, NULL, &mapRect);
		draw_rectangles();
		display_coins();

		check_contact();

		for (int i = 0; i<=47; i++){
			if (!(out_of_bounds(pac_sprite.rect, rect[i]))){
//				printf("\nPac rect: rect[%d]\n", i);
				check_events(rect[i], i);
			}
		}
		
		if(actu_key == SDLK_LEFT){
			flip_left();
		}
		
		else if (actu_key == SDLK_UP){
			flip_up();
		}
		
		else if (actu_key == SDLK_DOWN){
			flip_down();
		}
		
		else{
			flip_right();
		}
		
		
		for(int i=0; i<4; i++){
			move_ghost(&ghost[i]);
		}
		
		display_score();
		SDL_RenderPresent(renderer);		
	}
	
	SDL_RenderClear(renderer);

}

void quit(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	printf("Exiting SDL.");
}

void sounds_init(){
	
	bgmusic[0] = Mix_LoadMUS("songs\\Daniel Janin - Trumpet Flower.mp3");
	bgmusic[1] = Mix_LoadMUS("songs\\Dragonball Z – Cha-La Head-Cha-La.mp3");
	bgmusic[2] = Mix_LoadMUS("songs\\Cowboy Bebop OST – Cats on Mars.mp3");
	bgmusic[3] = Mix_LoadMUS("songs\\It-Was-A-Good-Day.mp3");
	bgmusic[4] = Mix_LoadMUS("songs\\Bill Withers - Lovely day.mp3");
	gameover = Mix_LoadWAV("sfx\\common_00126.wav");
	eat_supercoin = Mix_LoadWAV("sfx\\common_00048.wav");
	eat_supercoin1 = Mix_LoadWAV("sfx\\f_09_voice_481654217.ogg");
	killed = Mix_LoadWAV("sfx\\common_00014.wav");
	killed1 = Mix_LoadWAV("sfx\\f_0F_voice_188404847.ogg");
	coin500 = Mix_LoadWAV("sfx\\snk_coin.mp3");
	gameover1 = Mix_LoadWAV("sfx\\mixkit-cartoon-whistle-game-over-606.wav");
	eating = Mix_LoadWAV("sfx\\collectcoin-6075.mp3");
	winner = Mix_LoadWAV("sfx\\common_00128.wav");
	winner1 = Mix_LoadWAV("sfx\\13. Winner (Victory Demo).mp3");
	coin_500_1 = Mix_LoadWAV("sfx\\f_0B_voice_987605327.ogg");
	eaten = Mix_LoadWAV("sfx\\f_10_voice_409820902.ogg");
}

int difficulty(){
	SDL_SetRenderDrawColor(renderer,255,255,255,0);
	SDL_RenderClear(renderer);
	char level[50];
	font1 = TTF_OpenFont("fonts\\Rush_turbo.ttf",84);
	
	
	sprintf(level, "SELECT DIFFICULTY");
	SDL_Surface* sel_surf= TTF_RenderText_Solid(font1,level,textColor1);
	SDL_Texture* sel_tex = SDL_CreateTextureFromSurface(renderer,sel_surf);
	SDL_Rect selRect = {120,10,sel_surf->w, sel_surf->h};
	SDL_FreeSurface(sel_surf);
	SDL_RenderCopy(renderer, sel_tex,NULL, &selRect);
	
	sprintf(level, "1. Newbie");
	SDL_Surface* new_surf= TTF_RenderText_Solid(font1,level,textColor1);
	SDL_Texture* new_tex = SDL_CreateTextureFromSurface(renderer,new_surf);
	SDL_Rect newRect = {160,selRect.y+selRect.h+20,new_surf->w, new_surf->h};
	SDL_FreeSurface(new_surf);
	SDL_RenderCopy(renderer, new_tex,NULL, &newRect);
	
	sprintf(level, "2. Connoisseur");
	SDL_Surface* con_surf= TTF_RenderText_Solid(font1,level,black);
	SDL_Texture* con_tex = SDL_CreateTextureFromSurface(renderer,con_surf);
	SDL_Rect conRect = {160,newRect.y+newRect.h+20,con_surf->w, con_surf->h};
	SDL_FreeSurface(con_surf);
	SDL_RenderCopy(renderer, con_tex,NULL, &conRect);
	
	sprintf(level, "3. Inferno");
	SDL_Surface* in_surf= TTF_RenderText_Solid(font1,level,red);
	SDL_Texture* in_tex = SDL_CreateTextureFromSurface(renderer,in_surf);
	SDL_Rect inRect = {160,conRect.y+conRect.h+20,in_surf->w, in_surf->h};
	SDL_FreeSurface(in_surf);
	SDL_RenderCopy(renderer, in_tex,NULL, &inRect);
	
	thumpRect.x = conRect.x+conRect.w+100;
	thumpRect.y = newRect.y-150;
	SDL_RenderCopy(renderer,thump_tex,NULL,&thumpRect);
	
	
	SDL_SetRenderDrawColor(renderer, 10, 150, 255, 255);
	SDL_RenderDrawLine(renderer, selRect.x, selRect.y+selRect.h-20, selRect.x+selRect.w, selRect.y+selRect.h-20);
	SDL_RenderCopy(renderer,creator,NULL,&rect_move);
	SDL_RenderPresent(renderer);
	
	SDL_Event event; int n;
	
	while(1){
		
		while (SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				return QUIT;
			}
			if (event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym){
					
					case SDLK_m:
						return QUIT;
					
					case SDLK_1:
						Mix_FreeMusic(menuMusic);
						Mix_PlayChannel(-1,chosen,0);
						SDL_SetRenderDrawColor(renderer, 10, 150, 255, 255);
						SDL_RenderDrawRect(renderer, &newRect);
						SDL_RenderDrawLine(renderer, selRect.x, selRect.y+selRect.h-20, selRect.x+selRect.w, selRect.y+selRect.h-20);
						SDL_RenderPresent(renderer);
						Mix_PlayChannel(-1,okay,0);
						SDL_Delay(1000);
						SDL_SetRenderDrawColor(renderer, 0,0,0,0);
						return 1;			
						break;
					
					case SDLK_2:
						Mix_FreeMusic(menuMusic);
						Mix_PlayChannel(-1,chosen,0);
						SDL_SetRenderDrawColor(renderer,0,0,0,255);
						SDL_RenderDrawRect(renderer, &conRect);
						SDL_RenderDrawLine(renderer, selRect.x, selRect.y+selRect.h-20, selRect.x+selRect.w, selRect.y+selRect.h-20);
						SDL_RenderPresent(renderer);
						Mix_PlayChannel(-1,okay,0);
						SDL_Delay(1000);
						SDL_SetRenderDrawColor(renderer, 0,0,0,0);
						return 2;
						break;
						
					case SDLK_3:
						Mix_FreeMusic(menuMusic);
						Mix_PlayChannel(-1,chosen,0);
						SDL_SetRenderDrawColor(renderer, 250, 0, 0, 255);
						SDL_RenderDrawRect(renderer, &inRect);
						SDL_RenderDrawLine(renderer, selRect.x, selRect.y+selRect.h-20, selRect.x+selRect.w, selRect.y+selRect.h-20);
						SDL_RenderPresent(renderer);
						Mix_PlayChannel(-1,okay,0);
						SDL_Delay(1000);
						SDL_SetRenderDrawColor(renderer, 0,0,0,0);
						return 3;
						break;
				}
			}
			
		}
	}	
}