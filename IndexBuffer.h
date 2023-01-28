#pragma once

#include "stdafx.h"
#include "typedefs3D.h"

class IndexBuffer final
{
public:
   enum Format {
#ifdef ENABLE_SDL
      FMT_INDEX16 = 16,
      FMT_INDEX32 = 32
#else
      FMT_INDEX16 = D3DFMT_INDEX16,
      FMT_INDEX32 = D3DFMT_INDEX32
#endif
   };

   enum LockFlags
   {
#ifdef ENABLE_SDL
      WRITEONLY,
      NOOVERWRITE,
      DISCARDCONTENTS
#else
      WRITEONLY = 0,                        // in DX9, this is specified during VB creation
      NOOVERWRITE = D3DLOCK_NOOVERWRITE,    // meaning: no recently drawn vertices are overwritten. only works with dynamic VBs.
                                            // it's only needed for VBs which are locked several times per frame
      DISCARDCONTENTS = D3DLOCK_DISCARD     // discard previous contents; only works with dynamic VBs
#endif
   };

   IndexBuffer(RenderDevice* rd, const unsigned int numIndices, const DWORD usage, const IndexBuffer::Format format);
   static IndexBuffer* CreateAndFillIndexBuffer(RenderDevice* rd, const unsigned int numIndices, const unsigned int* indices);
   static IndexBuffer* CreateAndFillIndexBuffer(RenderDevice* rd, const unsigned int numIndices, const WORD* indices);
   static IndexBuffer* CreateAndFillIndexBuffer(RenderDevice* rd, const vector<unsigned int>& indices);
   static IndexBuffer* CreateAndFillIndexBuffer(RenderDevice* rd, const vector<WORD>& indices);

   static void bindNull() { m_curIndexBuffer = nullptr; }

   void lock(const unsigned int offsetToLock, const unsigned int sizeToLock, void** dataBuffer, const DWORD flags);
   void unlock();
   void release();
   void bind();

   static IndexBuffer* m_curIndexBuffer; // for caching

private:
   RenderDevice* m_rd;
   DWORD m_usage;
   unsigned int m_sizePerIndex;
   Format m_indexFormat;

#ifdef ENABLE_SDL
public:
   GLuint getOffset() const { return m_offset; }
   Format getIndexFormat() const { return m_indexFormat; }

private:
   GLuint m_count;
   GLuint m_size;
   GLuint m_offset;
   bool m_isUploaded;
   bool m_sharedBuffer;

   // CPU memory management
   unsigned int m_offsetToLock;
   unsigned int m_sizeToLock;
   void* m_dataBuffer = nullptr;

   //GPU memory management
   GLuint m_Buffer = 0;

   void UploadData();
   void addToNotUploadedBuffers();
   static vector<IndexBuffer*> notUploadedBuffers;
   static void UploadBuffers();
#else
   IDirect3DIndexBuffer9* m_ib = nullptr;
#endif
};
