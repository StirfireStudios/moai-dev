//
//  Husky.h
//  Amazon Game Circle Husky
//
//  Created by Brendan Ragan on 16/02/14.
//
//

#include "pch.h"

#ifndef __libmoai__HuskyGameCircle__
#define __libmoai__HuskyGameCircle__

#include <AchievementsClientInterface.h>
#include <WhispersyncClientInterface.h>

#include <Husky.h>

class ProgressCallback {
public:
	char* name;
	bool success;
};

typedef std::vector<ProgressCallback> ProgressCallbackList;

class HuskyGameCircle : Husky, public AmazonGames::IUpdateProgressCb {
public:
	HuskyGameCircle();
	~HuskyGameCircle();

	static HuskyGameCircle* getInstance();
	static void shutdownInstance();

	void setObserver(HuskyObserver* observer);

	uint16_t getCapabilities();

	void doTick();

	void setAchievement(const char* name);

	void resetAchievements();

	void uploadLeaderboardScore(const char *name, int32_t score, HuskyLeaderboardScoreToKeep tokeep, int64_t extradata);

	void requestLeaderboardScores(const char *name, bool friends, HuskyLeaderboardScoreTimeFrame timeframe, int offset, int number);

	void requestLeaderboardScoresNearPlayer(const char *name, bool friends, HuskyLeaderboardScoreTimeFrame timeframe, int offset, int number);

	void uploadCloudData(const char *cloudfilename, void* data, int32_t bytes);

	void requestCloudData(const char *cloudfilename);

	void onUpdateProgressCb(AmazonGames::ErrorCode errorCode, const AmazonGames::UpdateProgressResponse* responseStruct, int developerTag);

private:
	static HuskyGameCircle* instance;
	HuskyObserver* _observer;
	AmazonGames::GameDataMap* _cloudDataMap;
	std::vector<ProgressCallback>* _progressCallbackList;
};

#endif /* defined(__libmoai__HuskyGameCircle__) */
