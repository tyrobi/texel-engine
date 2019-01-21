#ifndef GAME_ENUMS
#define GAME_ENUMS

namespace Enums
{

enum class Direction
{
    NONE,
    NORTH,
    SOUTH,
    EAST,
    WEST,

    NORTH_WEST,
    NORTH_EAST,
    SOUTH_WEST,
    SOUTH_EAST
};

enum class GameStatus
{
    LOADING,
    STOP_REQUESTED,
    STOPPING,
    STOPPED,
    UNINITIALIZED,
    INITIALIZING,
    READY
};

enum class PlayerMovementStatus
{
    STOPPED,
    WALKING,
    RUNNING
};

enum class PlayerActionStatus
{
    AVAILABLE,
    DEBOUNCE,
    BLOCKED
};

enum GroupLabels : std::size_t
{
    SUB_MAP,
    MAP,
    MAP_SPRITES,
    PLAYER,
    EQUIPPABLES,
    WALLS,
    UI
};

enum class TileType
{
    NOT_FOUND,
    NONE,
    DIRT,
    GRASS,
    SAND,
    WATER,
    MARSH,
    DOOR,
};

enum class CollisionType
{
    WALL,
    FLOOR,
    HALF_LEFT,
    HALF_RIGHT,
    HALF_TOP,
    HALF_BOTTOM
};

enum class MaterialType
{
    SOLID,
    LIQUID,
    STICKY
};

enum class TimeStatus
{
    FULL,
    STOPPED,
    SLOW
};

enum class EquippedStatus
{
    EQUIPPED,
    UNEQUIPPED
};

enum class ItemType
{
    ACCESSORY,
    WEAPON,
    AMMO,
    HELMET,
    TORSO,
    LEGGINGS,
    HEALING,
    MANA,
    KEY,
    LIGHT
};

enum class HealthStatus
{
    INVULNERABLE,
    ALIVE,
    INJURED,
    DEAD
};

enum class AnchorPos
{
    TOP_LEFT,
    TOP_MID,
    TOP_RIGHT,
    MID_LEFT,
    MID_MID,
    MID_RIGHT,
    BOT_LEFT,
    BOT_MID,
    BOT_RIGHT,
    CENTER
};
};
#endif