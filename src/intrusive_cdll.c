// Intrusive Circular Doubly Linked List

typedef struct list_node {
        struct list_node *next;
        struct list_node *prev;
} list_node;

typedef struct task_struct {
        int id;
        int state;
        list_node list_node;
} task_struct;

#define GET_OFFSET(type, member) ((size_t)&((type *)0)->member)
#define GET_ENTRY(ptr, type, member) ((type *)((char *)(ptr) - GET_OFFSET(type, member)))

void init_list(list_node *head) {
        head->next = head;
        head->prev = head;
}

void add_tail(list_node *new_node, list_node *head) {
        new_node->prev = head->prev;
        new_node->next = head;
        head->prev->next = new_node;
        head->prev = new_node;
}

int main() {}
