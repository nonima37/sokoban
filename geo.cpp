#include <iostream>
#include <utility>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <time.h>

const int SCREEN_WIDTH = 540;
const int SCREEN_HEIGHT = 420;

const int NUM_OF_BLOCKS = 6;
const int NUM_OF_BOXES = 2;

int player_w = 60; // this is used for default size for all the block and boxes
int player_h = 60;
int player_x = 120;
int player_y = 360;

int blocks[NUM_OF_BLOCKS][2] = 
{ 
	{240, SCREEN_HEIGHT - player_w}, 
	//{240, SCREEN_HEIGHT - (player_w * 2)},
	{240, SCREEN_HEIGHT - (player_w * 3)},
	{240, SCREEN_HEIGHT - (player_w * 4)},
	{0, player_w * 2},
	{0 + player_w, player_w * 2},
	{0 + player_w * 2, player_w * 2},
};

int boxes[NUM_OF_BOXES][2] = 
{
	{player_w, player_w},
	{player_w * 6, player_w * 2}
};

int objective[NUM_OF_BOXES][2] = 
{
	{player_w * 2, player_w * 5},
	{0, player_w * 4}
};
//starts up sdl an dcreates window
bool init();

//loads media
bool load_media();

//free mdeia and shuts down sdl
void close();

//object positions
int object_positions[4][2];

//load individual image as texture
SDL_Texture* load_texture( std::string path );

//the window we'll be rendering to
SDL_Window* g_window = NULL;

//the window rendered
SDL_Renderer* g_renderer = NULL;


// Creates window g_window
// Creates renderer g_renderer
// And returns boolean if everything went ok
bool init() {
	bool success = true;

	if (SDL_Init( SDL_INIT_VIDEO) < 0) {
		printf( "SDL error: %s\n", SDL_GetError() );
		success = false;
	}
	else {
		//set texture filtering to linear
		if (!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" )) {
			printf( "Warning: Linear texture filterint not enabled!\n" );
		}

		//create window
		g_window = SDL_CreateWindow ( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (g_window == NULL) {
			printf( "Window could not be created: %s\n", SDL_GetError());
			success = false;
		}
		else {
			//craete renderer for window
			g_renderer = SDL_CreateRenderer( g_window, -1, SDL_RENDERER_ACCELERATED );
			if (g_renderer == NULL) { 
				success = false;
			}
			else {
				//initialize renderer coor
				SDL_SetRenderDrawColor( g_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
			}
		}
	}
	return success;
}

SDL_Texture* load_texture( std::string path ) {
	//The final texure
	SDL_Texture* new_texture = NULL;

	//Load image at specified path
	SDL_Surface* loaded_surface = IMG_Load( path.c_str() );
	if (loaded_surface == NULL) {
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else {
		//Create texture from surface pixels
		new_texture = SDL_CreateTextureFromSurface( g_renderer, loaded_surface);
		if (new_texture == NULL) {
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		//Get rid of old loaded surface
		SDL_FreeSurface( loaded_surface );
	}
	return new_texture;
}

// Creates g_texture and return bool if its ok (we don't need this)
bool load_media() {
	bool success = true;
	return success;
}

void close() {
	//destroy window
	SDL_DestroyRenderer( g_renderer );
	SDL_DestroyWindow( g_window );
	g_window = NULL;
	g_renderer = NULL;

	//quit sdl subsystems
	IMG_Quit();
	SDL_Quit();
}

// Set srand() before calling
int* random_pos() {
	const int len = 4;
	static int cords[2];

	for (int i = 0; i < len; i++) {
		cords[0] = rand() % 740;
		cords[1] = rand() % 480;
	}
	return cords;
}

// Check collision
bool check_collision(int object_x, int object_y, int branch_x, int branch_y) {
	bool good = 1;
	for (int i = 0; i < NUM_OF_BLOCKS; i++) {
		// Branchless programming for some reason
		if (object_x + (player_w * branch_x) == blocks[i][0] && object_y + (player_w * branch_y) == blocks[i][1]) { good = 0; }
	}
	return good;
}

int* check_to_push(int x, int y) {
	// 0:x, 1:y, 2: index
	static int box_cords[3];
	for (int i = 0; i < NUM_OF_BOXES; i++) {
		if (player_x + player_w * x == boxes[i][0] && player_y + (player_w * y) == boxes[i][1]) {
			box_cords[0] = boxes[i][0];
			box_cords[1] = boxes[i][1];
			box_cords[2] = i;
			return box_cords;
		}; 
	}
	return 0;
}

bool check_win() {
	int count = 0;
	for (int i = 0; i < NUM_OF_BOXES; i++) {
		for (int j = 0; j < NUM_OF_BOXES; j++) {
			if (boxes[i][0] == objective[j][0] && boxes[i][1] == objective[j][1]) { 
				count++; 
				break; 
			}
		}
		if (count == NUM_OF_BOXES) return 1;		
	}
	return 0;
}

int main (int argc, char* args[]) {
	int* box_cords;	

	if (!init()) {
		printf( "Failed to initialize!\n" );
	}
	else {
		if (!load_media()) {
			printf( "Failed to load media!\n" );
		}
		else {
			bool quit = false;
			//event handler
			SDL_Event e;

			while (!quit) {
				if (check_win()) {
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
											"You Win!","You Win Dumbass!", NULL);
					break;
				}
				while (SDL_PollEvent( &e ) != 0) {
					if( e.type == SDL_QUIT ) { quit = true; }

					else if( e.type == SDL_KEYDOWN ) {
						switch( e.key.keysym.sym ) {
							case SDLK_w:
								if (box_cords = check_to_push(0, -1)) {
									if ( check_collision( boxes[box_cords[2]][0], boxes[box_cords[2]][1], 0, -1 )
										&& ( boxes[box_cords[2]][1] - player_w > 0 - player_w ) ) {
										boxes[box_cords[2]][1] -= player_w;
										player_y -= player_w;
									}
								}
								else if (player_y > 0 && check_collision(player_x, player_y, 0, -1)) {
									player_y -= player_w; 
								}
								break;

							case SDLK_s:
								if (box_cords = check_to_push(0, 1)) {
									if (check_collision( boxes[box_cords[2]][0], boxes[box_cords[2]][1], 0, 1)
										&& ( boxes[box_cords[2]][1] + player_w < SCREEN_HEIGHT ) ) {
										boxes[box_cords[2]][1] += player_w;
										player_y += player_w;
									}
								}
								else if (player_y + player_w < SCREEN_HEIGHT && check_collision(player_x, player_y, 0, 1)) {
									player_y += player_w; 
								}
								break;
							  
							case SDLK_d:
								if (box_cords = check_to_push(1, 0)) {
									if (check_collision( boxes[box_cords[2]][0], boxes[box_cords[2]][1], 1, 0)
										&& ( boxes[*(box_cords + 2)][0] + player_w < SCREEN_WIDTH) ) {
										boxes[*(box_cords + 2)][0] += player_w;
										player_x += player_w;
									}
								}
								else if (player_x + player_w < SCREEN_WIDTH && check_collision(player_x, player_y, 1, 0)) {
									player_x += player_w; 
								}
								break;

							case SDLK_a:
								if (box_cords = check_to_push(-1, 0)) {
									if ( check_collision(boxes[box_cords[2]][0], boxes[box_cords[2]][1], -1, 0 )
										&& ( boxes[*(box_cords + 2)][0] - player_w > 0 - player_w) ) {
										boxes[*(box_cords + 2)][0] -= player_w;
										player_x -= player_w;
									}
								}
								else if (player_x > 0 && check_collision(player_x, player_y, -1, 0)) {
									player_x -= player_w; 
								}
								break;

							default:
								break;
						}
					}
				}
				
				// Clear screen
				SDL_SetRenderDrawColor( g_renderer, 0xC5, 0xB9, 0xB6, 0x00 );
				SDL_RenderClear( g_renderer );

				// Render the player
				SDL_Rect player_rect = { player_x, player_y, player_w, player_h };
				SDL_SetRenderDrawColor( g_renderer, 0x25, 0x22, 0x82, 0x00 );
				SDL_RenderFillRect( g_renderer, &player_rect );

				// Blocks
				for (int i = 0; i < NUM_OF_BLOCKS; i++) {
					// Blocks and players are the same size
					SDL_Rect block_rect = { blocks[i][0], blocks[i][1], player_w, player_h };
					SDL_SetRenderDrawColor( g_renderer, 0x82, 0x6f, 0x5c, 0x00 );
					SDL_RenderFillRect( g_renderer, &block_rect );
				}

				// Boxes
				for (int i = 0; i < NUM_OF_BOXES; i++) {
					SDL_Rect box_rect = { boxes[i][0], boxes[i][1], player_w, player_h };
					SDL_SetRenderDrawColor( g_renderer, 0x3e, 0x29, 0x25, 0x00 );
					SDL_RenderFillRect( g_renderer, &box_rect );
				}

				// Objective
				for (int i = 0; i < NUM_OF_BOXES; i++) {
					SDL_Rect objective_outline = { objective[i][0], objective[i][1], player_w, player_h };
					SDL_SetRenderDrawColor( g_renderer, 0x6b, 0xff, 0x21, 0x00 );
					SDL_RenderDrawRect( g_renderer, &objective_outline );
				}
				// Update screen
				SDL_RenderPresent( g_renderer );
			}
		}
	}
	close();
	return 0;
}

