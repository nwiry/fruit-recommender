package main

import (
	"bufio"
	"fmt"
	"math"
	"os"
	"strings"
)

// calcUserSimilarity calculates the similarity between users
func calcUserSimilarity(users map[int]map[string]int, targetUser int) map[int]float64 {
	s := make(map[int]float64)

	for u, f := range users {
		// Skip the target user
		if u == targetUser {
			continue
		}

		var n, d1, d2 int

		// Calculate the numerator and denominators for the similarity formula
		for fr, c1 := range users[targetUser] {
			if c2, e := f[fr]; e {
				n += c1 * c2
				d1 += c1 * c1
				d2 += c2 * c2
			}
		}

		// Calculate the denominator and avoid division by zero
		d := math.Sqrt(float64(d1)) * math.Sqrt(float64(d2))
		if d == 0 {
			s[u] = 0
		} else {
			s[u] = float64(n) / d
		}
	}

	return s
}

// userRecommendation provides recommendations based on user similarity
func userRecommendation(users map[int]map[string]int, targetUser int) map[int][]string {
	r := make(map[int][]string)

	for u := range users {
		// Skip the target user
		if u == targetUser {
			continue
		}

		// Calculate user similarity scores
		s := calcUserSimilarity(users, targetUser)

		// Check if the similarity score is positive
		if s[u] > 0 {
			var uR []string

			// Check the choices of the user for potential recommendations
			for fr, c := range users[u] {
				if users[targetUser][fr] == 0 && c == 1 {
					uR = append(uR, fmt.Sprintf("Recommendation for User [%d]: Fruit %s", targetUser, fr))
				}
			}

			r[u] = uR
		}
	}

	return r
}

func main() {
	// Request the number of users
	var n int
	fmt.Print("Enter the number of program users: ")
	fmt.Scan(&n)
	fmt.Println("")

	// Map to store users' fruit choices
	users := make(map[int]map[string]int)

	// Interactive menu
	for {
		fmt.Println("Menu:")
		fmt.Println("1 - Enter the Chosen Fruit")
		fmt.Println("2 - Get Recommendation")
		fmt.Println("0 - Exit the Program")

		var c int
		fmt.Scan(&c)

		switch c {
		case 1:
			var id int
			var fr string

			fmt.Printf("Enter the User ID (1 to %d): ", n)
			fmt.Scan(&id)

			// Check if the entered ID is valid
			if id < 1 || id > n {
				fmt.Print("Please, enter a valid User ID! ")
				continue
			}

			fmt.Print("Enter the Chosen Fruit: ")

			// Using bufio to read user input as string
			r := bufio.NewReader(os.Stdin)
			f, _ := r.ReadString('\n')

			// Removing whitespace and converting to lowercase letters
			fr = strings.TrimSpace(strings.ToLower(f))

			// Initialize the map if it's nil
			if users[id] == nil {
				users[id] = make(map[string]int)
			}

			// Mark that the user chose this fruit
			users[id][fr] = 1

			//fmt.Printf("Fruit \"%s\" chosen for user [%d].\n", fr, id)

		case 2:
			var t int
			fmt.Printf("Enter the ID of the user to get a recommendation (1 to %d): ", n)
			fmt.Scan(&t)

			// Check if the entered ID is valid
			if t < 1 || t > n {
				fmt.Println("Invalid user ID.")
				continue
			}

			// Get recommendations based on n users
			rec := userRecommendation(users, t)

			// Display recommendations
			fmt.Println("Recommendations:")
			for u, ur := range rec {
				fmt.Printf("To user [%d]:\n", u)
				for _, r := range ur {
					fmt.Println(r)
				}
			}

		case 0:
			fmt.Println("Program ended successfully.")
			return

		default:
			fmt.Print("Please, enter a valid option! ")
		}
	}
}
