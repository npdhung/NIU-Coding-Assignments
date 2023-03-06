//KC03DB5A JOB ,'H. Nguyen',MSGCLASS=H
//*
//*****************************************************************
//*                                                               *
//*  CSCI 465               ASSIGNMENT 4             SPRING 2022  *
//*                                                               *
//*  DEVELOPER NAME: HUNG NGUYEN                                  *
//*  DUE DATE: 03/04/2022                                         *
//*                                                               *
//*  PURPOSE: PROVIDE EXPERIENCE WRITING A COMPLETE COBOL PROGRAM *
//*  THAT CREATES REPORTS BASED ON INPUT WITH UNKNOWN NUMBER OF   *
//*  RECORDS.                                                     *
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
      ****************************************************************

       IDENTIFICATION DIVISION.

       PROGRAM-ID.    SALESRPT.
       AUTHOR.        HUNG NGUYEN.
       DATE-WRITTEN.  02/27/2022.
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

       DATA DIVISION.

       FILE SECTION.

       FD  SALES-FILE
           RECORDING MODE F.

       01  SALES-RECORD.
           05  IN-BRANCH-NME   PIC X(25).
           05  IN-BROKER-NME   PIC X(25).
           05  FILLER          PIC X(2).
           05  IN-DEP-AMT      PIC S9(7)V99.
           05  FILLER          PIC X(19).

       FD  REPORT-FILE
           RECORDING MODE F
           BLOCK CONTAINS 20 RECORDS.

       01  REPORT-RECORD            PIC X(132).

       WORKING-STORAGE SECTION.

      *******************************************************
      * VARIABLES USED:                                     *
      *                                                     *
      * EOF-FLAG:          EOF FLAG                         *
      *                                                     *
      * DEP-AMT:           DEPOSIT AMOUNT                   *
      * TOT-DEP-AMT:       TOTAL DEPOSIT AMOUNT             *
      *                                                     *
      * BRANCH-NME:        BRANCH NAME                      *
      * BROKER-NME:        BROKER NAME                      *
      *                                                     *
      *******************************************************

       01  EOF-FLAG                 PIC X      VALUE 'N'.

       01  ACCUMULATORS.
           05  TOT-DEP-AMT          PIC S9(9)V99  PACKED-DECIMAL
                                                  VALUE 0.
       01  VARIABLES.
           05  DEP-AMT              PIC S9(7)V99  PACKED-DECIMAL.

       01  DETAIL-LINE.
           05  OUT-BRANCH-NME       PIC X(25).
           05  FILLER               PIC X(10)  VALUE SPACES.
           05  OUT-BROKER-NME       PIC X(25).
           05  FILLER               PIC X(58)  VALUE SPACES.
           05  OUT-DEP-AMT          PIC $$$,$$$,$$9.99.

       01  TOTALS-LINE.
           05  FILLER               PIC X(117)  VALUE SPACES.
           05  OUT-TOT-DEP-AMT      PIC $$$$,$$$,$$9.99.

      ******************************************************
      *                                                    *
      * THIS DIVISION CONTROLS THE FLOW OF READING IN AND  *
      * PRINTING OUT THE RESULTING PROCESSED RECORDS       *
      *                                                    *
      ******************************************************

       PROCEDURE DIVISION.

       0000-MAIN.

           OPEN INPUT  SALES-FILE
                OUTPUT REPORT-FILE.

           READ SALES-FILE
             AT END MOVE 'Y' TO EOF-FLAG
           END-READ.

           PERFORM 0100-PROCESS-RECORD
             UNTIL EOF-FLAG = 'Y'.

           PERFORM 0200-WRITE-TOTALS.

           CLOSE SALES-FILE
                 REPORT-FILE.

           GOBACK.

       0000-EXIT. EXIT.

      ******************************************************
      *                                                    *
      * PROCESSES EACH RECORD AND ADDS THE AMOUNT IN EACH  *
      * RECORD TO THE TOTAL AMOUNT                         *
      *                                                    *
      ******************************************************

       0100-PROCESS-RECORD.

           MOVE IN-BRANCH-NME TO OUT-BRANCH-NME.

           MOVE IN-BROKER-NME TO OUT-BROKER-NME.

           MOVE IN-DEP-AMT TO DEP-AMT.

           MOVE DEP-AMT TO OUT-DEP-AMT.

           COMPUTE TOT-DEP-AMT ROUNDED = TOT-DEP-AMT + DEP-AMT.

           WRITE REPORT-RECORD FROM DETAIL-LINE AFTER 2.

           READ SALES-FILE
             AT END MOVE 'Y' TO EOF-FLAG
           END-READ.

       0100-EXIT. EXIT.

       0200-WRITE-TOTALS.

           MOVE TOT-DEP-AMT TO OUT-TOT-DEP-AMT.

           WRITE REPORT-RECORD FROM TOTALS-LINE AFTER 2.

       0200-EXIT. EXIT.
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
//SALESFLE DD DSN=KC02322.CSCI465.DATASP22(DATA4),DISP=SHR
//*
//RPTFLE   DD SYSOUT=*
//*
//SYSUDUMP DD SYSOUT=*
//
