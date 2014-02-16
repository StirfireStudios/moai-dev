#include "HuskyGameCircle.h"

HuskyGameCircle* HuskyGameCircle::instance;

HuskyGameCircle::HuskyGameCircle():
	_progressCallbackList(new NamedCallbackList()),
	_leaderboardSetCallbackList(new NamedCallbackList()),
	_observer(NULL),
	_cloudDataMap(NULL),
	_leaderboardTag(0),
	_leaderboardTagNameMap(new LeaderboardTagNameMap()),
	_leaderboardNameTagMap(new LeaderboardNameTagMap()),
	_leaderboardScoresCallbackList(new LeaderboardScoresCallbackList()),
	_leaderboardPlayerScoreCallbackList(new LeaderboardPlayerScoreCallbackList()),
	_playerId(NULL),
	_playerAlias(NULL) {
}

HuskyGameCircle::~HuskyGameCircle() {
	delete(_progressCallbackList);
	delete(_leaderboardNameTagMap);
	delete(_leaderboardTagNameMap);
}

HuskyGameCircle* HuskyGameCircle::getInstance() {
	if(!instance) {
		instance = new HuskyGameCircle();
	}
	return instance;	
}

void HuskyGameCircle::shutdownInstance() {
	if (instance)
		delete(instance);
}

void HuskyGameCircle::setObserver(HuskyObserver* observer) {
	_observer = observer;
}

uint16_t HuskyGameCircle::getCapabilities() {
	return HuskyHasAchievements;
}

void HuskyGameCircle::doTick() {
	if (_observer == NULL)
		return;

	NamedCallbackList::iterator i = _progressCallbackList->begin();
	if (i != _progressCallbackList->end()) {
		_observer->HuskyObserverAchievementCallback(i->name, i->success);
		_progressCallbackList->erase(i);
	}

	i = _leaderboardSetCallbackList->begin();
	if (i != _leaderboardSetCallbackList->end()) {
		_observer->HuskyObserverLeaderboardScoreSetCallback(i->name, i->success);
		_leaderboardSetCallbackList->erase(i);
	}

	LeaderboardPlayerScoreCallbackList::iterator i1 = _leaderboardPlayerScoreCallbackList->begin();
	if (i1 != _leaderboardPlayerScoreCallbackList->end()) {
		if (!i1->success) {
			_observer->HuskyObserverLeaderboardScoreGetCallback(i1->name, NULL, 0);	
		} else if (_playerAlias != NULL) {
			HuskyLeaderboardEntry entry;
			entry.name = _playerAlias;
			entry.globalrank = i1->rank;
			entry.score = i1->score;
			entry.data = 0;
			_observer->HuskyObserverLeaderboardScoreGetCallback(i1->name, &entry, 1);	
			_leaderboardPlayerScoreCallbackList->erase(i1);
		} else {
			AmazonGames::ProfilesClientInterface::getLocalPlayerProfile(this, 0);
		}
	}

	LeaderboardScoresCallbackList::iterator i2 = _leaderboardScoresCallbackList->begin();
	if (i2 != _leaderboardScoresCallbackList->end()) {
		_observer->HuskyObserverLeaderboardScoreGetCallback(i2->name, i2->entries, i2->number);
		_leaderboardScoresCallbackList->erase(i2);
	}
}

void HuskyGameCircle::setAchievement(const char* name) {
	AmazonGames::AchievementsClientInterface::updateProgress(name, 100, this, 0);
}

void HuskyGameCircle::resetAchievements() {
	printf("HuskyGameCircle: Cannot reset Game Circle achievements from client");
}

int HuskyGameCircle::getLeaderboardTag(const char *name) {
	int tag;
	LeaderboardNameTagMap::iterator value = _leaderboardNameTagMap->find(name);
	if (value == _leaderboardNameTagMap->end()) {
		_leaderboardNameTagMap->insert(LeaderboardNameTagMap::value_type(name, _leaderboardTag));
		_leaderboardTagNameMap->insert(LeaderboardTagNameMap::value_type(_leaderboardTag, name));
		tag = _leaderboardTag;
		_leaderboardTag++;
	} else {
		tag = value->second;
	}
	return tag;
}

char* HuskyGameCircle::getLeaderboardName(int tag) {
	LeaderboardTagNameMap::iterator value = _leaderboardTagNameMap->find(tag);
	if (value != _leaderboardTagNameMap->end())
		return (char*)value->second.c_str();
	else
		return NULL;
}

AmazonGames::LeaderboardFilter HuskyGameCircle::getFilterForSettings(bool friends, HuskyLeaderboardScoreTimeFrame timeframe) {
	if (friends)
		return AmazonGames::FRIENDS_ALL_TIME;
	switch(timeframe) {
		case HuskyLeaderboardTodaysScores:
			return AmazonGames::GLOBAL_DAY;
		case HuskyLeaderboardWeeksScores:
			return AmazonGames::GLOBAL_WEEK;
		default:
			return AmazonGames::GLOBAL_ALL_TIME;
	}
}

void HuskyGameCircle::uploadLeaderboardScore(const char *name, int32_t score, HuskyLeaderboardScoreToKeep tokeep, int64_t extradata) {
	int tag = getLeaderboardTag(name);
	AmazonGames::LeaderboardsClientInterface::submitScore(name, score, this, tag);
}

void HuskyGameCircle::requestLeaderboardScores(const char *name, bool friends, HuskyLeaderboardScoreTimeFrame timeframe, int offset, int number) {
	int tag = getLeaderboardTag(name);
	AmazonGames::LeaderboardFilter filter;
	AmazonGames::LeaderboardsClientInterface::getScores(name, filter, this, tag);
}

void HuskyGameCircle::requestLeaderboardScoresNearPlayer(const char *name, bool friends, HuskyLeaderboardScoreTimeFrame timeframe, int offset, int number) {
	int tag = getLeaderboardTag(name);
	AmazonGames::LeaderboardFilter filter;
	AmazonGames::LeaderboardsClientInterface::getPlayerScore(name, filter, this, tag);
}

void HuskyGameCircle::uploadCloudData(const char *cloudfilename, void* data, int32_t bytes) {
	if (_cloudDataMap == NULL)
			_cloudDataMap = AmazonGames::WhispersyncClient::getGameData();

	AmazonGames::SyncableString* syncString = _cloudDataMap->getLatestString(cloudfilename);
	syncString->set((char*)data);
	_observer->HuskyObserverCloudDataUploaded(cloudfilename, true);
}

void HuskyGameCircle::requestCloudData(const char *cloudfilename) {
	if (_cloudDataMap == NULL)
			_cloudDataMap = AmazonGames::WhispersyncClient::getGameData();

	char* stringData = (char*)_cloudDataMap->getLatestString(cloudfilename)->getValue();
	if (stringData != NULL) {
		_observer->HuskyObserverCloudDataDownloaded(cloudfilename, stringData, strlen(stringData));	
	} else {
		_observer->HuskyObserverCloudDataDownloaded(cloudfilename, stringData, 0);	
	}
}

/* AMAZON CALLBACKS */
void HuskyGameCircle::onUpdateProgressCb(AmazonGames::ErrorCode errorCode, const AmazonGames::UpdateProgressResponse* responseStruct, int developerTag) {
	NamedCallback callback;
	size_t namelength = strlen(responseStruct->achievementId);
	callback.name = (char*)calloc(sizeof(char), namelength + 1);
	memcpy(callback.name, responseStruct->achievementId, namelength);
	callback.success = errorCode == 0;
	_progressCallbackList->push_back(callback);
}

void HuskyGameCircle::onSubmitScoreCb(AmazonGames::ErrorCode errorCode, const AmazonGames::SubmitScoreResponse* responseStruct, int developerTag) {
	NamedCallback callback;
	callback.name = getLeaderboardName(developerTag);
	callback.success = errorCode == 0;
	_leaderboardSetCallbackList->push_back(callback);
}

void HuskyGameCircle::onGetPlayerScoreCb(AmazonGames::ErrorCode errorCode, const AmazonGames::PlayerScoreInfo* responseStruct, int developerTag) {
	LeaderboardPlayerScoreCallback callback;
	callback.name = getLeaderboardName(developerTag);
	callback.success = errorCode == 0;
	if (errorCode == 0) {
		callback.rank = responseStruct->rank;
		callback.score = responseStruct->scoreValue;
	}
	_leaderboardPlayerScoreCallbackList->push_back(callback);
}

void HuskyGameCircle::onGetScoresCb(AmazonGames::ErrorCode errorCode, const AmazonGames::LeaderboardScores* responseStruct, int developerTag) {
	LeaderboardScoresCallback callback;
	callback.name = getLeaderboardName(developerTag);
	if (errorCode == 0) {
		callback.number = responseStruct->numScores;
		callback.entries = (HuskyLeaderboardEntry*)malloc(sizeof(HuskyLeaderboardEntry) * responseStruct->numScores);
		int length;
		char* name;
		for(int i = 0; i < responseStruct->numScores; i++) {
			length = strlen(responseStruct->scores[i].playerAlias);
			name = (char*)calloc(sizeof(char), length + 1);
			memcpy(name, responseStruct->scores[i].playerAlias, length);
			callback.entries[i].name = name;
			callback.entries[i].globalrank = responseStruct->scores[i].rank;
			callback.entries[i].score = (int32_t)responseStruct->scores[i].scoreValue;
		}
	} else {
		callback.number = 0;
		callback.entries = NULL;
	}
	_leaderboardScoresCallbackList->push_back(callback);
}

void HuskyGameCircle::onGetLocalPlayerProfileCb(AmazonGames::ErrorCode errorCode, const AmazonGames::PlayerInfo* responseStruct, int developerTag) {
	if (errorCode == 0) {
		int length;
		if (_playerId != NULL)
			free(_playerId);
		length = strlen(responseStruct->playerId);
		_playerId = (char*)calloc(sizeof(char), length + 1);
		memcpy(_playerId, responseStruct->playerId, length);

		if (_playerAlias != NULL)
			free(_playerAlias);
		length = strlen(responseStruct->alias);
		_playerAlias = (char*)calloc(sizeof(char), length + 1);
		memcpy(_playerAlias, responseStruct->alias, length);
	}
}
