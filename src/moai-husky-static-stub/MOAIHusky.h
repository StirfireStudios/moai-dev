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

class MOAIHusky : public MOAIGlobalClass <MOAIHusky, MOAILuaObject>, public HuskyObserver {
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
	
	void HuskyObserverAchievementCallback(const char *name, bool success);
	void HuskyObserverLeaderboardScoreSetCallback(const char *name, bool success);
	void HuskyObserverLeaderboardScoreGetCallback(const char *name, HuskyLeaderboardEntry *entries, int number);
	void HuskyObserverCloudDataDownloaded(const char *cloudfilename, void* buffer, int32_t bytes);
	void HuskyObserverCloudDataUploaded(const char *path, bool success);
	
	MOAILuaLocal _achievementCallback;
	MOAILuaLocal _leaderboardScoreSetCallback;
	MOAILuaLocal _leaderboardScoreGetCallback;
	MOAILuaLocal _cloudDataUploadCallback;
	MOAILuaLocal _cloudDataDownloadCallback;
	
public:
	DECL_LUA_SINGLETON ( MOAIHusky )
	
							MOAIHusky	(  );
							~MOAIHusky	(  );
	//----------------------------------------------------------------//
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};

#endif /* defined(__libmoai__MOAIHusky__) */
