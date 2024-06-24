#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <bitset>

using namespace std;

struct Node {
    char symbol;
    int frequency;
    Node* left, *right;
};

Node* getNode(char symbol, int frequency, Node* left, Node* right) {
    Node* node = new Node();
    node->symbol = symbol;
    node->frequency = frequency;
    node->left = left;
    node->right = right;
    return node;
}

struct comp {
    bool operator()(Node* left, Node* right) {
        return left->frequency > right->frequency;
    }
};

void encode(Node* root, string str, unordered_map<char, string>& HuffmanCode) {
    if (root == nullptr) {
        return;
    }
    if (!root->left && !root->right) {
        // Asegurarse de que el código tenga una longitud de 3 dígitos
        string paddedStr = str;
        while (paddedStr.size() < 3) {
            paddedStr = "0" + paddedStr;
        }
        if (paddedStr.size() > 3) {
            paddedStr = paddedStr.substr(paddedStr.size() - 3);  // Limitar a 3 dígitos
        }
        HuffmanCode[root->symbol] = paddedStr;
    }
    encode(root->left, str + "0", HuffmanCode);
    encode(root->right, str + "1", HuffmanCode);
}

void decode(Node* root, int& index, string str) {
    if (root == nullptr) {
        return;
    }
    if (!root->left && !root->right) {
        cout << root->symbol;
        return;
    }
    index++;
    if (str[index] == '0') {
        decode(root->left, index, str);
    }
    else {
        decode(root->right, index, str);
    }
}

void createTree(string text) {
    unordered_map<char, int> frequency;
    for (char symbol : text) {
        frequency[symbol]++;
    }

    priority_queue<Node*, vector<Node*>, comp> pq;
    for (auto pair : frequency) {
        pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
    }

    while (pq.size() != 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        int sum = left->frequency + right->frequency;
        pq.push(getNode('\0', sum, left, right));
    }

    Node* root = pq.top();
    unordered_map<char, string> HuffmanCode;
    encode(root, "", HuffmanCode);

    cout << "\nHuffman Codes are:\n" << '\n';
    for (auto pair : HuffmanCode) {
        cout << pair.first << " " << pair.second << '\n';
    }

    cout << "\nThe original message was: \n" << text << '\n';
    string encodedText = "";
    for (char symbol : text) {
        encodedText += HuffmanCode[symbol];
    }
    cout << "\nThe encoded text is: \n" << encodedText << '\n';

    int index = -1;
    cout << "\nThe decoded text is: \n";
    while (index < (int)encodedText.size() - 2) {
        decode(root, index, encodedText);
    }
}
