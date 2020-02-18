
CoordOBJ :: CoordOBJ() : status(0)
{
}
COORD CoordOBJ :: getCOORD() const
{
    return coord ;
}
void CoordOBJ :: plusCOORD(int x, int y)
{
    coord.X += x ;
    coord.Y += y ;
}
void CoordOBJ :: setCOORD(int x, int y)
{
    coord.X = x ;
    coord.Y = y ;
}
