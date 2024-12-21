#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Library for character handling functions

// Function prototypes
int romanToDecimal(char *roman);
int performOperation(int num1, int num2, char operation); // this will perform arithmetic operations
void numberToWords(int num, char *output);
void processInputFile(const char *inputFile, const char *outputFile); // Process input and output files
void processInputFileWithLogging(const char *inputFile, const char *outputFile); // Process input and output files with logging
void writeOutput(FILE *outputFile, int result);
int isValidRoman(char *roman);
void cleanString(char *str); // Clean input string from unwanted characters

// Main function
int main() {
    const char *inputFile = "Input.txt";
    const char *outputFile = "Output.txt";

    // Call the function to process the input file and generate output
    processInputFileWithLogging(inputFile, outputFile);

    printf("Processing completed! Check %s for results.\n", outputFile);
    return 0;
}

// Function definitions

// Convert Roman numeral to decimal
int romanToDecimal(char *roman) {
    int total = 0, current = 0, next = 0;
    int length = strlen(roman);

    for (int i = 0; i < length; i++) { // Loop through each character of the Roman numeral
        switch (toupper(roman[i])) {
            case 'I': current = 1; break;
            case 'V': current = 5; break;
            case 'X': current = 10; break;
            case 'L': current = 50; break;
            case 'C': current = 100; break;
            case 'D': current = 500; break;
            case 'M': current = 1000; break;
            default: return -1;
        }

        // Check the value of the next Roman numeral character
        if (i + 1 < length) {
            switch (toupper(roman[i + 1])) {
                case 'I': next = 1; break;
                case 'V': next = 5; break;
                case 'X': next = 10; break;
                case 'L': next = 50; break;
                case 'C': next = 100; break;
                case 'D': next = 500; break;
                case 'M': next = 1000; break;
                default: next = 0;
            }
        } else {
            next = 0;
        }

        if (current < next) {
            total -= current;
        } else {
            total += current;
        }
    }
    return total;
}

// Perform arithmetic operations
int performOperation(int num1, int num2, char operation) {
    switch (operation) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return (num2 != 0) ? num1 / num2 : -1;
        default: return -1;
    }
}

// Convert number to words
void numberToWords(int num, char *output) {
    // Arrays for units, tens, teens, and thousands place names
    const char *units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const char *teens[] = {"Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char *tens[] = {"", "Ten", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    if (num == 0) {
        strcpy(output, "Zero");
        return;
    }

    char buffer[256] = "";   // Buffer to store words as we build them
    char temp[256] = "";      // Temporary buffer for constructing words
    int isNegative = (num < 0);   // Check if the number is negative
    num = abs(num);   // Convert number to positive for processing

    int chunk, place = 0;

    while (num > 0) {
        chunk = num % 1000;

        if (chunk > 0) {
            char chunkWords[256] = "";

            if (chunk / 100 > 0) {
                strcat(chunkWords, units[chunk / 100]);
                strcat(chunkWords, " Hundred ");
            }

            int remainder = chunk % 100;
            if (remainder > 10 && remainder < 20) {
                strcat(chunkWords, teens[remainder - 11]);
            } else {
                if (remainder / 10 > 0) {
                    strcat(chunkWords, tens[remainder / 10]);
                    strcat(chunkWords, " ");
                }
                if (remainder % 10 > 0) {
                    strcat(chunkWords, units[remainder % 10]);
                }
            }

            if (place > 0) {
                strcat(chunkWords, " ");
                strcat(chunkWords, thousands[place]);
            }

            snprintf(temp, sizeof(temp), "%s %s", chunkWords, buffer);
            strcpy(buffer, temp);
        }

        num /= 1000;
        place++;
    }

    snprintf(output, 256, "%s%s", isNegative ? "Negative " : "", buffer);  // Add "Negative" if necessary
    output[strcspn(output, "\n")] = '\0';
}

// Process input file
void processInputFile(const char *inputFile, const char *outputFile) {
    FILE *input = fopen(inputFile, "r");
    FILE *output = fopen(outputFile, "w");

    if (!input || !output) {
        printf("Error opening file(s).\n");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), input)) { // Read each line from the input file
        cleanString(line); // I use this to remove unwanted characters)
        char roman1[50], roman2[50], operation;
        int num1, num2, result;

        if (sscanf(line, "%49s %c %49s", roman1, &operation, roman2) == 3) {
            if (!isValidRoman(roman1) || !isValidRoman(roman2)) { // Validate Roman numerals
                fprintf(output, "Invalid Roman numeral in line: %s\n", line); // Error: Invalid Roman numeral in line.
                continue;
            }
//
            num1 = romanToDecimal(roman1);
            num2 = romanToDecimal(roman2);

            result = performOperation(num1, num2, operation);

            if (result == -1) {
                fprintf(output, "Invalid operation in line: %s\n", line);
            } else {
                writeOutput(output, result);
            }
        } else {
            fprintf(output, "Invalid input format in line: %s\n", line);
        }
    }

    fclose(input);
    fclose(output);
}

// Process input file with logging
void processInputFileWithLogging(const char *inputFile, const char *outputFile) {
    FILE *input = fopen(inputFile, "r");
    FILE *output = fopen(outputFile, "w");
    FILE *processLog = fopen("Process.txt", "w");

    if (!input || !output || !processLog) {
        printf("Error opening file(s).\n");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), input)) {
        cleanString(line);
        char roman1[50], roman2[50], operation;
        int num1, num2, result;

        fprintf(processLog, "Processing line: %s", line);

        if (sscanf(line, "%49s %c %49s", roman1, &operation, roman2) == 3) {
            if (!isValidRoman(roman1) || !isValidRoman(roman2)) {
                fprintf(output, "Invalid Roman numeral in line: %s\n", line);
                fprintf(processLog, "Error: Invalid Roman numeral in line.\n");
                continue;
            }

            num1 = romanToDecimal(roman1);
            num2 = romanToDecimal(roman2);

            fprintf(processLog, "Converted: %s -> %d, %s -> %d\n", roman1, num1, roman2, num2);

            result = performOperation(num1, num2, operation);

            if (result == -1) {
                fprintf(output, "Invalid operation in line: %s\n", line);
                fprintf(processLog, "Error: Invalid operation '%c'.\n", operation);
            } else {
                writeOutput(output, result);
                fprintf(processLog, "Result: %d\n", result);
            }
        } else {
            fprintf(output, "Invalid input format in line: %s\n", line);
            fprintf(processLog, "Error: Invalid input format.\n");
        }
    }

    fclose(input);
    fclose(output);
    fclose(processLog);
}

// Validate Roman numerals
int isValidRoman(char *roman) {
    for (int i = 0; roman[i]; i++) {
        char c = toupper(roman[i]);
        if (c != 'I' && c != 'V' && c != 'X' && c != 'L' && c != 'C' && c != 'D' && c != 'M') { // Check for invalid characters
            return 0;
        }
    }
    return 1;
}

// Clean input strings
void cleanString(char *str) {
    char *ptr = str;
    while (*ptr) {
        if (isprint(*ptr) || isspace(*ptr)) {
            ptr++;
        } else {
            *ptr = '\0';
            break;
        }
    }
}

// Write output to file
void writeOutput(FILE *outputFile, int result) {
    char resultInWords[256]; // Buffer to store the result in words
    numberToWords(result, resultInWords); // Convert result to words
    fprintf(outputFile, "%s\n", resultInWords); // Write the result in words to output file
}
