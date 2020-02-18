#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <time.h>
#include <ctime>
#include <cmath>
#include <stdio.h>
#include <fstream>
#include <string>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32
#define ENTER 13
#define ESC 27
#define MENUARROW 175
#define BOSS_HP 176
#define ENEMY 35

using namespace std ; /// abs spwan base




int mainEngineCode(Save *save) ; /// Game's engine code. Send address of score as parameter.

const bool operator ==(const COORD &c1, const COORD &c2) ; /// Compare between 2 COORD struct.

void banner(); /// Display game banner(X-WING FORCE).
void printWall() ; /// Display wall in the game. Called by mainEngineCode.

int mainmenu(int x, int y) ; /// Menu's engine code, returns choice back.
void printmenu(int x, int y) ; /// Display menu, called by mainmenu.
void information(int x,int y);
bool quitEsc() ; /// Quit game using ESC function.
int wingSelect() ; /// Select fleet type.
void showResult(int result, Save save) ;

int saveInFile(char filename[],Save *save); /// Save data into file.
bool loadFile(Save *save) ; /// Load data from file.
bool loadCheck(Save save) ; /// Ask user if they want to load.
void setHighScore(Save save) ;
void viewHighScore() ;

void loadAnimation(); /// Display loading animation.

void gotoxy(int x,int y); /// Set cursor position by using 2 dimension COORD system.
void setcolor(int fg,int bg); /// Set texts, background color.

void setcursor(int visible); /// Make cursor visible/invisible.

int main()
{
    Save save ;
    int result ;
    char filename[10] ;
    int choice ;
    gotoxy(35, 10) ;
    cout << "  Please enter your name : " ;
    cin >> filename ;
    strcpy(save.filename, filename) ;
    setcursor(0) ;

    if(loadCheck(save))
    {
        loadFile(&save) ;
    }
    while(TRUE)
    {
        system("CLS") ;
        choice = mainmenu(10,2) ;
        if(choice == 1)
        {
            result = mainEngineCode(&save) ;
        }
        else if(choice == 4)
            break ;

        showResult(result, save) ;

    }
    system("CLS") ;
    return 0;
}

/// ALGORITHMS PART.
int mainEngineCode(Save *save)
{
    char control ; /// Input from user, stores fleet's moves.
    int score = 0 ;

    /// Declare Enemies part.
    Enemy e[5] ; /// Declare 5 Enemies with array for using loops easily.
    int enemy_randx ; /// Random enemies spawn (x)position.
    int enemy_amount = 0 ; /// Amount of enemies at the present.

    /// Declare Wings part.
    int wing_type = -1 ;
    X_Wing x ;
    Y_Wing y ;
    A_Wing a ;
    Fleet *wing ; /// Polymorphism, base class pointer.

    Laser laser[5] ; /// Declare 5 Lasers with arrays for using loops easily.

    /// Declare Missile part.
    Missile missile ;
    int missile_cooltime = 1 ; /// Time counter for reset missile cooldown.

    /// Declare Boss part.
    Boss boss ;
    int rand_RL ; /// Boss's random direction.
    int rand_sucess = 1 ; ///Boss's random flag.

    /// Protection variables.
    int protection = 0 ;
    int protection_cooltime = 0 ; /// cooltime counter.

    /// Timer variables.
    clock_t begin ;
    clock_t save_begin = clock();
    clock_t enemy_begin = clock();
    clock_t laser_begin = clock();
    clock_t wing_begin = clock();
    clock_t firerate_begin = clock();
    clock_t protection_begin = clock();
    clock_t boss_begin = clock();
    clock_t missile_begin = clock();
    clock_t missile_cooldown_begin = clock();

    /// Pointing derived class.
    wing_type = save->wing_type ; /// Check wingtype from save first.
    if(wing_type == -1)
    {
        system("CLS") ;
        wing_type = wingSelect() ;
    }
    switch(wing_type)
    {
        case 0 : wing = &a ; break ;
        case 1 : wing = &x ; break ;
        case 2 : wing = &y ; break ;
    }

    /// Loading data.
    if(save->loaded)
    {
        wing->setCOORD(save->wing.getCOORD().X, save->wing.getCOORD().Y) ;
        wing->plusHp(-(3 - save->wing.getHp())) ;
        score = save->score ;
        enemy_amount = save->enemy_amount ;
        boss.hp = save->boss.hp ;
    }

    /// Preparing things.
    srand(time(NULL)) ;

    wing->setCOORD(37, 15) ;
    boss.setCOORD(50,0) ;
    system("CLS") ;
    printWall() ;
    wing->spawn() ;

    gotoxy(85, 2) ;
    setcolor(12,0);
    cout << "Score         : " << score ;
    setcolor(7,0);

    /// START GAME LOOPS.
    while (TRUE)
    {
        begin = clock(); /// Set base timer.
        /// MOVES CONTROL PART.
        if(begin - wing_begin >= wing->getSpeed()) /// Speed is depend on wing's type.
        {
            if(kbhit()) /// Do this part if keyboard is hitted.
            {
                control = getch() ; /// The key is one that was pressed to enter this part.
                wing->clearFleet() ; /// Clear wing's old position.
                if(control == LEFT && wing->getCOORD().X >= 4)
                {
                    wing->plusCOORD(-1, 0) ;
                    wing->spawn();
                }
                else if(control == RIGHT && wing->getCOORD().X + wing->getSizeConst().X <= 75)
                {
                    wing->plusCOORD(1, 0) ;
                    wing->spawn() ;
                }
                else if(control == UP && wing->getCOORD().Y > 0)
                {
                    wing->plusCOORD(0, -1) ;
                    wing->spawn() ;
                }
                else if(control == DOWN && wing->getCOORD().Y + wing->getSizeConst().Y < 28)
                {
                    wing->plusCOORD(0, 1) ;
                    wing->spawn() ;
                }
                else if(control == SPACE) /// Fire laser by pressing SPACEBAR.
                {
                    if(begin - firerate_begin >= wing->getFireRate()) /// Fire rate speed control.
                    {
                        for(int i = 0 ; i < 5 ; i ++ ) /// Finding avaliable laser.
                        {
                            if(laser[i].status == 0) /// Check avaliable laser.
                            {
                                laser[i].status = 1 ; /// Queue for firing animation.
                                laser[i].setCOORD(wing->getCOORD().X + wing->getFireConst(),
                                                  wing->getCOORD().Y ) ; /// Set laser position using wing's position
                                                                /// to match the gun (position x at the present + const of each wing)
                                break ; /// Finding laser sucess, then brake the loop.
                            }
                        }
                    firerate_begin = clock() ; /// Set new firerate timer.
                    }
                }
                else if(control == 'm') /// Fire Missile by pressing 'm'.
                {
                    if(missile.status == 0 && missile.cooldown == 1) /// Activate if Missile isn't fired at the present (status = 0)
                    {
                        for (int i = 0 ; i < 5 ; i++)
                        {
                            gotoxy(100,i+5) ;
                            cout << " " ;
                        }                                /// and cooltime is end (cooldown = 1).
                        missile.status = 1 ; /// Queue for firing animation.
                        missile.cooldown = 0 ; /// Prevent firing 2 Missiles.
                        missile.setCOORD(wing->getCOORD().X + 3, wing->getCOORD().Y ) ; /// Set Missile position(same as Laser).
                        missile_cooltime = 0 ;  /// Reset cooltime.
                    }
                }
                else if(control == ESC )
                {
                    if (quitEsc())
                    {
                        save->loaded = 1 ;
                        return -1 ;
                    }
                }
            }
            wing_begin = clock() ; /// Set new wing control timer.
        }
        /// Clear keyboard buffer for clearing excess key.

        /// WING INVINCIBLE COOLDOWN PART.
        if(protection == 1)  /// if wing is in invincible mode.
        {
            wing->clearFleet() ; /// Make wing become winking by clear and respawn(at end of loop).
            if(begin - protection_begin >= 1000) /// Timer for invincible cooldown.
            {
                protection_cooltime++ ; /// Counter.
                if(protection_cooltime == 3) /// 1000 ms * 3 = 3 sec in invincible mode.
                {
                     protection = 0 ; /// Become no more invincible.
                     protection_cooltime = 0 ; /// Reset cooltime.
                }
                protection_begin = clock() ; /// Set new timer.
            }
        }

        /// ENEMIES SPAWNING PART.
        if(begin - enemy_begin >= 1000 ) /// Set enemies speed = 1s
        {
            for(int i = 0 ; i < 5 ; i++) /// To process all enemies.
            {
                e[i].clearEnemy() ; /// Clear previous enemy.
                if (e[i].status == 1 ) /// Make enemy falls if enemy is still alive (status = 1)
                {
                    e[i].plusCOORD(0,1) ; /// Plus 1 to Y COORD for falling animation.
                     gotoxy(e[i].getCOORD().X,e[i].getCOORD().Y);
                     e[i].spawn() ;
                }
                if(e[i].getCOORD().Y >= 27) /// If enemy hit the ground.
                {
                    e[i].clearEnemy() ;
                    e[i].status = 0 ; /// Stop their falling animation(first if condition)
                    e[i].setCOORD(0, 0) ; /// Set them back to origin to prevent unexpected bugs.
                }
                if (enemy_amount < 12 && e[i].status == 0) /// Control enemies amount / Spawning part.
                {
                    enemy_randx = (rand() + clock()) % 60 + 7 ; /// Random spawnpoint.
                    e[i].setCOORD(enemy_randx, 1) ;
                    gotoxy(e[i].getCOORD().X,e[i].getCOORD().Y);
                    e[i].spawn() ;
                    e[i].status = 1 ; /// Allow them for moving (first if condition).
                    enemy_amount ++ ; /// Amount counter.
                }
            }
            enemy_begin = clock(); /// Set new timer.
        }

        /// LASER ANIMATION PART.
        for (int i = 0 ; i < 5 ; i ++) /// To process all Lasers.
        {

            /// If encounter the enemy.
            for(int j = 0 ; j < 5 ; j++) /// Nested loops processing all of enemies.
            {
                if(laser[i].isEncounter(e[j]))
                {
                    if(e[j].status == 1)
                    {
                        /// Disappear the Laser.
                    laser[i].status = 0 ;
                    laser[i].clearLaser() ;

                    /// Disappear the enemy.
                    e[j].status = 0 ;
                    e[j].clearEnemy() ;
                    e[j].setCOORD(0,0);

                    score += 5 ;
                    }
                }
            }

            if(begin - laser_begin >= 100) /// Laser animation speed.
            {
                if(laser[i].status == 1) /// Only allowed lasers(from moving part) can do this part.
                {
                    /// Make Laser animation.
                    laser[i].clearLaser() ;
                    laser[i].plusCOORD(0,-1) ;
                    gotoxy(laser[i].getCOORD().X, laser[i].getCOORD().Y);
                    laser[i].spawn();

                    if(laser[i].getCOORD().Y == 0) /// If laser reach the ceiling.
                    {
                        laser[i].clearLaser() ;
                        laser[i].status = 0 ;
                    }
                    if(boss.status == 1 && boss.getCOORD().X <= laser[i].getCOORD().X && boss.getCOORD().X+11 >= laser[i].getCOORD().X
                       && boss.getCOORD().Y + 4 >= laser[i].getCOORD().Y && boss.getCOORD().Y <= laser[i].getCOORD().Y)
                       /// If laser encounter boss.
                        {
                            boss.hp -- ;
                            laser[i].clearLaser() ;
                            laser[i].status = 0 ;
                        }
                }
                if(i == 4) /// If all of Lasers is calculated.
                    laser_begin = clock(); /// Set new timer.
            }
        }

        /// MISSILE ANIMATION PART.
        if(wing->getSpecial() == 1) /// Only special wing can acess this part.
             {

                gotoxy(85,6);
                setcolor(13,0);cout << "MISSILE GAUGE :" ;setcolor(7,0);

                if(begin - missile_cooldown_begin >= 1000) /// Cooltime timer.
                {
                    for (int i = 0 ; i < missile_cooltime ; i++)
                    {
                        gotoxy(i+101,6);
                        setcolor(13,0);cout << "I" ;setcolor(7,0);
                    }
                    if(missile_cooltime <5)
                        missile_cooltime ++ ;
                    else if(missile_cooltime == 5) /// Overall time 1000ms * 5 = 5s as Missile cooltime.
                        missile.cooldown = 1 ; /// Ready to fire.

                    missile_cooldown_begin = clock() ;

                }

                if(begin - missile_begin >= 50) /// Missile animation speed.
                {
                    if(missile.status == 1)
                    {
                        missile.clearLaser() ;
                        missile.plusCOORD(0,-1) ;
                        gotoxy(missile.getCOORD().X, missile.getCOORD().Y);
                        missile.spawn();
                        for(int j = 0 ; j < 5 ; j++)
                        {
                            if(missile.isEncounter(e[j]))
                            {
                                for(int k = 0 ; k < 5 ; k++)
                                {
                                    if(e[k].getCOORD().X >= missile.getCOORD().X-3 && e[k].getCOORD().X <= missile.getCOORD().X+3
                                       && e[k].getCOORD().Y >= missile.getCOORD().Y-2 && e[k].getCOORD().Y <= missile.getCOORD().Y+2)
                                       /// Blowing radius is 7*4 .
                                    {
                                        score += 5 ;
                                        e[k].status = 0 ;
                                        e[k].clearEnemy() ;
                                        e[k].setCOORD(0,0);
                                    }
                                }
                                missile.status = 0 ;
                                missile.clearLaser() ;
                                break ;
                            }
                        }
                        if(missile.getCOORD().Y == 0)
                        {
                            missile.clearLaser() ;
                            missile.status = 0 ;
                        }
                        if(boss.status == 1 && boss.getCOORD().X <= missile.getCOORD().X && boss.getCOORD().X+10 >= missile.getCOORD().X
                        && boss.getCOORD().Y + 4 >= missile.getCOORD().Y && boss.getCOORD().Y <= missile.getCOORD().Y )
                        {
                            boss.hp -= 5 ;
                            missile.clearLaser() ;
                            missile.status = 0 ;
                        }
                    }
                    missile_begin = clock();
                }
            }

        /// IF WING ENCOUNTERS ENEMIES PART.
        for (int i = 0 ; i < 5 ; i++)
        {
            if(wing->isEnemyHit(e[i]))
            {
                e[i].status = 0 ;
                e[i].clearEnemy() ;
                e[i].setCOORD(0, 0) ;
                if(protection == 0) /// If Wing isn't in an invincible mode.
                {
                    wing->plusHp(-1);
                    protection = 1 ; /// Activate invincible mode.
                }
            }
        }

        /// BOSS SPAWNING PART.
        if(enemy_amount == 12) /// Boss appears condition.
         {
            boss.status = 1 ;
            if(begin - boss_begin>= 250) /// Boss speed.
            {
                boss.clearBoss() ;
                if(rand_sucess == 1) /// Random Boss's X,Y position.
                {
                    /// Random Algorithms.
                    rand_RL = rand() % 2 ;
                    boss.random_coord.X += pow(-1, rand_RL)*(rand() % 20 ) ;
                    boss.random_coord.Y =  (rand() % 22 ) ;
                    if(boss.random_coord.X > 68 )
                        boss.random_coord.X = 68 ;
                    else if(boss.random_coord.X < 4 )
                        boss.random_coord.X = 4 ;
                    if(boss.random_coord.Y > 23 )
                        boss.random_coord.Y = 23 ;
                    else if(boss.random_coord.Y < 0 )
                        boss.random_coord.Y = 0 ;
                    rand_sucess = 0 ;
                }
                /// Move Boss to the position.
                if(boss.getCOORD().Y > boss.random_coord.Y)
                {
                    boss.plusCOORD(0,-1) ;
                }
                else if(boss.getCOORD().Y < boss.random_coord.Y)
                {
                    boss.plusCOORD(0,1) ;
                }
                if(boss.getCOORD().X < boss.random_coord.X)
                {
                    boss.plusCOORD(1,0) ;
                }
                else if(boss.getCOORD().X > boss.random_coord.X)
                {
                    boss.plusCOORD(-1,0) ;
                }
                if(boss.getCOORD() == boss.random_coord || boss.getCOORD().X == boss.random_coord.X)//|| boss.getCOORD().X == boss.random_coord.X || boss.getCOORD().Y == boss.random_coord.Y)
                {
                    rand_sucess = 1 ;;
                }
                boss.spawn() ;

                if(wing->isBossHit(boss) && protection == 0)
                {
                    wing -> plusHp(-1) ;
                    protection = 1 ;
                }
                boss_begin = clock() ;
            }
        }
        /// Redisplay game status.
        gotoxy(85, 2) ;
        setcolor(12,0);
        cout << "Score         : " << score ;
        setcolor(7,0);
        if(boss.status == 1)
        {
            gotoxy(100, 7) ;
            cout << "boss HP "  ;
            for(int i = 0 ; i < 30-(boss.hp/2)*2 ; i++)
            {
                gotoxy(116 - i, 8) ;
                cout << "  " ;
            }
            gotoxy(86 + (boss.hp/2)*2, 8) ;
                cout << "  " ;
            gotoxy(86, 8) ;
            for(int i = 0 ; i < boss.hp ; i+=2)
                cout << static_cast<char>(BOSS_HP) ;
        }
        gotoxy(85, 3) ;setcolor(14,0);
        cout << "HP            : " << wing->getHp() ;setcolor(7,0);
        gotoxy(85, 4) ;setcolor(10,0);
        cout << "Wing position : " << wing->getCOORD().X << ", " << wing->getCOORD().Y ;
        setcolor(7,0);
        gotoxy(85, 5) ;setcolor(11,0);
        cout << "Enemy amount  : " << enemy_amount ;
        setcolor(7,0) ;
        wing->spawn() ;

        /// AUTO SAVE PART.
        if(begin - save_begin>= 1000) /// Save every 1s.
        {
            save->wing = *wing ;
            save->enemy_amount = enemy_amount ;
            save->boss = boss ;
            save->score = score ;
            saveInFile(save->filename, save) ;
            save_begin = clock() ;
        }

        /// END CONDITIONS PART.
        if(wing->getHp() <= 0 )
        {
            save->loaded = 0 ;
            return 0 ;
        }
        else if(boss.hp <= 0)
        {
            save->loaded = 0 ;
            return 1 ;
        }
    }
}


const bool operator ==(const COORD &c1, const COORD &c2)
{
    if (c1.X == c2.X && c1.Y == c2.Y)
        return TRUE ;
    else
        return FALSE ;
}
int mainmenu(int x, int y)
{

    char selarrow ;
    int sel = 1;
    while(1)
    {   gotoxy(15,15); banner();
        printmenu(x+65,y-1);
        gotoxy(x+64, sel*2+y+1);
        setcolor(14,0);
        cout << static_cast<char>(MENUARROW) ;
        setcolor(7,0);
        do
        {
            selarrow = _getch();
            if(selarrow == UP&&sel>1)
            {
                gotoxy(x+64,(sel)*2+y+1);
                cout << " " ;
                sel--;
                gotoxy(x+64,(sel*2)+y+1);
                setcolor(14,0);
                cout << static_cast<char>(MENUARROW) ;
                setcolor(7,0);
            }
            if(selarrow == DOWN&&sel<4)
            {
                gotoxy(x+64,(sel)*2+y+1);
                cout << " " ;
                sel++;
                gotoxy(x+64,(sel*2)+y+1);
                setcolor(14,0);
                cout << static_cast<char>(MENUARROW) ;
                setcolor(7,0);
            }
        }while(selarrow != ENTER);


        switch(sel)
        {
            case 1 : return 1 ;
            case 2 : information(x,y) ; break ;
            case 3 : viewHighScore() ; break ;
            case 4 : return 4 ;
        }
    }
}
int saveInFile(char filename[],Save *save)
{
    ofstream fout(filename);
    if (! fout)
    {
        cout << filename << " could not be opened." << endl ;
        return -1;
    }
    fout << 1 << endl ;
    fout << save->wing.getFleetType() << endl ;
    fout << save->wing.getCOORD().X << endl ;
    fout << save->wing.getCOORD().Y << endl ;
    fout << save->wing.getHp() << endl ;
    fout << save->score << endl ;
    fout << save->enemy_amount << endl ;
    fout << save->boss.hp << endl ;

    fout.close();
    return 0 ;
}
bool loadFile(Save *save)
{
    char input_line[10];
    int temp ;
    COORD ctemp ;
    system("CLS") ;
    gotoxy(50,6) ;
    ifstream file_in(save->filename);
    if (! file_in)
    {
        return 0 ;
    }
    for (int i = 0; ! file_in.eof() ; i++)
    {
        file_in.getline(input_line, 10);
        temp = atoi(input_line) ;
        switch(i)
        {
            case 1 : save->wing_type = temp ; break ;
            case 2 : ctemp.X = temp ; break ;
            case 3 : ctemp.Y = temp ; break ;
            case 4 : save->wing.plusHp(-(3-temp)) ; break ;
            case 5 : save->score = temp ; break ;
            case 6 : save->enemy_amount = temp ; break ;
            case 7 : save->boss.hp = temp ; break ;
        }
    }
    save->wing.setCOORD(ctemp.X, ctemp.Y) ;
    save->loaded = 1 ;
    return 1 ;
}
bool loadCheck(Save save)
{
    int sel = 1 ;
    char selarrow ;
    char input_line[10];
    ifstream file_in(save.filename);

    file_in.getline(input_line, 2);
    if(atoi(input_line) != 1)
        return 0 ;

    system("CLS") ;
    gotoxy(40, 9) ;
    cout << "Welcome back, " << save.filename << ". " ;
    gotoxy(35, 10) ;
    cout << "Do you want to use your old data?" ;
    gotoxy(36, 14) ;
    cout << "Yes" ;
    gotoxy(36, 15) ;
    cout << "No" ;
    gotoxy(35, 14) ;
    cout << ">" ;
    for (int i = 0; ! file_in.eof() ; i++)
    {
        file_in.getline(input_line, 3);
        switch(i)
        {
            case 0 : gotoxy(60, 13+i) ; cout << "Fleet type : " << input_line ; break ;
            case 3 : gotoxy(60, 11+i) ; cout << "HP left    : " << input_line ; break ;
            case 4 : gotoxy(60, 11+i) ; cout << "Score      : " << input_line ; break ;
            case 5 : gotoxy(60, 11+i) ; cout << "Enemy      : " << input_line ; break ;
            case 6 : gotoxy(60, 11+i) ; cout << "Boss HP    : " << input_line ; break ;
        }
    }
    do
    {
        selarrow = _getch();
        if(selarrow == UP && sel == 0)
        {
            sel = 1 ;
            gotoxy(35, 15) ;
            cout << " " ;
            gotoxy(35, 14) ;
            cout << ">" ;
        }
        if(selarrow == DOWN && sel == 1)
        {
            sel = 0 ;
            gotoxy(35, 14) ;
            cout << " " ;
            gotoxy(35, 15) ;
            cout << ">" ;
        }
    }while(selarrow != ENTER);
    system("CLS") ;
    if(sel == 1)
        return 1 ;
    else
        return 0 ;
}
void setHighScore(Save save)
{
    char filename[10] = "HighScore" ;
    time_t t = time(NULL);
    ofstream fout ;
    fout.open(filename, ios_base::app);
    if (! fout)
    {
        cout << filename << " could not be opened." << endl ;
        return ;
    }

    fout << "Player name : " << save.filename << endl ;
    fout << "Score : " << save.score << endl ;
    fout << "Live left : " << save.wing.getHp() << endl ;
    fout << "Time played : " << ctime(&t) << endl ;
    fout << "----------------------------" << endl ;

    fout.close() ;
}
void viewHighScore()
{
    string input_line;
    char filename[10] = "HighScore" ;
    ifstream file_in(filename);
    system("CLS") ;
    if (! file_in)
    {
        cout << "File could not be opened." << endl ;
        return ;
    }
    while(getline(file_in, input_line))
    {
        cout << input_line << endl ;
    }
    cout << "\aPress any key to go back . . . " ;
    getch() ;
    system("CLS") ;
    cout << "\a" ;
}
bool quitEsc()
{
    bool sel = 1 ;
    char selarrow  ;
    system("CLS") ;
    gotoxy(53, 20) ;
    cout << "Are you sure to want to back to mainmenu ?" ;
    gotoxy(53, 21) ;
    cout << "Yes" ;
    gotoxy(53, 22) ;
    cout << "No" ;
    gotoxy(52, 21) ;
    cout << ">" ;
    do
    {
        selarrow = _getch();
        if(selarrow == UP && sel == 0)
        {
            sel = 1 ;
            gotoxy(52, 22) ;
            cout << " " ;
            gotoxy(52, 21) ;
            cout << ">" ;
        }
        if(selarrow == DOWN && sel == 1)
        {
            sel = 0 ;
            gotoxy(52, 21) ;
            cout << " " ;
            gotoxy(52, 22) ;
            cout << ">" ;
        }
    }while(selarrow != ENTER);
    system("CLS") ;
    return sel ;

}

void loadAnimation()
{
    int i,j;
    system("cls");
    gotoxy(53,11);
    setcolor(11,0);
  //  printf("Now Loading...");
    gotoxy(39,13);
    for(i=1;i<=21;i++)
    {
        for(j=0;j<=100000000;j++); //to display the character slowly
   //     printf("%c ",162);
    }
    gotoxy(46,15);
   // printf("Press any key to continue . . .");
    setcolor(7,0);
    getch();
    system("cls");
}
int wingSelect()
{
    char sel ;
    int page = 0 ;

    Fleet *wing ;
    X_Wing x ;
    Y_Wing y ;
    A_Wing a ;
    printWall();
    gotoxy(30, 13) ;
    setcolor(14,0);
    cout << "Please select your fleet." ;
    setcolor(7,0);
    a.setCOORD(37, 15) ;
    a.spawn() ;

    while(sel != ENTER)
    {
        gotoxy(30, 13) ;
        setcolor(14,0);cout << "Please select your fleet." ;setcolor(7,0);
        sel = getch() ;
        system("CLS") ;
        printWall();
        switch(sel)
        {
            case LEFT : if(page>0)page -- ; break ;
            case RIGHT : if(page<2)page ++ ; break ;
        }
        switch(page)
        {
            case 0 : wing = &a ; break ;
            case 1 : wing = &x ; break ;
            case 2 : wing = &y ; break ;
        }
        wing->setCOORD(37, 15) ;
        wing->spawn() ;
    }
    return page;
}
void printWall()
{
    setcolor(14,0);
    cout << endl << endl <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  *                                                                              *" << endl
                         <<  "  ********************************************************************************" << endl ;
    setcolor(7,0);
}
void printmenu(int x, int y)
{
    setcolor(10,0);
    gotoxy(x,y);
    cout << "                           __"; gotoxy(x,y+1) ;
    cout << "|\\ /|  /\\  || |\\ |  |\\ /| |__ |\\ | |  |" ;  gotoxy(x,y+2) ;
    cout << "| | | /__\\ || | \\|  | | | |__ | \\| |__|" ; gotoxy(x,y+4) ;

    cout << "::   Start game   ::" ; gotoxy(x,y+6) ;
    cout << "::   Information  ::" ; gotoxy(x,y+8) ;
    cout << "::   High score   ::" ; gotoxy(x,y+10) ;
    cout << "::      Exit      ::" ;
    setcolor(7,0);
}
void showResult(int result, Save save)
{
    system("CLS") ;
    if(result == 1)
    {
        gotoxy(34, 10) ;
        cout << "CONGRATULATIONS YOU WIN ! ! ! " ;
        remove(save.filename) ;
        setHighScore(save) ;
        gotoxy(30, 11) ;
        cout << "We're saving your score, press any key to continue . . ." ;
        Sleep(5000) ;
        while(kbhit())
            getch() ;
        viewHighScore() ;
    }
    else if(result == 0)
    {
        gotoxy(35, 10) ;
        cout << "YOU LOSE" ;
        gotoxy(32, 11) ;
        cout << "Press any key to continue . . ." ;
        getch() ;
    }
}
void information(int x, int y)
{
    system("CLS") ;
    int pagenum ;
    char selarrow ;
    X_Wing X ;
    Y_Wing Y ;
    A_Wing A ;

    page_1 :
        pagenum = 1 ;
        gotoxy(x,y);setcolor(14,0);
        cout << "  ___INFORMATION___" ; gotoxy(x,y+2);
        cout << "|================= CONTROL ==================|" ; gotoxy(x,y+3);
        cout << "|         Press 'W' or 'w' to go up.         |" ; gotoxy(x,y+4);
        cout << "|        Press 'S' or 's' to go down.        |" ; gotoxy(x,y+5);
        cout << "|       Press 'A' or 'a' to turn left.       |" ; gotoxy(x,y+6);
        cout << "|       Press 'D' or 'd' to turn right.      |" ; gotoxy(x,y+7);
        cout << "|         Press 'Space bar' to fire.         |" ; gotoxy(x,y+8);
        cout << "|       Press 'D' or 'd' to turn right.      |" ; gotoxy(x,y+9);
        cout << "|============================================|" ; gotoxy(x,y+11);
        cout << "|" << "Page " << pagenum << "/4 " << "------->Please ENTER to RETURN to main menu.";setcolor(7,0);
        selarrow = getch();
        system("cls");
        if(selarrow == RIGHT )
            goto page_2 ;
        else if (selarrow == ENTER )
            return ;
        else
            goto page_1 ;
        system("cls");

    page_2 :
        pagenum = 2 ;
        gotoxy(x,y);setcolor(14,0);
        cout << "  ___INFORMATION___" ; gotoxy(x,y+2);
        cout << "|=================== SHIP ===================|" ; gotoxy(x,y+3);
        cout << " The A-Wing" ;
        A.setCOORD(x, y+4) ;
        A.spawn(); gotoxy(x+15,y+5);setcolor(14,0);
        cout << "Speed     : III" ; gotoxy(x+15,y+6);
        cout << "Fire rate : II " ; gotoxy(x+15,y+7);
        cout << "Special   : -" ; gotoxy(x,y+11);
        cout << "|" << "Page " << pagenum << "/4 " << "------->Please ENTER to RETURN to main menu.";setcolor(7,0);
        selarrow = getch();
        system("cls");
        if(selarrow == RIGHT )
            goto page_3 ;
        else if (selarrow == LEFT )
            goto page_1 ;
        else if (selarrow == ENTER )
            return ;
        else
            goto page_2 ;

    page_3 :
        pagenum = 3 ;
        gotoxy(x,y);setcolor(14,0);
        cout << "  ___INFORMATION___" ; gotoxy(x,y+2);
        cout << "|=================== SHIP ===================|" ; gotoxy(x,y+3);
        cout << " The Y-Wing" ;
        Y.setCOORD(x,y+5) ;
        Y.spawn(); gotoxy(x+15,y+5);setcolor(14,0);
        cout << "Speed     : I" ; gotoxy(x+15,y+6);
        cout << "Fire rate : I " ; gotoxy(x+15,y+7);
        cout << "Special   : Missile" ; gotoxy(x,y+11);
        cout << "|" << "Page " << pagenum << "/4 " << "------->Please ENTER to RETURN to main menu.";setcolor(7,0);
        selarrow = getch();
        system("cls");
        if(selarrow == RIGHT )
            goto page_4 ;
        else if (selarrow == LEFT )
            goto page_2 ;
        else if (selarrow == ENTER )
            return ;
        else
            goto page_3 ;


    page_4 :
        pagenum = 4 ;
        gotoxy(x,y);setcolor(14,0);
        cout << "  ___INFORMATION___" ;gotoxy(x,y+2);
        cout << "|=================== SHIP ===================|" ; gotoxy(x,y+3);
        cout << " The X-Wing" ;
        X.setCOORD(x,y+5) ;
        X.spawn(); gotoxy(x+15,y+5);setcolor(14,0);
        cout << "Speed     : II" ; gotoxy(x+15,y+6);
        cout << "Fire rate : III " ; gotoxy(x+15,y+7);
        cout << "Special   : -" ; gotoxy(x,y+11);
        cout << "|" << "Page " << pagenum << "/4 " << "------->Please ENTER to RETURN to main menu.";setcolor(7,0);
        selarrow = getch();
        system("cls");
        if (selarrow == LEFT )
            goto page_3 ;
        else if (selarrow == ENTER )
            return ;
        else
            goto page_4 ;

}

void banner()
{
    setcolor(14,0);
    gotoxy(2,2);cout<<"XXX== XXX==          XXX==X==XXX== XXXXXXX== XXX== XXX==   XXXXXXX=="<<endl;
    gotoxy(2,3);cout<<" XXX=XXX==           XXX==X==XXX==   XXX==   XXXXX=XXX==  XXX==      "<<endl;
    gotoxy(2,4);cout<<"  XXXXX==   XXXXX==  XXX=XXX=XXX==   XXX==   XXXXXXXXX== XXX== XXXXX==" <<endl;
    gotoxy(2,5);cout<<" XXX=XXX==            XXXX=XXXX==    XXX==   XXX=XXXXX==  XXX== XXX=="<<endl;
    gotoxy(2,6);cout<<"XXX== XXX==            XX== XX==   XXXXXXX== XXX== XXX==   XXXXXXX== "<<endl;
    gotoxy(2,8);cout<<"     XXXXXXXX==   XXXXX==    XXXXXXX==     XXXXXXXX==  XXXXXXX=="<<endl;
    gotoxy(2,9);cout<<"     XXX==      XXX== XXX==  XXX==XXX==   XXX==  XXX== XXX==    "<<endl;
    gotoxy(2,10);cout<<"     XXXXXXX== XXX==   XXX== XXXXXXX==   XXX==         XXXXXXX=="<<endl;
    gotoxy(2,11);cout<<"     XXX==      XXX== XXX==  XXX==XXX==   XXX==  XXX== XXX==    "<<endl;
    gotoxy(2,12);cout<<"     XXX==        XXXXX==    XXX== XXX==   XXXXXXXX==  XXXXXXX=="<<endl;
    setcolor(7,0);
}

/// WINDOWS FUNCTIONS PART.
void gotoxy(int x,int y)
{
    COORD coord;
    coord.X =x;
    coord.Y =y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
void setcursor(int visible)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = 20;
    SetConsoleCursorInfo(console,&lpCursor);
}
void setcolor(int fg,int bg)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, bg*16+fg);
}


