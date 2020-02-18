
class CoordOBJ /// Base class of all classes. Contains all of position(COORD) basic functions.
{
    public :
        CoordOBJ() ;
        int status ;
        COORD getCOORD() const ;
        void setCOORD(int x, int y) ;
        void plusCOORD(int x, int y) ;

    protected :
        COORD coord ;

};
