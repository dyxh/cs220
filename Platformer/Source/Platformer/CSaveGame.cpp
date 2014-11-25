// Fill out your copyright notice in the Description page of Project Settings.

#include "Platformer.h"
#include "CSaveGame.h"


UCSaveGame::UCSaveGame(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
    SaveSlotName = TEXT("TestSaveSlot");
    UserIndex = 0;
}


