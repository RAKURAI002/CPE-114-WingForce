

Boss :: Boss() : hp(30)
{
}
void Boss :: clearBoss()
{
    gotoxy(coord.X,coord.Y);
    cout << "             " ; gotoxy(coord.X, coord.Y+1);
    cout << "             " ; gotoxy(coord.X, coord.Y+2);
    cout << "             " ; gotoxy(coord.X, coord.Y+3);
    cout << "             " ; gotoxy(coord.X, coord.Y+4);
    cout << "             " ; gotoxy(coord.X, coord.Y+5);
    cout << "             " ;
}

void Boss :: spawn()
{
    setcolor(7,0);
    gotoxy(coord.X,coord.Y);
    cout << "\\_III--III_/" ;gotoxy(coord.X, coord.Y+1);
    cout << " \\\\\\||||///" ; gotoxy(coord.X, coord.Y+2);
    cout << "  \\\\\\||///" ; gotoxy(coord.X, coord.Y+3);
    cout << "   \\\\__//" ;gotoxy(coord.X, coord.Y+4);
    cout << "    \\\\//" ;gotoxy(coord.X, coord.Y+5);
    cout << "|-0-|\\/|-0-|"  ;
    setcolor(7,0);
}
