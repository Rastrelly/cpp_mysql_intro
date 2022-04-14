#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include "mysql.h"
#include <stdio.h>
#include <ctime>
#include <string>

using namespace std;

int query=0;
MYSQL_RES *mysqlres;
MYSQL_ROW row;
MYSQL_FIELD *fld;

int w=10, h=10;

bool tsucc = false;

int main(int argc, TCHAR* argv[])
{
	srand(time(NULL));
	
	//1. Объявляем объект MySQL. Этот объект будет
	//  символизировать наше соединение с сервером
	MYSQL *conn;
	// 2. Получаем дескриптор соединения
	conn = mysql_init(NULL);
	if (conn == NULL)
	{
		// Если дескриптор не получен – выводим сообщение об ошибке
		fprintf(stderr, "Error: can'tcreate MySQL-descriptor\n");
		//exit(1); //Если используется оконное приложение
	}
	// 3. Подключаемся к серверу
	if (!mysql_real_connect(conn, "localhost", "root",
		"DOOM", "test_schema", NULL, NULL, 0))
	{
		// Если нет возможности установить соединение с сервером
		// базы данных выводим сообщение об ошибке
		fprintf(stderr, "Error: can't connect to database %s\n", mysql_error(conn));
	}
	else
	{
		// Если соединение успешно установлено выводим фразу - "Success!"
		fprintf(stdout, "Success!\n");

		fprintf(stdout, "Input w, h\n");
		cin >> w >> h;

		if (w < 5) w = 5;
		if (h < 5) h = 5;

		query = mysql_query(conn, "SHOW TABLES LIKE 'new_table';");
		mysqlres = mysql_store_result(conn);
		if (mysqlres->row_count > 0)
		{
			query = mysql_query(conn, "DROP TABLE new_table");
		}

		string req = "";
		int i = 0;
		while (i < w)
		{
			req += "id_" + to_string(i) + " INT";
			if (i != w - 1) req += ",";
			i++;
		}
		string freq = "CREATE TABLE new_table (" + req + ")";
		query = mysql_query(conn,freq.c_str());
		if (query == 0) { cout << "Table Creation Success\n"; tsucc = true; }
		else { cout << "Table Creation Error\n"; tsucc = false; }

		if (tsucc)
		{
			//заполняем таблицу
			for (int j = 0; j < h; j++)
			{
				req = "";
				freq = "";
				for (int i = 0; i < w; i++)
				{
					req += to_string(rand() % 100);
					if (i != w - 1) req += ",";
				}
				freq = "INSERT INTO new_table VALUES("+req+")";
				query = mysql_query(conn, freq.c_str());
			}
		}

		int tw = 0, th = 0;
		query = mysql_query(conn, "SELECT * FROM new_table;");
		mysqlres = mysql_store_result(conn);
		th = mysqlres->row_count;
		tw = mysqlres->field_count;

		do
		{
			row = mysql_fetch_row(mysqlres);
			if (row)
				for (int i = 0; i < tw; i++)
				{
					cout << row[i] << " ";
					if (i==tw-1)
					cout << endl;
				}
			
				
		} while (row);

		/*query = mysql_query(conn, "INSERT INTO new_table VALUES(0,'TEST')");
		query = mysql_query(conn, "INSERT INTO new_table VALUES(1,'TEST2')");
		query = mysql_query(conn, "INSERT INTO new_table VALUES(2,'TEST3')");

		query = mysql_query(conn, "SELECT * FROM new_table;");
		mysqlres = mysql_store_result(conn);

		do
		{
			row = mysql_fetch_row(mysqlres);
			if (row)
				cout << row[0] << " " << row[1] << endl;
		} while (row);

		int coln = mysql_field_count(conn);
		cout << "Field count: " << coln << endl;
		while (fld = mysql_fetch_field(mysqlres))
		{
			cout << "Field data: " << fld->name << "; " << fld->type << endl;
		}*/

	}
	// 4. Закрываем соединение с сервером базы данных
	mysql_close(conn);

	system("Pause");
	return 0;
}
