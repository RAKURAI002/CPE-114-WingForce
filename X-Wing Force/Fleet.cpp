
Fleet :: Fleet() : hp(3), speed(0), fire_rate(0), special(0), fire_const(0), fleet_type(0)
{
    setCOORD(0, 0) ;
}
Fleet :: Fleet(int hp, int sp, int fr, int spe, int fc, int sx, int sy, int ft) : hp(hp), speed(sp), fire_rate(fr)
                , special(spe), fire_const(fc), fleet_type(ft)
{
    setCOORD(37, 15) ;
    size_const.X = sx ;
    size_const.Y = sy ;
}
void Fleet :: spawn()
{
}
void Fleet :: clearFleet()
{
}
bool Fleet :: isBossHit(Boss boss)
{
    return FALSE ;
}
bool Fleet :: isEnemyHit(Enemy e)
{
    return FALSE ;
}
int Fleet :: getFireConst()
{
    return fire_const ;
}
COORD Fleet ::  getSizeConst()
{
    return size_const ;
}
int Fleet :: getSpeed()
{
    return speed ;
}
int Fleet :: getFireRate()
{
    return fire_rate ;
}
int Fleet :: getSpecial()
{
    return special ;
}
int Fleet :: getHp()
{
    return hp ;
}
int Fleet :: getFleetType ()
{
    return fleet_type ;
}
void Fleet :: plusHp(int x)
{
    hp += x ;
}
