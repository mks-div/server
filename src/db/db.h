
//#pragma once
#include <iostream>
//#include <../../includes/sqlite3.h>
#include <vector>
#include <unordered_set>

#include "sqlite3.h"
#include "../utility/variables.h"
// g++ db_test.cpp -0 test -L. -lsqlite3
// test.exe

using namespace std;





const unordered_set<string> TABLE_FIELDS = {"name", "password", "token", "date"};
// const char * DB_PATH = "files/db/users.db";
//const char * DB_PATH = "users.db";

struct user {
    string name;
    string password;
    string token;
    string date;
    int empty() {return name.empty() && password.empty() && token.empty() && date.empty();}
    user() : name(""), password(""), token(""), date("") {}
    user(string nm, string pswrd, string tkn="", string dt="") : name(nm), password(pswrd), token(tkn), date(dt) {}
};



// Callback-функция для вывода результатов запроса
static void callback(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
    cout << "\n";
}

class Database {
    private:
        sqlite3* db;
        char* errMsg = nullptr;
        int rc;


    public:
        void Create_db() {
            rc = sqlite3_open(server_values::DB_PATH, &db);
            if (rc != SQLITE_OK) 
                throw string{"Database reading error:" + string(sqlite3_errmsg(db)) + "\n"};   
                                                                                                                                                                                 
            else 
                cout << "Database opened\n";
        }

        void Create_table() {
            const char * sql = "CREATE TABLE IF NOT EXISTS Users ("
            "name     TEXT PRIMARY KEY UNIQUE NOT NULL,"      
            "password TEXT NOT NULL,"
            "token    TEXT,"
            "date     TEXT);";
            

            rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
            if (rc != SQLITE_OK) {
                string mes = string(errMsg);
                sqlite3_free(errMsg);
                throw string{"SQL Error: " + mes + "\n"};
            } 
            else 
                cout << "Table opened\n";
        }

        void Create_Connection() {
            try {
                Create_db();
                Create_table();
            } catch(string err) {
                throw string(err);
            }
        }
        
        ~Database() {
            sqlite3_close(db);
        }

        // нужен для записи новых пользователей
        void Insert(const string& name, const string& password, const string& bearer_token="", const string& date="") {
            const char* sql = "INSERT INTO Users (name, password, token, date) VALUES (?, ?, ?, ?);";
            sqlite3_stmt* stmt;
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

            if (rc != SQLITE_OK) 
                throw string{"Error preparing: " + string(sqlite3_errmsg(db)) + "\n"};
            
            sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

            if (bearer_token.empty()) 
                sqlite3_bind_null(stmt, 3);  
            else 
                sqlite3_bind_text(stmt, 3, bearer_token.c_str(), -1, SQLITE_TRANSIENT);
            

            if (date.empty()) 
                sqlite3_bind_null(stmt, 4);  
            else 
                sqlite3_bind_text(stmt, 4, date.c_str(), -1, SQLITE_TRANSIENT);
            

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) 
                throw string{"Error serving: " + string(sqlite3_errmsg(db)) + "\n"};
            else 
                cout << "Data added\n";
                
            sqlite3_finalize(stmt);
        }

        // изменяет значение строки если есть имя и пароль
        void Change(const string& name, const string& password, const string& field, const string& new_value) {

            if (!TABLE_FIELDS.count(field)) 
                throw string{"Error: Invalid field name. Allowed fields: name, password, token, date\n"};
       
        
            string sql = "UPDATE Users SET " + field + " = ? WHERE name = ? AND (password = ? OR token = ?);";
            sqlite3_stmt* stmt;
            
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
            if (rc != SQLITE_OK) 
                throw string{ "Preparing error : " + string(sqlite3_errmsg(db)) + "\n"};
        
            sqlite3_bind_text(stmt, 1, new_value.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_TRANSIENT);
        
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                sqlite3_finalize(stmt);
                throw string{"Update error: " + string(sqlite3_errmsg(db)) + "\n"};
            }
        
            if (sqlite3_changes(db) == 0) {
                sqlite3_finalize(stmt);
                throw string{"user not found\n"};
            }
        
            cout << "Field '" << field << "' updated\n";
            sqlite3_finalize(stmt);
        }


        // ищет данные конкретного человека если есть (по паролю и нику)
        user Authorize(const string& name, const string& password, const string&token="") { 


            auto authorize_vals = password.empty() ? make_pair("token", token.c_str()) : make_pair("password", password.c_str());
            string sql = "SELECT * FROM Users WHERE " + string(token.empty() ? "Name = ? AND " : "") + string(authorize_vals.first) + " = ?;";
            sqlite3_stmt* stmt;
            string result = "nothing";
            user answ;

            int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
            if (rc != SQLITE_OK) 
                throw string{"preparing error: " + string(sqlite3_errmsg(db)) + "\n"};


            if (token.empty()) {
                
                sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt, 2, authorize_vals.second, -1, SQLITE_TRANSIENT);
            } else {
                sqlite3_bind_text(stmt, 1, authorize_vals.second, -1, SQLITE_TRANSIENT);
            }
 

            // Подготовка запроса


            
            // sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
            // sqlite3_bind_text(stmt, 2, authorize_vals.second, -1, SQLITE_TRANSIENT);
            // sqlite3_bind_text(stmt, 2, authorize_vals.second, -1, SQLITE_TRANSIENT);


            bool found = false;
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                found = true;
                //int id = sqlite3_column_int(stmt, 0);
                string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                const char* token = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                const char* date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)); 

                result = "----------------------"
                        "\nName: " + name +
                        "\nPassword: " + password +
                        "\nToken: " + (token ? token : "NULL") +
                        "\nDate: " + (date ? date : "NULL") + "\n";
                answ = user(name, password, (token ? token : "NULL"), (date ? date : "NULL"));
            }

            if (rc != SQLITE_DONE) 
                throw string{"reading error: " + string(sqlite3_errmsg(db)) + "\n"};
            
            
            sqlite3_finalize(stmt);
            if (!found) throw string{"not found"};
            return answ;
        }


};


// int main () {

//     // пока используются пароли вместо хешей
//     Database database;
//     try {
//         database.Create_db();
//         database.Create_table();
//         cout << database.Authorize("mks", "12345");
//         cout << database.Authorize("mks", "", "ty5");
//     } catch (const string err) {
//         cout << err;
//     }
//     //system("pause");
//     return 0;
// }
