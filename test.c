#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char *input;      // Pointer to the input command line
char current_char;      // Current character being processed

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum e_token {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_END
} t_token;

typedef struct s_token_node {
    t_token type;
    char *value;
    struct s_token_node *next;
} t_token_node;

// Function prototypes
t_token_node* create_token_node(t_token type, const char *value);
void add_token(t_token_node **head, t_token type, const char *value);
void tokenize(const char *input, t_token_node **tokens);
void print_tokens(t_token_node *tokens);
void free_tokens(t_token_node *tokens);

// Create a new token node
t_token_node* create_token_node(t_token type, const char *value) {
    t_token_node *new_node = malloc(sizeof(t_token_node));
    new_node->type = type;
    new_node->value = strdup(value); // Duplicate the string value
    new_node->next = NULL;
    return new_node;
}

// Add a token to the linked list
void add_token(t_token_node **head, t_token type, const char *value) {
    t_token_node *new_node = create_token_node(type, value);
    new_node->next = *head; // Insert at the beginning
    *head = new_node;
}

// Tokenize the input string into tokens
void tokenize(const char *input, t_token_node **tokens) {
    const char *ptr = input;
    
    while (*ptr) {
        // Skip whitespace
        while (isspace(*ptr)) ptr++;

        if (*ptr == '|') {
            add_token(tokens, TOKEN_PIPE, "|");
            ptr++;
        } else if (*ptr == '>') {
            add_token(tokens, TOKEN_REDIRECT_OUT, ">");
            ptr++;
        } else if (*ptr == '<') {
            add_token(tokens, TOKEN_REDIRECT_IN, "<");
            ptr++;
        } else if (isalnum(*ptr) || *ptr == '-') { // Command or argument
            const char *start = ptr;
            while (isalnum(*ptr) || *ptr == '-') ptr++; // Read until non-alphanumeric
            size_t length = ptr - start;
            char *command = strndup(start, length); // Duplicate the command/argument
            add_token(tokens, TOKEN_WORD, command);
            free(command); // Free temporary command storage
        } else {
            ptr++; // Move past unrecognized characters
        }
    }
    
    add_token(tokens, TOKEN_END, ""); // Add end token for completeness
}

// Print the tokens for debugging purposes
void print_tokens(t_token_node *tokens) {
    while (tokens) {
        printf("Token Type: %d, Value: '%s'\n", tokens->type, tokens->value);
        tokens = tokens->next;
    }
}

// Free allocated memory for tokens
void free_tokens(t_token_node *tokens) {
    while (tokens) {
        t_token_node *temp = tokens;
        tokens = tokens->next;
        free(temp->value);
        free(temp);
    }
}


// Function prototypes
void next_char();
void parse_command();
void parse_simple_command();
void parse_argument();
void parse_pipe();
void skip_whitespace();

// Move to the next character in the input
void next_char() {
    current_char = *input++;
}

// Skip whitespace characters
void skip_whitespace() {
    while (isspace(current_char)) {
        next_char();
    }
}

// Parse a command (which may consist of multiple simple commands)
void parse_command() {
    parse_simple_command(); // Parse the first simple command
    while (current_char == '|') { // Check for pipes
        parse_pipe(); // Handle pipe character
        parse_simple_command(); // Parse the next simple command after the pipe
    }
}

// Parse a simple command (a command followed by its arguments)
void parse_simple_command() {
    skip_whitespace(); // Skip any leading whitespace
    if (!isalpha(current_char)) { // Check if it's a valid command name
        fprintf(stderr, "Error: Expected command name\n");
        exit(1);
    }
    
    // Parse command name (assumed to be alphabetic)
    while (isalpha(current_char)) {
        printf("Command Name: %c", current_char);
        next_char(); // Move to the next character
    }

    // Parse arguments (assumed to be alphanumeric)
    while (isalnum(current_char)) {
        parse_argument(); // Call function to parse arguments
    }
}

// Parse an argument (assumed to be alphanumeric)
void parse_argument() {
    skip_whitespace(); // Skip whitespace before arguments
    
    if (!isalnum(current_char)) { // Check if it's a valid argument
        fprintf(stderr, "Error: Expected argument\n");
        exit(1);
    }

    // Print argument characters
    while (isalnum(current_char)) {
        printf(" Argument: %c", current_char);
        next_char(); // Move to the next character
    }
}

// Handle pipes between commands
void parse_pipe() {
    if (current_char == '|') { // Check for pipe character
        printf(" Pipe detected\n");
        next_char(); // Consume the pipe character
    } else {
        fprintf(stderr, "Error: Expected '|' for pipe\n");
        exit(1);
    }
}

// Main parsing function that initializes parsing from input string
void parse(const char *str) {
    const char *input = str;          // Set input string
    char current_char = *input; // Initialize current character
    
    skip_whitespace();   // Skip initial whitespace
    parse_command();     // Start parsing commands

    if (current_char != '\0') { // Check for unexpected characters at end
        fprintf(stderr, "Error: Unexpected character '%c'\n", current_char);
        exit(1);
    }
}

int main() {
    const char *command_line = "ls -l | grep txt | sort"; // Example command
	t_token_node *tokens = NULL;

    printf("Parsing command line: %s\n", command_line);
    
	tokenize(input, &tokens);
    print_tokens(tokens);
    parse(command_line);  // Start parsing the example command
    free_tokens(tokens);
    
    printf("\nParsing completed successfully!\n");
    
    return 0;
}
