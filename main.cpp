#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

const string subjects[6] = {"Design Analysis and Algorithm", "OOPS and ADS", "Operating Systems", "Computer Architecture", "Environmental Science", "HSMC"};


class User {
public:
    string username;
    string password;
    string user_type;

    User(string uname, string pwd, string type) : username(uname), password(pwd), user_type(type) {}
};

bool isUserRegistered(const string& username) {
    ifstream inFile("users.txt");
    string uname, pwd, type;
    if (inFile.is_open()) {
        while (inFile >> uname >> pwd >> type) {
            if (uname == username) {
                inFile.close();
                return true;
            }
        }
        inFile.close();
    } else {
        cerr << "Unable to open file for reading." << endl;
    }
    return false;
}

bool registerUser(const User& user) {
    if (isUserRegistered(user.username)) {
        cout << "User " << user.username << " is already registered." << endl;
        return false;
    }

    ofstream outFile("users.txt", ios::app);
    if (outFile.is_open()) {
        outFile << user.username << " " << user.password << " " << user.user_type << endl;
        outFile.close();
        cout << "User " << user.username << " registered successfully as " << user.user_type << "." << endl;
        return true;
    } else {
        cerr << "Unable to open file for writing." << endl;
        return false;
    }
}

User* authenticate(const string& username, const string& password) {
    ifstream inFile("users.txt");
    string uname, pwd, type;
    if (inFile.is_open()) {
        while (inFile >> uname >> pwd >> type) {
            if (uname == username && pwd == password) {
                inFile.close();
                return new User(uname, pwd, type);
            }
        }
        inFile.close();
    } else {
        cerr << "Unable to open file for reading." << endl;
    }
    return nullptr;
}

class AVLNode {
public:
    int rollNo;
    string name;
    int totalMarks;
    double gpa;
    int rank;
    string grades[6];
    int height;
    int marks[6];
    AVLNode* left;
    AVLNode* right;

    AVLNode(int roll, string n, int* m)
        : rollNo(roll), name(n), rank(0), gpa(0.0), height(1), left(nullptr), right(nullptr) {
        totalMarks = 0;
        for (int i = 0; i < 6; ++i) {
            marks[i] = m[i];
            totalMarks += m[i];
            grades[i] = "RA";
        }
    }
};

class AVLTree {
public:
    AVLNode* root;

    AVLTree() : root(nullptr) {}

    int height(AVLNode* node) {
        return node ? node->height : 0;
    }

    int getBalance(AVLNode* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }

    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;
        return y;
    }

    AVLNode* insert(AVLNode* node, int rollNo, string name, int* marks) {
        if (!node) return new AVLNode(rollNo, name, marks);

        if (rollNo < node->rollNo) {
            node->left = insert(node->left, rollNo, name, marks);
        } else if (rollNo > node->rollNo) {
            node->right = insert(node->right, rollNo, name, marks);
        } else {
            node->name = name;
            for (int i = 0; i < 6; ++i) {
                node->marks[i] = marks[i];
            }
            node->totalMarks = (marks[0] + marks[1] + marks[2] + marks[3] + marks[4]);
            return node;
        }

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        if (balance > 1 && rollNo < node->left->rollNo) return rightRotate(node);
        if (balance < -1 && rollNo > node->right->rollNo) return leftRotate(node);
        if (balance > 1 && rollNo > node->left->rollNo) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && rollNo < node->right->rollNo) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    AVLNode* minValueNode(AVLNode* node) {
        AVLNode* current = node;
        while (current->left) current = current->left;
        return current;
    }

    AVLNode* deleteNode(AVLNode* root, int rollNo) {
        if (!root) return root;

        if (rollNo < root->rollNo) {
            root->left = deleteNode(root->left, rollNo);
        } else if (rollNo > root->rollNo) {
            root->right = deleteNode(root->right, rollNo);
        } else {
            if (!root->left || !root->right) {
                AVLNode* temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }
                delete temp;
            } else {
                AVLNode* temp = minValueNode(root->right);
                root->rollNo = temp->rollNo;
                root->name = temp->name;
                for (int i = 0; i < 5; ++i) {
                    root->marks[i] = temp->marks[i];
                }
                root->totalMarks = temp->totalMarks;
                root->right = deleteNode(root->right, temp->rollNo);
            }
        }

        if (!root) return root;

        root->height = 1 + max(height(root->left), height(root->right));

        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    AVLNode* search(AVLNode* root, int rollNo) {
        if (!root || root->rollNo == rollNo) return root;
        if (rollNo < root->rollNo) return search(root->left, rollNo);
        return search(root->right, rollNo);
    }

    void inOrder(AVLNode* root, vector<AVLNode*>& nodes) {
        if (root) {
            inOrder(root->left, nodes);
            nodes.push_back(root);
            inOrder(root->right, nodes);
        }
    }
};

class Gradebook {
public:
    AVLTree tree;
    map<string, int> credits = {{"Design Analysis and Algorithm", 3}, {"OOPS and ADS", 3}, {"Operating Systems", 3}, {"Computer Architecture", 3}, {"Environmental Science", 3}, {"HSMC", 3}};
    map<string, double> gradePoints = {{"O", 10.0}, {"A+", 9.0}, {"A", 8.0}, {"B+", 7.0}, {"B", 6.0}, {"C", 5.0}, {"RA", 0.0}};

private:
    string calculateGrade(int mark) {
        if (mark >= 90) return "O";
        if (mark >= 80) return "A+";
        if (mark >= 70) return "A";
        if (mark >= 60) return "B+";
        if (mark >= 50) return "B";
        if (mark >= 40) return "C";
        return "RA";
    }

        double calculateGPA(AVLNode* student) {
        double totalGradePoints = 0;
        int totalCredits = 0;
        string subjects[6] = {"Design Analysis and Algorithm", "OOPS and ADS", "Operating Systems", "Computer Architecture", "Environmental Science", "HSMC"};
        string grade;

        for (int i = 0; i < 6; ++i) {
            grade = calculateGrade(student->marks[i]);
            totalGradePoints += gradePoints[grade] * credits[subjects[i]];
            totalCredits += credits[subjects[i]];
        }

        return totalGradePoints / totalCredits;
    }

    void updateStudentRankings() {
        vector<AVLNode*> students;
        tree.inOrder(tree.root, students);
        sort(students.begin(), students.end(), [](AVLNode* a, AVLNode* b) {
            return a->totalMarks > b->totalMarks;
        });

        for (size_t i = 0; i < students.size(); ++i) {
            students[i]->rank = i + 1;
        }
    }

public:
    void addStudent(int rollNo, string name, int marks[6]) {
        tree.root = tree.insert(tree.root, rollNo, name, marks);
        AVLNode* student = tree.search(tree.root, rollNo);
        if (student) {
            student->gpa = calculateGPA(student);
            updateStudentRankings();
        }
    }

    void removeStudent(int rollNo) {
        tree.root = tree.deleteNode(tree.root, rollNo);
        updateStudentRankings();
    }

    void searchStudent(int rollNo) {
        AVLNode* student = tree.search(tree.root, rollNo);
        if (student) {
            printStudentTable(student);	
		} else {
            cout << "Student with Roll No: " << rollNo << " not found." << endl;
        }
    }
    
    void updateStudentSubjectMark(int rollNo, int subjectIndex, int newMark) {
    AVLNode* student = tree.search(tree.root, rollNo);
    if (student) {
        student->marks[subjectIndex] = newMark;
        student->totalMarks = 0;
        for (int i = 0; i < 6; ++i) {
            student->totalMarks += student->marks[i];
        }
        student->gpa = calculateGPA(student);
        updateStudentRankings();
        cout << "Marks updated successfully for Roll No: " << rollNo << endl;
    } else {
        cout << "Student with Roll No: " << rollNo << " not found." << endl;
    }
	}
	void printStudentTable(AVLNode* student) {
    if (student) {
        const string subjects[6] = {"Design Analysis and Algorithm", "OOPS and ADS", "Operating Systems", "Computer Architecture", "Environmental Science", "HSMC"};
        cout << "---------------------------------------------------------------\n";
        cout << "| Roll No: " << setw(50) << left << student->rollNo << " |\n";
        cout << "| Name: " << setw(54) << left << student->name << "|\n";
        cout << "---------------------------------------------------------------\n";
        cout << "| Subject                      | Marks     | Grade |\n";
        cout << "---------------------------------------------------------------\n";
        for (int i = 0; i < 6; ++i) {
            string grade = calculateGrade(student->marks[i]);
            cout << "| " << setw(29) << left << subjects[i] << "| "
                 << setw(10) << left << student->marks[i] << "| "
                 << setw(6) << left << grade << "|\n";
        }
        cout << "---------------------------------------------------------------\n";
        cout << "| GPA: " << setw(54) << left << student->gpa << "|\n";
        cout << "| Rank: " << setw(53) << left << student->rank << "|\n";
        cout << "---------------------------------------------------------------\n";
    } else {
        cout << "Student not found." << endl;
    }
	}
    void displayAllStudents() {
        vector<AVLNode*> students;
	    tree.inOrder(tree.root, students);
	    cout << left << setw(13) << "Roll No" 
	         << setw(20) << "Name" 
	         << setw(10) << "GPA" 
	         << setw(10) << "Rank" << endl;
	    cout << "-------------------------------------------------------------\n";
	    for (auto& student : students) {
	        cout << setw(13) << student->rollNo 
	             << setw(20) << student->name 
	             << setw(10) << student->gpa 
	             << setw(10) << student->rank << endl;
	    }
    }

    void printStudentInfo(AVLNode* node) {
        if (node) {
            cout << "Roll No: " << node->rollNo << ", Name: " << node->name
                 << ", Rank: " << node->rank
                 << ", GPA: " << node->gpa << endl;
        } else {
            cout << "Student not found." << endl;
        }
    }

    void printAllStudents() {
        vector<AVLNode*> nodes;
        tree.inOrder(tree.root, nodes);
        sort(nodes.begin(), nodes.end(), [](AVLNode* a, AVLNode* b) {
            return a->rank < b->rank;
        });
        for (auto& node : nodes) {
            printStudentInfo(node);
        }
    }

   
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Unable to open file: " << filename << endl;
            return;
        }

        int rollNo;
        string name;
        int marks[6];

        while (file >> rollNo >> name >> marks[0] >> marks[1] >> marks[2] >> marks[3] >> marks[4] >> marks[5]) {
            addStudent(rollNo, name, marks);
        }

        file.close();
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (!file) {
            cerr << "Unable to open file: " << filename << endl;
            return;
        }

        vector<AVLNode*> students;
        tree.inOrder(tree.root, students);
        for (auto& student : students) {
            file << student->rollNo << " " << student->name << " ";
            for (int i = 0; i < 6; ++i) {
                file << student->marks[i] << " ";
            }
            file << endl;
        }

        file.close();
    }
};

int main() {
    Gradebook gradebook;
    gradebook.loadFromFile("gradebook.txt");

    while (true) {
        cout << "1. Register\n2. Login\n3. Exit\n";
        cout << "Enter your Choice : ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            string username, password, user_type;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            cout << "Enter user type (student/staff): ";
            cin >> user_type;

            User newUser(username, password, user_type);
            registerUser(newUser);
        } else if (choice == 2) {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            User* user = authenticate(username, password);
            if (user) {
                cout << "Welcome " << user->username << "! You are logged in as " << user->user_type << "." << endl;

                if (user->user_type == "staff") {
                    while (true) {
                        cout << "1. Add Student\n2. Remove Student\n3. Display All Students\n4. Search Student\n5. Display by Rank\n6. Update Student\n7. Logout\n";
						cout << "Enter your Choice : ";
						int staffChoice;
                        cin >> staffChoice;

                        if (staffChoice == 1) {
                            int rollNo, marks[5];
                            string name;
                            cout << "Enter Roll No: ";
                            cin >> rollNo;
                            cout << "Enter Name: ";
                            cin >> name;
                            cout << "Enter all 6 Marks: \n";
                            for (int i = 0; i < 6; ++i) {
                            	cout << "Enter Total Marks for " << subjects[i] << " : ";
                                cin >> marks[i];
                            }
                            gradebook.addStudent(rollNo, name, marks);
                        } else if (staffChoice == 2) {
                            int rollNo;
                            cout << "Enter Roll No: ";
                            cin >> rollNo;
                            gradebook.removeStudent(rollNo);
                        } else if (staffChoice == 3) {
                            gradebook.displayAllStudents();
                        } else if (staffChoice == 4) {
                            int rollNo;
                            cout << "Enter Roll No: ";
                            cin >> rollNo;
                            gradebook.searchStudent(rollNo);
                        } else if (staffChoice == 5) {
                            cout << "\nStudents sorted by rank:\n";
                            gradebook.printAllStudents();
                        } else if (staffChoice == 6) {  // Update Student Marks option
					            int rollNo, subjectIndex, newMark;
					            cout << "Enter Roll No: ";
					            cin >> rollNo;
					            cout << "Enter Subject Index (0 : Design Analysis and Algorithm,\n 1 : OOPS and ADS,\n 2 : Operating Systems,\n 3 : Computer Architecture,\n 4 : Environmental Science,\n 5 : HSMC): ";
					            cin >> subjectIndex;
					            if (subjectIndex < 0 || subjectIndex > 4) {
					                cout << "Invalid subject index. Please enter a value between 0 and 5." << endl;
					                continue;
					            }
					            cout << "Enter Updated Mark: ";
					            cin >> newMark;
					            gradebook.updateStudentSubjectMark(rollNo, subjectIndex, newMark);
					    }else if (staffChoice == 7) {
                            delete user;
                            break;
                        }
                    }
                } else if (user->user_type == "student") {
                    while (true) {
                        cout << "Enter 1 to view your details or 0 to logout : ";
                        int studentChoice;
                        cin >> studentChoice;

                        if (studentChoice == 1) {
                            int rollNo;
                            cout << "Enter your Roll No: ";
                            cin >> rollNo;
                            gradebook.searchStudent(rollNo);
                        } else if (studentChoice == 0) {
                            delete user;
                            break;
                        }
                    }
                }
            } else {
                cout << "Invalid credentials. Please try again." << endl;
            }
        } else if (choice == 3) {
            gradebook.saveToFile("gradebook.txt");
            break;
        }
    }

    return 0;
}



