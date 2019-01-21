
#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "../../game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <map>
#include <SDL_ttf.h>
#include "../../misc/enums.h"

// using namespace Enums;

class TextureManager
{
private:
    static std::map<const char*, SDL_Texture*> cache;
    static std::map<std::string, SDL_Texture*> textCache;
    static bool smallTextFlag;

    static TTF_Font* FORE_FONT;
    static TTF_Font* BACK_FONT;
    static TTF_Font* FORE_SMALL_FONT;
    static TTF_Font* BACK_SMALL_FONT;
    static constexpr int OUTLINE_WEIGHT = 1;
    static constexpr int FONT_SIZE = 32;
    static constexpr int FONT_SMALL_SIZE = 22;
    static constexpr int MAX_TEXT_CACHE_SIZE = 32;
    static const SDL_Color BLACK;
    static const SDL_Color WHITE;

    static SDL_RendererFlip getFlag(bool flipX, bool flipY)
    {
        // if (flipX && flipY) return (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
        if (flipX) return SDL_FLIP_HORIZONTAL;
        if (flipY) return SDL_FLIP_VERTICAL;
        return SDL_FLIP_NONE;
    }

    static void initFont()
    {
        TTF_Init();
        BACK_FONT = TTF_OpenFont("res/cactus.otf", FONT_SIZE);
        FORE_FONT = TTF_OpenFont("res/cactus.otf", FONT_SIZE);
        BACK_SMALL_FONT = TTF_OpenFont("res/cactus.otf", FONT_SMALL_SIZE);
        FORE_SMALL_FONT = TTF_OpenFont("res/cactus.otf", FONT_SMALL_SIZE);
        TTF_SetFontOutline(BACK_FONT, OUTLINE_WEIGHT);
    }

    static SDL_Rect shiftPosition(int x, int y, float w, float h, Enums::AnchorPos pos)
    {
        SDL_Rect r = {0, 0, w, h};
        int vw = GameCamera::getViewport()->w; // Parametrize to fit in other rects?
        int vh = GameCamera::getViewport()->h;
        switch(pos)
        {
            case Enums::AnchorPos::TOP_LEFT:
                r.x = x;
                r.y = y;
                return r;
            case Enums::AnchorPos::TOP_MID:
                r.x = (vw - w)/2;
                r.y = y;
                return r;
            case Enums::AnchorPos::TOP_RIGHT:
                r.x = (vw - w) - x;
                r.y = y;
                return r;
            case Enums::AnchorPos::MID_LEFT:
                r.x = x;
                r.y = (vh + h) / 2;
                return r;
            case Enums::AnchorPos::MID_MID:
            case Enums::AnchorPos::CENTER:
                r.x = ((vw - w) / 2) - x;
                r.y = ((vh - h) / 2) - y;
                return r;
            case Enums::AnchorPos::MID_RIGHT:
                r.x = (vw - w) - x;
                r.y = (h + vh) / 2;
                return r;
            case Enums::AnchorPos::BOT_LEFT:
                r.x = x;
                r.y = (vh - h) - y;
                return r;
            case Enums::AnchorPos::BOT_MID:
                r.x = (vw - w) / 2;
                r.y = (vh - h) - y;
                return r;
            case Enums::AnchorPos::BOT_RIGHT:
                r.x = vw - w - x;
                r.y = (vh - h) - y;
                return r;
        }
        return r;
    }

    SDL_Rect shiftPosition(SDL_Rect& r, Enums::AnchorPos pos)
    {
        return shiftPosition(r.x, r.y, r.w, r.h, pos);
    }

    SDL_Surface* applyAlphaBlend(const char* filename)
    {

    }

    static int getAlphaColumn(Enums::Direction dir)
    {
        using namespace Enums;

        switch(dir)
        {
            case Direction::NORTH_EAST:
                return 0;
            case Direction::NORTH_WEST:
                return 1;
            case Direction::SOUTH_WEST:
                return 2;
            case Direction::SOUTH_EAST:
                return 3;
            default:
                printf("Alpha column is unspecified for this direction\n");
                return 0;
        }
    }

    static std::string getTextIndex(const char* text)
    {
        std::string t(smallTextFlag?"text:sm:" : "text:nm:");
        t += text;
        return t;
    }


public:

    static SDL_Texture* loadTexture(const char* filename)
    {
        if (cache.count(filename) == 1)
        {
            return cache[filename];
        }

        SDL_Surface *surf = IMG_Load(filename);
        if (surf == NULL)
            printf("SDL Image error ocurred - failed to load '%s' : %s\n", filename, IMG_GetError());

        SDL_Texture *tex = SDL_CreateTextureFromSurface(GameRenderer::get(), surf);
        if (tex == NULL)
            printf("SDL failed to convert '%s' into a texture: %s\n", filename, SDL_GetError());

        const char* err = SDL_GetError();
        if (*err)
        {
            printf("SDL error enccountered: %s\n", err);
            SDL_ClearError();
            delete err;
        }

        err = IMG_GetError();
        if (*err)
        {
            printf("SDL Image error enccountered: %s\n", err);
            SDL_ClearError();
            delete err;
        }

        SDL_FreeSurface(surf);
        delete surf;

        cache[filename] = tex;

        return tex;
    }

    static SDL_Texture* loadClippedTexture(const char* sheet, Vec2 tilesrc, Enums::Direction dir)
    {
        SDL_Surface *surf;
        // if (cache.count(sheet) == 1)
        //     surf = cache[sheet];
        // else
        surf = IMG_Load(sheet);

        SDL_Surface *clipSurf = IMG_Load("res/tiles/alpha_masks.png");

        if (surf == NULL)
            printf("SDL Image error ocurred - failed to load '%s' : %s\n", sheet, IMG_GetError());

        SDL_Rect tile, clip;
        tile.w = tile.h = clip.w = clip.h = 16;
        tile.x = 16 * tilesrc.x();
        tile.y = 16 * tilesrc.y();
        clip.x = 16 * getAlphaColumn(dir);
        clip.y = 0;

        SDL_BlitSurface(clipSurf, &clip, surf, &tile);
        SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf-> format, 0xFF, 0x00, 0xFF));

        SDL_Texture *tex = SDL_CreateTextureFromSurface(GameRenderer::get(), surf);
        if (tex == NULL)
            printf("SDL failed to convert '%s' into a texture: %s\n", sheet, SDL_GetError());

        const char* err = SDL_GetError();
        if (*err)
        {
            printf("SDL error enccountered: %s\n", err);
            SDL_ClearError();
            delete err;
        }

        err = IMG_GetError();
        if (*err)
        {
            printf("SDL Image error enccountered: %s\n", err);
            SDL_ClearError();
            delete err;
        }

        SDL_FreeSurface(surf);
        SDL_FreeSurface(clipSurf);
        delete surf;

        // cache[filename] = tex;

        return tex;
    }

    static SDL_Texture* loadText(const char* text)
    {
        if (! TTF_WasInit())
            initFont();
        if (textCache.count(getTextIndex(text)) == 1)
            return textCache[getTextIndex(text)];

        SDL_Texture *tex = nullptr;
        SDL_Surface *bg_surface = TTF_RenderText_Blended(BACK_FONT, text, BLACK);
        SDL_Surface *fg_surface = TTF_RenderText_Blended(FORE_FONT, text, WHITE);
        SDL_Rect rect = {OUTLINE_WEIGHT, OUTLINE_WEIGHT, fg_surface->w, fg_surface->h};

        SDL_SetSurfaceBlendMode(fg_surface, SDL_BLENDMODE_BLEND);
        SDL_BlitSurface(fg_surface, NULL, bg_surface, &rect);
        tex = SDL_CreateTextureFromSurface(GameRenderer::get(), bg_surface);
        SDL_FreeSurface(bg_surface);
        SDL_FreeSurface(fg_surface);
        textCache[getTextIndex(text)] = tex;

        return tex;
    }

    static void renderText(const char* text, const Vec2 pos, int alpha, bool worldAnchored, Enums::AnchorPos anchor)
    {
        SDL_Texture *tex = loadText(text);
        int w, h;
        SDL_QueryTexture(tex, NULL, NULL, &w, &h);

        SDL_Rect dest = shiftPosition(pos.x(), pos.y(), w, h, anchor);
        SDL_Rect t = {
            dest.x * GameCamera::getScale(),
            dest.y * GameCamera::getScale(),
            dest.w * GameCamera::getScale(),
            dest.h * GameCamera::getScale()};

        // Check if within bounds
        // if (!SDL_HasIntersection(GameCamera::getViewport(), &t)) return;

        SDL_SetTextureAlphaMod(tex, alpha);
        if (worldAnchored)
        {
            SDL_Rect src = {0, 0, w, h};
            draw(tex, src, dest, false, false);
        }
        else
            SDL_RenderCopy(GameRenderer::get(), tex, NULL, &dest);
    }

    static void renderSmallText(const char* text, const Vec2 pos, int alpha, Enums::AnchorPos anchor)
    {
        smallTextFlag = true;
        TTF_Font* largeFontForePtr = FORE_FONT;
        TTF_Font* largeFontBackPtr = BACK_FONT;
        FORE_FONT = FORE_SMALL_FONT;
        BACK_FONT = BACK_SMALL_FONT;
        TTF_SetFontOutline(BACK_FONT, OUTLINE_WEIGHT);
        renderText(text, pos, alpha, anchor);
        FORE_FONT = largeFontForePtr;
        BACK_FONT = largeFontBackPtr;
        TTF_SetFontOutline(BACK_FONT, OUTLINE_WEIGHT);
        smallTextFlag = false;
    }

    static void renderText(const char* text, const Vec2 pos, int alpha, bool worldAnchored)
    {
        renderText(text, pos, alpha, worldAnchored, Enums::AnchorPos::TOP_LEFT);
    }

    static void renderText(const char* text, const Vec2 pos, int alpha, Enums::AnchorPos anchor)
    {
        renderText(text, pos, alpha, false, anchor);
    }


    static void renderText(const char* text, const Vec2 pos, bool worldAnchored)
    {
        renderText(text, pos, 255, worldAnchored);
    }

    static void renderText(const char* text, const Vec2 pos, Enums::AnchorPos anchor)
    {
        renderText(text, pos, 255, anchor);
    }

    static void renderText(const char* text, const Vec2 pos)
    {
        renderText(text, pos, 255, false);
    }

    static void refreshTextCache()
    {
        //TODO: ensure texture cache doesn't swell much

    }

    static void clearCache()
    {
        for (auto& x : cache)
        {
            SDL_DestroyTexture(x.second);
            delete x.first;
        }

        for (auto& x : textCache)
        {
            SDL_DestroyTexture(x.second);
        }

        cache.clear();
        textCache.clear();
    }

    static void destroy()
    {
        clearCache();
        TTF_CloseFont(FORE_FONT);
        TTF_CloseFont(BACK_FONT);
        TTF_CloseFont(FORE_SMALL_FONT);
        TTF_CloseFont(BACK_SMALL_FONT);
    }

    static void draw(SDL_Texture* tex, SDL_Rect &src, SDL_Rect &dest)
    {
        draw(tex, src, dest, false, false);
    }

    static void draw(SDL_Texture* tex, SDL_Rect &src, SDL_Rect &dest, bool flipX, bool flipY)
    {
        SDL_Rect adj;
        float s = GameCamera::getScale();
        adj.x = (dest.x * s) - GameCamera::getViewport()->x + GameCamera::getOffset().x();
        adj.y = (dest.y * s) - GameCamera::getViewport()->y + GameCamera::getOffset().y();
        adj.w = (dest.w * s + 1);
        adj.h = (dest.h * s + 1);
        SDL_RenderCopyEx(GameRenderer::get(), tex, &src, &adj, NULL, NULL, getFlag(flipX, flipY));
    }
};

std::map<const char*, SDL_Texture*> TextureManager::cache = {};
std::map<std::string, SDL_Texture*> TextureManager::textCache = {};

const SDL_Color TextureManager::BLACK{0, 0, 0};
const SDL_Color TextureManager::WHITE{255, 255, 255};
TTF_Font* TextureManager::BACK_FONT = nullptr;
TTF_Font* TextureManager::FORE_FONT = nullptr;
TTF_Font* TextureManager::BACK_SMALL_FONT = nullptr;
TTF_Font* TextureManager::FORE_SMALL_FONT = nullptr;
bool TextureManager::smallTextFlag = false;

#endif

