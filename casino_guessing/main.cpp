#include <Windows.h>
#include <iostream>
#include <random>
#include <string>

// The range that you should guess the number between
#define GUESSING_RANGE_MIN 0
#define GUESSING_RANGE_MAX 5

#define MINIMUM_WAGER 5

// How long a message should be displayed before the game is reset
#define RESET_TIME 3000 // 3 seconds

struct ctx_t
{
	bool should_quit;
	int current_money;
	int random_number;
};

// Generate a random number using a pseudo-random generator built into the standard library
int generate_random(int min, int max)
{
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int<int> uniform(min, max);
	
	return uniform(eng);
}

// Check if the string contains only digits
bool is_numerical_string(const std::string& input)
{
	auto alpha = std::find_if(input.begin(), input.end(), [](char c) { return isalpha(c); });
	return alpha == input.end();
}

int main()
{
	// Contains information about game state etc
	ctx_t ctx;

	// The last input specified
	std::string input;

	// The default amount of money you start with
	ctx.current_money = 100;
	ctx.should_quit = false;

	while (!ctx.should_quit)
	{
		// Windows only
		system("cls");

		printf("Balance: %d\n", ctx.current_money);

		ctx.random_number = generate_random(GUESSING_RANGE_MIN, GUESSING_RANGE_MAX);

		// Let the user specify a wager amount
		printf("Please enter the amount of money you would like to wager:\n");
		std::cin >> input;

		if (!is_numerical_string(input))
		{
			printf("Please enter a valid wager amount using only digits.\n");
			Sleep(RESET_TIME);
			continue;
		}

		int wager_amount = atoi(input.data());

		if (wager_amount > ctx.current_money)
		{
			printf("You do not have enough money to wager this amount.\n");
			Sleep(RESET_TIME);
			continue;
		}
		else if (wager_amount < MINIMUM_WAGER)
		{
			printf("The minimum amount is %d, please try again with a valid amount.\n", MINIMUM_WAGER);
			Sleep(RESET_TIME);
			continue;
		}

		// Let the user choose a number in the specified range
		printf("Place your guess with a number between %d and %d:\n", GUESSING_RANGE_MIN, GUESSING_RANGE_MAX);
		std::cin >> input;

		if (!is_numerical_string(input))
		{
			printf("Please enter a number using only digits.\n");
			Sleep(RESET_TIME);
			continue;
		}

		// Convert the string to an integer
		int guessed_number = atoi(input.data());

		printf("The random number was %d!\n", ctx.random_number);
		if (guessed_number == ctx.random_number) // You won!
		{
			printf("Congrats! You won the wager! You got double the amount of money back that you wagered.\n");
			ctx.current_money += wager_amount;
		}
		else // You lost...
		{
			printf("It seems that you guessed wrong. Better luck next time!\n");
			ctx.current_money -= wager_amount;
		}

		printf("Continue? [y/n]: ");
		char continue_prompt = 0x00;
		std::cin >> continue_prompt;

		// The player no longer wants to play
		if (tolower(continue_prompt) == 'n')
		{
			ctx.should_quit = true;
			continue;
		}

		Sleep(RESET_TIME);
	}

	return 0;
}