
class Fleet : public CoordOBJ /// Base class of Wings-Series. Contains all functions that Wings need.
{
    public :
        Fleet();
        Fleet(int hp, int sp, int fr, int spe,int fc, int sx, int sy, int ft) ;
        virtual void clearFleet () ;
        virtual void spawn() ;
        virtual bool isBossHit(Boss) ;
        virtual bool isEnemyHit(Enemy) ;
        void plusHp(int );
        int getHp() ;
        int getSpeed() ;
        int getFireRate() ;
        int getSpecial() ;
        int getFireConst() ;
        COORD getSizeConst() ;
        int getFleetType () ;

    private :
        int hp ;
        int speed ;
        int fire_rate ;
        int special ;
        int fire_const ; /// Gun's position.
        COORD size_const ;
        int fleet_type ;
};
