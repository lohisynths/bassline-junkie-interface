/*
 * Preset.h
 *
 *  Created on: Sep 22, 2018
 *      Author: alax
 */

#ifndef SRC_PRESET_H_
#define SRC_PRESET_H_

#include "utils.h"
#include "blocks/OSC.h"
#include "blocks/ADSR.h"
#include "blocks/LFO.h"
#include "blocks/FLT.h"
#include "blocks/MOD.h"

#include "EEPROM.h"

#include <bitset>

class Preset {
public:

    /*! \typedef logger
     *  \brief Typedef defining logger used in all instances of this object
     */
    typedef logger<PRESET_LOG_LEVEL> LOG;

    const char* get_name() {
        return "Preset";
    }

    struct SynthPreset {
        OSC::preset osc_preset;
        ADSR::preset adsr_preset;
        FLT::preset flt_preset;
        LFO::preset lfo_preset;
        MOD::preset mod_preset;
    };

    Preset() {
        init();
    }

    ~Preset() {
    }

    void init() {
        LOG::LOG0("%s eeprom write\r\n", get_name());
//        main_preset.osc_preset[0][OSC_PITCH] = 10;
//        main_preset.osc_preset[0][OSC_SAW] = 127;
//        main_preset.osc_preset[1][OSC_SAW] = 127;
//
//        main_preset.adsr_preset[0][ADSR_LOOP] = 1;
//        main_preset.flt_preset[0][FLT_FREQ] = 127;
//        main_preset.flt_preset[0][FLT_SHAPE] = 2;
//        main_preset.lfo_preset[0][LFO_SHAPE] = 2;
//        main_preset.lfo_preset[0][LFO_FREQ] = 64;
    }

    void load_preset_eeprom(int index) {
        load_preset_eeprom(eeprom, main_preset, index);
    }

    void load_preset_eeprom(EEPROM &eeprom, SynthPreset &input, int index) {
        LOG::LOG0("%s eeprom %d read\r\n", get_name(), index);
        uint32_t *output_data = (uint32_t *) &input;
        uint32_t addr_start = FLASH_USER_START_ADDR + sizeof(SynthPreset) * (index);

        for (size_t i = 0; i < sizeof(SynthPreset) / 4; i++) {
            uint32_t preset_address = (i * 4) + addr_start;
            uint32_t ret = eeprom.readEEPROMWord(preset_address);

            LOG::LOG3("%s eeprom read %d %d\r\n", get_name(), i, ret);
            LOG::LOG4("%#08x %s %#04x %d\r\n", preset_address, get_binary(ret), ret, ret);
            output_data[i] = ret;
        }
        print_preset(input);
    }

    void save_preset_eeprom(SynthPreset &input, int index) {
        LOG::LOG0("%s eeprom %d write\r\n", get_name(), index);
        uint32_t *data = (uint32_t *) &input;
        uint32_t addr_start = FLASH_USER_START_ADDR + sizeof(SynthPreset) * (index);

        eeprom.erase(sizeof(SynthPreset));
        for (size_t i = 0; i < sizeof(SynthPreset) / 4; i++) {
            uint32_t ret = data[i];
            uint32_t preset_address = (i * 4) + addr_start;
            LOG::LOG5("%s eeprom write %d %d\r\n", get_name(), i, ret);
            LOG::LOG6("%#08x %s %#04x %d\r\n", preset_address, get_binary(ret), ret, ret);
            eeprom.writeEEPROMWord(preset_address, ret);
        }
        print_preset(main_preset);
    }

    void print_preset(SynthPreset &input) {
        LOG::LOG1("%s print\r\n", get_name());
        LOG::LOG2("%s print\r\n", get_name());

        for (size_t i = 0; i < sizeof(SynthPreset) / 4; i++) {
            uint32_t *data = (uint32_t *) &input;
            uint32_t ret = data[i];
            LOG::LOG2("%s raw %d %d\r\n", get_name(), i, ret);
        }

        for (size_t i = 0; i < OSC_COUNT; i++) {
            for (unsigned j = 0; j < OSC_PARAM_COUNT; j++) {
                LOG::LOG1("%s osc %d %d %d\r\n", get_name(), i, j, input.osc_preset[i][j]);
            }
        }

        for (size_t i = 0; i < ADSR_COUNT; i++) {
            for (int j = 0; j < ADSR_PARAM_NR; j++) {
                LOG::LOG1("%s adsr %d %d %d\r\n", get_name(), i, j, input.adsr_preset[i][j]);
            }
        }

        for (size_t i = 0; i < FLT_COUNT; i++) {
            for (int j = 0; j < FLT_PARAM_COUNT; j++) {
                LOG::LOG1("%s flt %d %d %d\r\n", get_name(), i, j, input.flt_preset[i][j]);
            }
        }
    }
    const OSC::preset &get_osc_preset() {
        return (main_preset.osc_preset);
    }
    const ADSR::preset &get_adrsr_preset() {
        return (main_preset.adsr_preset);
    }
    const FLT::preset &get_flt_preset() {
        return (main_preset.flt_preset);
    }
    const LFO::preset &get_lfo_preset() {
        return (main_preset.lfo_preset);
    }
    const MOD::preset &get_mod_preset() {
        return (main_preset.mod_preset);
    }

private:
    EEPROM eeprom;
    SynthPreset main_preset = { };
};

#endif /* SRC_PRESET_H_ */
