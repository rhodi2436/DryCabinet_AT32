#include  "../Head file/includes.h" 
#define POLYNOMIAL1  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001

void I2ckey_StartCondition(void)
{
  SET_SDAKEY_BIT();
  Delay_us(1);
  SET_SCLKEY_BIT();
  Delay_us(1);
  RESET_SDAKEY_BIT();
  Delay_us(10);  // hold time start condition (t_HD;STA)
  RESET_SCLKEY_BIT();
  Delay_us(10);
}

void	IICKEY_SetI2cAdr(u8 i2cAddress)
{
	_i2ckeyAddress = i2cAddress;
}

etError I2ckey_WriteByte(u8 txByte)
{
  etError error = NO_ERROR;
  u8     mask;
  for(mask = 0x80; mask > 0; mask >>= 1)// shift bit for masking (8 times)
  {
    if((mask & txByte) == 0) RESET_SDAKEY_BIT(); // masking txByte, write bit to SDA-Line
    else                     SET_SDAKEY_BIT();
    Delay_us(1);               // data set-up time (t_SU;DAT)
    SET_SCLKEY_BIT();                         // generate clock pulse on SCL
    Delay_us(5);               // SCL high time (t_HIGH)
    RESET_SCLKEY_BIT();
    Delay_us(1);               // data hold time(t_HD;DAT)
  }
  SET_SDAKEY_BIT();                           // release SDA-line
  SET_SCLKEY_BIT();                           // clk #9 for ack
  Delay_us(1);                 // data set-up time (t_SU;DAT)
  if(READ_SDAKEY_BIT) error = ACK_ERROR;       // check ack from i2c slave
  RESET_SCLKEY_BIT();
  Delay_us(20);                // wait to see byte package on scope
  return error;                         // return error code
}


etError I2ckey_ReadByte(INT8U *rxByte, etI2cAck ack, INT8U timeout)
{
  etError error = NO_ERROR;
  INT8U mask;
  *rxByte = 0x00;
  SET_SDAKEY_BIT();                            // release SDA-line
  for(mask = 0x80; mask > 0; mask >>= 1) // shift bit for masking (8 times)
  { 
    SET_SCLKEY_BIT();                          // start clock on SCL-line
    Delay_us(1);                // clock set-up time (t_SU;CLK)
    Delay_us(timeout);// wait while clock streching
    Delay_us(3);                // SCL high time (t_HIGH)
    if(READ_SDAKEY_BIT) *rxByte |= mask;        // read bit
    RESET_SCLKEY_BIT();
    Delay_us(1);                // data hold time(t_HD;DAT)
  }
  if(ack == ACK) RESET_SDAKEY_BIT();              // send acknowledge if necessary
  else           SET_SDAKEY_BIT();
  Delay_us(1);                  // data set-up time (t_SU;DAT)
  SET_SCLKEY_BIT();                            // clk #9 for ack
  Delay_us(5);                  // SCL high time (t_HIGH)
  RESET_SCLKEY_BIT();
  SET_SDAKEY_BIT();                            // release SDA-line
  Delay_us(20);                 // wait to see byte package on scope
  
  return error;                          // return with no error
}

static INT8U IICKEY_CalcCrc(INT8U data[], INT8U nbrOfBytes)
{
  INT8U bit;        // bit mask
  INT8U crc = 0xFF; // calculated checksum
  INT8U byteCtr;    // byte counter
  
  // calculates 8-Bit checksum with given polynomial
  for(byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++)
  {
    crc ^= (data[byteCtr]);
    for(bit = 8; bit > 0; --bit)
    {
      if(crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL1;
      else           crc = (crc << 1);
    }
  }
  
  return crc;
}
static etError IICKEY_CheckCrc(INT8U data[], INT8U nbrOfBytes, INT8U checksum)
{
  INT8U crc;     // calculated checksum
  
  // calculates 8-Bit checksum
  crc = IICKEY_CalcCrc(data, nbrOfBytes);
  
  // verify checksum
  if(crc != checksum) return CHECKSUM_ERROR;
  else                return NO_ERROR;
}
static etError IICKEY_StartReadAccess(void)
{
  etError error; // error code

  // write a start condition
  I2ckey_StartCondition();

  // write the sensor I2C address with the read flag
  error = I2ckey_WriteByte(_i2ckeyAddress << 1 | 0x01);

  return error;
}


static etError IICKEY_Read2BytesAndCrc(INT16U* data, etI2cAck finaleAckNack,
                                      INT8U timeout)
{
  etError error;    // error code
  INT8U     bytes[2]; // read data array
  INT8U     checksum; // checksum byte
 
  // read two data bytes and one checksum byte
                        error = I2ckey_ReadByte(&bytes[0], ACK, timeout);
  if(error == NO_ERROR) error = I2ckey_ReadByte(&bytes[1], ACK, 0);
  if(error == NO_ERROR) error = I2ckey_ReadByte(&checksum, finaleAckNack, 0);
  
   // verify checksum
  if(error == NO_ERROR) error = IICKEY_CheckCrc(bytes, 2, checksum);
 
  // combine the two bytes to a 16-bit value
  *data = (bytes[0] << 8) | bytes[1];
  
  return error;
}

INT8U iic_key_read(void)
{
	  etError error = NO_ERROR;        // error code
	INT16U     rawValueTemp; // temperature raw value from sensor
	IICKEY_SetI2cAdr(0x50);
	if(error == NO_ERROR) error = IICKEY_StartReadAccess();
	
	if(error == NO_ERROR) error = IICKEY_Read2BytesAndCrc(&rawValueTemp, NACK, 0);

	return rawValueTemp >> 8;
}


