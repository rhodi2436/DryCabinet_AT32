#include "../Head file/includes.h" 				  // 主头文件

#define write 0
#define read  1

#define POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001
/*******************************************************************
 温湿度获取函数               
函数原型: SHT30_read_result(u8 addr);
功能: 用来接收从器件采集并合成温湿度
********************************************************************/ 
void I2c_StartCondition(void)
{
  SET_SDA_BIT();
  Delay_us(1);
  SET_SCL_BIT();
  Delay_us(1);
  RESET_SDA_BIT();
  Delay_us(10);  // hold time start condition (t_HD;STA)
  RESET_SCL_BIT();
  Delay_us(10);
}


etError I2c_WriteByte(u8 txByte)
{
  etError error = NO_ERROR;
  u8     mask;
  for(mask = 0x80; mask > 0; mask >>= 1)// shift bit for masking (8 times)
  {
    if((mask & txByte) == 0) RESET_SDA_BIT(); // masking txByte, write bit to SDA-Line
    else                     SET_SDA_BIT();
    Delay_us(1);               // data set-up time (t_SU;DAT)
    SET_SCL_BIT();                         // generate clock pulse on SCL
    Delay_us(5);               // SCL high time (t_HIGH)
    RESET_SCL_BIT();
    Delay_us(1);               // data hold time(t_HD;DAT)
  }
  SET_SDA_BIT();                           // release SDA-line
  SET_SCL_BIT();                           // clk #9 for ack
  Delay_us(1);                 // data set-up time (t_SU;DAT)
  if(READ_SDA_BIT) error = ACK_ERROR;       // check ack from i2c slave
  RESET_SCL_BIT();
  Delay_us(20);                // wait to see byte package on scope
  return error;                         // return error code
}


static etError SHT3X_StartReadAccess(void)
{
  etError error; // error code

  // write a start condition
  I2c_StartCondition();

  // write the sensor I2C address with the read flag
  error = I2c_WriteByte(_i2cAddress << 1 | 0x01);

  return error;
}


etError I2c_ReadByte(INT8U *rxByte, etI2cAck ack, INT8U timeout)
{
  etError error = NO_ERROR;
  INT8U mask;
  *rxByte = 0x00;
  SET_SDA_BIT();                            // release SDA-line
  for(mask = 0x80; mask > 0; mask >>= 1) // shift bit for masking (8 times)
  { 
    SET_SCL_BIT();                          // start clock on SCL-line
    Delay_us(1);                // clock set-up time (t_SU;CLK)
    Delay_us(timeout);// wait while clock streching
    Delay_us(3);                // SCL high time (t_HIGH)
    if(READ_SDA_BIT) *rxByte |= mask;        // read bit
    RESET_SCL_BIT();
    Delay_us(1);                // data hold time(t_HD;DAT)
  }
  if(ack == ACK) RESET_SDA_BIT();              // send acknowledge if necessary
  else           SET_SDA_BIT();
  Delay_us(1);                  // data set-up time (t_SU;DAT)
  SET_SCL_BIT();                            // clk #9 for ack
  Delay_us(5);                  // SCL high time (t_HIGH)
  RESET_SCL_BIT();
  SET_SDA_BIT();                            // release SDA-line
  Delay_us(20);                 // wait to see byte package on scope
  
  return error;                          // return with no error
}



static INT8U SHT3X_CalcCrc(INT8U data[], INT8U nbrOfBytes)
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
      if(crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
      else           crc = (crc << 1);
    }
  }
  
  return crc;
}



static etError SHT3X_CheckCrc(INT8U data[], INT8U nbrOfBytes, INT8U checksum)
{
  INT8U crc;     // calculated checksum
  
  // calculates 8-Bit checksum
  crc = SHT3X_CalcCrc(data, nbrOfBytes);
  
  // verify checksum
  if(crc != checksum) return CHECKSUM_ERROR;
  else                return NO_ERROR;
}



static etError SHT3X_Read2BytesAndCrc(INT16U* data, etI2cAck finaleAckNack,
                                      INT8U timeout)
{
  etError error;    // error code
  INT8U     bytes[2]; // read data array
  INT8U     checksum; // checksum byte
 
  // read two data bytes and one checksum byte
                        error = I2c_ReadByte(&bytes[0], ACK, timeout);
  if(error == NO_ERROR) error = I2c_ReadByte(&bytes[1], ACK, 0);
  if(error == NO_ERROR) error = I2c_ReadByte(&checksum, finaleAckNack, 0);
  
//  // verify checksum
  if(error == NO_ERROR) error = SHT3X_CheckCrc(bytes, 2, checksum);
  
  // combine the two bytes to a 16-bit value
  *data = (bytes[0] << 8) | bytes[1];
  
  return error;
}



void I2c_StopCondition(void)
{
  RESET_SCL_BIT();
  Delay_us(1);
  RESET_SDA_BIT();
  Delay_us(1);
  SET_SCL_BIT();
  Delay_us(10);  // set-up time stop condition (t_SU;STO)
  SET_SDA_BIT();
  Delay_us(10);
}



static etError SHT3X_WriteCommand(etCommands command)
{
  etError error; // error code

  // write the upper 8 bits of the command to the sensor
  error  = I2c_WriteByte(command >> 8);

  // write the lower 8 bits of the command to the sensor
  error |= I2c_WriteByte(command & 0xFF);

  return error;
}



static float SHT3X_CalcTemperature(INT16U rawValue)
{
  // calculate temperature [癈]
  // T = -45 + 175 * rawValue / (2^16-1)
	
  float raw_temp = 	(float)rawValue * 175.0f / 65535.0f - 45.0f;
	
	
  if(para_bk.str.SHT30_Tem_Cal_SET >= 10)
  {
    raw_temp += (para_bk.str.SHT30_Tem_Cal_SET - 10) ;
  }
  //if(para_bk.str.SHT30_Tem_Cal_SET >= 0 && para_bk.str.SHT30_Tem_Cal_SET < 10)
  else
  {	//设定值
    if(raw_temp  <= para_bk.str.SHT30_Tem_Cal_SET)
	{ raw_temp =  0; }
    else
	{ raw_temp -= para_bk.str.SHT30_Tem_Cal_SET;}
//    return (175.0f * (float)rawValue / 65535.0f - 45.0f - para_bk.str.SHT30_Tem_Cal_SET );
  }
  
  return (raw_temp);
  //return 175.0f * (float)rawValue / 65535.0f - 45.0f;
}



static float SHT3X_CalcHumidity(INT16U rawValue)
{
  // calculate relative humidity [%RH]
  // RH = rawValue / (2^16-1) * 100
	
  float raw_hum = 	(float)rawValue * 100.0f / 65535.0f;
	
  if(para_bk.str.SHT30_Hum_Cal_SET >= 10)
  {
//    return (100.0f * (float)rawValue / 65535.0f + para_bk.str.SHT30_Hum_Cal_SET - 10 );
	  raw_hum += (para_bk.str.SHT30_Hum_Cal_SET - 10 );
  }
//  if(para_bk.str.SHT30_Hum_Cal_SET >= 0 && para_bk.str.SHT30_Hum_Cal_SET < 10)
  else
  {
    //if(100.0f * (float)rawValue / 65535.0f - para_bk.str.SHT30_Hum_Cal_SET <= 0)
	if( raw_hum <= para_bk.str.SHT30_Hum_Cal_SET) 
    //return 0;
	{ raw_hum = 0; }
    else
    //return (100.0f * (float)rawValue / 65535.0f - para_bk.str.SHT30_Hum_Cal_SET );
	{ raw_hum -= para_bk.str.SHT30_Hum_Cal_SET; }
  }

  return(raw_hum); 
  //return (100.0f * (float)rawValue / 65535.0f );
}



etError SHT3X_GetTempAndHumiClkStretch(float* temperature, float* humidity,
                                       etRepeatability repeatability,
                                       INT8U timeout)
{
  etError error;        // error code
  u16    rawValueTemp; // temperature raw value from sensor
  u16    rawValueHumi; // humidity raw value from sensor
  
  error = SHT3X_StartWriteAccess();
  
  // if no error ...
  if(error == NO_ERROR)
  {
    // start measurement in clock stretching mode
    // use depending on the required repeatability, the corresponding command
    switch(repeatability)
    {
      case REPEATAB_LOW:
        error = SHT3X_WriteCommand(CMD_MEAS_CLOCKSTR_L);
        break;
      case REPEATAB_MEDIUM:
        error = SHT3X_WriteCommand(CMD_MEAS_CLOCKSTR_M);
        break;
      case REPEATAB_HIGH:
        error = SHT3X_WriteCommand(CMD_MEAS_CLOCKSTR_H);
        break;
      default:
        error = PARM_ERROR;
        break;
    }
  }

  // if no error, start read access
  if(error == NO_ERROR) error = SHT3X_StartReadAccess();
  // if no error, read temperature raw values
  if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(&rawValueTemp, ACK, timeout);
  // if no error, read humidity raw values
  if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(&rawValueHumi, NACK, 0);
  
  I2c_StopCondition();
  
  // if no error, calculate temperature in 癈 and humidity in %RH
  if(error == NO_ERROR)
  {
    *temperature = SHT3X_CalcTemperature(rawValueTemp);
    *humidity = SHT3X_CalcHumidity(rawValueHumi);
  }
  
  return error;
}

//-----------------------------------------------------------------------------
etError SHT3X_GetTempAndHumiPolling(float* temperature, float* humidity,
                                    etRepeatability repeatability,
                                    INT8U timeout)
{
  etError error;           // error code
  INT16U    rawValueTemp;    // temperature raw value from sensor
  INT16U    rawValueHumi;    // humidity raw value from sensor
  
  error  = SHT3X_StartWriteAccess();
  
  // if no error ...
  if(error == NO_ERROR)
  {
    // start measurement in polling mode
    // use depending on the required repeatability, the corresponding command
    switch(repeatability)
    {
      case REPEATAB_LOW:
        error = SHT3X_WriteCommand(CMD_MEAS_POLLING_L);
        break;
      case REPEATAB_MEDIUM:
        error = SHT3X_WriteCommand(CMD_MEAS_POLLING_M);
        break;
      case REPEATAB_HIGH:
        error = SHT3X_WriteCommand(CMD_MEAS_POLLING_H);
        break;
      default:
        error = PARM_ERROR;
        break;
    }
  }
  
  // if no error, wait until measurement ready
  if(error == NO_ERROR)
  {
    // poll every 1ms for measurement ready until timeout
    while(timeout--)
    {
      // check if the measurement has finished
      error = SHT3X_StartReadAccess();
  
      // if measurement has finished -> exit loop
      if(error == NO_ERROR) break;
  
      // delay 1ms
      Delay_us(1000);
    }
    
    // check for timeout error
    if(timeout == 0) error = TIMEOUT_ERROR;
  }
  
  // if no error, read temperature and humidity raw values
  if(error == NO_ERROR)
  {
    error |= SHT3X_Read2BytesAndCrc(&rawValueTemp, ACK, 0);
    error |= SHT3X_Read2BytesAndCrc(&rawValueHumi, NACK, 0);
  }
  
  I2c_StopCondition();
  
  // if no error, calculate temperature in 癈 and humidity in %RH
  if(error == NO_ERROR)
  {
    *temperature = SHT3X_CalcTemperature(rawValueTemp);
    *humidity = SHT3X_CalcHumidity(rawValueHumi);
  }
  
  return error;
}


etError SHT3X_GetTempAndHumi(float* temperature, float* humidity,
                             etRepeatability repeatability, etMode mode,
                             INT8U timeout)
{
  etError error;
                               
  switch(mode)
  {    
    case MODE_CLKSTRETCH: // get temperature with clock stretching mode
      error = SHT3X_GetTempAndHumiClkStretch(temperature, humidity,
                                             repeatability, timeout);
      break;
    case MODE_POLLING:    // get temperature with polling mode
      error = SHT3X_GetTempAndHumiPolling(temperature, humidity,
                                          repeatability, timeout);
      break;
    default:              
      error = PARM_ERROR;
      break;
  }
  
  return error;
}
//-----------------------------------------------------------------------------
etError SHT3X_StartPeriodicMeasurment(etRepeatability repeatability,
                                      etFrequency frequency)
{
  etError error;        // error code
  
  error = SHT3X_StartWriteAccess();
  
  // if no error, start periodic measurement 
  if(error == NO_ERROR)
  {
    // use depending on the required repeatability and frequency,
    // the corresponding command
    switch(repeatability)
    {
      case REPEATAB_LOW: // low repeatability
        switch(frequency)
        {
          case FREQUENCY_HZ5:  // low repeatability,  0.5 Hz
            error |= SHT3X_WriteCommand(CMD_MEAS_PERI_05_L);
            break;          
          case FREQUENCY_1HZ:  // low repeatability,  1.0 Hz
            error |= SHT3X_WriteCommand(CMD_MEAS_PERI_1_L);
            break;          
          case FREQUENCY_2HZ:  // low repeatability,  2.0 Hz
            error |= SHT3X_WriteCommand(CMD_MEAS_PERI_2_L);
            break;          
          case FREQUENCY_4HZ:  // low repeatability,  4.0 Hz
            error |= SHT3X_WriteCommand(CMD_MEAS_PERI_4_L);
            break;          
          case FREQUENCY_10HZ: // low repeatability, 10.0 Hz
            error |= SHT3X_WriteCommand(CMD_MEAS_PERI_10_L);
            break;          
          default:
            error |= PARM_ERROR;
            break;
        }
        break;
        
      case REPEATAB_MEDIUM: // medium repeatability
        switch(frequency)
        {
          case FREQUENCY_HZ5:  // medium repeatability,  0.5 Hz
            error |= SHT3X_WriteCommand(CMD_MEAS_PERI_05_M);
			break;
          case FREQUENCY_1HZ:  // medium repeatability,  1.0 Hz
            error |= SHT3X_WriteCommand(CMD_MEAS_PERI_1_M);
			break;        
          case FREQUENCY_2HZ:  // medium repeatability,  2.0 Hz
            error |= SHT3X_WriteCommand(CMD_MEAS_PERI_2_M);
			break;        
          case FREQUENCY_4HZ:  // medium repeatability,  4.0 Hz
            error |= SHT3X_WriteCommand(CMD_MEAS_PERI_4_M);
			break;      
          case FREQUENCY_10HZ: // medium repeatability, 10.0 Hz
            error |= SHT3X_WriteCommand(CMD_MEAS_PERI_10_M);
			break;
          default:
            error |= PARM_ERROR;
			break;
        }
        break;
        
      case REPEATAB_HIGH: // high repeatability
        switch(frequency)
        {
          case FREQUENCY_HZ5:  // high repeatability,  0.5 Hz
            error |= SHT3X_WriteCommand(CMD_MEAS_PERI_05_H);
            break;
          case FREQUENCY_1HZ:  // high repeatability,  1.0 Hz
            error |= SHT3X_WriteCommand(CMD_MEAS_PERI_1_H);
            break;
          case FREQUENCY_2HZ:  // high repeatability,  2.0 Hz
            error |= SHT3X_WriteCommand(CMD_MEAS_PERI_2_H);
            break;
          case FREQUENCY_4HZ:  // high repeatability,  4.0 Hz
            error |= SHT3X_WriteCommand(CMD_MEAS_PERI_4_H);
            break;
          case FREQUENCY_10HZ: // high repeatability, 10.0 Hz
            error |= SHT3X_WriteCommand(CMD_MEAS_PERI_10_H);
            break;
          default:
            error |= PARM_ERROR;
            break;
        }
        break;
      default:
        error |= PARM_ERROR;
        break;
    }
  }

  I2c_StopCondition();

  return error;
}

//-----------------------------------------------------------------------------
etError SHT3X_ReadMeasurementBuffer( SHT30_PAR *sht30_ptr )//(float* temperature, float* humidity)
{
  etError  error;        // error code
  INT16U     rawValueTemp; // temperature raw value from sensor
  INT16U     rawValueHumi; // humidity raw value from sensor

  error = SHT3X_StartWriteAccess();

  // if no error, read measurements
  if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_FETCH_DATA);
  if(error == NO_ERROR) error = SHT3X_StartReadAccess();  
  if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(&rawValueTemp, ACK, 0);
  if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(&rawValueHumi, NACK, 0);

  // if no error, calculate temperature in 癈 and humidity in %RH
  if(error == NO_ERROR)
  {
    //*temperature = SHT3X_CalcTemperature(rawValueTemp);
//	  if(*temperature  < 10)
//		  *temperature -= 6;
//	  else if((*temperature < 20) && (*temperature > 10))
//		  *temperature -= 8; 
//	  else if((*temperature < 30) && (*temperature > 20))
//		   *temperature -= 10; //lcq 注释掉
    //*humidity = SHT3X_CalcHumidity(rawValueHumi);
	  
	 sht30_ptr->SHT30_Temprature = SHT3X_CalcTemperature(rawValueTemp);
	 sht30_ptr->SHT30_Humidity   = SHT3X_CalcHumidity(rawValueHumi);

	 // 保存采集值
	 if(sht30_ptr->SampCnt >= SHT30_SAMP_NUM) 
	 { sht30_ptr->SampCnt = 0; }
	 
	 sht30_ptr->TemRawVal[sht30_ptr->SampCnt] = rawValueTemp;
	 sht30_ptr->HumRawVal[sht30_ptr->SampCnt] = rawValueHumi;
	  
	 sht30_ptr->SampCnt++;
  }

  I2c_StopCondition();
  if(OsCmdSet.set_addr != 1)
  {
  	SetLcdLay( PAGE_RUN );
  }
  return error;
}


static etError SHT3X_StartWriteAccess(void)
{
  etError error; // error code

  // write a start condition
  I2c_StartCondition();

  // write the sensor I2C address with the write flag
  error = I2c_WriteByte(_i2cAddress << 1);

  return error;
}

etError SHT3x_ReadSerialNumber(INT32U* serialNumber)
{
	etError error; // error code
//	INT16U serialNumWords[2];

	error = SHT3X_StartWriteAccess();
	
	return error;
	
}

void	SHT3X_SetI2cAdr(u8 i2cAddress)
{
	_i2cAddress = i2cAddress;

}

void SHT30_read_result(u8 i2cAddress)
{
	INT32U      serialNumber;// serial number
	etError   	error;       // error code
//	float       temperature; // temperature [癈]
//    float       humidity;    // relative humidity [%RH]
	SHT3X_SetI2cAdr(i2cAddress);

	error = SHT3x_ReadSerialNumber(&serialNumber);
	 if(error != NO_ERROR)
	{} // do error handling here

	// demonstrate a single shot measurement with clock-stretching
//	error = SHT3X_GetTempAndHumi(&temperature, &humidity, REPEATAB_HIGH, MODE_POLLING, 50);

	SHT3X_StartPeriodicMeasurment(REPEATAB_HIGH,FREQUENCY_1HZ);
//	sht30_par.SHT30_Temprature = temperature ;
//	sht30_par.SHT30_Humidity  = humidity ;

	if(error != NO_ERROR)
	{} // do error handling here 
}


