#include <unity.h>

void test_siempre_verdadero(void);

int main(int argc, char **argv) {
    
    // Comienzan los test
    UNITY_BEGIN();

    RUN_TEST(test_siempre_verdadero);

    // Finalizan los test
    UNITY_END();
}


void test_siempre_verdadero(void);


void test_siempre_verdadero(void){
    TEST_ASSERT_EQUAL(0, 0);
}