#include "Device.h"

Atlas::ADevice::ADevice(const bool isPhysicalDevice)
{
}

Atlas::ADevice::ADevice(const bool isPhysicalDevice, const bool isEnabled, const bool isReady)
{
}

void Atlas::ADevice::enable()
{
    this->mIsEnabled = true;
}

void Atlas::ADevice::disable()
{
    this->mIsEnabled = false;
}

void Atlas::ADevice::setReady(bool newState)
{
    this->mIsReady = newState;
}

bool Atlas::ADevice::isDeviceEnabled()
{
    return this->mIsEnabled;
}

bool Atlas::ADevice::isDeviceReady()
{
    return this->mIsReady;
}

bool Atlas::ADevice::isPhysicalDevice()
{
    return this->mIsPhysicalDevice;
}
