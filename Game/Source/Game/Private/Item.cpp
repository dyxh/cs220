// Fill out your copyright notice in the Description page of Project Settings.

#include "Game.h"
#include "Item.h"

Item::Item(int id, FString name, FString description) : id(id), name(name), description(description)
{
}

Item::Item()
{
}

Item::~Item()
{
}

Item::Item()
{
    id = -1;
    name = "default";
    description = "defualt";
}
