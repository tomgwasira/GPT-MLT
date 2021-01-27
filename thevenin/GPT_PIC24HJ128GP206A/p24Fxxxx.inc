;;
;; generic include file for PIC24F
;;

.ifdef __24F04KA200
.include "p24F04KA200.inc"
.equ VALID_ID,1
.endif

.ifdef __24F04KA201
.include "p24F04KA201.inc"
.equ VALID_ID,1
.endif

.ifdef __24F08KA101
.include "p24F08KA101.inc"
.equ VALID_ID,1
.endif

.ifdef __24F08KA102
.include "p24F08KA102.inc"
.equ VALID_ID,1
.endif

.ifdef __24F16KA101
.include "p24F16KA101.inc"
.equ VALID_ID,1
.endif

.ifdef __24F16KA102
.include "p24F16KA102.inc"
.equ VALID_ID,1
.endif

.ifdef __24F32KA101
.include "p24F32KA101.inc"
.equ VALID_ID,1
.endif

.ifdef __24F32KA102
.include "p24F32KA102.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ128GA006
.include "p24FJ128GA006.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ128GA008
.include "p24FJ128GA008.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ128GA010
.include "p24FJ128GA010.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ128GA106
.include "p24FJ128GA106.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ128GA108
.include "p24FJ128GA108.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ128GA110
.include "p24FJ128GA110.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ128GB106
.include "p24FJ128GB106.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ128GB108
.include "p24FJ128GB108.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ128GB110
.include "p24FJ128GB110.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ16GA002
.include "p24FJ16GA002.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ16GA004
.include "p24FJ16GA004.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ192GA106
.include "p24FJ192GA106.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ192GA108
.include "p24FJ192GA108.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ192GA110
.include "p24FJ192GA110.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ192GB106
.include "p24FJ192GB106.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ192GB108
.include "p24FJ192GB108.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ192GB110
.include "p24FJ192GB110.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ256GA106
.include "p24FJ256GA106.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ256GA108
.include "p24FJ256GA108.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ256GA110
.include "p24FJ256GA110.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ256GB106
.include "p24FJ256GB106.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ256GB108
.include "p24FJ256GB108.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ256GB110
.include "p24FJ256GB110.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ32GA002
.include "p24FJ32GA002.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ32GA004
.include "p24FJ32GA004.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ48GA002
.include "p24FJ48GA002.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ48GA004
.include "p24FJ48GA004.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ64GA002
.include "p24FJ64GA002.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ64GA004
.include "p24FJ64GA004.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ64GA006
.include "p24FJ64GA006.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ64GA008
.include "p24FJ64GA008.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ64GA010
.include "p24FJ64GA010.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ64GA106
.include "p24FJ64GA106.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ64GA108
.include "p24FJ64GA108.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ64GA110
.include "p24FJ64GA110.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ64GB106
.include "p24FJ64GB106.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ64GB108
.include "p24FJ64GB108.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ64GB110
.include "p24FJ64GB110.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ96GA006
.include "p24FJ96GA006.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ96GA008
.include "p24FJ96GA008.inc"
.equ VALID_ID,1
.endif

.ifdef __24FJ96GA010
.include "p24FJ96GA010.inc"
.equ VALID_ID,1
.endif

.ifndef VALID_ID
.error "processor ID not specified in generic include file"
.endif
