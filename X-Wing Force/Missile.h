
class Missile : public Laser /// Class of Missiles, an upgrade version of Laser.
{
    public :
        Missile() ;
        int cooldown ; /// Missile firing flag.
        void spawn() ;
};
