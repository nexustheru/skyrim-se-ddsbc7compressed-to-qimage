// test.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include "IL/il.h"
#include "IL/ilu.h"
//

#include <float.h>
#include <string.h> // memset, memcpy
#include <d3d11.h>
#include <d3d10.h>
#include "DirectXTex.h"
#include "QtWidgets/qapplication.h"
#include "QtGui/qimage.h"
#include "QtGui/qpixmap.h"

struct rgba_t
{
	unsigned char   red;
	unsigned char green;
	unsigned char  blue;
	unsigned char  alpha;
};


void copypix(const DirectX::Image* img)
{
	size_t w=img->width;
	size_t h=img->height;
	const uint8_t* pPixels = img->pixels;/*<pointer to pixel data>*/;	 
	uint32_t PixelIndex = 0;

	QImage* qimg = new QImage(w, h, QImage::Format_RGB888);
	qimg->fill(QColor(Qt::white).rgb());


	for (size_t y = 0; y < img->height; ++y)
	{       
		for (size_t x = 0; x < img->width; ++x)
		{
			rgba_t ColorBuffer;
			ColorBuffer.red = pPixels[PixelIndex];
			ColorBuffer.green = pPixels[PixelIndex + 1];
			ColorBuffer.blue = pPixels[PixelIndex + 2];
			ColorBuffer.alpha = pPixels[PixelIndex + 3];
			// Advance
			pPixels += 4;		
			qimg->setPixel(x, y, qRgb(ColorBuffer.red, ColorBuffer.green, ColorBuffer.blue));
		}
		
	}
	qimg->save("femalehead.png");
}

void loadFile()
{	
	std::unique_ptr<DirectX::ScratchImage> image(new (std::nothrow) DirectX::ScratchImage);
	DirectX::TexMetadata info;
	
	HRESULT res = DirectX::LoadFromDDSFile(L"femalehead.dds", DirectX::DDS_FLAGS_NONE, &info, *image);
	 if (SUCCEEDED(res))
	 {
		 std::unique_ptr<DirectX::ScratchImage> cimage;
		if (DirectX::IsCompressed(info.format))
		{
			std::cout << "compressed" << std::endl;

		    auto img = image->GetImage(0, 0, 0);
	        size_t nimg = image->GetImageCount();

	        std::unique_ptr<DirectX::ScratchImage> timage(new (std::nothrow) DirectX::ScratchImage);
			HRESULT hr = DirectX::Decompress(img, nimg, info, DXGI_FORMAT_UNKNOWN , *timage);
				if (FAILED(hr))
				{
					wprintf(L" FAILED [decompress] (%x)\n", hr);
					
				}
				else
				{
					std::cout << "decompressed succesfully" << std::endl;
					auto& tinfo = timage->GetMetadata();
					info.format = tinfo.format;
					info.width == tinfo.width;
					info.height == tinfo.height;
					info.depth == tinfo.depth;
					info.arraySize == tinfo.arraySize;
					info.mipLevels == tinfo.mipLevels;
					info.miscFlags == tinfo.miscFlags;
					info.dimension == tinfo.dimension;
					image.swap(timage);
					cimage.reset();
					auto limg = image->GetImage(0, 0, 0);
					copypix(limg);
					//HRESULT hr = DirectX::SaveToTGAFile(limg[0], L"test.tga");					
					
				}
	    }
		
        std::cout << "" << std::endl;
	}
	
}

void loadFile1()
{
	std::string name="femalehands_1.dds";
	ILuint ImageName;
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);
	ILboolean success = ilLoadImage(name.c_str());
	ILenum error;
	if (success == IL_FALSE)
	{

		error = ilGetError();
		printf("%d: %s/n", error, iluErrorString(error));

	}
	else
	{
	std::cout << ilGetInteger(IL_IMAGE_WIDTH) << "width" << std::endl;
	std::cout << ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL) << "pixels" << std::endl;
	std::cout << ilGetInteger(IL_IMAGE_HEIGHT) << "height" << std::endl;
	iluFlipImage();
	ILubyte *Data = ilGetData();
	std::cout << sizeof(Data) << std::endl;
	}


}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	ilInit();
	iluInit();
	loadFile();
	return a.exec();
	system("PAUSE");
}

