#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm> 
using namespace std;

struct TrieNode {
    bool endOfWord;
    vector<TrieNode*> children;

    TrieNode() {
        endOfWord = false;
        children = vector<TrieNode*>(26, nullptr);
    }
};
void printRed(const string& word) {
    // Print any misspelled words in red
    cout << "\033[31m" << word << "\033[0m";
}

void insertWord(TrieNode *root, const string &word) {
    TrieNode *currentNode = root;
    for (char c : word) { // For each character in the word,
        int index = tolower(c) - 'a'; // Convert to lowercase to remove case sensitivity and Calculate where the character will be placed in the prefix tree
        if (currentNode->children[index] == nullptr) { // If there's no child node available for a character to be inserted,
            currentNode->children[index] = new TrieNode(); // Create a new node and Assign it to the child node at the calculated index
        }
        currentNode = currentNode->children[index]; // Update the current node pointer to move to next node
    }
    currentNode->endOfWord = true; // We have reached the end of the word, so set the current node as the end of the word and Set endOfWord to true
}

bool searchWord(TrieNode *root, string &word) {
    TrieNode *currentNode = root;
    for (char c : word) { // For each character in the word,
        int index = c - 'a'; // Convert to lowercase to remove case sensitivity and Calculate where the character will be in the prefix tree
        if (currentNode->children[index] == nullptr) { // If the child node for the character doesn't exist,
            return false; // Return false. Word does not exist in the dictionary 
        }
        currentNode = currentNode->children[index]; // Otherwise, keep checking. Move to the next node
    }
    return currentNode != nullptr && currentNode->endOfWord; // Return true if the current node exists and Marks the end of a word in the dictionary
}

// Get suggestions for a misspelled word
vector<string> getSuggestions(TrieNode* root, const string &word) {
    vector<string> suggestions;

    // Generate suggestions by changing each character
    for (int i = 0; i < word.length(); ++i) {
        for (char c = 'a'; c <= 'z'; c++) {
            string modified = word;
            modified[i] = c;
            if (searchWord(root, modified)) {
                suggestions.push_back(modified);
            }
        }
    }

    // Generate suggestions by inserting each character
    for (int i = 0; i <= word.length(); ++i) {
        for (char c = 'a'; c <= 'z'; c++) {
            string modified = word;
            modified.insert(i, 1, c);
            if (searchWord(root, modified)) {
                suggestions.push_back(modified);
            }
        }
    }

    // Generate suggestions by deleting each character
    for (int i = 0; i < word.length(); ++i) {
        string modified = word;
        modified.erase(i, 1);
        if (searchWord(root, modified)) {
            suggestions.push_back(modified);
        }
    }

    // Generate suggestions by swapping adjacent characters
    for (int i = 0; i < word.length() - 1; ++i) {
        string modified = word;
        swap(modified[i], modified[i + 1]);
        if (searchWord(root, modified)) {
            suggestions.push_back(modified);
        }
    }

    return suggestions;
}

int main() {
    // Load the dictionary into memory
    TrieNode *root = new TrieNode();
    ifstream dictFile("dictionary.txt");
    string word;
    while (getline(dictFile, word)) {
        insertWord(root, word);
    }
    dictFile.close();

    // Take a sentence from user input
    string sentence;
    cout << "Enter a sentence: ";
    getline(cin, sentence);
    for (char &c : sentence) {
        c = tolower(c);
    }

    // Separate the sentence into words
    stringstream ss(sentence);
    bool firstWord = true; // Variable to track the first word in the sentence
    vector<string> misspelledWords; // Store misspelled words

    while (ss >> word) {
        if (!searchWord(root, word)) { // If word is misspelled,
            if (!firstWord) {
                cout << " ";
            }
            printRed(word);
            misspelledWords.push_back(word); // Add misspelled word to the vector
        } else {
            if (!firstWord) {
                cout << " ";
            }
            cout << word;
        }
        firstWord = false;
    }
    cout << endl;

    // Print suggestions for misspelled words
    for (const string &misspelledWord : misspelledWords) {
        vector<string> suggestions = getSuggestions(root, misspelledWord); // Get suggestions for misspelled word
        if (!suggestions.empty()) {
            cout << "Suggestions for " << misspelledWord << ": "; // Output the suggestions for the misspelled word
            for (const string &suggestion : suggestions) {
                cout << suggestion << " ";
            }
            cout << endl;
        }
    }

    return 0;
}