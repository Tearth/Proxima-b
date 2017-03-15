#pragma once

enum class EColor
{
	None,
	White,
	Black
};

enum class ECommandType
{
	None,
	Hello,
	AddPiece,
	Save,
	Load,
	Heuristic,
	MinMax,
	GetHash,
	IsMate,
	MarkPosition,
	DoMove,
	SetMaxTimeAI,
	SetGameMode,
	StopFICS,
	ResetBoard,
	Help,
	SendFICSCommand,
	FICSLoop
};

enum class EGamePhase
{
	None,
	Opening,
	Middlegame,
	Endgame
};

enum class EMoveCharacterFlags
{
	None = 0,
	Pawn = 1,
	Knight = 2,
	Rook = 4,
	Bishop = 8
};

enum class ValueType
{
	None,
	Exact,
	Upper,
	Lower
};

enum class GameMode
{
	None,
	AIvsPlayer,
	AIvsAI,
	FICSvsAI
};

enum class FICSMode
{
	None,
	LoginSequence,
	LoginConfirm,
	Seek,
	SeekWaiting,
	WaitingForAIMove,
	AIMove,
	WaitingForEnemyMove,
	EnemyMove,
	Reset
};

inline EMoveCharacterFlags operator~   (EMoveCharacterFlags a)       { return (EMoveCharacterFlags)~(int)a; }
inline EMoveCharacterFlags operator|   (EMoveCharacterFlags a, EMoveCharacterFlags b)  { return (EMoveCharacterFlags)((int)a | (int)b); }
inline EMoveCharacterFlags operator&   (EMoveCharacterFlags a, EMoveCharacterFlags b)  { return (EMoveCharacterFlags)((int)a & (int)b); }
inline EMoveCharacterFlags operator^   (EMoveCharacterFlags a, EMoveCharacterFlags b)  { return (EMoveCharacterFlags)((int)a ^ (int)b); }
inline EMoveCharacterFlags& operator|= (EMoveCharacterFlags& a, EMoveCharacterFlags b) { return (EMoveCharacterFlags&)((int&)a |= (int)b); }
inline EMoveCharacterFlags& operator&= (EMoveCharacterFlags& a, EMoveCharacterFlags b) { return (EMoveCharacterFlags&)((int&)a &= (int)b); }
inline EMoveCharacterFlags& operator^= (EMoveCharacterFlags& a, EMoveCharacterFlags b) { return (EMoveCharacterFlags&)((int&)a ^= (int)b); }