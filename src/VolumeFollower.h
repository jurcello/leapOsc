//
// Created by Jur de Vries on 29/11/2020.
//

#ifndef LEAPOSC_VOLUMEFOLLOWER_H
#define LEAPOSC_VOLUMEFOLLOWER_H

#include "ofPoint.h"

class VolumeFollower {
public:
    VolumeFollower();

    void setup(float minBoundary, float maxboundary);
    void update(ofPoint currentPosition);

    float stepSize = 0.05f;
    float minBoundary;
    float maxBoundary;
    float currentVolume;

    bool currentPositionOutsideBoundary(const ofPoint &currentPosition) const;

    bool volumeIsFarAwayFromCurrentVolume(float volume) const;

    void approachVolume(float volume);

    void updateVolume(float volume);
};


#endif //LEAPOSC_VOLUMEFOLLOWER_H
