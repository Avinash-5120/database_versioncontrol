#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"dataBaseStorage.h"
#include<pthread.h>
#include<Windows.h>
#include<myDetails.h>
//#include"Custom_strngs.h"
//#include"database.h"

#define ThreadCount 4
#define SizeOfQue 10
typedef void* (*func)(void **);
pthread_mutex_t lock;
pthread_t threads[ThreadCount];

struct que{
	func function=NULL;
	void **array;
};

struct Thread
{
	int isFree = 1;
	func function = NULL;
	void **array;
};

Thread threadStruct[ThreadCount];
void* shedular(void *);
int insertIntoShedularQue(func, void **);
int first = -1, next = 0;
que SheduleQue[SizeOfQue];

int createThreads();
column* createColumn(int version, char *value,int columnIndexVal)
{
	column *newCol;
	newCol = (column*)ab_malloc(sizeof(column));
	newCol->value = (char*)ab_malloc(sizeof(char) * 30);
	newCol->prevVersion = NULL;
	newCol->next = NULL;
	if (value != NULL)
	{
		str_cpy(value, newCol->value);
		newCol->version = version;
		newCol->indexOfColumn = columnIndexVal;
		return newCol;
	}
	return NULL;
}

int columnIndex(char **columnNames, int numberOfColumns, char *columnToFind)
{
	int i = 0;
	for (i = 0; i < numberOfColumns; i++)
		if (str_cmp(columnNames[i], columnToFind) == 0)
			return i;
	return -1;
}

column* createColumnList(table *Table, char **values, char **columnnames,int numOfValues,int version)
{
	column *head = NULL, *temp = NULL, *newColumn;
	int index,i = 0,count=0;
	for (i = 0; i < numOfValues; i++)
	{
		/*if (values!=NULL)
			newColumn = createColumn(version, values[i]);
		else
			newColumn = createColumn(version, NULL);*/
		index = columnIndex(Table->columnnames, Table->numberOfColumns, columnnames[i]);
		newColumn = createColumn(version, values[i], index);
		if (index == -1)
		{
			printf("Error in  column creation No column as %s\n",columnnames[i]);
			return NULL;
		}
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

row* createRow(table *Table, char **columnValues,char **columnnames,int numOfValues, int version,char *rowid)
{
	int i = 0;
	row *newRow;
	newRow = (row*)ab_malloc(sizeof(row));
	newRow->columns = NULL;
	newRow->rowVersion = version;
	newRow->nextVer = version + 1;
	newRow->rowId = rowid;
	newRow->columns = createColumnList(Table, columnValues,columnnames,numOfValues, version);
	return newRow;
}

table* getTableFromDB(Database *db, char *tableId)
{
	int i = 0;
	for (i = 0; i <= db->tablesInList; i++)
		if (str_cmp( db->tableNames[i],tableId)==0)
			return db->TablesCreated[i];
	return NULL;
}

int getType(char *str)
{
	if (str_cmp(str, "int") == 0)
		return 0;
	else if (str_cmp(str, "float") == 0)
		return 1;
	else if (str_cmp(str, "string") == 0)
		return 2;
	else
	{
		return -1;
	}
}

table* createTable(int numberOfColumns, char **columnnames,char **types,int *IntTypes)
{
	table *head;
	int i = 0;
	head = (table*)ab_malloc(sizeof(table));
	head->numberOfRows = -1;
	head->rows = NULL;
	head->type = (int*)ab_malloc(sizeof(int) * 30);
	head->numberOfColumns = numberOfColumns;
	head->columnnames = (char**)ab_malloc(sizeof(char*)*numberOfColumns);
	for (i = 0; i < numberOfColumns; i++)
	{
		head->columnnames[i] = (char*)ab_malloc(sizeof(char) * 30);
		str_cpy(columnnames[i], head->columnnames[i]);
		if (IntTypes == NULL)
			head->type[i] = getType(types[i]);
		else
			head->type[i] = IntTypes[i];
	}
	head->rows = (row**)malloc(sizeof(row) * 50);
	head->rowsSize = 50;
	return head;
}

table* addNewTable(Database *db, char *newTableId, int numOfColumns, char **columnNames,char **types,int *IntTypes)
{
	if (db->tablesInList == db->sizeOfList - 1)
	{
		db->sizeOfList += 30;
		db->TablesCreated = (table**)realloc(db->TablesCreated, sizeof(table*)*db->sizeOfList);
		db->tableNames = (char**)realloc(db->tableNames, sizeof(char*)*db->sizeOfList);
	}
	db->TablesCreated[++db->tablesInList] = createTable(numOfColumns, columnNames,types,IntTypes);
	str_cpy(newTableId, db->tableNames[db->tablesInList]);
	return db->TablesCreated[db->tablesInList];
}

void* insertRowInTable(void **args)
{
	table *Table; char **columnnames; char **values; int numberOfValues; int version; char *rowId;
	Table = (table*)args[0];
	columnnames = (char**)args[1];
	values = (char**)args[2];
	numberOfValues = (int)args[3];
	version = (int)args[4];
	rowId = (char*)args[5];
	pthread_mutex_lock(&lock);
	if (Table->numberOfRows == Table->rowsSize - 1)
	{
		//increase size later
	}
	Table->numberOfRows++;
	Table->rows[Table->numberOfRows] = createRow(Table, values,columnnames,numberOfValues,version, rowId);
	printf("inserted\n");
	Sleep(10000);
	pthread_mutex_unlock(&lock);
	return (void*)1;
}



int isRowidinTable(table *Table, char *rowid)
{
	int i = 0;
	for (i = 0; i <= Table->numberOfRows; i++)
		if (Table->rows[i]!=NULL && str_cmp(Table->rows[i]->rowId , rowid)==0)
			return i;
	return -1;
}

void* update(void **arr)
{
	column *newColumn, *requiredColumn, *temp;
	table *Table = (table*)arr[0]; char *rowId = (char*)arr[1]; char **columnNamesToUpdate = (char**)arr[2]; char **values = (char**)arr[3]; int numberOfColumnsToUpdate = (int)arr[4]; int reqVersion = (int)arr[5];
	int index = -1, i = 0, rowNumber, commitedVersion, versionToCommit;
	if (str_cmp(columnNamesToUpdate[0], Table->columnnames[0]) != 0)
	{
		printf("Plese enter the first argument as '%s' as it is primary key", Table->columnnames[0]);
		return (void*)-1;
	}
	if ((rowNumber=isRowidinTable(Table,rowId))==-1)
	{
		void *ar[6];
		ar[0] = Table; ar[1] = Table->columnnames; ar[2] = NULL; ar[3] = (void*)0; ar[4] = (void*)-1; ar[5] = rowId;
		insertRowInTable(ar);
		rowNumber = Table->numberOfRows;
	}
	commitedVersion = Table->rows[rowNumber]->rowVersion;
	if (reqVersion>0 && reqVersion != commitedVersion&&commitedVersion!=-1)
		return (void*)-1;
	versionToCommit = Table->rows[rowNumber]->nextVer++;
	for (i = 0; i < numberOfColumnsToUpdate; i++)
	{
		index = columnIndex(Table->columnnames, Table->numberOfColumns, columnNamesToUpdate[i]);
		if (index == -1)
		{
			//error fill this
			return (void*)-1;
		}
		newColumn = createColumn(versionToCommit, values[i],index);
		requiredColumn = Table->rows[rowNumber]->columns;
		if (requiredColumn == NULL)
		{
			Table->rows[rowNumber]->columns = newColumn;
		}
		else if (index != 0)
		{
			while (index > 1)
			{
				requiredColumn = requiredColumn->next;
				index--;
			}
			temp = requiredColumn->next;
			if (requiredColumn->next!=NULL)
				newColumn->next = requiredColumn->next->next;
			if (commitedVersion != -1)
			{
				newColumn->prevVersion = requiredColumn->next;
				//free(temp);
			}
			else
				newColumn->prevVersion = NULL;
			requiredColumn->next = newColumn;

		}
		else
		{
			temp = Table->rows[rowNumber]->columns;
			Table->rows[rowNumber]->columns = newColumn;
			if (commitedVersion!=-1)
				newColumn->prevVersion = temp;
			newColumn->next = temp->next;
			//free(temp);
		}
	}  
	Table->rows[rowNumber]->rowVersion = versionToCommit;
	return 0;
}

int readRow(table *Table, char *rowId)
{
	int versionCommited, rowNumber;
	column *head, *temp;
	rowNumber = isRowidinTable(Table, rowId);
	versionCommited = Table->rows[rowNumber]->rowVersion;
	head = Table->rows[rowNumber]->columns;
	while (head != NULL)
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

void printRow(row *Row,table *Table)
{
	int currentVersion = Row->rowVersion;
	column *temp,*col = Row->columns;
	for (int i = 0; i < (Table->numberOfColumns); i++)
	{
		temp = col;
		while (temp != NULL&&temp->version > currentVersion)
		{
			temp = temp->prevVersion;
		}
		if (temp != NULL)
			printf(" %s \n", temp->value);
		col = col->next;
	}
}

void* getRow(void **arr)
{
	table *Table; char *rowId;
	Table = (table*)arr[0];
	rowId = (char*) arr[1];
	int rowNum;
	rowNum = isRowidinTable(Table, rowId);
	if (rowNum != -1)
		printRow(Table->rows[rowNum], Table);
	else
		printf("not found\n");
	return NULL;
}

column* getColumn(row *Row,int index,int version)
{
	int i = 0;
	column *col;
	col = Row->columns;
	while (index != 0 && col != NULL)
	{
		col = col->next;
		index--;
	}
	while (col->version > version&&col != NULL)
	{
		col = col->prevVersion;
	}
	return col;
}



int commandUI(Database *db)
{
	table *Table;
	row *Row = NULL;
	column *col = NULL;
	char **strings, buf[70], *columnNames[10], *columnValues[10], *tableId, *rowId,*types[10];
	int childId=0,numofStrings, i, numofColumnsToUpdate = 0;
		for (i = 0; i < 10; i++)
		{//update here for later
			columnValues[i] = (char*)ab_malloc(sizeof(char) * 30);
			columnNames[i] = (char*)ab_malloc(sizeof(char) * 30);
			types[i] = (char*)ab_malloc(sizeof(char) * 30);
		}
	while (1)
	{ 
		printf("\n$>");
 		scanf("%[^\n]s", buf); 
		getchar();
		numofColumnsToUpdate = 0;
		strings = splitStr(buf, ' ', &numofStrings);
		if (str_cmp(strings[0], "UPDATE") == 0)
		{ 
			tableId = strings[1];
			rowId = strings[4];
			for (i = 2; i < numofStrings-1;)
			{
				str_cpy(strings[i], columnNames[numofColumnsToUpdate]);
				str_cpy(strings[i + 2], columnValues[numofColumnsToUpdate]);
				i += 3;
				numofColumnsToUpdate++;
			}
			int version = str_to_num(strings[numofStrings - 1]);
			void **ar;
			ar = (void**)malloc(sizeof(void*) * 6);
			ar[0] = Table; ar[1] = rowId; ar[2] = columnNames; ar[3] = columnValues; ar[4] = (void*)numofColumnsToUpdate; ar[5] = (void*)version;
			/*update(Table, rowId, columnNames, columnValues, numofColumnsToUpdate,version);*/
			insertIntoShedularQue(update, ar);
		}
		else if (str_cmp(strings[0], "CREATE") == 0)
		{//create tablename(name type)
			tableId = strings[1];
			for (i = 2; i < numofStrings; i+=2)
			{
				str_cpy(strings[i], columnNames[numofColumnsToUpdate]);
				str_cpy(strings[i + 1], types[numofColumnsToUpdate++]);
			}
			Table = addNewTable(db,tableId,numofColumnsToUpdate, columnNames,types,NULL);
		}
		else if (str_cmp(strings[0], "ADD") == 0)
		{
			tableId = strings[1];
			Table = getTableFromDB(db, tableId);
			for (i = 2; i < numofStrings; i++)
				str_cpy(strings[i], columnValues[numofColumnsToUpdate++]);
			void **ar;
			ar = (void**)ab_malloc(sizeof(void*) * 6);
			ar[0] = Table; ar[1] = Table->columnnames; ar[2] = columnValues; ar[3] =(void*)(numofStrings - 2); ar[4] = (void*)0; ar[5] = strings[2];
			insertIntoShedularQue(insertRowInTable, ar);
		}
		else if (str_cmp(strings[0], "GET") == 0)
		{
			tableId = strings[1];
			Table = getTableFromDB(db, tableId);
			void **ar;
			ar = (void**)malloc(sizeof(void*) * 2);
			ar[0] = Table;ar[1]=strings[2];
			//Row=getRow(ar);
			insertIntoShedularQue(getRow, ar);
			/*currentVersion = Row->rowVersion;
			col = Row->columns;
			for (i = 0; i < Table->numberOfColumns; i++)
			{
				temp = col;
				while (temp != NULL&&temp->version > currentVersion)
				{
					temp = temp->prevVersion;
				}
				if (temp != NULL)
					printf("%s ", temp->value);
				col = col->next;
			}*/
		}
		else if (str_cmp(strings[0],"PUT")==0)
		{//put(tb, cl1 = val1,...,version)
			tableId = strings[1];
			Table = getTableFromDB(db, tableId);
			rowId = strings[4];
			for (i = 2; i < numofStrings-1;)
			{
				str_cpy(strings[i], columnNames[numofColumnsToUpdate]);
				str_cpy(strings[i + 2], columnValues[numofColumnsToUpdate]);
				i += 3;
				numofColumnsToUpdate++;
			}
			int version = str_to_num(strings[numofStrings - 1]);
			void **ar;
			ar = (void**)ab_malloc(sizeof(void*) * 6);
			ar[0] = Table; ar[1] = rowId; ar[2] = columnNames; ar[3] = columnValues; ar[4] = (void*)numofColumnsToUpdate; ar[5] = (void*)version;
			/*update(Table, rowId, columnNames, columnValues, numofColumnsToUpdate,version);*/
			insertIntoShedularQue(update, ar);
		}
		else if (str_cmp(strings[0], "POST") == 0)
		{
			tableId = strings[1];
			Table = getTableFromDB(db, tableId);
			rowId = strings[4];
			str_cpy("TIMESTAMP", columnNames[numofColumnsToUpdate]);
			str_cpy(__TIMESTAMP__, columnValues[numofColumnsToUpdate]);
			numofColumnsToUpdate++;
			for (i = 1; i < numofStrings - 1;)
			{
				str_cpy(strings[i], columnNames[numofColumnsToUpdate]);
				str_cpy(strings[i + 2], columnValues[numofColumnsToUpdate]);
				i += 3;
				numofColumnsToUpdate++;
			}
			int version = str_to_num(strings[numofStrings - 1]);
			void **ar;
			ar = (void**)malloc(sizeof(void*) * 6);
			ar[0] = Table; ar[1] = rowId; ar[2] = columnNames; ar[3] = columnValues; ar[4] = (void*)numofColumnsToUpdate; ar[5] = (void*)version;
			/*update(Table, rowId, columnNames, columnValues, numofColumnsToUpdate,version);*/
			insertIntoShedularQue(update, ar);
		}
		else if (str_cmp(strings[0], "EXIT") == 0)
		{
			writeIndexFile(db, "index.txt");
			writeDataBase(db);
			ab_free(db);
			break;
		}
	}
	return 0;
}

 
  
table* createTableFromIndexFile(char **tableData, Database *db)
{
	table *Table = (table*)malloc(sizeof(table));
	char **strings,**columnNames;
	int num=0, i, j,*typeArray;
	strings = splitStr(tableData[1], '_', &num);
	columnNames = (char**)malloc(sizeof(char*)*(num / 2));
	typeArray = (int*)malloc(sizeof(int)*(num / 2));
	for (i = 0,j=0; i < num/2;)
	{
		columnNames[i] = (char*)malloc(sizeof(char)*(30));
		str_cpy(strings[j], columnNames[i]);
		j++;
		typeArray[i] = str_to_num(strings[j]);
		i++; j++;
	}
	addNewTable(db, tableData[0], i, columnNames, NULL, typeArray);
	return NULL;
}

int main()
{
	Database *db;
	pthread_t shedularThread;
	db = (Database*)malloc(sizeof(Database));
	db->sizeOfList = 30;
	db->TablesCreated = (table**)malloc(sizeof(table) * 30);//c
	db->tableNames = (char**)malloc(sizeof(char*) * 30);
	for (int i = 0; i < 30; i++)
		db->tableNames[i] = (char*)malloc(sizeof(char) * 30);
	db->tablesInList = -1;
	if (pthread_mutex_init(&lock, NULL) != 0)
	{
		printf("error in mutex initialization\n");
		return -1;
	}
	createThreads();
	pthread_create(&shedularThread, NULL, shedular, db);
	commandUI(db);
	system("pause");
}
/*
create users(emailid string,phone int)
create uFollow(emailid string,follows string)
create followU(emailid string,followers string)
create post(emailid string,time string,message string,imgUrl string)

*/


//int copyDataToTable(table *Table)
//{
//	return 0;
//}

void* funcForThread(void *arg)
{
	Thread *struc = (Thread*)arg;
	while (1)
	{
		if (struc->function != NULL)
		{ 
			struc->function(struc->array);
			struc->function = NULL;
			struc->isFree = 1;
		}
		else
			Sleep(100);
	}
}

int createThreads()
{
	int i = 0;
	for (i = 0; i < ThreadCount; i++)
	{
		pthread_create(&threads[i], NULL, funcForThread, &threadStruct[i]);
	}
	return 0;
}



int getThread(func Function,void **array)
{
	int i = 0;
	for (i = 0; i < ThreadCount; i++)
	{
		if (threadStruct[i].isFree == 1)
		{
			threadStruct[i].array = array;
			threadStruct[i].function = Function;
			threadStruct[i].isFree = -1;
			return i;
		}
	}
	return -1;
}

int insertIntoShedularQue(func function,void **array)
{
	if (next == first||(next==SizeOfQue&&first==0))
	{
		printf("que is full please try again");
	}
	if (first == -1)
	{
		first++;
	}
	if (next == SizeOfQue&&first > 0)
		next = -1;
	SheduleQue[next].function = function;
	SheduleQue[next].array = array;
	next++;
	return next;
}

que returnFirstOfQue()
{
	que result;
	if (first == -1)
	{
		result.function = NULL;
		return result;
	}
	result = SheduleQue[first++];
	if (first == SizeOfQue && next < first)
		first = 0;
	else if (first == next)
	{
		first = -1;
		next = 0;
	}
	return result;
}


void* shedular(void *arg)
{
	que queToExecute;
	queToExecute.function = NULL;
	while (1)
	{
		if (queToExecute.function == NULL)
		{
			queToExecute = returnFirstOfQue();
		}
		else if (getThread(queToExecute.function,queToExecute.array) == -1)
		{
			Sleep(100);
			//printf("error 505 : no thread is available\n");
		}
		else
			queToExecute.function = NULL;
	}
	return NULL;
}

//void* Randomgen(void *arg)
//{
//	func functionToExecute;
//	void *array[6],**args;
//	args = (void**)arg;
//	table *Table;
//	int numOfStrings;
//	Table = (table*)args[0];
//	numOfStrings = (int)args[1];
//	//ar = (void**)malloc(sizeof(void*) * 6);
//	//ar[0] = Table; ar[1] = Table->columnnames; ar[2] = columnValues; ar[3] = (void*)(numofStrings - 2); ar[4] = (void*)0; ar[5] = strings[1];
//	int count = 0;
//	for (;;count++)
//	{
//		//to insert
//		
//		
//	}
//}

void gen_random(char *s, const int len) {
	int i = 0;
	static const char alphanum[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	srand(NULL);
	for (i = 0; i < len; ++i) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	s[len] = '\0';
}