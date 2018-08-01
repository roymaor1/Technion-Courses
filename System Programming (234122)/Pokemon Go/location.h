#ifndef LOCATION_H
#define LOCATION_H

#include <algorithm>
#include <vector>

#include "exceptions.h"
#include "trainer.h"

namespace mtm {
namespace pokemongo {

class Location {
 public:
  virtual ~Location() {};
  virtual void Arrive(Trainer& trainer) {
    if (std::find(trainers_.begin(), trainers_.end(), &trainer) !=
            trainers_.end()) {
      throw LocationTrainerAlreadyInLocationException();
    }
    trainers_.push_back(&trainer);
  }

  virtual void Leave(Trainer& trainer) {
    std::vector<Trainer*>::iterator position = 
        std::find(trainers_.begin(), trainers_.end(), &trainer);
    if (position == trainers_.end()) {
      throw LocationTrainerNotFoundException();
    }
    trainers_.erase(position);
  }

  const std::vector<Trainer*>& GetTrainers() {
    return trainers_;
  }

 protected:
  std::vector<Trainer*> trainers_;
};

}  // pokemongo
}  // mtm

#endif  // LOCATION_H