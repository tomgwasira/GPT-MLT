//Tables.h automatically created in Excel		
		
#define	max_cal_table	9
#define	max_Data_table	11
		
typedef struct cals		
{		
	const char * Tag;	
	const char * Units;	
	const unsigned char Scale;	
} CalType;

typedef struct tags
{		
	const char * Tag;	
} TagType;		
		
extern int calibrations[max_cal_table];		
extern const CalType Cal_names[max_cal_table];		
extern const CalType Data_names[max_Data_table];		
		
#define	Va_cal	calibrations[0]
#define	Vb_cal	calibrations[1]
#define	Vc_cal	calibrations[2]
#define	Ia_cal	calibrations[3]
#define	Ib_cal	calibrations[4]
#define	Ic_cal	calibrations[5]
#define	Vbat_cal	calibrations[6]
#define	Vext_cal	calibrations[7]
#define	CT_Size	calibrations[8]
		

#define	Va_RMS	Data_table[0]
#define	Vb_RMS	Data_table[1]
#define	Vc_RMS	Data_table[2]
#define	Ia_RMS	Data_table[3]
#define	Ib_RMS	Data_table[4]
#define	Ic_RMS	Data_table[5]
#define	Vbat	Data_table[6]
#define	Vext	Data_table[7]
#define	Pa_W	Data_table[8]
#define	Pb_W	Data_table[9]
#define	Pc_W	Data_table[10]