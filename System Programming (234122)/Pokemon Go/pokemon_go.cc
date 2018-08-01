//
// Created by roei_ on 24/01/2017.
//

#include "pokemon_go.h"
#include "k_graph.h"
#include <string>
#include <iostream>
#include <typeinfo>

using namespace mtm::pokemongo;

PokemonGo::PokemonGo(const World* world): gameWorld(world),
             trainersMap(std::map<std::string,Trainer>()),yellowScore(0),
                blueScore(0), redScore(0){}

void PokemonGo::AddTrainer(
        const std::string& name, const Team& team, const std::string& location){
    if(!(*gameWorld).Contains(location))
        return;PokemonGoLocationNotFoundException();
    if(trainersMap.find(name)!=trainersMap.end())
        throw PokemonGoTrainerNameAlreadyUsedExcpetion();
    if(name=="")
        throw PokemonGoInvalidArgsException();
    int* teamScore=NULL;
    switch (team){
        case BLUE:
            teamScore=&blueScore;
            break;
        case RED:
            teamScore=&redScore;
            break;
        case YELLOW:
            teamScore=&yellowScore;
            break;
        default:
            throw PokemonGoTrainerNeutralType();
    }
   Trainer trainerToAdd=Trainer(name,team);
    trainerToAdd.teamScore=teamScore;
    (*teamScore)++;
    trainerToAdd.setTrainersLocation(location);
    trainersMap.insert(std::pair<std::string,Trainer>(name,trainerToAdd));
    gameWorld->operator[](location)->Arrive(trainersMap.at(name));
}

void PokemonGo::MoveTrainer(const std::string& trainer_name,
                            const Direction& dir){
    auto trainerIt = trainersMap.find(trainer_name);
    if(trainerIt==trainersMap.end())
        throw PokemonGoTrainerNotFoundExcpetion();
    std::string newLoc;
    auto locationIt=(*gameWorld).BeginAt
            ((*trainerIt).second.getTrainersLocation());
    try {
        locationIt.Move(dir);
        newLoc=*locationIt;
    }
    catch (...){
        throw PokemonGoReachedDeadEndException();
    }

    ((*gameWorld)[(*trainerIt).second.getTrainersLocation()])->Leave(
            (*trainerIt).second); //LEAVING CURRENT LOCATION
    (*gameWorld)[*locationIt]->Arrive((*trainerIt).second);
    trainerIt->second.setTrainersLocation(newLoc);
}
std::string PokemonGo::WhereIs(const std::string& trainer_name){
    auto trainerIt = trainersMap.find(trainer_name);
    if(trainerIt==trainersMap.end())
        throw PokemonGoTrainerNotFoundExcpetion();
    return trainerIt->second.getTrainersLocation();
}
const std::vector<Trainer*>& PokemonGo::GetTrainersIn(const std::string& location){
    if(!(*gameWorld).Contains(location))
        throw PokemonGoLocationNotFoundException();
    return (*gameWorld)[location]->GetTrainers();
}

int PokemonGo::GetScore(const Team& team){
    switch (team){
        case YELLOW:
            return yellowScore;
        case BLUE:
            return blueScore;
        case RED:
            return redScore;
        default:
            return 0;
    }

}

PokemonGo::~PokemonGo(){
    delete gameWorld;
}

