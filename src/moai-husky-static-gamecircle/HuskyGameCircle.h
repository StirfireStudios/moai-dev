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
#include <LeaderboardsClientInterface.h>
#include <ProfilesClientInterface.h>
#include <GameCircleClientInterface.h>

#include <Husky.h>

class NamedCallback {
public:
	char* name;
	bool success;
};

class LeaderboardPlayerScoreCallback {
public:
	char* name;
	bool success;
	int rank;
	long long score;
};

class LeaderboardScoresCallback {
public:
	char* name;
	int number;
	HuskyLeaderboardEntry* entries;
};

typedef std::vector<NamedCallback> NamedCallbackList;
typedef std::vector<LeaderboardScoresCallback> LeaderboardScoresCallbackList;
typedef std::vector<LeaderboardPlayerScoreCallback> LeaderboardPlayerScoreCallbackList;
typedef std::map<int, std::string> LeaderboardTagNameMap;
typedef std::map<std::string, int> LeaderboardNameTagMap;

class HuskyGameCircle : Husky, 
	public AmazonGames::IUpdateProgressCb,
	public AmazonGames::ILeaderboardSubmitScoreCb,
	public AmazonGames::ILeaderboardGetPlayerScoreCb,
	public AmazonGames::ILeaderboardGetScoresCb,
	public AmazonGames::IProfileGetLocalPlayerProfileCb {
public:
	HuskyGameCircle();
	~HuskyGameCircle();

	static HuskyGameCircle* getInstance();
	static void shutdownInstance();

	void setObserver(HuskyObserver* observer);

	uint16_t getCapabilities();

	void showOverlay();

	void doTick();

	void showAchievementsOverlay();

	void setAchievement(const char* name);

	void resetAchievements();

	void showLeaderboardsOverlay();

	void showLeaderboardOverlay(const char *name);

	int leaderboardMetadataByteStorage();

	void uploadLeaderboardScore(const char *name, int32_t score, HuskyLeaderboardScoreToKeep tokeep, int64_t extradata);

	void requestLeaderboardScores(const char *name, bool friends, HuskyLeaderboardScoreTimeFrame timeframe, int offset, int number);

	void requestLeaderboardScoresNearPlayer(const char *name, bool friends, HuskyLeaderboardScoreTimeFrame timeframe, int offset, int number);

	void uploadCloudData(const char *cloudfilename, void* data, int32_t bytes);

	void requestCloudData(const char *cloudfilename);

	void onUpdateProgressCb(AmazonGames::ErrorCode errorCode, const AmazonGames::UpdateProgressResponse* responseStruct, int developerTag);

	void onSubmitScoreCb(AmazonGames::ErrorCode errorCode, const AmazonGames::SubmitScoreResponse* responseStruct, int developerTag);

	void onGetPlayerScoreCb(AmazonGames::ErrorCode errorCode, const AmazonGames::PlayerScoreInfo* responseStruct, int developerTag);

	void onGetScoresCb(AmazonGames::ErrorCode errorCode, const AmazonGames::LeaderboardScores* responseStruct, int developerTag);

	void onGetLocalPlayerProfileCb(AmazonGames::ErrorCode errorCode, const AmazonGames::PlayerInfo* responseStruct, int developerTag);

private:
	static HuskyGameCircle* instance;
	HuskyObserver* _observer;
	AmazonGames::GameDataMap* _cloudDataMap;
	NamedCallbackList* _progressCallbackList;
	NamedCallbackList* _leaderboardSetCallbackList;
	LeaderboardTagNameMap* _leaderboardTagNameMap;
	LeaderboardNameTagMap* _leaderboardNameTagMap;
	LeaderboardScoresCallbackList* _leaderboardScoresCallbackList;
	LeaderboardPlayerScoreCallbackList* _leaderboardPlayerScoreCallbackList;
	int _leaderboardTag;
	char* _playerId;
	char* _playerAlias;

	int getLeaderboardTag(const char *name);
	char* getLeaderboardName(int tag);
	AmazonGames::LeaderboardFilter getFilterForSettings(bool friends, HuskyLeaderboardScoreTimeFrame timeframe);
};

#endif /* defined(__libmoai__HuskyGameCircle__) */
