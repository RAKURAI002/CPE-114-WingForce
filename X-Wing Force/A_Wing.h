
class A_Wing : public Fleet /// A type, with unique functions.
{
    public:
        A_Wing() ;
        void spawn() ;
        void clearFleet () ;
        bool isBossHit(Boss) ;
        bool isEnemyHit(Enemy) ;
};
