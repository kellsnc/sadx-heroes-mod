#pragma once

extern bool chrsounds;
extern bool jmpsounds;
extern bool flysounds;
extern bool envsounds;

void PlayHeroesSound(int ID);
void PlayDelayedHeroesSound(int ID, int time);
void PlayHeroesSoundQueue(int ID, ObjectMaster* obj, float dist, bool loop);

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
	SonicSound_ThatHurts,
	SonicSound_Hurt1,
	SonicSound_Hurt2,
	SonicSound_Trick,
	SonicSound_Ready,
	SonicSound_Death,
	SonicSound_LevelUp,
	SonicSound_Win,
	SonicSound_Lose,
	SonicSound_Idle1,
	SonicSound_Idle2
};