//
// Created by roei_ on 22/01/2017.
//

#ifndef POKEMONGO_GYM_H
#define POKEMONGO_GYM_H

#include "location.h"

namespace mtm{
    namespace pokemongo{
        class Gym: public Location{
            Trainer* gymLeader;
            Team gymLeadingTeam;
            /// set gym new leading trainer
            /// if gym is empty set leader to null
            /// \param trainer - the leaving leader
            void setNewLeaderAfterLeaving(Trainer& trainer);
            /// findes the best trainer of the given team
            /// \param team
            /// \return the best trainer of the given team
            ///Null - if team have no trainers in Gym
            Trainer* findBestTrainerOfTeam(Team team);



            public:
                Gym();
                ~Gym()= default;
                virtual void Arrive(Trainer& trainer) override;
                virtual void Leave(Trainer& trainer) override;
                 Trainer*& getGymLeader();
                 Team& getGymLeadingTeam();



        };
    }
}







#endif //POKEMONGO_GYM_H
