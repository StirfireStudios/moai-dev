//
//  MOAIHusky.cpp
//  libmoai
//
//  Created by Brendan Ragan on 20/12/13.
//
//

#include "MOAIHusky.h"

HuskyGameCircle* MOAIHusky::_instance = NULL;

MOAIHusky::MOAIHusky() {
	RTTI_BEGIN
	RTTI_EXTEND(MOAILuaObject)
	RTTI_END

	_instance = HuskyGameCircle::getInstance();
	_instance->setObserver(this);
}

MOAIHusky::~MOAIHusky() {
	HuskyGameCircle::shutdownInstance();
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

	state.Push((bool)(self->_instance->getCapabilities() && HuskyHasLeaderboards));

	return 1;
}

int MOAIHusky::_hasAchievements( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
	
	state.Push((bool)(self->_instance->getCapabilities() && HuskyHasAchievements));

	return 1;
}

int MOAIHusky::_hasCloudSaves( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	state.Push((bool)self->_instance->getCapabilities() && HuskyHasCloudSaves);

	return 1;
}

int MOAIHusky::_hasGenericOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
	
	state.Push((bool)(self->_instance->getCapabilities() && HuskyHasGenericOverlay));

	return 1;
}

int MOAIHusky::_hasAchievementsOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
	
	state.Push((bool)(self->_instance->getCapabilities() && HuskyHasAchievementsOverlay));

	return 1;
}

int MOAIHusky::_hasAchievementsReset( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
	
	state.Push((bool)(self->_instance->getCapabilities() && HuskyHasAchievementReset));
	
	return 1;
}

int MOAIHusky::_hasLeaderboardRangeFetch( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
	
	state.Push((bool)(self->_instance->getCapabilities() && HuskyHasLeaderboardRangeFetch));
	
	return 1;
}

int MOAIHusky::_hasLeaderboardsOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
	
	state.Push((bool)(self->_instance->getCapabilities() && HuskyHasLeaderboardsOverlay));

	return 1;
}

int MOAIHusky::_hasSingleLeaderboardOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
	
	state.Push((bool)(self->_instance->getCapabilities() && HuskyHasSingleLeaderboardOverlay));
	
	return 1;
}

int MOAIHusky::_showGenericOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "" )
	
	self->_instance->showOverlay();

	return 0;
}

int MOAIHusky::_showAchievementsOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
	
	self->_instance->showAchievementsOverlay();
	
	return 0;
}

int MOAIHusky::_showLeaderboardsOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	self->_instance->showLeaderboardsOverlay();

	return 0;
}

int MOAIHusky::_showSingleLeaderboardOverlay( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "US" )
		
	cc8* name = lua_tostring ( state, 2 );
	self->_instance->showLeaderboardOverlay(name);
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
	self->_instance->setAchievement(name);

	return 0;
}

int MOAIHusky::_achievementSetCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )

	self->SetLocal(state, 2, self->_achievementCallback);
	
	return 0;
}

int MOAIHusky::_leaderboardMetadataBytes( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
	
	state.Push(self->_instance->leaderboardMetadataByteStorage());
	
	return 1;
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

	self->_instance->uploadLeaderboardScore(name, score, update, data);

	return 0;
}

int MOAIHusky::_leaderboardSetScoreCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )
	
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
		self->_instance->requestLeaderboardScoresNearPlayer(name, friends, timeframe, offset, number);
	else
		self->_instance->requestLeaderboardScores(name, friends, timeframe, offset, number);
	
	return 0;
}

int MOAIHusky::_leaderboardSetGetScoresCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )

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

	self->_instance->uploadCloudData(cloudpath, stringData, strlen(stringData));

	return 0;
}

int MOAIHusky::_cloudDataSetUploadCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )

	self->SetLocal(state, 2, self->_cloudDataUploadCallback);
	
	return 0;
}

int MOAIHusky::_cloudDataDownload( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "US" )

	if (!self->_cloudDataDownloadCallback)
		return 0;

	cc8* cloudpath = state.GetValue<cc8*>(2, 0);
	self->_instance->requestCloudData(cloudpath);

	return 0;
}

int MOAIHusky::_cloudDataSetDownloadCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )

	self->SetLocal(state, 2, self->_cloudDataDownloadCallback);

	return 0;
}

int MOAIHusky::_doTick(lua_State *L) {
	MOAI_LUA_SETUP(MOAIHusky, "U");

	self->_instance->doTick();

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
		state.Push();
		lua_settable(state, -3);
		lua_settable(state, -3);
	}
	state.DebugCall ( 2, 0 );
}

void MOAIHusky::HuskyObserverCloudDataDownloaded(const char *cloudfilename, void* buffer, int32_t bytes) {
	ZLLog::Print("Got download callback");
	if (!_cloudDataDownloadCallback)
		return;
	ZLLog::Print("There is a callback into lualand");
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
