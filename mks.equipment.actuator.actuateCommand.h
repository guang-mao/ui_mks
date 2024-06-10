#ifndef MKS_EQUIPMENT_ACTUATOR_ACTUATECOMMAND_H
#define MKS_EQUIPMENT_ACTUATOR_ACTUATECOMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct __attribute__((packed))
{
    uint8_t header1;
    uint8_t header2;
    uint8_t reserved;
    uint8_t command;
    union
    {
        struct __attribute__((packed))
        {
            uint8_t req_act:1;
            uint8_t req_pos:1;
            uint8_t req_cir:1;
            uint8_t req_tmp:1;
            uint8_t fsh_cnt:4;
        } fcn;
        uint8_t byte;
    } byte4;
    uint8_t id;
    uint8_t payload[2]; // setpoint command
    uint8_t sum_h;
    uint8_t sum_l;
} actuateCommand_req_t;

uint32_t mks_equipment_actuator_actuateCommand_encode(actuateCommand_req_t *msg, uint8_t id, uint8_t mode, uint16_t setpoint);

#ifdef __cplusplus
}
#endif

#endif // MKS_EQUIPMENT_ACTUATOR_ACTUATECOMMAND_H
