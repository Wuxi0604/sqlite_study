#ifndef SQLITE_H
#define SQLITE_H
  
#include <stdio.h>
#include "dbbe.h"

#define N_HASH        51

/*
** Forward references to structures
*/
typedef struct sqlite sqlite;

typedef struct Column Column;
typedef struct Table Table;
typedef struct Index Index;

/*
** Each database is an instance of the following structure
*/
struct sqlite {
    Dbbe*       pBe;                 /* The backend driver */
    int         flags;               /* Miscellanous flags */
    int         file_format;         /* What file format version is this database? */
    int         nTable;              /* Number of tables in the database */
    void*       pBusyArg;            /* 1st Argument to the busy callback */
    int         (*xBusyCallback)(void*, const char*, int);  /* The busy callback */
    Table*      apTblHash[N_HASH];   /* All tables of the database */
    Index*      apIdxHash[N_HASH];   /* All indices of the database */
};

/*
** Each SQL table is represented in memory by
** an instance of the following structure.
*/
struct Table {
    char* zName;     /* Name of the table */
    Table* pHash;    /* Next table with same hash on zName */
    int nCol;        /* Number of columns in this table */
    Column* aCol;    /* Information about each column */
    int readOnly;    /* True if this table should not be written by the user */
    Index* pIndex;   /* List of SQL indexes on this table. */
};

/*
** Each SQL index is represented in memory by and
** instance of the following structure.
**
** The columns of the table that are to be indexed are described
** by the aiColumn[] field of this structure.  For example, suppose
** we have the following table and index:
**
**     CREATE TABLE Ex1(c1 int, c2 int, c3 text);
**     CREATE INDEX Ex2 ON Ex1(c3,c1);
**
** In the Table structure describing Ex1, nCol==3 because there are
** three columns in the table.  In the Index structure describing
** Ex2, nColumn==2 since 2 of the 3 columns of Ex1 are indexed.
** The value of aiColumn is {2, 0}.  aiColumn[0]==2 because the
** first column to be indexed (c3) has an index of 2 in Ex1.aCol[].
** The second column to be indexed (c1) has an index of 0 in
** Ex1.aCol[], hence Ex2.aiColumn[1]==0.
*/
struct Index {
    char* zName;     /* Name of this index */
    Index* pHash;    /* Next index with the same hash on zName */
    int nColumn;     /* Number of columns in the table used by this index */
    int* aiColumn;   /* Which columns are used by this index.  1st is 0 */
    Table* pTable;   /* The SQL table being indexed */
    int isUnique;    /* True if keys must all be unique */
    Index* pNext;    /* The next index associated with the same table */
};

/*
** An pointer to an instance of this structure is passed from
** the main program to the callback.  This is used to communicate
** state and mode information.
*/
struct callback_data {
    sqlite* db;            /* The database */
    int cnt;               /* Number of records displayed so far */
    FILE* out;             /* Write results here */
    int mode;              /* An output mode setting */
    int showHeader;        /* True to show column names in List or Column mode */
    int escape;            /* Escape this character when in MODE_List */
    char zDestTable[250];  /* Name of destination table when MODE_Insert */
    char separator[20];    /* Separator character for MODE_List */
    int colWidth[100];     /* Requested width of each column when in column mode*/
    int actualWidth[100];  /* Actual width of each column */
};

#endif // !SQLITE_H

