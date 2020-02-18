
Y_Wing :: Y_Wing() : Fleet(3,60,800,1,3,3,5,2)
{
}
void Y_Wing :: spawn()
{   setcolor(14,0);
    gotoxy(coord.X,coord.Y);
    cout << "   ++" ; gotoxy(coord.X, coord.Y+1);
    cout << "  <()>" ; gotoxy(coord.X, coord.Y+2);
    cout << "   TT" ; gotoxy(coord.X, coord.Y+3);
    cout << "||=YY=||" ; gotoxy(coord.X, coord.Y+4);
    cout << "||    ||" ;
    setcolor(7,0);
}
void Y_Wing :: clearFleet()
{
    gotoxy(coord.X, coord.Y);   cout << "     " ;
    gotoxy(coord.X, coord.Y+1); cout << "      " ;
    gotoxy(coord.X, coord.Y+2); cout << "     " ;
    gotoxy(coord.X, coord.Y+3); cout << "        " ;
    gotoxy(coord.X, coord.Y+4); cout << "        " ;
}
bool Y_Wing :: isBossHit(Boss boss)
{
    return ((boss.getCOORD().X <= coord.X && boss.getCOORD().X+11 >= coord.X)
            || (boss.getCOORD().X <= coord.X + 7 && boss.getCOORD().X+11 >= coord.X+7))
            && ((boss.getCOORD().Y + 4 >= coord.Y && boss.getCOORD().Y <= coord.Y)
            || (boss.getCOORD().Y + 4 >= coord.Y+4 && boss.getCOORD().Y <= coord.Y+4)) ;
}
bool Y_Wing :: isEnemyHit(Enemy e)
{
    return (coord.X <= e.getCOORD().X && coord.X+7 >= e.getCOORD().X
            && coord.Y <= e.getCOORD().Y && coord.Y+3 >= e.getCOORD().Y) ;
}
