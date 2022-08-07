//
// Created by digitalfairy on 2022/08/07.
//

#ifndef YMFM_ANDROID_3_VGM_H
#define YMFM_ANDROID_3_VGM_H

using emulated_time = int64_t;

enum chip_type
{
    CHIP_YM2149,
    CHIP_YM2151,
    CHIP_YM2203,
    CHIP_YM2413,
    CHIP_YM2608,
    CHIP_YM2610,
    CHIP_YM2612,
    CHIP_YM3526,
    CHIP_Y8950,
    CHIP_YM3812,
    CHIP_YMF262,
    CHIP_YMF278B,
    CHIP_TYPES
};



template<typename ChipType> void add_chips(uint32_t clock, chip_type type, char const *chipname);
//vgm_chip_base *find_chip(chip_type type, uint8_t index);
void write_chip(chip_type type, uint8_t index, uint32_t reg, uint8_t data);
void add_rom_data(chip_type type, ymfm::access_class access, std::vector<uint8_t> &buffer, uint32_t &localoffset, uint32_t size);
void generate_all(std::vector<uint8_t> &buffer, uint32_t data_start, uint32_t output_rate, std::vector<int32_t> &wav_buffer);


#endif //YMFM_ANDROID_3_VGM_H
