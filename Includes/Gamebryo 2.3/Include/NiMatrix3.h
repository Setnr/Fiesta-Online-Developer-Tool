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

#ifndef NIMATRIX3_H
#define NIMATRIX3_H

// NOTE.  The(x,y,z) coordinate system is assumed to be right-handed.
// Coordinate axis rotation matrices are of the form
//   RX =    1       0       0
//           0     cos(t)  sin(t)
//           0    -sin(t)  cos(t)
// where t > 0 indicates a clockwise rotation in the yz-plane,
//   RY =  cos(t)    0    -sin(t)
//           0       1       0
//         sin(t)    0     cos(t)
// where t > 0 indicates a clockwise rotation in the zx-plane, and
//   RZ =  cos(t)  sin(t)    0
//        -sin(t)  cos(t)    0
//           0       0       1
// where t > 0 indicates a clockwise rotation in the xy-plane.  It is
// important to understand that positive angles are associated with clockwise
// rotations.  This is not the usual convention in applications.  The
// original version of Gamebryo used the positive/clockwise convention.  The
// decision was made not to convert to the standard convention to avoid
// clients having to rewrite large amounts of code and re-export their models.

#include "NiPoint3.h"

#if defined(_XENON) || defined (_PS3)
#include "NiPoint4.h"
#endif  //#if defined(_XENON)

class NiStream;

#if defined(_XENON)
__declspec(align(16)) // Allows for typecasting to XMFLOAT4X4, XMFLOAT4X4A16
#endif
class NIMAIN_ENTRY NiMatrix3 : public NiMemObject
{
public:
    // Declare NiTransform as a friend so that platform specific math routines
    // can get access to the internal vector representation of NiMatrix3.
    friend class NiTransform;

    NiMatrix3();
    NiMatrix3(const NiPoint3& col0, const NiPoint3& col1, 
        const NiPoint3& col2);

    //  Row access functions
    void GetRow(unsigned int uiRow, NiPoint3& row) const;
    void SetRow(unsigned int uiRow, const NiPoint3& row);
    void GetRow(unsigned int uiRow, float& f0, float& f1, float& f2) const;
    void SetRow(unsigned int uiRow, float f0, float f1, float f2);
    void GetRow(unsigned int uiRow, float* pRow) const;
    void SetRow(unsigned int uiRow, const float* pRow);

    //  Column access functions
    void GetCol(unsigned int uiCol, NiPoint3& col) const;
    void SetCol(unsigned int uiCol, const NiPoint3& col);
    void GetCol(unsigned int uiCol, float& f0, float& f1, float& f2) const;
    void SetCol(unsigned int uiCol, float f0, float f1, float f2);
    void GetCol(unsigned int uiCol, float* pCol) const;
    void SetCol(unsigned int uiCol, const float* pCol);

    //  Entry access functions
    float GetEntry(unsigned int uiRow, unsigned int uiCol) const;
    void SetEntry(unsigned int uiRow, unsigned int uiCol, float fEntry);

    void MakeZero();
    void MakeIdentity();
    void MakeDiagonal(float fDiag0, float fDiag1, float fDiag2);
    void MakeXRotation(float angle);
    void MakeYRotation(float angle);
    void MakeZRotation(float angle);
    void MakeRotation(float angle, float x, float y, float z);
    void MakeRotation(float angle, const NiPoint3& axis);

    bool operator==(const NiMatrix3& mat) const;
    bool operator!=(const NiMatrix3& mat) const { return !(*this == mat); }

    NiMatrix3 operator+(const NiMatrix3& mat) const;
    NiMatrix3 operator-(const NiMatrix3& mat) const;
    NiMatrix3 operator*(const NiMatrix3& mat) const;

    NiMatrix3 operator*(float fScalar) const;

    // use this operator to compute MATRIX*VECTOR
    NiPoint3 operator*(const NiPoint3& pt) const;

#ifdef _PS3
    NiPoint4 operator*(const NiPoint4& pt) const;
#endif

    // Use this operator to compute Transpose(VECTOR)*MATRIX.  This function
    // is used to avoid having to explicitly transpose MATRIX when computing
    // Transpose(MATRIX)*VECTOR.
    NIMAIN_ENTRY friend NiPoint3 operator*(const NiPoint3& pt,
        const NiMatrix3& mat);

    // utilities
    NiMatrix3 Inverse() const;
    bool Inverse(NiMatrix3& inv) const;
    NiMatrix3 Transpose() const;
    void ExtractAngleAndAxis(float& fAngle, float& x, float& y,
        float& z) const;

    // The matrix must be orthonormal for these routines to produce
    // meaningful results.  The return value of the "To" calls indicates
    // whether or not there was a unique factorization.
    bool ToEulerAnglesXYZ(float& rfXAngle, float& rfYAngle,
        float& rfZAngle) const;
    bool ToEulerAnglesXZY(float& rfXAngle, float& rfZAngle,
        float& rfYAngle) const;
    bool ToEulerAnglesYXZ(float& rfYAngle, float& rfXAngle,
        float& rfZAngle) const;
    bool ToEulerAnglesYZX(float& rfYAngle, float& rfZAngle,
        float& rfXAngle) const;
    bool ToEulerAnglesZXY(float& rfZAngle, float& rfXAngle,
        float& rfYAngle) const;
    bool ToEulerAnglesZYX(float& rfZAngle, float& rfYAngle,
        float& rfXAngle) const;
    void FromEulerAnglesXYZ(float fXAngle, float fYAngle, float fZAngle);
    void FromEulerAnglesXZY(float fXAngle, float fZAngle, float fYAngle);
    void FromEulerAnglesYXZ(float fYAngle, float fXAngle, float fZAngle);
    void FromEulerAnglesYZX(float fYAngle, float fZAngle, float fXAngle);
    void FromEulerAnglesZXY(float fZAngle, float fXAngle, float fYAngle);
    void FromEulerAnglesZYX(float fZAngle, float fYAngle, float fXAngle);

    bool Reorthogonalize();

    // compute Transpose(this)*mat
    NiMatrix3 TransposeTimes(const NiMatrix3& mat) const;

    // Compute the congruence transformation rot*this*Transpose(rot).  The
    // matrix 'rot' must be orthogonal.  The congruence transformation is
    // a special form of a similarity transformation mat*this*Inverse(mat)
    // where 'mat' can be any invertible matrix.
    //
    // For Transpose(rot)*this*rot, use rot.TransposeTimes(mat)*rot.
    NiMatrix3 Congruence(const NiMatrix3& rot) const;

    // batch operations for rigid transforms(for efficiency purposes)
    static void TransformVertices(const NiMatrix3& rot, const NiPoint3& trn,
        unsigned int uiVertexCount, const NiPoint3* pInVertex,
        NiPoint3* pOutVertex);
    static void TransformNormals(const NiMatrix3& rot,
        unsigned int uiNormalCount, const NiPoint3* pInNormal,
        NiPoint3* pOutNormal);
    static void TransformVerticesAndNormals(const NiMatrix3& rot,
        const NiPoint3& trn, unsigned int uiCount, const NiPoint3* pInVertex,
        NiPoint3* pOutVertex, const NiPoint3* pInNormal,
        NiPoint3* pOutNormal);

    // eigensolving
    void EigenSolveSymmetric(float afEigenvalue[3],
        NiPoint3 akEigenvector[3]);

    static const NiMatrix3 ZERO;
    static const NiMatrix3 IDENTITY;

    // *** begin Emergent internal use only ***
#if defined(WIN32) 
    // It is possible to generate denormalized float values for each index,
    // this method will clamp very small values to zero.
    void Snap();
#endif

    // streaming
    void LoadBinary(NiStream& stream);
    void SaveBinary(NiStream& stream);
    char* GetViewerString(const char* pPrefix) const;

    // *** end Emergent internal use only ***

protected:
#if defined(_XENON) || defined (_PS3)
    NiPoint4 m_pEntry[3];
#else
    float m_pEntry[3][3];
#endif

    // Support for the eigendecomposition of a matrix.  The return value of
    // Tridiagonal is 'true' if a Householder transformation was applied to
    // the matrix (a reflection) and 'false' if no transformation was
    // applied (the identity, a rotation).  The return value of QLAlgorithm
    // is 'true' iff the iterations converged to produce a diagonal matrix.
    bool Tridiagonal(float afDiag[3], float afSubd[2]);
    bool QLAlgorithm(float afDiag[3], float afSubd[2]);
};

#include "NiMatrix3.inl"

#endif // NIMATRIX3_H
