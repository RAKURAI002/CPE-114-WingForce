
A_Wing :: A_Wing() : Fleet(3,10,400,0,3,2,4,0)
{
}
void A_Wing :: spawn()
{   setcolor(11,0);
    gotoxy(coord.X,coord.Y);
    cout << "   _" ; gotoxy(coord.X, coord.Y+1);
    cout << "  / \\" ; gotoxy(coord.X, coord.Y+2);
    cout << " A( )A" ; gotoxy(coord.X, coord.Y+3);
    cout << "|_ - _|" ;
    setcolor(7,0);
}
void A_Wing :: clearFleet()
{
    gotoxy(coord.X, coord.Y);   cout << "     " ;
    gotoxy(coord.X, coord.Y+1); cout << "      " ;
    gotoxy(coord.X, coord.Y+2); cout << "      " ;
    gotoxy(coord.X, coord.Y+3); cout << "       " ;
}

bool A_Wing :: isBossHit(Boss boss)
{
    return ((boss.getCOORD().X <= coord.X && boss.getCOORD().X+11 >= coord.X)
            || (boss.getCOORD().X <= coord.X + 6 && boss.getCOORD().X+11 >= coord.X+6))
            && ((boss.getCOORD().Y + 5 >= coord.Y && boss.getCOORD().Y <= coord.Y)
            || (boss.getCOORD().Y + 5 >= coord.Y + 4 && boss.getCOORD().Y <= coord.Y+3)) ;
}
bool A_Wing :: isEnemyHit(Enemy e)
{
    return (coord.X <= e.getCOORD().X && coord.X+6 >= e.getCOORD().X
            && coord.Y <= e.getCOORD().Y && coord.Y+3 >= e.getCOORD().Y) ;
}
