#ifndef _CANOP_H_
#define _CANOP_H_

//-----
int WR0,WR1;
int fd_CAN;
unsigned int T_accmask,T_acccode,T_baud,T_mode_reg;
int port,operation;
int old_errcode=0,old_status;

char data201[40];
int write200;
int n60c;

unsigned int id;
unsigned int longDispl;
unsigned int latDispl;
unsigned int measStat;
unsigned int length;
unsigned int width;
unsigned int probOfExist;
unsigned int vrelLong;
unsigned int dynProp;



canmsg_t     RecvBuf,RecvBuf1;
Config_par_t cfg;

void CAN_get_status0(void);
void op_AccMask();
void op_AccCode();
void op_Baud   ();
void op_Sleep_mode();
void op_Set_filter();
void op_Self_test_mode();
void op_ListenOnly();
void op_set_mode();
int  str2int(char para[10]);
void bus_run_wrong(int can_num,int fd);
void bus_run_wrong_masked(int can_num,int fd);
//-----
int  PortOpen();
void PortClose();
int  PortSet();
void write_data0(char *da);
void read_data0 (void);

void can_init(void);
void can_close(void);

void dataCanToSerial();

int dataFuc(canmsgs_t msgs);

void CharToHex(unsigned char* buf,char*str,int len);



#endif //_CANOP_H_
