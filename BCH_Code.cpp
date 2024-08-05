#include<iostream>
#include<vector>
#include<string>
#include<cmath>

/**
 * This Program takes input of the message and the length of the codeword.
 * It then uses BCH coding technique to code the message and then decode it and return both values
 * Demonstrates how a message is coded and then decoded using BCH coding technique.
*/

std::vector<int> polynomialDivision(const std::vector<int>& dividend, const std::vector<int>& divisor) {
    std::vector<int> result = dividend; // Copy dividend to result for modification
    for (size_t i = 0; i <= dividend.size() - divisor.size(); ++i) {
        if (result[i] == 1) { // Only perform subtraction when coefficient is 1
            for (size_t j = 0; j < divisor.size(); ++j) {
                result[i + j] ^= divisor[j]; // Perform binary subtraction (XOR operation)
            }
        }
    }
    return std::vector<int>(result.end() - divisor.size() + 1, result.end());
}
// BCH encoding function
std::vector<int> coding(const std::vector<int>& message, const std::vector<int>& generator, int n, int k) {
    std::vector<int> paddedMessage = message; // Start with the original message
    paddedMessage.resize(n, 0); // Pad with zeros to length n

    std::vector<int> remainder = polynomialDivision(paddedMessage, generator); // Get remainder

    std::vector<int> codeword = paddedMessage; // Start with padded message
    for (int i = 0; i < remainder.size(); i++) {
        codeword[n - k + i] = remainder[i]; // Append the remainder to form the codeword
    }

    return codeword;
}

// BCH decoding function
std::vector<int> decoding(const std::vector<int>& codeword, const std::vector<int>& generator, int n, int k) {
    std::vector<int> remainder = polynomialDivision(codeword, generator);
    int errorPosition = -1;

    // Check if there is an error (remainder should be zero if no errors)
    for (int i = 0; i < remainder.size(); i++) {
        if (remainder[i] != 0) {
            errorPosition = i;
            break;
        }
    }

    std::vector<int> correctedMessage = codeword;
    if (errorPosition != -1) {
        // Flip the bit at the error position
        correctedMessage[errorPosition] ^= 1;
    }

    // Extract the original message (assumes no errors remaining after correction)
    return std::vector<int>(correctedMessage.begin(), correctedMessage.begin() + k);
}


int main(){
    int n,k;
    std::string inputString;


    //Getting the input Message as a binary string and length of the result codeword as int
    std::cout<<"Enter the length of the codeword:\t"; //Length of the code word input
    std::cin>>n;

    std::cout<<"Enter the input Message:\t";
    std::cin>>inputString;

    k = inputString.length(); //since in ASCII 1 and 0 represent 1 bit we can use .length() method

    //Input Validation
    if (k > n) {
        std::cerr << "Message length k must be less than or equal to codeword length n." << std::endl;
        return 1;
    }
    std::vector<int> message(k);
    for (int i = 0; i < k; i++) {
        message[i] = inputString[i] == '1' ? 1 : 0;
    }

    // Example generator polynomial
    std::vector<int> generator = {1, 0, 0, 1, 0, 1};

    // Encode the message
    std::vector<int> codeword = coding(message, generator, n, k);

    // Introduce an error for demonstration (flip one bit)
    codeword[5] ^= 1; //Doing an XOR operation to toggle a bit

    // Decode the message (correcting a single error)
    std::vector<int> decodedMessage = decoding(codeword, generator, n, k);

    // Output the results
    std::cout << "Encoded Codeword with error: ";
    for (int bit : codeword) {
        std::cout << bit;
    }
    std::cout << std::endl;

    std::cout << "Decoded and corrected Message: ";
    for (int bit : decodedMessage) {
        std::cout << bit;
    }
    std::cout << std::endl;

    return 0;
}




