//
// Created by Jur de Vries on 29/11/2020.
//

#include "VolumeFollower.h"
#include "math.h"

VolumeFollower::VolumeFollower()
:minBoundary(-1.0f), maxBoundary(1.0f), currentVolume(0.f)
{
}

void VolumeFollower::setup(float minBoundary, float maxboundary) {
    this->minBoundary = minBoundary;
    this->maxBoundary = maxboundary;
}


void VolumeFollower::update(ofPoint currentPosition) {
    float volume = currentPosition.y;
    if (currentPosition.x < minBoundary || currentPosition.x > maxBoundary) {
        return;
    }
    if (abs(volume - this->currentVolume) > stepSize * 2) {
        if (volume > currentVolume) {
            this->currentVolume += stepSize;
        }
        else {
            this->currentVolume -= stepSize;
        }
        return;
    }
    this->currentVolume = volume;
}
