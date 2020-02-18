
class Y_Wing : public Fleet /// Y type, with unique functions.
{
    public:
        Y_Wing() ;
        void spawn() ;
        void clearFleet () ;
        bool isBossHit(Boss) ;
        bool isEnemyHit(Enemy) ;
};
