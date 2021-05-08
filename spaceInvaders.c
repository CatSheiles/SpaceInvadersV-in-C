#include "CConsole.h"
#define numberOfAliens 45
#define numberOfBullets 9999

struct Entity{
    int x, y;
    char character;
    int color;
};

typedef struct Entity Entity;                                  //change name of structEntity to just be entity(shorten)

int alienDirection = 1;

void AlienThink(Entity *alien, int width )
{
     //current bug when alien hits right or left wall 1 alien compacts onto another
     //but then eventually grows back- funky stuff happening - leave as work in progress
     //also first version only 1 line of 45 aliens
    if((alien->x + 1 == width && alienDirection == 1) || (alien->x - 1 == -1 && alienDirection == -1))
    {
        alienDirection = -(alienDirection);
        return;
    }

    alien->x += alienDirection;
}

void BulletThink(Entity *bullet, Entity *aliens)
{
    bullet->y --;                                              //-- =up - sending bullets up

    for(int i = 0; i < numberOfAliens; i++)
    {
        if(bullet->y == aliens[i].y && bullet->x == aliens[i].x)
            aliens[i].character = ' ';                         //if bullet hits alien wipe alien out
    }
}

int main()
{
    Console console;                                           //CConsole auto sets size of playing field to be size of command prompt console
    StartConsole(&console);

    Entity aliens[numberOfAliens];                             //coding aliens on the stack
    Entity bullets[numberOfBullets];

    int currentBullet = 0;

    Entity player = {console.bufferWidth /2, console.bufferHeight -2, FULL_PIXEL, LRED};   //full pixel is defined in CConsole code

    for(int i = 0; i < numberOfAliens; i++)
    {
        Entity alien = {i, 1, 146, LGREEN};                    //Add a new alien - 146 is ascii for ae char - C can use ascii only, its an age thing
        aliens[i] = alien;                                     //put alien at location i and keeps running to build all 45 aliens
    }

    int ticks = 0;                                             //speed of player movement

    while(1)
    {
        if(ticks % 40 == 0) {                 
        if(GetAsyncKeyState('D')) player.x++;                  //move player right
        if(GetAsyncKeyState('A')) player.x--;                  //move player left
        }

        if(ticks % 500 == 0){                                  //speed bullets shoot up from player
        if(GetAsyncKeyState(' '))
        {
            Entity bullet = {player.x, player.y-1, 194, YELLOW}; //194 is an ascii kind of capT
            bullets[currentBullet] = bullet;
            currentBullet++;
        }
        }

        if(ticks % 150 == 0)                                   //note C runs so much faster than javascript!! so high tick speed means slower movement
        {
            for(int i = 0; i < numberOfAliens; i++)
            {
                if(aliens[i].character != ' ')
                AlienThink(&aliens[i], console.bufferWidth);
            }
        }

        if(ticks % 75 == 0)                                   //bulet speed - lower number = faster bullets
        {
            for(int i = 0; i < currentBullet; i++)
            {
                BulletThink(&bullets[i], aliens);
            }
        }

        Clear(&console);
        PlaceChar(&console, player.character, player.x, player.y, player.color);  //CConsole PlaceChar is faster than CConsole DrawChar

        for(int i = 0; i < numberOfAliens; i++)
        {
            PlaceChar(&console, aliens[i].character, aliens[i].x, aliens[i].y, aliens[i].color);
        }

        for(int i = 0; i < currentBullet; i++)
        {
            PlaceChar(&console, bullets[i].character, bullets[i].x, bullets[i].y, bullets[i].color);
        }

        Draw(&console);

        ticks++;
    }

    FreeConsoleMemory(&console);
}