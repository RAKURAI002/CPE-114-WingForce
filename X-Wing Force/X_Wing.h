
class X_Wing : public Fleet /// X type, with unique functions.
{
    public:
        X_Wing() ;
        void spawn() ;
        void clearFleet () ;
        bool isBossHit(Boss) ;
        bool isEnemyHit(Enemy) ;
};
