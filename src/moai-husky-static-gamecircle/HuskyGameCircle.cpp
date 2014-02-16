#include "HuskyGameCircle.h"

HuskyGameCircle* HuskyGameCircle::instance;

HuskyGameCircle::HuskyGameCircle():
	_progressCallbackList(new ProgressCallbackList()),
	_observer(NULL),
	_cloudDataMap(NULL) {

}

HuskyGameCircle::~HuskyGameCircle() {
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

	ProgressCallbackList::iterator i = _progressCallbackList->begin();
	if (i != _progressCallbackList->end()) {
		_observer->HuskyObserverAchievementCallback(i->name, i->success);
		_progressCallbackList->erase(i);
	}
}

void HuskyGameCircle::setAchievement(const char* name) {
	AmazonGames::AchievementsClientInterface::updateProgress(name, 100, this, 0);
}

void HuskyGameCircle::resetAchievements() {
	printf("HuskyGameCircle: Cannot reset Game Circle achievements from client");
}

void HuskyGameCircle::uploadLeaderboardScore(const char *name, int32_t score, HuskyLeaderboardScoreToKeep tokeep, int64_t extradata) {

}

void HuskyGameCircle::requestLeaderboardScores(const char *name, bool friends, HuskyLeaderboardScoreTimeFrame timeframe, int offset, int number) {

}

void HuskyGameCircle::requestLeaderboardScoresNearPlayer(const char *name, bool friends, HuskyLeaderboardScoreTimeFrame timeframe, int offset, int number) {

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
	if (_observer == NULL)
		return;
	ProgressCallback callback;
	size_t namelength = strlen(responseStruct->achievementId);
	callback.name = (char*)calloc(sizeof(char), namelength + 1);
	memcpy(callback.name, responseStruct->achievementId, namelength);
	callback.success = errorCode == 0;
	_progressCallbackList->push_back(callback);
}
