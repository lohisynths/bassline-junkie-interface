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
#include "blocks/VOL.h"
#include "blocks/LED_DISP.h"

#include "EEPROM.h"

#include <bitset>

#define GLOBAL_PRESET_COUNT 32

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
        VOL::preset vol_preset;
        MOD::mod_preset mod_preset;
    };

    Preset() { }

    ~Preset() { }

    void update() {
        int ret = -1;
        ret = _disp->preset_changed();
        if(ret > -1) {
            LOG::LOG1("loading preset %d\r\n", ret);
//            load_preset_eeprom(ret);
//            update_preset();
            LOG::LOG1("preset %d loaded\r\n", ret);
        }

        ret = _disp->get_long_push();
        if(ret != -1) {
            LOG::LOG1("saving preset %d\r\n", _disp->get_actual_preset_nr());
            save_preset_eeprom(_disp->get_actual_preset_nr());
            LOG::LOG1("preset %d saved\r\n", _disp->get_actual_preset_nr());
        }
    }

    void update_preset() {
        _osc->set_preset(get_osc_preset());
        _adsr->set_preset(get_adrsr_preset());
        _filter->set_preset(get_flt_preset());
        _lfo->set_preset(get_lfo_preset());
        _vol->set_preset(get_vol_preset());
        _mod->set_mod_preset(get_mod_preset());
    }

    void init(ADSR *adsr, OSC *osc, LFO *lfo, MOD *mod, FLT *filter, VOL *vol, LED_DISP *disp) {
        _adsr = adsr;
        _osc = osc;
        _lfo = lfo;
        _mod = mod;
        _filter = filter;
        _vol = vol;
        _disp = disp;
    }

    void load_global(int index) {
        for(int i = 0; i < GLOBAL_PRESET_COUNT; i++) {
            LOG::LOG0("%s eeprom %d write\r\n", get_name(), i);
            load_preset_eeprom(eeprom, global_presets[i], i);
        }
        main_preset = global_presets[index];
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

    void save_preset_eeprom(int index) {
        Preset::SynthPreset asdsa = {
                _osc->get_preset(),
                _adsr->get_preset(),
                _filter->get_preset(),
                _lfo->get_preset(),
                _vol->get_preset(),
                _mod->get_mod_preset()};
        global_presets[index] = asdsa;
        save_global();
        print_preset(main_preset);
    }

    void erase() {
        memset(&global_presets, 0, sizeof(global_presets));
        save_global();
    }


    void save_global() {
        eeprom.erase(sizeof(SynthPreset)*GLOBAL_PRESET_COUNT);

        for(int i = 0; i < GLOBAL_PRESET_COUNT; i++) {
            LOG::LOG0("%s eeprom %d write\r\n", get_name(), i);

            uint32_t *data = (uint32_t *)&global_presets[i];
            uint32_t addr_start = FLASH_USER_START_ADDR + (sizeof(SynthPreset) * i);
            for (size_t i = 0; i < sizeof(SynthPreset) / 4; i++) {
                uint32_t ret = data[i];
                uint32_t preset_address = (i * 4) + addr_start;
                LOG::LOG5("%s eeprom write %d %d\r\n", get_name(), i, ret);
                LOG::LOG6("%#08x %s %#04x %d\r\n", preset_address, get_binary(ret), ret, ret);
                eeprom.writeEEPROMWord(preset_address, ret);
            }
        }
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

        for (size_t i = 0; i < VOL_COUNT; i++) {
            for (int j = 0; j < VOL_PARAM_COUNT; j++) {
                LOG::LOG1("%s vol %d %d %d\r\n", get_name(), i, j, input.vol_preset[i][j]);
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
    const MOD::mod_preset &get_mod_preset() {
        return (main_preset.mod_preset);
    }
    const VOL::preset &get_vol_preset() {
        return (main_preset.vol_preset);
    }

private:
    EEPROM eeprom;
    SynthPreset main_preset = { };
    SynthPreset global_presets[GLOBAL_PRESET_COUNT] = { };

    ADSR *_adsr;
    OSC *_osc;
    LFO *_lfo;
    MOD *_mod;
    FLT *_filter;
    VOL *_vol;
    LED_DISP *_disp;

};

#endif /* SRC_PRESET_H_ */
