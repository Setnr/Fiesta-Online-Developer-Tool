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

#ifndef NISCREENELEMENTSDATA_H
#define NISCREENELEMENTSDATA_H

#include "NiTriShapeData.h"

class NIMAIN_ENTRY NiScreenElementsData : public NiTriShapeData
{
public:
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    // Construction and destruction.  The initial storage quantities for
    // polygons (iMaxPQuantity and iPGrowBy), vertices (iMaxVQuantity and
    // iVGrowBy), and triangles (iMaxTQuantity and iTGrowBy) have default
    // values for the common case of a single axis-aligned rectangle on the
    // screen.  The growth quantities for the containers also have default
    // values.  If you know how much storage is required for your application,
    // provide these values to avoid memory reallocations during the lifetime
    // of this object.
    NiScreenElementsData(bool bWantNormals, bool bWantColors,
        unsigned short usNumTextureSets, int iMaxPQuantity = 1,
        int iPGrowBy = 1, int iMaxVQuantity = 4, int iVGrowBy = 1,
        int iMaxTQuantity = 2, int iTGrowBy = 1);

    virtual ~NiScreenElementsData();

    // Primitive data member access.  The growth values should be positive.
    // Internally, this is ensured by setting them to one if the requested
    // values are nonpositive.
    int GetMaxPQuantity() const;
    void SetPGrowBy(int iPGrowBy);
    int GetPGrowBy() const;
    int GetMaxVQuantity() const;
    void SetVGrowBy(int iVGrowBy);
    int GetVGrowBy() const;
    int GetMaxTQuantity() const;
    void SetTGrowBy(int iTGrowBy);
    int GetTGrowBy() const;

    // Polygon management is transparent to the user.  All polygon operations
    // involve polygon handles, which are integer valued.  The validity of a
    // polygon handle may be tested with this function.
    bool IsValid(int iPolygon) const;

    // Polygon insertion.  The number of vertices must be positive.  An
    // optional triangle index array may be passed in.  If no array is passed
    // in (number of triangles is zero and array is NULL), the requested
    // polygon is assumed to be convex and a triangle fan is generated for
    // the polygon.  If an array is requested, the requested polygon can be
    // as complex as you like, even a collection of disjoint polygons.  The
    // input array is a const pointer, this class copies the values and the
    // caller is responsible for deleting the array in the event it was
    // dynamically allocated.  The return value is an opaque handle for the
    // created polygon.  See the comments for IsValid(...) regarding validity
    // of the handle.  On insertion, all you get is storage for the vertices
    // and their attributes.  You must fill them in with the Set* calls
    // listed later in the class interface.
    int Insert(unsigned short usNumVertices,
        unsigned short usNumTriangles = 0,
        const unsigned short* ausTriList = 0);

    // Polygon removal.  The return value is 'true' iff the input polygon
    // handle is valid.  After removal of one or more polygons, the polygon
    // handles that your application has stored for the remaining polygons
    // are still valid.  NOTE:  The handles are reused.  If you store a
    // handle to a polygon, remove the polygon, and then insert a new
    // polygon, the handle you stored might be valid, but it would be for a
    // polygon you are unaware of.
    bool Remove(int iPolygon);
    void RemoveAll();

    // The number of polygons in the container.
    int GetNumPolygons() const;

    // The number of vertices in the specified polygon.  The return value is
    // positive for a valid polygon handle; otherwise, the return value is
    // zero.
    int GetNumVertices(int iPolygon) const;

    // The number of triangles in the specified polygon.  The return value is
    // positive for a valid polygon handle; otherwise, the return value is
    // zero.
    int GetNumTriangles(int iPolygon) const;

    // Set and get vertices and attributes.  The return Boolean value
    // indicates the success of the operation.  It is 'false' if the input
    // polygon handle is invalid, if the input vertex index is out of range,
    // or if the input number of texture sets is invalid.

    // Set and get the polygon vertices.  The vertices (x,y) are in normalized
    // display coordinates (NDC); a point on the screen satisfies 0 <= x <= 1
    // and 0 <= y <= 1.  You may use NDC values outside the unit square;
    // for example, this is useful if you have GUI components that may be
    // translated by the user on/off the screen.  The {Set,Get}Vertices
    // functions require arrays with (at least) the same number of vertices
    // that the polygon has.  The {Set,Get}Rectangle functions are for
    // axis-aligned rectangles.
    //
    // If you have a known width w and height h, and you want to use screen
    // coordinates (x,y) relative to w and h, you may set a vertex using
    // SetVertex(polygon, vertex, NiPoint2(x/w, y/h)).  The ratios x/w and
    // y/w should be computed using floating-point arithmetic.
    bool SetVertex(int iPolygon, int iVertex, const NiPoint2& kValue);
    bool GetVertex(int iPolygon, int iVertex, NiPoint2& kValue) const;
    bool SetVertices(int iPolygon, const NiPoint2* akValue);
    bool GetVertices(int iPolygon, NiPoint2* akValue) const;
    bool SetRectangle(int iPolygon, float fLeft, float fTop, float fWidth,
        float fHeight);
    bool GetRectangle(int iPolygon, float& fLeft, float& fTop, float& fWidth,
        float& fHeight) const;

    // Set and get the vertex normals.  The {Set,Get}Normals functions that
    // take array inputs require arrays with (at least) the same number of
    // normals as the number of polygon vertices.  The last {Set,Get}Normals
    // pair is for axis-aligned rectangles.
    bool SetNormal(int iPolygon, int iVertex, const NiPoint3& kValue);
    bool GetNormal(int iPolygon, int iVertex, NiPoint3& kValue) const;
    bool SetNormals(int iPolygon, const NiPoint3* akValue);
    bool SetNormals(int iPolygon, const NiPoint3& kCommonValue);
    bool GetNormals(int iPolygon, NiPoint3* akValue) const;
    bool SetNormals(int iPolygon, const NiPoint3& kUpperLeft,
        const NiPoint3& kLowerLeft, const NiPoint3& kLowerRight,
        const NiPoint3& kUpperRight);
    bool GetNormals(int iPolygon, NiPoint3& kUpperLeft, NiPoint3& kLowerLeft,
        NiPoint3& kLowerRight, NiPoint3& kUpperRight) const;

    // Set and get the vertex colors.  The {Set,Get}Colors functions that
    // take array inputs require arrays with (at least) the same number of
    // colors as the number of polygon vertices.  The last {Set,Get}Colors
    // pair is for axis-aligned rectangles.
    bool SetColor(int iPolygon, int iVertex, const NiColorA& kValue);
    bool GetColor(int iPolygon, int iVertex, NiColorA& kValue) const;
    bool SetColors(int iPolygon, const NiColorA* akValue);
    bool SetColors(int iPolygon, const NiColorA& kCommonValue);
    bool GetColors(int iPolygon, NiColorA* akValue) const;
    bool SetColors(int iPolygon, const NiColorA& kUpperLeft,
        const NiColorA& kLowerLeft, const NiColorA& kLowerRight,
        const NiColorA& kUpperRight);
    bool GetColors(int iPolygon, NiColorA& kUpperLeft, NiColorA& kLowerLeft,
        NiColorA& kLowerRight, NiColorA& kUpperRight) const;

    // Set and get the texture coordinates.  The {Set,Get}Textures functions
    // that take array inputs require arrays with (at least) the same number
    // of texture coordinates as the number of polygon vertices. The
    // SetTextures function that takes four floating-point values is for
    // axis-aligned rectangles for which the texture subimage mapped to it is
    // an axis-aligned rectangle.  The last {Set,Get}Textures pair is for
    // axis-aligned rectangles.
    bool SetTexture(int iPolygon, int iVertex, unsigned short usSet,
        const NiPoint2& kValue);
    bool GetTexture(int iPolygon, int iVertex, unsigned short usSet,
        NiPoint2& kValue) const;
    bool SetTextures(int iPolygon, unsigned short usSet,
        const NiPoint2* akValue);
    bool GetTextures(int iPolygon, unsigned short usSet, NiPoint2* akValue)
        const;
    bool SetTextures(int iPolygon, unsigned short usSet, float fLeft,
        float fTop, float fRight, float fBottom);
    bool SetTextures(int iPolygon, unsigned short usSet,
        const NiPoint2& kUpperLeft, const NiPoint2& kLowerLeft,
        const NiPoint2& kLowerRight, const NiPoint2& kUpperRight);
    bool GetTextures(int iPolygon, unsigned short usSet,
        NiPoint2& kUpperLeft, NiPoint2& kLowerLeft, NiPoint2& kLowerRight,
        NiPoint2& kUpperRight) const;

    // Access to vertex arrays.  If you hold onto a pointer value, and then
    // insert and/or remove polygons, the pointer values returned from these
    // functions might no longer point to what they used to.  If you modify
    // vertices or attributes, you must call the function
    // MarkAsChanged(NiGeometryData::FOOBAR_MASK), where FOOBAR is in
    // {VERTEX, NORMAL, COLOR, TEXTURE}.
    NiPoint3* GetVertices(int iPolygon);
    const NiPoint3* GetVertices(int iPolygon) const;
    NiPoint3* GetNormals(int iPolygon);
    const NiPoint3* GetNormals(int iPolygon) const;
    NiColorA* GetColors(int iPolygon);
    const NiColorA* GetColors(int iPolygon) const;
    NiPoint2* GetTextures(int iPolygon, unsigned short usSet);
    const NiPoint2* GetTextures(int iPolygon, unsigned short usSet) const;

    // Access to index arrays.  If you modify indices, you must call
    // MarkAsChanged(NiTriBasedGeomData::TRIANGLE_INDEX_MASK).  It is not
    // possible to modify the number of indices directly.  Instead, you must
    // remove the old polygon and insert a new one.
    unsigned short* GetIndices(int iPolygon);
    const unsigned short* GetIndices(int iPolygon) const;

    // Update the model-space bounding volume.  The bound-needs-update flag
    // is automatically set when polygons are inserted or removed and when
    // vertices are modified.
    bool BoundNeedsUpdate() const;
    void UpdateBound();

    // Resizing to support the UI system.  The input scales should be
    // positive.  The returned Boolean value is 'true' iff the operation was
    // successful (the inputs are positive).
    bool Resize (float fXScale, float fYScale);

private:
    // Streaming support.
    NiScreenElementsData();

    // Screen elements have normals pointing out of the screen, so disallow
    // calculating the normals.  NOTE:  If you access NiScreenElementsData
    // through a base-class pointer, you will circumvent this call stopper.
    virtual void CalculateNormals() { /**/ }

    // Polygon array management.  The active number of polygons is
    // m_usPQuantity.
    class Polygon : public NiMemObject
    {
    public:
        unsigned short m_usNumVertices;
        unsigned short m_usVOffset;  // offset into the vertex arrays
        unsigned short m_usNumTriangles;
        unsigned short m_usIOffset;  // offset into the index array
    };

    // Get a polygon from a handle.  The input handle must be valid.
    Polygon& GetPolygon(int iPolygon);
    const Polygon& GetPolygon(int iPolygon) const;

    Polygon* m_akPolygon;
    unsigned short* m_ausPIndexer;
    unsigned short m_usMaxPQuantity, m_usPGrowBy, m_usPQuantity;

    // Vertex array management.  The active number of vertices is
    // m_usVertices, including those not used due to removed polygons.
    unsigned short m_usMaxVQuantity, m_usVGrowBy;

    // Index array management.  The active number of indices is
    // 3*m_usTriangles.
    unsigned short m_usMaxIQuantity, m_usIGrowBy;

    // Set to 'true' whenever vertex positions are modified or when polygons
    // are removed.
    bool m_bBoundNeedsUpdate;

    static const unsigned short ms_usInvalid;
};

NiSmartPointer(NiScreenElementsData);
#include "NiScreenElementsData.inl"

#endif //NISCREENELEMENTSDATA_H
