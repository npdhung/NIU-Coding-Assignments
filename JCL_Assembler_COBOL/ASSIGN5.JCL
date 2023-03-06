//KC03DB5A JOB ,'H. Nguyen',MSGCLASS=H
//*
//*****************************************************************
//*                                                               *
//*  CSCI 465               ASSIGNMENT 5             SPRING 2022  *
//*                                                               *
//*  DEVELOPER NAME: HUNG NGUYEN                                  *
//*  DUE DATE: 04/01/2022                                         *
//*                                                               *
//*  PURPOSE: PROVIDE EXPERIENCE WRITING A COMPLETE COBOL PROGRAM *
//*  THAT CREATES REPORTS BASED ON INPUT WITH UNKNOWN NUMBER OF   *
//*  RECORDS AND USING A TEMPORARY DATASET FOR OUTPUT AND INPUT.  *
//*                                                               *
//*****************************************************************
//*
//JSTEP01  EXEC PGM=IGYCRCTL,REGION=0M,PARM=APOST
//*
//***************************************************************
//*                                                             *
//*  JSTEP01 COMPILES THE COBOL SOURCE CODE USING CURRENT       *
//*  COBOL COMPILER                                             *
//*                                                             *
//*  DDNAME              DESCRIPTION                            *
//*                                                             *
//*  STEPLIB    INPUT:   LOCATION OF THE DATA SET WHERE         *
//*                      THE COBOL COMPILER IS STORED           *
//*  SYSIN      INPUT:   SOURCE CODE TO BE COMPILED             *
//*                                                             *
//*  SYSLIN    OUTPUT:   THE RESULTING OBJECT MODULE            *
//*  SYSMDECK  OUTPUT:   DATA SET CONTAINING A COPY OF THE      *
//*                      UPDATED INPUT SOURCE AFTER LIBRARY     *
//*                      PROCESSING                             *
//*  SYSUT1 - SYSUT15    DATA SET REQUIRED BY COMPILER          *
//*  SYSUDUMP  OUTPUT:   OUTPUT INCASE OF AN ADBEND             *
//*                                                             *
//***************************************************************
//*
//STEPLIB  DD DSN=IGY630.SIGYCOMP,DISP=SHR
//*
//SYSIN    DD *

      ****************************************************************
      *                                                              *
      * PROGRAM NAME:  SALESRPT                                      *
      *                                                              *
      * FUNCTION:  THIS PROGRAM CREATES A SINGLE REPORT BASED ON     *
      *            INFORMATION CONTAINED IN A FILE WITH AN UNKNOWN   *
      *            NUMBER OF RECORDS                                 *
      *                                                              *
      * INPUT:     SALESFILE - THE INPUT DATA RECORDS READ IN.       *
      *                                                              *
      * OUTPUT:    RPTFLE - THE OUTPUT DATA RECORDS WRITTEN OUT.     *
      *                                                              *
      * TEMPORARY DATASET: HIGHFLE - STORES HIGH SALE RECORDS.       *
      *                                                              *
      ****************************************************************

       IDENTIFICATION DIVISION.

       PROGRAM-ID.    SALESRPT.
       AUTHOR.        HUNG NGUYEN.
       DATE-WRITTEN.  03/28/2022.
       DATE-COMPILED.

       ENVIRONMENT DIVISION.

      ******************************************************
      *                                                    *
      * THIS ROUTINE DEFINES REQUIREMENTS FOR RETREIVING   *
      * AND PRINTING THE DATA.                             *
      *                                                    *
      ******************************************************

       INPUT-OUTPUT SECTION.

       FILE-CONTROL.

           SELECT SALES-FILE ASSIGN TO SALESFLE.
           SELECT REPORT-FILE ASSIGN TO RPTFLE.
           SELECT HIGH-SALE-FILE ASSIGN TO HIGHFLE.

       DATA DIVISION.

       FILE SECTION.

       FD  SALES-FILE
           RECORDING MODE F.

       01  INPUT-RECORD             PIC X(80).

       FD  REPORT-FILE
           RECORDING MODE F
           BLOCK CONTAINS 20 RECORDS.

       01  REPORT-RECORD            PIC X(132).

       FD  HIGH-SALE-FILE
           RECORDING MODE F
           BLOCK CONTAINS 20 RECORDS.

       01  HIGH-SALES         PIC X(38).

       WORKING-STORAGE SECTION.

      *******************************************************
      *                                                     *
      * FUND-RECORD:       STORES FIRST RECORD IN THE FILE  *
      * SALES-RECORD:      STORES REMAINING RECORDS         *
      *                                                     *
      * EOF-FLAG:          EOF FLAG                         *
      *                                                     *
      * SALE-AMT:          SALE AMOUNT                      *
      * TOT-SALE-AMT:      TOTAL DEPOSIT AMOUNT             *
      * SHR-AMT:           SHARE AMOUNT                     *
      * TOT-SHR-AMT:       TOTAL SHARE AMOUNT               *
      * COMM-AMT:          COMMISSION AMOUNT                *
      *                                                     *
      * BRANCH-NME:        BRANCH NAME                      *
      * BROKER-NME:        BROKER NAME                      *
      * SHR-PRC:           SHARE PRICE                      *
      * COMM-PCT:          COMMISSION PERCENTAGE            *
      * COMM-FLG:          COMMISSION FLAG                  *
      *                                                     *
      *******************************************************

       01  FUND-RECORD.
           05  IN-SHR-PRC      PIC S9(3)V99.
           05  IN-FUND-NME     PIC X(35).
           05  IN-COMM-PCT-1   PIC S9(3)V999.
           05  IN-COMM-PCT-2   PIC S9(3)V999.
           05  IN-COMM-PCT-3   PIC S9(3)V999.
           05  FILLER          PIC X(22).

       01  SALES-RECORD.
           05  IN-BRANCH-NME   PIC X(25).
           05  IN-BROKER-NME   PIC X(25).
           05  IN-SALE-AMT     PIC S9(9)V99.
           05  FILLER          PIC X(3).
           05  IN-COMM-FLG     PIC S9(1).
           05  FILLER          PIC X(15).

       01  HIGH-SALES-IO.
           05  HS-BROKER-NME   PIC X(25).
           05  HS-SHR-AMT      PIC S9(10)V999 PACKED-DECIMAL VALUE 0.

       01  EOF-FLAG            PIC X      VALUE 'N'.

       01  ACCUMULATORS.
           05  SALE-CTR        PIC S9(3) PACKED-DECIMAL VALUE 0.
           05  LINE-CTR        PIC S9(2) BINARY SYNC VALUE 99.
           05  PAGE-CTR        PIC S9(2) PACKED-DECIMAL VALUE 0.
           05  TOT-SALE-AMT    PIC S9(10)V99 PACKED-DECIMAL VALUE 0.
           05  TOT-SHR-AMT     PIC S9(13)V999 PACKED-DECIMAL VALUE 0.
           05  TOT-COMM-AMT    PIC S9(10)V99 PACKED-DECIMAL VALUE 0.

       01  VARIABLES.
           05  SHR-PRC         PIC S9(3)V99 PACKED-DECIMAL VALUE 0.
           05  SALE-AMT        PIC S9(8)V99 PACKED-DECIMAL VALUE 0.
           05  SHR-AMT         PIC S9(10)V999 PACKED-DECIMAL VALUE 0.
           05  COMM-PCT        PIC S9V9(5) PACKED-DECIMAL VALUE 0.
           05  COMM-AMT        PIC S9(8)V99 PACKED-DECIMAL VALUE 0.

       01  HEADLINE-1.
           05  HDLN-1-CURRENT-DATE.
               10  CURRENT-MONTH       PIC 9(2).
               10  FILLER              PIC X VALUE '/'.
               10  CURRENT-DAY         PIC 9(2).
               10  FILLER              PIC X VALUE '/'.
               10  CURRENT-YEAR        PIC 9(4).
           05  FILLER                  PIC X(38) VALUE SPACES.
           05  OUT-FUND-NME            PIC X(35).
           05  FILLER                  PIC X(40) VALUE SPACES.
           05  FILLER                  PIC X(6)  VALUE 'PAGE: '.
           05  OUT-PAGE-CTR            PIC ZZ9.

       01  HEADLINE-2.
           05  HDLN-2-CURRENT-TIME.
               10  CURRENT-HOUR        PIC 9(2).
               10  FILLER              PIC X VALUE ':'.
               10  CURRENT-MINUTE      PIC 9(2).
               10  FILLER              PIC X VALUE SPACE.
               10  AM-PM               PIC X(2) VALUE 'AM'.
           05  FILLER                  PIC X(40) VALUE SPACES.
           05  FILLER                  PIC X(35)
               VALUE 'BROKER SALES AND COMMISSIONS REPORT'.
           05  FILLER                  PIC X(49) VALUE SPACES.

       01  HEADLINE-2-TOT.
           05  HDLN-2-TOT-CURRENT-TIME.
               10  CURRENT-HOUR        PIC 9(2).
               10  FILLER              PIC X VALUE ':'.
               10  CURRENT-MINUTE      PIC 9(2).
               10  FILLER              PIC X VALUE SPACE.
               10  AM-PM               PIC X(2) VALUE 'AM'.
           05  FILLER                  PIC X(36) VALUE SPACES.
           05  FILLER                  PIC X(42)
               VALUE 'BROKER SALES AND COMMISSIONS REPORT TOTALS'.
           05  FILLER                  PIC X(46) VALUE SPACES.

       01  HEADLINE-2-HS.
           05  HDLN-2-HS-CURRENT-TIME.
               10  CURRENT-HOUR        PIC 9(2).
               10  FILLER              PIC X VALUE ':'.
               10  CURRENT-MINUTE      PIC 9(2).
               10  FILLER              PIC X VALUE SPACE.
               10  AM-PM               PIC X(2) VALUE 'AM'.
           05  FILLER                  PIC X(44) VALUE SPACES.
           05  FILLER                  PIC X(25)
               VALUE 'BROKER LARGE SALES REPORT'.
           05  FILLER                  PIC X(55) VALUE SPACES.

       01  HEADLINE-2-TOT-HS.
           05  HDLN-2-TOT-HS-CURRENT-TIME.
               10  CURRENT-HOUR        PIC 9(2).
               10  FILLER              PIC X VALUE ':'.
               10  CURRENT-MINUTE      PIC 9(2).
               10  FILLER              PIC X VALUE SPACE.
               10  AM-PM               PIC X(2) VALUE 'AM'.
           05  FILLER                  PIC X(41) VALUE SPACES.
           05  FILLER                  PIC X(32)
               VALUE 'BROKER LARGE SALES REPORT TOTALS'.
           05  FILLER                  PIC X(51) VALUE SPACES.

       01  HEADLINE-3.
           05  FILLER          PIC X(55) VALUE SPACES.
           05  FILLER          PIC X(13) VALUE 'SHARE PRICE: '.
           05  OUT-SHR-PRC     PIC $$$9.99.
           05  FILLER          PIC X(57) VALUE SPACES.

       01  COL-HEADER.
           05  FILLER      PIC X(11) VALUE 'BRANCH NAME'.
           05  FILLER      PIC X(22) VALUE SPACES.
           05  FILLER      PIC X(11) VALUE 'BROKER NAME'.
           05  FILLER      PIC X(25) VALUE SPACES.
           05  FILLER      PIC X(12) VALUE 'SALES AMOUNT'.
           05  FILLER      PIC X(15) VALUE SPACES.
           05  FILLER      PIC X(12) VALUE 'SHARE AMOUNT'.
           05  FILLER      PIC X(14) VALUE SPACES.
           05  FILLER      PIC X(10) VALUE 'COMMISSION'.

       01  COL-DASH.
           05  FILLER      PIC X(25) VALUE ALL '-'.
           05  FILLER      PIC X(8)  VALUE SPACES.
           05  FILLER      PIC X(25) VALUE ALL '-'.
           05  FILLER      PIC X(8)  VALUE SPACES.
           05  FILLER      PIC X(15) VALUE ALL '-'.
           05  FILLER      PIC X(8)  VALUE SPACES.
           05  FILLER      PIC X(19) VALUE ALL '-'.
           05  FILLER      PIC X(9)  VALUE SPACES.
           05  FILLER      PIC X(15) VALUE ALL '-'.

       01  DETAIL-LINE.
           05  OUT-BRANCH-NME       PIC X(25).
           05  FILLER               PIC X(8) VALUE SPACES.
           05  OUT-BROKER-NME       PIC X(25).
           05  FILLER               PIC X(8) VALUE SPACES.
           05  OUT-SALE-AMT         PIC $$$$,$$$,$$9.99.
           05  FILLER               PIC X(8) VALUE SPACES.
           05  OUT-SHR-AMT          PIC ZZ,ZZZ,ZZZ,ZZ9.9999.
           05  FILLER               PIC X(9) VALUE SPACES.
           05  OUT-COMM-AMT         PIC $$$$,$$$,$$9.99.

       01  TOT-COL-HEADER.
           05  FILLER      PIC X(39) VALUE SPACES.
           05  FILLER      PIC X(15) VALUE 'NUMBER OF SALES'.
           05  FILLER      PIC X(9)  VALUE SPACES.
           05  FILLER      PIC X(18) VALUE 'TOTAL SALES AMOUNT'.
           05  FILLER      PIC X(9)  VALUE SPACES.
           05  FILLER      PIC X(18) VALUE 'TOTAL SHARE AMOUNT'.
           05  FILLER      PIC X(8)  VALUE SPACES.
           05  FILLER      PIC X(16) VALUE 'TOTAL COMMISSION'.

       01  TOT-COL-DASH.
           05  FILLER      PIC X(39) VALUE SPACES.
           05  FILLER      PIC X(15) VALUE ALL '-'.
           05  FILLER      PIC X(9)  VALUE SPACES.
           05  FILLER      PIC X(18) VALUE ALL '-'.
           05  FILLER      PIC X(5)  VALUE SPACES.
           05  FILLER      PIC X(22) VALUE ALL '-'.
           05  FILLER      PIC X(6)  VALUE SPACES.
           05  FILLER      PIC X(18) VALUE ALL '-'.

       01  TOTAL-LINE.
           05  FILLER               PIC X(51) VALUE SPACES.
           05  OUT-SALE-CTR         PIC ZZ9.
           05  FILLER               PIC X(12) VALUE SPACES.
           05  OUT-TOT-SALE-AMT     PIC $$$$,$$$,$$9.99.
           05  FILLER               PIC X(8)  VALUE SPACES.
           05  OUT-TOT-SHR-AMT      PIC ZZ,ZZZ,ZZZ,ZZ9.9999.
           05  FILLER               PIC X(13) VALUE SPACES.
           05  OUT-TOT-COMM-AMT     PIC $$$$,$$9.99.

       01  HS-COL-HEADER.
           05  FILLER      PIC X(40) VALUE SPACES.
           05  FILLER      PIC X(22) VALUE 'HIGH SALES BROKER NAME'.
           05  FILLER      PIC X(10) VALUE SPACES.
           05  FILLER      PIC X(23) VALUE 'HIGH SALES SHARE AMOUNT'.
           05  FILLER      PIC X(37) VALUE SPACES.

       01  HS-COL-DASH.
           05  FILLER      PIC X(37) VALUE SPACES.
           05  FILLER      PIC X(25) VALUE ALL '-'.
           05  FILLER      PIC X(10) VALUE SPACES.
           05  FILLER      PIC X(23) VALUE ALL '-'.
           05  FILLER      PIC X(37) VALUE SPACES.

       01  HIGH-SALES-REPORT.
           05  FILLER               PIC X(37) VALUE SPACES.
           05  OUT-HS-BROKER-NME    PIC X(25).
           05  FILLER               PIC X(14) VALUE SPACES.
           05  OUT-HS-SHR-AMT       PIC ZZ,ZZZ,ZZZ,ZZ9.9999.
           05  FILLER               PIC X(37) VALUE SPACES.

       01  HS-TOT-COL-HEADER.
           05  FILLER  PIC X(34) VALUE SPACES.
           05  FILLER  PIC X(28) VALUE 'NUMBER OF HIGH SALES BROKERS'.
           05  FILLER  PIC X(4)  VALUE SPACES.
           05  FILLER  PIC X(29) VALUE 'TOTAL HIGH SALES SHARE AMOUNT'.
           05  FILLER  PIC X(37)  VALUE SPACES.

       01  HS-TOT-COL-DASH.
           05  FILLER  PIC X(34) VALUE SPACES.
           05  FILLER  PIC X(28) VALUE ALL '-'.
           05  FILLER  PIC X(4)  VALUE SPACES.
           05  FILLER  PIC X(29) VALUE ALL '-'.
           05  FILLER  PIC X(37) VALUE SPACES.

       01  HS-TOTAL-LINE.
           05  FILLER               PIC X(59) VALUE SPACES.
           05  OUT-HS-SALE-CTR      PIC ZZ9.
           05  FILLER               PIC X(14) VALUE SPACES.
           05  OUT-HS-TOT-SHR-AMT   PIC ZZ,ZZZ,ZZZ,ZZ9.9999.
           05  FILLER               PIC X(37) VALUE SPACES.

       01  CURRENT-DATE-FIELDS.
           05  CURRENT-DATE.
               10  CURRENT-YEAR               PIC 9(4).
               10  CURRENT-MONTH              PIC 9(2).
               10  CURRENT-DAY                PIC 9(2).
           05  CURRENT-TIME.
               10  CURRENT-HOUR               PIC 9(2).
               10  CURRENT-MINUTE             PIC 9(2).
               10  CURRENT-SECOND             PIC 9(2).
               10  CURRENT-MS                 PIC 9(2).
           05  DIFF-FROM-GMT                  PIC S9(4).

      ******************************************************
      *                                                    *
      * THIS DIVISION CONTROLS THE FLOW OF READING IN AND  *
      * PRINTING OUT THE RESULTING PROCESSED RECORDS       *
      *                                                    *
      ******************************************************

       PROCEDURE DIVISION.

       0000-MAIN.

           PERFORM 0100-DATE-TIME.

           OPEN INPUT  SALES-FILE
                OUTPUT REPORT-FILE
                OUTPUT HIGH-SALE-FILE.

           READ SALES-FILE INTO FUND-RECORD.
           MOVE IN-SHR-PRC TO SHR-PRC.
           MOVE SHR-PRC TO OUT-SHR-PRC.
           MOVE IN-FUND-NME TO OUT-FUND-NME.

           READ SALES-FILE INTO SALES-RECORD
             AT END MOVE 'Y' TO EOF-FLAG
           END-READ.

           PERFORM 0200-PROCESS-RECORD
             UNTIL EOF-FLAG = 'Y'.

           PERFORM 0400-PRINT-TOTAL.

           CLOSE SALES-FILE
                 HIGH-SALE-FILE.

           OPEN INPUT HIGH-SALE-FILE.

           MOVE 'N' TO EOF-FLAG.
           MOVE 0 TO SALE-CTR.
           MOVE 0 TO TOT-SHR-AMT.
           MOVE 99 TO LINE-CTR.
           MOVE 0 TO PAGE-CTR.

           READ HIGH-SALE-FILE INTO HIGH-SALES-IO
             AT END MOVE 'Y' TO EOF-FLAG
           END-READ.

           PERFORM 0500-PROCESS-HS-RECORD
             UNTIL EOF-FLAG = 'Y'.

           PERFORM 0600-PRINT-HS-TOTAL.

           CLOSE HIGH-SALE-FILE
                 REPORT-FILE.

           GOBACK.

       0000-EXIT. EXIT.

      ******************************************************
      *                                                    *
      * GET SYSTEM DATE AND TIME AND MOVE DATE ANG TIME    *
      * DATA TO CORRESPONDING OUTPUT FIELDS.
      *                                                    *
      ******************************************************

       0100-DATE-TIME.

           MOVE FUNCTION CURRENT-DATE TO CURRENT-DATE-FIELDS.

           IF CURRENT-HOUR OF CURRENT-TIME >= 12
             MOVE 'PM' TO AM-PM OF HDLN-2-CURRENT-TIME
             MOVE 'PM' TO AM-PM OF HDLN-2-TOT-CURRENT-TIME
             MOVE 'PM' TO AM-PM OF HDLN-2-HS-CURRENT-TIME
             MOVE 'PM' TO AM-PM OF HDLN-2-TOT-HS-CURRENT-TIME
             SUBTRACT 12 FROM CURRENT-HOUR OF CURRENT-TIME
           END-IF.
           IF CURRENT-HOUR OF CURRENT-TIME = 0
             ADD 12 TO CURRENT-HOUR OF CURRENT-TIME
           END-IF.

           MOVE CORRESPONDING CURRENT-DATE TO HDLN-1-CURRENT-DATE.
           MOVE CORRESPONDING CURRENT-TIME TO HDLN-2-CURRENT-TIME.
           MOVE CORRESPONDING CURRENT-TIME TO HDLN-2-TOT-CURRENT-TIME.
           MOVE CORRESPONDING CURRENT-TIME TO HDLN-2-HS-CURRENT-TIME.
           MOVE CORRESPONDING CURRENT-TIME TO
             HDLN-2-TOT-HS-CURRENT-TIME.

       0100-EXIT. EXIT.

      ******************************************************
      *                                                    *
      * PROCESSES EACH RECORD AND ADDS THE AMOUNT IN EACH  *
      * RECORD TO THE TOTAL AMOUNT. WRITES HIGH SALE       *
      * RECORDS TO A SEPARATE STORAGE.                     *
      *                                                    *
      ******************************************************

       0200-PROCESS-RECORD.

           MOVE IN-BRANCH-NME TO OUT-BRANCH-NME.
           MOVE IN-BROKER-NME TO OUT-BROKER-NME.
           MOVE IN-SALE-AMT TO SALE-AMT.
           MOVE SALE-AMT TO OUT-SALE-AMT.

           COMPUTE SHR-AMT ROUNDED = SALE-AMT / SHR-PRC.
           MOVE SHR-AMT TO OUT-SHR-AMT.

           EVALUATE IN-COMM-FLG
             WHEN 1 MOVE IN-COMM-PCT-1 TO COMM-PCT
             WHEN 2 MOVE IN-COMM-PCT-2 TO COMM-PCT
             WHEN 3 MOVE IN-COMM-PCT-3 TO COMM-PCT
           END-EVALUATE.

           COMPUTE COMM-AMT ROUNDED = COMM-PCT * SALE-AMT / 100.
           MOVE COMM-AMT TO OUT-COMM-AMT.

           ADD 1 TO SALE-CTR.
           COMPUTE TOT-SALE-AMT ROUNDED = TOT-SALE-AMT + SALE-AMT.
           COMPUTE TOT-SHR-AMT = TOT-SHR-AMT + SHR-AMT.
           COMPUTE TOT-COMM-AMT = TOT-COMM-AMT + COMM-AMT.

           IF LINE-CTR > 17
             MOVE 0 TO LINE-CTR
             ADD 1 TO PAGE-CTR
             PERFORM 0300-PRINT-HEADER
           END-IF.
           WRITE REPORT-RECORD FROM DETAIL-LINE AFTER 2.
           ADD 1 TO LINE-CTR.

           IF SHR-AMT >= 150000
             MOVE IN-BROKER-NME TO HS-BROKER-NME
             MOVE SHR-AMT TO HS-SHR-AMT
             WRITE HIGH-SALES FROM HIGH-SALES-IO
           END-IF.

           READ SALES-FILE INTO SALES-RECORD
             AT END MOVE 'Y' TO EOF-FLAG
           END-READ.
       0200-EXIT. EXIT.

      ******************************************************
      *                                                    *
      * PRINTS HEADER IN EACH PAGE OF THE FIRST REPORT     *
      *                                                    *
      ******************************************************

       0300-PRINT-HEADER.
           MOVE PAGE-CTR TO OUT-PAGE-CTR.
           WRITE REPORT-RECORD FROM HEADLINE-1 AFTER PAGE.
           WRITE REPORT-RECORD FROM HEADLINE-2 AFTER 1.
           WRITE REPORT-RECORD FROM HEADLINE-3 AFTER 2.
           WRITE REPORT-RECORD FROM COL-HEADER AFTER 2.
           WRITE REPORT-RECORD FROM COL-DASH AFTER 1.
       0300-EXIT. EXIT.

      ******************************************************
      *                                                    *
      * PRINTS HEADER IN EACH PAGE FOR THE HIGH SALES OF   *
      * THE SECOND REPORT                                  *
      *                                                    *
      ******************************************************

       0310-PRINT-HS-HEADER.
           MOVE PAGE-CTR TO OUT-PAGE-CTR.
           WRITE REPORT-RECORD FROM HEADLINE-1 AFTER PAGE.
           WRITE REPORT-RECORD FROM HEADLINE-2-HS AFTER 1.
           WRITE REPORT-RECORD FROM HEADLINE-3 AFTER 2.
           WRITE REPORT-RECORD FROM HS-COL-HEADER AFTER 2.
           WRITE REPORT-RECORD FROM HS-COL-DASH AFTER 1.
       0310-EXIT. EXIT.

      ******************************************************
      *                                                    *
      * PRINTS OUT TOTAL NUMBER OF SALES, SALES AMOUNT,    *
      * TOTAL SHARE AMOUNT, AND TOTAL COMMISSION OF THE    *
      * SECOND REPORT.                                     *
      *                                                    *
      ******************************************************

       0400-PRINT-TOTAL.
           ADD 1 TO PAGE-CTR.
           MOVE PAGE-CTR TO OUT-PAGE-CTR.
           WRITE REPORT-RECORD FROM HEADLINE-1 AFTER PAGE.
           WRITE REPORT-RECORD FROM HEADLINE-2-TOT AFTER 1.
           WRITE REPORT-RECORD FROM HEADLINE-3 AFTER 2.

           MOVE SALE-CTR TO OUT-SALE-CTR.
           MOVE TOT-SALE-AMT TO OUT-TOT-SALE-AMT.
           MOVE TOT-SHR-AMT TO OUT-TOT-SHR-AMT.
           MOVE TOT-COMM-AMT TO OUT-TOT-COMM-AMT.

           WRITE REPORT-RECORD FROM TOT-COL-HEADER AFTER PAGE.
           WRITE REPORT-RECORD FROM TOT-COL-DASH AFTER 1.
           WRITE REPORT-RECORD FROM TOTAL-LINE AFTER 2.
       0400-EXIT. EXIT.

      ******************************************************
      *                                                    *
      * PROCESSES EACH HIGH SALE RECORD, WRITES OUT THE    *
      * HIGH SALE RECORD AND ADD TO CORRESPONDING TOTAL    *
      * FOR THE SECOND REPORT.                             *
      *                                                    *
      ******************************************************

       0500-PROCESS-HS-RECORD.
           MOVE HS-BROKER-NME TO OUT-HS-BROKER-NME.
           MOVE HS-SHR-AMT TO OUT-HS-SHR-AMT.
           ADD 1 TO SALE-CTR.
           COMPUTE TOT-SHR-AMT ROUNDED = TOT-SHR-AMT + HS-SHR-AMT.

           IF LINE-CTR > 17
             MOVE 0 TO LINE-CTR
             ADD 1 TO PAGE-CTR
             PERFORM 0310-PRINT-HS-HEADER
           END-IF.
           WRITE REPORT-RECORD FROM HIGH-SALES-REPORT AFTER 2.
           ADD 1 TO LINE-CTR.

           READ HIGH-SALE-FILE INTO HIGH-SALES-IO
             AT END MOVE 'Y' TO EOF-FLAG
           END-READ.
       0500-EXIT. EXIT.

      ******************************************************
      *                                                    *
      * PRINTS OUT TOTAL NUMBER OF HIGH SALES BROKERS AND  *
      * TOTAL HIGH SALES SHARE AMOUNT OF THE SECOND        *
      * REPORT.                                            *
      *                                                    *
      ******************************************************

       0600-PRINT-HS-TOTAL.
           ADD 1 TO PAGE-CTR.
           MOVE PAGE-CTR TO OUT-PAGE-CTR.
           WRITE REPORT-RECORD FROM HEADLINE-1 AFTER PAGE.
           WRITE REPORT-RECORD FROM HEADLINE-2-TOT-HS AFTER 1.
           WRITE REPORT-RECORD FROM HEADLINE-3 AFTER 2.

           WRITE REPORT-RECORD FROM HS-TOT-COL-HEADER AFTER 2.
           WRITE REPORT-RECORD FROM HS-TOT-COL-DASH AFTER 2.
           MOVE SALE-CTR TO OUT-HS-SALE-CTR.
           MOVE TOT-SHR-AMT TO OUT-HS-TOT-SHR-AMT.
           WRITE REPORT-RECORD FROM HS-TOTAL-LINE AFTER 2.
       0600-EXIT. EXIT.

/*
//SYSLIN   DD DSN=&&OBJMOD,SPACE=(CYL,(1,1)),DISP=(MOD,PASS)
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
//SYSLMOD  DD DSN=KC03DB5.CSCI465.LOADLIB(SALESRPT),
//            SPACE=(1024,(50,20,1)),DSNTYPE=LIBRARY,
//            DISP=(MOD,KEEP,KEEP)
//*
//SYSPRINT DD SYSOUT=*
//*
//JSTEP03  EXEC PGM=SALESRPT,COND=(0,LT)
//*
//***************************************************************
//*                                                             *
//*  JSTEP03 FETCHES AND EXECUTES PROGRAM OBJECT                *
//*                                                             *
//*  DDNAME              DESCRIPTION                            *
//*                                                             *
//*  STEPLIB    INPUT:   LOCATION OF PROGRAM OBJECT             *
//*  SALESFLE   INPUT:   INPUT DATA FOR THE PROGRAM OBJECT      *
//*                                                             *
//*  RPTFLE    OUTPUT:   TO STANDARD OUPUT                      *
//*  SYSUDUMP  OUTPUT:   OUTPUT INCASE OF AN ADBEND             *
//*                                                             *
//***************************************************************
//*
//STEPLIB  DD DSN=KC03DB5.CSCI465.LOADLIB,DISP=SHR
//*
//SALESFLE DD DSN=KC02322.CSCI465.DATASP22(DATA5),DISP=SHR
//*
//HIGHFLE  DD SPACE=(TRK,(1,1))
//*
//RPTFLE   DD SYSOUT=*
//*
//SYSUDUMP DD SYSOUT=*
//
