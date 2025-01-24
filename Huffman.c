//Sct221-0378/2022 Neema Mutanu
//SCT221-0232/2022 Roy Mwangi
//SCT221-0257/2022 Douglas Bett
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the maximum number of characters
#define MAX_CHARACTERS 256

// Structure for a node in the Huffman Tree
struct Node {
    char character;
    int frequency;
    struct Node * left;
    struct Node * right;
};

// Function to create a new node
struct Node * createNode(char character, int frequency) {
    struct Node * newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode -> character = character;
    newNode -> frequency = frequency;
    newNode -> left = NULL;
    newNode -> right = NULL;
    return newNode;
}

// Function to sort nodes by frequency (used to simplify priority queue logic)
void sortNodes(struct Node ** nodes, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (nodes[j] -> frequency > nodes[j + 1] -> frequency) {
                struct Node * temp = nodes[j];
                nodes[j] = nodes[j + 1];
                nodes[j + 1] = temp;
            }
        }
    }
}

// Build the Huffman Tree
struct Node * buildHuffmanTree(char characters[], int frequencies[], int size) {
    struct Node * nodes[MAX_CHARACTERS];
    for (int i = 0; i < size; i++) {
        nodes[i] = createNode(characters[i], frequencies[i]);
    }

    while (size > 1) {
        // Sort nodes by frequency
        sortNodes(nodes, size);

        // Merge the two smallest nodes
        struct Node * left = nodes[0];
        struct Node * right = nodes[1];
        struct Node * parent = createNode('$', left->frequency + right->frequency);
        parent -> left = left;
        parent -> right = right;

        // Replace the two smallest nodes with the new parent node
        nodes[0] = parent;
        for (int i = 1; i < size - 1; i++) {
            nodes[i] = nodes[i + 1];
        }
        size--;
    }

    return nodes[0];
}

// Generate Huffman Codes recursively
void generateCodes(struct Node * root, char code[], int length, char codes[MAX_CHARACTERS][MAX_CHARACTERS]) {
    if (root -> left == NULL && root -> right == NULL) {
        code[length] = '\0';
        strcpy(codes[(unsigned char)root -> character], code);
        return;
    }

    if (root -> left) {
        code[length] = '0';
        generateCodes(root -> left, code, length + 1, codes);
    }

    if (root -> right) {
        code[length] = '1';
        generateCodes(root -> right, code, length + 1, codes);
    }
}

// Calculate and display sizes before and after compression
void calculateSizes(char * text, char codes[MAX_CHARACTERS][MAX_CHARACTERS]) {
    int originalSize = strlen(text) * 8; // Each character is 8 bits
    int compressedSize = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        compressedSize += strlen(codes[(unsigned char)text[i]]);
    }

    printf("\n\nThe Original size is: %d bits\n\n", originalSize);
    printf("\nThe Compressed size is: %d bits\n\n", compressedSize);
}

int main() {
    char text[1000];
    printf("Maximum Number of Characters should not exceed 256.\n\n");
    printf("Enter text you want to compress: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n\n")] = '\0'; // Remove trailing newline character

    // Count frequencies of each character
    int frequencies[MAX_CHARACTERS] = {0};
    for (int i = 0; text[i] != '\0'; i++) {
        frequencies[(unsigned char)text[i]]++;
    }

    // Prepare the character and frequency arrays
    char characters[MAX_CHARACTERS];
    int freqs[MAX_CHARACTERS];
    int size = 0;

    for (int i = 0; i < MAX_CHARACTERS; i++) {
        if (frequencies[i] > 0) {
            characters[size] = (char)i;
            freqs[size] = frequencies[i];
            size++;
        }
    }

    // Build the Huffman Tree
    struct Node * root = buildHuffmanTree(characters, freqs, size);

    // Generate Huffman Codes
    char codes[MAX_CHARACTERS][MAX_CHARACTERS] = {0};
    char code[MAX_CHARACTERS];
    generateCodes(root, code, 0, codes);

    // Display the sizes
    calculateSizes(text, codes);

    return 0;
}
