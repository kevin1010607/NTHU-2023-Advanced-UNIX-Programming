#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Define our list node
/**********************************************************/
/* You can not modify the code in this block */
/* You can not modify the code in this block */
/* You can not modify the code in this block */
struct ListNode {
	int val;
 	struct ListNode *next;
};
struct ListNode* append_node(struct ListNode* head, int val);
struct ListNode* delete_node(struct ListNode* head, int position);
struct ListNode* insertionSortList(struct ListNode* head);
/**********************************************************/

int main(int argc, char const *argv[])
{
	//Given array
	int int_arr[6] = {5,4,3,1,2,6};
	struct ListNode *head = NULL; 
	//transform array to linked list
    for(int i = 0; i < 6; i++){
		head = append_node(head,int_arr[i]);
	}
    
    //insertion sort
    head = insertionSortList(head);
    
    //Print the list
    /*
     The output should be
     1
     2
     3
     4
     5
     6
     */
	struct ListNode *node = head;
	while(node != NULL){
		printf("%d\n",node->val);
		node = node->next;
	}

    while(head != NULL) {
        head = delete_node(head, 0);
    }
	
	return 0;
}

struct ListNode* append_node(struct ListNode* head, int val){
    struct ListNode *newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->val = val;
    newNode->next = NULL;

    if(head == NULL) {
        head = newNode;
    }
    else {
        struct ListNode *now = head;
        // Move to tail
        while(now->next) {
            now = now->next;
        }
        now->next = newNode;
    }

    return head;
}

//Insertion sort
/**********************************************************/
/* You don't have to modify the code in this block */
struct ListNode* insertionSortList(struct ListNode* head) {
    if(head == NULL)return NULL;

    struct ListNode *node = head->next, *pre = head; 
    while(node != NULL){
        struct ListNode *tmp = node;
        //handle head
        if(tmp->val < head->val){
            pre->next = node->next;
            node->next = head;
            head = node;
        } else {
            struct ListNode *j = head->next, *jpre = head;
            while(j != node){
                if(tmp->val < j->val){
                    pre->next = node->next;
                    jpre->next = node;
                    node->next = j;
                    break;
                }
                jpre = j;
                j = j->next;
            }
        }
	    pre = tmp;
        node = tmp->next;
        
    }
    return head;
}
/**********************************************************/

struct ListNode* delete_node(struct ListNode* head, int position) {
    if(head == NULL) return NULL;

    if(position == 0) {
        struct ListNode *tmp = head;
        head = head->next;
        free(tmp);
    }
    else{
        struct ListNode *now = head;
        // Move to the previous node of deleted node
        for(int i = 0; i < position - 1; i++) {
            if(now == NULL) return head;
            now = now->next;
        }
        if(now->next == NULL) return head;
        struct ListNode *tmp = now->next;
        now->next = now->next->next;
        free(tmp);
    }

    return head;
}
