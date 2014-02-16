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
	HuskyGameCircle::shutdownInstance();
}

HuskyGameCircle* MOAIHusky::getInstance() {
	return HuskyGameCircle::getInstance();
}

int MOAIHusky::_getAvailable( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
	
	int index = 0;
	
	lua_newtable(L);
	lua_pushstring(L, "AmazonGameCircle");
	return 1;
}

int MOAIHusky::_getCurrent( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" );

	lua_pushstring(L, "AmazonGameCircle");

	return 1;
}

int MOAIHusky::_setCurrent( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "US" )
	
	cc8* name = lua_tostring ( state, 2 );
	if (strcasecmp(name, "AmazonGameCircle") != 0)
		ZLLog::Print ( "Static GameCircle Husky provides no other huskies than \"AmazonGameCircle\"" );
	
	return 0;
}

int MOAIHusky::_hasLeaderboards( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
		
	state.Push(1);
	return 1;
}

int MOAIHusky::_hasAchievements( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
	
	state.Push(1);
	return 1;
}

int MOAIHusky::_hasCloudSaves( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	state.Push(1);
	return 1;
}

int MOAIHusky::_hasGenericOverlay( lua_State* L) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	state.Push(1);
	return 1;	
}

int MOAIHusky::_showGenericOverlay( lua_State* L) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	return 0;	
}

int MOAIHusky::_achievementReset( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	ZLLog::Print ( "Static GameCircle Husky cannot reset achievements" );
	
	return 0;
}

int MOAIHusky::_achievementSet( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "US" )

	cc8* name = lua_tostring ( state, 2 );
	self->getInstance()->setAchievement(name);

	return 0;
}

int MOAIHusky::_achievementSetCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )
	
	self->getInstance()->setObserver(self);
	self->SetLocal(state, 2, self->_achievementCallback);
	
	return 0;
}

int MOAIHusky::_leaderboardUploadScore( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "USNS" )
	
	cc8* name = lua_tostring ( state, 2 );
	int32_t score = lua_tointeger( state, 3 );
	cc8* replacement = lua_tostring ( state, 4 );
	MOAIStream* moaistream = state.GetLuaObject < MOAIStream >( 5, true );
	int64_t data = 0;
	if (moaistream != NULL) {
		ZLStream *stream = moaistream->GetZLStream();
		stream->Seek(0, SEEK_SET);
		if (stream->GetLength() < 8) {
			stream->ReadBytes((void*)&data, stream->GetLength());
		} else {
			stream->ReadBytes((void*)&data, 8);
		}
	}
	
	HuskyLeaderboardScoreToKeep update = HuskyLeaderboardScoreToKeepNone;
	if (strcasecmp(replacement, "best") == 0) {
		update = HuskyLeaderboardScoreToKeepBest;
	} else if (strcasecmp(replacement, "update") == 0) {
		update = HuskyLeaderboardScoreToKeepUpdate;
	}

	self->getInstance()->uploadLeaderboardScore(name, score, update, data);

	return 0;
}

int MOAIHusky::_leaderboardSetScoreCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )
	
	self->getInstance()->setObserver(self);
	self->SetLocal(state, 2, self->_leaderboardScoreSetCallback);
	
	return 0;
}

int MOAIHusky::_leaderboardGetScores( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "USBBSNN" )
		
	cc8* name = state.GetValue<cc8*>(2, 0);
	bool friends = state.GetValue<bool>(3,0);
	bool around = state.GetValue<bool>(4,0);
	cc8* timeframestring = state.GetValue<cc8*>(5,0);
	int32_t offset = state.GetValue<int>(6, 0);
	int32_t number = state.GetValue<int>(7, 0);
	
	HuskyLeaderboardScoreTimeFrame timeframe = HuskyLeaderboardAllScores;
	if (strcasecmp(timeframestring, "week"))
		timeframe = HuskyLeaderboardWeeksScores;
	else if (strcasecmp(timeframestring, "day"))
		timeframe = HuskyLeaderboardTodaysScores;

	if (around)
		self->getInstance()->requestLeaderboardScoresNearPlayer(name, friends, timeframe, offset, number);
	else
		self->getInstance()->requestLeaderboardScores(name, friends, timeframe, offset, number);
	
	return 0;
}

int MOAIHusky::_leaderboardSetGetScoresCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )

	self->getInstance()->setObserver(self);	
	self->SetLocal(state, 2, self->_leaderboardScoreGetCallback);
	
	return 0;
}

int MOAIHusky::_cloudDataUpload( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "US" )
	
	cc8* cloudpath = state.GetValue<cc8*>(2, 0);
	MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 3, true );
	data->Base64Encode();
	char* stringData = (char*)calloc(sizeof(char), data->getBuffer()->Size() + 1);
	memcpy(stringData, data->getBuffer()->Data(), data->getBuffer()->Size());

	self->getInstance()->uploadCloudData(cloudpath, stringData, strlen(stringData));

	return 0;
}

int MOAIHusky::_cloudDataSetUploadCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )
	
	self->SetLocal(state, 2, self->_cloudDataUploadCallback);
	self->getInstance()->setObserver(self);
	
	return 0;
}

int MOAIHusky::_cloudDataDownload( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "US" )
	
	char temp[255];

	if (!self->_cloudDataDownloadCallback)
		return 0;

	cc8* cloudpath = state.GetValue<cc8*>(2, 0);
	self->getInstance()->requestCloudData(cloudpath);

	return 0;
}

int MOAIHusky::_cloudDataSetDownloadCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )

	self->SetLocal(state, 2, self->_cloudDataDownloadCallback);
	self->getInstance()->setObserver(self);

	return 0;
}

int MOAIHusky::_doTick(lua_State *L) {
	MOAI_LUA_SETUP(MOAIHusky, "U");

	self->getInstance()->doTick();

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
		{ "hasGenericOverlay",		_hasGenericOverlay },
		{ "showGenericOverlay",		_showGenericOverlay },
		{ "hasAchievements",						_hasAchievements },
		//{ "hasAchievementsOverlay",						_hasAchievements },
		//{ "showAchievementsOverlay",		_showGenericOverlay },
		//{ "hasAchievementsReset",						_hasAchievements },
		{ "achievementReset",					_achievementReset },
		{ "achievementSet",						_achievementSet },
		{ "achievementSetCallback",				_achievementSetCallback },
		{ "hasLeaderboards",						_hasLeaderboards },
		//{ "leaderboardMetadataBytes",		_hasLeaderboards },
		//{ "hasLeaderboardOverlay",						_hasLeaderboards },
		//{ "showLeaderboardOverlay",		_showGenericOverlay },
		{ "leaderboardUploadScore",				_leaderboardUploadScore },
		{ "leaderboardSetUploadScoreCallback",	_leaderboardSetScoreCallback },
		{ "leaderboardGetScores",				_leaderboardGetScores },
		{ "leaderboardSetGetScoresCallback",		_leaderboardSetGetScoresCallback },
		//{ "hasLeaderboardRangeFetch",						_hasLeaderboards },
		{ "hasCloudSaves",						_hasCloudSaves },
		{ "cloudDataUpload",						_cloudDataUpload },
		{ "cloudDataSetUploadCallback",			_cloudDataSetUploadCallback },
		{ "cloudDataDownload",					_cloudDataDownload },
		{ "cloudDataSetDownloadCallback",		_cloudDataSetDownloadCallback },
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
		state.Push();
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
	if (bytes > 0) {
		moaibuffer->Load(buffer, bytes);
		moaibuffer->Base64Decode();
	}
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
