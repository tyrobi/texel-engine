#include "..\items\helmet.h"
#include "..\items\torso.h"
#include "..\items\leggings.h"
#include "..\items\hairstyle.h"

class Humanoid
{
private:
    Helmet head;
    Torso torso;
    Leggings legs;
    Hairstyle hairstyle;
    int bodystyle;

public:
    Humanoid()
    {};
    ~Humanoid()
    {};

};