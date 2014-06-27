//
//  MOAIHusky.cpp
//  libmoai
//
//  Created by Brendan Ragan on 20/12/13.
//
//

#include "MOAIHusky.h"

MOAIHusky::MOAIHusky() {
	RTTI_BEGIN
	RTTI_EXTEND(MOAILuaObject)
	RTTI_END
}

MOAIHusky::~MOAIHusky() {
}

int MOAIHusky::_getAvailable( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
	
	int index = 0;
	
	lua_newtable(L);
	return 1;
}

int MOAIHusky::_getCurrent( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" );

	ZLLog::Print ( "Static Husky Stub provides no functions - getCurrent" );

	return 0;
}

int MOAIHusky::_setCurrent( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "US" )
	
	cc8* name = lua_tostring ( state, 2 );
	ZLLog::Print ( "Static Husky Stub provides no functions - setCurrent" );
	
	return 0;
}

int MOAIHusky::_hasLeaderboards( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
		
	ZLLog::Print ( "Static Husky Stub provides no functions - hasLeaderboards" );

	return 0;
}

int MOAIHusky::_hasAchievements( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	ZLLog::Print ( "Static Husky Stub provides no functions - hasAchievements" );
		
	return 0;
}

int MOAIHusky::_hasCloudSaves( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	ZLLog::Print ( "Static Husky Stub provides no functions - hasCloudSaves" );

	return 0;
}

int MOAIHusky::_hasGenericOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	ZLLog::Print ( "Static Husky Stub provides no functions - hasGenericOverlay" );

	return 0;
}

int MOAIHusky::_hasAchievementsOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	ZLLog::Print ( "Static Husky Stub provides no functions - hasAchievementsOverlay" );

	return 0;
}

int MOAIHusky::_hasAchievementsReset( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	ZLLog::Print ( "Static Husky Stub provides no functions - hasAchievementsReset" );

	return 0;
}

int MOAIHusky::_hasLeaderboardRangeFetch( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	ZLLog::Print ( "Static Husky Stub provides no functions - hasLeaderboardRangeFetch" );

	return 0;
}

int MOAIHusky::_hasLeaderboardsOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	ZLLog::Print ( "Static Husky Stub provides no functions - hasLeaderboardsOverlay" );

	return 0;
}

int MOAIHusky::_hasSingleLeaderboardOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	ZLLog::Print ( "Static Husky Stub provides no functions - hasSingleLeaderboardOverlay" );

	return 0;
}

int MOAIHusky::_showGenericOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	ZLLog::Print ( "Static Husky Stub provides no functions - showGenericOverlay" );

	return 0;
}

int MOAIHusky::_showAchievementsOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	ZLLog::Print ( "Static Husky Stub provides no functions - showAchievementsOverlay" );

	return 0;
}

int MOAIHusky::_achievementReset( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	ZLLog::Print ( "Static Husky Stub provides no functions - achievementReset" );
	
	return 0;
}


int MOAIHusky::_achievementSet( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "US" )

	cc8* name = lua_tostring ( state, 2 );

	ZLLog::Print ( "Static Husky Stub provides no functions - achievementSet" );	

	return 0;
}

int MOAIHusky::_achievementSetCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )
	
	self->SetLocal(state, 2, self->_achievementCallback);

	ZLLog::Print ( "Static Husky Stub provides no functions - achievementSetCallback" );	
	
	return 0;
}

int MOAIHusky::_leaderboardMetadataBytes( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	ZLLog::Print ( "Static Husky Stub provides no functions - leaderboardMetadataBytes" );
	
	return 0;
}

int MOAIHusky::_showLeaderboardsOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	ZLLog::Print ( "Static Husky Stub provides no functions - showLeaderboardsOverlay" );
	
	return 0;
}

int MOAIHusky::_showSingleLeaderboardOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	ZLLog::Print ( "Static Husky Stub provides no functions - showSingleLeaderboardOverlay" );
	
	return 0;
}

int MOAIHusky::_leaderboardUploadScore( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "USNS" )

	ZLLog::Print ( "Static Husky Stub provides no functions - leaderboardUploadScore" );	

	return 0;
}

int MOAIHusky::_leaderboardSetScoreCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )
	
	ZLLog::Print ( "Static Husky Stub provides no functions - leaderboardSetScoreCallback" );	
	
	return 0;
}

int MOAIHusky::_leaderboardGetScores( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "USBBSNN" )

	ZLLog::Print ( "Static Husky Stub provides no functions - leaderboardGetScores" );	
			
	return 0;
}

int MOAIHusky::_leaderboardSetGetScoresCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )
	
	ZLLog::Print ( "Static Husky Stub provides no functions - leaderboardSetGetScoresCallback" );	
	
	return 0;
}

int MOAIHusky::_cloudDataUpload( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "US" )
	
	ZLLog::Print ( "Static Husky Stub provides no functions - cloudDataUpload" );	

	return 0;
}

int MOAIHusky::_cloudDataSetUploadCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )
	
	ZLLog::Print ( "Static Husky Stub provides no functions - cloudDataUploadCallback" );		
	
	return 0;
}

int MOAIHusky::_cloudDataDownload( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "US" )
	
	ZLLog::Print ( "Static Husky Stub provides no functions - cloudDataDownload" );		

	return 0;
}

int MOAIHusky::_cloudDataSetDownloadCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )

	ZLLog::Print ( "Static Husky Stub provides no functions - cloudDataDownloadCallback" );		
	
	return 0;
}

int MOAIHusky::_doTick(lua_State *L) {
	MOAI_LUA_SETUP(MOAIHusky, "U");
	
	return 0;
}

//----------------------------------------------------------------//
void MOAIHusky::RegisterLuaClass ( MOAILuaState& state ) {
	
	// call any initializers for base classes here:
	// SledgeInputHandlerBase::RegisterLuaClass ( state );
	
	// also register constants:
	// state.SetField ( -1, "FOO_CONST", ( u32 )FOO_CONST );
	// here are the class methods:
	luaL_Reg regTable [] = {
		{ "getAvailable",						_getAvailable },
		{ "getCurrent",							_getCurrent },
		{ "setCurrent",							_setCurrent },
		{ "hasGenericOverlay",					_hasGenericOverlay },
		{ "showGenericOverlay",					_showGenericOverlay },
		{ "achievementSet",						_achievementSet },
		{ "achievementSetCallback",				_achievementSetCallback },
		{ "showAchievementsOverlay",				_showAchievementsOverlay },
		{ "hasAchievementsOverlay",				_hasAchievementsOverlay },
		{ "hasAchievements",						_hasAchievements },
		{ "achievementReset",					_achievementReset },
		{ "hasAchievementsReset",				_hasAchievementsReset },
		{ "cloudDataUpload",						_cloudDataUpload },
		{ "cloudDataSetUploadCallback",			_cloudDataSetUploadCallback },
		{ "cloudDataDownload",					_cloudDataDownload },
		{ "cloudDataSetDownloadCallback",		_cloudDataSetDownloadCallback },
		{ "hasCloudSaves",						_hasCloudSaves },
		{ "hasLeaderboards",						_hasLeaderboards },
		{ "leaderboardMetadataBytes",			_leaderboardMetadataBytes },
		{ "leaderboardUploadScore",				_leaderboardUploadScore },
		{ "leaderboardSetUploadScoreCallback",	_leaderboardSetScoreCallback },
		{ "leaderboardGetScores",				_leaderboardGetScores },
		{ "leaderboardSetGetScoresCallback",		_leaderboardSetGetScoresCallback },
		{ "hasLeaderboardRangeFetch",			_hasLeaderboardRangeFetch },
		{ "hasLeaderboardsOverlay",				_hasLeaderboardsOverlay },
		{ "showLeaderboardsOverlay",				_showLeaderboardsOverlay },
		{ "hasSingleLeaderboardOverlay",			_hasSingleLeaderboardOverlay },
		{ "showSingleLeaderboardOverlay",		_showSingleLeaderboardOverlay },
		{ "doTick", _doTick },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIHusky::RegisterLuaFuncs ( MOAILuaState& state ) {
	luaL_Reg regTable [] = {
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

void MOAIHusky::HuskyObserverAchievementCallback(const char *name, bool success) {
	if (_achievementCallback) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		this->PushLocal ( state, _achievementCallback );
		state.Push(name);
		state.Push(success);
		state.DebugCall ( 2, 0 );
	}
}

void MOAIHusky::HuskyObserverLeaderboardScoreSetCallback(const char *name, bool success) {
	if (_leaderboardScoreSetCallback) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		this->PushLocal ( state, _leaderboardScoreSetCallback );
		state.Push(name);
		state.Push(success);
		state.DebugCall ( 2, 0 );
	}
}

void MOAIHusky::HuskyObserverLeaderboardScoreGetCallback(const char *name, HuskyLeaderboardEntry *entries, int number) {
	if (!_leaderboardScoreGetCallback)
		return;

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushLocal ( state, _leaderboardScoreGetCallback );
	state.Push(name);
	lua_newtable(state);
	for(int i = 0; i < number; i++) {
		state.Push(i+1);
		lua_newtable(state);
		state.Push("name");
		state.Push(entries[i].name);
		lua_settable(state, -3);
		state.Push("globalrank");
		state.Push(entries[i].globalrank);
		lua_settable(state, -3);
		state.Push("score");
		state.Push(entries[i].score);
		lua_settable(state, -3);
		state.Push("data");
		if (entries[i].data != 0) {
			MOAIDataBuffer *moaibuffer = new MOAIDataBuffer();
			moaibuffer->Load((void*)&(entries[i].data), 8);
			state.Push(moaibuffer);
		} else {
			state.Push();
		}
		lua_settable(state, -3);
		lua_settable(state, -3);
	}
	state.DebugCall ( 2, 0 );
}

void MOAIHusky::HuskyObserverCloudDataDownloaded(const char *cloudfilename, void* buffer, int32_t bytes) {
	if (!_cloudDataDownloadCallback)
		return;
	MOAIScopedLuaState state = MOAILuaRuntime::Get().State ();
	MOAIDataBuffer *moaibuffer = new MOAIDataBuffer();
	moaibuffer->Load(buffer, bytes);
	this->PushLocal(state, _cloudDataDownloadCallback);
	state.Push(cloudfilename);
	state.Push(moaibuffer);
	state.DebugCall(2, 0);
}

void MOAIHusky::HuskyObserverCloudDataUploaded(const char *path, bool success) {
	if (!_cloudDataUploadCallback)
		return;

	MOAIScopedLuaState state = MOAILuaRuntime::Get().State();
	this->PushLocal(state, _cloudDataUploadCallback);
	state.Push(path);
	state.Push(success);
	state.DebugCall(2, 0);
}
