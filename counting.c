#include <stdio.h>

#define MAX 21 // Maximum number of students and topics (as per typical competitive programming constraints)

int dp[MAX][1 << MAX];
int likes[MAX][MAX];

int countWays(int student, int mask, int n) {
    // Base case: All students have been assigned topics
    if (student == n) {
        return 1;
    }

    // If this subproblem has already been solved, return the result
    if (dp[student][mask] != -1) {
        return dp[student][mask];
    }

    int ways = 0;

    // Try assigning each topic to the current student
    for (int topic = 0; topic < n; ++topic) {
        // Check if the student likes the topic and if the topic hasn't been assigned yet
        if (likes[student][topic] && !(mask & (1 << topic))) {
            ways += countWays(student + 1, mask | (1 << topic), n);
        }
    }

    // Store the result and return it
    return dp[student][mask] = ways;
}

int main() {
    int n;
    printf("Enter the number of students/topics: ");
    scanf("%d", &n);

    printf("Enter the preference matrix:\n");
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            scanf("%d", &likes[i][j]);
        }
    }

    // Initialize the DP table with -1 (indicating unsolved subproblems)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < (1 << n); ++j) {
            dp[i][j] = -1;
        }
    }

    // Start the recursion from the first student with no topics assigned (mask = 0)
    int result = countWays(0, 0, n);
    printf("Number of different assignments: %d\n", result);

    return 0;
}
