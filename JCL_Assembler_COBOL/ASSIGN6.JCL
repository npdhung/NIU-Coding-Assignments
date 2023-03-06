//KC03DB5A JOB ,'H. Nguyen',MSGCLASS=H
//*
//*****************************************************************
//*                                                               *
//*  CSCI 465               ASSIGNMENT 6             SPRING 2022  *
//*                                                               *
//*  DEVELOPER NAME: HUNG NGUYEN                                  *
//*  DUE DATE: 04/22/2022                                         *
//*                                                               *
//*  PURPOSE: PROVIDE EXPERIENCE USING 5 ASSEMBLER QSAM MACROS    *
//*  AND PRACTICE ASSMEBLER PROGRAMMING SKILLS                    *
//*                                                               *
//*****************************************************************
//*
//JSTEP01  EXEC PGM=ASMA90,PARM=ASA
//*
//***************************************************************
//*                                                             *
//*  JSTEP01 COMPILES THE ASSEMBLER SOURCE CODE USING CURRENT   *
//*  ASSEMBLER COMPILER                                         *
//*                                                             *
//*  DDNAME              DESCRIPTION                            *
//*                                                             *
//*  SYSLIB     INPUT:   DATA SET WHERE IBM MACROS LOCATED      *
//*  SYSIN      INPUT:   SOURCE CODE TO BE COMPILED             *
//*                                                             *
//*  SYSLIN    OUTPUT:   THE RESULTING OBJECT MODULE            *
//*  SYSPRINT  OUTPUT:   DATA SET TO WHICH THE PROGRAM'S SOURCE *
//*                      LISTING AND MESSAGES FROM THE ASSEMBLY *
//*                      PROCESS ARE TO BE WRITTEN              *
//*  SYSUT1    OUTPUT:   DATA SET REQUIRED BY COMPILER          *
//*  SYSUDUMP  OUTPUT:   OUTPUT INCASE OF AN ADBEND             *
//*                                                             *
//***************************************************************
//*
//SYSLIB   DD DSN=SYS1.MACLIB,DISP=SHR
//*
//SYSIN    DD *
         PRINT  NOGEN                MACRO CALL SUPPRESSION
**********************************************************************
*                                                                    *
* PROGRAM:    ASSEMBL3                                               *
* AUTHOR:     HUNG NGUYEN                                            *
* DATE:       04/22/2022                                             *
*                                                                    *
* FUNCTION:  THIS PROGRAM CREATES A SINGLE REPORT BASED ON           *
*            INFORMATION CONTAINED IN A FILE WITH AN UNKNOWN         *
*            NUMBER OF RECORDS                                       *
*                                                                    *
* INPUT:     INDATA - THE INPUT DATA RECORDS READ IN.                *
*                                                                    *
* OUTPUT:    RPTDATA - THE OUTPUT DATA RECORDS WRITTEN OUT.          *
*                                                                    *
* TEMPORARY DATASET: HSFILE - STORES HIGH SALE RECORDS.              *
*                                                                    *
**********************************************************************
$FUNDREC DSECT
$SHRPRC  DS    ZL5
$FUNDNME DS    CL35
$COMPCT1 DS    ZL6
$COMPCT2 DS    ZL6
$COMPCT3 DS    ZL6
*
$SALEREC DSECT
$BRNCNME DS    CL25
$BRKRNME DS    CL25
$SALEAMT DS    ZL11
$FILLER  DS    CL3
$COMMFLG DS    ZL1
*
ASSEMBL3 CSECT                       BEGIN ASSEMBL3
*
         STM   14,12,12(13)          SAVE CALLER'S REGS
         LR    12,15                 SET R12 TO R15
         USING ASSEMBL3,12           ESTABLISH R12 AS 1ST BASE REG
         LA    14,REGS               R14 -> CURRENT SAVE AREA
         ST    13,4(,14)             SAVE CALLER'S SAVE AREA ADDR
         ST    14,8(,14)             SAVE CURRENT SAVE AREA ADDR
         LR    13,14                 R13 -> CURRENT SAVE AREA
*
         LA    11,4095(,12)  POINT R11 4095 BYTES BEYOND R12
         LA    11,1(,11)     POINT R11 4096 BYTES BEYOND R12
         USING ASSEMBL3+4096,11  ESTABLISH R11 AS SECOND BASE REG
*
         OPEN  (INDCB,(INPUT))       OPEN INPUT FILE
         LTR   15,15                 CHECK FOR SUCCESSFUL OPEN
         BZ    OPENOK1               BRANCH IF SUCCESSFUL TO OPENOK1
         ABEND 222,DUMP              ABEND IF OPEN FAILS & DISPLAY 222
*
OPENOK1  OPEN  (RPTDCB,(OUTPUT))     OPEN REPORT OUTPUT FILE
         LTR   15,15                 CHECK FOR SUCCESSFUL OPEN
         BZ    OPENOK2               BRANCH IF SUCCESSFUL TO OPENOK2
         ABEND 333,DUMP              ABEND IF OPEN FAILS & DISPLAY 333
*
OPENOK2  OPEN  (HSALEDCB,(OUTPUT))   OPEN HIGH SALE OUTPUT FILE
         LTR   15,15                 CHECK FOR SUCCESSFUL OPEN
         BZ    OPENOK3               BRANCH IF SUCCESSFUL TO OPENOK3
         ABEND 444,DUMP              ABEND IF OPEN FAILS & DISPLAY 444
*
OPENOK3  DS    0H
         USING $FUNDREC,2            EST. ADDRESSABILITY FOR FUND REC
         GET   INDCB                 READ THE FIRST RECORD
         LR    2,1                   COPY ADDRESS OF FUND RECORD
*
LOOP0    CLI   EOFFLAG,C'Y'          CHECK FOR END OF INPUT DATA
*
         BE    ENDLOOP1              BRANCH TO ENDLOOP1 IF EOF
*
* GET DATE
*
         TIME  DEC,DATETIME,LINKAGE=SYSTEM,DATETYPE=MMDDYYYY
         MVC   SETDATE(13),=X'40202120206120206120202020'
         ED    SETDATE(13),DATETIME+7  MOVE DATETIME TO SETDATE
         MVC   OUTDATE(10),SETDATE+3   MOVE DATE TO OUTPUT FIELD
*
* GET HOUR AND EDIT TO OUTPUT
*
         MVC   SETHOUR(8),=X'40202120207A2020'
         ED    SETHOUR(8),DATETIME-1  MOVE HOUR FIELD TO SET HOUR
         MVC   OHOUR(5),SETHOUR+3     MOVE HOUR MIN TO OUTPUT FIELD
         PACK  PTEMPHR(2),OHOUR(2)    PACK HOUR
         CP    PTEMPHR(2),=P'12'      COMPARE HOUR WITH 12
         BL    BRLT12                 BRANCH TO BRLT12 IF LESS THAN 12
         BZ    ADJPM                  BRANCH TO ADJPM IF EQUAL 12
*
* BRANCH IF HOUR GREATER THAN 12
BRGT12   SP    PTEMPHR(2),=P'12'      SUBTRACT 12
*
* BRANCH TO CHANGE AM TO PM WHEN HOUR >= 12
ADJPM    MVI   AMPM,C'P'              CHANGE AM TO PM
*
*BRANCH IF HOUR IS LESS THAN 12
BRLT12   CP    PTEMPHR(2),=P'0'       COMPARE HOUR WITH ZERO
         BNZ   PROCREC                BRANCH TO PROCREC IF NOT ZERO
         ZAP   PTEMPHR(2),=P'12'      CHANGE HOUR TO 12
*
* PUT HOUR FIELD BACK TO OUTPUT AFTER ADJUST
*
         MVC   SETHR2(5),=X'4020212020'
         ED    SETHR2(5),PTEMPHR-1    MOVE PTEMPHR FIELD TO SETHR2
         MVC   OHOUR(2),SETHR2+3      MOVE HOUR MIN TO OUTPUT FIELD
*
* PROCESS FIRST RECORD
*
PROCREC  PACK  PSHRPRC(3),$SHRPRC(5)    PACK SHARE PRICE TO STORAGE
         MVC   OFUNDNME(35),$FUNDNME    MOVE FUND NAME FROM RECORD
         PACK  PCOMPCT1(4),$COMPCT1(6)  PACK COMM PCT1 INTO STORAGE
         PACK  PCOMPCT2(4),$COMPCT2(6)  PACK COMM PCT2 INTO STORAGE
         PACK  PCOMPCT3(4),$COMPCT3(6)  PACK COMM PCT3 INTO STORAGE
         DROP  2
*
* WRITE SHARE PRICE TO OUTPUT FIELD
*
         LA    1,OSALEAMT+4  POINT REG 1 RIGHT BEFORE DEC PT
         MVC   OSHRPRC(7),=X'402021204B2020'
         EDMK  OSHRPRC(7),PSHRPRC    OUTPUT SHARE PRICE
         BCTR  1,0                   DECREASE REG 1 BY 1
         MVI   0(1),C'$'             PLACE DOLLAR SIGN
*
         USING $SALEREC,2            EST. ADDRESSABILITY FOR SALE REC
         GET   INDCB                 GET THE SECOND RECORD
         LR    2,1                   COPY ADDRESS OF SALE RECORD
*
         LA    3,99                  SET LINE COUNTER TO 99
*
LOOP1    CLI   EOFFLAG,C'Y'          CHECK FOR END OF INPUT DATA
*
         BE    ENDLOOP1              BRANCH TO ENDLOOP1 IF EOF
*
         AP    RECCTR(2),=P'1'       ADD 1 TO RECORD COUNTER
*
* PROCESS BRANCH NAME, BROKER NAME, AND SALE AMOUNT
*
         MVC   OBRNCNME(25),$BRNCNME     MOVE BRANCH NAME FROM RECORD
         MVC   OBRKRNME(25),$BRKRNME     MOVE BROKER NAME FROM RECORD
         PACK  PSALEAMT(6),$SALEAMT(11)  PACK SALE AMOUNT INTO STORAGE
         MVC   ICOMMFLG(1),$COMMFLG    MOVE COMMISSION FLAG TO STORAGE
*
         DROP  2
*
         LA    1,OSALEAMT+11  POINT REG 1 RIGHT BEFORE DEC PT
         MVC   OSALEAMT(15),XSALEAMT
         EDMK  OSALEAMT(15),PSALEAMT     OUPUT SALE AMOUNT
         BCTR  1,0                       DECREASE REG 1 BY 1
         MVI   0(1),C'$'                 PLACE DOLLAR SIGN
*
* CALCULATE SHARE AMOUNT
*
         ZAP   PCALC(9),PSALEAMT(6)  GET THE DIVIDEND TO LARGE FIELD
         SRP   PCALC(9),4,0          SHIFT 3 DIGIT LEFT FOR FAKE DEC
         DP    PCALC(9),PSHRPRC(3)   DIVIDE
         SRP   PCALC(6),64-1,5       SHIFT QUOTIENT 1 TO THE RIGHT
         ZAP   PSHRAMT(8),PCALC(8)   GET SHARE AMOUNT FROM RESULT
*
         MVC   OSHRAMT(20),XSHRAMT   MOVE PATTERN TO OUTPUT FIELD
         ED    OSHRAMT(20),PSHRAMT   OUPUT SHARE AMOUNT
*
* CHECK IF SHARE >= 150K THEN MOVE TO HSALEDCB
*
         CP    PSHRAMT(8),=P'150000000'
         BL    SKIPHS                SKIP HIGH SALE PROCESS
         MVC   OUTPUT(25),OBRKRNME   MOVE BROKER NAME TO HIGH SALE
         UNPK  ISHRAMT(15),PSHRAMT(8) UNPACK SHARE AMOUNT FOR OUTPUT
         MVC   OUTPUT+25(15),ISHRAMT  MOVE SHARE AMOUNT TO HIGH SALE
*
         PUT   HSALEDCB              WRITE THE DETAIL LINE
         MVC   0(41,1),HSOUT         COPY 41 BYTES FROM HSOUT LINE TO
*                                      0 BYTES OFF THE ADDRESS IN R1
*
* CALCULATE COMMISSION AMOUNT
*
SKIPHS   DS    0H
         CLI   ICOMMFLG,C'1'           IF COMMISSION FLAG = 1
         BE    BR1                     BRANCH TO BR1
         CLI   ICOMMFLG,C'2'           IF COMMISSION FLAG = 2
         BE    BR2                     BRANCH TO BR1
         CLI   ICOMMFLG,C'3'           IF COMMISSION FLAG = 3
         BE    BR3                     BRANCH TO BR1
*
BR1      ZAP   PCOMPCT(4),PCOMPCT1(4)  SET COMM PCT = COMM PCT1
         B     NEXT
BR2      ZAP   PCOMPCT(4),PCOMPCT2(4)  SET COMM PCT = COMM PCT2
         B     NEXT
BR3      ZAP   PCOMPCT(4),PCOMPCT3(4)  SET COMM PCT = COMM PCT3
         B     NEXT
*
NEXT     ZAP   PCALC(10),PSALEAMT(6)    SET COMMAMT = SALEAMT
         MP    PCALC,PCOMPCT            MULTIPLY PCOMPCT
         SRP   PCALC,(64-5),5           SHIFT RIGHT 5
         ZAP   PCOMMAMT(6),PCALC(10)    SET PCOMMAMT = PCOMMCAL
*
         LA    1,OCOMAMT+11  POINT REG 1 RIGHT BEFORE DEC PT
         MVC   OCOMAMT(15),XSALEAMT
         EDMK  OCOMAMT(15),PCOMMAMT      OUPUT COMMISSION AMOUNT
         BCTR  1,0                       DECREASE REG 1 BY 1
         MVI   0(1),C'$'                 PLACE DOLLAR SIGN
*
* IF LINE COUNTER > 18 THEN INCREASE PAGECTR BY 1 AND PRINT HEADER
*
         C     3,=F'18'              COMPARE LINE COUNTER
         BC    B'0100',SKIPHDR       IF LINECTR<18, SKIP WRITE HDR
*
         SR    3,3                   RESET LINE COUNTER TO ZERO
         AP    PPAGECTR(2),=P'1'     ADD 1 TO PAGE COUNTER
*
         MVC   OPAGECTR(4),=X'40202120'
         ED    OPAGECTR(4),PPAGECTR  EDIT PAGE COUNTER INTO OUTPUT
         PUT   RPTDCB,HEADLN1        WRITE HEADLINE 1
         MVC   RPTNAME(42),RPT       MOVE REPORT NAME TO RPTNAME FIELD
         PUT   RPTDCB,HEADLN2        WRITE HEADLINE 2
         PUT   RPTDCB,HEADLN3        WRITE HEADLINE 3
         PUT   RPTDCB,COLHEAD        WRITE COLUMN HEADER
         PUT   RPTDCB,COLDASH        WRITE DASHLINE UNDER COLUMN HEADER
*
SKIPHDR  PUT   RPTDCB,DETAIL           WRITE THE DETAIL LINE
         LA    3,1(,3)                 ADD 1 TO LINE COUNTER
         AP    PTSALAMT(7),PSALEAMT(6) ADD SALE AMOUNT TO TOTAL
         AP    PTSHRAMT(9),PSHRAMT(8)  ADD SHARE AMOUNT TO TOTAL
         AP    PTCOMAMT(7),PCOMMAMT(6) ADD COMMISSION AMT TO TOTAL
*
         USING $SALEREC,2            EST. ADDRESSABILITY FOR SALE REC
         GET   INDCB                 GET THE NEXT RECORD
         LR    2,1                   COPY ADDRESS OF SALE RECORD
*
         B     LOOP1                 BRANCH TO TOP OF LOOP1
*
ENDLOOP1 ED    ORECCTR(4),RECCTR     EDIT REC COUNTER INTO TOTALS
*
* WRITE REPORT TOTAL HEADER
*
         AP    PPAGECTR(2),=P'1'     ADD 1 TO PAGE COUNTER
*
         MVC   OPAGECTR(4),=X'40202120'
         ED    OPAGECTR(4),PPAGECTR  EDIT PAGE COUNTER INTO OUTPUT
         PUT   RPTDCB,HEADLN1        WRITE HEADLINE 1 OF TOTAL REPORT
         MVC   RPTNAME(42),RPTTOT    MOVE REPORT NAME TO RPTNAME FIELD
         PUT   RPTDCB,HEADLN2        WRITE HEADLINE 2 OF TOTAL REPORT
         PUT   RPTDCB,HEADLN3        WRITE HEADLINE 3 OF TOTAL REPORT
         PUT   RPTDCB,TOTCOLHD       WRITE TOTAL COLUMN HEADER
         PUT   RPTDCB,TOTCOLDA       WRITE TOTAL COLUMN DASH HEADER
*
* WRITE TOTAL SALE
*
         LA    1,OTSALAMT+11  POINT REG 1 RIGHT BEFORE DEC PT
         MVC   OTSALAMT(18),XTSALAMT
         EDMK  OTSALAMT(18),PTSALAMT     OUPUT TOTAL SALE AMOUNT
         BCTR  1,0                       DECREASE REG 1 BY 1
         MVI   0(1),C'$'                 PLACE DOLLAR SIGN
*
* WRITE TOTAL SHARE
*
         MVC   OTSHRAMT(23),XTSHRAMT   MOVE PATTERN TO OUTPUT FIELD
         ED    OTSHRAMT(23),PTSHRAMT   OUPUT SHARE AMOUNT
*
* WRITE TOTAL COMMISSION
*
         LA    1,OCOMAMT+11  POINT REG 1 RIGHT BEFORE DEC PT
         MVC   OTCOMAMT(18),XTSALAMT
         EDMK  OTCOMAMT(18),PTCOMAMT     OUPUT COMMISSION AMOUNT
         BCTR  1,0                       DECREASE REG 1 BY 1
         MVI   0(1),C'$'                 PLACE DOLLAR SIGN
*
         PUT   RPTDCB,TOTSALE            WRITE TOTAL AMOUNTS
         CLOSE (HSALEDCB)                CLOSE RECORD DCB
*
* START PROCESSING HIGH SALE REPORT
*
         OPEN  (HSALEDCB,(INPUT))    OPEN HISALE DCB
         LTR   15,15                 CHECK FOR SUCCESSFUL OPEN
         BZ    OPENOK4               BRANCH IF SUCCESSFUL TO OPENOK4
         ABEND 555,DUMP              ABEND IF OPEN FAILS & DISPLAY 555
*
OPENOK4  SP    RECCTR(2),RECCTR(2)     ZERO OUT RECORD COUNTER
         SP    PTSHRAMT(9),PTSHRAMT(9) ZERO OUT SHARE AMOUNT COUNTER
         GET   HSALEDCB,HSIN         READ THE FIRST RECORD
*
         LA    3,99                  SET LINE COUNTER TO 99
         SP    PPAGECTR(2),PPAGECTR(2)  RESET PAGECTR TO ZERO
*
LOOP2    CLI   EOFFLAG2,C'Y'         CHECK FOR END OF INPUT DATA
         BE    ENDLOOP2              BRANCH TO ENDLOOP2 IF EOF
*
         AP    RECCTR(2),=P'1'       ADD 1 TO RECORD COUNTER
         MVC   OHBRKNME(25),HSIN+1   MOVE HISALE BROKER NAME TO OUTPUT
         PACK  PSHRAMT(8),HSIN+26(15)  MOVE SHARE AMOUNT TO STORAGE
         AP    PTSHRAMT(9),PSHRAMT(8)  ADD SHARE AMOUNT TO TOTAL
         MVC   OHSHRAMT(20),XSHRAMT  MOVE PATTERN TO OUTPUT FIELD
         ED    OHSHRAMT(20),PSHRAMT  OUPUT SHARE AMOUNT
*
* IF LINE COUNTER > 18 THEN INCREASE PAGECTR BY 1 AND PRINT HEADER
*
         C     3,=F'18'              COMPARE LINE COUNTER
         BC    B'0100',SKPHSHDR     IF LINECTR<18, SKIP WRITE HDR
*
         SR    3,3                   RESET LINE COUNTER TO ZERO
         AP    PPAGECTR(2),=P'1'     ADD 1 TO PAGE COUNTER
*
         MVC   OPAGECTR(4),=X'40202120'
         ED    OPAGECTR(4),PPAGECTR  EDIT PAGE COUNTER INTO OUTPUT
         PUT   RPTDCB,HEADLN1        WRITE HEADLINE 1 OF HISALE REPORT
         MVC   RPTNAME(42),HISALE    MOVE REPORT NAME TO RPTNAME FIELD
         PUT   RPTDCB,HEADLN2        WRITE HEADLINE 2 OF HISALE REPORT
         PUT   RPTDCB,HEADLN3        WRITE HEADLINE 3 OF HISALE REPORT
         PUT   RPTDCB,HSCOLHD        WRITE HIGH SALE COLUMN HEADER
         PUT   RPTDCB,HSCOLDA        WRITE HIGH SALE COLUMN DASH
*
SKPHSHDR PUT   RPTDCB,HSLINE         WRITE HISALE DETAIL LINE
         LA    3,1(,3)               ADD 1 TO LINE COUNTER
         GET   HSALEDCB,HSIN         GET THE NEXT RECORD
         B     LOOP2                 BRANCH TO TOP OF LOOP2
*
ENDLOOP2 MVC   OPAGECTR(4),=X'40202120'
         AP    PPAGECTR(2),=P'1'     ADD 1 TO RECORD COUNTER
         ED    OPAGECTR(4),PPAGECTR  EDIT PAGE COUNTER INTO OUTPUT
         PUT   RPTDCB,HEADLN1        WRITE HEADLINE 1 OF TOTAL REPORT
         MVC   RPTNAME(42),HISALETO  MOVE REPORT NAME TO RPTNAME FIELD
         PUT   RPTDCB,HEADLN2        WRITE HEADLINE 2 OF TOTAL REPORT
         PUT   RPTDCB,HEADLN3        WRITE HEADLINE 3 OF TOTAL REPORT
         PUT   RPTDCB,HTOTCHD        WRITE HIGH SALE COLUMN HEADER
         PUT   RPTDCB,HTOTCDA        WRITE HIGH SALE COLUMN DASH
*
         ED    OHRECCTR(4),RECCTR    EDIT REC COUNTER INTO TOTALS
         MVC   OHTSHR(23),XTSHRAMT   MOVE PATTERN TO OUTPUT FIELD
         ED    OHTSHR(23),PTSHRAMT   OUPUT SHARE AMOUNT
         PUT   RPTDCB,HSTOTAL        WRITE THE TOTALS LINE
*
         CLOSE (INDCB,,HSALEDCB,,RPTDCB)    CLOSE ALL DCBs
*
         SR    15,15                 SET RC TO 0
         L     13,4(,13)             R13 -> CALLER'S SAVE AREA
         L     14,12(,13)            RESTORE R14
         LM    0,12,20(13)           RESTORE R0 THROUGH R12
*
         BR    14                    RETURN TO CALLER
*
         LTORG                       LTORG TO ORGANIZE LITERALS
*
         ORG   ASSEMBL3+((*-ASSEMBL3+31)/32)*32
         DC    C'HERE IS THE STORAGE FOR ASSEMBL3'
*
REGS     DC    18F'0'                MAINREGS FOR STANDARD LINKAGE
*
         DC    PL1'0'                STORE FIRST TWO FAKE BYTES
DATETIME DS    4F                    STORAGE FOR TIME MACRO
         DC    PL1'0'                STORE FIRST TWO FAKE BYTES
PTEMPHR  DC    PL2'0'                STORAGE FOR PACKED TEMP HOUR
SETDATE  DS    CL13                  TEMPORARY STORAGE FOR FORMAT DATE
         DC    PL1'0'
SETHOUR  DS    CL8                   TEMPORARY STORAGE FOR FORMAT HOUR
SETHR2   DS    CL5                   TEMPORARY STORAGE FOR SETHR2
RECCTR   DC    PL2'0'                PACKED RECORD CTR FIELD
*
HSIN     DC    40C' '                HISALE INPUT BUFFER
*
HSOUT    DS    CL1' '
OUTPUT   DC    40C' '                STORAGE FOR HISALE BROKER NAME
*
*ISALEAMT DS    ZL11                  PACKED INTO 6 BYTES
ICOMMFLG DS    ZL1                   COMMISSION FLAG
ISHRAMT  DS    ZL15                  SHARE AMOUNT
*
PSHRPRC  DC    PL3'0'                PACKED SHARE PRICE
PPAGECTR DC    PL2'0'
PCOMPCT1 DC    PL4'0'                PACKED COMMISSION PERCENTAGE 1
PCOMPCT2 DC    PL4'0'                PACKED COMMISSION PERCENTAGE 2
PCOMPCT3 DC    PL4'0'                PACKED COMMISSION PERCENTAGE 3
PSALEAMT DC    PL6'0'                PACKED SALE AMOUNT
PTSALAMT DC    PL7'0'                PACKED TOTAL SALE AMOUNT
PCOMPCT  DC    PL4'0'                PACKED COMMISSION PERCENTAGE TEMP
PSHRAMT  DC    PL8'0'                PACKED SHARE AMOUNT
PTSHRAMT DC    PL9'0'                PACKED TOTAL SHARE AMOUNT
PCALC    DC    PL10'0'               FOR MULTIPLICATION/DIVISION
PCOMMAMT DC    PL6'0'                PACKED COMMISSION AMOUNT
PTCOMAMT DC    PL7'0'                PACKED TOTAL COMMISSION AMOUNT
* OUTPUT NUMBER FORMAT
XSALEAMT DC    X'402020206B2020206B2021204B2020' CL14
XSHRAMT  DC    X'402020206B2020206B2020206B2021204B202020' CL20
XTSHRAMT DC    X'4020206B2020206B2020206B2020206B2021204B202020' CL23
XTSALAMT DC    X'4020206B2020206B2020206B2021204B2020' CL18
*
HEADLN1  DC    C'1'                  CARRIAGE CONTROL - NEW PAGE
OUTDATE  DS    CL10                  OUTPUT DATE FIELD
         DC    38C' '                SPACES
OFUNDNME DS    CL30                  OUTPUT FIELD FOR MUTUAL FUND NAME
         DC    45C' '                SPACES
         DC    CL5'PAGE:'
OPAGECTR DC    X'40202120'           OUTPUT FIELD FOR PAGE NUMBER
*
RPT      DC    CL42'    BROKER SALES AND COMMISSIONS REPORT   '
RPTTOT   DC    CL42'BROKER SALES AND COMMISSIONS REPORT TOTALS'
HISALE   DC    CL42'        BROKER LARGE SALES REPORT         '
HISALETO DC    CL42'     BROKER LARGE SALES REPORT TOTALS     '
*
         DC    PL1'0'                STORE FIRST TWO FAKE BYTES
HEADLN2  DC    C' '                  CARRIAGE CONTROL - SINGLE SPACE
OHOUR    DS    CL5                   OUTPUT FIELD FOR HOUR
         DC    C' '                  SPACES
AMPM     DC    CL2'AM'
         DC    36C' '                SPACES
RPTNAME  DC    42C' '                STORAGE FIELD FOR REPORT NAME
         DC    46C' '                SPACES
*
HEADLN3  DC    C'0'                  CARRIAGE CONTROL - DOUBLE SPACE
         DC    55C' '                SPACES
         DC    CL13'SHARE PRICE: '
OSHRPRC  DS    CL7                   OUTPUT FIELD FOR SHARE PRICE
         DC    57C' '                SPACES
*
COLHEAD  DC    C'0'                  CARRIAGE CONTROL - DOUBLE SPACE
         DC    CL11'BRANCH NAME'
         DC    22C' '                SPACES
         DC    CL11'BROKER NAME'
         DC    25C' '                SPACES
         DC    CL12'SALES AMOUNT'
         DC    15C' '                SPACES
         DC    CL12'SHARE AMOUNT'
         DC    14C' '                SPACES
         DC    CL10'COMMISSION'
*
COLDASH  DC    C' '                  CARRIAGE CONTROL - SINGLE SPACE
         DC    25C'-'
         DC    8C' '                 SPACES
         DC    25C'-'
         DC    8C' '                 SPACES
         DC    15C'-'
         DC    8C' '                 SPACES
         DC    19C'-'
         DC    9C' '                 SPACES
         DC    15C'-'
*
DETAIL   DC    C'0'                  CARRIAGE CONTROL - DOUBLE SPACE
OBRNCNME DC    25C' '                OUTPUT FIELD FOR BRANCH NAME
         DC    8C' '                 SPACES
OBRKRNME DC    25C' '                OUTPUT FIELD FOR BROKER NAME
         DC    8C' '                 SPACES
OSALEAMT DC    15C' '                OUTPUT FIELD FOR SALE AMOUNT
         DC    6C' '                 SPACES
OSHRAMT  DS    CL20                  OUTPUT FIELD FOR SHARE AMOUNT
         DC    C'0'
         DC    9C' '                 SPACES
OCOMAMT  DC    15C' '                OUTPUT FIELD FOR COMMISSION
*
TOTCOLHD DC    C'0'                  CARRIAGE CONTROL - DOUBLE SPACE
         DC    39C' '                SPACES
         DC    CL15'NUMBER OF SALES'
         DC    9C' '                 SPACES
         DC    CL18'TOTAL SALES AMOUNT'
         DC    9C' '                 SPACES
         DC    CL18'TOTAL SHARE AMOUNT'
         DC    8C' '                 SPACES
         DC    CL16'TOTAL COMMISSION'
*
TOTCOLDA DC    C' '                  CARRIAGE CONTROL - SINGLE SPACE
         DC    39C' '                SPACES
         DC    15C'-'
         DC    9C' '                 SPACES
         DC    18C'-'
         DC    5C' '                 SPACES
         DC    22C'-'
         DC    6C' '                 SPACES
         DC    18C'-'
*
TOTSALE  DC    C'0'                  CARRIAGE CONTROL - DOUBLE SPACE
         DC    50C' '                SPACES
ORECCTR  DC    X'40202120'           OUTPUT FOR RECORD COUNT
         DC    9C' '                 SPACES
OTSALAMT DS    CL18                  OUTPUT FOR TOTAL SALE
         DC    3C' '                 SPACES
OTSHRAMT DS    CL23                  OUTPUT FOR TOTAL SHARE
         DC    C'0'
         DC    6C' '                 SPACES
OTCOMAMT DS    CL18                  OUTPUT FOR TOTAL COMMISSION
*
HSCOLHD  DC    C'0'                  CARRIAGE CONTROL - DOUBLE SPACE
         DC    40C' '                SPACES
         DC    CL22'HIGH SALES BROKER NAME'
         DC    10C' '                SPACES
         DC    CL23'HIGH SALES SHARE AMOUNT'
         DC    37C' '                SPACES
*
HSCOLDA  DC    C' '                  CARRIAGE CONTROL - SINGLE SPACE
         DC    37C' '                SPACES
         DC    25C'-'
         DC    10C' '                SPACES
         DC    23C'-'
         DC    37C' '                SPACES
*
HSLINE   DC    C'0'                  CARRIAGE CONTROL - DOUBLE SPACE
         DC    37C' '                SPACES
OHBRKNME DC    25C' '
         DC    12C' '                SPACES
OHSHRAMT DS    CL20                  SHARE AMOUNT
         DC    C'0'
         DC    37C' '                SPACES
*
HTOTCHD  DC    C'0'                  CARRIAGE CONTROL - DOUBLE SPACE
         DC    34C' '                SPACES
         DC    CL28'NUMBER OF HIGH SALES BROKERS'
         DC    4C' '                 SPACES
         DC    CL29'TOTAL HIGH SALES SHARE AMOUNT'
         DC    37C' '                SPACES
*
HTOTCDA  DC    C' '                  CARRIAGE CONTROL - SINGLE SPACE
         DC    34C' '                SPACES
         DC    28C'-'
         DC    4C' '                 SPACES
         DC    29C'-'
         DC    37C' '                SPACES
*
HSTOTAL  DC    C' '                  CARRIAGE CONTROL - SINGLE SPACE
         DC    58C' '                SPACES
OHRECCTR DC    X'40202120'           OUTPUT FOR RECORD COUNT
         DC    9C' '                 SPACES
OHTSHR   DS    CL23                  OUTPUT FOR TOTAL SHARE AMOUNT
         DC    C'0'
         DC    37C' '                SPACES
*
*  INPUT DCB
*
INDCB    DCB   DDNAME=INDATA,                                          X
               DEVD=DA,                                                X
               DSORG=PS,                                               X
               MACRF=GL,                                               X
               RECFM=FB,                                               X
               LRECL=80,                                               X
               EODAD=EOFRTN
*
*  INPUT DCB'S EOF FLAG AND EOF ROUTINE
*
EOFFLAG  DC    C'N'                 EOFFLAG FOR INPUT FILE
*
EOFRTN   MVI   EOFFLAG,C'Y'          SET EOF FLAG TO Y
         BR    14                    BRANCH TO STMT FOLLOWING GET
*
*  OUTPUT DCB
*
RPTDCB   DCB   DDNAME=RPTDATA,                                         X
               DEVD=DA,                                                X
               DSORG=PS,                                               X
               MACRF=PM,                                               X
               RECFM=FBA,                                              X
               LRECL=133,                                              X
               BLKSIZE=2660
*
*  OUTPUT/INPUT HIGH SALE DCB
*
HSALEDCB DCB   DDNAME=HSFILE,                                          X
               DEVD=DA,                                                X
               DSORG=PS,                                               X
               MACRF=(GM,PL),                                          X
               RECFM=FB,                                               X
               LRECL=41,                                               X
               EODAD=EOFRTN2
*
*  HIGH SALE DCB'S EOF FLAG AND EOF ROUTINE
*
EOFFLAG2 DC    C'N'                  EOFFLAG2 FOR HIGH SALE FILE
*
EOFRTN2  MVI   EOFFLAG2,C'Y'         SET EOF FLAG TO Y
         BR    14                    BRANCH TO STMT FOLLOWING GET
*
         END   ASSEMBL3
/*
//SYSLIN   DD DSN=&&OBJMOD,SPACE=(CYL,(1,1)),DISP=(MOD,PASS)
//*
//SYSUT1   DD SPACE=(CYL,(1,1))
//*
//SYSPRINT DD SYSOUT=*
//*
//JSTEP02  EXEC PGM=HEWL,COND=(0,LT)
//*
//***************************************************************
//*                                                             *
//*  JSTEP02 CREATES A LOAD MODULE FROM THE INPUT OBJECT        *
//*  MODULE AND STORES AS A PROGRAM OBJECT.                     *
//*                                                             *
//*  DDNAME              DESCRIPTION                            *
//*                                                             *
//*  SYSLIB     INPUT:   LOAD MODULE LIBRARIES NEEDED BY        *
//*                      THE LOADER FOR A COBOL OBJECT MODULE   *
//*  SYSLIN     INPUT:   THE OBJECT MODULE                      *
//*                                                             *
//*  RECSOUT   OUTPUT:   TO STANDARD OUPUT                      *
//*  SYSUDUMP  OUTPUT:   OUTPUT INCASE OF AN ADBEND             *
//*                                                             *
//***************************************************************
//*
//SYSLIB   DD DSN=CEE.SCEELKED,DISP=SHR
//*
//SYSLIN   DD DSN=&&OBJMOD,DISP=(OLD,DELETE)
//*
//SYSLMOD  DD DSN=KC03DB5.CSCI465.LOADLIB(ASSEMBL3),
//            SPACE=(1024,(50,20,1)),DSNTYPE=LIBRARY,
//            DISP=(MOD,KEEP,KEEP)
//*
//SYSPRINT DD SYSOUT=*
//*
//JSTEP03  EXEC PGM=ASSEMBL3,COND=(0,LT)
//*
//***************************************************************
//*                                                             *
//*  JSTEP03 FETCHES AND EXECUTES PROGRAM OBJECT                *
//*                                                             *
//*  DDNAME              DESCRIPTION                            *
//*                                                             *
//*  STEPLIB    INPUT:   LOCATION OF PROGRAM OBJECT             *
//*  INDATA     INPUT:   INPUT DATA FOR THE PROGRAM OBJECT      *
//*                                                             *
//*  RPTDATA   OUTPUT:   TO STANDARD OUPUT                      *
//*  SYSUDUMP  OUTPUT:   OUTPUT INCASE OF AN ADBEND             *
//*                                                             *
//***************************************************************
//*
//STEPLIB  DD DSN=KC03DB5.CSCI465.LOADLIB,DISP=SHR
//*
//INDATA   DD DSN=KC02322.CSCI465.DATASP22(DATA5),DISP=SHR
//*
//HSFILE   DD SPACE=(TRK,(1,1))
//*
//RPTDATA  DD SYSOUT=*
//*
//SYSUDUMP DD SYSOUT=*
//
