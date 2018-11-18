#include<stdio.h>
#include<stdlib.h>
#include"Custom_strngs.h"

struct column
{
	int version;
	column *next;
	column *prevVersion;
	char value[30];
};

struct row
{
	column *columns;
	int rowVersion;
	int nextVer;
};

struct table
{
	int numberOfRows;
	row **rows;
	int rowsSize = 0;
	char **columnnames;
	int numberOfColumns;
};

column* createColumn(int version,char *value)
{
	column *newCol;
	newCol = (column*)malloc(sizeof(column));
	newCol->prevVersion = NULL;
	newCol->next = NULL;
	str_cpy(value, newCol->value);
}

column* createColumnList(int numberOfColumns, char **values,int version)
{
	column *head=NULL,*temp=NULL,*newColumn;
	int i = 0;
	for (i = 0; i < numberOfColumns; i++)
	{
		newColumn = createColumn(version, values[i]);
		if (head == NULL)
		{
			head = newColumn;
			continue;
		}
		temp = head;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = newColumn;
	}
	return head;
}

row* createRow(int numberOfColumns, char **columnValues,int version)
{
	int i = 0;
	row *newRow;
	newRow = (row*)malloc(sizeof(row));
	newRow->columns = NULL;
	newRow->rowVersion = version;
	newRow->nextVer = version + 1;
	newRow->columns = createColumnList(numberOfColumns, columnValues, version);
	return newRow;
}

table* createTable(int numberOfColumns,char **columnnames)
{
	table *head;
	int i = 0;
	head = (table*)malloc(sizeof(table));
	head->numberOfRows = 0;
	head->rows = NULL;
	head->numberOfColumns = numberOfColumns;
	head->columnnames = (char**)malloc(sizeof(char*)*numberOfColumns);
	for (i = 0; i < numberOfColumns; i++)
	{
		head->columnnames[i] = (char*)malloc(sizeof(char)*30);
		str_cpy(columnnames[i], head->columnnames[i]);
	}
	head->rows = (row**)malloc(sizeof(row) * 50);
	head->rowsSize = 50;
	return head;
}

int insertRowInTable(table *Table, char **values)
{
	if (Table->numberOfRows == Table->rowsSize - 1)
	{
		//increase size later
	}
	Table->numberOfRows++;
	Table->rows[Table->numberOfRows] = createRow(Table->numberOfColumns, values, 0);
	return 1;
}

int columnIndex(char **columnNames, int numberOfColumns,char *columnToFind)
{
	int i = 0;
	for (i = 0; i < numberOfColumns; i++)
		if (str_cmp(columnNames[i], columnToFind) == 0)
			return i;
	return -1;
}

int update(table *Table, int rowNumber, char **columnNamesToUpdate, char **values, int numberOfColumnsToUpdte)
{
	column *newColumn,*requiredColumn,*temp;
	int index=-1,i = 0,commitedVersion,versionToCommit,newColumn;
	commitedVersion = Table->rows[rowNumber]->rowVersion;
	versionToCommit = Table->rows[rowNumber]->nextVer++;
	for (i = 0; i < numberOfColumnsToUpdte; i++)
	{
		index = columnIndex(Table->columnnames, Table->numberOfColumns, columnNamesToUpdate[i]);
		if (index == -1)
		{
			//error fill this
			return -1;
		}
		newColumn = createColumn(versionToCommit, values[i]);
		requiredColumn = Table->rows[rowNumber]->columns;
		while (index != 1)
		{
			requiredColumn = requiredColumn->next;
			index--;
		}
		temp = requiredColumn->next;
		newColumn->next = requiredColumn->next->next;
		newColumn->prevVersion = requiredColumn->next;
		requiredColumn->next = newColumn;
	}
	Table->rows[rowNumber]->rowVersion = versionToCommit;
	return 0;
}

int readRow(table *Table, int rowNumber)
{
	int versionCommited, i;
	column *head,*temp;
	versionCommited = Table->rows[rowNumber]->rowVersion;
	head = Table->rows[rowNumber]->columns;
	while (head!=NULL)
	{
		temp = head;
		while (temp->version != versionCommited)
		{
			temp = temp->prevVersion;
		}
		printf(" %s ", temp->value);
		head = head->next;
	}
	return 0;
}
