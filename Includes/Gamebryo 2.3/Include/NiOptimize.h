// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2007 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net

#ifndef NIOPTIMIZE_H
#define NIOPTIMIZE_H

#include "NiOptimizationLibType.h"
#include <NiMemManager.h>
#include <NiUniversalTypes.h>
#include <NiDevImageConverter.h>
#include <NiTArray.h>
#include <NiTPointerMap.h>
#include <NiTPointerList.h>
#include <NiTriShape.h>

// Forward declarations.
class NiNode;
class NiAVObject;
class NiVisController;
class NiTexturingProperty;
class NiTransformController;
class NiPoint3;
class NiMatrix3;
class NiQuaternion;
class NiFloatKey;
class NiProperty;
class NiGeometry;
class NiRotKey;
class NiPosKey;
class NiTriBasedGeom;
class NiSkinInstance;
class NiTriShapeData;

typedef NiTPrimitiveArray<NiNode*> NiNodeArray;
typedef NiTPrimitiveArray<NiVisController*> NiVisControllerArray;
typedef NiTPrimitiveArray<NiTPointerList<NiProperty*>*>      
    NiTPropertyPointerListArray;
typedef NiTPrimitiveArray<NiProperty*> NiPropertyArray;
typedef NiTObjectArray<NiTriShapePtr> NiTriShapePtrArray;


#define NI_BONES_PER_PARTITION_KEY "NiBonesPerPartition"

class NIOPTIMIZATION_ENTRY NiOptimize
{
public:
    //***********************************************************************
    // General functions.
    //***********************************************************************
    //***********************************************************************
    // Optimization functions.
    //***********************************************************************
    // These functions are called via Optimize when the appropriate options
    // are active. They may also be called directly.

    // Removes all the duplicate properties in the scene graph rooted at
    // pkObject.
    static void RemoveDupProperties(NiAVObject* pkObject);

    // Detects the presence of childless nodes that can be removed by
    // RemoveChildlessNodes and stores a pointer to each in apkNodes.
    static bool CollectChildlessNodes(NiNode* pkNode,
        NiNodeArray& apkNodes);

    // Removes all childless nodes. A node will not be removed if it is
    // singled out by BuildHashTable or has an "sgoKeep" extra data tag.
    static void RemoveChildlessNodes(NiNode* pkNode);

    // Detects the presence of multi-material nodes that can be removed by
    // RemoveMultiMtlNodes and stores a pointer to each in apkNodes.
    static bool CollectMultiMtlNodes(NiNode* pkNode,
        NiNodeArray& apkNodes);

    // Recursively removes all multi-material nodes. A node will not be
    // removed if it is singled out by BuildHashTable, has an "sgoKeep" extra
    // data tag, or has any controllers.
    static void RemoveMultiMtlNodes(NiNode* pkNode);

    // Detects the presence of single-child nodes that can be removed by
    // RemoveSingleChildNodes and stores a pointer to each in apkNodes.
    static bool CollectSingleChildNodes(NiNode* pkNode,
        NiNodeArray& apkNodes);

    // Recursively removes all single-child nodes in the scene of exact type
    // NiNode, propagating their properties, transforms and names to the
    // single child. A node will not be removed if it is singled out by
    // BuildHashTable, has an "sgoKeep" extra data tag, has any
    // controllers, or whose child has any controllers.
    static void RemoveSingleChildNodes(NiNode* pkNode);

    // Recursively merges sibling nodes. A node will not be merged if it is
    // singled out by BuildHashTable, has an "sgoKeep" extra data tag, has
    // any controllers, has an ABV, has any extra data, or whose children
    // have any controllers.
    // Two nodes will be merged if the following holds:
    // The nodes are
    //     sibling nodes,
    //     of exact type NiNode,
    // and they have
    //     a parent of exact type NiNode,
    //     the same properties,
    //     the same AppCulled flag.
    // If two nodes can be merged and they have different transforms on them,
    // the transforms for both nodes are pushed down to their children before
    // they are merged.
    static void MergeSiblingNodes(NiNode* pkNode);

    // Recurses through the scene graph, removing degenerate triangles and
    // collapsing vertices of all NiTriShape objects, deleting them if all
    // their triangles are degenerate. The first function recurses through
    // the scene graph calling the second function.
    static void OptimizeTriShapes(NiNode* pkNode);
    static bool OptimizeTriShape(NiTriShape* pkShape);

    // Recursively merges all sibling NiTriShapes except those singled out
    // by BuildHashTable and those with an "sgoKeep" extra data tag. If
    // bNoMergeOnAlpha is true, recursive merging does not occur below nodes
    // with an NiAlphaProperty attached when alpha blending is enabled or
    // sorters are allowed. Two NiTriShapes will not be merged if either of
    // them has any extra data attached.
    // Two NiTriShapes can be merged if the following holds:
    // They have
    //     a common parent of exact type NiNode,
    //     the same number of properties,
    //     the same set of property pointers,
    //     the same number of texture sets,
    //     no time controllers attached,
    //     different NiGeometryData pointers,
    //     the same AppCulled flag,
    // and they both/neither have
    //     vertices,
    //     normals,
    //     textures,
    //     colors.
    static void MergeSiblingTriShapes(NiNode* pkNode,
        bool bNoMergeOnAlpha);

    // Removes all UV sets in geometry that are not used by any
    // NiTexturingProperty attached to the current or a higher node.
    static void RemoveExtraUVSets(NiAVObject* pkObject);

    // Recursively removes normals that are not needed. Will not remove
    // normals from scenes without lights.
    static void RemoveUnnecessaryNormals(NiAVObject* pkObject);

    // Detects the presence of any visibility controllers in the scene whose
    // keys all have the same visibility flag. Adds a pointer to each
    // offending controller to apkCtlrs.
    static bool CollectUnnecessaryVisControllers(NiAVObject* pkObject,
        NiVisControllerArray& apkCtlrs);

    // Removes all visibility controllers whose keys all have the same
    // visibility data. If that data is not the same as the AppCulled flag of
    // pkObject, pkObject's AppCulled flag is updated to reflect the
    // visibility data.
    static void RemoveUnnecessaryVisControllers(NiAVObject* pkObject);

    // Removes the name string from all objects and their material
    // properties.
    static void RemoveNames(NiAVObject* pkObject);

    // Adds unique names to all objects in the scene that have none.
    static void NameUnnamedObjects(NiAVObject* pkObject);

    // Detects the presence of NiTexturingProperties with no image data and
    // those that use a nonexistent set of texture coordinates. Will ignore
    // bad texturing properties on NiParticles objects. Stores a pointer
    // to each bad texturing property in kBadProps.
    static bool CollectBadTexturingProps(NiAVObject* pkObject,
        NiTPointerMap<NiTexturingProperty*,bool>& kBadProps);

    // Removes NiTexturingProperties with no image data and those that use
    // a nonexistent set of texture coordinates. This fuction only removes
    // bad texturing properties if they are directly attached to the
    // objects that use them (and aren't just inherited from ancestors).
    // Will not remove bad texturing properties on NiParticles objects.
    static void RemoveBadTexturingProps(NiAVObject* pkObject);

    // Removes extra data strings that are empty or that contain the strings
    // "zMode" or "billboard". If bRemoveKeepTags is true, extra data strings
    // that contain the string "sgoKeep" are also removed.
    static void RemoveUnnecessaryExtraData(NiAVObject* pkObject,
        bool bRemoveKeepTags);

    // Removes NiLookAtControllers in the scene if there are no keyframe
    // controllers or path controllers on either the target or the LookAt
    // of the controller or above them in the scene graph.
    static void RemoveUnnecessaryLookAtControllers(NiAVObject* pkObject);

    // Removes any NiGeometry object that is a child of a bone and whose
    // AppCulled flag is true.
    static void RemoveHiddenBoneGeometry(NiAVObject* pkObject);

    // Reduces redundant animation keys for all NiTransformControllers in the
    // scene graph rooted at pkObject. fTolerance specifies a ratio of
    // how far off keys are allowed to be to still be considered redundant.
    static void ReduceAnimationKeys(NiAVObject* pkObject, float fTolerance);

    // Reduces redundant animation keys for the specified
    // NiTransformController fTolerance specifies a ratio of how far off keys
    // are allowed to be to still be considered redundant.
    static void ReduceAnimationKeys(NiTransformController* pkCtlr,
        float fTolerance);

    // Recursively sorts all the children of each node by the address of the
    // NiTexturingProperty present in their property states. Children with no
    // NiTexturingProperty in their property state attached are sorted first.
    static void SortByTexturingProp(NiNode* pkNode);

    // Detects if there are any images whose dimensions are not powers of two.
    // A pointer to each improperly sized texture is stored in kBadTex.
    static bool CollectImproperlySizedTextures(NiAVObject* pkObject,
        NiTPointerMap<NiTexture*,bool>& kBadTex);

    // Optimize skin data for platform
    static void OptimizeSkinData(NiAVObject* pkObject, bool bHardwareSkin, 
        bool bSoftwareSkin, unsigned int uiBonesPerPartition,
        bool bForcePalette);

    // Weld together any inter-object cracks created by skin partitioning
    // code.
    static void WeldSkin(NiAVObject* pkScene);

    // Guarantee keys at start and end and remove single keys arrays.
    static void CleanUpAnimationKeys(NiAVObject* pkObject);

    // Remove AppCulled objects.
    static void RemoveAppCulledObjects(NiAVObject* pkObject);

    //*********************************************************
    // Helper functions.
    //*********************************************************
    static const float ms_fEpsilon;

    // Determines if float fA is within fEpsilon of float fB.
    static bool CloseTo(float fA, float fB, float fEpsilon = ms_fEpsilon);

    // Determines if point kP1 is within fEpsilon of point kP2.
    static bool Point3CloseTo(const NiPoint3& kP1, const NiPoint3& kP2,
        float fEpsilon = ms_fEpsilon);

    // Determines if matrix kM1 is within fEpsilon of matrix kM2.
    static bool Matrix3CloseTo(const NiMatrix3& kM1, const NiMatrix3& kM2,
        float fEpsilon = ms_fEpsilon);

    // Determines if quaternion kQ1 is within fEpsilon of quaternion kQ2.
    static bool QuatCloseTo(const NiQuaternion& kQ1, const NiQuaternion& kQ2,
        float fEpsilon = ms_fEpsilon);

    // Determines if the provided integer is a power of two.
    static bool IsPowerOf2(unsigned int uiNum);
    
    // Checks if a given triangle is degenerate.  Returns false if triangle
    // *IS* degenerate, thus the name.
    static bool NonDegenerateTriangle(unsigned int uiI0, unsigned uiI1,
        unsigned int uiI2, const NiPoint3& kP0, const NiPoint3& kP1, 
        const NiPoint3& kP2);

    // NiTriShape optimization functions.  Previously these were members
    // of NiTriShape, but will live in the optimization library going
    // forward.    
    static bool RemoveDegenerateTriangles(NiTriShape* pkShape);
    static void CollapseVertices(NiTriShape* pkShape, 
        unsigned short** ppusNewToOld = 0);

    // Removes redundant NiLinFloatKeys.
    static void ReduceLinFloatKeys(unsigned int uiNumKeys,
        NiFloatKey* pkKeys, unsigned int& uiNumNewKeys,
        NiFloatKey*& pkNewKeys, float fTolerance);

       // Checks for extra data strings that indicate an object should not
    // be removed.
    static bool CheckForExtraDataTags(NiAVObject* pkObject);
    
    // Checks for the string extra data tags in ppcTags
    static bool CheckForExtraDataTags(NiAVObject* pkObject, 
        const char* ppcTags[], unsigned int uiNumTags);

    // Builds a hash table containing the pointers of objects that should
    // not be removed by removal or merge functions. The following objects
    // are added to kMap:
    // - LookAt targets of NiLookAtControllers
    // - objects referenced by particle systems
    // - affected nodes of dynamic effects
    // - all bones
    // - Biped Footsteps
    // - Objects with ABV's
    // - Objects used with the NiPortal system
    static void BuildHashTable(NiAVObject* pkObject,
        NiTPointerMap<NiAVObject*,bool>& kMap);

private:

    //*********************************************************
    // Optimization functions.
    //*********************************************************
    // Collects childless nodes.
    static void CollectChildlessNodes(NiNode* pkNode,
        NiNodeArray& apkNodes,
        const NiTPointerMap<NiAVObject*, bool>& kMap);

    // Removes childless nodes.
    static bool RemoveChildlessNodes(NiNode* pkNode,
        const NiTPointerMap<NiAVObject*, bool>& kMap);

    // Collects multi-material nodes.
    static void CollectMultiMtlNodes(NiNode* pkNode,
        NiNodeArray& apkNodes,
        const NiTPointerMap<NiAVObject*, bool>& kMap);

    // Removes multi-material nodes.
    static bool RemoveMultiMtlNodes(NiNode* pkNode,
        const NiTPointerMap<NiAVObject*, bool>& kMap);

    // Collects single-child nodes.
    static void CollectSingleChildNodes(NiNode* pkNode,
        NiNodeArray& apkNodes,
        const NiTPointerMap<NiAVObject*, bool>& kMap);

    // Removes single-child nodes.
    static bool RemoveSingleChildNodes(NiNode* pkNode,
        const NiTPointerMap<NiAVObject*, bool>& kMap);

    // Merges sibling nodes.
    static void MergeSiblingNodes(NiNode* pkNode,
        const NiTPointerMap<NiAVObject*, bool>& kMap);

    // Removes bad texturing properties.
    static void RemoveBadTexturingProps(NiAVObject* pkObject,
        NiTPointerMap<NiTexturingProperty*, bool>& kBadProps);

    // Removes hidden bone geometry.
    static void RemoveHiddenBoneGeometry(NiAVObject* pkObject,
        const NiTPointerMap<NiAVObject*, bool>& kMap);

    // Merges sibling NiTriShape objects.
    static void MergeSiblingTriShapes(NiNode* pkNode, bool bNoMergeOnAlpha,
        const NiTPointerMap<NiAVObject*, bool>& kMap);

    // Sorts by NiTexturingProperty.
    static void SortByTexturingProp(NiNode* pkNode,
        NiTexturingProperty*& pkLoProp, NiTexturingProperty*& pkHiProp);

    // Remove AppCulled objects.
    static void RemoveAppCulledObjects(NiAVObject* pkObject,
        const NiTPointerMap<NiAVObject*, bool>& kMap);

    //*********************************************************
    // Helper functions.
    //*********************************************************

    // Checks to determin if a property is unique compared to a list of
    // properties. If not unique it will pass back the duplicate property.
    static bool IsPropertyUnique(NiProperty* pkProp, 
        NiTPointerList<NiProperty*>& pkPropList, NiProperty* &pkDupProp);

    // Recursively walk the scene graph remove all duplicate properties. 
    // Properties are checked against an array of unique property lists.
    // There is one array element for each type of property.
    static void RecursiveRemoveDupProperties(NiAVObject* pkObject,
        NiTPropertyPointerListArray& apaUniqueProps);

    // Recursively collects all properties of the specified type and stores
    // pointers to them in apkProps.
    static void CollectProperties(NiAVObject* pkObject,
        NiPropertyArray& apkProps, unsigned int uiType);

    // Propagates properties from pkObj1 to pkObj2.
    static void PropagateProperties(NiAVObject* pkObj1, NiAVObject* pkObj2);

    // Propagates transforms from pkObj1 to pkObj2.
    static void PropagateTransforms(NiAVObject* pkObj1, NiAVObject* pkObj2);

    // Checks whether or not two objects have the same properties.
    static bool CompareProperties(NiAVObject* pkObj1, NiAVObject* pkObj2);

    // Checks whether or not two objects have the same transforms.
    static bool CompareTransforms(NiAVObject* pkObj1, NiAVObject* pkObj2);

    // Tests whether or not the two geometry objects can be merged. They can
    // be merged if the following is true:
    // They have:
    //     the same or no shader,
    //     the same number of properties,
    //     the same set of property pointers,
    //     the same number of texture sets,
    //     different NiGeometryData pointers,
    //     the same consistency flags,
    //     the same keep flags,
    //     the same compression flags,
    // and they both/neither have:
    //     vertices,
    //     normals,
    //     textures,
    //     colors.
    static bool CanMergeTriShape(NiGeometry* pkObj1, NiGeometry* pkObj2);

    // Merges all the NiTriShape objects in aspMergeShapes and returns the
    // first NiTriShape in aspMergeShapes containing all the merged data.
    // Transforms on all NiTriShape objects are pushed into the vertex and
    // normal data.
    static NiTriShapePtr MergeTriShapes(
        const NiTriShapePtrArray& aspMergeShapes);

    // Finds the NiTexturingProperty attached to the object or one
    // of its ancestors.
    static NiTexturingProperty* FindTexturingProperty(NiAVObject* pkObject);

    //this will find all the instances to the input geometry
    static void FindInstances(
        const NiGeometryData* pkInputGeometryData,
        const NiAVObject* pkNode,
        NiTPrimitiveArray<NiGeometry*>* pkInstances);

    //This recurses up the scene graph and finds the root
    static NiAVObject* FindRoot(const NiAVObject* pkNode);

    // Searches for keyframe controllers or path controllers on the current
    // object and, if bCheckUpTree is true, on its ancestors.
    static bool CheckForTransformControllers(NiAVObject* pkObject,
        bool bCheckUpTree);

    // Checks whether or not the texture dimensions are powers of two.
    static bool CheckTextureSize(NiTexture* pkTexture);

    // Determines if normals can be removed from the geometry object.
    static bool CanRemoveNormals(NiGeometry* pkObject);

    // Removes redundant NiLinRotKeys.
    static void ReduceLinRotKeys(unsigned int uiNumKeys, NiRotKey* pkKeys,
        unsigned int& uiNumNewKeys, NiRotKey*& pkNewKeys, 
        float fRotationTolerance, NiAVObject* pkObject);

    // Removes redundant NiLinPosKeys.
    static void ReduceLinPosKeys(unsigned int uiNumKeys, NiPosKey* pkKeys,
        unsigned int& uiNumNewKeys, NiPosKey*& pkNewKeys, float fTolerance,
        NiAVObject* pkObject);

    // Removes redundant NiEulerRotKeys.
    static void ReduceEulerRotKeys(unsigned int uiNumKeys, NiRotKey* pkKeys,
        float fTolerance);

    // Removes redundant NiBezRotKeys.
    static void ReduceBezRotKeys(unsigned int uiNumKeys, NiRotKey* pkKeys,
        unsigned int& uiNumNewKeys, NiRotKey*& pkNewKeys,
        float fRotationTolerance, NiAVObject* pkObject);

    // Removes redundant NiBezPosKeys.
    static void ReduceBezPosKeys(unsigned int uiNumKeys, NiPosKey* pkKeys,
        unsigned int& uiNumNewKeys, NiPosKey*& pkNewKeys, float fTolerance,
        NiAVObject* pkObject);

    // Removes redundant NiBezFloatKeys.
    static void ReduceBezFloatKeys(unsigned int uiNumKeys,
        NiFloatKey* pkKeys, unsigned int& uiNumNewKeys,
        NiFloatKey*& pkNewKeys, float fTolerance);

    // Stores a pointer to each child of bones in the scene to kMap.
    static void CollectBoneChildren(NiAVObject* pkObject,
        NiTPointerMap<NiAVObject*, bool>& kMap);

    // Creates a skin partition and removes bone vert data, if appropriate.
    static void OptimizeSkinInstance(NiTriBasedGeom* pkGeom,
        NiSkinInstance* pkSkinInstance, bool bHardwareSkin, 
        bool bSoftwareSkin, unsigned int uiBonesPerPartition,
        bool bForcePalette);

    // NiTriShape optimization helper functions
    static void RemoveUnusedVertices(NiTriShape* pkShape,
        unsigned short** ppusNewToOld);
    static void ReallocateVertexArrays(NiTriShapeData* pkData,
        unsigned short usNewVertices, int* piNewIndex);

    // Used for texture sorting.
    class TexSortObj : public NiMemObject
    {
    public:
        NiAVObject* m_pkObject;
        NiTexturingProperty* m_pkLoProp;
        NiTexturingProperty* m_pkHiProp;
    };
};

#include "NiOptimize_OptFunc.inl"
#include "NiOptimize_HelpFunc.inl"

#endif  // #ifndef NIOPTIMIZE_H
