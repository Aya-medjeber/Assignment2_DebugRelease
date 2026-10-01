#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Store the information for each student.
struct STUDENT_DATA
{
    std::string firstName;
    std::string lastName;

#ifdef PRE_RELEASE
    std::string email;
#endif
};

// Remove spaces and line-ending characters around a field.
std::string trim(const std::string& text)
{
    const std::size_t first = text.find_first_not_of(" \t\r\n");

    if (first == std::string::npos)
    {
        return "";
    }

    const std::size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

int main()
{
    std::vector<STUDENT_DATA> students;

#ifdef _DEBUG
    std::cout << "Build mode: DEBUG\n";
#else
    std::cout << "Build mode: RELEASE\n";
#endif

    // Include the email functionality only in pre-release builds.
#ifdef PRE_RELEASE
    std::cout << "Version: Pre-Release\n\n";
    const std::string fileName = "StudentData_Emails.txt";
#else
    std::cout << "Version: Standard\n\n";
    const std::string fileName = "StudentData.txt";
#endif

    std::ifstream inputFile(fileName);

    if (!inputFile.is_open())
    {
        std::cerr << "Could not open " << fileName << '\n';
        return 1;
    }

    std::string line;

    // Read the selected file and store each student in the vector.
    while (std::getline(inputFile, line))
    {
        if (trim(line).empty())
        {
            continue;
        }

        std::stringstream row(line);
        STUDENT_DATA student;

        // Both supplied files list the last name first.
        if (!std::getline(row, student.lastName, ','))
        {
            std::cerr << "Invalid student record.\n";
            return 1;
        }

#ifdef PRE_RELEASE
        // Pre-release records also contain an email address.
        if (!std::getline(row, student.firstName, ',') ||
            !std::getline(row, student.email))
        {
            std::cerr << "Invalid pre-release student record.\n";
            return 1;
        }

        student.email = trim(student.email);

        if (student.email.empty())
        {
            std::cerr << "Missing student email.\n";
            return 1;
        }
#else
        if (!std::getline(row, student.firstName))
        {
            std::cerr << "Invalid student record.\n";
            return 1;
        }
#endif

        student.firstName = trim(student.firstName);
        student.lastName = trim(student.lastName);

        if (student.firstName.empty() || student.lastName.empty())
        {
            std::cerr << "Missing student name.\n";
            return 1;
        }

        students.push_back(student);
    }

    // Detect a file-reading error before displaying results.
    if (inputFile.bad())
    {
        std::cerr << "Error reading " << fileName << '\n';
        return 1;
    }

#ifdef _DEBUG
    // Student information is displayed only in Debug builds.
    std::cout << "Students loaded: " << students.size() << "\n\n";

    for (const STUDENT_DATA& student : students)
    {
        std::cout << student.firstName << " "
            << student.lastName;

#ifdef PRE_RELEASE
        std::cout << " | " << student.email;
#endif

        std::cout << '\n';
    }
#endif

    return 0;
}