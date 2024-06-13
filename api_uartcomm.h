#ifndef API_UARTCOMM_H
#define API_UARTCOMM_H

#ifdef __cplusplus
extern "C" {
#endif

inline static unsigned int API_checksum16_protocol(unsigned char *data, unsigned char length)
{
    int j;
    unsigned int reg_crc = 0;
    for ( j = 0; j < length; j++ )
    {
        reg_crc += *data++;
    }
    reg_crc &= ( 0xffff );

    return reg_crc;
}

#ifdef __cplusplus
}
#endif

#endif // API_UARTCOMM_H
