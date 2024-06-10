#include "mks.equipment.actuator.actuateCommand.h"

uint32_t mks_equipment_actuator_actuateCommand_encode(actuateCommand_req_t *msg, uint8_t id, uint8_t mode, uint16_t setpoint)
{
    uint32_t length = 8;

    msg->header1 = 0xB9;
    msg->header2 = 0x42;
    msg->reserved = 0xFF;
    msg->command = 0x02;
    msg->byte4.byte |= mode;
    msg->id = id;
    msg->payload[0] = (uint8_t ) ( setpoint >> 0u );
    msg->payload[1] = (uint8_t ) ( setpoint >> 8u );

    return 0;
}
