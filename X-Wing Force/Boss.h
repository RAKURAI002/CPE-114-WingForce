

class Boss : public CoordOBJ /// Class of Boss.
{
    public:
        Boss();
        void spawn() ;
        void clearBoss() ;
        int hp ;
        COORD random_coord ;
};
