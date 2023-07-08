#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>

#define EMPTY_STRING ""

// Structure representing a node in the Huffman tree
struct Node
{
    char character;     // Character stored in the node
    int frequency;      // Frequency of the character
    Node *left, *right; // Pointers to the left and right child nodes
};

// Function to create a new node with the specified character, frequency, and children
Node *createNode(char character, int frequency, Node *left, Node *right)
{
    Node *node = new Node();
    node->character = character;
    node->frequency = frequency;
    node->left = left;
    node->right = right;
    return node;
}

// Comparison object to order the nodes in the priority queue based on frequency
struct Compare
{
    bool operator()(const Node *left, const Node *right) const
    {
        return left->frequency > right->frequency;
    }
};

// Check if a node is a leaf node
bool isLeaf(Node *node)
{
    return node->left == nullptr && node->right == nullptr;
}

// Traverse the Huffman tree and encode characters with their corresponding Huffman codes
void encode(Node *node, std::string currentCode, std::unordered_map<char, std::string> &huffmanCode)
{
    if (node == nullptr)
    {
        return;
    }

    // If a leaf node is encountered, store the Huffman code for the character
    if (isLeaf(node))
    {
        huffmanCode[node->character] = (currentCode != EMPTY_STRING) ? currentCode : "1";
    }

    // Recursively traverse the left and right subtrees, appending "0" and "1" to the current code respectively
    encode(node->left, currentCode + "0", huffmanCode);
    encode(node->right, currentCode + "1", huffmanCode);
}

// Traverse the Huffman tree and decode an encoded string
void decode(Node *node, int &index, std::string encodedString)
{
    if (node == nullptr)
    {
        return;
    }

    // If a leaf node is encountered, print its character
    if (isLeaf(node))
    {
        std::cout << node->character;
        return;
    }

    index++;

    // Recursively traverse the left or right subtree based on the encoded string
    if (encodedString[index] == '0')
    {
        decode(node->left, index, encodedString);
    }
    else
    {
        decode(node->right, index, encodedString);
    }
}

// Build the Huffman tree and perform encoding and decoding
void buildHuffmanTree(std::string inputText)
{
    // Base case: empty string
    if (inputText == EMPTY_STRING)
    {
        return;
    }

    // Count the frequency of each character in the input text
    std::unordered_map<char, int> characterFrequency;
    for (char ch : inputText)
    {
        characterFrequency[ch]++;
    }

    // Create a priority queue to store the nodes of the Huffman tree based on their frequency
    std::priority_queue<Node *, std::vector<Node *>, Compare> pq;

    // Create a leaf node for each character and add it to the priority queue
    for (auto pair : characterFrequency)
    {
        pq.push(createNode(pair.first, pair.second, nullptr, nullptr));
    }

    // Build the Huffman tree by merging nodes from the priority queue
    while (pq.size() != 1)
    {
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();

        // Create a new internal node with the sum of frequencies of the two nodes
        pq.push(createNode('\0', left->frequency + right->frequency, left, right));
    }

    // Get the root of the Huffman tree
    Node *root = pq.top();

    // Traverse the Huffman tree and generate Huffman codes for each character
    std::unordered_map<char, std::string> huffmanCode;
    encode(root, EMPTY_STRING, huffmanCode);

    // Print the Huffman codes for each character
    std::cout << "Huffman Codes are:\n"
              << std::endl;
    for (auto pair : huffmanCode)
    {
        std::cout << pair.first << " " << pair.second << std::endl;
    }

    // Print the original input text
    std::cout << "\nThe original string is:\n"
              << inputText << std::endl;

    // Encode the input text using the generated Huffman codes
    std::string encodedString;
    for (char ch : inputText)
    {
        encodedString += huffmanCode[ch];
    }

    // Print the encoded string
    std::cout << "\nThe encoded string is:\n"
              << encodedString << std::endl;
    std::cout << "\nThe decoded string is:\n";

    if (isLeaf(root))
    {
        // Special case: If the root is a leaf node, print the character multiple times based on its frequency
        while (root->frequency--)
        {
            std::cout << root->character;
        }
    }
    else
    {
        // Decode the encoded string using the Huffman tree
        int index = -1;
        while (index < (int)encodedString.size() - 1)
        {
            decode(root, index, encodedString);
        }
    }
}

// Huffman coding algorithm implementation in C++
int main()
{
    // Specify the input text
    std::string inputText = "This is a sample text.";

    // Build the Huffman tree and perform encoding and decoding
    buildHuffmanTree(inputText);

    return 0;
}