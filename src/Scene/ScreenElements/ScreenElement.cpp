#include "ScreenElement.h"

#include <Scene/ScreenElements/LuaElement/LuaElement.h>
#include <Scene/ScreenElements/LoadObject/AddSingleObject.h>
#include <Scene/ScreenElements/LoadObject/AddMultipleObject.h>
#include <Scene/ScreenElements/LoadObject/ReplaceObjects.h>

NiImplementRootRTTI(ScreenElement);

NiImplementRTTI(LuaElement, ScreenElement);
NiImplementRTTI(AddMultipleObject, ScreenElement);
NiImplementRTTI(AddSingleObject, ScreenElement);
NiImplementRTTI(ReplaceObjects, ScreenElement); 