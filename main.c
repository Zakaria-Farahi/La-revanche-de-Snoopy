#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include<unistd.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct {
	int x;
	int y;
} obstacle;

struct ThreadTime {
	int* running;
	int* time;
	SDL_Surface *fenetre;
};

void youLose(int lvl, SDL_Surface *fenetre);
void* timeCalc(void* data);
int remove_element(obstacle *array, int index, size_t *array_length);
bool checkCollision(int newX, int newY, obstacle obstacles[], size_t obsSize);
void youWin(int lvl, SDL_Surface *fenetre);
void checkWin(int lvl, obstacle bones[], size_t *BoneSize, SDL_Surface *fenetre, int currentX, int currentY, int *running);
void bonesPos(int lvl, obstacle bones[], size_t BoneSize, SDL_Surface *fenetre);

int main(int argc, char const *argv[]) {
	if(SDL_Init(SDL_INIT_VIDEO)){
		fprintf(stderr, "Error SDL : %s\n", SDL_GetError());
		exit(0);
	}
	// code here
	//cree surface
	SDL_Surface *fenetre=NULL;
	fenetre=SDL_SetVideoMode(500,450, 32, SDL_HWSURFACE);
	//remplir la surface
	SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 126,217,87));// null = entierement
	SDL_Flip(fenetre);//mettre a jour la fenetre
	SDL_Surface* background = IMG_Load("./img/lvl1bg.png");
	SDL_BlitSurface(background, NULL, fenetre, NULL);
	SDL_Flip(fenetre);//mettre a jour la fenetre
    int running = 1;  // Variable indicating whether the main loop is running
    int time = 10;   // Initial time value (in seconds)
    struct ThreadTime threadData;  // Structure to pass data to the thread
    pthread_t timeThread;  // Variable for the time calculation thread

    // Initialize the thread data
    threadData.running = &running;
    threadData.time = &time;
    threadData.fenetre = fenetre;

    // Create the time calculation thread
    if (pthread_create(&timeThread, NULL, timeCalc, (void*)&threadData) != 0) {
        perror("Error creating time calculation thread");
        exit(EXIT_FAILURE);
    }
	obstacle obstacles[4] = {{100, 150}, {350, 150}, {100, 300}, {350, 300}};
	obstacle bones[] = {{450, 50}, {450, 400}, {0, 400}};
	size_t obsSize = sizeof(obstacles)/sizeof(obstacles[0]);
	size_t bonesSize = sizeof(bones)/sizeof(bones[0]);


	// Cree Personnage
	SDL_Surface *pers;
	pers = IMG_Load("./img/1r.png");
	SDL_Rect PersPos;
	PersPos.x = 200;
	PersPos.y = 200;
	SDL_BlitSurface(pers, NULL, fenetre, &PersPos);
	bonesPos(1, bones, bonesSize, fenetre);
	SDL_Flip(fenetre);
	//boucle "pseudo" infini
	Uint32 startTime = SDL_GetTicks();
	int moveSpeed = 25;  // Adjust the speed of movement
	int moveDistance = 50;
	while (1) {
    	SDL_Event event;
    	SDL_WaitEvent(&event);
	    if (event.type == SDL_QUIT) {
	        break;
	    } else if (event.type == SDL_KEYDOWN) {
	        if (event.key.keysym.sym == SDLK_SPACE) {
            break;
	        }
	        if (event.key.keysym.sym == SDLK_RIGHT) {
	            if(PersPos.x < 450 && checkCollision(PersPos.x + moveDistance, PersPos.y, obstacles, obsSize)) {
	            	int tempPos = PersPos.x;
	            	while(PersPos.x < tempPos + 50){
		                pers = IMG_Load("./img/2r.png");
		                SDL_BlitSurface(background, NULL, fenetre, NULL);
		                PersPos.x += moveSpeed;
		                SDL_BlitSurface(pers, NULL, fenetre, &PersPos);
						checkWin(1, bones, &bonesSize, fenetre, PersPos.x, PersPos.y, &running);
		                bonesPos(1, bones, bonesSize, fenetre);
		                SDL_Flip(fenetre);
		                SDL_Delay(25);  // Adjust the delay to control animation speed
		                startTime = SDL_GetTicks();

		                pers = IMG_Load("./img/1r.png");
		                SDL_BlitSurface(background, NULL, fenetre, NULL);
		                PersPos.x += moveSpeed;
		                SDL_BlitSurface(pers, NULL, fenetre, &PersPos);
		                checkWin(1, bones, &bonesSize, fenetre, PersPos.x, PersPos.y, &running);
		                bonesPos(1, bones, bonesSize, fenetre);
		                SDL_Flip(fenetre);
		                SDL_Delay(25);  // Adjust the delay to control animation speed
		                startTime = SDL_GetTicks();
		            }
	            }
	        }
	        if (event.key.keysym.sym == SDLK_LEFT) {
	            if(PersPos.x > 0 && checkCollision(PersPos.x - moveDistance, PersPos.y, obstacles, obsSize)) {
	            	int tempPos = PersPos.x;
	            	while(PersPos.x > tempPos - 50){
		                pers = IMG_Load("./img/2l.png");
		                SDL_BlitSurface(background, NULL, fenetre, NULL);
		                PersPos.x -= moveSpeed;
		                SDL_BlitSurface(pers, NULL, fenetre, &PersPos);
		                checkWin(1, bones, &bonesSize, fenetre, PersPos.x, PersPos.y, &running);
		                bonesPos(1, bones, bonesSize, fenetre);
		                SDL_Flip(fenetre);
		                SDL_Delay(50);  // Adjust the delay to control animation speed
		                startTime = SDL_GetTicks();

		                pers = IMG_Load("./img/1l.png");
		                SDL_BlitSurface(background, NULL, fenetre, NULL);
		                PersPos.x -= moveSpeed;
		                SDL_BlitSurface(pers, NULL, fenetre, &PersPos);
		                checkWin(1, bones, &bonesSize, fenetre, PersPos.x, PersPos.y, &running);
		                bonesPos(1, bones, bonesSize, fenetre);
		                SDL_Flip(fenetre);
		                SDL_Delay(50);  // Adjust the delay to control animation speed
		                startTime = SDL_GetTicks();
		            }
	            }
	        }
	        if (event.key.keysym.sym == SDLK_UP) {
	            if(PersPos.y > 50 && checkCollision(PersPos.x, PersPos.y - moveDistance, obstacles, obsSize)) {
	            	int tempPos = PersPos.y;
	            	while(PersPos.y > tempPos - 50){
		                pers = IMG_Load("./img/2l.png");
		                SDL_BlitSurface(background, NULL, fenetre, NULL);
		                PersPos.y -= moveSpeed;
		                SDL_BlitSurface(pers, NULL, fenetre, &PersPos);
		                checkWin(1, bones, &bonesSize, fenetre, PersPos.x, PersPos.y, &running);
		                bonesPos(1, bones, bonesSize, fenetre);
		                SDL_Flip(fenetre);
		                SDL_Delay(50);  // Adjust the delay to control animation speed
		                startTime = SDL_GetTicks();

		                pers = IMG_Load("./img/1l.png");
		                SDL_BlitSurface(background, NULL, fenetre, NULL);
		                PersPos.y -= moveSpeed;
		                SDL_BlitSurface(pers, NULL, fenetre, &PersPos);
		                checkWin(1, bones, &bonesSize, fenetre, PersPos.x, PersPos.y, &running);
		                bonesPos(1, bones, bonesSize, fenetre);
		                SDL_Flip(fenetre);
		                SDL_Delay(50);  // Adjust the delay to control animation speed
		                startTime = SDL_GetTicks();
		            }
	            }
	        }
	        if (event.key.keysym.sym == SDLK_DOWN) {
	            if(PersPos.y < 400 && checkCollision(PersPos.x, PersPos.y + moveDistance, obstacles, obsSize)) {
	            	int tempPos = PersPos.y;
	            	while(PersPos.y < tempPos + 50){
		                pers = IMG_Load("./img/2r.png");
		                SDL_BlitSurface(background, NULL, fenetre, NULL);
		                PersPos.y += moveSpeed;
		                SDL_BlitSurface(pers, NULL, fenetre, &PersPos);
		                checkWin(1, bones, &bonesSize, fenetre, PersPos.x, PersPos.y, &running);
		                bonesPos(1, bones, bonesSize, fenetre);
		                SDL_Flip(fenetre);
		                SDL_Delay(50);  // Adjust the delay to control animation speed
		                startTime = SDL_GetTicks();

		                pers = IMG_Load("./img/1r.png");
		                SDL_BlitSurface(background, NULL, fenetre, NULL);
		                PersPos.y += moveSpeed;
		                SDL_BlitSurface(pers, NULL, fenetre, &PersPos);
		                checkWin(1, bones, &bonesSize, fenetre, PersPos.x, PersPos.y, &running);
		                bonesPos(1, bones, bonesSize, fenetre);
		                SDL_Flip(fenetre);
		                SDL_Delay(50);  // Adjust the delay to control animation speed
		                startTime = SDL_GetTicks();
		            }
	            }
	        }
    }
}
Quit:
	running = 0;
	pthread_join(timeThread, NULL);
	//liberer la memoire
	SDL_FreeSurface(background);
	SDL_FreeSurface(pers);
	SDL_FreeSurface(fenetre);
	SDL_Quit(); //atexit(SDL_Quit);
	return 0;
}


void youLose(int lvl, SDL_Surface *fenetre){
		SDL_Surface* background = IMG_Load("./img/lose.png");
		SDL_BlitSurface(background, NULL, fenetre, NULL);
		SDL_Delay(200);
		int i= 1000;
		while(i){
			SDL_BlitSurface(background, NULL, fenetre, NULL);
			SDL_Flip(fenetre);//mettre a jour la fenetre
			i--;
			SDL_Delay(1);
		}
		exit(0);
}

void* timeCalc(void* data){
	struct ThreadTime* Time = (struct ThreadTime*)data;
	SDL_Surface *paw;
	paw = IMG_Load("./img/paw.png");
	SDL_Rect pawPos;
	pawPos.x = 0;
	pawPos.y = 0;
	SDL_Surface *carrVert;
	carrVert = SDL_CreateRGBSurface(SDL_HWSURFACE, 50, 50, 32, 0, 0, 0, 0);
	SDL_FillRect(carrVert, NULL, SDL_MapRGB((Time->fenetre)->format, 126,217,87));
	SDL_Rect carrVertPos;
	carrVertPos.x = 0;
	carrVertPos.y = 0;
	for (int i = 0; i < 10; ++i){
		SDL_BlitSurface(paw, NULL, (Time->fenetre), &pawPos);
		SDL_Flip((Time->fenetre));
		pawPos.x += 50;	
	}
	while(*(Time->running) && *(Time->time) > 0){
		SDL_Delay(1000);
		(*(Time->time))--;
		SDL_BlitSurface(carrVert, NULL, (Time->fenetre), &carrVertPos);
		SDL_Flip((Time->fenetre));
		carrVertPos.x += 50;
	}
	SDL_FreeSurface(carrVert);
	SDL_FreeSurface(paw);
	if(*(Time->running)){
		*(Time->running) = 0;
		youLose(1, (Time->fenetre));
	}
    pthread_exit(NULL);
}


int remove_element(obstacle *array, int index, size_t *array_length) {
    if (*array_length <= 1) {
        // If there is only one element or an empty array, set array_length to 0
        *array_length = 0;
        return 1;
    }
    else {
        int i;
        for (i = index; i < *array_length - 1; i++){
            array[i] = array[i + 1];
        }
        (*array_length)--;
    }
    return 0;
}


bool checkCollision(int newX, int newY, obstacle obstacles[], size_t obsSize) {
	for (int i = 0; i < obsSize; ++i){
		if (obstacles[i].x == newX && newY == obstacles[i].y){
			return false;
		}
	}
	return true;
}

void youWin(int lvl, SDL_Surface *fenetre){
		SDL_Surface* background = IMG_Load("./img/win.png");
		SDL_BlitSurface(background, NULL, fenetre, NULL);
		SDL_Delay(200);
		int i= 1000;
		while(i){
			SDL_BlitSurface(background, NULL, fenetre, NULL);
			SDL_Flip(fenetre);//mettre a jour la fenetre
			i--;
			SDL_Delay(1);
		};
		exit(1);
}
void checkWin(int lvl, obstacle bones[], size_t *BoneSize, SDL_Surface *fenetre, int currentX, int currentY, int *running){
	for (int i = 0; i < *BoneSize; i++){
		if(((currentX) == bones[i].x && currentY == bones[i].y) || ((currentX == (bones[i].x + 5)) && currentY == bones[i].y)){
			int win = remove_element(bones, i, BoneSize);
			if(win && *running){
				*running = 0;
				youWin(lvl, fenetre);
			}
		}
	}
}

void bonesPos(int lvl, obstacle bones[], size_t BoneSize, SDL_Surface *fenetre){
	SDL_Surface* bone = IMG_Load("./img/bone.png");
	for (int i = 0; i < BoneSize; i++){
		SDL_Rect bonePos;
		bonePos.x = bones[i].x;
		bonePos.y = bones[i].y;
		SDL_BlitSurface(bone, NULL, fenetre, &bonePos);
	}
}