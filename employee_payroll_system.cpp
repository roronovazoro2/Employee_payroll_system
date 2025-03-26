#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
using namespace std;

const string EMP_FILE = "employees.txt";
const string PAYROLL_FILE = "payroll.txt";

// ------------------ Employee Class ------------------
class Employee
{
public:
    int id;
    string name;
    string designation;
    string department;
    double basicSalary;
    double overtimeHours;
    double overtimeRate;
    double bonus;

    Employee(int id, string name, string designation, string department,
             double basicSalary, double overtimeHours, double overtimeRate, double bonus)
        : id(id), name(name), designation(designation), department(department),
          basicSalary(basicSalary), overtimeHours(overtimeHours), overtimeRate(overtimeRate), bonus(bonus) {}

    // Default constructor for file loading
    Employee() : id(0), basicSalary(0), overtimeHours(0), overtimeRate(0), bonus(0) {}

    // Salary components
    double getHRA() const { return basicSalary * 0.2; }  // 20% House Rent Allowance
    double getDA() const { return basicSalary * 0.1; }   // 10% Dearness Allowance
    double getTax() const { return basicSalary * 0.05; } // 5% Tax Deduction
    double getPF() const { return basicSalary * 0.02; }  // 2% Provident Fund
    double getOvertimePay() const { return overtimeHours * overtimeRate; }

    double calculateNetSalary() const
    {
        return basicSalary + getHRA() + getDA() + bonus + getOvertimePay() - getTax() - getPF();
    }

    void display() const
    {
        cout << left << setw(5) << id
             << setw(15) << name
             << setw(15) << designation
             << setw(15) << department
             << setw(12) << basicSalary
             << setw(10) << getHRA()
             << setw(10) << getDA()
             << setw(10) << bonus
             << setw(10) << getOvertimePay()
             << setw(10) << getTax()
             << setw(10) << getPF()
             << setw(12) << calculateNetSalary() << endl;
    }
};

// ------------------ Payroll System Class ------------------
class PayrollSystem
{
private:
    vector<Employee> employees;

public:
    PayrollSystem()
    {
        loadEmployees();
    }

    // File handling: Load employees from file
    void loadEmployees()
    {
        employees.clear();
        ifstream inFile(EMP_FILE);
        if (!inFile)
        {
            // File doesn't exist; no employees yet.
            return;
        }
        string line;
        while (getline(inFile, line))
        {
            if (line.empty())
                continue;
            stringstream ss(line);
            string token;
            Employee emp;
            // CSV Format: id,name,designation,department,basicSalary,overtimeHours,overtimeRate,bonus
            getline(ss, token, ',');
            emp.id = stoi(token);
            getline(ss, emp.name, ',');
            getline(ss, emp.designation, ',');
            getline(ss, emp.department, ',');
            getline(ss, token, ',');
            emp.basicSalary = stod(token);
            getline(ss, token, ',');
            emp.overtimeHours = stod(token);
            getline(ss, token, ',');
            emp.overtimeRate = stod(token);
            getline(ss, token, ',');
            emp.bonus = stod(token);
            employees.push_back(emp);
        }
        inFile.close();
    }

    // Save employees to file
    void saveEmployees()
    {
        ofstream outFile(EMP_FILE);
        if (!outFile)
        {
            cout << "Error: Unable to open file for writing!" << endl;
            return;
        }
        for (const auto &emp : employees)
        {
            outFile << emp.id << ","
                    << emp.name << ","
                    << emp.designation << ","
                    << emp.department << ","
                    << emp.basicSalary << ","
                    << emp.overtimeHours << ","
                    << emp.overtimeRate << ","
                    << emp.bonus << "\n";
        }
        outFile.close();
    }

    // Add a new employee
    void addEmployee()
    {
        int id;
        string name, designation, department;
        double basicSalary, overtimeHours, overtimeRate, bonus;
        cout << "Enter Employee ID: ";
        cin >> id;
        clearInputBuffer();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Designation: ";
        getline(cin, designation);
        cout << "Enter Department: ";
        getline(cin, department);
        cout << "Enter Basic Salary: ";
        cin >> basicSalary;
        cout << "Enter Overtime Hours: ";
        cin >> overtimeHours;
        cout << "Enter Overtime Rate: ";
        cin >> overtimeRate;
        cout << "Enter Bonus: ";
        cin >> bonus;
        employees.push_back(Employee(id, name, designation, department, basicSalary, overtimeHours, overtimeRate, bonus));
        saveEmployees();
        cout << "\nEmployee added successfully!\n";
    }

    // Update an existing employee record
    void updateEmployee()
    {
        int id;
        cout << "Enter Employee ID to update: ";
        cin >> id;
        clearInputBuffer();
        bool found = false;
        for (auto &emp : employees)
        {
            if (emp.id == id)
            {
                found = true;
                cout << "Updating record for " << emp.name << endl;
                cout << "Enter new Name (current: " << emp.name << "): ";
                getline(cin, emp.name);
                cout << "Enter new Designation (current: " << emp.designation << "): ";
                getline(cin, emp.designation);
                cout << "Enter new Department (current: " << emp.department << "): ";
                getline(cin, emp.department);
                cout << "Enter new Basic Salary (current: " << emp.basicSalary << "): ";
                cin >> emp.basicSalary;
                cout << "Enter new Overtime Hours (current: " << emp.overtimeHours << "): ";
                cin >> emp.overtimeHours;
                cout << "Enter new Overtime Rate (current: " << emp.overtimeRate << "): ";
                cin >> emp.overtimeRate;
                cout << "Enter new Bonus (current: " << emp.bonus << "): ";
                cin >> emp.bonus;
                saveEmployees();
                cout << "\nEmployee record updated successfully!\n";
                break;
            }
        }
        if (!found)
        {
            cout << "\nEmployee not found!\n";
        }
    }

    // Delete an employee record
    void deleteEmployee()
    {
        int id;
        cout << "Enter Employee ID to delete: ";
        cin >> id;
        clearInputBuffer();
        for (auto it = employees.begin(); it != employees.end(); ++it)
        {
            if (it->id == id)
            {
                cout << "Deleting record of " << it->name << endl;
                employees.erase(it);
                saveEmployees();
                cout << "\nEmployee deleted successfully!\n";
                return;
            }
        }
        cout << "\nEmployee not found!\n";
    }

    // View details of a specific employee
    void viewEmployee()
    {
        int id;
        cout << "Enter Employee ID to view: ";
        cin >> id;
        clearInputBuffer();
        bool found = false;
        for (const auto &emp : employees)
        {
            if (emp.id == id)
            {
                cout << "\nEmployee Details:\n";
                cout << "ID: " << emp.id << "\nName: " << emp.name
                     << "\nDesignation: " << emp.designation
                     << "\nDepartment: " << emp.department
                     << "\nBasic Salary: " << emp.basicSalary
                     << "\nOvertime Hours: " << emp.overtimeHours
                     << "\nOvertime Rate: " << emp.overtimeRate
                     << "\nBonus: " << emp.bonus
                     << "\nNet Salary: " << emp.calculateNetSalary() << "\n";
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "\nEmployee not found!\n";
        }
    }

    // List all employees in a tabular format
    void viewAllEmployees()
    {
        if (employees.empty())
        {
            cout << "\nNo employees to display!\n";
            return;
        }
        cout << "\n=================== EMPLOYEE LIST ===================\n";
        cout << left << setw(5) << "ID"
             << setw(15) << "Name"
             << setw(15) << "Designation"
             << setw(15) << "Department"
             << setw(12) << "BasicSalary"
             << setw(10) << "HRA"
             << setw(10) << "DA"
             << setw(10) << "Bonus"
             << setw(10) << "OT Pay"
             << setw(10) << "Tax"
             << setw(10) << "PF"
             << setw(12) << "NetSalary" << endl;
        cout << "---------------------------------------------------------------"
             << "------------------------------------------------\n";
        for (const auto &emp : employees)
        {
            emp.display();
        }
        cout << "===============================================================\n";
    }

    // Process payroll for a specific employee (generates payslip)
    void processPayroll()
    {
        int id;
        cout << "Enter Employee ID for payroll processing: ";
        cin >> id;
        clearInputBuffer();
        for (const auto &emp : employees)
        {
            if (emp.id == id)
            {
                double netSalary = emp.calculateNetSalary();
                cout << "\n----------------- PAYSLIP -----------------\n";
                cout << "Employee ID: " << emp.id << "\nName: " << emp.name << "\nDesignation: " << emp.designation
                     << "\nDepartment: " << emp.department << "\nBasic Salary: " << emp.basicSalary << endl;
                cout << "HRA: " << emp.getHRA() << "\nDA: " << emp.getDA() << "\nBonus: " << emp.bonus
                     << "\nOvertime Pay: " << emp.getOvertimePay() << endl;
                cout << "Tax Deduction: " << emp.getTax() << "\nPF Deduction: " << emp.getPF() << endl;
                cout << "Net Salary: " << netSalary << endl;
                cout << "--------------------------------------------\n";
                // Optionally, save payslip details to a payroll file.
                ofstream payrollOut(PAYROLL_FILE, ios::app);
                if (payrollOut)
                {
                    payrollOut << "Payslip for Employee ID " << emp.id << " (" << emp.name << "): Net Salary = " << netSalary << "\n";
                    payrollOut.close();
                }
                return;
            }
        }
        cout << "\nEmployee not found!\n";
    }

    // Generate reports: In this demo, we show employee-wise and department-wise summaries.
    void generateReports()
    {
        if (employees.empty())
        {
            cout << "\nNo employee data available for reports!\n";
            return;
        }
        cout << "\n=================== REPORTS ===================\n";
        cout << "\n1. Employee-wise Salary Report:\n";
        viewAllEmployees();

        // Department-wise summary (simple example)
        cout << "\n2. Department-wise Summary:\n";
        // Map department name to cumulative net salary and count of employees.
        // For simplicity, using a vector of pairs.
        vector<pair<string, pair<double, int>>> deptSummary;
        for (const auto &emp : employees)
        {
            bool found = false;
            for (auto &dept : deptSummary)
            {
                if (dept.first == emp.department)
                {
                    dept.second.first += emp.calculateNetSalary();
                    dept.second.second++;
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                deptSummary.push_back({emp.department, {emp.calculateNetSalary(), 1}});
            }
        }
        cout << left << setw(15) << "Department" << setw(20) << "Total Net Salary" << "Employee Count" << endl;
        for (const auto &dept : deptSummary)
        {
            cout << left << setw(15) << dept.first << setw(20) << dept.second.first << dept.second.second << endl;
        }
        cout << "\n===============================================\n";
    }
};

// ------------------ Utility Functions ------------------
void clearInputBuffer()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pause()
{
    cout << "\nPress Enter to continue...";
    cin.get();
}

// ------------------ Authentication ------------------
bool adminLogin()
{
    string password;
    cout << "Enter Admin Password: ";
    getline(cin, password);
    if (password == "admin123")
    {
        cout << "Admin login successful.\n";
        return true;
    }
    else
    {
        cout << "Incorrect password!\n";
        return false;
    }
}

void employeeMenu(PayrollSystem &ps)
{
    int empId;
    cout << "Enter your Employee ID: ";
    cin >> empId;
    clearInputBuffer();
    // For simplicity, we assume employee login by ID only.
    bool found = false;
    for (int i = 0; i < 3; i++)
    { // Allow up to 3 attempts to view details
        for (const auto &emp : ps.employees)
        {
            if (emp.id == empId)
            {
                cout << "\nWelcome, " << emp.name << "!\n";
                cout << "Your Payslip:\n";
                cout << "--------------------------------------------\n";
                cout << "Basic Salary: " << emp.basicSalary << "\nHRA: " << emp.getHRA()
                     << "\nDA: " << emp.getDA() << "\nBonus: " << emp.bonus
                     << "\nOvertime Pay: " << emp.getOvertimePay() << "\nTax: " << emp.getTax()
                     << "\nPF: " << emp.getPF() << "\nNet Salary: " << emp.calculateNetSalary() << "\n";
                cout << "--------------------------------------------\n";
                found = true;
                pause();
                return;
            }
        }
        if (!found)
        {
            cout << "Employee not found! Try again: ";
            cin >> empId;
            clearInputBuffer();
        }
    }
    if (!found)
    {
        cout << "Unable to authenticate Employee ID.\n";
    }
}

void adminMenu(PayrollSystem &ps)
{
    int choice;
    do
    {
        cout << "\n========= ADMIN MENU =========\n";
        cout << "1. Add Employee\n";
        cout << "2. Update Employee\n";
        cout << "3. Delete Employee\n";
        cout << "4. View Employee Details\n";
        cout << "5. View All Employees\n";
        cout << "6. Process Payroll (Generate Payslip)\n";
        cout << "7. Generate Reports\n";
        cout << "8. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        clearInputBuffer();
        switch (choice)
        {
        case 1:
            ps.addEmployee();
            break;
        case 2:
            ps.updateEmployee();
            break;
        case 3:
            ps.deleteEmployee();
            break;
        case 4:
            ps.viewEmployee();
            break;
        case 5:
            ps.viewAllEmployees();
            break;
        case 6:
            ps.processPayroll();
            break;
        case 7:
            ps.generateReports();
            break;
        case 8:
            cout << "Logging out...\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
        }
        pause();
    } while (choice != 8);
}

// ------------------ Main ------------------
int main()
{
    PayrollSystem ps;
    int choice;
    do
    {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Admin Login\n";
        cout << "2. Employee Login\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        clearInputBuffer();
        switch (choice)
        {
        case 1:
            if (adminLogin())
            {
                adminMenu(ps);
            }
            break;
        case 2:
            employeeMenu(ps);
            break;
        case 3:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 3);
    return 0;
}
