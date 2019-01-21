#ifndef PROFILE_H
#define PROFILE_H
#include <string>
#include "..\player\character.h"
#include "..\player\progress.h"

class Profile
{
private:
    std::string name;
    Character player;
    Progress progress;
    float timePlayed;
    double created;

public:
    Profile(std::string playerName, Character& playerCharacter)
    {
        player = playerCharacter;
        name = playerName;
    };

    ~Profile()
    {

    };
};
#endif