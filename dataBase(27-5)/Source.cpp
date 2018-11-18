////#include<stdio.h>
////#include<stdlib.h>
////#include"str2.h"
////int main()
////{
////	FILE *fp = fopen("sample.bin", "wb");
////	FILE *rd = fopen("sample2.csv", "rt");
////	char **strings,buf[50];
////	
////	int i = 0;
////	struct ch
////	{
////		//short b;
////		int a;
////		char na[32];
////		char diatrict[20];
////	};
////	ch b[6];
////	while ((fgets(buf, 49, rd) != NULL)&&i!=6)
////	{
////		int num=0;
////		strings = splitStr(buf, ',', &num);
////		b[i].a = str_to_num(strings[0]);
////		str_cpy(strings[1], b[i].na);
////		str_cpy(strings[2], b[i].diatrict);
////		i++;
////	}
////	fwrite(b,sizeof(struct ch),6, fp);
////	fclose(fp);
////	//fp=fopen("sample2.bin", "w");
////	//fwrite(&b, sizeof(v), 1, fp);
////	system("pause");
////}
//
//
//
//
//#include<stdio.h>
//#include<stdlib.h>
//#include<time.h>
//#include"dataBaseStorage.h"
//#include<pthread.h>
////#include"Custom_strngs.h"
////#include"database.h"
//
//
//typedef void* (*func)(void *, void *, void *, void *, void *, void *);
//
//pthread_t threads[5];
//
//struct Thread
//{
//	int isFree = 1;
//	func function = NULL;
//};
//
//Thread threadStruct[5];
//
//column* createColumn(int version, char *value, int columnIndexVal)
//{
//	column *newCol;
//	newCol = (column*)malloc(sizeof(column));
//	newCol->value = (char*)malloc(sizeof(char) * 30);
//	newCol->prevVersion = NULL;
//	newCol->next = NULL;
//	if (value != NULL)
//	{
//		str_cpy(value, newCol->value);
//		newCol->version = version;
//		newCol->indexOfColumn = columnIndexVal;
//		return newCol;
//	}
//	return NULL;
//}
//
//int columnIndex(char **columnNames, int numberOfColumns, char *columnToFind)
//{
//	int i = 0;
//	for (i = 0; i < numberOfColumns; i++)
//		if (str_cmp(columnNames[i], columnToFind) == 0)
//			return i;
//	return -1;
//}
//
//column* createColumnList(table *Table, char **values, char **columnnames, int numOfValues, int version)
//{
//	column *head = NULL, *temp = NULL, *newColumn;
//	int index, i = 0, count = 0;
//	for (i = 0; i < numOfValues; i++)
//	{
//		/*if (values!=NULL)
//		newColumn = createColumn(version, values[i]);
//		else
//		newColumn = createColumn(version, NULL);*/
//		index = columnIndex(Table->columnnames, Table->numberOfColumns, columnnames[i]);
//		newColumn = createColumn(version, values[i], index);
//		if (index == -1)
//		{
//			printf("Error in  column creation No column as %s\n", columnnames[i]);
//			return NULL;
//		}
//		if (head == NULL)
//		{
//			head = newColumn;
//			continue;
//		}
//		temp = head;
//		while (temp->next != NULL)
//		{
//			temp = temp->next;
//		}
//		temp->next = newColumn;
//	}
//	return head;
//}
//
//row* createRow(table *Table, char **columnValues, char **columnnames, int numOfValues, int version, char *rowid)
//{
//	int i = 0;
//	row *newRow;
//	newRow = (row*)malloc(sizeof(row));
//	newRow->columns = NULL;
//	newRow->rowVersion = version;
//	newRow->nextVer = version + 1;
//	newRow->rowId = rowid;
//	newRow->columns = createColumnList(Table, columnValues, columnnames, numOfValues, version);
//	return newRow;
//}
//
//table* getTableFromDB(Database *db, char *tableId)
//{
//	int i = 0;
//	for (i = 0; i <= db->tablesInList; i++)
//		if (str_cmp(db->tableNames[i], tableId) == 0)
//			return db->TablesCreated[i];
//	return NULL;
//}
//
//int getType(char *str)
//{
//	if (str_cmp(str, "int") == 0)
//		return 0;
//	else if (str_cmp(str, "float") == 0)
//		return 1;
//	else if (str_cmp(str, "string") == 0)
//		return 2;
//	else
//	{
//		return -1;
//	}
//}
//
//table* createTable(int numberOfColumns, char **columnnames, char **types, int *IntTypes)
//{
//	table *head;
//	int i = 0;
//	head = (table*)malloc(sizeof(table));
//	head->numberOfRows = -1;
//	head->rows = NULL;
//	head->type = (int*)malloc(sizeof(int) * 30);
//	head->numberOfColumns = numberOfColumns;
//	head->columnnames = (char**)malloc(sizeof(char*)*numberOfColumns);
//	for (i = 0; i < numberOfColumns; i++)
//	{
//		head->columnnames[i] = (char*)malloc(sizeof(char) * 30);
//		str_cpy(columnnames[i], head->columnnames[i]);
//		if (IntTypes == NULL)
//			head->type[i] = getType(types[i]);
//		else
//			head->type[i] = IntTypes[i];
//	}
//	head->rows = (row**)malloc(sizeof(row) * 50);
//	head->rowsSize = 50;
//	return head;
//}
//
//table* addNewTable(Database *db, char *newTableId, int numOfColumns, char **columnNames, char **types, int *IntTypes)
//{
//	if (db->tablesInList == db->sizeOfList - 1)
//	{
//		db->sizeOfList += 30;
//		db->TablesCreated = (table**)realloc(db->TablesCreated, sizeof(table*)*db->sizeOfList);
//		db->tableNames = (char**)realloc(db->tableNames, sizeof(char*)*db->sizeOfList);
//	}
//	db->TablesCreated[++db->tablesInList] = createTable(numOfColumns, columnNames, types, IntTypes);
//	str_cpy(newTableId, db->tableNames[db->tablesInList]);
//	return db->TablesCreated[db->tablesInList];
//}
//
//int insertRowInTable(table *Table, char **columnnames, char **values, int numberOfValues, int version, char *rowId)
//{
//	if (Table->numberOfRows == Table->rowsSize - 1)
//	{
//		//increase size later
//	}
//	Table->numberOfRows++;
//	Table->rows[Table->numberOfRows] = createRow(Table, values, columnnames, numberOfValues, version, rowId);
//	return 1;
//}
//
//
//
//int isRowidinTable(table *Table, char *rowid)
//{
//	int i = 0;
//	for (i = 0; i <= Table->numberOfRows; i++)
//		if (Table->rows[i] != NULL && str_cmp(Table->rows[i]->rowId, rowid) == 0)
//			return i;
//	return -1;
//}
//
//int update(table *Table, char *rowId, char **columnNamesToUpdate, char **values, int numberOfColumnsToUpdate, int reqVersion)
//{
//	column *newColumn, *requiredColumn, *temp;
//	int index = -1, i = 0, rowNumber, commitedVersion, versionToCommit;
//	if (str_cmp(columnNamesToUpdate[0], Table->columnnames[0]) != 0)
//	{
//		printf("Plese enter the first argument as '%s' as it is primary key", Table->columnnames[0]);
//		return -1;
//	}
//	if ((rowNumber = isRowidinTable(Table, rowId)) == -1)
//	{
//		insertRowInTable(Table, Table->columnnames, NULL, 0, -1, rowId);
//		rowNumber = Table->numberOfRows;
//	}
//	commitedVersion = Table->rows[rowNumber]->rowVersion;
//	if (reqVersion>0 && reqVersion != commitedVersion&&commitedVersion != -1)
//		return -1;
//	versionToCommit = Table->rows[rowNumber]->nextVer++;
//	for (i = 0; i < numberOfColumnsToUpdate; i++)
//	{
//		index = columnIndex(Table->columnnames, Table->numberOfColumns, columnNamesToUpdate[i]);
//		if (index == -1)
//		{
//			//error fill this
//			return -1;
//		}
//		newColumn = createColumn(versionToCommit, values[i], index);
//		requiredColumn = Table->rows[rowNumber]->columns;
//		if (requiredColumn == NULL)
//		{
//			Table->rows[rowNumber]->columns = newColumn;
//		}
//		else if (index != 0)
//		{
//			while (index > 1)
//			{
//				requiredColumn = requiredColumn->next;
//				index--;
//			}
//			temp = requiredColumn->next;
//			if (requiredColumn->next != NULL)
//				newColumn->next = requiredColumn->next->next;
//			if (commitedVersion != -1)
//			{
//				newColumn->prevVersion = requiredColumn->next;
//				//free(temp);
//			}
//			else
//				newColumn->prevVersion = NULL;
//			requiredColumn->next = newColumn;
//
//		}
//		else
//		{
//			temp = Table->rows[rowNumber]->columns;
//			Table->rows[rowNumber]->columns = newColumn;
//			if (commitedVersion != -1)
//				newColumn->prevVersion = temp;
//			newColumn->next = temp->next;
//			//free(temp);
//		}
//	}
//	Table->rows[rowNumber]->rowVersion = versionToCommit;
//	return 0;
//}
//
//int readRow(table *Table, char *rowId)
//{
//	int versionCommited, rowNumber;
//	column *head, *temp;
//	rowNumber = isRowidinTable(Table, rowId);
//	versionCommited = Table->rows[rowNumber]->rowVersion;
//	head = Table->rows[rowNumber]->columns;
//	while (head != NULL)
//	{
//		temp = head;
//		while (temp->version != versionCommited)
//		{
//			temp = temp->prevVersion;
//		}
//		printf(" %s ", temp->value);
//		head = head->next;
//	}
//	return 0;
//}
//
//row* getRow(table *Table, char *rowId)
//{
//	int rowNum;
//	rowNum = isRowidinTable(Table, rowId);
//	if (rowNum != -1)
//		return Table->rows[rowNum];
//	return NULL;
//}
//
//column* getColumn(row *Row, int index, int version)
//{
//	int i = 0;
//	column *col;
//	col = Row->columns;
//	while (index != 0 && col != NULL)
//	{
//		col = col->next;
//		index--;
//	}
//	while (col->version > version&&col != NULL)
//	{
//		col = col->prevVersion;
//	}
//	return col;
//}
//
///*int addFollow(table *Table1_follower, table *Table2_follow, int userid1_Follower, int userId2_Follow,char *followerColumnName,char *followColumnName)
//{
//int followIndex, followerIndex,currentVersion;
//row *followerRow, *followRow;
//column *colFollow, *colFollower;
//followerIndex = columnIndex(Table1_follower->columnnames, Table1_follower->numberOfColumns, followerColumnName);
//followIndex = columnIndex(Table2_follow->columnnames, Table2_follow->numberOfColumns, followColumnName);
//followerRow = getRow(Table1_follower, userid1_Follower);
//followRow = getRow(Table2_follow, userId2_Follow);
//if (followRow == NULL)
//{
//
//}
//colFollow = getColumn(followRow, followIndex, followRow->rowVersion);
//colFollower = getColumn(followerRow, followerIndex, followerRow->rowVersion);
//while (colFollow->next != NULL)
//colFollow = colFollow->next;
//while (colFollower->next != NULL)
//colFollower = colFollower->next;
//currentVersion = followRow->rowVersion;
//colFollow = createColumn(currentVersion, to_str(userid1_Follower));
//currentVersion = followerRow->rowVersion;
//colFollower = createColumn(currentVersion, to_str(userId2_Follow));
//return 0;
//}*/
//
//int commandUI(Database *db)
//{
//	table *Table, *Table2;
//	row *Row = NULL;
//	column *col = NULL, *temp;
//	char **strings, buf[70], *columnNames[10], *columnValues[10], *tableId, *rowId, *types[10];
//	int childId = 0, numofStrings, i, numofColumnsToUpdate = 0, currentVersion;
//	for (i = 0; i < 10; i++)
//	{//update here for later
//		columnValues[i] = (char*)malloc(sizeof(char) * 30);
//		columnNames[i] = (char*)malloc(sizeof(char) * 30);
//		types[i] = (char*)malloc(sizeof(char) * 30);
//	}
//	while (1)
//	{
//		printf("\n$> ");
//		scanf("%[^\n]s", buf);
//		getchar();
//		numofColumnsToUpdate = 0;
//		strings = splitStr(buf, ' ', &numofStrings);
//		if (str_cmp(strings[0], "UPDATE") == 0)
//		{
//			tableId = strings[1];
//			rowId = strings[4];
//			for (i = 2; i < numofStrings - 1;)
//			{
//				str_cpy(strings[i], columnNames[numofColumnsToUpdate]);
//				str_cpy(strings[i + 2], columnValues[numofColumnsToUpdate]);
//				i += 3;
//				numofColumnsToUpdate++;
//			}
//			int version = str_to_num(strings[numofStrings - 1]);
//			update(Table, rowId, columnNames, columnValues, numofColumnsToUpdate, version);
//		}
//		else if (str_cmp(strings[0], "CREATE") == 0)
//		{//create tablename(name type)
//			tableId = strings[1];
//			for (i = 2; i < numofStrings; i += 2)
//			{
//				str_cpy(strings[i], columnNames[numofColumnsToUpdate]);
//				str_cpy(strings[i + 1], types[numofColumnsToUpdate++]);
//			}
//			Table = addNewTable(db, tableId, numofColumnsToUpdate, columnNames, types, NULL);
//		}
//		else if (str_cmp(strings[0], "ADD") == 0)
//		{
//			tableId = strings[1];
//			Table = getTableFromDB(db, tableId);
//			for (i = 2; i < numofStrings; i++)
//				str_cpy(strings[i], columnValues[numofColumnsToUpdate++]);
//			insertRowInTable(Table, Table->columnnames, columnValues, numofStrings - 2, 0, strings[1]);
//		}
//		else if (str_cmp(strings[0], "GET") == 0)
//		{
//			tableId = strings[1];
//			Table = getTableFromDB(db, tableId);
//			Row = getRow(Table, strings[2]);
//			currentVersion = Row->rowVersion;
//			col = Row->columns;
//			for (i = 0; i < Table->numberOfColumns; i++)
//			{
//				temp = col;
//				while (temp != NULL&&temp->version > currentVersion)
//				{
//					temp = temp->prevVersion;
//				}
//				if (temp != NULL)
//					printf("%s ", temp->value);
//				col = col->next;
//			}
//		}
//		else if (str_cmp(strings[0], "PUT") == 0)
//		{//put(tb, cl1 = val1,...,version)
//			tableId = strings[1];
//			Table = getTableFromDB(db, tableId);
//			rowId = strings[4];
//			for (i = 2; i < numofStrings - 1;)
//			{
//				str_cpy(strings[i], columnNames[numofColumnsToUpdate]);
//				str_cpy(strings[i + 2], columnValues[numofColumnsToUpdate]);
//				i += 3;
//				numofColumnsToUpdate++;
//			}
//			int version = str_to_num(strings[numofStrings - 1]);
//			update(Table, rowId, columnNames, columnValues, numofColumnsToUpdate, version);
//		}
//		/*else if (str_cmp(strings[0], "FOLLOW") == 0)
//		{//1 userTableid 2 followTableid 3 followersTableId  4 postTable
//		Table = getTableFromDB(db, 2);//follow
//		Table2 = getTableFromDB(db, 3);//follower
//		addFollow(Table2, Table,str_to_num(strings[1]),str_to_num(strings[2]), "FOLLOWS", "FOLLOW");
//		}*/
//		else if (str_cmp(strings[0], "POST") == 0)
//		{
//			str_cpy("TIMESTAMP", columnNames[numofColumnsToUpdate]);
//			str_cpy(__TIMESTAMP__, columnValues[numofColumnsToUpdate]);
//			numofColumnsToUpdate++;
//			for (i = 1; i < numofStrings - 1;)
//			{
//				str_cpy(strings[i], columnNames[numofColumnsToUpdate]);
//				str_cpy(strings[i + 2], columnValues[numofColumnsToUpdate]);
//				i += 3;
//				numofColumnsToUpdate++;
//			}
//			int version = str_to_num(strings[numofStrings - 1]);
//			update(Table, rowId, columnNames, columnValues, numofColumnsToUpdate, version);
//		}
//		else if (str_cmp(strings[0], "EXIT") == 0)
//		{
//			writeIndexFile(db, "index.txt");
//			writeDataBase(db);
//			break;
//		}
//	}
//	return 0;
//}
//
//
//
//table* createTableFromIndexFile(char **tableData, Database *db)
//{
//	table *Table = (table*)malloc(sizeof(table));
//	char **strings, **columnNames;
//	int num = 0, i, j, *typeArray;
//	strings = splitStr(tableData[1], '_', &num);
//	columnNames = (char**)malloc(sizeof(char*)*(num / 2));
//	typeArray = (int*)malloc(sizeof(int)*(num / 2));
//	for (i = 0, j = 0; i < num / 2;)
//	{
//		columnNames[i] = (char*)malloc(sizeof(char)*(30));
//		str_cpy(strings[j], columnNames[i]);
//		j++;
//		typeArray[i] = str_to_num(strings[j]);
//		i++; j++;
//	}
//	addNewTable(db, tableData[0], i, columnNames, NULL, typeArray);
//	return NULL;
//}
//
//int main()
//{
//	Database *db;
//	db = (Database*)malloc(sizeof(Database));
//	db->sizeOfList = 30;
//	db->TablesCreated = (table**)malloc(sizeof(table) * 30);//c
//	db->tableNames = (char**)malloc(sizeof(char*) * 30);
//	for (int i = 0; i < 30; i++)
//		db->tableNames[i] = (char*)malloc(sizeof(char) * 30);
//	db->tablesInList = -1;
//	commandUI(db);
//	system("pause");
//}
///*
//create users(emailid string,phone int)
//create uFollow(emailid string,follows string)
//create followU(emailid string,followers string)
//create post(emailid string,time string,message string,imgUrl string)
//
//*/
//
//
//int copyDataToTable(table *Table)
//{
//	return 0;
//}
//
//int getThread(func Function, void **array)
//{
//	int i = 0;
//	for (i = 0; i < 5; i++)
//	{
//		if (threadStruct[i].isFree == 1)
//		{
//
//		}
//	}
//}
//
//int shedular(func function, void **array)
//{
//
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
