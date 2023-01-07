#ifndef __H_DATA_ADPCM__
#define __H_DATA_ADPCM__

#define ADPCM_DATA1_LENGTH (7032)
#define ADPCM_DATA2_LENGTH (3125)
#define ADPCM_DATA3_LENGTH (3125)
#define ADPCM_DATA4_LENGTH (3125)

extern unsigned char adpcm_data1[ ADPCM_DATA1_LENGTH ];
extern unsigned char adpcm_data2[ ADPCM_DATA2_LENGTH ];
extern unsigned char adpcm_data3[ ADPCM_DATA3_LENGTH ];
extern unsigned char adpcm_data4[ ADPCM_DATA4_LENGTH ];

#define ADPCM_DATA_MUSIC1_LENGTH (62500)

extern unsigned char adpcm_data_music1[ ADPCM_DATA_MUSIC1_LENGTH ];

#endif