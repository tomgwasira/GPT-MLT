
#define max_cal_table 9
#define max_Data_table 11

extern int calibrations[max_cal_table];
extern int Data_table[max_Data_table];

extern int	Va_AVG;
extern int	Vb_AVG;
extern int	Vc_AVG;
extern int	Ia_AVG;
extern int	Ib_AVG;
extern int	Ic_AVG;

extern int VaWave[256];
extern int VbWave[256];
extern int VcWave[256];
extern int IaWave[256];
extern int IbWave[256];
extern int IcWave[256];

extern unsigned char waveCounter;


extern long Pwr_a_sum;
extern long Pwr_b_sum;
extern long Pwr_c_sum;

extern long Va_disp_sum;
extern long Vb_disp_sum;
extern long Vc_disp_sum;
extern long Ia_disp_sum;
extern long Ib_disp_sum;
extern long Ic_disp_sum;
extern long Pa_disp_sum;
extern long Pb_disp_sum;
extern long Pc_disp_sum;

extern unsigned long Va_RMS_sum;
extern unsigned long Vb_RMS_sum;
extern unsigned long Vc_RMS_sum;
extern unsigned long Ia_RMS_sum;
extern unsigned long Ib_RMS_sum;
extern unsigned long Ic_RMS_sum;

extern unsigned long Va_AVG_temp;
extern unsigned long Vb_AVG_temp;
extern unsigned long Vc_AVG_temp;
extern unsigned long Ia_AVG_temp;
extern unsigned long Ib_AVG_temp;
extern unsigned long Ic_AVG_temp;
extern unsigned long Vbat_AVG_temp;
extern unsigned long Vext_AVG_temp;

extern int Va_temp;
extern int Vb_temp;
extern int Vc_temp;
extern int Ia_temp;
extern int Ib_temp;
extern int Ic_temp;
extern int Vbat_temp;
extern int Vext_temp;

extern int Va_old;
extern int Vb_old;
extern int Vc_old;

extern int Va_disp;
extern int Vb_disp;
extern int Vc_disp;
extern int Ia_disp;
extern int Ib_disp;
extern int Ic_disp;
extern int Pa_disp;
extern int Pb_disp;
extern int Pc_disp;

extern unsigned int RMS_a_Count;
extern unsigned int RMS_b_Count;
extern unsigned int RMS_c_Count;

extern unsigned int A_disp_cnt;
extern unsigned int B_disp_cnt;
extern unsigned int C_disp_cnt;

extern unsigned int CT_Ratio;

extern unsigned char Zero_cross;
extern unsigned char AVG_Count;

void ADC_Init(void);
void ADC_measure_all(void);
