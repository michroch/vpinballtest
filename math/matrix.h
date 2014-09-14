#pragma once

// 3x3 matrix for representing linear transformation of 3D vectors
class Matrix3
{
public:
   inline Matrix3()
   {
      Identity();
   }

   inline void scaleX(const float factor)
   {
      m_d[0][0] *= factor;
   }
   inline void scaleY(const float factor)
   {
      m_d[1][1] *= factor;
   }
   inline void scaleZ(const float factor)
   {
      m_d[2][2] *= factor;
   }

   inline void CreateSkewSymmetric(const Vertex3Ds &pv3D)
   {
      m_d[0][0] = 0; m_d[0][1] = -pv3D.z; m_d[0][2] = pv3D.y;
      m_d[1][0] = pv3D.z; m_d[1][1] = 0; m_d[1][2] = -pv3D.x;
      m_d[2][0] = -pv3D.y; m_d[2][1] = pv3D.x; m_d[2][2] = 0;
   }

   inline void MultiplyScalar(const float scalar)
   {
      for (int i=0; i<3; ++i)
         for (int l=0; l<3; ++l)
            m_d[i][l] *= scalar;
   }

   template <class VecType>
   inline Vertex3Ds operator* (const VecType& v) const
   {
      return Vertex3Ds(
         m_d[0][0] * v.x + m_d[0][1] * v.y + m_d[0][2] * v.z,
         m_d[1][0] * v.x + m_d[1][1] * v.y + m_d[1][2] * v.z,
         m_d[2][0] * v.x + m_d[2][1] * v.y + m_d[2][2] * v.z);
   }

   template <class VecType>
   inline Vertex3Ds MultiplyVector(const VecType& v) const
   {
	   return (*this) * v;
   }

   // multiply vector with matrix transpose
   template <class VecType>
   inline Vertex3Ds MultiplyVectorT(const VecType& v) const
   {
      return Vertex3Ds(
         m_d[0][0] * v.x + m_d[1][0] * v.y + m_d[2][0] * v.z,
         m_d[0][1] * v.x + m_d[1][1] * v.y + m_d[2][1] * v.z,
         m_d[0][2] * v.x + m_d[1][2] * v.y + m_d[2][2] * v.z);
   }

   inline void MultiplyMatrix(const Matrix3 * const pmat1, const Matrix3 * const pmat2)
   {
      Matrix3 matans;
      for(int i=0; i<3; ++i)
         for(int l=0; l<3; ++l)
            matans.m_d[i][l] = pmat1->m_d[i][0] * pmat2->m_d[0][l] +
            pmat1->m_d[i][1] * pmat2->m_d[1][l] +
            pmat1->m_d[i][2] * pmat2->m_d[2][l];

      // Copy the final values over later.  This makes it so pmat1 and pmat2 can
      // point to the same matrix.
      for(int i=0; i<3; ++i)
         for (int l=0; l<3; ++l)
            m_d[i][l] = matans.m_d[i][l];
   }

   inline void AddMatrix(const Matrix3 * const pmat1, const Matrix3 * const pmat2)
   {
      for (int i=0; i<3; ++i)
         for (int l=0; l<3; ++l)
            m_d[i][l] = pmat1->m_d[i][l] + pmat2->m_d[i][l];
   }

   inline void OrthoNormalize()
   {
      Vertex3Ds vX(m_d[0][0], m_d[1][0], m_d[2][0]);
      Vertex3Ds vY(m_d[0][1], m_d[1][1], m_d[2][1]);
      vX.Normalize();
      Vertex3Ds vZ = CrossProduct(vX, vY);
      vZ.Normalize();
      vY = CrossProduct(vZ, vX);
      vY.Normalize();

      m_d[0][0] = vX.x; m_d[0][1] = vY.x; m_d[0][2] = vZ.x;
      m_d[1][0] = vX.y; m_d[1][1] = vY.y; m_d[1][2] = vZ.y;
      m_d[2][0] = vX.z; m_d[2][1] = vY.z; m_d[2][2] = vZ.z;
   }

   inline void Transpose(Matrix3 * const pmatOut) const
   {
      for(int i=0; i<3; ++i)
      {
         pmatOut->m_d[0][i] = m_d[i][0];
         pmatOut->m_d[1][i] = m_d[i][1];
         pmatOut->m_d[2][i] = m_d[i][2];
      }
   }

   inline void Identity(const float value = 1.0f)
   {
      m_d[0][0] = m_d[1][1] = m_d[2][2] = value;
      m_d[0][1] = m_d[0][2] = 
         m_d[1][0] = m_d[1][2] = 
         m_d[2][0] = m_d[2][1] = 0.0f;
   }

   // Create matrix for rotating around an arbitrary vector
   // NB: axis must be normalized
   // NB: this actually rotates by -angle in right-handed coordinates
   void RotationAroundAxis(const Vertex3Ds& axis, const float angle)
   {
       const float rsin = sinf(angle);
       const float rcos = cosf(angle);

       m_d[0][0] = axis.x*axis.x + rcos*(1.0f-axis.x*axis.x);
       m_d[1][0] = axis.x*axis.y*(1.0f-rcos) - axis.z*rsin;
       m_d[2][0] = axis.z*axis.x*(1.0f-rcos) + axis.y*rsin;

       m_d[0][1] = axis.x*axis.y*(1.0f-rcos) + axis.z*rsin;
       m_d[1][1] = axis.y*axis.y + rcos*(1.0f-axis.y*axis.y);
       m_d[2][1] = axis.y*axis.z*(1.0f-rcos) - axis.x*rsin;

       m_d[0][2] = axis.z*axis.x*(1.0f-rcos) - axis.y*rsin;
       m_d[1][2] = axis.y*axis.z*(1.0f-rcos) + axis.x*rsin;
       m_d[2][2] = axis.z*axis.z + rcos*(1.0f-axis.z*axis.z);
   }

   float m_d[3][3];
};


////////////////////////////////////////////////////////////////////////////////


// 4x4 matrix for representing affine transformations of 3D vectors
class Matrix3D : public D3DMATRIX
{
public:
	inline Matrix3D() {}
	inline Matrix3D(const float Scale) { SetScaling(Scale,Scale,Scale); }

    // premultiply the given matrix, i.e., result = mult * (*this)
    inline void Multiply(const Matrix3D &mult, Matrix3D &result) const
    {
        Matrix3D matrixT;
        for (int i=0; i<4; ++i)
        {
            for (int l=0; l<4; ++l)
            {
                matrixT.m[i][l] = (m[0][l] * mult.m[i][0]) + (m[1][l] * mult.m[i][1]) +
                    (m[2][l] * mult.m[i][2]) + (m[3][l] * mult.m[i][3]);
            }
        }
        result = matrixT;
    }

    inline void SetTranslation(const float tx, const float ty, const float tz)
    {
        SetIdentity();
        _41 = tx;
        _42 = ty;
        _43 = tz;
    }
    inline void SetTranslation(const Vertex3Ds& t)
    {
        SetTranslation(t.x, t.y, t.z);
    }

    inline void SetScaling(const float sx, const float sy, const float sz)
    {
        SetIdentity();
        _11 = sx;
        _22 = sy;
        _33 = sz;
    }

    inline void RotateXMatrix(const float x)
    {
        SetIdentity();
        _22 = _33 = cosf(x);
        _23 = sinf(x);
        _32 = -_23;
    }

    inline void RotateYMatrix(const float y)
    {
        SetIdentity();
        _11 = _33 = cosf(y);
        _31 = sinf(y);
        _13 = -_31;
    }

    inline void RotateZMatrix(const float z)
    {
        SetIdentity();
        _11 = _22 = cosf(z);
        _12 = sinf(z);
        _21 = -_12;
    }

    inline void SetIdentity()
    {
        _11 = _22 = _33 = _44 = 1.0f;
        _12 = _13 = _14 = _41 =
        _21 = _23 = _24 = _42 =
        _31 = _32 = _34 = _43 = 0.0f;
    }
    inline void Scale(const float x, const float y, const float z)
    {
        _11 *= x; _12 *= x; _13 *= x;
        _21 *= y; _22 *= y; _23 *= y;
        _31 *= z; _32 *= z; _33 *= z;
    }

    // extract the matrix corresponding to the 3x3 rotation part
    inline void GetRotationPart(Matrix3D& rot)
    {
        rot._11 = _11; rot._12 = _12; rot._13 = _13; rot._14 = 0.0f;
        rot._21 = _21; rot._22 = _22; rot._23 = _23; rot._24 = 0.0f;
        rot._31 = _31; rot._32 = _32; rot._33 = _33; rot._34 = 0.0f;
        rot._41 = rot._42 = rot._43 = 0.0f; rot._44 = 1.0f;
    }

    // generic multiply function for everything that has .x, .y and .z
    template <class VecIn, class VecOut>
    inline void MultiplyVector(const VecIn& vIn, VecOut& vOut) const
    {
        // Transform it through the current matrix set
        const float xp = _11*vIn.x + _21*vIn.y + _31*vIn.z + _41;
        const float yp = _12*vIn.x + _22*vIn.y + _32*vIn.z + _42;
        const float zp = _13*vIn.x + _23*vIn.y + _33*vIn.z + _43;
        const float wp = _14*vIn.x + _24*vIn.y + _34*vIn.z + _44;

        const float inv_wp = 1.0f/wp;
        vOut.x = xp*inv_wp;
        vOut.y = yp*inv_wp;
        vOut.z = zp*inv_wp;
    }

    inline Vertex3Ds MultiplyVector(const Vertex3Ds &v) const
    {
        // Transform it through the current matrix set
        const float xp = _11*v.x + _21*v.y + _31*v.z + _41;
        const float yp = _12*v.x + _22*v.y + _32*v.z + _42;
        const float zp = _13*v.x + _23*v.y + _33*v.z + _43;
        const float wp = _14*v.x + _24*v.y + _34*v.z + _44;

        const float inv_wp = 1.0f/wp;
        Vertex3Ds pv3DOut;
        pv3DOut.x = xp*inv_wp;
        pv3DOut.y = yp*inv_wp;
        pv3DOut.z = zp*inv_wp;
        return pv3DOut;
    }

    inline Vertex3Ds MultiplyVectorNoTranslate(const Vertex3Ds &v) const
    {
        // Transform it through the current matrix set
        const float xp = _11*v.x + _21*v.y + _31*v.z;
        const float yp = _12*v.x + _22*v.y + _32*v.z;
        const float zp = _13*v.x + _23*v.y + _33*v.z;

        Vertex3Ds pv3DOut;
        pv3DOut.x = xp;
        pv3DOut.y = yp;
        pv3DOut.z = zp;
        return pv3DOut;
    }

    template <class VecIn, class VecOut>
    inline void MultiplyVectorNoTranslate(const VecIn& vIn, VecOut& vOut) const
    {
        // Transform it through the current matrix set
        const float xp = _11*vIn.x + _21*vIn.y + _31*vIn.z;
        const float yp = _12*vIn.x + _22*vIn.y + _32*vIn.z;
        const float zp = _13*vIn.x + _23*vIn.y + _33*vIn.z;

        vOut.x = xp;
        vOut.y = yp;
        vOut.z = zp;
    }

    void Invert();

   inline void Transpose()
   {
	  Matrix3D tmp;
      for(int i=0; i<4; ++i)
      {
         tmp.m[0][i] = m[i][0];
         tmp.m[1][i] = m[i][1];
         tmp.m[2][i] = m[i][2];
         tmp.m[3][i] = m[i][3];
      }
	  memcpy(this->m,tmp.m,4*4*sizeof(float));
   }
};
