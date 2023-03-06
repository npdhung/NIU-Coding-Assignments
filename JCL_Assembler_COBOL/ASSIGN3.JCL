//KC03DB5A JOB ,'H. Nguyen',MSGCLASS=H
//*
//*****************************************************************
//*                                                               *
//*  CSCI 465               ASSIGNMENT 3             SPRING 2022  *
//*                                                               *
//*  DEVELOPER NAME: HUNG NGUYEN                                  *
//*  DUE DATE: 02/11/2022                                         *
//*                                                               *
//*  PURPOSE: PROVIDE EXPERIENCE WRITING AND RUNNING BASIC JCL    *
//*  JOBS ON THE MARIST MAINFRAME.                                *
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
      * PROGRAM NAME:  COBOL3                                        *
      *                                                              *
      * FUNCTION:  THIS PROGRAM READS INPUT DATA RECORDS AND THEN    *
      *            WRITES THEM TO STANDARD OUTPUT.                   *
      *                                                              *
      * INPUT:     INDATA - THE INPUT DATA RECORDS READ IN.          *
      *                                                              *
      * OUTPUT:    REPORT - THE INPUT DATA RECORDS WRITTEN OUT.      *
      *                                                              *
      ****************************************************************

       IDENTIFICATION DIVISION.

       PROGRAM-ID.    COBOL3.
       AUTHOR.        GEOFFREY D DECKER.
       DATE-WRITTEN.  09/12/2021.
       DATE-COMPILED.

       ENVIRONMENT DIVISION.

       INPUT-OUTPUT SECTION.

       FILE-CONTROL.

           SELECT INPUT-FILE ASSIGN TO INDATA.
           SELECT REPORT-FILE ASSIGN TO RPTDATA.

       DATA DIVISION.

       FILE SECTION.

       FD  INPUT-FILE
           RECORDING MODE F.

       01  INPUT-RECORD             PIC X(80).

       FD  REPORT-FILE
           RECORDING MODE F
           BLOCK CONTAINS 20 RECORDS.

       01  REPORT-RECORD            PIC X(132).

       WORKING-STORAGE SECTION.

       01  EOFFLAG                  PIC X      VALUE 'N'.

       01  RECORD-CTR               PIC S9(3)  BINARY SYNC VALUE 0.

       01  DETAIL-LINE.
           05  OUTPUT-RECORD        PIC X(80).
           05  FILLER               PIC X(52)  VALUE SPACES.

       01  TOTALS-LINE.
           05  FILLER               PIC X(14)  VALUE 'RECORD COUNT: '.
           05  OUT-RECORD-CTR       PIC ZZ9.
           05  FILLER               PIC X(115) VALUE SPACES.

       PROCEDURE DIVISION.

       0000-MAIN.

           OPEN INPUT  INPUT-FILE
                OUTPUT REPORT-FILE.

           READ INPUT-FILE
             AT END MOVE 'Y' TO EOFFLAG
           END-READ.

           PERFORM 0100-PROCESS-RECORD
             UNTIL EOFFLAG = 'Y'.

           PERFORM 0200-WRITE-TOTALS.

           CLOSE INPUT-FILE
                 REPORT-FILE.

           GOBACK.

       0000-EXIT. EXIT.

       0100-PROCESS-RECORD.

           ADD 1 TO RECORD-CTR.

           MOVE INPUT-RECORD TO OUTPUT-RECORD.

           WRITE REPORT-RECORD FROM DETAIL-LINE AFTER 2.

           READ INPUT-FILE
             AT END MOVE 'Y' TO EOFFLAG
           END-READ.

       0100-EXIT. EXIT.

       0200-WRITE-TOTALS.

           MOVE RECORD-CTR TO OUT-RECORD-CTR.

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
//SYSLMOD  DD DSN=KC03DB5.CSCI465.LOADLIB(COBOL3),
//            SPACE=(1024,(50,20,1)),DSNTYPE=LIBRARY,
//            DISP=(MOD,KEEP,KEEP)
//*
//SYSPRINT DD SYSOUT=*
//*
//JSTEP03  EXEC PGM=COBOL3,COND=(0,LT)
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
//INDATA   DD DSN=KC02322.CSCI465.DATASP22(DATA3),DISP=SHR
//*
//RPTDATA  DD SYSOUT=*
//*
//SYSUDUMP DD SYSOUT=*
//
