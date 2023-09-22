#include "test.h"
#include <stdio.h>
#include <string.h> // for testing generate_splits()

/*
 * Generate k-selections of a[0..n-1] in lexicographic order and call process_selection to process them.
 *
 * The array b[] will have enough space to hold k elements.
 * For a selection i1, ..., ik, you should set b[0] = a[i1], ..., b[k-1] = a[ik].
 * Selections should be generated in lexicographic order.
 * a[0..k-1] is the smallest selection and a[n-k..n-1] is the largest.
 *     b[0] = 2; b[1] = 1;
    process_selection(b, 2, data);
    b[0] = 2; b[1] = 6;
    process_selection(b, 2, data);
    b[0] = 2; b[1] = 5;
    process_selection(b, 2, data);
    b[0] = 1; b[1] = 6;
    process_selection(b, 2, data);
    b[0] = 1; b[1] = 5;
    process_selection(b, 2, data);
    b[0] = 6; b[1] = 5;
    process_selection(b, 2, data);
 */
void helper(int i, int j, int a[], int n, int k, int b[],void *data, void (*process_selection)(int *b, int k, void *data)){
    if(j > n) return;
    if(i == k) {
        process_selection(b,k,data);
        return;
    }

    b[i] = a[j];
    helper(i + 1,j + 1, a, n, k , b, data, process_selection);
    helper(i, j + 1, a, n, k, b,data, process_selection);
}

void generate_selections(int a[], int n, int k, int b[], void *data, void (*process_selection)(int *b, int k, void *data))
{
    helper(0, 0, a, n, k, b, data, process_selection);
}


/*
 * See Exercise 2 (a), page 94 in Jeff Erickson's textbook.
 * The exercise only asks you to count the possible splits.
 * In this assignment, you have to generate all possible splits into buf[]
 * and call process_split() to process them.
 * The dictionary parameter is an array of words, sorted in dictionary order.
 * nwords is the number of words in this dictionary.
 */

int isword(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return 0;
        }
        i++;
    }

    if (a[i] == '\0' && b[i] == '\0') {
        return 1;
    } else {
        return 0;
    }
}

void generate_all_splits(const char *source, const char *dictionary[], int nwords, char buf[], int start, int space, void *data, void (*process_split)(char buf[], void *data)) {
    int n = strlen(source);

    if (start == n) {
        buf[space] = '\0';
        process_split(buf, data);
        return;
    }

    for (int end = start; end < n; end++) {
        int len = end - start + 1;
        if (len > 99) {
            // Avoid buffer overflow
            break;
        }

        char temp_buf[100];
        strncpy(temp_buf, source + start, len);
        temp_buf[len] = '\0';

        int present = 0;
        for (int i = 0; i < nwords; i++) {
            if (isword(temp_buf, dictionary[i]) == 1) {
                present = 1;
                break;
            }
        }

        if (present) {
            if (space > 0) {
                buf[space] = ' ';
                space++;
            }

            strcpy(buf + space, temp_buf);
            generate_all_splits(source, dictionary, nwords, buf, end + 1, space + len, data, process_split);
        }
    }
}
void generate_splits(const char *source, const char *dictionary[], int nwords, char buf[], void *data, void (*process_split)(char buf[], void *data)) {
    buf[0] = '\0';
    generate_all_splits(source, dictionary, nwords, buf, 0, 0, data, process_split);
}

/*
 * Transform a[] so that it becomes the previous permutation of the elements in it.
 * If a[] is the first permutation, leave it unchanged.
 */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void previous_permutation(int a[], int n)
{   
    int idx = n - 2;
    while (idx >= 0 && a[idx] <= a[idx + 1]) {
        idx--;
    }
    if(idx < 0) return;

    int s_max = idx+1;
    for(int i = idx+1; i < n; i++){
        if(a[i] > a[s_max] && a[i] < a[idx]) 
            s_max = i;
    }
    swap(&a[idx], &a[s_max]);

    int x = idx + 1;
    int y = n - 1;
    while(x < y){
        swap(&a[x], &a[y]);
        x++; y--;
    }
    return;
}

/* Write your tests here. Use the previous assignment for reference. */

typedef struct {
    int index;
    int err;
    int first;
} state_t;

static void test_selections_2165(int b[], int k, void *data)
{
    state_t *s = (state_t *)data;
    if (s->first) {
        s->err = 0;
        s->first = 0;
    }
    switch (s->index) {
    case 0:
        if ((b[0] != 2) || (b[1] != 1)) {
            s->err = 1;
        }
        break;
    case 1:
        if ((b[0] != 2) || (b[1] != 6)) {
            s->err = 1;
        }
        break;
    case 2:
        if ((b[0] != 2) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    case 3:
        if ((b[0] != 1) || (b[1] != 6)) {
            s->err = 1;
        }
        break;
    case 4:
        if ((b[0] != 1) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    case 5:
        if ((b[0] != 6) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    default:
        s->err = 1;
    }
    ++(s->index);
}

void count_selections(int b[], int k, void *data)
{
    int *d = (int*)data;
    ++*d;
}

typedef struct {
    int b[100];
} selection_t;

void last_selection(int b[], int k, void *data)
{
    selection_t *s = (selection_t*)data;
    for (int i = 0; i < k; ++i) {
        s->b[i] = b[i];
    }
}

BEGIN_TEST(generate_selections) {
    int a[] = { 2, 1, 6, 5 };
    int b[10];
    state_t s2165 = { .index = 0, .err = 1, .first = 1 };
    generate_selections(a, 4, 2, b, &s2165, test_selections_2165);
    ASSERT(!s2165.err, "Failed on 2 1 6 5.");
    
    int c = 0;
    int aa[] = { 1, 5, 3, 0, 1, 12, 4, 3, 6, 6 };
    generate_selections(aa, 10, 5, b, &c, count_selections);
    ASSERT_EQ(c, 252, "Failed on 10C5.");
    c = 0;
    int a3[] = { 0, 1, 12, 4, 3, 6, 6 };
    generate_selections(aa, 7, 0, b, &c, count_selections);
    ASSERT_EQ(c, 1, "Failed on 7C0.");

    selection_t s;
    generate_selections(aa, 10, 5, b, &s, last_selection);
    ASSERT_ARRAY_VALUES_EQ(s.b, 5, "Failed on last of 10C5.", 12, 4, 3, 6, 6);
} END_TEST



void test_splits_sun(char buf[], void *data)
{
    state_t *s = (state_t*)data;
    if (s->first) {
        s->err = 0;
        s->first = 0;
    }
    switch (s->index) {
    case 0:
        if (strcmp(buf, "sun flower")) {
            s->err = 1;
        }
        break;
    case 1:
        if (strcmp(buf, "sunflower")) {
            s->err = 1;
        }
        break;
    default:
        s->err = 1;
    }
    ++(s->index);
}

void test_splits_art(char buf[], void *data)
{
    state_t *s = (state_t*)data;
    if (s->first) {
        s->err = 0;
        s->first = 0;
    }
    switch (s->index) {
    case 0:
        if (strcmp(buf, "art is toil")) {
            s->err = 1;
        }
        break;
    case 1:
        if (strcmp(buf, "artist oil")) {
            s->err = 1;
        }
        break;
    default:
        s->err = 1;
    }
    ++(s->index);
}

BEGIN_TEST(generate_splits) {
    const char *a = "artistoil";
    const char *dict[] = {
        "art",
        "artist",
        "is",
        "oil",
        "toil"
    };
    int nwords = 5;
    state_t s = { .index = 0, .err = 1, .first = 1 };
    char buf[256] = "";
    generate_splits(a, dict, nwords, buf, &s, test_splits_art);
    ASSERT(!s.err, "Failed on 'artistoil'.");

    const char *b = "sunflower";
    const char *dict2[] = {
        "sun",
        "flower",
        "sunflower"
    };
    int nwords_2 = 3;
    state_t s_2 = { .index = 0, .err = 1, .first = 1 };
    char buf_2[256] = "";
    generate_splits(b, dict2, nwords_2, buf_2, &s_2, test_splits_sun);
    ASSERT(!s_2.err, "Failed on 'sunflower'.");
} END_TEST

BEGIN_TEST(previous_permutation) {
    int a[] = { 1, 5, 6, 2, 3, 4 };
    previous_permutation(a, 6);
    ASSERT_ARRAY_VALUES_EQ(a, 6, "Failed on 1 5 6 2 3 4.", 1, 5, 4, 6, 3, 2);
    int aa[] = { 1, 2, 3, 5, 4, 6 };
    previous_permutation(aa, 3); // 3 is correct.
    ASSERT_ARRAY_VALUES_EQ(aa, 3, "Failed on 1 2 3.", 1, 2, 3);
    int a3[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1};
    previous_permutation(a3, 9);
    ASSERT_ARRAY_VALUES_EQ(a3, 9, "Failed on 9 8 7 6 5 4 3 2 1.", 9, 8, 7, 6, 5, 4, 3, 1, 2);
    int a4[] = { 4, 1, 2, 3};
    previous_permutation(a4, 4);
    ASSERT_ARRAY_VALUES_EQ(a4, 4, "Failed on 4 1 2 3.", 3, 4, 2, 1);
} END_TEST

int main()
{
    run_tests((test_t[]) {
            TEST(generate_selections),
            TEST(generate_splits),
            TEST(previous_permutation),
            0
        });
    return 0;
}
