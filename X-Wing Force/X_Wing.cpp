
X_Wing :: X_Wing() : Fleet(3,30,200,0,5,6,4,1)
{
}
void X_Wing :: spawn()
{   setcolor(12,0);
    gotoxy(coord.X,coord.Y);
    cout <<" |   A   |" ; gotoxy(coord.X, coord.Y+1);
    cout <<"||_  H  _||" ; gotoxy(coord.X, coord.Y+2);
    cout <<"|I\\_HXH_/I|" ; gotoxy(coord.X, coord.Y+3);
    cout <<"    \" \"" ;
    setcolor(7,0);
}
void X_Wing :: clearFleet()
{
    gotoxy(coord.X, coord.Y);   cout << "          " ;
    gotoxy(coord.X, coord.Y+1); cout << "           " ;
    gotoxy(coord.X, coord.Y+2); cout << "           " ;
    gotoxy(coord.X, coord.Y+3); cout << "           " ;
}
bool X_Wing :: isBossHit(Boss boss)
{
    return ((boss.getCOORD().X <= coord.X && boss.getCOORD().X+11 >= coord.X)
            || (boss.getCOORD().X <= coord.X + 10 && boss.getCOORD().X+11 >= coord.X+10))
            && ((boss.getCOORD().Y + 5>= coord.Y && boss.getCOORD().Y <= coord.Y)
            || (boss.getCOORD().Y + 5 >= coord.Y+3 && boss.getCOORD().Y <= coord.Y+3)) ;
}
bool X_Wing :: isEnemyHit(Enemy e)
{
    return (coord.X <= e.getCOORD().X && coord.X+10 >= e.getCOORD().X
            && coord.Y <= e.getCOORD().Y && coord.Y+3 >= e.getCOORD().Y) ;
}
