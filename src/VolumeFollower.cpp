//
// Created by Jur de Vries on 29/11/2020.
//

#include "VolumeFollower.h"
#include "math.h"
#include "ofMath.h"

VolumeFollower::VolumeFollower()
:minBoundary(-1.0f), maxBoundary(1.0f), currentVolume(0.f)
{
}

void VolumeFollower::setup(float minBoundary, float maxboundary) {
    this->minBoundary = minBoundary;
    this->maxBoundary = maxboundary;
}


void VolumeFollower::update(ofPoint currentPosition) {
    if (currentPositionOutsideBoundary(currentPosition)) {
        return;
    }
    float volume = currentPosition.y;
    if (volumeIsFarAwayFromCurrentVolume(volume)) {
        approachVolume(volume);
    } else {
        updateVolume(volume);
    }
    this->currentVolume = ofClamp(currentVolume, 0.f, 1.f);
}

void VolumeFollower::updateVolume(float volume) {
    currentVolume = volume;
}

void VolumeFollower::approachVolume(float volume) {
    if (volume > currentVolume) {
        currentVolume += stepSize;
    }
    else {
        currentVolume -= stepSize;
    }
}

bool VolumeFollower::volumeIsFarAwayFromCurrentVolume(float volume) const {
    return abs(volume - currentVolume) > stepSize * 2;
}

bool VolumeFollower::currentPositionOutsideBoundary(const ofPoint &currentPosition) const {
    return currentPosition.z > 0.75f || currentPosition.x < minBoundary || currentPosition.x > maxBoundary;
}
