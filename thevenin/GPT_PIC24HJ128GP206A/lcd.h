
#define LCD_EN1			LATDbits.LATD10
#define LCD_EN2			LATDbits.LATD9
#define LCD_RS			LATDbits.LATD8
#define LCD_EN1_TRIS	TRISDbits.TRISD10
#define LCD_EN2_TRIS	TRISDbits.TRISD9
#define LCD_RS_TRIS		TRISDbits.TRISD8


void DelayUs(unsigned int delay);
void LCD_STROBE1(void);
void LCD_STROBE2(void);
void lcd_cmd(unsigned char c);
void lcd_data(unsigned char c);
void lcd_home(void);
void lcd_line2(void);
void lcd_line3(void);
void lcd_line4(void);
void lcd_puts(const char * s);
void lcd_init(void);
void NumDisp(unsigned char num);
void LongIntDisp(unsigned long num,unsigned char len);
void IntDisp(unsigned int num,unsigned char len);
void FracDisp(unsigned int num,unsigned char units,unsigned char decimal);
void s16IntDisp(int num,unsigned char len);
void s16FracDisp(int num,unsigned char units,unsigned char decimal);

