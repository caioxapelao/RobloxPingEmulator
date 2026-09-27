#include <iostream>
#include <thread>
#include <cstdint>
#include <excgg/memory.h>
#include "offsets.h"
#include "utils.h"
#include "curl_wrapper.h"

void entry_point() {
	drv::attach("RobloxPlayerBeta.exe");
	auto base = drv::getBase("RobloxPlayerBeta.exe");
	auto fakeDm = drv::read<uintptr_t>(base + Offsets::FakeDataModel::Pointer);
	auto realDm = drv::read<uintptr_t>(fakeDm + Offsets::FakeDataModel::RealDataModel);
	auto ipString = drv::roblox::readString(realDm + Offsets::DataModel::ServerIP);
	auto serverIp = robloxIp::parseIp(ipString);
	auto serverLocation = ipGeolocation::locateIp(serverIp);
	auto userLocation = ipGeolocation::locateIp(ipGeolocation::getUserIp());
	auto estimatedPing = ping::estimatePing(userLocation, serverLocation);
	std::cout << estimatedPing << std::endl;
}

int main() {
	std::thread(entry_point).join();
}