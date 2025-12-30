#include "scher_khan.h"
#include <lib/toolbox/manchester_decoder.h>
#include <lib/toolbox/manchester_encoder.h>
#include "../blocks/const.h"
#include "../blocks/decoder.h"
#include "../blocks/encoder.h"
#include "../blocks/generic.h"
#include "../blocks/math.h"

//https://phreakerclub.com/72
//https://phreakerclub.com/forum/showthread.php?t=7&page=2
//https://phreakerclub.com/forum/showthread.php?t=274&highlight=magicar
//!!!  https://phreakerclub.com/forum/showthread.php?t=489&highlight=magicar&page=5

#define TAG "SubGhzProtocolScherKhan"

static const SubGhzBlockConst subghz_protocol_scher_khan_const = {
    .te_short = 750,
    .te_long = 1100,
    .te_delta = 150,
    .min_count_bit_for_found = 31,
};

struct SubGhzProtocolDecoderScherKhan {
    SubGhzProtocolDecoderBase base;

    SubGhzBlockDecoder decoder;
    SubGhzBlockGeneric generic;

    uint16_t header_count;
    const char* protocol_name;
};

struct SubGhzProtocolEncoderScherKhan {
    SubGhzProtocolEncoderBase base;

    SubGhzProtocolBlockEncoder encoder;
    SubGhzBlockGeneric generic;
};

typedef enum {
    ScherKhanDecoderStepReset = 0,
    ScherKhanDecoderStepCheckPreambula,
    ScherKhanDecoderStepSaveDuration,
    ScherKhanDecoderStepCheckDuration,
} ScherKhanDecoderStep;

// Forward declarations for encoder functions
static void* subghz_protocol_encoder_scher_khan_alloc(SubGhzEnvironment* environment);
static void subghz_protocol_encoder_scher_khan_free(void* context);
static SubGhzProtocolStatus subghz_protocol_encoder_scher_khan_deserialize(void* context, FlipperFormat* flipper_format);
static void subghz_protocol_encoder_scher_khan_stop(void* context);
static LevelDuration subghz_protocol_encoder_scher_khan_yield(void* context);

const SubGhzProtocolDecoder subghz_protocol_scher_khan_decoder = {
    .alloc = subghz_protocol_decoder_scher_khan_alloc,
    .free = subghz_protocol_decoder_scher_khan_free,

    .feed = subghz_protocol_decoder_scher_khan_feed,
    .reset = subghz_protocol_decoder_scher_khan_reset,

    .get_hash_data = NULL,
    .get_hash_data_long = subghz_protocol_decoder_scher_khan_get_hash_data,
    .serialize = subghz_protocol_decoder_scher_khan_serialize,
    .deserialize = subghz_protocol_decoder_scher_khan_deserialize,
    .get_string = subghz_protocol_decoder_scher_khan_get_string,
    .get_string_brief = NULL,
};

const SubGhzProtocolEncoder subghz_protocol_scher_khan_encoder = {
    .alloc = subghz_protocol_encoder_scher_khan_alloc,
    .free = subghz_protocol_encoder_scher_khan_free,
    .deserialize = subghz_protocol_encoder_scher_khan_deserialize,
    .stop = subghz_protocol_encoder_scher_khan_stop,
    .yield = subghz_protocol_encoder_scher_khan_yield,
};

const SubGhzProtocol subghz_protocol_scher_khan = {
    .name = SUBGHZ_PROTOCOL_SCHER_KHAN_NAME,
    .type = SubGhzProtocolTypeDynamic,
    .flag = SubGhzProtocolFlag_315 | SubGhzProtocolFlag_433 | SubGhzProtocolFlag_868 | SubGhzProtocolFlag_FM | SubGhzProtocolFlag_AM | SubGhzProtocolFlag_Decodable |
            SubGhzProtocolFlag_Load | SubGhzProtocolFlag_Save | SubGhzProtocolFlag_Send,

    .decoder = &subghz_protocol_scher_khan_decoder,
    .encoder = &subghz_protocol_scher_khan_encoder,
    .filter = SubGhzProtocolFilter_Cars,
};

void* subghz_protocol_decoder_scher_khan_alloc(SubGhzEnvironment* environment) {
    UNUSED(environment);
    SubGhzProtocolDecoderScherKhan* instance = malloc(sizeof(SubGhzProtocolDecoderScherKhan));
    instance->base.protocol = &subghz_protocol_scher_khan;
    instance->generic.protocol_name = instance->base.protocol->name;

    return instance;
}

void subghz_protocol_decoder_scher_khan_free(void* context) {
    furi_assert(context);
    SubGhzProtocolDecoderScherKhan* instance = context;
    free(instance);
}

void subghz_protocol_decoder_scher_khan_reset(void* context) {
    furi_assert(context);
    SubGhzProtocolDecoderScherKhan* instance = context;
    instance->decoder.parser_step = ScherKhanDecoderStepReset;
}

void subghz_protocol_decoder_scher_khan_feed(void* context, bool level, uint32_t duration) {
    furi_assert(context);
    SubGhzProtocolDecoderScherKhan* instance = context;

    switch(instance->decoder.parser_step) {
    case ScherKhanDecoderStepReset:
        if((level) && (DURATION_DIFF(duration, subghz_protocol_scher_khan_const.te_short * 2) <
                       subghz_protocol_scher_khan_const.te_delta)) {
            instance->decoder.parser_step = ScherKhanDecoderStepCheckPreambula;
            instance->decoder.te_last = duration;
            instance->header_count = 0;
        }
        break;
    case ScherKhanDecoderStepCheckPreambula:
        if(level) {
            if((DURATION_DIFF(duration, subghz_protocol_scher_khan_const.te_short * 2) <
                subghz_protocol_scher_khan_const.te_delta) ||
               (DURATION_DIFF(duration, subghz_protocol_scher_khan_const.te_short) <
                subghz_protocol_scher_khan_const.te_delta)) {
                instance->decoder.te_last = duration;
            } else {
                instance->decoder.parser_step = ScherKhanDecoderStepReset;
            }
        } else if(
            (DURATION_DIFF(duration, subghz_protocol_scher_khan_const.te_short * 2) <
             subghz_protocol_scher_khan_const.te_delta) ||
            (DURATION_DIFF(duration, subghz_protocol_scher_khan_const.te_short) <
             subghz_protocol_scher_khan_const.te_delta)) {
            if(DURATION_DIFF(
                   instance->decoder.te_last, subghz_protocol_scher_khan_const.te_short * 2) <
               subghz_protocol_scher_khan_const.te_delta) {
                // Found header
                instance->header_count++;
                break;
            } else if(
                DURATION_DIFF(
                    instance->decoder.te_last, subghz_protocol_scher_khan_const.te_short) <
                subghz_protocol_scher_khan_const.te_delta) {
                // Found start bit
                if(instance->header_count >= 2) {
                    instance->decoder.parser_step = ScherKhanDecoderStepSaveDuration;
                    instance->decoder.decode_data = 0;
                    instance->decoder.decode_count_bit = 1;
                } else {
                    instance->decoder.parser_step = ScherKhanDecoderStepReset;
                }
            } else {
                instance->decoder.parser_step = ScherKhanDecoderStepReset;
            }
        } else {
            instance->decoder.parser_step = ScherKhanDecoderStepReset;
        }
        break;
    case ScherKhanDecoderStepSaveDuration:
        if(level) {
            if(duration >= (subghz_protocol_scher_khan_const.te_delta * 2UL +
                            subghz_protocol_scher_khan_const.te_long)) {
                //Found stop bit
                instance->decoder.parser_step = ScherKhanDecoderStepReset;
                if(instance->decoder.decode_count_bit >=
                   subghz_protocol_scher_khan_const.min_count_bit_for_found) {
                    instance->generic.data = instance->decoder.decode_data;
                    instance->generic.data_count_bit = instance->decoder.decode_count_bit;
                    if(instance->base.callback)
                        instance->base.callback(&instance->base, instance->base.context);
                }
                instance->decoder.decode_data = 0;
                instance->decoder.decode_count_bit = 0;
                break;
            } else {
                instance->decoder.te_last = duration;
                instance->decoder.parser_step = ScherKhanDecoderStepCheckDuration;
            }

        } else {
            instance->decoder.parser_step = ScherKhanDecoderStepReset;
        }
        break;
    case ScherKhanDecoderStepCheckDuration:
        if(!level) {
            if((DURATION_DIFF(
                    instance->decoder.te_last, subghz_protocol_scher_khan_const.te_short) <
                subghz_protocol_scher_khan_const.te_delta) &&
               (DURATION_DIFF(duration, subghz_protocol_scher_khan_const.te_short) <
                subghz_protocol_scher_khan_const.te_delta)) {
                subghz_protocol_blocks_add_bit(&instance->decoder, 0);
                instance->decoder.parser_step = ScherKhanDecoderStepSaveDuration;
            } else if(
                (DURATION_DIFF(
                     instance->decoder.te_last, subghz_protocol_scher_khan_const.te_long) <
                 subghz_protocol_scher_khan_const.te_delta) &&
                (DURATION_DIFF(duration, subghz_protocol_scher_khan_const.te_long) <
                 subghz_protocol_scher_khan_const.te_delta)) {
                subghz_protocol_blocks_add_bit(&instance->decoder, 1);
                instance->decoder.parser_step = ScherKhanDecoderStepSaveDuration;
            } else {
                instance->decoder.parser_step = ScherKhanDecoderStepReset;
            }
        } else {
            instance->decoder.parser_step = ScherKhanDecoderStepReset;
        }
        break;
    }
}

/** 
 * Analysis of received data
 * @param instance Pointer to a SubGhzBlockGeneric* instance
 * @param protocol_name 
 */
static void subghz_protocol_scher_khan_check_remote_controller(
    SubGhzBlockGeneric* instance,
    const char** protocol_name) {
    /* 
    * MAGICAR 51 bit 00000001A99121DE83C3 MAGIC CODE, Dynamic
    * 0E8C1619E830C -> 000011101000110000010110 0001 1001 1110 1000001100001100
    * 0E8C1629D830D -> 000011101000110000010110 0010 1001 1101 1000001100001101
    * 0E8C1649B830E -> 000011101000110000010110 0100 1001 1011 1000001100001110
    * 0E8C16897830F -> 000011101000110000010110 1000 1001 0111 1000001100001111
    *                             Serial         Key  Ser ~Key   CNT
    */

    switch(instance->data_count_bit) {
    case 35: //MAGIC CODE, Static
        *protocol_name = "MAGIC CODE, Static";
        instance->serial = 0;
        instance->btn = 0;
        instance->cnt = 0;
        break;
    case 51: //MAGIC CODE, Dynamic
        *protocol_name = "MAGIC CODE, Dynamic";
        instance->serial = ((instance->data >> 24) & 0xFFFFFF0) | ((instance->data >> 20) & 0x0F);
        instance->btn = (instance->data >> 24) & 0x0F;
        instance->cnt = instance->data & 0xFFFF;
        break;
    case 57: //MAGIC CODE PRO / PRO2
        *protocol_name = "MAGIC CODE PRO/PRO2";
        instance->serial = 0;
        instance->btn = 0;
        instance->cnt = 0;
        break;
    case 63: //MAGIC CODE, Dynamic Response
        *protocol_name = "MAGIC CODE, Response";
        instance->serial = 0;
        instance->btn = 0;
        instance->cnt = 0;
        break;
    case 64: //MAGICAR, Response ???
        *protocol_name = "MAGICAR, Response";
        instance->serial = 0;
        instance->btn = 0;
        instance->cnt = 0;
        break;
    case 81: //MAGIC CODE PRO / PRO2 Response ???
        *protocol_name = "MAGIC CODE PRO,\n Response";
        instance->serial = 0;
        instance->btn = 0;
        instance->cnt = 0;
        break;
    case 82: //MAGIC CODE PRO / PRO2 Response ???
        *protocol_name = "MAGIC CODE PRO,\n Response";
        instance->serial = 0;
        instance->btn = 0;
        instance->cnt = 0;
        break;

    default:
        *protocol_name = "Unknown";
        instance->serial = 0;
        instance->btn = 0;
        instance->cnt = 0;
        break;
    }
}

uint32_t subghz_protocol_decoder_scher_khan_get_hash_data(void* context) {
    furi_assert(context);
    SubGhzProtocolDecoderScherKhan* instance = context;
    return subghz_protocol_blocks_get_hash_data_long(
        &instance->decoder, (instance->decoder.decode_count_bit / 8) + 1);
}

SubGhzProtocolStatus subghz_protocol_decoder_scher_khan_serialize(
    void* context,
    FlipperFormat* flipper_format,
    SubGhzRadioPreset* preset) {
    furi_assert(context);
    SubGhzProtocolDecoderScherKhan* instance = context;
    return subghz_block_generic_serialize(&instance->generic, flipper_format, preset);
}

SubGhzProtocolStatus
    subghz_protocol_decoder_scher_khan_deserialize(void* context, FlipperFormat* flipper_format) {
    furi_assert(context);
    SubGhzProtocolDecoderScherKhan* instance = context;
    return subghz_block_generic_deserialize(&instance->generic, flipper_format);
}

void subghz_protocol_decoder_scher_khan_get_string(void* context, FuriString* output) {
    furi_assert(context);
    SubGhzProtocolDecoderScherKhan* instance = context;

    subghz_protocol_scher_khan_check_remote_controller(
        &instance->generic, &instance->protocol_name);

    furi_string_cat_printf(
        output,
        "%s %dbit\r\n"
        "Key:0x%lX%08lX\r\n"
        "Sn:%07lX Btn:%X Cnt:%04lX\r\n"
        "Pt: %s\r\n",
        instance->generic.protocol_name,
        instance->generic.data_count_bit,
        (uint32_t)(instance->generic.data >> 32),
        (uint32_t)instance->generic.data,
        instance->generic.serial,
        instance->generic.btn,
        instance->generic.cnt,
        instance->protocol_name);
}

// Implementation of encoder functions

static void* subghz_protocol_encoder_scher_khan_alloc(SubGhzEnvironment* environment) {
    UNUSED(environment);
    SubGhzProtocolEncoderScherKhan* instance = malloc(sizeof(SubGhzProtocolEncoderScherKhan));
    instance->base.protocol = &subghz_protocol_scher_khan;
    instance->generic.protocol_name = instance->base.protocol->name;
    
    // Initialize encoder specific fields
    instance->encoder.upload = malloc(1024 * sizeof(LevelDuration)); // Or a more specific size
    instance->encoder.size_upload = 0;
    instance->encoder.repeat = 3; // Default repeat count
    instance->encoder.is_running = false;

    return instance;
}

static void subghz_protocol_encoder_scher_khan_free(void* context) {
    furi_assert(context);
    SubGhzProtocolEncoderScherKhan* instance = context;
    free(instance->encoder.upload);
    free(instance);
}

static SubGhzProtocolStatus subghz_protocol_encoder_scher_khan_deserialize(void* context, FlipperFormat* flipper_format) {
    furi_assert(context);
    SubGhzProtocolEncoderScherKhan* instance = context;
    SubGhzProtocolStatus result = SubGhzProtocolStatusError;

    // 1. Deserialize data into instance->generic
    result = subghz_block_generic_deserialize(&instance->generic, flipper_format);
    if (result != SubGhzProtocolStatusOk) {
        FURI_LOG_E(TAG, "Error deserializing generic block");
        return result;
    }

    // 2. Handle rolling counter for dynamic codes (e.g., 51-bit)
    if (instance->generic.data_count_bit == 51) { // Assuming 51-bit is dynamic
        uint32_t count_increment = furi_hal_subghz_get_rolling_counter_mult();
        if (count_increment == 0) count_increment = 1;
        
        uint32_t initial_cnt = instance->generic.cnt;
        instance->generic.cnt += count_increment;
        if (instance->generic.cnt > 0xFFFF) { // Assuming 16-bit counter
            instance->generic.cnt &= 0xFFFF; 
        }
        FURI_LOG_I(TAG, "Rolling count: %lu -> %lu (inc: %lu)", initial_cnt, instance->generic.cnt, count_increment);
    }

    // 3. Reconstruct the transmit_data payload based on protocol type (data_count_bit)
    uint64_t transmit_data = 0;
    bool payload_constructed = false;

    if (instance->generic.data_count_bit == 51) { // MAGIC CODE, Dynamic
        uint32_t serial_val = instance->generic.serial;
        uint8_t btn_val = instance->generic.btn;
        uint16_t cnt_val = instance->generic.cnt; // This is the already incremented counter
        
        transmit_data = (uint64_t)cnt_val;                                  // CNT (0-15)
        transmit_data |= ((uint64_t)(serial_val & 0x0000000F) << 20);       // SER_LO (20-23)
        transmit_data |= ((uint64_t)btn_val << 24);                         // BTN (24-27)
        transmit_data |= ((uint64_t)((serial_val >> 4) & 0x07FFFFFF) << 28); // SER_HI (28-50)

        payload_constructed = true;
        FURI_LOG_I(TAG, "Constructed 51-bit payload: 0x%llX (Ser:0x%lX Btn:%X Cnt:%u)", transmit_data, serial_val, btn_val, cnt_val);

    } else if (instance->generic.data_count_bit == 35) { // MAGIC CODE, Static
        transmit_data = instance->generic.data;
        payload_constructed = true;
        FURI_LOG_I(TAG, "Using static 35-bit payload: 0x%llX", transmit_data);
    } else {
        FURI_LOG_E(TAG, "Unsupported bit count for encoding: %d", instance->generic.data_count_bit);
        return SubGhzProtocolStatusError;
    }

    if(!payload_constructed) {
        FURI_LOG_E(TAG, "Failed to construct payload.");
        return SubGhzProtocolStatusErrorEncoderGetUpload;
    }

    // 4. Generate LevelDuration sequence
    size_t index = 0;
    for(int h = 0; h < 2; ++h) { 
        instance->encoder.upload[index++] = level_duration_make(true, subghz_protocol_scher_khan_const.te_short * 2);
        instance->encoder.upload[index++] = level_duration_make(false, subghz_protocol_scher_khan_const.te_short * 2);
    }
    instance->encoder.upload[index++] = level_duration_make(true, subghz_protocol_scher_khan_const.te_short);
    instance->encoder.upload[index++] = level_duration_make(false, subghz_protocol_scher_khan_const.te_short);

    for (uint8_t i = instance->generic.data_count_bit; i > 0; i--) {
        if (bit_read(transmit_data, i - 1)) { 
            instance->encoder.upload[index++] = level_duration_make(true, subghz_protocol_scher_khan_const.te_long);
            instance->encoder.upload[index++] = level_duration_make(false, subghz_protocol_scher_khan_const.te_long);
        } else { 
            instance->encoder.upload[index++] = level_duration_make(true, subghz_protocol_scher_khan_const.te_short);
            instance->encoder.upload[index++] = level_duration_make(false, subghz_protocol_scher_khan_const.te_short);
        }
    }

    instance->encoder.upload[index++] = level_duration_make(true, subghz_protocol_scher_khan_const.te_long * 2);
    instance->encoder.upload[index++] = level_duration_make(false, 0); 

    instance->encoder.size_upload = index;
    FURI_LOG_I(TAG, "Prepared %zu levels for %d bits", index, instance->generic.data_count_bit);
    result = SubGhzProtocolStatusOk;
    
    if(result == SubGhzProtocolStatusOk) {
        instance->encoder.is_running = true;
    }
    return result;
}

static void subghz_protocol_encoder_scher_khan_stop(void* context) {
    SubGhzProtocolEncoderScherKhan* instance = context;
    instance->encoder.is_running = false;
}

static LevelDuration subghz_protocol_encoder_scher_khan_yield(void* context) {
    SubGhzProtocolEncoderScherKhan* instance = context;

    if(instance->encoder.repeat == 0 || !instance->encoder.is_running || instance->encoder.size_upload == 0) {
        instance->encoder.is_running = false;
        return level_duration_reset(); // Signal end
    }

    LevelDuration ret = instance->encoder.upload[instance->encoder.front];
    instance->encoder.front++;

    if (instance->encoder.front >= instance->encoder.size_upload) {
        instance->encoder.repeat--;
        instance->encoder.front = 0;
        if(instance->encoder.repeat == 0) {
            instance->encoder.is_running = false;
            // Optional: Increment counter here if it should only change after all repeats of one button press
        }
    }
    return ret;
}
