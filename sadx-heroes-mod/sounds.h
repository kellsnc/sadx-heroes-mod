#pragma once

#define Status_KillSound 0x4000

extern bool chrsounds;
extern bool jmpsounds;
extern bool flysounds;
extern bool envsounds;

void PlayHeroesSound(int ID);
void PlayDelayedHeroesSound(int ID, int time);
void PlayHeroesSound_Entity(int ID, ObjectMaster* obj, float dist, bool loop);
void PlayHeroesSound_EntityAndPos(int ID, ObjectMaster* obj, NJS_VECTOR* pos, float dist, float volume, bool loop);
void PlayHeroesSound_EntityAndVolume(int ID, ObjectMaster* obj, float dist, float volume, bool loop);
void PlayHeroesSound_Pos(int ID, NJS_VECTOR* pos, float dist, float volume, bool loop);

enum HeroesSounds {
	CommonSound_DashPanel = 10100,
	CommonSound_Jumping,
	CommonSound_BoxBreak,
	CommonSound_CannonIn,
	CommonSound_CannonMov,
	CommonSound_CannonStp,
	CommonSound_CannonLch,
	CommonSound_GoalRing,
	CommonSound_Pulley,
	CommonSound_Rail,
	CommonSound_ItemBox,
	CommonSound_Tornado,
	CommonSound_AccelAttack,
	CommonSound_HomingAttack,
	CommonSound_GlideBegin,
	CommonSound_Glide,
	CommonSound_FlyPunchBegin,
	CommonSound_FlyPunchHit,
	CommonSound_RailHit,
	CommonSound_Propeller,
	CommonSound_Explosion,
	CommonSound_Detect1,
	CommonSound_Detect2,
	CommonSound_Shoot1,
	CommonSound_Shoot2,
	LevelSound_Sea_Seagul = 10200,
	LevelSound_Sea_RuinStart,
	LevelSound_Sea_RuinEnd,
	LevelSound_Cty_FluidPath = 10220,
	LevelSound_Csn_Bumper1 = 10240,
	LevelSound_Csn_Bumper2,
	LevelSound_Csn_Bumper3,
	LevelSound_Csn_Door,
	LevelSound_Csn_GlassBreak,
	LevelSound_Csn_JackPot,
	LevelSound_Csn_Lost,
	LevelSound_Csn_Machine,
	LevelSound_Csn_Slot,
	LevelSound_Csn_Trigger,
	LevelSound_Mys_Door1 = 10260,
	LevelSound_Mys_Door2,
	LevelSound_Mys_Ghost1,
	LevelSound_Mys_Ghost2,
	LevelSound_Mys_MvPltf,
	LevelSound_Mys_Warp1,
	LevelSound_Mys_Warp2,
	LevelSound_Mys_Char,
	LevelSound_Egg_Door = 10280,
	LevelSound_Egg_Boom1,
	LevelSound_Egg_Boom2,
	LevelSound_Egg_Boom3,
	LevelSound_Egg_Engines,
	LevelSound_Egg_Fan,
	CreamSound_Attack = 10900,
	CreamSound_FlyBegin,
	CreamSound_FlyAttack,
	CreamSound_FlyUp,
	CreamSound_TeamSwap,
	CreamSound_ThatHurts,
	CreamSound_Hurt1,
	CreamSound_Hurt2,
	CreamSound_Trick,
	CreamSound_CheeseGetHim,
	CreamSound_Death,
	CreamSound_LevelUp,
	CreamSound_Win,
	CreamSound_Lose,
	CreamSound_Idle1,
	CreamSound_Idle2,
	RougeSound_Attack = 11000,
	RougeSound_FlyBegin,
	RougeSound_FlyAttack,
	RougeSound_FlyUp,
	RougeSound_TeamSwap,
	RougeSound_ThatHurts,
	RougeSound_Hurt1,
	RougeSound_Hurt2,
	RougeSound_Trick,
	RougeSound_BombAttack,
	RougeSound_Death,
	RougeSound_LevelUp,
	RougeSound_Win,
	RougeSound_Lose,
	RougeSound_Idle1,
	RougeSound_Idle2,
	CharmySound_Attack = 11100,
	CharmySound_FlyBegin,
	CharmySound_FlyAttack,
	CharmySound_Trick,
	CharmySound_TeamSwap,
	CharmySound_ThatHurts,
	CharmySound_Hurt1,
	CharmySound_Hurt2,
	CharmySound_FlyUp,
	CharmySound_DardAttack,
	CharmySound_Death,
	CharmySound_LevelUp,
	CharmySound_Win,
	CharmySound_Lose,
	CharmySound_Idle1,
	CharmySound_Idle2,
	TailsSound_Attack = 11200,
	TailsSound_FlyBegin,
	TailsSound_FlyAttack,
	TailsSound_FlyUp,
	TailsSound_TeamSwap,
	TailsSound_ThatHurts,
	TailsSound_Hurt1,
	TailsSound_Hurt2,
	TailsSound_Trick,
	TailsSound_BombAttack,
	TailsSound_Death,
	TailsSound_LevelUp,
	TailsSound_Win,
	TailsSound_Lose,
	TailsSound_Idle1,
	TailsSound_Idle2,
	TailsSound_Flying,
	SonicSound_Attack = 11300,
	SonicSound_Ya,
	SonicSound_Go,
	SonicSound_FlyUp,
	SonicSound_TeamSwap,
	SonicSound_HereWeGo,
	SonicSound_Hurt1,
	SonicSound_Hurt2,
	SonicSound_Trick,
	SonicSound_Ready,
	SonicSound_Death,
	SonicSound_LevelUp,
	SonicSound_Win,
	SonicSound_Lose,
	SonicSound_Idle1,
	SonicSound_Idle2,
	ShadowSound_Attack = 11400,
	ShadowSound_Ya,
	ShadowSound_Go,
	ShadowSound_FlyUp,
	ShadowSound_TeamSwap,
	ShadowSound_HereWeGo,
	ShadowSound_Hurt1,
	ShadowSound_Hurt2,
	ShadowSound_Trick,
	ShadowSound_Ready,
	ShadowSound_Death,
	ShadowSound_LevelUp,
	ShadowSound_Win,
	ShadowSound_Lose,
	ShadowSound_Idle1,
	ShadowSound_Idle2,
	AmySound_Attack = 11500,
	AmySound_Ya,
	AmySound_TeamSwap,
	AmySound_FlyUp,
	AmySound_Jump,
	AmySound_HereWeGo,
	AmySound_Hurt1,
	AmySound_Hurt2,
	AmySound_Trick,
	AmySound_Ready,
	AmySound_Death,
	AmySound_LevelUp,
	AmySound_Win,
	AmySound_Lose,
	AmySound_Idle1,
	AmySound_Idle2,
	EspioSound_Attack = 11600,
	EspioSound_Ya,
	EspioSound_TeamSwap,
	EspioSound_FlyUp,
	EspioSound_Jump,
	EspioSound_Letsgo,
	EspioSound_Hurt1,
	EspioSound_Hurt2,
	EspioSound_Trick,
	EspioSound_Ready,
	EspioSound_Death,
	EspioSound_LevelUp,
	EspioSound_Win,
	EspioSound_Lose,
	EspioSound_Idle1,
	EspioSound_Idle2,
	KnucklesSound_Attack = 11700,
	KnucklesSound_Combo1,
	KnucklesSound_Combo2,
	KnucklesSound_Combo3,
	KnucklesSound_Jump,
	KnucklesSound_TeamSwap,
	KnucklesSound_Hurt1,
	KnucklesSound_Hurt2,
	KnucklesSound_Woah,
	KnucklesSound_Trick,
	KnucklesSound_Death,
	KnucklesSound_LevelUp,
	KnucklesSound_Win,
	KnucklesSound_Lose,
	KnucklesSound_Idle1,
	KnucklesSound_Idle2,
	OmegaSound_Attack = 11800,
	OmegaSound_Combo1,
	OmegaSound_Combo2,
	OmegaSound_Combo3,
	OmegaSound_TeamSwap,
	OmegaSound_Woah,
	OmegaSound_Trick,
	OmegaSound_CannotDig,
	OmegaSound_Yes,
	OmegaSound_Walk,
	OmegaSound_Death, OmegaSound_Lose = 11810,
	OmegaSound_LevelUp,
	OmegaSound_Win,
	OmegaSound_Idle1,
	OmegaSound_Idle2,
	BigSound_Attack = 11900,
	BigSound_Combo1,
	BigSound_Combo2,
	BigSound_Combo3,
	BigSound_Jump,
	BigSound_TeamSwap,
	BigSound_Hurt1,
	BigSound_Hurt2,
	BigSound_Woah,
	BigSound_Trick,
	BigSound_Death,
	BigSound_LevelUp,
	BigSound_Win,
	BigSound_Lose,
	BigSound_Idle1,
	BigSound_Idle2,
	VectorSound_Attack = 12000,
	VectorSound_Combo1,
	VectorSound_Combo2,
	VectorSound_Combo3,
	VectorSound_Jump,
	VectorSound_TeamSwap,
	VectorSound_Hurt1,
	VectorSound_Hurt2,
	VectorSound_Woah,
	VectorSound_Trick,
	VectorSound_Death,
	VectorSound_LevelUp,
	VectorSound_Win,
	VectorSound_Lose,
	VectorSound_Idle1,
	VectorSound_Idle2,
};