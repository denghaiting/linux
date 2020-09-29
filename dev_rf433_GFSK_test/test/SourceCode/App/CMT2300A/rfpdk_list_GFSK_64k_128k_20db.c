/*
;---------------------------------------
;  CMT2300A Configuration File
;  Generated by CMOSTEK RFPDK 1.45
;  2020.08.05 15:08
;---------------------------------------
; Mode                      = Advanced
; Part Number               = CMT2300A
; Frequency                 = 433.920 MHz
; Xtal Frequency            = 26.0000 MHz
; Demodulation              = GFSK
; AGC                       = On
; Data Rate                 = 64.0 kbps
; Deviation                 = 128.0 kHz
; Tx Xtal Tol.              = 20 ppm
; Rx Xtal Tol.              = 20 ppm
; TRx Matching Network Type = 20 dBm
; Tx Power                  = +20 dBm
; Gaussian BT               = 0.5
; Bandwidth                 = Auto-Select kHz
; CDR Type                  = Counting
; CDR DR Range              = NA
; AFC                       = On
; AFC Method                = Auto-Select
; Data Representation       = 0:F-low 1:F-high
; Rx Duty-Cycle             = Off
; Tx Duty-Cycle             = Off
; Sleep Timer               = Off
; Sleep Time                = NA
; Rx Timer                  = Off
; Rx Time T1                = NA
; Rx Time T2                = NA
; Rx Exit State             = STBY
; Tx Exit State             = STBY
; SLP Mode                  = Disable
; RSSI Valid Source         = PJD
; PJD Window                = 8 Jumps
; LFOSC Calibration         = On
; Xtal Stable Time          = 155 us
; RSSI Compare TH           = NA
; Data Mode                 = Packet
; Whitening                 = Disable
; Whiten Type               = NA
; Whiten Seed Type          = NA
; Whiten Seed               = NA
; Manchester                = Disable
; Manchester Type           = NA
; FEC                       = Disable
; FEC Type                  = NA
; Tx Prefix Type            = Preamble
; Tx Packet Number          = 1
; Tx Packet Gap             = 32
; Packet Type               = Variable Length
; Node-Length Position      = First Node, then Length
; Payload Bit Order         = Start from msb
; Preamble Rx Size          = 2
; Preamble Tx Size          = 8
; Preamble Value            = 0
; Preamble Unit             = 8-bit
; Sync Size                 = 3-byte
; Sync Value                = 16755285
; Sync Tolerance            = None
; Sync Manchester           = Disable
; Node ID Size              = NA
; Node ID Value             = NA
; Node ID Mode              = None
; Node ID Err Mask          = Disable
; Node ID Free              = Disable
; Payload Length            = 32
; CRC Options               = CCITT-16
; CRC Seed                  = 5526 crc_seed
; CRC Range                 = Entire Payload
; CRC Swap                  = Start from MSB
; CRC Bit Invert            = Normal
; CRC Bit Order             = Start from bit 15
; Dout Mute                 = Off
; Dout Adjust Mode          = Disable
; Dout Adjust Percentage    = NA
; Collision Detect          = Off
; Collision Detect Offset   = NA
; RSSI Detect Mode          = Always
; RSSI Filter Setting       = No Filtering
; RF Performance            = High
; LBD Threshold             = 2.4 V
; RSSI Offset               = 26
; RSSI Offset Sign          = 1

;---------------------------------------
;  The following are the Register contents
;---------------------------------------
[CMT Bank]
Addr  Value	
*/
const unsigned char CMTBank_GFSK_64k_128k_20db[0x60] = 
{
0x00,
0x66,
0xEC,
0x1D,
0x30,
0x80,
0x14,
0x08,
0x91,
0x02,
0x02,
0xD0,
0xAE,
0xE0,
0x35,
0x00,
0x00,
0xF4,
0x10,
0xE2,
0x42,
0x20,
0x00,
0x81,
0x42,
0x71,
0xCE,
0x1C,
0x42,
0x5B,
0x1C,
0x1C,
0x47,
0x85,
0x72,
0x10,
0xF3,
0x13,
0x28,
0x0A,
0x9F,
0x5A,
0x29,
0x29,
0xC0,
0x96,
0x01,
0x53,
0x20,
0x00,
0xB4,
0x00,
0x00,
0x01,
0x00,
0x00,
0x12,
0x08,
0x00,
0x00,
0x04,
0x00,
0x00,
0x00,
0x00,
0x00,
0x55,
0xAA,
0xFF,
0x01,
0x1F,
0x00,
0x00,
0x00,
0x00,
0x00,
0x01,
0x96,
0x15,
0x60,
0xFF,
0x02,
0x00,
0x1F,
0x10,
0x50,
0xA8,
0x50,
0x00,
0x01,
0x30,
0x00,
0x8A,
0x18,
0x3F,
0x7F,	
};
