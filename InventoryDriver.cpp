/*
* Author: Richard Mauricio
*/
#include <iostream>
#include "./Map/map.h"
#include "./Character/Fighter.h"
#include "./Dice/Dice.h"
#include "Observer/MapObserver/MapObserver.h"
#include "Director/FighterDirector/FighterDirector.h"
#include "./Strategy/CharacterStrategy/HumanPlayerStrategy/HumanPlayerStrategy.h"
#include "./Strategy/CharacterStrategy/AggressorStrategy/AggressorStrategy.h"
#include "./Strategy/CharacterStrategy/FriendlyStrategy/FriendlyStrategy.h"
#include "Builder/FighterBuilder/BullyFighterBuilder/BullyFighterBuilder.h"
#include "./Observer/GameObserver/GameObserver.h"
#include "CharacterCreator/CharacterCreatorUI.h"
#include "./Utils/utils.h"
#include "../TurnManager/TurnManager.h"
#include "../Campaign/Campaign.h"
using namespace std;

int main()
{
   /* CharacterCreatorUI characterCreatorUI{
    };
    characterCreatorUI.run();*/
    Campaign* c= new Campaign();
    auto* logger = new GameObserver();
    auto* m = new dungeonMap(10, 10);
    logger->attachObservable(m);
    m->notifyMapCreated();
    auto* control = new MapObserver(m);
    m->attach(control);
    Dice::GetGlobal().attach(logger);
    FighterDirector director{};
    BullyFighterBuilder builder;
    director.setBuilder(&builder);

    Fighter* fighter = director.constructFighter("Bully");
    Fighter* fighter2 = director.constructFighter("Bully 2");
    Fighter* fighter3 = director.constructFighter("Bully 3");
    fighter2->setEXP(300);
    fighter3->setEXP(300);

    Fighter* fighter4 = director.constructFighter("Friendly");
    fighter->setStrategy(new HumanPlayerStrategy());
    fighter2->setStrategy(new AggressorStrategy());
    fighter3->setStrategy(new AggressorStrategy());
    fighter4->setStrategy(new FriendlyStrategy());

    m->setUserPlayer(fighter);
   /* m->setPlayer(fighter2, 4, 4);*/
    auto* chest1 = new container();
    auto* chest2 = new container();
    auto* chest3 = new container();
    Item* ring = new Item(Item::ItemType::RING);
    Item* ring2 = new Item(Item::ItemType::RING);
    Item* helmet = new Item(Item::ItemType::HELMET);
    Item* weapon = new Item(Item::ItemType::WEAPON);
    Item* weapon2 = new Item(Item::ItemType::WEAPON);
    Item* weapon3 = new Item(Item::ItemType::WEAPON);
    Item* armor = new Item(Item::ItemType::ARMOR);
    Item* armor2 = new Item(Item::ItemType::ARMOR);
    Item* armor3 = new Item(Item::ItemType::ARMOR);
    fighter->getStrategy()->attach(logger);
    chest1->storeItem(*ring);
    chest1->storeItem(*armor);
    chest1->storeItem(*armor2);
    chest1->storeItem(*armor3);
    chest2->storeItem(*helmet);
    chest2->storeItem(*weapon);
    chest2->storeItem(*ring2);
    chest3->storeItem(*weapon2);
    chest3->storeItem(*weapon3);

    m->setStart(1, 2);
    m->setPlayer(fighter, m->getStartX(), m->getStartY());
    m->setEnd(4, 4);
    m->setChest(chest1, 3, 3);
    m->setChest(chest2, 0, 0);
    m->setChest(chest3, 0, 1);
    m->setWall(5, 2);
    m->setWall(9, 3);
    m->setWall(4, 2);
    //fighter->move(*m);
    TurnManager* map = new TurnManager(m, fighter,c);
    map->addNPC(fighter2);
    fighter->setCurrentHP(100);
    map->addNPC(fighter3);
    map->addNPC(fighter4);
    map->setAllNPCS();
    map->play();
	return 0;
}