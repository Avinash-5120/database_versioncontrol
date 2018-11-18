#pragma once
#include<stdio.h>
#include<stdlib.h>
#include"str2.h"
#include<myDetails.h>
struct column
{
	int version;
	column *next;
	column *prevVersion;
	char *value;
	int indexOfColumn;
};

struct row
{
	column *columns;
	int rowVersion;
	int nextVer;
	char *rowId;
};

struct table
{
	int numberOfRows;
	row **rows;
	int rowsSize = 0;
	char **columnnames;
	int numberOfColumns;
	int *type;
};

struct Database
{
	int tablesInList = 0;
	table **TablesCreated;
	int sizeOfList;
	char **tableNames;
};

FILE** createFilesForTable(table *Table,char *tableName,int mode)
{
	FILE **inp;
	int i;
	char buf[50];
	inp = (FILE**)malloc(sizeof(FILE*)*(Table->numberOfColumns - 1));
	for (i = 1; i < Table->numberOfColumns; i++)
	{
		str_cpy(tableName, buf);
		str_concat(buf, "_");
		str_concat(buf, Table->columnnames[i]);
		str_concat(buf, ".db");
		if (mode == 1)
			inp[i - 1] = fopen(buf, "w");
		else if (mode == 2)
			inp[i - 1] = fopen(buf, "r");
	}
	return inp;
	//return NULL;
}

int writeDataFromRow(FILE **inp, row *Row)
{
	char buf[50];
	int index;
	column *col, *prev;
	str_cpy(Row->columns->value, buf);
	str_concat(buf, to_str(Row->rowVersion));
	col = Row->columns->next;
	while (col != NULL)
	{
		index = col->indexOfColumn;
		prev = col;
		fprintf(inp[index - 1], "%s_%d :", Row->rowId, Row->rowVersion);
		/*while (prev != NULL)
		{
			fprintf(inp[index - 1], " %d_%s", prev->version, prev->value);
			prev = prev->prevVersion;
		}*/
		fprintf(inp[index - 1], " %d_%s", prev->version, prev->value);//uncomment above and comment this one
		fprintf(inp[index - 1], "\n");
		ab_free(prev);
		col = col->next;
	}
	return 0;
}

int writeTable(FILE **inp, table *Table)
{
	int i = 0;
	for (i = 0; i <= Table->numberOfRows; i++)
	{
		writeDataFromRow(inp, Table->rows[i]);
		ab_free(Table->rows[i]);
	}
	ab_free(Table);
	return 0;
}

void closeAllFiles(FILE **inp, int num)
{
	for (int i = 0; i < num; i++)
	{
		fclose(inp[i]);
	}
}

int writeDataBase(Database *db)
{
	int i = 0;
	FILE **inp;
	for (i = 0; i <= db->tablesInList; i++)
	{
		inp = createFilesForTable(db->TablesCreated[i], db->tableNames[i],1);
		writeTable(inp, db->TablesCreated[i]);
		closeAllFiles(inp, db->TablesCreated[i]->numberOfColumns - 1);
	}
	return 0;
}

int writeIndexFile(Database *db,char *indexFilename)
{
	FILE *inf;
	int i = 0, j = 0;
	inf = fopen(indexFilename, "w");
	for (i = 0; i <= db->tablesInList; i++)
	{
		fprintf(inf, "%s ", db->tableNames[i]);
		for (j = 0; j < db->TablesCreated[i]->numberOfColumns; j++)
		{
			fprintf(inf, "%s_%d ", db->TablesCreated[i]->columnnames[j], db->TablesCreated[i]->type[j]);
		}
		fprintf(inf, "\n");
	}
	return 0;
}

//FILE** createFilesForTable(table *Table, char *tableName, int mode)
//{
//	FILE **inp;
//	int num, i, j;
//	char buf[50];
//	inp = (FILE**)malloc(sizeof(FILE*)*(Table->numberOfColumns - 1));
//	for (i = 1; i < Table->numberOfColumns; i++)
//	{
//		str_cpy(tableName, buf);
//		str_concat(buf, "_");
//		str_concat(buf, Table->columnnames[i]);
//		str_concat(buf, ".db");
//		if (mode == 1)
//			inp[i - 1] = fopen(buf, "w");
//		else if (mode == 2)
//			inp[i - 1] = fopen(buf, "r");
//	}
//	return inp;
//	//return NULL;
//}
