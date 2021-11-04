#ifndef __ENGINE_ENUM_H__
#define __ENGINE_ENUM_H__
namespace Engine
{
	enum class WINDOW { FULL = 0, WIN };
	enum class COMPONENTID 
	{
		TRICOL, 
		RCCOL, 
		RCTEX, 
		TERRAINTEX, 
		TRANSFORM,
		PLAYER_TEX,
		BACKGROUND_TEX,
		MONSTER_TEX
	};
	enum class COMPONENTTYPE {TYPE_DYNAMIC, TYPE_STATIC, TYPE_END};
	enum class LAYERID {ENVIRONMENT, GAME_LOGIC,UI};
	enum class GAMEOBJECTID {PLAYER,MONSTER,BOSS,TERRAIN,BACKGROUND,GAMEOBJECT_END};
	enum class TEXTURETYPE {TEX_NORMAL, TEX_CUBE,TEX_HEIGHT,TEX_END};
	enum class MATRIXINFO {MAT_RIGHT, MAT_UP, MAT_LOOK, MAT_POS};
	enum class RENDERGROUP {PRIORITY, NONALPHA, ALPHA, UI, MAX};
}
#endif