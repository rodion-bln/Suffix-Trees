/* Balaniuc Rodion 315CB */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static const int MAX_CHARS_PER_LINE = 1024 * 8;
static const int ALPHABET_SIZE = 27;

typedef struct node {
    char *str;
    struct node *children[27];
} Node, *Tree;

// structure for file format of tasks: 1, 2, 4
typedef struct {
    int num_lines;
    char **content;
} FileContent124;

// structure for file format of task: 3
typedef struct {
    int num_lines;
    char **content;
    int num_suffixes;
    char **suffixes;
} FileContent3;

/* Breadth First Search */

typedef struct queue_node {
    Tree tree;
    int depth;
    struct queue_node *next;
} QueueNode;

typedef struct queue {
    QueueNode *front;
    QueueNode *rear;
} Queue;

Queue *create_queue() {
    Queue *q = (Queue *) malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, Tree tree, int depth) {
    QueueNode *temp = (QueueNode *) malloc(sizeof(QueueNode));
    temp->tree = tree;
    temp->depth = depth;
    temp->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
}

QueueNode *dequeue(Queue *q) {
    if (q->front == NULL)
        return NULL;

    QueueNode *temp = q->front;
    q->front = q->front->next;

    if (q->front == NULL)
        q->rear = NULL;

    return temp;
}

void breadth_first_traversal(Tree root, const char *output_file) {
    FILE *file = fopen(output_file, "w");
    if (file == NULL) {
        printf("Failed to open file: %s\n", output_file);
        return;
    }

    Queue *q = create_queue();
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i] != NULL) {
            enqueue(q, root->children[i], 1);
        }
    }

    int current_depth = 1;
    while (q->front != NULL) {
        QueueNode *temp_node = dequeue(q);
        Tree temp = temp_node->tree;
        int depth = temp_node->depth;
        free(temp_node);

        if (depth != current_depth) {
            fprintf(file, "\n");
            current_depth = depth;
        }

        fprintf(file, "%s ", temp->str);

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (temp->children[i] != NULL) {
                enqueue(q, temp->children[i], depth + 1);
            }
        }
    }

    fprintf(file, "\n");
    fclose(file);
    free(q);
}

/* END Breadth First Search */

char *char_to_string(char letter) {
    char *str = malloc(2 * sizeof(char));
    str[0] = letter;
    str[1] = '\0';
    return str;
}

Tree new_node(char letter) {
    Tree tree = (Tree) malloc(sizeof(Node));
    tree->str = char_to_string(letter);
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        tree->children[i] = NULL;
    }
    return tree;
}

void free_tree(Tree root) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        free_tree(root->children[i]);
    }

    free(root->str);
    free(root);
}


char *append_dollar_to_string(const char *str) {
    int length = strlen(str);
    char *new_str = (char *) malloc((length + 2) * sizeof(char)); // +2 for the "$" and the '\0' terminator

    if (new_str == NULL) {
        printf("Failed to allocate memory for new string\n");
        return NULL;
    }

    strcpy(new_str, str);
    new_str[length] = '$';
    new_str[length + 1] = '\0';

    return new_str;
}

int append_to_suffix_tree(Tree root, char *line) {
    char *new_line = append_dollar_to_string(line);
    for (int i = strlen(new_line) - 1; i >= 0; i--) {
        Tree current = root;
        for (int j = i; j < strlen(new_line); j++) {
            int index;
            if (new_line[j] == '$') {
                index = 0;
            } else {
                char lower_letter = tolower(new_line[j]);
                if (lower_letter >= 'a' && lower_letter <= 'z') {
                    //shift by 1 to make the index 1-based for [$, a-z]
                    index = lower_letter - 'a' + 1;
                } else {
                    return 1; //error, invalid character
                }
            }
            if (current->children[index] == NULL) {
                current->children[index] = new_node(new_line[j]);
            }
            current = current->children[index];
        }
    }
    free(new_line);
    return 0;
}

int count_leaf_nodes(Tree tree) {
    if (tree == NULL) {
        return 0;
    }

    int is_leaf_node = 1;
    int count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (tree->children[i] != NULL) {
            is_leaf_node = 0;
            count += count_leaf_nodes(tree->children[i]);
        }
    }

    return is_leaf_node ? 1 : count;
}

int count_paths_of_depth(Tree tree, int depth, int target_depth, Tree root) {
    if (tree == NULL) {
        return 0;
    }

    // If we've reached the target depth, check if it's a leaf node
    if (depth == target_depth) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (tree->children[i] != NULL) {
                // If it's not a leaf node, return 0
                return 0;
            }
        }

        // If it's a leaf node, return 1
        return 1;
    }

    int path_count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (tree->children[i] != NULL) {
            path_count += count_paths_of_depth(tree->children[i], depth + 1, target_depth, root);
        }
    }

    return path_count;
}

int max_descendants(Tree tree) {
    if (tree == NULL) {
        return 0;
    }

    int current_node_descendants = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (tree->children[i] != NULL) {
            current_node_descendants++;
        }
    }

    int max = current_node_descendants;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (tree->children[i] != NULL) {
            int child_max_descendants = max_descendants(tree->children[i]);
            if (child_max_descendants > max) {
                max = child_max_descendants;
            }
        }
    }

    return max;
}

FileContent124 *read_file_content_124(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s. "
               "Common reasons are invalid file path or missing read permissions\n", filename);
        return NULL;
    }

    FileContent124 *fc = (FileContent124 *) malloc(sizeof(FileContent124));
    if (fc == NULL) {
        printf("Failed to allocate memory for file_content\n");
        fclose(file);
        return NULL;
    }

    fscanf(file, "%d\n", &(fc->num_lines));

    fc->content = (char **) malloc(fc->num_lines * sizeof(char *));
    if (fc->content == NULL) {
        printf("Failed to allocate memory for content\n");
        free(fc);
        fclose(file);
        return NULL;
    }

    char *line = (char *) malloc(MAX_CHARS_PER_LINE);
    for (int i = 0; i < fc->num_lines; i++) {
        fscanf(file, "%s", line);
        fc->content[i] = (char *) malloc(strlen(line) + 1);
        strcpy(fc->content[i], line);
    }
    free(line);

    fclose(file);
    return fc;
}

void free_file_content_124(FileContent124 *fc) {
    if (fc != NULL) {
        if (fc->content != NULL) {
            for (int i = 0; i < fc->num_lines; i++) {
                if (fc->content[i] != NULL) {
                    free(fc->content[i]);
                }
            }
            free(fc->content);
        }
        free(fc);
    }
}

FileContent3 *read_file_content_3(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s. "
               "Common reasons are invalid file path or missing read permissions\n", filename);
        return NULL;
    }

    FileContent3 *fc = (FileContent3 *) malloc(sizeof(FileContent3));
    if (fc == NULL) {
        printf("Failed to allocate memory for file_content\n");
        fclose(file);
        return NULL;
    }

    fscanf(file, "%d %d\n", &(fc->num_lines), &(fc->num_suffixes));

    fc->content = (char **) malloc(fc->num_lines * sizeof(char *));
    if (fc->content == NULL) {
        printf("Failed to allocate memory for content\n");
        free(fc);
        fclose(file);
        return NULL;
    }
    fc->suffixes = (char **) malloc(fc->num_suffixes * sizeof(char *));
    if (fc->suffixes == NULL) {
        printf("Failed to allocate memory for suffixes\n");
        free(fc);
        fclose(file);
        return NULL;
    }

    char *line = (char *) malloc(MAX_CHARS_PER_LINE);
    for (int i = 0; i < fc->num_lines; i++) {
        fscanf(file, "%s", line);
        fc->content[i] = (char *) malloc(strlen(line) + 1);
        strcpy(fc->content[i], line);
    }
    for (int i = 0; i < fc->num_suffixes; i++) {
        fscanf(file, "%s", line);
        fc->suffixes[i] = (char *) malloc(strlen(line) + 1);
        strcpy(fc->suffixes[i], line);
    }
    free(line);

    fclose(file);
    return fc;
}

void free_file_content_3(FileContent3 *fc) {
    if (fc != NULL) {
        if (fc->content != NULL) {
            for (int i = 0; i < fc->num_lines; i++) {
                if (fc->content[i] != NULL) {
                    free(fc->content[i]);
                }
            }
            free(fc->content);
        }
        if (fc->suffixes != NULL) {
            for (int i = 0; i < fc->num_suffixes; i++) {
                if (fc->suffixes[i] != NULL) {
                    free(fc->suffixes[i]);
                }
            }
            free(fc->suffixes);
        }
        free(fc);
    }
}

Tree find_node_by_path(Tree tree, const char *target_path, char *current_path, int depth) {
    if (tree == NULL) {
        return NULL;
    }

    // Add the current node's letter to the current path
    current_path[depth] = tree->str[0];
    current_path[depth + 1] = '\0'; // Null terminate the string

    // If the current path matches the target path, return the current node
    char *path = current_path + 1;
    if (strcmp(path, target_path) == 0) {
        return tree;
    }

    // Recursively search the children
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        Tree found_node = find_node_by_path(tree->children[i], target_path, current_path, depth + 1);
        if (found_node != NULL) {
            return found_node;
        }
    }

    // If the target node was not found in this subtree, return NULL
    return NULL;
}

Tree find_single_child(Tree input) {
    Tree last_child = NULL;
    int children_count = 0;
    for (int i = 0; i < ALPHABET_SIZE && children_count < 2; i++) {
        if (input->children[i] != NULL) {
            last_child = input->children[i];
            children_count++;
        }
    }
    return children_count == 1 ? last_child : NULL;
}

void compact_node(Tree input) {
    if (input == NULL) {
        return;
    }

    char path[MAX_CHARS_PER_LINE];
    int index = 0;
    Tree p = input;
    Tree last_child;
    while (p != NULL && p->str[0] != '$') {
        path[index++] = p->str[0];
        last_child = p;
        p = find_single_child(p);
    }
    path[index] = '\0';

    Tree to_clean = find_single_child(input);
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        input->children[i] = last_child->children[i];
    }
    free(input->str);
    input->str = (char *) malloc(index + 1);
    strcpy(input->str, path);

    while (to_clean != NULL && to_clean->str[0] != '$') {
        Tree temp = to_clean;
        to_clean = find_single_child(to_clean);
        free(temp->str);
        free(temp);
    };
}

void compact_tree(Tree input) {
    if (input == NULL) {
        return;
    }

    if (find_single_child(input)) {
        compact_node(input);
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (input->children[i] != NULL) {
            compact_tree(input->children[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    int command = 0;
    int k = 0;
    char *input_file = NULL;
    char *output_file = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c1") == 0) {
            command = 1;
        } else if (strcmp(argv[i], "-c2") == 0) {
            command = 2;
            if (i + 1 < argc) {
                k = atoi(argv[++i]);
            } else {
                printf("Missing value for K after -c2\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-c3") == 0) {
            command = 3;
        } else if (strcmp(argv[i], "-c4") == 0) {
            command = 4;
        } else if (input_file == NULL) {
            input_file = argv[i];
        } else if (output_file == NULL) {
            output_file = argv[i];
        } else {
            printf("Unknown argument: %s\n", argv[i]);
            return 1;
        }
    }

    if (input_file == NULL) {
        printf("Input file argument is missing");
        return 1;
    }

    if (output_file == NULL) {
        printf("Output file argument is missing");
        return 1;
    }

    switch (command) {
        case 1:
        case 2: {
            FileContent124 *fc_124 = read_file_content_124(input_file);
            Tree root = new_node('#');
            for (int i = 0; i < fc_124->num_lines; i++) {
                char *line = fc_124->content[i];
                printf("Append line: %s\n", line);
                append_to_suffix_tree(root, line);
            }
            if (command == 1) {
                breadth_first_traversal(root, output_file);
            } else {
                int leaf_nodes = count_leaf_nodes(root);
                //+1 for $ character
                int paths_of_depth_k = count_paths_of_depth(root, 0, k + 1, root);

                printf("Paths of depth %d: %d\n", k, paths_of_depth_k);

                int max_desc = max_descendants(root);
                FILE *file = fopen(output_file, "w");
                if (file == NULL) {
                    printf("Failed to open file: %s\n", output_file);
                    return 1;
                }
                fprintf(file, "%d\n%d\n%d\n", leaf_nodes, paths_of_depth_k, max_desc);
                fclose(file);
            }
            free_file_content_124(fc_124);
            free_tree(root);
            break;
        }
        FileContent3 *fc_3;
        Tree root;
        case 3: 
            fc_3 = read_file_content_3(input_file);
            root = new_node('#');
            for (int i = 0; i < fc_3->num_lines; i++) {
                char *line = fc_3->content[i];
                printf("Append line: %s\n", line);
                append_to_suffix_tree(root, line);
            }

            FILE *file = fopen(output_file, "w");
            if (file == NULL) {
                printf("Failed to open file: %s\n", output_file);
                return 1;
            }
            for (int i = 0; i < fc_3->num_suffixes; i++) {
                char *suffix = fc_3->suffixes[i];
                char *search_suffix = append_dollar_to_string(suffix);

                char path[MAX_CHARS_PER_LINE];
                Tree found_node = find_node_by_path(root, search_suffix, path, 0);
                fprintf(file, "%d\n", found_node != NULL);
                free(search_suffix);
            }
            fclose(file);
            free_file_content_3(fc_3);
            free_tree(root);
            break;
        case 4: {
            FileContent124 *fc_124 = read_file_content_124(input_file);
            Tree root = new_node('#');
            for (int i = 0; i < fc_124->num_lines; i++) {
                char *line = fc_124->content[i];
                printf("Append line: %s\n", line);
                append_to_suffix_tree(root, line);
            }

            compact_tree(root);
            breadth_first_traversal(root, output_file);

            free_file_content_124(fc_124);
            free_tree(root);
            break;
        }
    }

    return 0;
}