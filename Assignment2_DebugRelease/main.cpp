#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Store each student's first and last names.
struct STUDENT_DATA
{
    std::string firstName;
    std::string lastName;
};

int main()
{
    std::vector<STUDENT_DATA> students;
    std::ifstream inputFile("StudentData.txt");

    // Stop if the input file cannot be opened.
    if (!inputFile.is_open())
    {
        std::cerr << "Could not open StudentData.txt\n";
        return 1;
    }

    std::string line;

    // Read one student from each line.
    while (std::getline(inputFile, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream row(line);
        STUDENT_DATA student;

        // The supplied file lists last name, then first name.
        if (!std::getline(row, student.lastName, ',') ||
            !std::getline(row, student.firstName))
        {
            std::cerr << "Invalid student record.\n";
            return 1;
        }

        // Remove the space after the comma and trailing whitespace.
        const std::size_t first = student.firstName.find_first_not_of(" \t\r");
        const std::size_t last = student.firstName.find_last_not_of(" \t\r");

        if (first == std::string::npos || student.lastName.empty())
        {
            std::cerr << "Missing student name.\n";
            return 1;
        }

        student.firstName = student.firstName.substr(first, last - first + 1);
        students.push_back(student);
    }

#ifdef _DEBUG
    // Display the stored students only in Debug builds.
    std::cout << "Students loaded: " << students.size() << "\n\n";

    for (const STUDENT_DATA& student : students)
    {
        std::cout << student.firstName << " "
            << student.lastName << '\n';
    }
#endif

    return 0;
}