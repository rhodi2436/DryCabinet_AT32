

#define Bank1_NOR2_ADDR       ((u32)0x64000000)
#define TFT_DateBaseADDR	  ((u32)0x6C020000)
#define TFT1_DateBaseADDR	  ((u32)0x68020000)
#define TFT2_DateBaseADDR	  ((u32)0x6C020000)
#define BlockErase_Timeout    ((u32)0x00A00000)
#define ChipErase_Timeout     ((u32)0x30000000) 
#define Program_Timeout       ((u32)0x00001400)

#define ADDR_SHIFT(A)   (Bank1_NOR2_ADDR + (2 * (A)))
#define NOR_WRITE(Address, Data)    (*(vu16 *)(Address) = (Data))
