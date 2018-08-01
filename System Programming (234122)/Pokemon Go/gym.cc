//
// Created by roei_ on 22/01/2017.
//

#include "gym.h"
#include "trainer.h"

namespace mtm {
    namespace pokemongo {
        Gym::Gym():gymLeader(NULL), gymLeadingTeam(NEUTRAL){}

        void Gym::Arrive (Trainer& trainer){
            if (std::find(trainers_.begin(), trainers_.end(), &trainer) !=
                trainers_.end()) {
                throw LocationTrainerAlreadyInLocationException();
            }
            trainers_.push_back(&trainer);
            if(!gymLeader){
                gymLeader=&trainer;
                (*trainer.teamScore)+=10;
                gymLeadingTeam=trainer.GetTeam();
            }
            if(gymLeader->IsAlly(trainer))
                return;
            if(!((*gymLeader)==TrainersFight(trainer,*gymLeader))){
                (*gymLeader->teamScore)-=10;
                (*trainer.teamScore)+=10;
                gymLeadingTeam=trainer.GetTeam();
                gymLeader=&trainer;
            }
        }
        Trainer*& Gym::getGymLeader(){
            if(!gymLeader)
                throw PokemonGoGymHasNoLeader();
            return gymLeader;
        }

        Team& Gym::getGymLeadingTeam() {
            return gymLeadingTeam;
        }


        void Gym::Leave(Trainer& trainer) {
            std::vector<Trainer *>::iterator position =
                    std::find(trainers_.begin(), trainers_.end(), &trainer);
            if (position == trainers_.end()) {
                throw LocationTrainerNotFoundException();
            }
            trainers_.erase(position);
            if (*gymLeader != trainer)
                return;
            (*trainer.teamScore) -= 10;
            if (trainers_.empty()) {
                gymLeader = NULL;
                return;
            }
            setNewLeaderAfterLeaving(trainer);
            if(gymLeader) {
                (*gymLeader->teamScore) += 10;
                gymLeadingTeam=gymLeader->GetTeam();
            } else{
                gymLeadingTeam=NEUTRAL;
            }
        }

        Trainer* getLeaderBetweenTrainers(Trainer* trainer1, Trainer* trainer2){
            if(!trainer1){
                if(trainer2)
                    return trainer2;
                return NULL;
            }
            if(!trainer2)
                return trainer1;
            return &TrainersFight(*trainer1,*trainer2);
        }

        void Gym::setNewLeaderAfterLeaving(Trainer& trainer) {
            if (findBestTrainerOfTeam(trainer.GetTeam())) {
                gymLeader = findBestTrainerOfTeam(trainer.GetTeam());
                return;
            }

            Trainer *blueNewLeader = findBestTrainerOfTeam(BLUE);
            Trainer *redNewLeader = findBestTrainerOfTeam(RED);
            Trainer *yellowNewLeader = findBestTrainerOfTeam(YELLOW);
            switch (trainer.GetTeam()) {
                case RED:
                    gymLeader = getLeaderBetweenTrainers(blueNewLeader,
                                                         yellowNewLeader);
                    return;
                case BLUE:
                    gymLeader = getLeaderBetweenTrainers(redNewLeader,
                                                         yellowNewLeader);
                    return;
                case YELLOW:
                    gymLeader = getLeaderBetweenTrainers(redNewLeader,
                                                         blueNewLeader);
                    return;
                default: throw PokemonGoTrainerNeutralType();
            }
        }

        Trainer* Gym::findBestTrainerOfTeam(Team team){
            Trainer* bestTrainer=NULL;
            std::vector<Trainer*>::iterator it=trainers_.begin();
            while(it!=trainers_.end()){
                if((*it)->GetTeam()==team){
                    if(!bestTrainer){
                        bestTrainer=*it;
                    } else{
                        if(*bestTrainer<**it)
                            bestTrainer=*it;
                    }
                }
                it++;
            }
            return bestTrainer;
        }
    }
}
