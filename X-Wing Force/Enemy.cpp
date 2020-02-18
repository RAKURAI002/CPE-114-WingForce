
Enemy :: Enemy()
{
    setCOORD(0, 0) ;
}
Enemy :: Enemy(int x, int y) : CoordOBJ()
{
    setCOORD(x, y) ;
}
void Enemy :: spawn()
{
    gotoxy(coord.X,coord.Y);
    cout << static_cast<char>(ENEMY) ;
}
void Enemy :: clearEnemy()
{
    gotoxy(coord.X,coord.Y);
    cout << " " ;
}
