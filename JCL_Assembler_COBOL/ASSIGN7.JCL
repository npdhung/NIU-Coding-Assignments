//KC03DB5A JOB ,'H. Nguyen',MSGCLASS=H
//*
//*****************************************************************
//*                                                               *
//*  CSCI 465               ASSIGNMENT 7             SPRING 2022  *
//*                                                               *
//*  DEVELOPER NAME: HUNG NGUYEN                                  *
//*  DUE DATE: 05/07/2022                                         *
//*                                                               *
//*  PURPOSE: PROVIDE EXPERIENCE WORKING WITH TABLES, CALLING SUB *
//*           PROGRAMS STATIC AND DYNAMIC TO PROCESS A BUSINESS   *
//*           DAYS WORTH OF BROKER DEPOSITS AND SALES DATA AND    *
//*           CREATE A SINGLE REPORT.                             *
//*                                                               *
//*****************************************************************
//*
//JSTEP01  EXEC PGM=SORT,PARM='CMP=CLC'
//*
//*****************************************************************
//*                                                               *
//*  JSTEP01 - DFSORT                                             *
//*                                                               *
//*  JSTEP01 SORTS THE INPUT MUTUAL FUND DATA                     *
//*                                                               *
//*****************************************************************
//*
//SORTIN   DD DSN=KC02322.CSCI465.DATASP22(FUNDS),DISP=SHR
//SORTOUT  DD DSN=&&SORTED,SPACE=(TRK,(1,5)),DISP=(NEW,PASS)
//*
//SYSOUT   DD SYSOUT=*
//*
//SYSIN    DD *
  SORT FIELDS=(1,3,ZD,A)
/*
//*
//JSTEP02  EXEC PGM=IGYCRCTL,PARM=APOST,REGION=0M
//*
//*****************************************************************
//*                                                               *
//*  JSTEP01 COMPILES THE COBOL SOURCE CODE USING CURRENT         *
//*  COBOL COMPILER                                               *
//*                                                               *
//*  DDNAME              DESCRIPTION                              *
//*                                                               *
//*  STEPLIB    INPUT:   LOCATION OF THE DATA SET WHERE           *
//*                      THE COBOL COMPILER IS STORED             *
//*  SYSLIB     INPUT:   INDICATES COPY LIBRARIES                 *
//*  SYSIN      INPUT:   SOURCE CODE TO BE COMPILED               *
//*                                                               *
//*  SYSLIN    OUTPUT:   THE RESULTING OBJECT MODULE              *
//*  SYSMDECK  OUTPUT:   DATA SET CONTAINING A COPY OF THE        *
//*                      UPDATED INPUT SOURCE AFTER LIBRARY       *
//*                      PROCESSING                               *
//*  SYSUT1 - SYSUT15    DATA SET REQUIRED BY COMPILER            *
//*  SYSUDUMP  OUTPUT:   OUTPUT INCASE OF AN ADBEND               *
//*                                                               *
//*****************************************************************
//*
//*
//STEPLIB  DD DSN=IGY630.SIGYCOMP,DISP=SHR
//*
//SYSLIB   DD DSN=KC02322.CSCI465.COPYLIB,DISP=SHR
//*
//SYSIN    DD *

      *************************************************************
      *                                                           *
      * PROGRAM NAME:  SALESRPT                                   *
      *                                                           *
      * FUNCTION:  THIS PROGRAM CREATES A SINGLE REPORT BASED ON  *
      *            INFORMATION CONTAINED IN A FILE WITH AN UNKNOWN*
      *            NUMBER OF RECORDS                              *
      *                                                           *
      * INPUT:     FUNDSFLE  - THE INPUT DATA OF FUND INFO.       *
      *            SALESFLE  - THE INPUT DATA RECORDS READ IN.    *
      *                                                           *
      * OUTPUT:    RPTFLE - THE OUTPUT DATA RECORDS WRITTEN OUT.  *
      *                                                           *
      * TEMPORARY DATASET: HIGHFLE - STORES HIGH SALE RECORDS.    *
      *                                                           *
      *************************************************************

       IDENTIFICATION DIVISION.

       PROGRAM-ID.    SALESRPT.
       AUTHOR.        HUNG NGUYEN.
       DATE-WRITTEN.  05/07/2022.
       DATE-COMPILED.

       ENVIRONMENT DIVISION.

      *************************************************************
      *                                                           *
      * THIS ROUTINE DEFINES REQUIREMENTS FOR RETREIVING          *
      * AND PRINTING THE DATA.                                    *
      *                                                           *
      *************************************************************

       INPUT-OUTPUT SECTION.

       FILE-CONTROL.

           SELECT FUND-FILE ASSIGN TO FUNDSFLE.
           SELECT SALES-FILE ASSIGN TO SALESFLE.
           SELECT REPORT-FILE ASSIGN TO RPTFLE.
           SELECT HIGH-SALE-FILE ASSIGN TO HIGHFLE.

       DATA DIVISION.

       FILE SECTION.

       FD  FUND-FILE
            RECORDING MODE IS F.
            COPY FUNDREC.
            05  FILLER         PIC X(41) VALUE SPACES.
       FD  SALES-FILE
            RECORDING MODE IS F.
            COPY SALEREC.
       FD  REPORT-FILE
            RECORDING MODE IS F.
       01  PRINT-RECORD        PIC X(132).

       FD  HIGH-SALE-FILE
            RECORDING MODE F
            BLOCK CONTAINS 20 RECORDS.

       01  HIGH-SALES         PIC X(41).

       WORKING-STORAGE SECTION.

      *************************************************************
      *                                                           *
      * EOF-FLAG:          EOF FLAG                               *
      *                                                           *
      * SALE-AMT:          SALE AMOUNT                            *
      * TOT-SALE-AMT:      TOTAL DEPOSIT AMOUNT                   *
      * SHR-AMT:           SHARE AMOUNT                           *
      * TOT-SHR-AMT:       TOTAL SHARE AMOUNT                     *
      * COMM-AMT:          COMMISSION AMOUNT                      *
      *                                                           *
      * BRANCH-NME:        BRANCH NAME                            *
      * BROKER-NME:        BROKER NAME                            *
      * SHR-PRC:           SHARE PRICE                            *
      * COMM-PCT:          COMMISSION PERCENTAGE                  *
      *                                                           *
      *************************************************************

       01  FUND-TBL.
            05  FUND-TBL-CTR   PIC S9(3) BINARY SYNC.
            05  FUND-ENTRY     OCCURS 1 TO 200
                               DEPENDING ON FUND-TBL-CTR
                               ASCENDING KEY TBL-FUND-NBR
                               INDEXED BY FUND-NDX.
            COPY FUNDTBL.

      * FORMAT FOR HIGH SALE RECORD (TOTAL LENGTH 41)
       COPY HSREC.

       01  EOF-FLAG            PIC X      VALUE 'N'.

       01  ACCUMULATORS.
            05  LINE-CTR       PIC S9(2) BINARY SYNC VALUE 99.
            05  PAGE-CTR       PIC S9(2) PACKED-DECIMAL VALUE 0.
            05  TOT-BROKER     PIC S9(3) PACKED-DECIMAL VALUE 0.
            05  SALE-CTR       PIC S9(3) PACKED-DECIMAL VALUE 0.
            05  TOT-SALE-AMT   PIC S9(10)V99 PACKED-DECIMAL VALUE 0.
            05  TOT-COMM-AMT   PIC S9(10)V99 PACKED-DECIMAL VALUE 0.
            05  AVG-DEP-AMT    PIC S9(7)V99 PACKED-DECIMAL VALUE 0.
            05  TOT-SHR-AMT    PIC S9(13)V999 PACKED-DECIMAL VALUE 0.

       01  VARIABLES.
            05  SALE-AMT       PIC S9(8)V99 PACKED-DECIMAL VALUE 0.
            05  SHR-PRC        PIC S9(3)V99 PACKED-DECIMAL VALUE 0.
            05  COMM-PCT       PIC S9V9(5) PACKED-DECIMAL VALUE 0.
            05  SHR-AMT        PIC S9(10)V9(3) PACKED-DECIMAL VALUE 0.
            05  SHR-AMT2       PIC S9(10)V9(3) PACKED-DECIMAL VALUE 0.
            05  COMM-AMT       PIC S9(8)V99 PACKED-DECIMAL VALUE 0.

       01  HEADER-1.
            05  OUT-CURRENT-DATE.
               10  CURRENT-MONTH   PIC 9(2).
               10  FILLER          PIC X     VALUE '/'.
               10  CURRENT-DAY     PIC 9(2).
               10  FILLER          PIC X     VALUE '/'.
               10  CURRENT-YEAR    PIC 9(4).
            05  FILLER       PIC X(41) VALUE SPACES.
            05  FILLER       PIC X(29)
                             VALUE 'JAMES A. GARFIELD INVESTMENTS'.
            05  FILLER       PIC X(43) VALUE SPACES.
            05  FILLER       PIC X(6)  VALUE 'PAGE: '.
            05  OUT-PAGE-CTR PIC ZZ9.

       01  HEADER-2.
            05  OUT-CURRENT-TIME.
               10  CURRENT-HOUR        PIC 9(2).
               10  FILLER              PIC X VALUE ':'.
               10  CURRENT-MINUTE      PIC 9(2).
               10  FILLER              PIC X VALUE SPACE.
               10  AM-PM               PIC X(2) VALUE 'AM'.
            05  FILLER       PIC X(44) VALUE SPACES.
            05  REPORT-NME   PIC X(27) VALUE SPACES.
            05  FILLER       PIC X(53)   VALUE SPACES.

       01 COLUMNS-HD1.
             05 FILLER       PIC X(52) VALUE SPACES.
             05 FILLER       PIC X(4)  VALUE 'FUND'.
             05 FILLER       PIC X(35) VALUE SPACES.
             05 FILLER       PIC X(7)  VALUE 'DEPOSIT'.
             05 FILLER       PIC X(24) VALUE SPACES.
             05 FILLER       PIC X(10)  VALUE 'COMMISSION'.

       01 COLUMNS-HD2.
            05 FILLER        PIC X(11) VALUE 'BRANCH NAME'.
            05 FILLER        PIC X(15) VALUE SPACES.
            05 FILLER        PIC X(11) VALUE 'BROKER NAME'.
            05 FILLER        PIC X(16) VALUE SPACES.
            05 FILLER        PIC X(3)  VALUE 'NBR'.
            05 FILLER        PIC X(2)  VALUE SPACES.
            05 FILLER        PIC X(9)  VALUE 'FUND NAME'.
            05 FILLER        PIC X(25) VALUE SPACES.
            05 FILLER        PIC X(6)  VALUE 'AMOUNT'.
            05 FILLER        PIC X(8)  VALUE SPACES.
            05 FILLER        PIC X(11) VALUE 'SHARES SOLD'.
            05 FILLER        PIC X(9)  VALUE SPACES.
            05 FILLER        PIC X(6)  VALUE 'AMOUNT'.

       01 COLUMNS-HD3.
            05 FILLER        PIC X(25) VALUE ALL '-'.
            05 FILLER        PIC X     VALUE SPACE.
            05 FILLER        PIC X(25) VALUE ALL '-'.
            05 FILLER        PIC X     VALUE SPACE.
            05 FILLER        PIC X(4)  VALUE ALL '-'.
            05 FILLER        PIC X(2)  VALUE SPACES.
            05 FILLER        PIC X(25) VALUE ALL '-'.
            05 FILLER        PIC X     VALUE SPACE.
            05 FILLER        PIC X(14) VALUE ALL '-'.
            05 FILLER        PIC X     VALUE SPACES.
            05 FILLER        PIC X(18) VALUE ALL '-'.
            05 FILLER        PIC X     VALUE SPACES.
            05 FILLER        PIC X(14) VALUE ALL '-'.

       01  DETAIL-LNE.
            05  OUT-BRANCH-NME PIC X(25).
            05  FILLER         PIC X VALUE SPACES.
            05  OUT-BROKER-NME PIC X(25).
            05  FILLER         PIC X(2) VALUE SPACES.
            05  OUT-FUND-NBR   PIC 9(3).
            05  FILLER         PIC X(2) VALUE SPACES.
            05  OUT-FUND-NME   PIC X(25).
            05  FILLER         PIC X VALUE SPACES.
            05  OUT-DEP-AMT    PIC $$$,$$$,$$9.99.
            05  FILLER         PIC X(7) VALUE SPACES.
            05  OUT-SHR-AMT    PIC ZZZ,ZZ9.9999.
            05  FILLER         PIC X(5) VALUE SPACES.
            05  OUT-COMM-AMT   PIC $$$,$$9.99.

       01 TOTAL-HD1.
            05  FILLER       PIC X(46) VALUE SPACES.
            05  FILLER       PIC X(39)
                  VALUE '***** DAILY DEPOSIT REPORT TOTALS *****'.
            05  FILLER       PIC X(47) VALUE SPACES.

       01 TOTAL-HD2.
            05  FILLER       PIC X(26) VALUE SPACES.
            05  FILLER       PIC X(7)  VALUE 'BROKERS'.
            05  FILLER       PIC X(11) VALUE SPACES.
            05  FILLER       PIC X(15) VALUE 'NUMBER OF SALES'.
            05  FILLER       PIC X(25) VALUE SPACES.
            05  FILLER       PIC X(14) VALUE 'TOTAL DEPOSITS'.
            05  FILLER       PIC X(17) VALUE SPACES.
            05  FILLER       PIC X(17) VALUE 'TOTAL COMMISSIONS'.

       01 TOTAL-HD3.
            05  FILLER       PIC X(26) VALUE SPACES.
            05  FILLER       PIC X(7)  VALUE ALL '-'.
            05  FILLER       PIC X(11) VALUE SPACES.
            05  FILLER       PIC X(15) VALUE ALL '-'.
            05  FILLER       PIC X(21) VALUE SPACES.
            05  FILLER       PIC X(18) VALUE ALL '-'.
            05  FILLER       PIC X(16) VALUE SPACES.
            05  FILLER       PIC X(18) VALUE ALL '-'.

       01  TOTALS-LINE.
            05 FILLER            PIC X(27) VALUE SPACES.
            05 OUT-TOT-BROKER    PIC ZZZZZ9.
            05 FILLER            PIC X(20) VALUE SPACES.
            05 OUT-SALE-CTR      PIC ZZZZZ9.
            05 FILLER            PIC X(23) VALUE SPACES.
            05 OUT-TOT-SALE-AMT  PIC $,$$$,$$$,$$9.99.
            05 FILLER            PIC X(18) VALUE SPACES.
            05 OUT-TOT-COMM-AMT  PIC $,$$$,$$$,$$9.99.

       01 AVG-LINE.
            05 FILLER          PIC X(52) VALUE SPACES.
            05 FILLER          PIC X(27)
                               VALUE 'AVERAGE DEPOSIT BY BROKER: '.
            05 FILLER          PIC X(7) VALUE SPACES.
            05 OUT-AVG-DEP-AMT PIC $,$$$,$$9.99.
            05 FILLER          PIC X(35) VALUE SPACES.

       01 HS-COLUMNS-HD1.
             05 FILLER       PIC X(50) VALUE SPACES.
             05 FILLER       PIC X(4)  VALUE 'FUND'.
             05 FILLER       PIC X(78) VALUE SPACES.

       01 HS-COLUMNS-HD2.
            05 FILLER        PIC X(16) VALUE SPACES.
            05 FILLER        PIC X(11) VALUE 'BROKER NAME'.
            05 FILLER        PIC X(24) VALUE SPACES.
            05 FILLER        PIC X(3)  VALUE 'NBR'.
            05 FILLER        PIC X(10) VALUE SPACES.
            05 FILLER        PIC X(9)  VALUE 'FUND NAME'.
            05 FILLER        PIC X(33) VALUE SPACES.
            05 FILLER        PIC X(11) VALUE 'SHARES SOLD'.
            05 FILLER        PIC X(15) VALUE SPACES.

       01 HS-COLUMNS-HD3.
            05 FILLER        PIC X(16) VALUE SPACES.
            05 FILLER        PIC X(25) VALUE ALL '-'.
            05 FILLER        PIC X(9)  VALUE SPACES.
            05 FILLER        PIC X(4)  VALUE ALL '-'.
            05 FILLER        PIC X(10) VALUE SPACES.
            05 FILLER        PIC X(25) VALUE ALL '-'.
            05 FILLER        PIC X(10) VALUE SPACES.
            05 FILLER        PIC X(18) VALUE ALL '-'.
            05 FILLER        PIC X(15) VALUE SPACES.

       01 HS-LINE.
            05 FILLER            PIC X(16) VALUE SPACES.
            05 OUT-HS-BROKER-NME PIC X(25).
            05 FILLER            PIC X(10) VALUE SPACES.
            05 OUT-HS-FUND-NBR   PIC ZZ9.
            05 FILLER            PIC X(10) VALUE SPACES.
            05 OUT-HS-FUND-NME   PIC X(25).
            05 FILLER            PIC X(16) VALUE SPACES.
            05 OUT-HS-SHR-AMT    PIC ZZZ,ZZ9.9999.
            05 FILLER            PIC X(15) VALUE SPACES.

       01 HS-TOTAL-HD1.
            05  FILLER       PIC X(48) VALUE SPACES.
            05  FILLER       PIC X(36)
                  VALUE '***** HIGH SALES REPORT TOTALS *****'.
            05  FILLER       PIC X(48) VALUE SPACES.

       01 HS-TOT-COLUMNS-HD1.
            05 FILLER        PIC X(37) VALUE SPACES.
            05 FILLER        PIC X(17) VALUE 'HIGH SALE BROKERS'.
            05 FILLER        PIC X(14) VALUE SPACES.
            05 FILLER PIC X(27) VALUE 'TOTAL HIGH SALE SHARES SOLD'.
            05 FILLER        PIC X(37)  VALUE SPACES.

       01 HS-TOT-COLUMNS-HD2.
            05 FILLER        PIC X(37) VALUE SPACES.
            05 FILLER        PIC X(17) VALUE ALL '-'.
            05 FILLER        PIC X(14) VALUE SPACES.
            05 FILLER        PIC X(27) VALUE ALL '-'.
            05 FILLER        PIC X(37) VALUE SPACES.

       01 HS-TOT-LINE.
            05 FILLER             PIC X(48) VALUE SPACES.
            05 OUT-HS-TOT-BROKER  PIC ZZZZZ9.
            05 FILLER             PIC X(22) VALUE SPACES.
            05 OUT-HS-TOT-SHR-AMT PIC ZZ,ZZZ,ZZZ,ZZ9.9999.
            05 FILLER             PIC X(37) VALUE SPACES.

       01  CURRENT-DATE-TIME-FIELDS.
            05  CURRENT-DATE.
               10  CURRENT-YEAR             PIC 9(4).
               10  CURRENT-MONTH            PIC 9(2).
               10  CURRENT-DAY              PIC 9(2).
            05  CURRENT-TIME.
               10  CURRENT-HOUR             PIC 9(2).
               10  CURRENT-MINUTE           PIC 9(2).
               10  CURRENT-SECOND           PIC 9(2).
               10  CURRENT-MS               PIC 9(2).
            05  DIFF-FROM-GMT                PIC S9(4).

       01  PROG-NME            PIC X(8) VALUE 'TBLBUILD'.

      *************************************************************
      *                                                           *
      * THIS DIVISION CONTROLS THE FLOW OF READING IN AND         *
      * PRINTING OUT THE RESULTING PROCESSED RECORDS              *
      *                                                           *
      *************************************************************

       PROCEDURE DIVISION.

       0000-MAIN.
           PERFORM 0100-DATE-TIME.
           CALL PROG-NME USING FUND-TBL.
           OPEN INPUT SALES-FILE
                OUTPUT REPORT-FILE
                OUTPUT HIGH-SALE-FILE.

           READ SALES-FILE
                AT END MOVE 'Y' TO EOF-FLAG
           END-READ.
           PERFORM 0300-WRITE-SALES-REC
                   UNTIL EOF-FLAG = 'Y'.

           PERFORM 0400-WRITE-TOTALS.

           CLOSE SALES-FILE
                 HIGH-SALE-FILE.

           OPEN INPUT HIGH-SALE-FILE.
           MOVE 'N' TO EOF-FLAG.
           MOVE 0 TO TOT-BROKER.
           MOVE 99 TO LINE-CTR.
           MOVE 0 TO PAGE-CTR.

           READ HIGH-SALE-FILE INTO HIGH-SALE-RECORD
             AT END MOVE 'Y' TO EOF-FLAG
           END-READ.

           PERFORM 0500-WRITE-HS-RECORD
             UNTIL EOF-FLAG = 'Y'.

           PERFORM 0600-WRITE-HS-TOTALS.

           CLOSE REPORT-FILE
                 HIGH-SALE-FILE.
           GOBACK.
       0000-EXIT. EXIT.

      *************************************************************
      *                                                           *
      * GET SYSTEM DATE AND TIME AND MOVE DATE ANG TIME DATA      *
      * TO CORRESPONDING OUTPUT FIELDS.                           *
      *                                                           *
      *************************************************************

       0100-DATE-TIME.
           MOVE FUNCTION CURRENT-DATE TO CURRENT-DATE-TIME-FIELDS.

           IF CURRENT-HOUR OF CURRENT-TIME >= 12
             MOVE 'PM' TO AM-PM
             SUBTRACT 12 FROM CURRENT-HOUR OF CURRENT-TIME
           END-IF.
           IF CURRENT-HOUR OF CURRENT-TIME = 0
             ADD 12 TO CURRENT-HOUR OF CURRENT-TIME
           END-IF.

           MOVE CORRESPONDING CURRENT-DATE TO OUT-CURRENT-DATE.
           MOVE CORRESPONDING CURRENT-TIME TO OUT-CURRENT-TIME.
       0100-EXIT. EXIT.

       0200-WRITE-HEADERS.
           MOVE PAGE-CTR TO OUT-PAGE-CTR.
           WRITE PRINT-RECORD FROM HEADER-1 AFTER PAGE.
           MOVE 'BROKER DAILY DEPOSIT REPORT' TO REPORT-NME.

           WRITE PRINT-RECORD FROM HEADER-2 AFTER 1.
           WRITE PRINT-RECORD FROM COLUMNS-HD1 AFTER 2.
           WRITE PRINT-RECORD FROM COLUMNS-HD2 AFTER 1.
           WRITE PRINT-RECORD FROM COLUMNS-HD3 AFTER 1.
       0200-EXIT. EXIT.

      *************************************************************
      *                                                           *
      * PROCESSES EACH RECORD AND ADDS THE AMOUNT IN EACH         *
      * RECORD TO THE TOTAL AMOUNT. WRITES HIGH SALE              *
      * RECORDS TO A SEPARATE STORAGE.                            *
      *                                                           *
      *************************************************************

       0300-WRITE-SALES-REC.
           ADD 1 TO TOT-BROKER
           PERFORM VARYING SALE-NDX FROM 1 BY 1
             UNTIL SALE-NDX > 4 OR SALE-FUND-NBR (SALE-NDX) = 0

               IF SALE-NDX = 1
                 MOVE SALE-BRANCH-NME TO OUT-BRANCH-NME
                 MOVE SALE-BROKER-NME TO OUT-BROKER-NME
               ELSE
                 MOVE SPACES TO OUT-BRANCH-NME OUT-BROKER-NME
               END-IF

               ADD 1 TO SALE-CTR
               COMPUTE TOT-SALE-AMT ROUNDED
                = SALE-DEP-AMT(SALE-NDX) + TOT-SALE-AMT

               MOVE SALE-DEP-AMT(SALE-NDX) TO OUT-DEP-AMT
               MOVE SALE-FUND-NBR(SALE-NDX) TO OUT-FUND-NBR

               SEARCH ALL FUND-ENTRY
                 AT END
                   PERFORM 0310-FUND-NOT-FOUND
                 WHEN TBL-FUND-NBR(FUND-NDX) = SALE-FUND-NBR(SALE-NDX)
                   PERFORM 0320-FUND-FOUND
               END-SEARCH

               IF LINE-CTR > 16
                 MOVE 0 TO LINE-CTR
                 ADD 1 TO PAGE-CTR
                 PERFORM 0200-WRITE-HEADERS
               END-IF
               WRITE PRINT-RECORD FROM DETAIL-LNE AFTER 2
               ADD 1 TO LINE-CTR

           END-PERFORM.
           READ SALES-FILE
             AT END MOVE 'Y' TO EOF-FLAG
           END-READ.
       0300-EXIT. EXIT.

       0310-FUND-NOT-FOUND.
            MOVE '** FUND INFO NOT FOUND **' TO OUT-FUND-NME.
            MOVE '0' TO OUT-SHR-AMT.
            MOVE '0' TO OUT-COMM-AMT.
       0310-EXIT. EXIT.

       0320-FUND-FOUND.
           MOVE TBL-FUND-NME(FUND-NDX) TO OUT-FUND-NME.
           MOVE SALE-DEP-AMT(SALE-NDX) TO SALE-AMT.
           MOVE TBL-FUND-SHR-PRC(FUND-NDX) TO SHR-PRC.
           MOVE TBL-FUND-COMM-PCT(FUND-NDX) TO COMM-PCT.

           CALL 'COMPUTE' USING SALE-AMT SHR-PRC SHR-AMT
                                COMM-PCT COMM-AMT.
           MOVE SHR-AMT TO OUT-SHR-AMT.

      * PROCESS HIGH SALE RECORDS
           IF SHR-AMT >= 10000
             MOVE SALE-BROKER-NME TO HS-BROKER-NME
             MOVE SALE-FUND-NBR(SALE-NDX) TO HS-FUND-NBR
             MOVE SHR-AMT TO HS-SHR-AMT
             WRITE HIGH-SALES FROM HIGH-SALE-RECORD
           END-IF.

           MOVE COMM-AMT TO OUT-COMM-AMT.
           COMPUTE TOT-COMM-AMT ROUNDED = TOT-COMM-AMT + COMM-AMT.
       0320-EXIT. EXIT.

       0400-WRITE-TOTALS.
           ADD 1 TO PAGE-CTR.
           MOVE PAGE-CTR TO OUT-PAGE-CTR.
           WRITE PRINT-RECORD FROM HEADER-1 AFTER PAGE.
           MOVE 'BROKER DAILY DEPOSIT REPORT' TO REPORT-NME.
           WRITE PRINT-RECORD FROM HEADER-2 AFTER 1.

           MOVE TOT-BROKER TO OUT-TOT-BROKER.
           MOVE SALE-CTR TO OUT-SALE-CTR.
           MOVE TOT-SALE-AMT TO OUT-TOT-SALE-AMT.
           MOVE TOT-COMM-AMT TO OUT-TOT-COMM-AMT.
           WRITE PRINT-RECORD FROM TOTAL-HD1 AFTER 2.
           WRITE PRINT-RECORD FROM TOTAL-HD2 AFTER 2.
           WRITE PRINT-RECORD FROM TOTAL-HD3 AFTER 1.
           WRITE PRINT-RECORD FROM TOTALS-LINE AFTER 1.

           COMPUTE AVG-DEP-AMT ROUNDED = TOT-SALE-AMT / TOT-BROKER.
           MOVE AVG-DEP-AMT TO OUT-AVG-DEP-AMT.
           WRITE PRINT-RECORD FROM AVG-LINE AFTER 2.
       0400-EXIT. EXIT.

      *************************************************************
      *                                                           *
      * PROCESSES EACH HIGH SALE RECORD, WRITES OUT THE           *
      * HIGH SALE RECORD AND ADD TO CORRESPONDING TOTAL           *
      * FOR THE SECOND REPORT.                                    *
      *                                                           *
      *************************************************************

       0500-WRITE-HS-RECORD.
           ADD 1 TO TOT-BROKER
           MOVE HS-BROKER-NME TO OUT-HS-BROKER-NME.
           MOVE HS-FUND-NBR TO OUT-HS-FUND-NBR.

           SEARCH ALL FUND-ENTRY
             AT END
               MOVE '** FUND INFO NOT FOUND **' TO OUT-FUND-NME
             WHEN TBL-FUND-NBR(FUND-NDX) = HS-FUND-NBR
               MOVE TBL-FUND-NME(FUND-NDX) TO OUT-HS-FUND-NME
           END-SEARCH.

           COMPUTE TOT-SHR-AMT ROUNDED
                = HS-SHR-AMT + TOT-SHR-AMT
           MOVE HS-SHR-AMT TO OUT-HS-SHR-AMT.

           IF LINE-CTR > 16
             MOVE 0 TO LINE-CTR
             ADD 1 TO PAGE-CTR
             PERFORM 0510-WRITE-HS-HEADER
           END-IF.
           WRITE PRINT-RECORD FROM HS-LINE AFTER 2.
           ADD 1 TO LINE-CTR.

           READ HIGH-SALE-FILE INTO HIGH-SALE-RECORD
             AT END MOVE 'Y' TO EOF-FLAG
           END-READ.
       0500-EXIT. EXIT.

       0510-WRITE-HS-HEADER.
           MOVE PAGE-CTR TO OUT-PAGE-CTR.
           WRITE PRINT-RECORD FROM HEADER-1 AFTER PAGE.
           MOVE '     HIGH SALES REPORT     ' TO REPORT-NME.
           WRITE PRINT-RECORD FROM HEADER-2 AFTER 1.
           WRITE PRINT-RECORD FROM HS-COLUMNS-HD1 AFTER 2.
           WRITE PRINT-RECORD FROM HS-COLUMNS-HD2 AFTER 1.
           WRITE PRINT-RECORD FROM HS-COLUMNS-HD3 AFTER 1.
       0510-EXIT. EXIT.

       0600-WRITE-HS-TOTALS.
           ADD 1 TO PAGE-CTR.
           MOVE PAGE-CTR TO OUT-PAGE-CTR.
           WRITE PRINT-RECORD FROM HEADER-1 AFTER PAGE.
           MOVE '     HIGH SALES REPORT     ' TO REPORT-NME.
           WRITE PRINT-RECORD FROM HEADER-2 AFTER 1.

           WRITE PRINT-RECORD FROM HS-TOTAL-HD1 AFTER 2.
           MOVE TOT-BROKER TO OUT-HS-TOT-BROKER.
           MOVE TOT-SHR-AMT TO OUT-HS-TOT-SHR-AMT.
           WRITE PRINT-RECORD FROM HS-TOT-COLUMNS-HD1 AFTER 2.
           WRITE PRINT-RECORD FROM HS-TOT-COLUMNS-HD2 AFTER 1.
           WRITE PRINT-RECORD FROM HS-TOT-LINE AFTER 1.
       0600-EXIT. EXIT.
//*
//SYSLIN   DD DSN=&&OBJMOD1,SPACE=(CYL,(1,1)),
//            DISP=(MOD,PASS)
//*
//SYSPRINT DD SYSOUT=*
//*
//SYSUT1   DD SPACE=(CYL,(1,1))
//SYSUT2   DD SPACE=(CYL,(1,1))
//SYSUT3   DD SPACE=(CYL,(1,1))
//SYSUT4   DD SPACE=(CYL,(1,1))
//SYSUT5   DD SPACE=(CYL,(1,1))
//SYSUT6   DD SPACE=(CYL,(1,1))
//SYSUT7   DD SPACE=(CYL,(1,1))
//SYSUT8   DD SPACE=(CYL,(1,1))
//SYSUT9   DD SPACE=(CYL,(1,1))
//SYSUT10  DD SPACE=(CYL,(1,1))
//SYSUT11  DD SPACE=(CYL,(1,1))
//SYSUT12  DD SPACE=(CYL,(1,1))
//SYSUT13  DD SPACE=(CYL,(1,1))
//SYSUT14  DD SPACE=(CYL,(1,1))
//SYSUT15  DD SPACE=(CYL,(1,1))
//SYSMDECK DD SPACE=(CYL,(1,1))
//*
//*
//JSTEP03  EXEC PGM=ASMA90,PARM=ASA,COND=(0,LT)
//*
//SYSLIB   DD DSN=SYS1.MACLIB,DISP=SHR
//*
//SYSIN    DD *
*******************************************************************
*                                                                 *
* PROGRAM:    COMPUTE                                             *
* AUTHOR:     HUNG NGUYEN                                         *
* DATE:       05/07/2022                                          *
*                                                                 *
* FUNCTION:  THIS PROGRAMI CALCULATES THE COMMISION AMOUNT EARNED *
*            AND THE SHARES SOLD AMOUNTS BASED ON DEPOSIT, PRICE  *
*            OF THE SHARE, AND THE BROKER'S COMMISION PERCENTAGE. *
*                                                                 *
*******************************************************************
         PRINT  NOGEN                MACRO CALL SUPPRESSION
COMPUTE  CSECT
*
* STANDARD ENTRY LINKAGE
         STM   14,12,12(13)          SAVE CALLER'S REGS
         LR    12,15                 SET R12 TO R15
         USING COMPUTE,12            ESTABLISH R12 AS 1ST BASE REG
         LA    14,REGS               R14 -> CURRENT SAVE AREA
         ST    13,4(,14)             SAVE CALLER'S SAVE AREA ADDR
         ST    14,8(,14)             SAVE CURRENT SAVE AREA ADDR
         LR    13,14                 R13 -> CURRENT SAVE AREA
*
         LM    2,6,0(1)              LOAD VARIABLES IN R2-R6
         ZAP   PSALEAMT(6),0(6,2)    LOAD R2 -> PSALEAMT
         ZAP   PSHRPRC(3),0(3,3)     LOAD R3 -> PSHRPRC
         ZAP   PCOMPCT(4),0(4,5)     LOAD R5 -> PCOMPCT
*
* CALCULATE SHARE AMOUNT
         ZAP   PCALC(9),PSALEAMT(6) MOVE SALE AMT TO BIGGER FIELD
         SRP   PCALC(9),4,0         SHIFT LEFT SHARE AMT
         DP    PCALC(9),PSHRPRC(3)  DIVIDE TO GET SHARE AMT
         SRP   PCALC(6),64-1,5       SHIFT QUOTIENT 1 TO THE RIGHT
         ZAP   0(7,4),PCALC(6)       SET R4 TO SHARE AMT RESULT
*
* CALCULATE COMMISSION AMOUNT
         ZAP   PCALC(10),PSALEAMT(6) MOVE SALE AMT TO BIGGER FIELD
         MP    PCALC,PCOMPCT         CALCULATE COMM AMT
         SRP   PCALC,(64-7),5        SHIFT COMM AMOUNT
         ZAP   0(6,6),PCALC(10)      SET R6 TO COMM AMT RESULT
*
* STANDARD EXIT LINKAGE
         SR    15,15                 SET RC TO 0
         L     13,4(,13)             R13 -> CALLER'S SAVE AREA
         L     14,12(,13)            RESTORE R14
         LM    0,12,20(13)           RESTORE R0 THROUGH R12
         BR    14                    RETURN TO CALLER
*
REGS     DC    18F'0'                MAIN REGS FOR STANDARD LINKAGE
*
PSALEAMT DC    PL6'0'                PACKED SALE AMOUNT
PCOMPCT  DC    PL4'0'                PACKED COMMISSION PERCENTAGE
PSHRPRC  DC    PL3'0'                PACKED SHARE PRICE
PCALC    DC    PL10'0'               FOR MULTIPLICATION/DIVISION
*
         END   COMPUTE
//*
//SYSLIN   DD DSN=&&OBJMOD2,SPACE=(CYL,(1,1)),
//            DISP=(MOD,PASS,DELETE)
//*
//SYSUT1   DD SPACE=(CYL,(1,1))
//*
//SYSPRINT DD SYSOUT=*
//*
//*
//JSTEP04  EXEC PGM=HEWL,COND=(0,LT)
//*
//*****************************************************************
//*                                                               *
//* JSTEP04 - BINDER (HEWL)                                       *
//*                                                               *
//* JSTEP04 CREATES A LOAD MODULE FROM THE INPUT OBJECT           *
//* MODULE AND STORES IT AS A PROGRAM OBJECT.                     *
//*                                                               *
//* SYSLIB - THE IBM DATA SET LBRARY ROUTINES WILL BE             *
//* AUTOMATICALLY INCLUDED AS SECONDARY INPUT INTO THE            *
//* EXECUTABLE PROGRAM.                                           *
//*                                                               *
//* SYSLIN - ONE OR MORE OBJECT MODULES.                          *
//*                                                               *
//* SYSLMOD - LOAD MODULE OR PROGRAM OBJECT CREATED BY            *
//* THE BINDER. MUST BE STORED AS A PDSE MEMBER.                  *
//*                                                               *
//* SYSPRINT - MESSAGES FROM THE BINDER.                          *
//*                                                               *
//*****************************************************************
//*
//SYSLIB   DD DSN=CEE.SCEELKED,DISP=SHR
//*
//SYSLIN   DD DSN=&&OBJMOD1,DISP=(OLD,DELETE,DELETE)
//         DD DSN=&&OBJMOD2,DISP=(OLD,DELETE,DELETE)
//*
//SYSLMOD  DD DSN=KC03DB5.CSCI465.LOADLIB(SALESRPT),
//            SPACE=(1024,(50,20,1)),DSNTYPE=LIBRARY,
//            DISP=(MOD,KEEP,KEEP)
//*
//SYSPRINT DD SYSOUT=*
//*
//*
//JSTEP05  EXEC PGM=IGYCRCTL,COND=(0,LT),REGION=0M
//*
//*****************************************************************
//*                                                               *
//* JSTEP05 - COBOL COMPILER FOR TBLBUILD                         *
//*                                                               *
//*****************************************************************
//*
//STEPLIB  DD DSN=IGY630.SIGYCOMP,DISP=SHR
//*
//SYSLIB   DD DSN=KC02322.CSCI465.COPYLIB,DISP=SHR
//*
//SYSIN    DD *

       IDENTIFICATION DIVISION.
       PROGRAM-ID.    TBLBUILD.
       AUTHOR.        HUNG NGUYEN.
       DATE-WRITTEN.  05/07/2022.
       DATE-COMPILED.
       ENVIRONMENT DIVISION.

       INPUT-OUTPUT SECTION.

       FILE-CONTROL.
            SELECT FUND-FILE ASSIGN TO FUNDSFLE.

       DATA DIVISION.

       FILE SECTION.
       FD  FUND-FILE
            RECORDING MODE IS F.
            COPY FUNDREC.
            05   FILLER         PIC X(41) VALUE SPACES.

       WORKING-STORAGE SECTION.
       01  EOF-FLAG             PIC X VALUE 'N'.

       LINKAGE SECTION.
       01  FUND-TBL.
            05  FUND-TBL-CTR    PIC S9(3) BINARY SYNC.
            05  FUND-ENTRY      OCCURS 1 TO 200
                                DEPENDING ON FUND-TBL-CTR
                                ASCENDING KEY TBL-FUND-NBR
                                INDEXED BY FUND-NDX.
            COPY FUNDTBL.

       PROCEDURE DIVISION USING FUND-TBL.

       0000-MAIN.
           OPEN INPUT FUND-FILE.
           READ FUND-FILE
             AT END MOVE 'Y' TO EOF-FLAG
           END-READ.
           PERFORM 0100-BUILD-TBL
             VARYING FUND-NDX FROM 1 BY 1
             UNTIL FUND-NDX > 200 OR EOF-FLAG = 'Y'.
           CLOSE FUND-FILE.
           GOBACK.
       0000-EXIT. EXIT.

       0100-BUILD-TBL.
           ADD 1 TO FUND-TBL-CTR.
           MOVE IN-FUND-NBR TO TBL-FUND-NBR(FUND-NDX).
           MOVE IN-FUND-NME TO TBL-FUND-NME(FUND-NDX).
           MOVE IN-FUND-SHR-PRC TO TBL-FUND-SHR-PRC(FUND-NDX).
           MOVE IN-FUND-COMM-PCT TO TBL-FUND-COMM-PCT(FUND-NDX).
           READ FUND-FILE
             AT END MOVE 'Y' TO EOF-FLAG
           END-READ.
       0100-EXIT. EXIT.

//*
//SYSLIN   DD DSN=&&OBJMOD3,SPACE=(CYL,(1,1)),
//            DISP=(MOD,PASS)
//*
//SYSPRINT DD SYSOUT=*
//*
//SYSUT1   DD SPACE=(CYL,(1,1))
//SYSUT2   DD SPACE=(CYL,(1,1))
//SYSUT3   DD SPACE=(CYL,(1,1))
//SYSUT4   DD SPACE=(CYL,(1,1))
//SYSUT5   DD SPACE=(CYL,(1,1))
//SYSUT6   DD SPACE=(CYL,(1,1))
//SYSUT7   DD SPACE=(CYL,(1,1))
//SYSUT8   DD SPACE=(CYL,(1,1))
//SYSUT9   DD SPACE=(CYL,(1,1))
//SYSUT10  DD SPACE=(CYL,(1,1))
//SYSUT11  DD SPACE=(CYL,(1,1))
//SYSUT12  DD SPACE=(CYL,(1,1))
//SYSUT13  DD SPACE=(CYL,(1,1))
//SYSUT14  DD SPACE=(CYL,(1,1))
//SYSUT15  DD SPACE=(CYL,(1,1))
//SYSMDECK DD SPACE=(CYL,(1,1))
//*
//JSTEP06  EXEC PGM=HEWL,COND=(0,LT)
//*
//*****************************************************************
//*                                                               *
//* JSTEP06 - BINDER FOR TBLBUILD                                 *
//*                                                               *
//*****************************************************************
//*
//SYSLIB   DD DSN=CEE.SCEELKED,DISP=SHR
//*
//SYSLIN   DD DSN=&&OBJMOD3,DISP=(OLD,DELETE,DELETE)
//*
//SYSLMOD  DD DSN=KC03DB5.CSCI465.LOADLIB(TBLBUILD),
//            SPACE=(1024,(50,20,1)),DSNTYPE=LIBRARY,
//            DISP=(MOD,KEEP,KEEP)
//*
//SYSPRINT DD SYSOUT=*
//*
//*
//JSTEP07  EXEC PGM=SALESRPT,COND=(0,LT)
//*
//*****************************************************************
//*                                                               *
//* JSTEP07 - FETCH AND EXECUTE                                   *
//*                                                               *
//* JSTEP07 IS THE PROGRAM FETCH STEP.                            *
//*                                                               *
//* STEPLIB - DATA SET USED FOR FETCHING.                         *
//*                                                               *
//* SALESFLE - INPUT: SALE REPORT DATA.                           *
//*                                                               *
//* HIGHFLE - INPUT/OUTPUT: HIGH SALE RECORD.                     *
//*                                                               *
//* RPTFLE - OUTPUT: PRINTS OUTPUT REPORT.                        *
//*                                                               *
//*****************************************************************
//*
//STEPLIB  DD DSN=KC03DB5.CSCI465.LOADLIB,DISP=SHR
//*
//FUNDSFLE DD DSN=&&SORTED,SPACE=(TRK,(1,5)),
//            DISP=(OLD,DELETE,DELETE)
//*
//SALESFLE DD DSN=KC02322.CSCI465.DATA7(SALES),DISP=SHR
//*
//HIGHFLE  DD SPACE=(TRK,(1,1))
//*
//RPTFLE   DD SYSOUT=*
//*
//SYSUDUMP DD SYSOUT=*
//
