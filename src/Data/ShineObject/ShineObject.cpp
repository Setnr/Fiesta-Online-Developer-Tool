#include "ShineObject.h"
#include "FiestaOnlineTool.h"

NiImplementRTTI(ShineObject, NiPickable);


void ShineObject::UpdateActor(NiActorManagerPtr NewActor)
{
	bool BoundingBox = BoundingBoxIsVisible();
	if (BoundingBox)
		HideBoundingBox();
	if(_Actor)
		DetachChild(_Actor->GetNIFRoot());
	_Actor = NewActor;
	if (_Actor)
		AttachChild(_Actor->GetNIFRoot(), 0);

	CompactChildArray();
	UpdateProperties();
	UpdateEffects();
	Update(0.0f);

	if (BoundingBox)
		ShowBoundingBox();
}
