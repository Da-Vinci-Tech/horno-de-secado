const unsigned char logo[] ={
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,   // 0x0010 (16) pixels
0x80, 0xC0, 0x40, 0x60, 0x30, 0x30, 0xD8, 0xE8, 0x3C, 0x1C, 0x0E, 0x07, 0x0E, 0x1E, 0x3C, 0xE8,   // 0x0020 (32) pixels
0xD8, 0x30, 0x30, 0x60, 0x40, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0030 (48) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0x60, 0x20, 0x30, 0x18,   // 0x0040 (64) pixels
0x18, 0x0C, 0x04, 0x06, 0x03, 0x03, 0x81, 0xC1, 0x60, 0x38, 0x1C, 0x06, 0x03, 0x01, 0x00, 0x00,   // 0x0050 (80) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x1C, 0x38, 0x60, 0xC0, 0x81, 0x03,   // 0x0060 (96) pixels
0x03, 0x06, 0x04, 0x0C, 0x18, 0x18, 0x30, 0x30, 0x60, 0x40, 0xC0, 0x80, 0x80, 0x00, 0xFF, 0xFF,   // 0x0070 (112) pixels
0x03, 0x06, 0x0C, 0x0C, 0x18, 0x18, 0x30, 0x20, 0xE0, 0xF0, 0xF8, 0x4C, 0x46, 0x43, 0x41, 0x40,   // 0x0080 (128) pixels
0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,   // 0x0090 (144) pixels
0x40, 0x40, 0x40, 0x40, 0x40, 0x43, 0x47, 0x4C, 0x78, 0xF0, 0xE0, 0x20, 0x30, 0x10, 0x18, 0x0C,   // 0x00A0 (160) pixels
0x0C, 0x06, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x78, 0x0F, 0x03,   // 0x00B0 (176) pixels
0x01, 0x07, 0x0E, 0x38, 0xE0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF2, 0x02, 0x02, 0xFE,   // 0x00C0 (192) pixels
0x02, 0xFA, 0x0A, 0x0A, 0x0A, 0x92, 0x64, 0x08, 0xF0, 0x00, 0x00, 0x80, 0xE0, 0x38, 0x1E, 0x07,   // 0x00D0 (208) pixels
0x01, 0x01, 0x0F, 0x3C, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x80,   // 0x00E0 (224) pixels
0xE0, 0x3C, 0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x1C, 0x70,   // 0x00F0 (240) pixels
0xE0, 0x80, 0x00, 0x7F, 0x20, 0x20, 0x27, 0x14, 0x13, 0x0A, 0x09, 0x09, 0x05, 0x86, 0xC3, 0x70,   // 0x0100 (256) pixels
0x1C, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x3C, 0xF0, 0x80,   // 0x0110 (272) pixels
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xBC, 0x87, 0x81, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0120 (288) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0E, 0x38, 0xF0, 0xC0, 0x00, 0x00, 0x00,   // 0x0130 (304) pixels
0xC0, 0xE0, 0x38, 0x0E, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0140 (320) pixels
0x00, 0x00, 0x00, 0x80, 0x80, 0x81, 0x87, 0xBC, 0xFF, 0xFF, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07,   // 0x0150 (336) pixels
0x05, 0x0D, 0x19, 0x19, 0x33, 0x23, 0x63, 0xC2, 0xC2, 0x82, 0x02, 0x06, 0x06, 0x06, 0x04, 0x04,   // 0x0160 (352) pixels
0x04, 0x0C, 0x0C, 0x0D, 0x0F, 0xFE, 0x0F, 0x0D, 0x0C, 0x0C, 0x04, 0x04, 0x04, 0x06, 0x06, 0x06,   // 0x0170 (368) pixels
0x02, 0x82, 0xC2, 0xC2, 0x63, 0x23, 0x33, 0x19, 0x19, 0x0D, 0x0D, 0x07, 0x03, 0x03, 0x01, 0x01,   // 0x0180 (384) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0190 (400) pixels
0x01, 0x01, 0x03, 0x02, 0x06, 0x0C, 0x0C, 0x18, 0x10, 0x30, 0x60, 0x60, 0xFF, 0x60, 0x60, 0x30,   // 0x01A0 (416) pixels
0x10, 0x18, 0x0C, 0x0C, 0x06, 0x06, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x01B0 (432) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};
