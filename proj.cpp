
/*
    Sardar Muneeb Hassan (19i-2015) CS-B
    Ayesha Zafar (19i-1983) CS-B
*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <random>
#include <string>
#include <iostream>
using namespace std;
using namespace sf;


//DECLARING ALL THE VARIABLES THAT ARE USED THROUGHOUT THE GAMEE 
int COLOR_CHECKER;

//DECLARING THE SFML VARIABLES TO DRAW BOARD, DICE AND TOKENS
sf::Image DICEimg;
sf::Sprite DICE_Sprite;
sf::Texture DICE_texture;

sf::Image golaImg;
sf::Sprite golaSprite;
sf::Texture golaTexture;

sf::RenderWindow* window;
sf::Event event1;
sf::VideoMode video_MODE;
sf::Sprite BoardSprite;
sf::Texture Boardtexture;

sf::Image green, yellow, red, blue;
sf::Texture greenTexture, yellowTexture, redTexture, blueTexture;
sf::Sprite greenSprite[4], redSprite[4], blueSprite[4], yellowSprite[4];

//SFML VARIABLE TO RUN MUSIC IN GAME
sf::Music music;

//VARIABLES TO SET COORDINATES OF BOARD TO MOVE TOKENS
int redCoordinates [57][2];
int blueCoordinates [57][2];
int greenCoordinates [57][2];
int yellowCoordinates [57][2];

int RED_INCREMENTOR=0;
int BLUE_INCREMENTOR=0;
int GREEN_INCREMENTOR=0;
int YELLOW_INCREMENTOR=0;

bool RED_CHAKKA_CHECKER[4] = {false}; 
bool BLUE_CHAKKA_CHECKER[4] = {false}; 
bool GREEN_CHAKKA_CHECKER[4] = {false};
bool YELLOW_CHAKKA_CHECKER[4] = {false};

int PLAYER_NUM;
int TURN_COUNTER;


//VARIABLES FOR PTHREADS AND SEMAPHORES
sem_t PLAYER_SEMA; //SEMAPHORE FOR PLAYER THREADS
pthread_t player_thread[4]; //ALL FOUR PLAYER THREADS
pthread_t KILLER_THREAD;    //THREAD THAT CHECKS IF COORDINATES MATCHES AND KILLS THE TOKEN
pthread_t DICE_THREAD;   
sem_t DICE_SEMA;






/*
    FUNCTION THAT KEEPS CHECK OF COORDINATES OF BOARD AND IF TWO 
    TOKENS ARE ON SAME COORDINATES THEN IT KILLS THE OTHER TOKEN
*/
void* KILLER (void* arg) {

    //arg IS A RANDOM NUMBER THAT IS PASSSED IN PTHREAD CREATE AND HERE WHATEVER THAT NUMBER IS DETERMINES WHICH COLORED TOKEN SHOULD BE USED
    int color = *(int*)arg;

    if(color == 0) //FOR RED TOKEN 
    {
        //CONDITION FOR IF RED MATCHES COORDINATES WITH BLUE 
        //IN SUCH CASE BLUE INCREMENTOR OR INDEX IS SET BACK TO 0TH POSITION
        if (redSprite[0].getPosition().x == blueSprite[0].getPosition().x && redSprite[0].getPosition().y == blueSprite[0].getPosition().y) {
            blueSprite[0].setPosition(505,505);    
            BLUE_INCREMENTOR = 0; 
        }
        //CONDITION FOR IF RED MATCHES COORDINATES WITH GREEN 
        //IN SUCH CASE GREEN INCREMENTOR OR INDEX IS SET BACK TO 0TH POSITION
        else if (redSprite[0].getPosition().x == greenSprite[0].getPosition().x && redSprite[0].getPosition().y == greenSprite[0].getPosition().y) {
            greenSprite[0].setPosition(90,90);
            GREEN_INCREMENTOR = 0;     
        }
        //CONDITION FOR IF RED MATCHES COORDINATES WITH YELLOW 
        //IN SUCH CASE YELLOW INCREMENTOR OR INDEX IS SET BACK TO 0TH POSITION
        else if (redSprite[0].getPosition().x == yellowSprite[0].getPosition().x && redSprite[0].getPosition().y == yellowSprite[0].getPosition().y) {
            yellowSprite[0].setPosition(505,90);
            YELLOW_INCREMENTOR = 0;
        }
    }

    else if(color == 1) //FOR BLUE TOKEN
    {
        //CONDITION FOR IF BLUE MATCHES COORDINATES WITH RED 
        //IN SUCH CASE RED INCREMENTOR OR INDEX IS SET BACK TO 0TH POSITION
        if (blueSprite[0].getPosition().x == redSprite[0].getPosition().x && blueSprite[0].getPosition().y == redSprite[0].getPosition().y) {
            redSprite[0].setPosition(90,505);     
            RED_INCREMENTOR = 0;
        }
        //CONDITION FOR IF BLUE MATCHES COORDINATES WITH GREEN
        //IN SUCH CASE GREEN INCREMENTOR OR INDEX IS SET BACK TO 0TH POSITION
        else if (blueSprite[0].getPosition().x == greenSprite[0].getPosition().x && blueSprite[0].getPosition().y == greenSprite[0].getPosition().y) {
            greenSprite[0].setPosition(90,90);     
            GREEN_INCREMENTOR = 0;
        }
        //CONDITION FOR IF BLUE MATCHES COORDINATES WITH YELLOW 
        //IN SUCH CASE YELLOW INCREMENTOR OR INDEX IS SET BACK TO 0TH POSITION
        else if (blueSprite[0].getPosition().x == yellowSprite[0].getPosition().x && blueSprite[0].getPosition().y == yellowSprite[0].getPosition().y) {
            yellowSprite[0].setPosition(505,90);
            YELLOW_INCREMENTOR = 0;
        }
    }

    else if(color == 2) //FOR GREEN TOKEN
    {
        //CONDITION FOR IF GREEN MATCHES COORDINATES WITH BLUE 
        //IN SUCH CASE BLUE INCREMENTOR OR INDEX IS SET BACK TO 0TH POSITION        
        if (greenSprite[0].getPosition().x == blueSprite[0].getPosition().x && greenSprite[0].getPosition().y == blueSprite[0].getPosition().y) {
            blueSprite[0].setPosition(505,505);     
            BLUE_INCREMENTOR = 0;
        }
        //CONDITION FOR IF GREEN MATCHES COORDINATES WITH RED
        //IN SUCH CASE RED INCREMENTOR OR INDEX IS SET BACK TO 0TH POSITION    
        else if (greenSprite[0].getPosition().x == redSprite[0].getPosition().x && greenSprite[0].getPosition().y == redSprite[0].getPosition().y) {
            redSprite[0].setPosition(90,505);     
            RED_INCREMENTOR = 0;
        }
        //CONDITION FOR IF GREEN MATCHES COORDINATES WITH YELLOW 
        //IN SUCH CASE YELLOW INCREMENTOR OR INDEX IS SET BACK TO 0TH POSITION
        else if (greenSprite[0].getPosition().x == yellowSprite[0].getPosition().x && greenSprite[0].getPosition().y == yellowSprite[0].getPosition().y) {
            yellowSprite[0].setPosition(505,90);
            YELLOW_INCREMENTOR = 0;
        }
    }

    else if(color == 3) //FOR YELLOW TOKEN
    {
        if (yellowSprite[0].getPosition().x == blueSprite[0].getPosition().x && yellowSprite[0].getPosition().y == blueSprite[0].getPosition().y) {
            blueSprite[0].setPosition(505,505);     
            BLUE_INCREMENTOR = 0;
        }
        else if (yellowSprite[0].getPosition().x == greenSprite[0].getPosition().x && yellowSprite[0].getPosition().y == greenSprite[0].getPosition().y) {
            greenSprite[0].setPosition(90,90);     
            GREEN_INCREMENTOR = 0;
        }
        else if (yellowSprite[0].getPosition().x == redSprite[0].getPosition().x && yellowSprite[0].getPosition().y == redSprite[0].getPosition().y) {
            redSprite[0].setPosition(90,505);
            RED_INCREMENTOR = 0;
        }
    }

    pthread_exit(NULL);
}


//THIS FUNCTION GENERATES RANDON VALUES FOR EACH DICE ROLL
int RANDOMIZER () {
    int roll = rand() % 6 + 1;
    return roll;
}

//CLASS FOR IMPLEMENTATION OF DICE AND DRAWING OF PLAYERS SCORE ON SCREEN
class DICE_CLASS {
public:
    int DICE_VALUE;
    int DICE_INCREMENTOR;
    int DICE_X;
    int DICE_Y;

    DICE_CLASS() {
        DICE_VALUE = 9;
        DICE_X = 200;
        DICE_Y = 200;        
        DICE_INCREMENTOR = 0;
    }

    virtual ~DICE_CLASS() {

    }

    //FUNCTION FOR FINDING SCORE OF EACH PLAYER AND THEN DISPLAYING IT ON SCREEN
    void SCORE () {

        string RED_SCORE, GREEN_SCORE, BLUE_SCORE, YELLOW_SCORE;

        RED_SCORE = to_string(RED_INCREMENTOR);
        GREEN_SCORE = to_string(GREEN_INCREMENTOR);
        BLUE_SCORE = to_string(BLUE_INCREMENTOR);
        YELLOW_SCORE = to_string(YELLOW_INCREMENTOR);

        Font font;
        font.loadFromFile("font.otf");
        Text RED_TEXT(RED_SCORE, font), GREEN_TEXT(GREEN_SCORE, font), BLUE_TEXT(BLUE_SCORE, font), YELLOW_TEXT(YELLOW_SCORE, font);

        RED_TEXT.setCharacterSize (30);
        BLUE_TEXT.setCharacterSize (30);
        YELLOW_TEXT.setCharacterSize (30);
        GREEN_TEXT.setCharacterSize (30);

        RED_TEXT.setFillColor(Color::Red);
        BLUE_TEXT.setFillColor(Color::Blue);
        GREEN_TEXT.setFillColor(Color::Green);
        YELLOW_TEXT.setFillColor(Color::Yellow);

        RED_TEXT.setPosition(1030,520);
        BLUE_TEXT.setPosition(1030,560);
        GREEN_TEXT.setPosition(1030,600);
        YELLOW_TEXT.setPosition(1030,640);


        Text RED_T("Red Player", font), GREEN_T("Green Player", font), BLUE_T("Blue Player", font), YELLOW_T("Yellow Player", font);
        RED_T.setCharacterSize (30);
        BLUE_T.setCharacterSize (30);
        YELLOW_T.setCharacterSize (30);
        GREEN_T.setCharacterSize (30);

        RED_T.setFillColor(Color::Black);
        BLUE_T.setFillColor(Color::Black);
        GREEN_T.setFillColor(Color::Black);
        YELLOW_T.setFillColor(Color::Black);

        RED_T.setPosition(760,520);
        BLUE_T.setPosition(760,560);
        GREEN_T.setPosition(760,600);
        YELLOW_T.setPosition(760,640);


        Text TURRN("YOUR TURN", font);
        TURRN.setCharacterSize (40);
        TURRN.setFillColor(Color::Black);
        TURRN.setPosition(80,780);

        Text SB("SCORE BOARD", font);
        SB.setCharacterSize (40);
        SB.setFillColor(Color::Black);
        SB.setPosition(760,440);

        window->draw(RED_TEXT);
        window->draw(BLUE_TEXT);
        window->draw(YELLOW_TEXT);
        window->draw(GREEN_TEXT);

        window->draw(RED_T);
        window->draw(BLUE_T);
        window->draw(YELLOW_T);
        window->draw(GREEN_T);

        window->draw(TURRN);
        window->draw(SB);

    }

    //RANDOM DICE DRAWING ON SCREEN FOR EACH TURN
    void draw_DICE () {
        int val = this->DICE_VALUE;

        if (val == 9) {
            if (!DICEimg.loadFromFile("mainchoti.png"))
                cout<<"ERROR"<<endl;
        }
        else if(val == 1) {
            if (!DICEimg.loadFromFile("choti1.png"))
                cout<<"ERROR"<<endl;
        }
        else if (val == 2) {
            if (!DICEimg.loadFromFile("choti2.png"))
                cout<<"ERROR"<<endl;
        }        
        else if (val == 3) {
            if (!DICEimg.loadFromFile("choti3.png"))
                cout<<"ERROR"<<endl;
        }        
        else if (val == 4) {
            if (!DICEimg.loadFromFile("choti4.png"))
                cout<<"ERROR"<<endl;
        }        
        else if (val == 5) {
            if (!DICEimg.loadFromFile("choti5.png"))
                cout<<"ERROR"<<endl;
        }        
        else if (val == 6) {
            if (!DICEimg.loadFromFile("choti6.png"))
                cout<<"ERROR"<<endl;
        }        

        DICE_texture.loadFromImage(DICEimg);
        DICE_Sprite.setTexture(DICE_texture); 
        DICE_Sprite.setPosition(830, 180);
    }

    //TURN INDICATOR
    void draw_GOLLA () {
        if (COLOR_CHECKER == 0) {
            if (!golaImg.loadFromFile("red.png"))
                cout<<"ERROR"<<endl;
        }        
        if (COLOR_CHECKER == 1) {
            if (!golaImg.loadFromFile("blue.png"))
                cout<<"ERROR"<<endl;
        }        
        if (COLOR_CHECKER == 2) {
            if (!golaImg.loadFromFile("green.png"))
                cout<<"ERROR"<<endl;
        }        
        if (COLOR_CHECKER == 3) {
            if (!golaImg.loadFromFile("yellow.png"))
                cout<<"ERROR"<<endl;
        }        

        golaTexture.loadFromImage(golaImg);
        golaSprite.setTexture(golaTexture); 
        golaSprite.setScale(1.5f, 1.5f);
        golaSprite.setPosition(330, 760);
    }

};

DICE_CLASS DICE_OBJECT;




//FUNCTION THAT IS USED BY PLAYER THREAD AND IS RESPONSIBLE FOR THE MOVEMENT OF TOKENS ON BOARD
void* TOKEN_MOVEMENT (void* arg) {

    COLOR_CHECKER = *(int*)arg;

    //DECREMENTING THE SEMAPHORE VALUE AGAIN SO NO OTHER THREAD ACCESSES CRITICAL PORTION UNLESS ONE TOKEN IS DONE
    sem_wait(&PLAYER_SEMA);

    int value;
    
    //CONDITION THAT A TOKEN NEEDS ONE SIX TO START GAME
    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 0 && redSprite[0].getPosition().x == 90 && redSprite[0].getPosition().y == 505) {
        RED_CHAKKA_CHECKER[0] = true;
        RED_INCREMENTOR -= 6;
    }
    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 1 && blueSprite[0].getPosition().x == 505 && blueSprite[0].getPosition().y == 505) {
        BLUE_CHAKKA_CHECKER[0] = true;
        BLUE_INCREMENTOR -= 6;
    }
    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 2 && greenSprite[0].getPosition().x == 90 && greenSprite[0].getPosition().y == 90) {
        GREEN_CHAKKA_CHECKER[0] = true;
        GREEN_INCREMENTOR -= 6;
    }
    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 3 && yellowSprite[0].getPosition().x == 505 && yellowSprite[0].getPosition().y == 90) {
        YELLOW_CHAKKA_CHECKER[0] = true;
        YELLOW_INCREMENTOR -= 6;
    }
/*

    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 0 && redSprite[1].getPosition().x == 90 && redSprite[1].getPosition().y == 550) {
        RED_CHAKKA_CHECKER[1] = true;
        RED_INCREMENTOR -= 6;
    }
    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 1 && blueSprite[1].getPosition().x == 550 && blueSprite[1].getPosition().y == 505) {
        BLUE_CHAKKA_CHECKER[1] = true;
        BLUE_INCREMENTOR -= 6;
    }
    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 2 && greenSprite[1].getPosition().x == 135 && greenSprite[1].getPosition().y == 90) {
        GREEN_CHAKKA_CHECKER[1] = true;
        GREEN_INCREMENTOR -= 6;
    }
    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 3 && yellowSprite[1].getPosition().x == 550 && yellowSprite[1].getPosition().y == 90) {
        YELLOW_CHAKKA_CHECKER[1] = true;
        YELLOW_INCREMENTOR -= 6;
    }


    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 0 && redSprite[2].getPosition().x == 135 && redSprite[2].getPosition().y == 505) {
        RED_CHAKKA_CHECKER[2] = true;
        RED_INCREMENTOR -= 6;
    }
    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 1 && blueSprite[2].getPosition().x == 550 && blueSprite[2].getPosition().y == 550) {
        BLUE_CHAKKA_CHECKER[2] = true;
        BLUE_INCREMENTOR -= 6;
    }
    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 2 && greenSprite[2].getPosition().x == 135 && greenSprite[2].getPosition().y == 135) {
        GREEN_CHAKKA_CHECKER[2] = true;
        GREEN_INCREMENTOR -= 6;
    }
    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 3 && yellowSprite[2].getPosition().x == 550 && yellowSprite[2].getPosition().y == 135) {
        YELLOW_CHAKKA_CHECKER[2] = true;
        YELLOW_INCREMENTOR -= 6;
    }


    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 0 && redSprite[3].getPosition().x == 135 && redSprite[3].getPosition().y == 550) {
        RED_CHAKKA_CHECKER[3] = true;
        RED_INCREMENTOR -= 6;
    }
    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 1 && blueSprite[3].getPosition().x == 505 && blueSprite[3].getPosition().y == 550) {
        BLUE_CHAKKA_CHECKER[3] = true;
        BLUE_INCREMENTOR -= 6;
    }
    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 2 && greenSprite[3].getPosition().x == 90 && greenSprite[3].getPosition().y == 135) {
        GREEN_CHAKKA_CHECKER[3] = true;
        GREEN_INCREMENTOR -= 6;
    }
    if (DICE_OBJECT.DICE_VALUE == 6 && COLOR_CHECKER == 3 && yellowSprite[3].getPosition().x == 505 && yellowSprite[3].getPosition().y == 135) {
        YELLOW_CHAKKA_CHECKER[3] = true;
        YELLOW_INCREMENTOR -= 6;
    }

*/

    //DRAWING THE SPRITE ON SCREEN ON WHATEVER POSITION IS SET HERE
    if( COLOR_CHECKER == 0 && RED_CHAKKA_CHECKER[0] == true) {
        RED_INCREMENTOR+=DICE_OBJECT.DICE_VALUE;
        redSprite[0].setPosition(redCoordinates[RED_INCREMENTOR][0], redCoordinates[RED_INCREMENTOR][1]);            
    }

    if( COLOR_CHECKER == 1 && BLUE_CHAKKA_CHECKER[0] == true) {
        BLUE_INCREMENTOR+=DICE_OBJECT.DICE_VALUE;
        blueSprite[0].setPosition(blueCoordinates[BLUE_INCREMENTOR][0], blueCoordinates[BLUE_INCREMENTOR][1]);            
    }
    if( COLOR_CHECKER == 2 && GREEN_CHAKKA_CHECKER[0] == true) {
        GREEN_INCREMENTOR+=DICE_OBJECT.DICE_VALUE;
        greenSprite[0].setPosition(greenCoordinates[GREEN_INCREMENTOR][0], greenCoordinates[GREEN_INCREMENTOR][1]);            
    }
    if( COLOR_CHECKER == 3 && YELLOW_CHAKKA_CHECKER[0] == true) {
        YELLOW_INCREMENTOR+=DICE_OBJECT.DICE_VALUE;
        yellowSprite[0].setPosition(yellowCoordinates[YELLOW_INCREMENTOR][0], yellowCoordinates[YELLOW_INCREMENTOR][1]);            
    }

//    cout << "the left button was pressed" << endl;
//    cout << "mouse x: " << event1.mouseButton.x << endl;
//    cout << "mouse y: " << event1.mouseButton.y << endl;
//sem_post(&PLAYER_SEMA);

    pthread_exit(NULL);
}


//THIS FUNCTION IS RESPONSIBLE FOR CREATING THE FOUR PLAYER THREADS
//AND SENDING THE i IN PTHREAD CREATE WHICH DECIDES THE TOKEN COLOR RANDOMELY 
void player_thread_maker() {

    for (int i=0; i < PLAYER_NUM; i++) {
//        cout<<" IIIIII"<<i<<endl;
        int* a = new int;
        *a = i;
        pthread_create(&player_thread[i], NULL, TOKEN_MOVEMENT, a);
    }


}


//CREATING GAME CLASS
//MAIN BOSS THING WHICH WILL CONTROLL EVERYTHING
class Game {

public:
    //INITALIZING ALL VARIABLES AND SETTING COORDINATES
    void init_VARIABLES() {

        //INITALIZING SEMAPHORE
        //sem_init (&sem1, 0, 0);


        window = nullptr;
        video_MODE.height = 600;
        video_MODE.width = 600;

        //SETTING INITAL POSITIONS
        redSprite[0].setPosition(90,505);
        redSprite[1].setPosition(90,550);
        redSprite[2].setPosition(135,505);
        redSprite[3].setPosition(135,550);

        greenSprite[0].setPosition(90,90);
        greenSprite[1].setPosition(135,90);
        greenSprite[2].setPosition(135,135);
        greenSprite[3].setPosition(90,135);

        yellowSprite[0].setPosition(505,90);
        yellowSprite[1].setPosition(550,90);
        yellowSprite[2].setPosition(550,135);
        yellowSprite[3].setPosition(505,135);

        blueSprite[0].setPosition(505,505);
        blueSprite[1].setPosition(550,505);
        blueSprite[2].setPosition(550,550);
        blueSprite[3].setPosition(505,550);



        //SETTING COORDINATES OF BOARD ACCORDING TO TOKENS
        redCoordinates[0][0] = 275;
        redCoordinates[0][1] = 600;

        redCoordinates[1][0] = 275;
        redCoordinates[1][1] = 554;

        redCoordinates[2][0] = 275;
        redCoordinates[2][1] = 508;

        redCoordinates[3][0] = 275;
        redCoordinates[3][1] = 462;

        redCoordinates[4][0] = 275;
        redCoordinates[4][1] = 416;

        redCoordinates[5][0] = 229;
        redCoordinates[5][1] = 369;

        redCoordinates[6][0] = 183;
        redCoordinates[6][1] = 369;

        redCoordinates[7][0] = 137;
        redCoordinates[7][1] = 369;

        redCoordinates[8][0] = 91;
        redCoordinates[8][1] = 369;

        redCoordinates[9][0] = 45;
        redCoordinates[9][1] = 369;

        redCoordinates[10][0] = 0;
        redCoordinates[10][1] = 369;

        redCoordinates[11][0] = 0;
        redCoordinates[11][1] = 323;


        redCoordinates[12][0] = 0;
        redCoordinates[12][1] = 277;

        redCoordinates[13][0] = 46;
        redCoordinates[13][1] = 277;

        redCoordinates[14][0] = 92;
        redCoordinates[14][1] = 277;

        redCoordinates[15][0] = 138;
        redCoordinates[15][1] = 277;

        redCoordinates[16][0] = 184;
        redCoordinates[16][1] = 277;


        redCoordinates[17][0] = 230;
        redCoordinates[17][1] = 277;

        redCoordinates[18][0] = 276;
        redCoordinates[18][1] = 231;

        redCoordinates[19][0] = 276;
        redCoordinates[19][1] = 185;

        redCoordinates[20][0] = 276;
        redCoordinates[20][1] = 139;

        redCoordinates[21][0] = 276;
        redCoordinates[21][1] = 93;

        redCoordinates[22][0] = 276;
        redCoordinates[22][1] = 47;


        redCoordinates[23][0] = 276;
        redCoordinates[23][1] = 0;

        redCoordinates[24][0] = 322;
        redCoordinates[24][1] = 0;


        redCoordinates[25][0] = 368;
        redCoordinates[25][1] = 0;

        redCoordinates[26][0] = 368;
        redCoordinates[26][1] = 46;

        redCoordinates[27][0] = 368;
        redCoordinates[27][1] = 92;

        redCoordinates[28][0] = 368;
        redCoordinates[28][1] = 138;

        redCoordinates[29][0] = 368;
        redCoordinates[29][1] = 184;

        redCoordinates[30][0] = 368;
        redCoordinates[30][1] = 230;


        redCoordinates[31][0] = 414;
        redCoordinates[31][1] = 276;

        redCoordinates[32][0] = 460;
        redCoordinates[32][1] = 276;


        redCoordinates[33][0] = 506;
        redCoordinates[33][1] = 276;

        redCoordinates[34][0] = 552;
        redCoordinates[34][1] = 276;

        redCoordinates[35][0] = 598;
        redCoordinates[35][1] = 276;

        redCoordinates[36][0] = 644;
        redCoordinates[36][1] = 276;


        redCoordinates[37][0] = 644;
        redCoordinates[37][1] = 322;

        redCoordinates[38][0] = 644;
        redCoordinates[38][1] = 368;


        redCoordinates[39][0] = 598;
        redCoordinates[39][1] = 368;

        redCoordinates[40][0] = 552;
        redCoordinates[40][1] = 368;

        redCoordinates[41][0] = 506;
        redCoordinates[41][1] = 368;

        redCoordinates[42][0] = 460;
        redCoordinates[42][1] = 368;

        redCoordinates[43][0] = 414;
        redCoordinates[43][1] = 368;

        redCoordinates[44][0] = 368;
        redCoordinates[44][1] = 414;

        redCoordinates[45][0] = 368;
        redCoordinates[45][1] = 460;

        redCoordinates[46][0] = 368;
        redCoordinates[46][1] = 506;

        redCoordinates[47][0] = 368;
        redCoordinates[47][1] = 552;

        redCoordinates[48][0] = 368;
        redCoordinates[48][1] = 598;

        redCoordinates[49][0] = 368;
        redCoordinates[49][1] = 644;


        redCoordinates[50][0] = 322;
        redCoordinates[50][1] = 644;

        redCoordinates[51][0] = 322;
        redCoordinates[51][1] = 598;

        redCoordinates[52][0] = 322;
        redCoordinates[52][1] = 552;

        redCoordinates[53][0] = 322;
        redCoordinates[53][1] = 506;

        redCoordinates[54][0] = 322;
        redCoordinates[54][1] = 460;

        redCoordinates[55][0] = 322;
        redCoordinates[55][1] = 414;

        redCoordinates[56][0] = 322;
        redCoordinates[56][1] = 368;


    //Green

        greenCoordinates[0][0] = 46;
        greenCoordinates[0][1] = 277;

        greenCoordinates[1][0] = 92;
        greenCoordinates[1][1] = 277;

        greenCoordinates[2][0] = 138;
        greenCoordinates[2][1] = 277;

        greenCoordinates[3][0] = 184;
        greenCoordinates[3][1] = 277;


        greenCoordinates[4][0] = 230;
        greenCoordinates[4][1] = 277;

        greenCoordinates[5][0] = 276;
        greenCoordinates[5][1] = 231;

        greenCoordinates[6][0] = 276;
        greenCoordinates[6][1] = 185;

        greenCoordinates[7][0] = 276;
        greenCoordinates[7][1] = 139;

        greenCoordinates[8][0] = 276;
        greenCoordinates[8][1] = 93;

        greenCoordinates[9][0] = 276;
        greenCoordinates[9][1] = 47;


        greenCoordinates[10][0] = 276;
        greenCoordinates[10][1] = 0;

        greenCoordinates[11][0] = 322;
        greenCoordinates[11][1] = 0;


        greenCoordinates[12][0] = 368;
        greenCoordinates[12][1] = 0;

        greenCoordinates[13][0] = 368;
        greenCoordinates[13][1] = 46;

        greenCoordinates[14][0] = 368;
        greenCoordinates[14][1] = 92;

        greenCoordinates[15][0] = 368;
        greenCoordinates[15][1] = 138;

        greenCoordinates[16][0] = 368;
        greenCoordinates[16][1] = 184;

        greenCoordinates[17][0] = 368;
        greenCoordinates[17][1] = 230;


        greenCoordinates[18][0] = 414;
        greenCoordinates[18][1] = 276;

        greenCoordinates[19][0] = 460;
        greenCoordinates[19][1] = 276;


        greenCoordinates[20][0] = 506;
        greenCoordinates[20][1] = 276;

        greenCoordinates[21][0] = 552;
        greenCoordinates[21][1] = 276;

        greenCoordinates[22][0] = 598;
        greenCoordinates[22][1] = 276;

        greenCoordinates[23][0] = 644;
        greenCoordinates[23][1] = 276;


        greenCoordinates[24][0] = 644;
        greenCoordinates[24][1] = 322;

        greenCoordinates[25][0] = 644;
        greenCoordinates[25][1] = 368;


        greenCoordinates[26][0] = 598;
        greenCoordinates[26][1] = 368;

        greenCoordinates[27][0] = 552;
        greenCoordinates[27][1] = 368;

        greenCoordinates[28][0] = 506;
        greenCoordinates[28][1] = 368;

        greenCoordinates[29][0] = 460;
        greenCoordinates[29][1] = 368;

        greenCoordinates[30][0] = 414;
        greenCoordinates[30][1] = 368;

        greenCoordinates[31][0] = 368;
        greenCoordinates[31][1] = 414;

        greenCoordinates[32][0] = 368;
        greenCoordinates[32][1] = 460;

        greenCoordinates[33][0] = 368;
        greenCoordinates[33][1] = 506;

        greenCoordinates[34][0] = 368;
        greenCoordinates[34][1] = 552;

        greenCoordinates[35][0] = 368;
        greenCoordinates[35][1] = 598;

        greenCoordinates[36][0] = 368;
        greenCoordinates[36][1] = 644;


        greenCoordinates[37][0] = 322;
        greenCoordinates[37][1] = 644;

        greenCoordinates[38][0] = 275;
        greenCoordinates[38][1] = 644;

        greenCoordinates[39][0] = 275;
        greenCoordinates[39][1] = 600;

        greenCoordinates[40][0] = 275;
        greenCoordinates[40][1] = 554;

        greenCoordinates[41][0] = 275;
        greenCoordinates[41][1] = 508;

        greenCoordinates[42][0] = 275;
        greenCoordinates[42][1] = 462;

        greenCoordinates[43][0] = 275;
        greenCoordinates[43][1] = 416;

        greenCoordinates[44][0] = 229;
        greenCoordinates[44][1] = 369;

        greenCoordinates[45][0] = 183;
        greenCoordinates[45][1] = 369;

        greenCoordinates[46][0] = 137;
        greenCoordinates[46][1] = 369;

        greenCoordinates[47][0] = 91;
        greenCoordinates[47][1] = 369;

        greenCoordinates[48][0] = 45;
        greenCoordinates[48][1] = 369;

        greenCoordinates[49][0] = 0;
        greenCoordinates[49][1] = 369;

        greenCoordinates[50][0] = 0;
        greenCoordinates[50][1] = 323;


        greenCoordinates[51][0] = 46;
        greenCoordinates[51][1] = 323;

        greenCoordinates[52][0] = 92;
        greenCoordinates[52][1] = 323;

        greenCoordinates[53][0] = 138;
        greenCoordinates[53][1] = 323;

        greenCoordinates[54][0] = 184;
        greenCoordinates[54][1] = 323;

        greenCoordinates[55][0] = 230;
        greenCoordinates[55][1] = 323;

        greenCoordinates[55][0] = 276;
        greenCoordinates[55][1] = 323;

        greenCoordinates[56][0] = 322;
        greenCoordinates[56][1] = 323;



    //Yellow
        yellowCoordinates[0][0] = 368;
        yellowCoordinates[0][1] = 46;

        yellowCoordinates[1][0] = 368;
        yellowCoordinates[1][1] = 92;

        yellowCoordinates[2][0] = 368;
        yellowCoordinates[2][1] = 138;

        yellowCoordinates[3][0] = 368;
        yellowCoordinates[3][1] = 184;

        yellowCoordinates[4][0] = 368;
        yellowCoordinates[4][1] = 230;


        yellowCoordinates[5][0] = 414;
        yellowCoordinates[5][1] = 276;

        yellowCoordinates[6][0] = 460;
        yellowCoordinates[6][1] = 276;


        yellowCoordinates[7][0] = 506;
        yellowCoordinates[7][1] = 276;

        yellowCoordinates[8][0] = 552;
        yellowCoordinates[8][1] = 276;

        yellowCoordinates[9][0] = 598;
        yellowCoordinates[9][1] = 276;

        yellowCoordinates[10][0] = 644;
        yellowCoordinates[10][1] = 276;


        yellowCoordinates[11][0] = 644;
        yellowCoordinates[11][1] = 322;

        yellowCoordinates[12][0] = 644;
        yellowCoordinates[12][1] = 368;


        yellowCoordinates[13][0] = 598;
        yellowCoordinates[13][1] = 368;

        yellowCoordinates[14][0] = 552;
        yellowCoordinates[14][1] = 368;

        yellowCoordinates[15][0] = 506;
        yellowCoordinates[15][1] = 368;

        yellowCoordinates[16][0] = 460;
        yellowCoordinates[16][1] = 368;

        yellowCoordinates[17][0] = 414;
        yellowCoordinates[17][1] = 368;

        yellowCoordinates[18][0] = 368;
        yellowCoordinates[18][1] = 414;

        yellowCoordinates[19][0] = 368;
        yellowCoordinates[19][1] = 460;

        yellowCoordinates[20][0] = 368;
        yellowCoordinates[20][1] = 506;

        yellowCoordinates[21][0] = 368;
        yellowCoordinates[21][1] = 552;

        yellowCoordinates[22][0] = 368;
        yellowCoordinates[22][1] = 598;

        yellowCoordinates[23][0] = 368;
        yellowCoordinates[23][1] = 644;


        yellowCoordinates[24][0] = 322;
        yellowCoordinates[24][1] = 644;

        yellowCoordinates[25][0] = 275;
        yellowCoordinates[25][1] = 644;

        yellowCoordinates[26][0] = 275;
        yellowCoordinates[26][1] = 600;

        yellowCoordinates[27][0] = 275;
        yellowCoordinates[27][1] = 554;

        yellowCoordinates[28][0] = 275;
        yellowCoordinates[28][1] = 508;

        yellowCoordinates[29][0] = 275;
        yellowCoordinates[29][1] = 462;

        yellowCoordinates[30][0] = 275;
        yellowCoordinates[30][1] = 416;

        yellowCoordinates[31][0] = 229;
        yellowCoordinates[31][1] = 369;

        yellowCoordinates[32][0] = 183;
        yellowCoordinates[32][1] = 369;

        yellowCoordinates[33][0] = 137;
        yellowCoordinates[33][1] = 369;

        yellowCoordinates[34][0] = 91;
        yellowCoordinates[34][1] = 369;

        yellowCoordinates[35][0] = 45;
        yellowCoordinates[35][1] = 369;

        yellowCoordinates[36][0] = 0;
        yellowCoordinates[36][1] = 369;

        yellowCoordinates[37][0] = 0;
        yellowCoordinates[37][1] = 323;


        yellowCoordinates[38][0] = 46;
        yellowCoordinates[38][1] = 323;



        yellowCoordinates[39][0] = 46;
        yellowCoordinates[39][1] = 277;

        yellowCoordinates[40][0] = 92;
        yellowCoordinates[40][1] = 277;

        yellowCoordinates[41][0] = 138;
        yellowCoordinates[41][1] = 277;

        yellowCoordinates[42][0] = 184;
        yellowCoordinates[42][1] = 277;


        yellowCoordinates[43][0] = 230;
        yellowCoordinates[43][1] = 277;

        yellowCoordinates[44][0] = 276;
        yellowCoordinates[44][1] = 231;

        yellowCoordinates[45][0] = 276;
        yellowCoordinates[45][1] = 185;

        yellowCoordinates[46][0] = 276;
        yellowCoordinates[46][1] = 139;

        yellowCoordinates[47][0] = 276;
        yellowCoordinates[47][1] = 93;

        yellowCoordinates[48][0] = 276;
        yellowCoordinates[48][1] = 47;


        yellowCoordinates[49][0] = 276;
        yellowCoordinates[49][1] = 0;

        yellowCoordinates[50][0] = 322;
        yellowCoordinates[50][1] = 0;


        yellowCoordinates[51][0] = 322;
        yellowCoordinates[51][1] = 46;

        yellowCoordinates[52][0] = 322;
        yellowCoordinates[52][1] = 92;

        yellowCoordinates[53][0] = 322;
        yellowCoordinates[53][1] = 138;

        yellowCoordinates[54][0] = 322;
        yellowCoordinates[54][1] = 184;

        yellowCoordinates[55][0] = 322;
        yellowCoordinates[55][1] = 230;

        yellowCoordinates[56][0] = 322;
        yellowCoordinates[56][1] = 276;



    //Blue

        blueCoordinates[0][0] = 598;
        blueCoordinates[0][1] = 368;

        blueCoordinates[1][0] = 552;
        blueCoordinates[1][1] = 368;

        blueCoordinates[2][0] = 506;
        blueCoordinates[2][1] = 368;

        blueCoordinates[3][0] = 460;
        blueCoordinates[3][1] = 368;

        blueCoordinates[4][0] = 414;
        blueCoordinates[4][1] = 368;

        blueCoordinates[5][0] = 368;
        blueCoordinates[5][1] = 414;

        blueCoordinates[6][0] = 368;
        blueCoordinates[6][1] = 460;

        blueCoordinates[7][0] = 368;
        blueCoordinates[7][1] = 506;

        blueCoordinates[8][0] = 368;
        blueCoordinates[8][1] = 552;

        blueCoordinates[9][0] = 368;
        blueCoordinates[9][1] = 598;

        blueCoordinates[10][0] = 368;
        blueCoordinates[10][1] = 644;


        blueCoordinates[11][0] = 322;
        blueCoordinates[11][1] = 644;

        blueCoordinates[12][0] = 275;
        blueCoordinates[12][1] = 644;

        blueCoordinates[13][0] = 275;
        blueCoordinates[13][1] = 600;

        blueCoordinates[14][0] = 275;
        blueCoordinates[14][1] = 554;

        blueCoordinates[15][0] = 275;
        blueCoordinates[15][1] = 508;

        blueCoordinates[16][0] = 275;
        blueCoordinates[16][1] = 462;

        blueCoordinates[17][0] = 275;
        blueCoordinates[17][1] = 416;

        blueCoordinates[18][0] = 229;
        blueCoordinates[18][1] = 369;

        blueCoordinates[19][0] = 183;
        blueCoordinates[19][1] = 369;

        blueCoordinates[20][0] = 137;
        blueCoordinates[20][1] = 369;

        blueCoordinates[21][0] = 91;
        blueCoordinates[21][1] = 369;

        blueCoordinates[22][0] = 45;
        blueCoordinates[22][1] = 369;

        blueCoordinates[23][0] = 0;
        blueCoordinates[23][1] = 369;

        blueCoordinates[24][0] = 0;
        blueCoordinates[24][1] = 323;


        blueCoordinates[25][0] = 0;
        blueCoordinates[25][1] = 277;

        blueCoordinates[26][0] = 46;
        blueCoordinates[26][1] = 277;

        blueCoordinates[27][0] = 92;
        blueCoordinates[27][1] = 277;

        blueCoordinates[28][0] = 138;
        blueCoordinates[28][1] = 277;

        blueCoordinates[29][0] = 184;
        blueCoordinates[29][1] = 277;


        blueCoordinates[30][0] = 230;
        blueCoordinates[30][1] = 277;

        blueCoordinates[31][0] = 276;
        blueCoordinates[31][1] = 231;

        blueCoordinates[32][0] = 276;
        blueCoordinates[32][1] = 185;

        blueCoordinates[33][0] = 276;
        blueCoordinates[33][1] = 139;

        blueCoordinates[34][0] = 276;
        blueCoordinates[34][1] = 93;

        blueCoordinates[35][0] = 276;
        blueCoordinates[35][1] = 47;


        blueCoordinates[36][0] = 276;
        blueCoordinates[36][1] = 0;
    
        blueCoordinates[37][0] = 322;
        blueCoordinates[37][1] = 0;

        blueCoordinates[38][0] = 368;
        blueCoordinates[38][1] = 0;


        blueCoordinates[39][0] = 368;
        blueCoordinates[39][1] = 46;

        blueCoordinates[40][0] = 368;
        blueCoordinates[40][1] = 92;

        blueCoordinates[41][0] = 368;
        blueCoordinates[41][1] = 138;

        blueCoordinates[42][0] = 368;
        blueCoordinates[42][1] = 184;

        blueCoordinates[43][0] = 368;
        blueCoordinates[43][1] = 230;


        blueCoordinates[44][0] = 414;
        blueCoordinates[44][1] = 276;

        blueCoordinates[45][0] = 460;
        blueCoordinates[45][1] = 276;


        blueCoordinates[46][0] = 506;
        blueCoordinates[46][1] = 276;

        blueCoordinates[47][0] = 552;
        blueCoordinates[47][1] = 276;

        blueCoordinates[48][0] = 598;
        blueCoordinates[48][1] = 276;

        blueCoordinates[49][0] = 644;
        blueCoordinates[49][1] = 276;


        blueCoordinates[50][0] = 644;
        blueCoordinates[50][1] = 322;

        blueCoordinates[51][0] = 598;
        blueCoordinates[51][1] = 322;

        blueCoordinates[52][0] = 552;
        blueCoordinates[52][1] = 322;

        blueCoordinates[53][0] = 506;
        blueCoordinates[53][1] = 322;

        blueCoordinates[54][0] = 460;
        blueCoordinates[54][1] = 322;

        blueCoordinates[55][0] = 414;
        blueCoordinates[55][1] = 322;

        blueCoordinates[56][0] = 368;
        blueCoordinates[56][1] = 322;

    }

    //CREATING THE GAME WINDOW
    void init_WINDOW() {
        video_MODE.height = 900;
        video_MODE.width = 1100;
        window = new sf::RenderWindow (video_MODE, "Ludo", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

    }

    //DRAWING LUDO BOARD
    void init_BOARD() {                
        //board
        sf::Image board;
        if (!board.loadFromFile("abc.jpg"))
            std::cout<<"ERROR"<<std::endl;
        Boardtexture.loadFromImage(board);
        BoardSprite.setTexture(Boardtexture);
    }

    //Constructors and destructor
    Game() {
        init_VARIABLES();
        init_WINDOW();
    }
    
    virtual ~Game() {

    }

    //WILL RETURN AND TELL GAME WINDOW STATUS
    bool OPENWINDOW() {
        return window->isOpen();
    }

    //TO DRAW GOTIYAN
    void GOTIYAN() {

        DICE_OBJECT.draw_DICE();
        window->draw(BoardSprite);
        window->draw(DICE_Sprite);
        window->draw(golaSprite);


        //RED
        if (!red.loadFromFile("red.png"))
            cout<<"ERROR"<<endl;

        redTexture.loadFromImage(red);

        for(int i = 0; i< 4; i++)
            redSprite[i].setTexture(redTexture); 

        for(int i = 0; i< 4; i++) {
            window->draw(redSprite[i]);
        }

        //BLUE
        if (!blue.loadFromFile("blue.png"))
            cout<<"ERROR"<<endl;

        blueTexture.loadFromImage(blue);

        for(int i = 0; i< 4; i++)
            blueSprite[i].setTexture(blueTexture); 

        for(int i = 0; i< 4; i++) {
            window->draw(blueSprite[i]);
        }


        //GREEN
        if (!green.loadFromFile("green.png"))
            cout<<"ERROR"<<endl;

        greenTexture.loadFromImage(green);

        for(int i = 0; i< 4; i++)
            greenSprite[i].setTexture(greenTexture); 

        for(int i = 0; i< 4; i++) {
            window->draw(greenSprite[i]);
        }


        //YELLOW
        if (!yellow.loadFromFile("yellow.png"))
            cout<<"ERROR"<<endl;

        yellowTexture.loadFromImage(yellow);

        for(int i = 0; i< 4; i++)
            yellowSprite[i].setTexture(yellowTexture); 

        for(int i = 0; i< 4; i++) {
            window->draw(yellowSprite[i]);
        }

    }

    //EVENT INFORMATION FROM HERE
    void EVENT_HAPPENING() {

        init_BOARD(); //DRAWING BOARD

        DICE_OBJECT.draw_DICE();  //DRAWING DICE


        int* temp = new int;
        while (window->pollEvent(event1)) {

            switch (event1.type)
            {
                //CASE FOR WHEN X IS PRESSED 
                //WINDOW CLOSES AND GAME TERMINATES
                case sf::Event::Closed:
                    window->close();
                    break;
                //CASE FOR WHEN ESC KEY IS PRESSED
                //WINDOW CLOSES AND GAME TERMINATES    
                case sf::Event::KeyPressed:
                    if(event1.key.code == Keyboard::Escape) {
                        window->close();
                        break; 
                    }       
                //CASE FOR WHEN MOUSE LEFT IS PRESSED 
                //THIS IS BASICALLY FOR THE PLAYER TO ROLL DICE
                //SEMAPHORE IS INCREMENTED HERE WHICH ALLOWS ONE OF THE PLAYER THREAD TO ACCESS THE CRITICAL PORTION OF TOKEN MOVEMENT FUNCTION 
                //LASTLY KILLER THREAD IS CREATED WHICH TRACKS ALL THE COORDINATES AND IF CURRENT PLAYERS COORDINATES MATCH WITH ANY OTHER, 
                //IT KILLS THEM BY SENDING IT HOME AND REDUCING SCORE TO 0 AGAIN    
                case sf::Event::MouseButtonPressed:
                    if (event1.mouseButton.button == sf::Mouse::Left) {
                        DICE_OBJECT.DICE_VALUE = RANDOMIZER();
                        DICE_OBJECT.draw_GOLLA();
                        sem_post(&PLAYER_SEMA);
                        *temp = COLOR_CHECKER;
                        pthread_create(&KILLER_THREAD, NULL, KILLER, temp);
                    }    
                    break;
            }
        }
    }

    //update whatever movement or new happening happens in game
    void UPDATE() {
        EVENT_HAPPENING();
    }

    //drawing or visualization all the stuff
    void RENDER() {
        window->clear(sf::Color(128, 128, 128, 40));
        GOTIYAN();
        DICE_OBJECT.SCORE();
        window->display();
    }

};


int main() {
    srand(time(NULL));
    
    //INITALIZING THE PLAYER AND DICE SEMAPHORES TO 0
    //THESE WILL BE INCREMENTED LATER TO 1 IN EVENT HAPPENING FUNCTION
    //AND WILL ALLOW THE THREADS TO ACCESS CRITICAL PORTIONS
    sem_init (&PLAYER_SEMA, 0, 0);
    sem_init (&DICE_SEMA, 0, 0);

//    cout<<"enter player num"<< endl;

    PLAYER_NUM = 4;
    TURN_COUNTER = 0;

    Game GAME_OBJECT;

    //ADDING BACKGROUND MUSIC TO GAME    
    music.openFromFile("sound.wav");
        music.play();
    music.setLoop(true);


    //MAIN GAME LOOP 
    while (GAME_OBJECT.OPENWINDOW()) {
        if (TURN_COUNTER % PLAYER_NUM == 0) {
            player_thread_maker();
        }
        //UPDATE GAME 
        GAME_OBJECT.UPDATE();
        //RENDER GAME
        GAME_OBJECT.RENDER();

    }

    //JOINING THE PLAYER THREADS 
    for(int m=0; m < PLAYER_NUM; m++)
        pthread_join(player_thread[m], NULL);

    pthread_exit(NULL);
    return 0;
}
