//
// Created by bigun on 12/28/2018.
//

#include <cstdint>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include "EasyBMP/EasyBMP.h"

#ifndef VBFEDIT_EIFIMAGE_H
#define VBFEDIT_EIFIMAGE_H

namespace fs = std::filesystem;
using namespace std;

namespace EIF {

static const uint8_t EIF_SIGNATURE[] = {'E','B','D',0x10,'E','I','F'};
static const int EIF_MULTICOLOR_PALETTE_SIZE = 0x300;
static const int EIF_MULTICOLOR_NUM_COLORS = 256;

enum EIF_TYPE : uint8_t {
    EIF_TYPE_MONOCHROME = 0x04,
    EIF_TYPE_MULTICOLOR = 0x07,
    EIF_TYPE_SUPERCOLOR = 0x0E
};

struct EifBaseHeader {
    uint8_t  signature[7];
    uint8_t  type;
    uint32_t length;
    uint16_t width;
    uint16_t height;
};

struct BitmapData {
    vector<uint8_t> bitmapRGBA;
    vector<uint8_t> bitmapAlpha;
    unsigned width;
    unsigned height;
};

class EifImageBase {
protected:
    unsigned width =0;
    unsigned height =0;
    vector<uint8_t> bitmap_data;
    virtual void store_palette(vector<uint8_t>& data) {};
public:
    virtual int openEif(const vector<uint8_t>& data) = 0;
    virtual int getType() = 0;
    virtual int openBmp(const fs::path& fileName) = 0;
    virtual void saveBmp(const fs::path& fileName) = 0;
    virtual void saveEif(const fs::path& fileName);
    virtual vector<uint8_t> saveEifToVector();
    virtual ~EifImageBase()= default;
    virtual unsigned getWidth() { return width; };
    virtual unsigned getHeight() { return height; };
};

class EifImage8bit: public EifImageBase {
    EIF_TYPE type = EIF_TYPE_MONOCHROME;
public:
    inline int getType() override { return type; };
    int openEif(const vector<uint8_t>& data) override;
    void saveBmp(const fs::path& file_name) override;
    int openBmp(const fs::path& file_name) override;
};

class EifImage16bit: public EifImageBase {
    EIF_TYPE type = EIF_TYPE_MULTICOLOR;
    vector<uint8_t> palette;
    uint8_t searchPixel(RGBApixel rgb_pixel);
    void store_palette(vector<uint8_t> &data) override;
public:
    inline int getType() override { return type; };
    EifImage16bit() = default;
    EifImage16bit(unsigned int w, unsigned int h, const vector<uint8_t> &pal,
                                 const vector<uint8_t> &bitmap, const vector<uint8_t> &alpha = vector<uint8_t>());
    int openEif(const vector<uint8_t>& data) override;
    void saveBmp(const fs::path& file_name) override;
    int openBmp(const fs::path& file_name) override;
    int setPalette(const vector<uint8_t>& data);
    void savePalette(const fs::path& file_name);
    int changePalette(const vector<uint8_t> &data);
    int setBitmap(const vector<uint8_t>& mapped_data);
    BitmapData getBitmap();
};

class EifImage32bit: public EifImageBase {
    EIF_TYPE type = EIF_TYPE_SUPERCOLOR;
public:
    inline int getType() override { return type; };
    int openEif(const vector<uint8_t>& data) override;
    int openBmp(const fs::path& file_name) override;
    void saveBmp(const fs::path& file_name) override;
};

class EifConverter {
public:
    static vector<EifImage16bit> mapMultiPalette(const vector<BitmapData>& bitmaps);
    static void eifToBmpFile(const vector<uint8_t>& data, const fs::path& out_file_name,
            const fs::path& palette_file_name = "");
    static void bmpFileToEifFile(const fs::path& file_name, uint8_t depth, const fs::path& out_file_name,
            const fs::path& palette_file_name = "");
    static int bulkPack(const fs::path& bmp_files, const fs::path& out_dir);
    static BitmapData bmpToBitmap(const fs::path& file);
};

}
#endif //VBFEDIT_EIFIMAGE_H

