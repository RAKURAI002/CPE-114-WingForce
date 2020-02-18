
class Laser : public CoordOBJ /// Class of Lasers.
{
    public :
        Laser() ;
        void spawn() ;
        void clearLaser() ;
        bool isEncounter(Enemy e) ;

};
