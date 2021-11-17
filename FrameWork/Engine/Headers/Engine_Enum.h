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
		CAMERA,
		ANIMATION,
		ANIMATOR,
		COLLISION,
	///////// 여기서부터 몬스터 ///////////
		FIREBALL,
		MONBULLET,
		MELEEMON_TEX,
		MELEEMON_IDLEANIM,
		MELEEMON_IDLETEX,
		MELEEMON_WALKANIM,
		MELEEMON_WALKTEX,
		MELEEMON_ATTACKANIM,
		MELEEMON_ATTACKTEX,
		SHOOTMON_TEX,
		FLYMON_TEX,
		TEXTURE
	};
	enum class COMPONENTTYPE {TYPE_DYNAMIC, TYPE_STATIC, TYPE_END};
	enum class LAYERID {ENVIRONMENT, GAME_LOGIC,UI,LOADING};
	enum class GAMEOBJECTID {PLAYER,
		MONSTER1,
		MONSTER2,
		MONSTER3,
		BOSS,
		MONBULLET,
		FIREBALL,
		TERRAIN,
		BACKGROUND,
		CAMERA,
		PLAYERMODEL,
		SHURIKEN,
		BOMB,
		FOG,
		QUAD,
		CUBE,
		ITEM,
		GAMEOBJECT_END
	};
	enum class COLLISIONTAG {PLAYER,MONSTER,BULLET,ETC,MAX};
	enum class COLLISIONTRIGGER {ATTACK, HIT, INTERACT,MAX};
	enum class TEXTURETYPE {TEX_NORMAL, TEX_CUBE,TEX_HEIGHT,TEX_END};
	enum class MATRIXINFO {MAT_RIGHT, MAT_UP, MAT_LOOK, MAT_POS};
	enum class VECAXIS {AXIS_RIGHT, AXIS_UP, AXIS_LOOK, AXIS_POS};
	enum class RENDERGROUP {PRIORITY, NONALPHA, ALPHA, UI, MAX};
	enum class eITEM{HP20,HP50,HP100,SHURIKEN20,SHURIKEN50,BOMB2,BOMB5,ITEMEND};
}
#endif