#ifndef TYPEDEF_H
#define TYPEDEF_H


typedef unsigned char  		    BOOLEAN;
typedef unsigned char  		    INT8U; 
typedef signed   char  		    INT8S;
typedef unsigned short 	    	INT16U;
typedef signed   short 	    	INT16S;
typedef unsigned long     		INT32U;  
typedef signed   long     		INT32S;
//typedef unsigned long long   INT64U; 
//typedef signed   long long   INT64S;
	
typedef volatile unsigned char  INT8VU;
typedef volatile unsigned short INT16VU;
typedef volatile unsigned long  INT32VU;

//typedef unsigned char			UINT8;	
//typedef unsigned short 	    	UINT16;	
//typedef unsigned long     		UINT32;

typedef signed   char  		    SINT8;	
typedef signed   short 	    	SINT16;	
typedef signed   long     		SINT32;	

typedef unsigned char  		    u8; 
typedef signed   char  		    s8;
typedef unsigned short 	    	u16;
typedef signed   short 	    	s16;
//typedef unsigned long     		u32;  
//typedef signed   long     		s32;
#define DATA	
#define IDATA	
#define XDATA	
#define CODE	const	

#define     BUSY        1
#define     NBUSY       0

#define 	ON			1
#define 	OFF			0

#define 	FALSE       0
#define 	TRUE        1

#define RGB565CONVERT(red, green, blue)\
(INT16U)( (( red   >> 3 ) << 11 ) | \
(( green >> 2 ) << 5  ) | \
( blue  >> 3 ))


/**************************************************
**NOR Status
**************************************************/
typedef enum
{
  NOR_SUCCESS = 0,
  NOR_ONGOING,
  NOR_ERROR,
  NOR_TIMEOUT
}NOR_Status;

/**************************************************
**NOR_IDTypeDef
**************************************************/
typedef struct
{
	INT16U Manufacturer_Code;
	INT16U Device_Code1;
	INT16U Device_Code2;
	INT16U Device_Code3;
}NOR_IDTypeDef;

/**************************************************
** ͼ���?����, 
** ImageAddrͼ���?��ַ������
**************************************************/
//typedef struct
//{
//	INT16U  XS;			   //�����꿪ʼλ��
//	INT16U  YS;			   //�����꿪ʼλ��
//	INT16U  High;		   //��
//	INT16U  Width;		   //��
//	INT32U	ImageAddr;	   //ͼƬ��NOR FLASH �еĴ洢��ַ
//}_ImageConstStr;

typedef struct
{
	INT16U  High;		   //��
	INT16U  Width;		   //��
	INT32U	ImageAddr;	   //ͼƬ��NOR FLASH �еĴ洢��ַ
}_ImageInfo;

typedef struct
{
	INT8U  	High;		   //��
	INT8U  	Width;		   //��
	INT16U	ByteNum;	   //һ���ַ�ռ�ö����ֽ�
	INT32U	CharBaseAddr;  //�ַ�����flash �еĴ洢��ַ
}_CharInfo;

typedef struct
{
	INT16U  XS;			   //�����꿪ʼλ��
	INT16U  YS;			   //�����꿪ʼλ��
}_ImageConstStr;

typedef struct
{
	INT16U row;			//��ʾ��ʼ��
	INT16U col;			//��ʾ��ʼ��
	INT8U  *ptr;	    //�ַ�ָ��
	INT8U  charnum;	    //��ʾ�ַ���
	INT8U  chartype;	//�ַ�����
	INT16U charColor;	//�ַ���ɫ
	INT16U bkColor;		//������ɫ
}_CharConstStr;

/**************************************************
** �ַ�����������
**************************************************/
//Ӣ��
typedef struct _strPicCharStr
{
	const INT8U	Col;		//�ַ�����
	const INT8U	Row;		//�ַ��߶�
	const INT8U *P;		    //�ַ�����ʼ��ַ
}strPicCharStr;

//Ӣ��
typedef struct _strCharStrColIndex
{
	const INT8U		Col;		//�ַ�����
	const INT32U	DifAddr;	//�ַ�ƫ�Ƶ�ַ
}strCharStrColIndex;

//����
typedef struct _strPicChinaStr
{
	const INT8U	Col;		//�ַ�����
	const INT8U	Row;		//�ַ��߶�
	const INT8U *P;		    //�ַ�����ʼ��ַ
	const INT8U *PCode;		//�������ı�����ʼ��ַ
}strPicChinaStr;

//����
//typedef struct _strPicChinaStr
//{
//	const INT8U	Col;		//�ַ�����
//	const INT8U	Row;		//�ַ��߶�
//	const INT8U *P;		    //�ַ�����ʼ��ַ
//	const INT8U *PCode;		//�������ı�����ʼ��ַ
//}strPicChinaStr;
/**************************************************
**MachineErro
**************************************************/
typedef enum
{
	GOOD  = 0, 
	ERRO1 = 1,
	ERRO2 = 2,
	ERRO3 = 3,
	ERRO4 = 4,
}MachineErro;

/**************************************************
** EEPROM
**************************************************/
typedef enum 
{
	EEPROM1    = 0x00, 
	EEPROM2    = 0x01, 
} _EEPROM;

/**************************************************
** ����
**************************************************/
typedef enum 
{
	UART1    = 0x00, 
	UART2    = 0x01, 
	UART3    = 0x02,
} _UART;
		
/**************************************************
** ��ʾ���ڻ���ʱ��ö��
**************************************************/
typedef enum
{
	DATE  = 0,
	CLOCK = 1,
}ClockOrDate;

/**************************************************
** ���屳��ɫö��
**************************************************/
typedef enum
{
	BKCOLOR   = 0,
	NOBKCOLOR = 1,
}DefBKColor;

/**************************************************
** ����������ģʽö��
**************************************************/
//typedef enum 
//{
//	KEYALM   = 0,		//������
//	ERROALM  = 1,		//������
//    STARTALM = 2,		//������
//    ERROALM2 = 3,       //3000V������
//} ALM_WORKTYPE;			//����������ģʽ

	

/**************************************************
** �˵����ú궨��
**************************************************/
#define    SETUPMENUNUM		  8 	 //���ò˵�����

/**************************************************
** ͨ��Э��
**************************************************/
#define    HOSTSTART		  0xE6
#define    SERVERSTART		  0x86

#define    CMD0               0x00   //�㲥����
#define    CMD1               0x01 	 //ͨ����������
#define    CMD2               0x02 	 //״̬��������
#define    CMD3               0x03 	 //
#define    CMD4               0x04   //
#define    CMD5               0x05   //�㲥����

/**************************************************
** �����궨��
**************************************************/



//#define TIMER_DATA_CHANGE	100      //100ms
//#define TIMER_CONFIG        10000    //30s  �޸�Ϊ 10s
//#define TIMER_MODIFY        10000    //10S

#define TIMER_BASE_100MS    1000    //10ms
#define TIMER_CONFIG        (10000/TIMER_BASE_100MS)    //5s ,��λΪ100ms
#define TIMER_MODIFY        (10000/TIMER_BASE_100MS)    //5S ,��λΪ100ms

// ������޸�����ʱ����λ��˸ʱ��?
//#define COUNT_FLICKER_ON	    500				// ��˸����ʱ��
//#define COUNT_FLICKER_OFF		200				// ��˸���ʱ��?
#define COUNT_FLICKER_ON	    500				// ��˸����ʱ��
#define COUNT_FLICKER_OFF		300				// ��˸���ʱ��?

// �������ȴ�ģʽ��ʱ��
//#define DIGITAL_LED_REFRESH_DELAY_LONG  800000    	// 1=1S
#define DIGITAL_LED_LOAD_CNT_PER	100			// �������ȴ���ʱ����ʱ��
#define DIGITAL_LED_LOAD_TIME_LONG  (800000/TIMER_BASE_100MS)    	// 1=1S

// ����ҳ�棬���ֵ���?��ֵ����
//#define MAX_CFGPARA       	5      	 //���ò���������
//#define MAX_ADDR        	255      //1~255	  ��ַ
//#define MAX_FINISHEDLED     3    	 //3   7����ɫ
//#define MAX_WTTIME        	5    	 //5
//#define MAX_FNDEFINE     	1    	 //1  
//#define MAX_DISPMODE        1    	 //1
//
//#define DEF_ADDR        	1     	 //1	  Ĭ��ֵ
//#define DEF_FINISHEDLED     0    	 //0  
//#define DEF_WTTIME        	5    	 //5
//#define DEF_FNDEFINE     	0    	 //0  
//#define DEF_DISPMODE        0    	 //0

// �������궨��
#define 	ALMSTP		0		//ֹͣģʽ
#define 	ALMKEY		1		//������ģʽ
#define 	ALMERR		2		//������ģʽ
#define 	ALMSEL		3		//�ְ���ģʽ
#define		ALMBAT		4		//Ԥ��������ʱ����ģʽ 
#define		ALMCNT		5		//������׼����ģʽ 		

// �������ֵ���?��ֵ����
#define MAX_ADDR        	247      //1~255	  ��ַ
#define MAX_HUMIDITYSET       	99      //1~255	  ��ַ
#define MAX_HUM_CAL_SET		19
#define MAX_TEM_CAL_SET		19
#define MAX_LANGUAGE_SWITCH		2
//#define MAX_FINISHEDLED     7    	 //3  
#define MAX_FINISHED_LED_DISP   0x17     //17  F1 ������ɺ����ƺͻ���?2����������4λΪ����״̬����4λΪ����ģʽ
#define MAX_FINISHEDLED         7    	 //7       MAX_FINISHED_LED_DISP ����״̬����?
#define MAX_FINISHEDBACKDISP    1        //1       MAX_FINISHED_LED_DISP ��������?
#define MAX_WTTIME        	5    	 //5
#define MAX_FNDEFINE     	1    	 //1  
#define MAX_DISPMODE        3    	 //1
#define MAX_KEYPROCLED      8    	 //1

#define DEF_ADDR        	1     	 //1	  Ĭ��ֵ
//#define DEF_FINISHEDLED     0    	 //0  
#define DEF_HUMIDITYSET          1//3    //       
#define DEF_HUMCALSET          0 
#define DEF_TEMCALSET          0    
#define DEF_LANGUAGE_SWITCH          0
#define DEF_FINISHED_LED_DISP  0x00    //       �ϲ�2��������ֵ
#define DEF_FINISHEDLED         0    	 //0      ������ɺ�����״�?��0==��1=�죬2=�̣�3=����4=�ƣ�5=�ۣ�6=�࣬7=��
#define DEF_FINISHEDBACKDISP    0    	 //0      ������ɺ��Ƿ���ԣ�0=�����ԣ�1=����
#define DEF_WTTIME        	5    	 //5
#define DEF_FNDEFINE    	0    	 //0  
#define DEF_DISPMODE        0    	 //0
#define DEF_KEYPROCLED      0    	 //0

#endif



