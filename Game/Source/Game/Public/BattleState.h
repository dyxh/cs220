// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class GAME_API BattleState
{
public:
	~BattleState();
    virtual BattleState *handleInput(AOCharacter& character, Input input);
    virtual BattleState *update(AOCharacter& character, Input input);
};
