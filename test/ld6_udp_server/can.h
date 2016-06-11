#ifndef __CAN_H
#define __CAN_H
 
#define CAN4LINUXVERSION 0x0303 /*(Version 3.3)*/

#define P  		0		//��ӡ������Ϣ
#define PP      1		//��ӡ������Ϣ
#define CAN   	2
#define RADAR   0      //�����״�

#ifndef __KERNEL__
#include <sys/time.h>
#endif
#include<pthread.h>
int msqid;
extern int sockfd;
extern int connfd;
extern int listenfd;
extern int sock;
extern int len;
extern char buff[512];
extern struct sockaddr_in clientAddr;
extern int udp_recv_flag;
extern pthread_mutex_t mutex;
struct canserial{
	unsigned char data[1024];
	unsigned int len;
};

struct num60b{
    unsigned int rolCount;
    unsigned int ID;
    unsigned int longDispl;
    unsigned int vrellong;
    unsigned int accelLong;
    unsigned int probOfExist;
    unsigned int dynProp;
    unsigned int latDispl;
    unsigned int length;
    unsigned int width;
    unsigned int measStat;
};

struct num60c{
    unsigned int rcsValue;
    unsigned int latSpeed;
    unsigned int  obstacle;
};

struct num702{
    unsigned int target;
    unsigned int pdh;
    unsigned int length;
    unsigned int width;
    unsigned int angstat;
    unsigned int type;
    unsigned int ang;
    unsigned int rcs;
};

struct num701{
    unsigned int target;
    unsigned int distrms;
    unsigned int angrms;
    unsigned int vrelrms;
    unsigned int vrel;
    unsigned int dist;
};

struct num201{
    unsigned int wstatus;
    unsigned int rstatus;
    unsigned int rangellen;
    unsigned int elevation;
    unsigned int power;
    unsigned int radar;
    unsigned int volt;
    unsigned int valid;
    unsigned int def;
    unsigned int temp;

};


struct canserial canserial_t;

 //## the can message structure 
#define CAN_MSG_LENGTH 8                /**< maximum length of a CAN frame */

#define MSG_RTR         (1<<0)          /**< RTR Message */
#define MSG_OVR         (1<<1)          /**< CAN controller Msg overflow error */
#define MSG_EXT         (1<<2)          /**< extended message format */
#define MSG_SELF        (1<<3)          /**< message received from own tx */
#define MSG_PASSIVE     (1<<4)          /**< controller in error passive */
#define MSG_BUSOFF      (1<<5)          /**< controller Bus Off  */
#define MSG_            (1<<6)          /**<  */
#define MSG_BOVR        (1<<7)          /**< receive/transmit buffer overflow */

//## mask used for detecting CAN errors in the canmsg_t flags field
#define MSG_ERR_MASK    (MSG_OVR + MSG_PASSIVE + MSG_BUSOFF + MSG_BOVR)

//## The CAN message structure.
typedef struct {
    //flags, indicating or controlling special message properties 
    int             flags;
    int             cob;         //CAN object number, used in Full CAN 
    unsigned   long id;          //CAN message ID, 4 bytes
    struct timeval  timestamp;   //time stamp for received messages 
    short      int  length;      //number of bytes in the CAN message 
    unsigned   char data[CAN_MSG_LENGTH]; //data, 0...8 bytes
} canmsg_t;

typedef struct {
	canmsg_t mmsg[83];
	int mnum;
}canmsgs_t;

//## IOCTL requests 
#define COMMAND          0      //IOCTL command request 
#define CONFIG           1      //IOCTL configuration request 
#define SEND             2      //IOCTL request
#define RECEIVE          3      //IOCTL request
#define CONFIGURERTR     4      //IOCTL request 
#define STATUS           5      //IOCTL status request

//## CAN ioctl parameter types 
//## IOCTL Command request parameter structure 
struct Command_par {
    int cmd;                    //special driver command 
    int target;                 //special configuration target 
    unsigned long val1;         //1. parameter for the target 
    unsigned long val2;         //2. parameter for the target 
    int error;                  //return value 
    unsigned long retval;       //return value 
};

//## IOCTL Command request parameter structure 
typedef struct Command_par Command_par_t ; //Command parameter struct
//PSW made them all the same IOCTL Configuration request parameter structure 

//## IOCTL CConfiguration request parameter structure 
typedef struct Command_par  Config_par_t ; //Configuration parameter struct


//## IOCTL generic CAN controller status request parameter structure 
typedef struct CanStatusPar {
    unsigned int baud;                  //actual bit rate 
    unsigned int accmask;               //+
    unsigned int acccode;               //+
    unsigned int irq_enable;            //+
    unsigned int mode;                  //+
    unsigned int irq_value;             //+

    unsigned int status;                //CAN controller status register 
    unsigned int error_warning_limit;   //the error warning limit 
    unsigned int rx_errors;             //content of RX error counter 
    unsigned int tx_errors;             //content of TX error counter 
    unsigned int error_code;            //content of error code register 
    unsigned int rx_buffer_size;        //size of rx buffer  
    unsigned int rx_buffer_used;        //number of messages 
    unsigned int tx_buffer_size;        //size of tx buffer  
    unsigned int tx_buffer_used;        //number of messages
    unsigned long retval;               //return value 
    unsigned int type;                  //CAN controller / driver type 
} CanStatusPar_t;

//## IOCTL  CanStatusPar.type CAN controller hardware chips 
#define CAN_TYPE_UNSPEC         0
#define CAN_TYPE_SJA1000        1
#define CAN_TYPE_FlexCAN        2
#define CAN_TYPE_TouCAN         3
#define CAN_TYPE_82527          4
#define CAN_TYPE_TwinCAN        5

//## IOCTL Send request parameter structure 
typedef struct Send_par {
    canmsg_t *Tx;               //CAN message struct  
    int error;                  //return value for errno 
    unsigned long retval;       //return value
} Send_par_t ;

//## IOCTL Receive request parameter structure 
typedef struct Receive_par {
    canmsg_t *Rx;               //CAN message struct  
    int error;                  //return value for errno 
    unsigned long retval;       //return value 
} Receive_par_t ;

//## IOCTL ConfigureRTR request parameter structure 
typedef struct ConfigureRTR_par {
    unsigned message;           //CAN message ID 
    canmsg_t *Tx;               //CAN message struct 
    int error;                  //return value for errno 
    unsigned long retval;       //return value 
} ConfigureRTR_par_t ;


typedef struct msgbuf{
	long mtype;
	canmsgs_t mrx;
}msgbuf_t;

typedef struct {
	long mtype;
	char da[8];
}msgradar_t;


//## IOCTL Command subcommands and there targets 
# define CMD_START              1
# define CMD_STOP               2
# define CMD_RESET              3
# define CMD_CLEARBUFFERS       4

//## IOCTL Configure targets 
#define CONF_ACC       0       //mask and code 
#define CONF_ACCM      1       //mask only 
#define CONF_ACCC      2       //code only 
#define CONF_TIMING    3       //bit timing 
#define CONF_OMODE     4       //output control register 
#define CONF_FILTER    5
#define CONF_FENABLE   6
#define CONF_FDISABLE  7
#define CONF_LISTEN_ONLY_MODE  8       //for SJA1000 PeliCAN 
#define CONF_SELF_RECEPTION    9       
#define CONF_IRQ_EN             10 ////
#define CONF_FILTER_MODE        11 ////
#define CONF_SELF_TEST_MODE     12 ////
#define CONF_SLEEP_MODE         13 ////
#define CONF_MODE               14 ////@@

#endif  /* __CAN_H */

