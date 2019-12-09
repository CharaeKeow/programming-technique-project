#include <iostream>
#include <string>
#include <fstream>
using namespace std;
const int SIZE = 50; //max size of the array, and also the number of item that can be read from the file
//We assume that the maximum number of students in a class is 50.

void calculateInstructorAverage(const double averageScore[5]);
int displayMenu(); //display menu
void enterData(char[5], int part); //enter new data into file. Overwrite current data in file
void appendData(char[5], int part); //append data to the end of the file. Old data remains
void getUserInput(ofstream&, int, char[5]); //to obtain and validate the students' scores entered by user
void countParticipant(int& participant); //count the number of participant in the file
void readData(string[][7], int& instructorcode2, int& coursecode2, int participant); //read and copy data of file into array
void findAverage(const string[50][7], int instructorcode2, int coursecode2, int participant, double[5]); //find the average marks
void displayDataInFile(); //display data in file

int main() {
	int instructorCode = 0, courseCode = 0, part = 0, participant = 0;
	char question[5]; //The five questions
	string evaluation[10][7]; //Used to set the 
	double averageScore[5] = {0, 0, 0, 0, 0}; //to hold the average score of each question, which will be used to calculate Instructor average
	int count; //For user's input for menu selection
	
	do {
		//display menu
		count = displayMenu(); 

		//Validate the input
		if (count < 1 || count > 5) {
			cout << "Invalid input! Please enter a valid number." << endl;
			count = displayMenu(); //recursion
		}

		//Simple switch to process user's input
		switch (count)
		{
		case 1: //enter new data
			enterData(question, part);
			break;
		case 2: //append data
			appendData(question, part);
			break;
		case 3:
			/*
			Here, we count how many participants are in the file first.
			Then we copy the data in the file into the array evaluation (which hold max 50 row)
			After that, we iterate the array to count the average score
			*/
			countParticipant(participant);
			readData(evaluation, instructorCode, courseCode, participant);
			findAverage(evaluation, instructorCode, courseCode, participant, averageScore);
			calculateInstructorAverage(averageScore);
			break;
		case 4: //display data in the file
			displayDataInFile();
			break;
		case 5: //exit program
			cout << "Program terminated! Have a nice day. Bye!" << endl;
			break;
		}
	} while (count != 5);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 7; j++) {
			cout << evaluation[i][j] << " ";
		}
		cout << endl;
	}
	
	return 0;
}

//This function will return an integer count, which will be used
//to navigate the menu
int displayMenu() {
	int count;
	cout << "1.Enter new data into file." << endl;
	cout << "2.Enter data with current data." << endl;
	cout << "3.Find average from data saved in the file." << endl;
	cout << "4. Display data in file." << endl;
	cout << "5. Exit" << endl;
	cin >> count;
	return count;
}

void enterData(char Question[5], int part) {
	ofstream outFile("evaluationSummary.txt");
	cout << "enter number of participant";
	//number of participant that user wanna enter. Note that this variable will be used
	//to determine how many time the for loop will iterate only, as this value will be reset
	//once user close the program, hence making it unsuitable for calculating the average
	cin >> part; 
	getUserInput(outFile, part, Question); 
}

void appendData(char Question[5], int part) {
	ofstream outFile("evaluationSummary.txt", ios::app); //append mode
	cout << "Enter number of participant";
	cin >> part;
	getUserInput(outFile, part, Question);
}

/*As we will ask for user input 2 times in enterData() and appendData(), we define a function
 *that will ask for user input and save it in the file to prevent us from repeating that task
*/
void getUserInput(ofstream& outFile, int part, char Question[5]) {
	int instructorNum, courseCode;
	for (int j = 0; j < part; j++) {
		cout << "Enter the instructor code, course code and the answers of 5 questions: ";
		cin >> instructorNum >> courseCode;
		for (int i = 0; i < 5; i++) {
			cin >> Question[i]; //Enter the students' answer into the array
		}
		//This validate that the students' answers enterd are correct. If not, ask the user to enter the right
		//score again
		for (int i = 0; i < 5; i++) {
			while (!(Question[i] == 'A' || Question[i] == 'a' || Question[i]
				== 'U' || Question[i] == 'u' || Question[i] == 'P' || Question[i] == 'p' ||
				Question[i] == 'G' || Question[i] == 'g' || Question[i] == 'E' || Question[i] == 'e'))
			{
				cout << "You have entered other than provided ansewes for question:" << (i + 1) << "\nEnter again";
				cin >> Question[i];
			}
		}
		//Write those input into the file
		outFile << endl << instructorNum << " " << courseCode << " " << Question[0] << " " << Question[1] << " " << Question[2]
			<< " " << Question[3] << " " << Question[4];
	}
}

void countParticipant(int& participant) {
	//temporary dumb variable to act like we copy data from file line by line, so that we can increment the variable participant
	string temp; 
	participant = 0; //reset participant number, just in case

	ifstream inFile("evaluationSummary.txt");
	if (!inFile) {
		cerr << "Error finding input file!" << endl;
		exit(1);
	}
	while (!inFile.eof()) {
		getline(inFile, temp); //read file line by line, then copy into temp
		participant++; //increment
	}
}

void readData(string arr[][7], int& instructorcode2, int& coursecode2, int participant) {
	ifstream inFile("evaluationSummary.txt");
	if (!inFile) {
		cerr << "Error finding input file" << endl;
		exit(1);
	}
	inFile >> instructorcode2 >> coursecode2; //copy instructor code and course code

	/*Then iterate the file line by line, and copy the data into the 2D array
	*/
	while (!inFile.eof()) {
		for (int i = 0; i < participant; i++) { //we know the number of participant, so use that to control the loop
			for (int j = 0; j < 7; j++) {
				inFile >> arr[i][j]; //copy
			}
		}
	}

	/* Why do we copy the Instructor number and Course code only once? Because we assume that the data input from
	   user is only from a class containing data from an instructor that teaches only one course.
	*/
}

void findAverage(const string evaluation[50][7], int instructorcode2, int coursecode2, int participant, double averageScore[5]) {
	double total = 0;
	int value = 0, count = 0;
	double average = 0;
	cout << "Instructor Number: " << instructorcode2 << "\nCourse Number: " <<
		coursecode2 << "\nNumber of Participant: " << participant << endl;
	cout << participant << endl;
	for (int j = 2; j < 7; j++) {
		for (int i = 0; i < participant; i++) {
			if (evaluation[i][j] == "U" || evaluation[i][j] == "u") {
				value = 0;
			}
			else if (evaluation[i][j] == "P" || evaluation[i][j] == "p") {
				value = 1;
			}
			else if (evaluation[i][j] == "A" || evaluation[i][j] == "a") {
				value = 2;
			}
			else if (evaluation[i][j] == "G" || evaluation[i][j] == "g") {
				value = 3;
			}
			else if (evaluation[i][j] == "E" || evaluation[i][j] == "e") {
				value = 4;
			}
			total += value;
		}
		count++;
		average = total / participant;
		cout << "Total " << count << ": " << total << endl;
		cout << "Average for Question " << count << ": " << average << endl;

		//Store the average into averageScore array
		averageScore[count] = average;
		total = 0;
	}
}

void calculateInstructorAverage(const double averageScore[5]) {
	double instructorAverage = 0;
	instructorAverage = (averageScore[0] * 0.15) + (averageScore[1] * 0.23) + (averageScore[2] * 0.08) + (averageScore[3] * 0.32) + (averageScore[4] * 0.22);
	cout << "Instructor average: " << instructorAverage << endl;
}

void displayDataInFile() {
	ifstream inFile("evaluationSummary.txt");
	string arr[7];

	if (!inFile) { //if file not found
		cerr << "File not found!";
		exit(0);
	}

	while (!inFile.eof()) {
		inFile >> arr[0] >> arr[1] >> arr[2] >> arr[3] >> arr[4] >> arr[5] >> arr[6]; //put data into empty array
		cout << arr[0] << " " << arr[1] << " " << arr[2] << " " << arr[3] << " " << 
			arr[4] << " " << arr[5] << " " << arr[6] << endl; //c them out 
	}
}
