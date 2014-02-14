// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-husky-static-stub/host.h>
#include <moai-husky-static-stub/MoaiHusky.h>

//================================================================//
// aku-util
//================================================================//

static bool sIsInitialized = false;

//----------------------------------------------------------------//
void AKUFinalizeHusky () {
	
	if ( !sIsInitialized ) return;
	sIsInitialized = false;
}

//----------------------------------------------------------------//
void AKUInitializeHusky () {

	if ( !sIsInitialized ) {
		sIsInitialized = true;
	}

	REGISTER_LUA_CLASS ( MOAIHusky ) 
}
