#include "FogChange.h"

NiImplementRTTI(ChangeNiColor, WorldChange);
NiImplementRTTI(FloatChange, WorldChange);
NiImplementRTTI(NodeChange, WorldChange);
NiImplementRTTI(UpdateNodePos, WorldChange);
NiImplementRTTI(UpdateNodeRotation, WorldChange);
NiImplementRTTI(UpdateNodeScale, WorldChange);
NiImplementRTTI(AttachingNode, WorldChange);
NiImplementRTTI(AttachingNodeList, WorldChange);
NiImplementRTTI(SHBDChange, WorldChange);
NiImplementRTTI(HTDGChange, WorldChange);
NiImplementRTTI(TextureChange, WorldChange);
NiImplementRTTI(LayerDiffuseChange, WorldChange);
NiImplementRTTI(LayerAdd, WorldChange);
NiImplementRTTI(LayerDelete, WorldChange);

NiImplementRTTI(FogChangeColor, ChangeNiColor);
NiImplementRTTI(ChangeDirectionalAmbientLightColor, ChangeNiColor);
NiImplementRTTI(ChangeDirectionalDiffuseLightColor, ChangeNiColor);
NiImplementRTTI(ChangeBackgroundColor, ChangeNiColor);
NiImplementRTTI(ChangeGlobalLightColor, ChangeNiColor);

NiImplementRTTI(FogChangeDepth, FloatChange);
NiImplementRTTI(ChangeFarFrustum, FloatChange);

NiImplementRTTI(GlobalLightNodeChange, NodeChange);