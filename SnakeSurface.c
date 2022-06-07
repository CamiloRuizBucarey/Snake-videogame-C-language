#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>	
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 595;
const int MENU_WIDTH = 295;
const int MENU_HEIGHT = 600;
const int startX = 230;
const int startY = 230;
const int heightSnake = 15;
const int widthSnake = 15;
const int vel = 15;

typedef struct{
	int x, y;
	int w, h;
	char direccion;
}Cuerpo;

typedef struct {
	int largo;
	Cuerpo Cuerpo[2500];
}Head;

typedef struct {
	int x, y;
	int w, h;
}Food;

void createSnake(Head * snake){
	snake->largo = 4;
	for(int i = 0; i < (snake->largo); i++){
		snake->Cuerpo[i].direccion = 'w';
	}
	for(int i = 0; i < (snake->largo); i++){
		snake->Cuerpo[i].x = startX;
		snake->Cuerpo[i].y = startY + (heightSnake * i);
		snake->Cuerpo[i].w = widthSnake;
		snake->Cuerpo[i].h = heightSnake;
	}
	for(int i = (snake->largo); i < 2500; i++){
		snake->Cuerpo[i].x = 0;
		snake->Cuerpo[i].y = 595;
		snake->Cuerpo[i].w = widthSnake;
		snake->Cuerpo[i].h = heightSnake;
	}
}

void createFood(Food * food){
	do{
		food->x = ((rand() % 32) * 15) + 5;
		food->y = ((rand() % 39) * 15) + 5;
	}while(food->x == startX || food->y == startY);
	food->h = heightSnake;
	food->w = widthSnake;
}

void conectBody(Head * snake){
	for(int i = (snake->largo) - 1; i >= 1; i--){
		snake->Cuerpo[i].x = snake->Cuerpo[i-1].x;
		snake->Cuerpo[i].y = snake->Cuerpo[i-1].y;
		snake->Cuerpo[i].direccion = snake->Cuerpo[i-1].direccion; 
	}
}

void grow(Head * snake, Food * food, Mix_Chunk ** eating, int * score){
	bool diferent;
	if(food->x == snake->Cuerpo[0].x && food->y == snake->Cuerpo[0].y){
		Mix_PlayChannel(-1, *eating, 0);
		snake->largo++;
		*score = *score + 1;
		do{
			food->x = ((rand() % 32) * 15) + 5;
			food->y = ((rand() % 39) * 15) + 5;
			for(int i = 0; i < (snake->largo) - 1; i++){
				if(food->x == snake->Cuerpo[i].x && food->y == snake->Cuerpo[i].y){
					diferent = false;
					break;
				}
				diferent = true;
			}
		}while(diferent == false);
	}
}

void movement(Head *snake, unsigned char *wasd){
	if(wasd[SDL_SCANCODE_LEFT] && snake->Cuerpo[0].direccion != 'd'){
		(snake->Cuerpo[0].direccion) = 'a';
	}
	else if(wasd[SDL_SCANCODE_RIGHT] && snake->Cuerpo[0].direccion != 'a'){
		(snake->Cuerpo[0].direccion) = 'd';
	}
	else if(wasd[SDL_SCANCODE_UP] && snake->Cuerpo[0].direccion != 's'){
		(snake->Cuerpo[0].direccion) = 'w';
	}
	else if(wasd[SDL_SCANCODE_DOWN] && snake->Cuerpo[0].direccion != 'w'){
		(snake->Cuerpo[0].direccion) = 's';
	}
	switch(snake->Cuerpo[0].direccion){
		case 'a':
			(snake->Cuerpo[0]).x -= vel;
			break;
		case 'd':
			(snake->Cuerpo[0]).x += vel;
			break;
		case 'w':
			(snake->Cuerpo[0]).y -= vel;
			break;
		case 's':
			(snake->Cuerpo[0]).y += vel;
			break;
	}
}

void loadImages(SDL_Surface ** back, SDL_Surface ** head, SDL_Surface ** hor, SDL_Surface ** vert, SDL_Surface ** tail, 
	SDL_Surface ** elbowSI, SDL_Surface ** elbowSD, SDL_Surface ** elbowII, SDL_Surface ** elbowID, SDL_Surface ** border, 
	SDL_Surface ** food, SDL_Surface ** menuImage){
	*back = SDL_LoadBMP("bin/BackgroundBuild/Fondo.bmp");
	*border = SDL_LoadBMP("bin/BackgroundBuild/Borders.bmp");
	*hor = SDL_LoadBMP("bin/SnakeBuild/SnakeBodyH.bmp");
	*vert = SDL_LoadBMP("bin/SnakeBuild/SnakeBodyV.bmp");
	*elbowSI = SDL_LoadBMP("bin/SnakeBuild/SnakeBodyElbSI.bmp");
	*elbowSD = SDL_LoadBMP("bin/SnakeBuild/SnakeBodyElbSD.bmp");
	*elbowII = SDL_LoadBMP("bin/SnakeBuild/SnakeBodyElbII.bmp");
	*elbowID = SDL_LoadBMP("bin/SnakeBuild/SnakeBodyElbID.bmp");
	*tail = SDL_LoadBMP("bin/SnakeBuild/SnakeTailUp.bmp");
	*food = SDL_LoadBMP("bin/SnakeBuild/Food.bmp");
	*menuImage = SDL_LoadBMP("bin/BackgroundBuild/MenuImage.bmp");
}

void loadMusic(Mix_Music ** menu, Mix_Music ** music, Mix_Chunk ** eating, Mix_Chunk ** press, Mix_Chunk ** scroll, 
	Mix_Chunk ** death){
	*menu = Mix_LoadMUS("bin/Music/MenuMusic.mp3");	
	*music = Mix_LoadMUS("bin/Music/GameMusic.mp3");
	*eating = Mix_LoadWAV("bin/Music/Eating.wav");
	*press = Mix_LoadWAV("bin/Music/Press.wav");
	*scroll = Mix_LoadWAV("bin/Music/Scroll.wav");
	*death = Mix_LoadWAV("bin/Music/Death.wav");
}

void loadFonts(TTF_Font ** font, TTF_Font ** menuFont, SDL_Surface ** scoreText, SDL_Surface ** inicio,
 SDL_Surface ** INICIO, SDL_Surface ** facil, SDL_Surface ** FACIL, SDL_Surface ** normal, SDL_Surface ** NORMAL, 
 SDL_Surface ** dificil, SDL_Surface ** DIFICIL, SDL_Surface ** frenesi, SDL_Surface ** FRENESI, SDL_Surface ** musica_on, 
 SDL_Surface ** MUSICA_ON, SDL_Surface ** musica_off, SDL_Surface ** MUSICA_OFF, SDL_Surface ** salir, SDL_Surface ** SALIR,
 SDL_Surface ** rankText){
	SDL_Color color = {4, 30, 8}; 
	*font = TTF_OpenFont("bin/Fonts/ScoreFont.ttf", 40);
	*menuFont = TTF_OpenFont("bin/Fonts/MenuFont.ttf", 30);
	*inicio = TTF_RenderText_Solid(*menuFont, "inicio", color);
	*INICIO = TTF_RenderText_Solid(*menuFont, "INICIO", color);
	*facil = TTF_RenderText_Solid(*menuFont, "facil", color);
	*FACIL = TTF_RenderText_Solid(*menuFont, "FACIL", color);
	*normal = TTF_RenderText_Solid(*menuFont, "normal", color);
	*NORMAL = TTF_RenderText_Solid(*menuFont, "NORMAL", color);
	*dificil = TTF_RenderText_Solid(*menuFont, "dificil", color);
	*DIFICIL = TTF_RenderText_Solid(*menuFont, "DIFICIL", color);
	*frenesi = TTF_RenderText_Solid(*menuFont, "frenesi", color);
	*FRENESI = TTF_RenderText_Solid(*menuFont, "FRENESI", color);
	*musica_on = TTF_RenderText_Solid(*menuFont, "sonido on", color);
	*MUSICA_ON = TTF_RenderText_Solid(*menuFont, "SONIDO ON", color);
	*musica_off = TTF_RenderText_Solid(*menuFont, "sonido off", color);
	*MUSICA_OFF = TTF_RenderText_Solid(*menuFont, "SONIDO OFF", color);
	*salir = TTF_RenderText_Solid(*menuFont, "salir", color);
	*SALIR = TTF_RenderText_Solid(*menuFont, "SALIR", color);
	*scoreText = TTF_RenderText_Solid(*font, "Puntaje:", color);
	*rankText = TTF_RenderText_Solid(*font, "Record:", color);
}

void pauseGame(bool * stop, unsigned char * keys){

	if(keys[SDL_SCANCODE_P] && *stop == false){		//Pause game if letter "p" is pressed
		*stop = true;
	}
	else if(keys[SDL_SCANCODE_P] && *stop == true){
		*stop = false;
	}
}

bool colision(Head * snake, bool * victory, int * score, bool * pause, Mix_Chunk ** death, int * record, char * diffPos,
	FILE ** scoreFile, int * recordE, int * recordN, int * recordH, int * recordF){
	if(snake->Cuerpo[0].x < 5 || snake->Cuerpo[0].x > SCREEN_WIDTH - MENU_WIDTH - widthSnake - 5 
		|| snake->Cuerpo[0].y < 5 || snake->Cuerpo[0].y > SCREEN_HEIGHT - heightSnake - 5){
		Mix_PlayChannel(-1, *death, 0);
		*scoreFile = fopen("bin/Ranking", "w+");
		if(*score > *record){
			switch(*diffPos){
				case 1:
					fprintf(*scoreFile, "%d %d %d %d", *score, *recordN, *recordH, *recordF);
					break;
				case 2:
					fprintf(*scoreFile, "%d %d %d %d", *recordE, *score, *recordH, *recordF);
					break;
				case 3:
					fprintf(*scoreFile, "%d %d %d %d", *recordE, *recordN, *score, *recordF);
					break;
				case 4:
					fprintf(*scoreFile, "%d %d %d %d", *recordE, *recordN, *recordH, *score);
					break;
			}
		}
		else{
			fprintf(*scoreFile, "%d %d %d %d", *recordE, *recordN, *recordH, *recordF);
		}
		fclose(*scoreFile);
		scanRanking(scoreFile, recordE, recordN, recordH, recordF);
		*victory = false;
		createSnake(snake);
		*score = 0;
		*pause = true;
		return false;
	}
	else{
		for(int i = (snake->largo) - 1; i >= 1; i--){
			if(snake->Cuerpo[0].x == snake->Cuerpo[i].x && snake->Cuerpo[0].y == snake->Cuerpo[i].y){
				Mix_PlayChannel(-1, *death, 0);
				*scoreFile = fopen("bin/Ranking", "w+");
				if(*score > *record){
					switch(*diffPos){
						case 1:
							fprintf(*scoreFile, "%d %d %d %d", *score, *recordN, *recordH, *recordF);
							break;
						case 2:
							fprintf(*scoreFile, "%d %d %d %d", *recordE, *score, *recordH, *recordF);
							break;
						case 3:
							fprintf(*scoreFile, "%d %d %d %d", *recordE, *recordN, *score, *recordF);
							break;
						case 4:
							fprintf(*scoreFile, "%d %d %d %d", *recordE, *recordN, *recordH, *score);
							break;
					}
				}
				else{
					fprintf(*scoreFile, "%d %d %d %d", *recordE, *recordN, *recordH, *recordF);
				}
				fclose(*scoreFile);
				scanRanking(scoreFile, recordE, recordN, recordH, recordF);
				*victory = false;
				createSnake(snake);
				*score = 0;
				*pause = true;
				return false;
			}
		}
	}
	return true;
}

void start(SDL_Event * event, bool * pause, unsigned char * wasd, bool * started){
	if(event->type == SDL_KEYDOWN && *started == false){
		if(wasd[SDL_SCANCODE_LEFT] || wasd[SDL_SCANCODE_RIGHT] || wasd[SDL_SCANCODE_UP] || wasd[SDL_SCANCODE_DOWN]){
			*pause = false;
		}
	}
}

void drawGame(Head * snake, SDL_Surface ** surface, SDL_Surface ** back, SDL_Surface ** head, SDL_Surface ** bodyHorImage, 
	SDL_Surface ** bodyVertImage, SDL_Surface ** bodyElbSIImage, SDL_Surface ** bodyElbSDImage, SDL_Surface ** bodyElbIIImage, 
	SDL_Surface ** bodyElbIDImage, SDL_Surface ** tail, SDL_Surface ** border){
	SDL_BlitSurface(*back, NULL, *surface, NULL);
	SDL_BlitSurface(*border, NULL, *surface, NULL);
	for(int i = 0; i < snake->largo; i++){
		if (snake->Cuerpo[i-1].direccion == snake->Cuerpo[i].direccion){
			if(snake->Cuerpo[i].direccion == 'w' || snake->Cuerpo[i].direccion == 's'){
				SDL_BlitSurface(*bodyVertImage, NULL, *surface, &snake->Cuerpo[i]);
			}
			else{
				SDL_BlitSurface(*bodyHorImage, NULL, *surface, &snake->Cuerpo[i]);
			}
		}
		else if((snake->Cuerpo[i].direccion == 'w' && snake->Cuerpo[i-1].direccion == 'a') 
			|| (snake->Cuerpo[i].direccion == 'd' && snake->Cuerpo[i-1].direccion == 's')){
			SDL_BlitSurface(*bodyElbSDImage, NULL, *surface, &snake->Cuerpo[i]);
		}
		else if((snake->Cuerpo[i].direccion == 'w' && snake->Cuerpo[i-1].direccion == 'd') 
			|| (snake->Cuerpo[i].direccion == 'a' && snake->Cuerpo[i-1].direccion == 's')){
			SDL_BlitSurface(*bodyElbSIImage, NULL, *surface, &snake->Cuerpo[i]);
		}
		else if((snake->Cuerpo[i].direccion == 's' && snake->Cuerpo[i-1].direccion == 'd') 
			|| (snake->Cuerpo[i].direccion == 'a' && snake->Cuerpo[i-1].direccion == 'w')){
			SDL_BlitSurface(*bodyElbIIImage, NULL, *surface, &snake->Cuerpo[i]);
		}
		else if((snake->Cuerpo[i].direccion == 'd' && snake->Cuerpo[i-1].direccion == 'w') 
			|| (snake->Cuerpo[i].direccion == 's' && snake->Cuerpo[i-1].direccion == 'a')){
			SDL_BlitSurface(*bodyElbIDImage, NULL, *surface, &snake->Cuerpo[i]);
		}
	}
	if(snake->Cuerpo[0].direccion == 'w'){
		*head = SDL_LoadBMP("bin/SnakeBuild/SnakeHeadUp.bmp");
		SDL_BlitSurface(*head, NULL, *surface, &snake->Cuerpo[0]);
	}
	else if(snake->Cuerpo[0].direccion == 's'){
		*head = SDL_LoadBMP("bin/SnakeBuild/SnakeHeadDown.bmp");
		SDL_BlitSurface(*head, NULL, *surface, &snake->Cuerpo[0]);
	}
	else if(snake->Cuerpo[0].direccion == 'a'){
		*head = SDL_LoadBMP("bin/SnakeBuild/SnakeHeadLeft.bmp");
		SDL_BlitSurface(*head, NULL, *surface, &snake->Cuerpo[0]);
	}
	else if(snake->Cuerpo[0].direccion == 'd'){
		*head = SDL_LoadBMP("bin/SnakeBuild/SnakeHeadRight.bmp");
		SDL_BlitSurface(*head, NULL, *surface, &snake->Cuerpo[0]);
	}
	if(snake->Cuerpo[(snake->largo)-2].direccion == 'w'){
		*tail = SDL_LoadBMP("bin/SnakeBuild/SnakeTailUp.bmp");
		SDL_BlitSurface(*tail, NULL, *surface, &snake->Cuerpo[(snake->largo)-1]);
	}
	else if(snake->Cuerpo[(snake->largo)-2].direccion == 's'){
		*tail = SDL_LoadBMP("bin/SnakeBuild/SnakeTailDown.bmp");
		SDL_BlitSurface(*tail, NULL, *surface, &snake->Cuerpo[(snake->largo)-1]);
	}
	else if(snake->Cuerpo[(snake->largo)-2].direccion == 'a'){
		*tail = SDL_LoadBMP("bin/SnakeBuild/SnakeTailLeft.bmp");
		SDL_BlitSurface(*tail, NULL, *surface, &snake->Cuerpo[(snake->largo)-1]);
	}
	else if(snake->Cuerpo[(snake->largo)-2].direccion == 'd'){
		*tail = SDL_LoadBMP("bin/SnakeBuild/SnakeTailRight.bmp");
		SDL_BlitSurface(*tail, NULL, *surface, &snake->Cuerpo[(snake->largo)-1]);
	}
}

void setPositions(SDL_Rect * score, SDL_Rect * scoring, SDL_Rect * rankPos, SDL_Rect * rankTxtPos){
	score->x = 530;
	score->y = 50;
	scoring->x = 680;
	scoring->y = 50;
	rankPos->x = 680;
	rankPos->y = 100;
	rankTxtPos->x = 530;
	rankTxtPos->y = 100;
}

void scoreUP(SDL_Surface ** scoreNumbers, TTF_Font * font, int * score, int * record, SDL_Surface ** rankNumbers){
	SDL_Color color = {4, 30, 8};
	char scoreCH[5];
	char rankCH[5];
	sprintf(scoreCH, "%04d\0", *score);
	sprintf(rankCH, "%04d\0", *record);
	*scoreNumbers = TTF_RenderText_Solid(font, scoreCH, color);
	*rankNumbers = TTF_RenderText_Solid(font, rankCH, color);
}

void selectButtons(bool * frenetic, int * diff, bool * run, bool * playing, SDL_Event * event, unsigned char * wasd, 
	 char * buttonPos,char * diffPos,bool * sound,SDL_Surface ** jugar,SDL_Surface ** dificultad ,
	 SDL_Surface ** sonido,SDL_Surface ** exit,SDL_Surface ** inicio,SDL_Surface ** INICIO,SDL_Surface ** facil,
	 SDL_Surface ** FACIL,SDL_Surface ** normal,SDL_Surface ** NORMAL,SDL_Surface ** dificil,SDL_Surface ** DIFICIL,
	 SDL_Surface ** frenesi,SDL_Surface ** FRENESI,SDL_Surface ** musica_on,SDL_Surface ** MUSICA_ON,
	 SDL_Surface ** musica_off,SDL_Surface ** MUSICA_OFF,SDL_Surface ** salir,SDL_Surface ** SALIR, Mix_Chunk ** scroll, 
	 Mix_Chunk ** press, int * record, int * recordE, int * recordN, int * recordH, int * recordF){
	if(event->type == SDL_KEYDOWN){
		if(wasd[SDL_SCANCODE_UP] && *buttonPos != 1){
			Mix_PlayChannel(-1, *scroll, 0);
			*buttonPos=*buttonPos - 1;
		}
		else if(wasd[SDL_SCANCODE_DOWN] && *buttonPos != 4){
			Mix_PlayChannel(-1, *scroll, 0);
			*buttonPos=*buttonPos + 1;
		}
		else if(wasd[SDL_SCANCODE_RETURN] && *buttonPos == 1 ){
			Mix_PlayChannel(-1, *press, 0);
			*playing = true;
		}
		else if(wasd[SDL_SCANCODE_RETURN] && *buttonPos == 2 ){
			Mix_PlayChannel(-1, *press, 0);
			*diffPos = *diffPos + 1;
			if(*diffPos == 5){
				*diffPos = 1;
			}
			switch(*diffPos){
				case 1:
					*diff = 70;
					*frenetic = false;
					break;
				case 2:
					*diff = 50;
					*frenetic = false;
					break;
				case 3:
					*diff = 25;
					*frenetic = false;
					break;
				case 4:
					*diff = 50;
					*frenetic = true;
					break;
			}
		}
		else if(wasd[SDL_SCANCODE_RETURN] && *buttonPos == 3 ){
			Mix_PlayChannel(-1, *press, 0);
			if(*sound == true){
				*sound = false;
				Mix_Volume(-1, 0);
				Mix_VolumeMusic(0);
			}
			else{
				*sound = true;
				Mix_Volume(-1, 128);
				Mix_VolumeMusic(128);
			}
		}
		else if(wasd[SDL_SCANCODE_RETURN] && *buttonPos == 4 ){
			Mix_PlayChannel(-1, *press, 0);
			*run = false;
		}
	}
	if(*buttonPos == 1){
		*jugar = *inicio;
		*exit = *SALIR;
		switch(*diffPos){
			case 1:
				*dificultad = *FACIL;
				break;
			case 2:
				*dificultad = *NORMAL;
				break;
			case 3:
				*dificultad = *DIFICIL;
				break;
			case 4:
				*dificultad = *FRENESI;
				break;
		}
		if(*sound == true){
			*sonido = *MUSICA_ON;
		}
		else{
			*sonido = *MUSICA_OFF;
		}
	}
	else if(*buttonPos == 2){
		*jugar = *INICIO;
		*exit = *SALIR;
		switch(*diffPos){
			case 1:
				*dificultad = *facil;
				break;
			case 2:
				*dificultad = *normal;
				break;
			case 3:
				*dificultad = *dificil;
				break;
			case 4:
				*dificultad = *frenesi;
				break;
		}
		if(*sound == true){
			*sonido = *MUSICA_ON;
		}
		else{
			*sonido = *MUSICA_OFF;
		}
	}
	else if(*buttonPos == 3){
		*jugar = *INICIO;
		*exit = *SALIR;
		switch(*diffPos){
			case 1:
				*dificultad = *FACIL;
				break;
			case 2:
				*dificultad = *NORMAL;
				break;
			case 3:
				*dificultad = *DIFICIL;
				break;
			case 4:
				*dificultad = *FRENESI;
				break;
		}
		if(*sound == true){
			*sonido = *musica_on;
		}
		else{
			*sonido = *musica_off;
		}
	}
	else if(*buttonPos == 4){
		*jugar = *INICIO;
		*exit = *salir;
		switch(*diffPos){
			case 1:
				*dificultad = *FACIL;
				break;
			case 2:
				*dificultad = *NORMAL;
				break;
			case 3:
				*dificultad = *DIFICIL;
				break;
			case 4:
				*dificultad = *FRENESI;
				break;
		}
		if(*sound == true){
			*sonido = *MUSICA_ON;
		}
		else{
			*sonido = *MUSICA_OFF;
		}
	}
	SDL_Delay(25);
}

void drawMenu(SDL_Surface ** surface, SDL_Surface ** interface, SDL_Surface ** menuImage, SDL_Surface ** jugar, 
	SDL_Surface ** dificultad, SDL_Surface ** sonido, SDL_Surface ** salir){
	SDL_Rect logo;
	logo.x = 170;
	logo.y = 100;
	SDL_Rect start;
	start.x = 200;
	start.y = 400;
	SDL_Rect dif;
	dif.x = 200;
	dif.y = 440;
	SDL_Rect sound;
	sound.x = 200;
	sound.y = 480;
	SDL_Rect exit;
	exit.x = 200;
	exit.y = 520;
	SDL_BlitSurface(*interface, NULL, *surface, NULL);
	SDL_BlitSurface(*menuImage, NULL, *surface, &logo);
	SDL_BlitSurface(*jugar, NULL, *surface, &start);
	SDL_BlitSurface(*dificultad, NULL, *surface, &dif);
	SDL_BlitSurface(*sonido, NULL, *surface, &sound);
	SDL_BlitSurface(*salir, NULL, *surface, &exit);
}

void blitCreate(SDL_Surface ** surface, SDL_Surface ** foodSurface, SDL_Surface ** scoreText, SDL_Surface ** scoreNumbers, 
	SDL_Surface ** rankText, SDL_Surface ** rankNumbers, SDL_Rect * Food, SDL_Rect * scorePosition, SDL_Rect * scoring, 
	SDL_Rect * rankTxtPos, SDL_Rect * rankPos){
	SDL_BlitSurface(*foodSurface, NULL, *surface, Food);
	SDL_BlitSurface(*scoreText, NULL, *surface, scorePosition);
	SDL_BlitSurface(*scoreNumbers, NULL, *surface, scoring);
	SDL_BlitSurface(*rankText, NULL, *surface, rankTxtPos);
	SDL_BlitSurface(*rankNumbers, NULL, *surface, rankPos);

}

void scanRanking(FILE ** scoreFile, int * recordE, int * recordN, int * recordH, int * recordF){
	*scoreFile = fopen("bin/Ranking", "r");
	fscanf(*scoreFile, "%d", recordE);
	fscanf(*scoreFile, "%d", recordN);
	fscanf(*scoreFile, "%d", recordH);
	fscanf(*scoreFile, "%d", recordF);
	fclose(*scoreFile);
}

void setRecord(char * diffPos, int * record, int * recordE, int * recordN, int * recordH, int * recordF){
	switch(*diffPos){
		case 1:
			*record = *recordE;
			break;
		case 2:
			*record = *recordN;
			break;
		case 3:
			*record = *recordH;
			break;
		case 4:
			*record = *recordF;
			break;
	}
}

int main(){
	srand(time(NULL));			//Set a new random seed
	bool pause = true;
	bool run = true;
	bool victory = NULL;
	bool started = false;
	bool playing = false;
	bool sound = true; 
	bool frenetic = false;
	int score = 0;
	int record;
	int diff = 50;
	int extra = 0;
	int recordE;
	int recordN;
	int recordH;
	int recordF;
	const unsigned char *key;
	Head Snake;
	createSnake(&Snake);
	Food Food;
	createFood(&Food);
	char buttonPos = 1;
	char diffPos = 2;
	FILE * scoreFile = NULL;
	SDL_Window* window = NULL;
	SDL_Event event;
	Mix_Music * menuMusic = NULL;
	Mix_Music * gameMusic = NULL;
	Mix_Chunk * eating = NULL;
	Mix_Chunk * press = NULL;
	Mix_Chunk * scroll = NULL;
	Mix_Chunk * death = NULL;
	SDL_Rect scorePosition;
	SDL_Rect scoring;
	SDL_Rect rankTxtPos;
	SDL_Rect rankPos;
	TTF_Font * scoreFont = NULL;
	TTF_Font * menuFont = NULL;
	SDL_Surface * foodSurface = NULL;
	SDL_Surface * headImage = NULL;
	SDL_Surface * bodyHorImage = NULL;
	SDL_Surface * bodyVertImage = NULL;
	SDL_Surface * bodyElbSIImage = NULL;
	SDL_Surface * bodyElbSDImage = NULL;
	SDL_Surface * bodyElbIIImage = NULL;
	SDL_Surface * bodyElbIDImage = NULL;
	SDL_Surface * tailImage = NULL;
	SDL_Surface * interface = NULL;
	SDL_Surface * borders = NULL;
	SDL_Surface * difficultButton = NULL;
	SDL_Surface * menuImage = NULL;
	SDL_Surface * surface = NULL;
	SDL_Surface * effect = NULL;
	SDL_Surface * scoreText = NULL;
	SDL_Surface * scoreNumbers = NULL;
	SDL_Surface * rankText = NULL;
	SDL_Surface * rankNumbers = NULL;
	SDL_Surface * inicio = NULL;
	SDL_Surface * INICIO = NULL;
	SDL_Surface * facil = NULL;
	SDL_Surface * FACIL = NULL;
	SDL_Surface * normal = NULL;
	SDL_Surface * NORMAL = NULL;
	SDL_Surface * dificil = NULL;
	SDL_Surface * DIFICIL = NULL;
	SDL_Surface * frenesi = NULL;
	SDL_Surface * FRENESI = NULL;
	SDL_Surface * musica_on = NULL;
	SDL_Surface * MUSICA_ON = NULL;
	SDL_Surface * musica_off = NULL;
	SDL_Surface * MUSICA_OFF = NULL;
	SDL_Surface * salir = NULL;
	SDL_Surface * SALIR = NULL;
	SDL_Surface * jugar = NULL;
	SDL_Surface * dificultad = NULL;
	SDL_Surface * sonido = NULL;
	SDL_Surface * exit = NULL;
	SDL_Surface * icon = NULL;

	TTF_Init();														//Init. TTF library
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);				//Init. mixer library
	int flags = IMG_INIT_PNG | IMG_INIT_JPG;
	effect = IMG_Load("bin/BackgroundBuild/fx.png");
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){				//Init. video and audio systems
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else{
		window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);		//Create the main window
		if(window == NULL){
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else{
			icon = SDL_LoadBMP("bin/Snake2Icon.bmp");
			SDL_SetWindowIcon(window, icon);
			key = SDL_GetKeyboardState(NULL);
			surface = SDL_GetWindowSurface(window);			//Create the surface of the game
			setPositions(&scorePosition, &scoring, &rankPos, &rankTxtPos);
			scanRanking(&scoreFile, &recordE, &recordN, &recordH, &recordF);
			loadImages(&interface, &headImage, &bodyHorImage, &bodyVertImage, &tailImage, &bodyElbSIImage, &bodyElbSDImage, &bodyElbIIImage, &bodyElbIDImage, &borders, &foodSurface, &menuImage);
			loadMusic(&menuMusic, &gameMusic, &eating, &press, &scroll, &death);
			loadFonts(&scoreFont, &menuFont, &scoreText, &inicio, &INICIO, &facil, &FACIL, &normal, &NORMAL, &dificil, &DIFICIL, &frenesi, &FRENESI, &musica_on, &MUSICA_ON, &musica_off, &MUSICA_OFF, &salir, &SALIR, &rankText);
			while(run){
				while(SDL_PollEvent(&event)){
					if(event.type == SDL_QUIT){				//Close window with "x" button
						run = false;
					}
				}
				if(playing){
					while(SDL_PollEvent(&event)){
						if(event.type == SDL_KEYDOWN){
							pauseGame(&pause, key);				//Pause game
						}
					}
					grow(&Snake, &Food, &eating, &score);
					start(&event, &pause, key, &started);
					drawGame(&Snake, &surface, &interface, &headImage, &bodyHorImage, &bodyVertImage, &bodyElbSIImage, &bodyElbSDImage, &bodyElbIIImage, &bodyElbIDImage, &tailImage, &borders);
					if(!pause){
						conectBody(&Snake);
						movement(&Snake, key);
						if(Snake.Cuerpo[0].direccion != NULL){
							playing = colision(&Snake, &victory, &score, &pause, &death, &record, &diffPos, &scoreFile, &recordE, &recordN, &recordH, &recordF);
						}
					}
					scoreUP(&scoreNumbers, scoreFont, &score, &record, &rankNumbers);
					blitCreate(&surface, &foodSurface, &scoreText, &scoreNumbers, &rankText, &rankNumbers, &Food, &scorePosition, &scoring, &rankTxtPos, &rankPos);
					if(!playing){
						Mix_HaltMusic();
						Mix_FreeMusic(gameMusic);
						menuMusic = Mix_LoadMUS("bin/Music/MenuMusic.mp3");
						Mix_PlayMusic(menuMusic, -1);
					}
				}
				else{
					if(Mix_PlayingMusic() == 0){
						Mix_PlayMusic(menuMusic, -1);
					}
					selectButtons(&frenetic, &diff, &run, &playing, &event, key, &buttonPos, &diffPos, &sound, &jugar,
					 &dificultad, &sonido, &exit, &inicio, &INICIO, &facil, &FACIL, &normal, &NORMAL, &dificil, &DIFICIL, 
					 &frenesi, &FRENESI, &musica_on, &MUSICA_ON, &musica_off, &MUSICA_OFF, &salir, &SALIR, &scroll, &press,
					 &record, &recordE, &recordN, &recordH, &recordF);
					drawMenu(&surface, &interface, &menuImage, &jugar, &dificultad, &sonido, &exit);
					if(playing){
						Mix_HaltMusic();
						Mix_FreeMusic(menuMusic);
						gameMusic = Mix_LoadMUS("bin/Music/GameMusic.mp3");
						Mix_PlayMusic(gameMusic, -1);
						setRecord(&diffPos, &record, &recordE, &recordN, &recordH, &recordF);
					}
				}
				SDL_BlitSurface(effect, NULL, surface, NULL);
				SDL_UpdateWindowSurface(window);
				if(!frenetic){
					SDL_Delay(diff);
				}
				else{
					extra = score;
					if(extra > 45){
						extra = 45;
					}
					SDL_Delay(diff - extra);
				}
			}
		}
	}
	fclose(scoreFile);
	Mix_FreeMusic(menuMusic);
	Mix_FreeMusic(gameMusic);
	Mix_FreeChunk(eating);
	Mix_FreeChunk(press);
	Mix_FreeChunk(scroll);
	Mix_FreeChunk(death);
	TTF_CloseFont(scoreFont);
	TTF_CloseFont(menuFont);
	SDL_FreeSurface(foodSurface);
	SDL_FreeSurface(headImage);
	SDL_FreeSurface(bodyHorImage);
	SDL_FreeSurface(bodyVertImage);
	SDL_FreeSurface(bodyElbSIImage);
	SDL_FreeSurface(bodyElbSDImage);
	SDL_FreeSurface(bodyElbIIImage);
	SDL_FreeSurface(bodyElbIDImage);
	SDL_FreeSurface(tailImage);
	SDL_FreeSurface(interface);
	SDL_FreeSurface(borders);
	SDL_FreeSurface(difficultButton);
	SDL_FreeSurface(menuImage);
	SDL_FreeSurface(effect);
	SDL_FreeSurface(scoreText);
	SDL_FreeSurface(scoreNumbers);
	SDL_FreeSurface(inicio);
	SDL_FreeSurface(INICIO);
	SDL_FreeSurface(facil);
	SDL_FreeSurface(FACIL);
	SDL_FreeSurface(normal);
	SDL_FreeSurface(NORMAL);
	SDL_FreeSurface(dificil);
	SDL_FreeSurface(DIFICIL);
	SDL_FreeSurface(frenesi);
	SDL_FreeSurface(FRENESI);
	SDL_FreeSurface(musica_on);
	SDL_FreeSurface(MUSICA_ON);
	SDL_FreeSurface(musica_off);
	SDL_FreeSurface(MUSICA_OFF);
	SDL_FreeSurface(salir);
	SDL_FreeSurface(SALIR);
	SDL_FreeSurface(jugar);
	SDL_FreeSurface(dificultad);
	SDL_FreeSurface(sonido);
	SDL_FreeSurface(exit);
	SDL_FreeSurface(icon);
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}