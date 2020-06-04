//
// Created by user on 04.05.2020.
//

#ifndef FORDTOOLS_IMAGESECTION_H
#define FORDTOOLS_IMAGESECTION_H

#include <vector>
#include <cstdint>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

struct zip_file {
    uint32_t width;
    uint32_t height;
    uint8_t  img_type;
    char     fileName[31];
};

struct ttf_file {
    char fileName[32];
};

enum image_type : uint8_t {
    EIF_TYPE_MONOCHROME = 0x04,
    EIF_TYPE_MULTICOLOR = 0x07,
    EIF_TYPE_SUPERCOLOR = 0x0E,
    EIF_TYPE_UNKNOWN = 0x00,
};

inline const char* ToString(image_type v)
{
    switch (v)
    {
        case EIF_TYPE_MONOCHROME: return "MONOCHROME";
        case EIF_TYPE_MULTICOLOR: return "MULTICOLOR";
        case EIF_TYPE_SUPERCOLOR: return "SUPERCOLOR";
        default:      return "[Unknown] ";
    }
}

inline image_type FromString(const std::string& type)
{
    if(type == "MONOCHROME") {
        return EIF_TYPE_MONOCHROME;
    } else if (type == "MULTICOLOR") {
        return EIF_TYPE_MULTICOLOR;
    }  else if (type == "SUPERCOLOR") {
        return EIF_TYPE_SUPERCOLOR;
    }

    return EIF_TYPE_UNKNOWN;
}

class ImageSection {

public:

    enum enResType : uint8_t {
        RT_TTF,
        RT_ZIP
    };
    ImageSection()=default;
    void Parse(const vector<uint8_t>& bin_data);
    void ParseFile(const fs::path &file);
    int Export(const fs::path &out_path, const string& name_prefix = "");
    int Import(const fs::path &config_path);
    int SaveToFile(const fs::path &out_path);
    int SaveToVector(vector<uint8_t>& v);

    int GetItemsCount(enResType res_type);
    int GetItemData(enResType res_type, unsigned idx, vector<uint8_t>& bin_data);
    int AddItem();
    int DelItem();

private:

    struct Item {
        enResType res_type;
        uint32_t  width;     // for zip only
        uint32_t  height;    // for zip only
        uint8_t   img_type;  // for zip only
        std::vector<uint8_t> data;
    };

    static uint32_t FindSectionData(const std::vector<uint8_t>& bin_data);
    static void GetItemData(const std::vector<uint8_t>& bin_data, const char* fileName, std::vector<uint8_t>& out);
    uint32_t m_unknownInt = 0;

    std::vector<uint8_t> m_header_data;
    std::vector<Item> m_zip_vec;
    std::vector<Item> m_ttf_vec;

};


#endif //FORDTOOLS_IMAGESECTION_H

