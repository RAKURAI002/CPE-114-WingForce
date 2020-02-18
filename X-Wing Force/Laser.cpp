
Laser :: Laser()
{
}
void Laser :: spawn()
{
    setcolor(coord.Y,0);
    gotoxy(coord.X, coord.Y);
    cout << "|" ;
    setcolor(7,0);
}
void Laser :: clearLaser()
{
    gotoxy(coord.X, coord.Y);
    cout << " " ;
}
bool Laser :: isEncounter(Enemy e)
{
    if(getCOORD() == e.getCOORD())
        return TRUE ;
    else
        return FALSE ;
}
