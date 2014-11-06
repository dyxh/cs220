// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class GAME_API Item
{
public:
	Item(int id, FString name, FString description);
    Item();
	~Item();
    int get_id();
    FString get_name();
    FString get_description();
private:
    int id; // The HEX id of the item (0x)
    FString name; // display name of item
    FString description; // descrition
};
