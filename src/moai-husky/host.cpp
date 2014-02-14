// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-husky/host.h>
#include <moai-husky/MOAIHusky.h>

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
