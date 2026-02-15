#include <CUnit/Basic.h>
#include "../Input_reader/Queue_buffer/Queue_buffer.h"

/* The suite setup and cleanup functions */
int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/* Test Cases */
void test_createQueue(void) {
    Queue *q = createQueue();
    CU_ASSERT_PTR_NOT_NULL(q);
    freeQueue(q);
}

void test_insert_and_move(void) {
    Queue *q = createQueue();
    
    insert_char(q, 'a');
    insert_char(q, 'b');
    
    // Assuming move_left returns the data at the current cursor
    move_left(q); 
    
    // Use CUnit assertions to verify behavior
    // CU_ASSERT_TRUE, CU_ASSERT_EQUAL, etc.
    
    freeQueue(q);
}

int main() {
    // 1. Initialize Registry
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // 2. Add Suite
    CU_pSuite pSuite = CU_add_suite("Queue_Test_Suite", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // 3. Add Tests to Suite
    if ((NULL == CU_add_test(pSuite, "test of createQueue()", test_createQueue)) ||
        (NULL == CU_add_test(pSuite, "test of insert/move", test_insert_and_move))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // 4. Run tests using the Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return CU_get_error();
}
