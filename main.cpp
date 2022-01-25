#include <iostream>
#include <occi.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;
using namespace oracle::occi;

// Read Oracle password to establish connection
string readPassword()
{
    struct termios settings;
    tcgetattr( STDIN_FILENO, &settings );
    settings.c_lflag =  (settings.c_lflag & ~(ECHO));
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );

    string password = "";
    getline(cin, password);

    settings.c_lflag = (settings.c_lflag |   ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );
    return password;
}

// Give each prepared statement a name
struct STMT {
    string name;
    Statement *stmt;
};

// Prepare the statements that would be used repeatedly
// in this program
int initStatements(Connection *conn, STMT * & statements)
{
    int size = 4;
    statements = new STMT[size];

    // shows all the employees
    statements[0].name = "show employee";
    string queryStr = "select * from employees";
    statements[0].stmt = conn->createStatement(queryStr);

    // shows all the doctors who are attending patients whose age is greater than 10
    statements[1].name = "displayRecords";
    queryStr = "select referring_doctor from patients where age>10";
    statements[1].stmt = conn->createStatement(queryStr);

    //shows all the patients information whose id="123"
    statements[2].name = "showpatient";
    queryStr = "select * from patients where p_id='123' ";
    statements[2].stmt = conn->createStatement(queryStr);

    //deleting a room number
    statements[3].name = "deleterecord";
    queryStr = "delete from rooms where r_no= :1 ";
    statements[3].stmt = conn->createStatement(queryStr);
	
    //insert
    statements[4].name = "insertrecord";
    queryStr = "insert into rooms VALUES (:1 ,:2 , :3) ";
    statements[4].stmt = conn->createStatement(queryStr);
	


    
    return size;
}

// Given the name, find the corresponding prepared sql statement
Statement * findStatement(string name, STMT *statements, int size)
{
    for(int i = 0; i < size; i++) {
        if (statements[i].name == name)
            return statements[i].stmt;
    }
    return 0;
}

// Terminate all the prepared statements
void terminateStatements(Connection *conn, STMT *statements, int size)
{
    for(int i = 0; i < size; i++)
        conn->terminateStatement(statements[i].stmt);
}





// display all employees
void displayEmployees(Connection *conn,STMT *statements, int size)
{
 
 Statement *stmt;
 
  stmt = findStatement("show employee", statements, size);

	ResultSet *rs2 = stmt -> executeQuery();
	while(rs2 ->next() ){
	string EID = rs2 ->getString(1);
	string name= rs2 ->getString(2);
	string address = rs2 ->getString(3);
	string email = rs2 ->getString(4);
	string age = rs2 ->getString(5);
	string contact= rs2 ->getString(6);
	cout<< "EID:" << EID << endl;
	cout<< "NAME:" << name << endl;
	cout<< "ADDRESS:" << address << endl;
	cout<< "EMAIL:" << email<< endl;
	cout<< "AGE:" << age << endl;
	cout<< "CONTACT:" << contact << endl;

	conn-> commit();
}
}

void displayRecords(Connection *conn,STMT *statements, int size)
{
 
 Statement *stmt;
 
  stmt = findStatement("displayRecords", statements, size);

	ResultSet *rs = stmt -> executeQuery();
	while(rs ->next() ){
	//string PID = rs ->getString(1);
	//string name = rs ->getString(2);
	string referring_doctor = rs ->getString(1);
	//string age = rs ->getString(4);
	cout<< "DOCTOR:  " << referring_doctor << endl;

	conn-> commit();
}
}
void showpatient(Connection *conn,STMT *statements, int size)
{
 
 Statement *stmt;
 
  stmt = findStatement("showpatient", statements, size);

	ResultSet *rs1 = stmt -> executeQuery();
	while(rs1 ->next() ){
	string PID = rs1 ->getString(1);
	string name = rs1 ->getString(2);
	string referring_doctor = rs1 ->getString(3);
	string age = rs1 ->getString(4);
	cout<< "PID:  " << PID<< endl;
	cout<< "NAME:  " << name << endl;
	cout<< "DOCTOR:  " << referring_doctor << endl;
	cout<< "AGE:  " << age << endl;

	conn-> commit();
}
}
void deleterecord(Connection *conn,STMT *statements, int size){
cout<<"cant do"<<endl;
}

void insertrecord(Connection *conn, STMT *statements, int size)
{
	 Statement *stmt;
	 string room;
	string floor;
	string eid;

	 cout <<"Please enter room ";
	 cin >> room;
	 cout <<endl;
		
     cout <<"Please enter floor";
	 cin >> floor;
	 cout <<endl;
		
	 cout <<"Please enter eid ";
	 cin >> eid;
	 cout <<endl;
		
	 stmt = findStatement("insertrecord", statements, size);
	
	
	 stmt->setString(1, room);
	 stmt->setString(2, floor);
	 stmt->setString(3, eid);
	 
	 ResultSet *rs = stmt->executeQuery();

	 cout <<"room: "<<room<<" Inserted."<<endl;
	 stmt->closeResultSet(rs);
    

	 conn->commit();


}

// menu
void printmenu()
{
  cout << "Enter A to show all employees," << endl;
  cout << "   or B to show all the referring doctors of the patients of age>10," << endl;
  cout << "   or C to show all the data of the patient with id '123' " << endl;
  cout<<"     or D to delete a room number"<<endl;
  cout << "   or Q to quit" << endl;
}

// get command from user
char getcommand()
{
  cout << "Enter your command choice (A,B,C,D,Q)" << endl;
  char cmd;
  cin >> cmd;
  cmd = toupper(cmd);
  switch (cmd) {
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'Q':
    return cmd;
  default:  cout << "You have entered an invalid command" << endl;
    return getcommand();
  }
}
// proccess command
bool command(Connection *conn, STMT *statements, int size, char cmd)
{
	switch(cmd){
		case 'A':
		  displayEmployees( conn, statements, size);
		  break;
		case 'B':
		  displayRecords( conn, statements, size);
		  break;
		case 'C':
		  showpatient( conn, statements, size);
		  break;
		case 'D':
		  deleterecord( conn, statements, size);
		  break;
		case 'E':
		  insertrecord( conn, statements, size);
		  break;
		case 'S':
		  printmenu();
		break;

		default:  return false;
	}
	return true;
		
}

// run
void RunProgram(Connection *conn, STMT *statements, int size)
{

  printmenu();
  char cmd = getcommand();
  while (command( conn,statements, size, cmd)) {
    cout << endl;
    cmd = getcommand();
  }

}

int main()
{
    string userName;
    string password;
    const string connectString = "sunfire.csci.viu.ca";

    // Establish connection using userName and password
    cout << "Your user name: ";
    getline(cin, userName);

    cout << "Your password: ";
    password = readPassword();
    cout << endl;

    Environment *env = Environment::createEnvironment();
    Connection *conn = env->createConnection(userName, password, connectString);

    STMT *statements;
    int size = initStatements(conn, statements);

   
    
   
 	 RunProgram(conn, statements, size);
	 

 
    // clean up environment before terminating
    terminateStatements(conn, statements, size);
    env->terminateConnection(conn);
    Environment::terminateEnvironment(env);

    return 0;
} 
