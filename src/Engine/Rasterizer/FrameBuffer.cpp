#include <AEEngine.h> // f32, u32, etc...
#include "FrameBuffer.h"
#include "Color.h"

// file io
#include <fstream>
#include <sstream>

#define COLOR_COMP 4

namespace Rasterizer
{
	// ------------------------------------------------------------------------
	// Our framebuffer
	unsigned char *	FrameBuffer::frameBuffer = NULL;
	unsigned int		FrameBuffer::frameBufferWidth = 0;
	unsigned int		FrameBuffer::frameBufferHeight = 0;

	// ---------------------------------------------------------------------------
	// \fn		Allocate
	// \brief	Allocate memory for the frame buffer given by the width and height. 
	bool FrameBuffer::Allocate(unsigned int width, unsigned int height)
	{
		// free any data
		Delete();

		frameBufferWidth = width;
		frameBufferHeight = height;
		frameBuffer = new unsigned char[width * height * COLOR_COMP];
		if (frameBuffer)
		{
			Clear(0, 0, 0);
			return true;
		}
		return false;
	}
	// ---------------------------------------------------------------------------
	// \fn		Delete
	// \brief	Free the memory allocated in the function above. 
	void FrameBuffer::Delete()
	{
		// TODO delete the data

		if (frameBuffer)
			delete[] frameBuffer;
	}

	// ---------------------------------------------------------------------------
	// \fn		Present
	// \brief	Draws the contents of the frame buffer to the screen using Alpha
	//			Engine.
	void FrameBuffer::Present()
	{
		if (frameBuffer && frameBufferWidth != 0 && frameBufferHeight != 0) {
			auto tex = AEGfxTextureLoad(frameBufferWidth, frameBufferHeight, frameBuffer);
			AEGfxTriStart();
			AEGfxTriAdd(
				-0.5f, 0.5f, AE_COLORS_WHITE, 0, 1,
				-0.5f, -0.5f, AE_COLORS_WHITE, 0, 0,
				0.5f, -0.5f, AE_COLORS_WHITE, 1, 0);
			AEGfxTriAdd(
				-0.5f, 0.5f, AE_COLORS_WHITE, 0, 1,
				0.5f, -0.5f, AE_COLORS_WHITE, 1, 0,
				0.5f, 0.5f, AE_COLORS_WHITE, 1, 1);
			auto quad = AEGfxTriEnd();

			AEGfxTextureSet(tex);
			AEMtx33 mtx = AEMtx33::Scale((float)AEGetSysWindowWidth(), (float)AEGetSysWindowHeight());
			AEGfxSetTransform(&mtx);
			AEGfxTriDraw(quad);

			AEGfxTextureUnload(tex);
			AEGfxTriFree(quad);
		}
	}

	// ---------------------------------------------------------------------------
	// \fn		GetBufferData
	// \brief	Returns the pointer to the frame buffer variable
	unsigned char *	FrameBuffer::GetBufferData()
	{
		return frameBuffer;
	}

	// ---------------------------------------------------------------------------
	// \fn		GetWidth
	// \brief	Returns the width of the frame buffer.
	unsigned int		FrameBuffer::GetWidth()
	{
		return frameBufferWidth;
	}

	// ---------------------------------------------------------------------------
	// \fn		GetHeight
	// \brief	Returns the height of the frame buffer.
	unsigned int		FrameBuffer::GetHeight()
	{
		return frameBufferHeight;
	}

	// ---------------------------------------------------------------------------
	// \fn		Clear
	// \brief	Sets the entire frame buffer to the provided color.
	void FrameBuffer::Clear(const Color & c)
	{
		Clear(unsigned char(c.r * 255.0f),
			unsigned char(c.g * 255.0f),
			unsigned char(c.b * 255.0f),
			unsigned char(c.a * 255.0f));
	}

	// ---------------------------------------------------------------------------
	// \fn		Clear
	// \brief	Sets the entire frame buffer to the provided color in rgb format
	void FrameBuffer::Clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		if (frameBuffer)
		{
			unsigned int totalSize = frameBufferWidth * frameBufferHeight * COLOR_COMP;
			for (unsigned int i = 0; i < totalSize; i += COLOR_COMP)
			{
				frameBuffer[i] = r;
				frameBuffer[i + 1] = g;
				frameBuffer[i + 2] = b;
				frameBuffer[i + 3] = a;
			}
		}
	}

	// ---------------------------------------------------------------------------
	// \fn		SetPixel
	// \brief	Sets the pixel at position x, y to the provided color. 
	void FrameBuffer::SetPixel(unsigned int  x, unsigned int  y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		// Sanity check
		if (NULL == frameBuffer || x >= frameBufferWidth || y >= frameBufferHeight)
			return;

		// advance to pixel
		unsigned int startOffset = COLOR_COMP * (y * frameBufferWidth + x);

		// set
		frameBuffer[startOffset] = r;
		frameBuffer[startOffset + 1] = g;
		frameBuffer[startOffset + 2] = b;
		frameBuffer[startOffset + 3] = a;
	}

	// ---------------------------------------------------------------------------
	// \fn		SetPixel
	// \brief	Sets the pixel at position x, y to the provided color. 
	void FrameBuffer::SetPixel(unsigned int  x, unsigned int  y, const Color& c)
	{
		SetPixel(x, y,
			unsigned char(c.r * 255.0f),
			unsigned char(c.g * 255.0f),
			unsigned char(c.b * 255.0f),
			unsigned char(c.a * 255.0f));
	}

	// ---------------------------------------------------------------------------
	// \fn		GetPixel
	// \brief	Returns the color of the pixel at position x, y.
	Color FrameBuffer::GetPixel(unsigned int  x, unsigned int  y)
	{
		// Sanity check
		if (NULL == frameBuffer || x >= frameBufferWidth || y >= frameBufferHeight)
			return Color();

		// advance to pixel
		unsigned int  startOffset = COLOR_COMP * (y * frameBufferWidth + x);

		// Get the color component
		unsigned char r = frameBuffer[startOffset];
		unsigned char g = frameBuffer[startOffset + 1];
		unsigned char b = frameBuffer[startOffset + 2];
		unsigned char a = frameBuffer[startOffset + 3];

		// Convert to color class
		return Color((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, (float)a/255.0f);
	}

	// ---------------------------------------------------------------------------
	// \fn		SaveToFile
	// \brief	Saves the frame buffer to a binary file. 
	void FrameBuffer::SaveToFile(const char *filename)
	{
		// Sanity Check
		if (!filename)
			return;

		// try to open the file
		std::fstream fp(filename, std::ios::out | std::ios::binary);

		if (fp.is_open() && fp.good())
		{
			// write header information - just width and height
			fp.write(reinterpret_cast<const char*>(&frameBufferWidth), sizeof(unsigned int ));
			fp.write(reinterpret_cast<const char*>(&frameBufferHeight), sizeof(unsigned int ));

			// write pixel data
			fp.write(reinterpret_cast<const char*>(frameBuffer), frameBufferWidth * frameBufferHeight * COLOR_COMP);

			// close the file
			fp.close();
		}
	}

	// ---------------------------------------------------------------------------
	// \fn		LoadFromFile
	// \brief	Load the frame buffer from a binary file. 
	void FrameBuffer::LoadFromFile(const char * filename)
	{
		// Sanity check
		if (!filename)
			return;

		// try to open the file
		std::fstream fp(filename, std::ios::in | std::ios::binary);

		if (fp.is_open() && fp.good())
		{
			unsigned int  fbWidth, fbHeight;
			fp.read(reinterpret_cast<char *>(&fbWidth), sizeof(unsigned int ));
			fp.read(reinterpret_cast<char *>(&fbHeight), sizeof(unsigned int ));

			// re-allocate the data if necessary
			if (NULL != frameBuffer && (fbWidth * fbHeight) != (frameBufferWidth * frameBufferHeight))
			{
				delete[] frameBuffer;
				frameBuffer = new unsigned char[fbWidth * fbHeight * COLOR_COMP];
			}
			// no frame buffer, allocate one
			else if (NULL == frameBuffer)
			{
				frameBuffer = new unsigned char[fbWidth * fbHeight * COLOR_COMP];
			}

			// store new width and new height
			frameBufferWidth = fbWidth;
			frameBufferHeight = fbHeight;

			// now read the framebuffer data
			fp.read(reinterpret_cast<char *>(frameBuffer), frameBufferWidth * frameBufferHeight * COLOR_COMP);

			// close the file
			fp.close();
		}
	}

	// ---------------------------------------------------------------------------
	// \fn		SaveToImageFile
	// \brief	Save the frame buffer to image file. 
	void FrameBuffer::SaveToImageFile(const char * filename)
	{
		AEGfxSaveImagePNG(filename, frameBuffer, frameBufferWidth, frameBufferHeight);
	}

	// Extra Challenges
	void FrameBuffer::ClearCheckerboard(unsigned int  colors[2], unsigned int  size)
	{
	}

	void FrameBuffer::LoadFromImageFile(const char* filename) 
	{
	}

	void FrameBuffer::CheckerboardImage(const char* filename, unsigned int  size)
	{
	}
}
