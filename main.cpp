#include <random>
#include <vector>
#include <string>
#include <ncurses.h>

void init_terminal_mode(){
	initscr();
	start_color();
	cbreak();
	noecho();
	curs_set(0);

	init_pair(1, 244, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_BLACK);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);
}

void close_terminal_mode(){
	endwin();
}

std::string generator(const std::string &alphabet){
	std::string str;
	std::random_device rd;
	std::mt19937 gen(rd());

	int count = 0;
	while(count != 11){

	
	std::uniform_int_distribution<int> distrib(0, alphabet.size()-1);

	char c = alphabet[distrib(gen)];

	str += c;
	
	++count;
	}
	return str;
}

bool is_correct_input(const char &user_key, const char &expected_key){
	if(user_key == expected_key) return true;
	else return false;	
}

void display(const std::string &str, const int &current_index, const std::vector<bool> &history){
	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	rows /= 2;
	cols = (cols / 2) - str.size() / 2;

	move(rows, cols);

	for(int i = 0; i < str.size(); ++i){
		if( i < current_index){
			if(history[i] == true){
				attron(COLOR_PAIR(1));
				addch(str[i]);
				attroff(COLOR_PAIR(1));	
			}

			else if(history[i] == false){
				attron(COLOR_PAIR(2));
				addch(str[i]);	
				attroff(COLOR_PAIR(2));
			}

		
		}

		else if( i == current_index){
			attron(COLOR_PAIR(3) | A_BOLD);
			addch(str[i]);
			attroff(COLOR_PAIR(3) | A_BOLD);

		}

		else{
			addch(str[i]);
		}

			
	}

	refresh();
}

int main(){
	std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

	while(true){
	
	std::string target = generator(alphabet);
	std::vector<bool> history;
	size_t current_index = 0;

	init_terminal_mode();


	while(current_index < target.size()-1){	

		display(target, current_index, history);

		int input = getch();

		if(input == 27){
		       	close_terminal_mode();
			return 0;
		}
		if(input > 255) continue;

		char user_key = static_cast<char>(input);
        	char expected_key = target[current_index];

		bool is_correct = is_correct_input(user_key, expected_key);
		history.push_back(is_correct);

		current_index++;
	}

	display(target, current_index, history);
	getch();

	close_terminal_mode();
 }
}
