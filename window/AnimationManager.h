#pragma once

#include "AnimatedModel.h"

enum class AnimationName {
	Idle,
	Attack,
	Reload,
	Death,
	Inspect,
	Run
};

class AnimationManager {
public:
	std::unordered_map<std::string, std::unordered_map<AnimationName, std::string>> table;

	void set(const std::string& modelId, AnimationName name, const std::string& clip) {
		table[modelId][name] = clip;
	}

	const std::string& get(const std::string& modelId, AnimationName name) const {
		return table.at(modelId).at(name);
	}

	bool has(const std::string& modelId, AnimationName name) const {
		auto it = table.find(modelId);
		if (it == table.end()) return false;
		return it->second.find(name) != it->second.end();
	}
};

/*
00 pose : 1 frames, 24 tps, 0.0416667 sec
01 select : 30 frames, 24 tps, 1.25 sec
02 putaway : 13 frames, 24 tps, 0.541667 sec
03 empty select : 13 frames, 24 tps, 0.541667 sec
04 idle : 70 frames, 24 tps, 2.91667 sec
05 inspect : 145 frames, 24 tps, 6.04167 sec
06 walk : 40 frames, 24 tps, 1.66667 sec
07 run : 40 frames, 24 tps, 1.66667 sec
08 fire : 8 frames, 24 tps, 0.333333 sec
09 alternate fire : 9 frames, 24 tps, 0.375 sec
10 melee attack : 23 frames, 24 tps, 0.958333 sec
11 zoom idle : 70 frames, 24 tps, 2.91667 sec
12 zoom walk : 40 frames, 24 tps, 1.66667 sec
13 zoom fire : 8 frames, 24 tps, 0.333333 sec
14 zoom alternate fire : 9 frames, 24 tps, 0.375 sec
15 alternate fire mode on : 20 frames, 24 tps, 0.833333 sec
16 dryfire : 5 frames, 24 tps, 0.208333 sec
17 reload : 45 frames, 24 tps, 1.875 sec
18 empty reload : 70 frames, 24 tps, 2.91667 sec
*/

/*
attack : 138 frames, 60 tps, 2.3 sec
death : 171 frames, 60 tps, 2.85 sec
idle : 179 frames, 60 tps, 2.98333 sec
idle2 : 452 frames, 60 tps, 7.53333 sec
roar : 200 frames, 60 tps, 3.33333 sec
run : 80 frames, 60 tps, 1.33333 sec
walk : 90 frames, 60 tps, 1.5 sec
*/