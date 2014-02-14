//
//  MOAIHusky.h
//  libmoai
//
//  Created by Brendan Ragan on 20/12/13.
//
//

#include "pch.h"

#ifndef __libmoai__MOAIHusky__
#define __libmoai__MOAIHusky__

#include <Husky.h>
#include <AchievementsClientInterface.h>
#include <WhispersyncClientInterface.h>

class ProgressCallback {
public:
	char* name;
	bool success;
};

typedef std::vector<ProgressCallback> ProgressCallbackList;

class MOAIHusky : public MOAIGlobalClass <MOAIHusky, MOAILuaObject>, public AmazonGames::IUpdateProgressCb {
private:
	static int _getAvailable						( lua_State *L );
	static int _getCurrent							( lua_State *L );
	static int _setCurrent							( lua_State *L );
	static int _hasLeaderboards						( lua_State *L );
	static int _hasAchievements						( lua_State *L );
	static int _hasCloudSaves						( lua_State *L );
	static int _achievementReset					( lua_State *L );
	static int _achievementSet						( lua_State *L );
	static int _achievementSetCallback				( lua_State *L );
	static int _leaderboardUploadScore				( lua_State *L );
	static int _leaderboardSetScoreCallback			( lua_State *L );
	static int _leaderboardGetScores				( lua_State *L );
	static int _leaderboardSetGetScoresCallback		( lua_State *L );
	static int _cloudDataUpload						( lua_State *L );
	static int _cloudDataSetUploadCallback			( lua_State *L );
	static int _cloudDataDownload					( lua_State *L );
	static int _cloudDataSetDownloadCallback		( lua_State *L );
	static int _doTick								( lua_State *L );
	
//	void HuskyObserverAchievementCallback(const char *name, bool success);
	void HuskyObserverLeaderboardScoreSetCallback(const char *name, bool success);
	void HuskyObserverLeaderboardScoreGetCallback(const char *name, HuskyLeaderboardEntry *entries, int number);
	void onUpdateProgressCb(AmazonGames::ErrorCode errorCode, const AmazonGames::UpdateProgressResponse* responseStruct, int developerTag);
	
	MOAILuaLocal _achievementCallback;
	MOAILuaLocal _leaderboardScoreSetCallback;
	MOAILuaLocal _leaderboardScoreGetCallback;
	MOAILuaLocal _cloudDataUploadCallback;
	MOAILuaLocal _cloudDataDownloadCallback;

//	ProgressCallbackList* _progressCallbackList;
	AmazonGames::GameDataMap* _cloudDataMap;
	
public:
	DECL_LUA_SINGLETON ( MOAIHusky )
	
							MOAIHusky	(  );
							~MOAIHusky	(  );
	//----------------------------------------------------------------//
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};



#endif /* defined(__libmoai__MOAIHusky__) */
