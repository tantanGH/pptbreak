#ifndef __H_DATA_ADPCM__
#define __H_DATA_ADPCM__

#include <stdint.h>

#define ADPCM_SE01_LENGTH (7031)
#define ADPCM_SE02_LENGTH (3125)
#define ADPCM_SE03_LENGTH (3125)
#define ADPCM_SE04_LENGTH (3125)
#define ADPCM_SE05_LENGTH (2734)

extern uint8_t adpcm_se01[ ADPCM_SE01_LENGTH ];
extern uint8_t adpcm_se02[ ADPCM_SE02_LENGTH ];
extern uint8_t adpcm_se03[ ADPCM_SE03_LENGTH ];
extern uint8_t adpcm_se04[ ADPCM_SE04_LENGTH ];
extern uint8_t adpcm_se05[ ADPCM_SE05_LENGTH ];

#endif