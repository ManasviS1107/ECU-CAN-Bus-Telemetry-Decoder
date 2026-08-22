#include "can_decoder.h"

EXPORT void decode_powertrain_frame(const uint8_t* payload, PowertrainMetrics* output) {
    if (!payload || !output) return;

    // 1. Reconstructing 64-bit integer bitfield from Big-Endian byte stream
    uint64_t frame = 0;
    for (int i = 0; i < 8; i++) {
        frame |= ((uint64_t)payload[i]) << (56 - (i * 8));
    }

    // 2. Extracting ENGINE_RPM (Bits 0-15, Scale 0.25)
    uint16_t raw_rpm = (uint16_t)((frame >> 48) & 0xFFFF);
    output->engine_rpm = raw_rpm * 0.25f;

    // 3. Extracting THROTTLE_POS (Bits 16-23, Scale 0.392)
    uint8_t raw_throttle = (uint8_t)((frame >> 40) & 0xFF);
    output->throttle_pos = raw_throttle * 0.392f;

    // 4. Extracting GEAR_SELECTION (Bits 24-27)
    output->gear = (uint8_t)((frame >> 36) & 0x0F);

    // 5. Extracting ERS_DEPLOY_MODE (Bits 28-31)
    output->ers_mode = (uint8_t)((frame >> 32) & 0x0F);
}