#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define STRING_LIMIT 100
#define FRUIT_ID_LIMIT 100

void calcUserSimilarity(int[][100], int, int, double*);
void userRecommendation(int[][100], int, int, char[][100][100]);

// calcUserSimilarity calculates the similarity between users
void calcUserSimilarity(int users[][100], int numUsers, int targetUser, double *similarity) {
    for (int u = 0; u < numUsers; ++u) {
        // Skip the target user
        if (u == targetUser) {
            continue;
        }

        int n = 0, d1 = 0, d2 = 0;

        // Calculate the numerator and denominators for the similarity formula
        for (int fr = 0; fr < 100; ++fr) {
            int c1 = users[targetUser][fr];
            int c2 = users[u][fr];

            n += c1 * c2;
            d1 += c1 * c1;
            d2 += c2 * c2;
        }

        // Calculate the denominator and avoid division by zero
        double d = sqrt((double)d1) * sqrt((double)d2);
        if (d == 0) {
            similarity[u] = 0;
        } else {
            similarity[u] = (double)n / d;
        }
    }
}

// userRecommendation provides recommendations based on user similarity
void userRecommendation(int users[][100], int numUsers, int targetUser, char recommendations[][100][100]) {
    for (int u = 0; u < numUsers; ++u) {
        // Skip the target user
        if (u == targetUser) {
            continue;
        }

        double similarity[numUsers];
        // Calculate user similarity scores
        calcUserSimilarity(users, numUsers, targetUser, similarity);

        // Check if the similarity score is positive
        if (similarity[u] > 0) {
            int count = 0;

            // Check the choices of the user for potential recommendations
            for (int fr = 0; fr < 100; ++fr) {
                int c = users[u][fr];
                if (users[targetUser][fr] == 0 && c == 1) {
                    sprintf(recommendations[u][count], "Recommendation for User %d: Fruit %d", targetUser, fr);
                    ++count;
                }
            }
        }
    }
}

int fruitNameToId(char fruitIDs[FRUIT_ID_LIMIT][STRING_LIMIT], const char fruitName[]) {
	for (int i = 0; i < FRUIT_ID_LIMIT; ++i) {
		// We've reached the "free space" part of the ID map, so it means we
		// couldn't find any fruit ID with that name. So we create one.
		if (fruitIDs[i][0] == '\0') {
			strcpy(fruitIDs[i], fruitName);
			return i;
		}

		// If the current fruit is the same as the fruit name being given, then
		// return its ID.
		if (strcmp(fruitIDs[i], fruitName) == 0) {
			return i;
		}
	}

	// We only get here if there are too many fruits
	printf("Fatal error: too many fruits defined!");
	exit(1);
}

int main(void) {
    // Request the number of users
    int numUsers;
    printf("Enter the number of program users: ");
    scanf("%d", &numUsers);

    // Matrix to store users' fruit choices
	// Format: [userID][fruitID] -> int (1 if chosen, 0 if not)
    int users[numUsers][FRUIT_ID_LIMIT];

	// Initialize `users`
	memset(users, 0, sizeof users);

	// Matrix to store known fruit choices
	// Format: [fruitID] -> string
	// Each fruit's name can thus only have up to 99 characters.
	char fruitIDs[FRUIT_ID_LIMIT][STRING_LIMIT];

    // Interactive menu
    while (1) {
        printf("Menu:\n");
        printf("1 - Enter the Chosen Fruit\n");
        printf("2 - Get Recommendation\n");
        printf("0 - Exit the Program\n");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int userID;
                char fruit[STRING_LIMIT];

                printf("Enter the User ID (1 to %d): ", numUsers);
                scanf("%d", &userID);

                // Check if the entered ID is valid
                if (userID < 1 || userID > numUsers) {
                    printf("Please, enter a valid User ID!\n");
                    continue;
                }

                printf("Enter the Chosen Fruit: ");
                scanf("%s", fruit);

				if (strlen(fruit) == 0) {
					printf("Please, enter a non-empty choice!\n");
					continue;
				}

                // Convert fruit to lowercase
                for (int i = 0; fruit[i]; ++i) {
                    fruit[i] = tolower(fruit[i]);
                }

				// Get the ID for the fruit.
				int fruitID = fruitNameToId(fruitIDs, fruit);

                // Mark that the user chose this fruit
                users[userID - 1][fruitID] = 1;

                printf("Fruit \"%s\" chosen for user [%d].\n", fruit, userID);
                break;
            }

            case 2: {
                int targetUser;
                printf("Enter the ID of the user to get a recommendation (1 to %d): ", numUsers);
                scanf("%d", &targetUser);

                // Check if the entered ID is valid
                if (targetUser < 1 || targetUser > numUsers) {
                    printf("Invalid user ID.\n");
                    continue;
                }

                char recommendations[numUsers][100][100];
                // Get recommendations based on n users
                userRecommendation(users, numUsers, targetUser - 1, recommendations);

                // Display recommendations
                printf("Recommendations:\n");
                for (int u = 0; u < numUsers; ++u) {
                    printf("To user [%d]:\n", u + 1);
                    for (int r = 0; recommendations[u][r][0] != '\0'; ++r) {
                        printf("%s\n", recommendations[u][r]);
                    }
                }
                break;
            }

            case 0:
                printf("Program ended successfully.\n");
                return 0;

            default:
                printf("Please, enter a valid option!\n");
        }
    }
    return 0;
}
